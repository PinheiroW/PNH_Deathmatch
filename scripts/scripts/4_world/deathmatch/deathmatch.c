/* Copyright (c) 2021-2025 The Crimson Zamboni
 *
 * This Software is licensed for non-commercial use only, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * The Software may not be monetized. This means that you cannot use it on a
 * server with access limited only to paying players, or a server with extra
 * privileges, such as priority queue slots, granted to paying players.
 *
 * The Software is not authorized for posting on Steam, except under the Steam
 * account https://steamcommunity.com/id/DirtySanchez/
 */

class Deathmatch
{
    static private ref TStringArray SelfInflictedReasons = {
        "%1 had a senior moment!",
        "%1 had a brain fart!",
        "%1 had an accident!",
        "%1 bought the farm!",
        "%1 took a dirt nap!",
        "%1 pined for the fjords!",
        "%1 kicked the bucket!",
        "%1 shuffled off this mortal coil!",
        "%1 is kaput!",
        "%1 took a permanent vacation!",
        "%1 paid the piper!",
        "%1 croaked!",
        "%1 flatlined!",
        "%1 is no more!",
        "%1 met an untimely end!",
        "%1 forgot to pay the brain bill!",
        "%1 was foolish.",
        "%1 stinks!",
        "%1 failed out of Noob School.",
        "%1 sucks!",
        "%1 is a potato!",
        "%1 ate too much lead paint!"
    };

    static private ref TStringArray SuicideReasons = {
        "%1 was a coward!",
        "Everyone hates %1!",
        "%1 was a pathetic loser!",
        "%1 couldn't take it anymore"
    };

    static private int COUNTDOWN_DURATION = 10;
    static private int COUNTDOWN_INTERVAL = 5;

    private autoptr DeathmatchSettings m_settings = new DeathmatchSettings();
    private autoptr DMArenaRotation m_arenaRotation;
    private autoptr DMWebhook m_webhook;
    private autoptr DMWeapons m_weapons;
    private autoptr DMScoreBoard m_scoreboard = new DMScoreBoard();
    private autoptr TStringStringMap m_Identities = new TStringStringMap();
    private autoptr TStringIntMap m_idleStart = new TStringIntMap();
    private autoptr DMClothes m_clothes;
    private autoptr DMChatCommands m_chatCommands;
    private autoptr DMAutoRespawn m_autoRespawn;
    private int m_num_rounds = 0;
    private int m_round_end;
    private bool m_round_ending = false;
    private DMEventSettings m_eventSettings;
    private DMEvent m_event;
    private MissionBaseWorld m_mission;
    private autoptr Timer m_tickTimer = new Timer();
    private autoptr Timer m_reminderTimer = new Timer();
    private autoptr Timer m_roundTimer = new Timer();
    private autoptr Timer m_eventAnnounceTimer = new Timer();
    private autoptr Timer m_shutdownTimer = new Timer();
    private autoptr array<Object> m_clutter = {};
    private autoptr DMVote m_vote;

    void Deathmatch()
    {
        string serverVersion;
        g_Game.GetVersion(serverVersion);

        PrintFormat(
                "Loaded CrimsonZamboniDeathmatch %1 on DayZ Server version %2",
                CRIMSON_ZAMBONI_DEATHMATCH_VERSION, serverVersion);

        DMVersionChecker.CheckVersion();

        Math.Randomize(g_Game.GetTime());
        for (int i = 0; i < 10; i++)
        {
            Math.RandomInt(0, 2);
        }

        m_mission = MissionBaseWorld.Cast(g_Game.GetMission());

        m_settings.Load();
        m_arenaRotation = new DMArenaRotation(m_settings);
        if (m_arenaRotation.HasError()) return;
        m_webhook = DMWebhook.Create(m_settings.killFeedWebhook);
        m_weapons = new DMWeapons(m_settings);
        m_clothes = new DMClothes(m_settings);
        m_chatCommands = new DMChatCommands(m_settings);
        m_autoRespawn = new DMAutoRespawn(m_settings);

#ifdef DEBUG_DEATHMATCH_ARENA
        EmoteManager.SetArenaRotation(m_arenaRotation);
#endif

        m_chatCommands.AddCommand(
            "autorespawn", this, "OnAutoRespawnCommand", ":: Toggle auto respawn");
        m_chatCommands.AddCommand("players", this, "OnPlayersCommand", ":: Show player count");
        m_chatCommands.AddCommand("arenas", this, "OnArenasCommand", ":: List all arena names");
        m_chatCommands.AddCommand("events", this, "OnEventsCommand", ":: List all event names");
        m_chatCommands.AddCommand(
                "timeleft", this, "OnTimeleftCommand", ":: Show time remaining in current round");
        m_chatCommands.AddCommand(
                "uptime", this, "OnUptimeCommand", ":: Show how long server has been running");
        m_chatCommands.AddCommand("score", this, "OnScoreCommand", ":: Show your score");
        m_chatCommands.AddCommand("version", this, "OnVersionCommand", ":: Show version");
        m_chatCommands.AddCommand(
            "arenavote", this, "OnArenavoteCommand",
            "<arena name> :: Start a vote to set the next arena");
        m_chatCommands.AddCommand(
            "eventvote", this, "OnEventvoteCommand",
            "<event name> :: Start a vote to set the next event");
        m_chatCommands.AddCommand(
            "endvote", this, "OnEndvoteCommand", ":: Start a vote to end the round early");
        m_chatCommands.AddCommand(
            "vote", this, "OnVoteCommand", "<option> :: Vote for or against the current proposal");
        m_chatCommands.AddAdminCommand(
                "nextarena", this, "OnNextarenaCommand", "<arena name> :: Set next round's arena");
        m_chatCommands.AddAdminCommand(
                "endround", this, "OnEndroundCommand", ":: Force round to end");
        m_chatCommands.AddAdminCommand(
                "nextevent", this, "OnNexteventCommand", "<event name> :: Set next round's event");
        m_chatCommands.AddAdminCommand(
                "shutdown", this, "OnShutdownCommand", ":: Force server to shut down");

        // Force the events.json file to get loaded or created
        DMEventRegistry.Instance();

        int tickInterval = 10;
        if (m_settings._debugArena)
        {
            tickInterval = 1;
        }

        m_tickTimer.Run(tickInterval, this, "OnTick", null, true);

        m_reminderTimer.Run(300, this, "Reminder", null, true);

        this.RollForEvent();
        this.StartRound();
    }

    private void OnTick()
    {
        TStringArray deadPlayerIds = new TStringArray();

        array<PlayerIdentity> ids = new array<PlayerIdentity>();
        g_Game.GetPlayerIndentities(ids);
        foreach (PlayerIdentity id : ids)
        {
            deadPlayerIds.Insert(id.GetId());
        }

        array<Man> players = new array<Man>();
        g_Game.GetPlayers(players);
        foreach (Man player : players)
        {
            if (player.IsAlive())
            {
                m_arenaRotation.CheckPlayerPosition(player);

                PlayerIdentity identity = player.GetIdentity();
                if (identity)
                {
                    deadPlayerIds.RemoveItem(identity.GetId());
                }
            }
        }

        if (m_settings.maxIdleSeconds > 0)
        {
            int current = g_Game.GetTickTime();

            foreach (string deadId : deadPlayerIds)
            {
                int last = 0;

                if (m_idleStart.Find(deadId, last))
                {
                    if (current > (last + m_settings.maxIdleSeconds))
                    {
                        PrintFormat("Disconnecting idle player %1", deadId);
                        this.KickPlayer(deadId);
                    }
                }
                else
                {
                    m_idleStart.Set(deadId, current);
                }
            }
        }

        DMInfected.OnTick(
                m_arenaRotation.CurrentArena(), m_Identities.Count(), m_settings, m_event);
    }

    private void KickPlayer(string id)
    {
        array<PlayerIdentity> identities = new array<PlayerIdentity>();
        g_Game.GetPlayerIndentities(identities);
        foreach (PlayerIdentity identity : identities)
        {
            if (id == identity.GetId())
            {
                g_Game.DisconnectPlayer(identity);
                this.PlayerDisconnected(identity, id);
            }
        }
    }

    private void OnNextarenaCommand(DMPlayer source, string args)
    {
        if (m_arenaRotation.SetNextArena(args))
        {
            DMChat.Announce(
                    string.Format("%1 forced next arena to be %2", source.GetName(), args));
        }
        else
        {
            DMChat.MessagePlayer(source, string.Format("Invalid arena: %1", args));
        }
    }

    private void OnEndroundCommand(DMPlayer source, string args)
    {
        DMChat.Announce(string.Format("%1 forced the round to end", source.GetName()));

        this.ForceEndRound();
    }

    private void OnNexteventCommand(DMPlayer source, string args)
    {
        if (m_settings.GetEventSettings(args))
        {
            DMEventRegistry.Instance().SetNextEventName(args);

            DMChat.Announce(
                    string.Format("%1 forced next event to be %2", source.GetName(), args));
        }
        else
        {
            DMChat.MessagePlayer(source, string.Format("Invalid event name: %1", args));
        }
    }

    private void OnShutdownCommand(DMPlayer source, string args)
    {
        DMChat.Announce(string.Format("%1 requested a shutdown", source.GetName()));

        m_shutdownTimer.Run(0.2, this, "ShutDown");
    }

    private void OnAutoRespawnCommand(DMPlayer source, string args)
    {
        m_autoRespawn.ToggleForID(source.GetId());

        DMChat.MessagePlayer(
            source,
            string.Format(
                "Auto-respawn is now %1",
                DMUtils.BoolToOnOff(m_autoRespawn.IsEnabledForID(source.GetId()))));
    }

    private void OnPlayersCommand(DMPlayer source, string args)
    {
        DMChat.MessagePlayer(
                source, string.Format("Current player count: %1", DMUtils.GetPlayerCount()));
    }

    private void OnArenasCommand(DMPlayer source, string args)
    {
        TStringArray arenas = DMArenaRegistry.Instance().GetAllArenaNames();
        arenas.Sort();

        DMChat.MessagePlayer(source, "Arena names:");

        float delay = 0;
        foreach (string name : arenas)
        {
            delay += 1;
            DMChat.MessagePlayerDelayed(source, string.Format(" %1", name), delay);
        }
    }

    private void OnEventsCommand(DMPlayer source, string args)
    {
        TStringArray events = m_settings.GetEventNames();
        events.Sort();

        DMChat.MessagePlayer(source, "Event names:");

        float delay = 0;
        foreach (string name : events)
        {
            delay += 1;
            DMChat.MessagePlayerDelayed(source, string.Format(" %1", name), delay);
        }
    }

    private void OnTimeleftCommand(DMPlayer source, string args)
    {
        DMChat.MessagePlayer(
                source, string.Format("Time remaining in round: %1", this.GetTimeRemaining()));
    }

    private void OnUptimeCommand(DMPlayer source, string args)
    {
        int seconds = g_Game.GetTickTime();
        int minutes = seconds / 60;
        int hours = minutes / 60;

        seconds = seconds % 60;
        minutes = minutes % 60;

        string uptime;
        string sep;
        if (hours > 0)
        {
            if (hours == 1) sep = ", "; else sep = "s, ";
            uptime += string.Format("%1 hour%2", hours, sep);
        }
        if (minutes > 0)
        {
            if (minutes == 1) sep = ", "; else sep = "s, ";
            uptime += string.Format("%1 minute%2", minutes, sep);
        }
        if (seconds == 1) sep = ""; else sep = "s";
        uptime += string.Format("%1 second%2", seconds, sep);

        DMChat.MessagePlayer(source, string.Format("Uptime: %1", uptime));
    }

    private void OnScoreCommand(DMPlayer source, string args)
    {
        DMScore score = m_scoreboard.GetPlayerScore(source.GetIdentity());

        DMChat.MessagePlayer(
                source, string.Format("Your K:D is %1:%2", score.m_kills, score.m_deaths));
    }

    private void OnVersionCommand(DMPlayer source, string args)
    {
        DMChat.MessagePlayer(
            source,
            string.Format("CrimsonZamboniDeathmatch %1", CRIMSON_ZAMBONI_DEATHMATCH_VERSION));
    }

    private void AnnounceVote(string player_name, string details)
    {
        this.NotifyAllPlayers(string.Format("%1 has started a vote", player_name), details, "",
            m_settings.voteMinutes * 15.0);

        DMChat.Announce(
            string.Format("To vote FOR, type: %1vote 1", m_settings.commandCharacter));
        DMChat.Announce(
            string.Format("To vote AGAINST, type: %1vote 2", m_settings.commandCharacter));
    }

    private void OnArenavoteCommand(DMPlayer source, string args)
    {
        if (!this.CanStartVote(source))
        {
            return;
        }

        if (m_arenaRotation.SetNextArena(args))
        {
            m_vote = DMArenaVote.Start(this, m_settings.voteMinutes, m_arenaRotation, args);
            m_vote.TallyFor(source);

            this.AnnounceVote(
                source.GetName(), string.Format("to locate the next round at %1", args));
        }
        else
        {
            DMChat.MessagePlayer(source, string.Format("Invalid arena: %1", args));
        }
    }

    private void OnEventvoteCommand(DMPlayer source, string args)
    {
        if (!this.CanStartVote(source))
        {
            return;
        }

        if (m_settings.GetEventSettings(args))
        {
            m_vote = DMEventVote.Start(
                this, m_settings.voteMinutes, DMEventRegistry.Instance(), args);
            m_vote.TallyFor(source);

            this.AnnounceVote(
                source.GetName(), string.Format("to make the next round have event %1", args));
        }
        else
        {
            DMChat.MessagePlayer(source, string.Format("Invalid event name: %1", args));
        }
    }

    private void OnEndvoteCommand(DMPlayer source, string args)
    {
        if (!this.CanStartVote(source))
        {
            return;
        }

        m_vote = DMEndVote.Start(this, m_settings.voteMinutes);
        m_vote.TallyFor(source);

        this.AnnounceVote(source.GetName(), "to end the round early");
    }

    private void OnVoteCommand(DMPlayer source, string args)
    {
        if (!m_vote)
        {
            DMChat.MessagePlayer(source, "There is no vote currently in progress");
            return;
        }

        if (m_vote.HasVoted(source))
        {
            DMChat.MessagePlayer(source, "You have already voted");
            return;
        }

        if (args == "1")
        {
            DMChat.Announce(string.Format("%1 voted FOR", source.GetName()));
            m_vote.TallyFor(source);
        }
        else if (args == "2")
        {
            DMChat.Announce(string.Format("%1 voted AGAINST", source.GetName()));
            m_vote.TallyAgainst(source);
        }
        else
        {
            DMChat.MessagePlayer(source, "Invalid vote option");
            return;
        }
    }

    private void OnChat(int channel, string playerName, string message)
    {
        if (channel != 0) return;

        string args;
        string command = m_chatCommands.ParseCommand(message, args);
        if (command == "") return;

        DMPlayer source = DMPlayer.GetByPlayerName(m_settings, playerName);
        if (!source) return;

        m_chatCommands.OnCommand(source, command, args);
    }

    void OnEvent(EventType eventTypeId, Param params)
    {
        switch (eventTypeId)
        {
            case ChatMessageEventTypeID:
                ChatMessageEventParams chatParams = ChatMessageEventParams.Cast(params);
                this.OnChat(chatParams.param1, chatParams.param2, chatParams.param3);
                break;
        }
    }

    private bool CanStartVote(DMPlayer player)
    {
        if (m_vote)
        {
            DMChat.MessagePlayer(player, "A vote is already in progress");
            return false;
        }

        if (DMUtils.GetPlayerCount() < m_settings.voteMinimumPlayers)
        {
            DMChat.MessagePlayer(player, "Not enough players to start a vote");
            return false;
        }

        return true;
    }

    private void Reminder()
    {
        DMChat.Announce("Deathmatch by Crimson Zamboni -- crimsonzamboni.com");
    }

    private void RemoveEnclosure()
    {
        if (m_clutter)
        {
            foreach (Object obj : m_clutter)
            {
                g_Game.ObjectDelete(obj);
            }

            m_clutter.Clear();
        }
    }

    private void CleanupObjects()
    {
        Print("Cleaning up objects");

        array<Object> objects = new array<Object>();
        m_arenaRotation.CurrentArena().FindObjectsToClean(objects);

        foreach (Object obj : objects)
        {
            ItemBase itemBase = ItemBase.Cast(obj);
            if (itemBase != null && itemBase.GetHierarchyParent() == null)
            {
                Print("Cleaning up object " + itemBase);
                itemBase.Delete();
            }

            DayZCreature creature = DayZCreature.Cast(obj);
            if (creature != null)
            {
                Print("Cleaning up " + creature);
                creature.Delete();
            }

            PlayerBase player = PlayerBase.Cast(obj);
            if (player != null)
            {
                if (player.GetIdentity() == null)
                {
                    Print("Cleaning up corpse " + player);
                    player.Delete();
                }
                else
                {
                    Print("Prepping corpse " + player);
                    player.DMPrepForCleanup();
                }
            }
        }

        this.RemoveEnclosure();

        Print("Done cleaning up objects");
    }

    private void NotifyPlayer(
        PlayerIdentity identity, string message, string details = "", string icon = "",
        float show_time = 5.0)
    {
        string now = DMGetTimestamp();
        string name = "ALL";
        string id = "NULL";
        if (identity)
        {
            name = identity.GetName();
            id = identity.GetId();
        }

        NotificationSystem.SendNotificationToPlayerIdentityExtended(
            identity, show_time, message, details, icon);

        PrintFormat("%1 | NOTIFY | %2 (%3) | %4 | %5", now, name, id, message, details);
    }

    private void NotifyAllPlayers(
        string message, string details = "", string icon = "", float show_time = 5)
    {
        this.NotifyPlayer(null, message, details, icon, show_time);
    }

    private string ReportPlayerStats()
    {
        array<PlayerIdentity> identities = new array<PlayerIdentity>();
        g_Game.GetPlayerIndentities(identities);
        string bestPlayer;
        int bestKills = -1;
        int bestDeaths = -1;

        for (int i = 0; i < identities.Count(); i++)
        {
            PlayerIdentity identity = identities.Get(i);
            DMScore score = m_scoreboard.GetPlayerScore(identity);
            this.NotifyPlayer(
                    identity, string.Format("Your K:D was %1:%2", score.m_kills, score.m_deaths),
                    "", "set:ccgui_enforce image:MapUserMarker");

            if (score.m_kills > bestKills || ((score.m_kills == bestKills) && (score.m_deaths < bestDeaths)))
            {
                bestPlayer = identity.GetName();
                bestKills = score.m_kills;
                bestDeaths = score.m_deaths;
            }
        }

        if (bestPlayer != "")
        {
            return string.Format("The top K:D was %1:%2 by %3", bestKills, bestDeaths, bestPlayer);
        }

        return "";
    }

    private void ReportLeaderboard()
    {
        array<PlayerIdentity> identities = new array<PlayerIdentity>();
        g_Game.GetPlayerIndentities(identities);

        if (identities.Count() < 1)
        {
            Print("No users -- skipping leaderboard");
            return;
        }

        TDMScoreArray leaderboard = m_scoreboard.GetLeaderboard(identities);
        m_webhook.PostLeaderboard(leaderboard);
        SaveLeaderboard(leaderboard);
    }

    private void SaveLeaderboard(TDMScoreArray scores)
    {
        if (m_settings.saveLeaderboards)
        {
            DMRecordLeaderboard.Save(m_arenaRotation.CurrentArena(), m_event, scores);
        }
    }

    private void KillAllPlayers()
    {
        array<Man> men = new array<Man>();
        g_Game.GetPlayers(men);
        foreach (Man man : men)
        {
            Print("Checking man " + man);
            PlayerBase playerBase = PlayerBase.Cast(man);
            if (playerBase != null)
            {
                Print("Killing player " + playerBase);
                playerBase.SetHealth(0.0);
            }
        }
    }

    private void RollForEvent()
    {
        DMEventRegistry registry = DMEventRegistry.Instance();

        if (registry.HaveNextEvent())
        {
            m_eventSettings = m_settings.GetEventSettings(registry.GetNextEventName());
            registry.ResetNextEvent();
        }
        else
        {
            m_eventSettings = m_settings.RollForRandomEvent();
        }

        if (m_eventSettings)
        {
            m_event = registry.GetEvent(m_eventSettings.name);
            if (!m_event)
            {
                Error(
                    string.Format("No event definition for event name: %1", m_eventSettings.name));
            }
        }
        else
        {
            m_event = null;
        }
    }

    private bool ServerShouldShutdown()
    {
        if (m_settings.maxRounds > 0 && m_num_rounds >= m_settings.maxRounds)
        {
            return true;
        }

        int uptimeHours = m_round_end / 3600;
        if (m_settings.maxUptimeHours > 0 && uptimeHours >= m_settings.maxUptimeHours)
        {
            return true;
        }

        return false;
    }

    private void EndRound()
    {
        Print("Ending round");

        this.m_round_ending = true;

        if (m_vote)
        {
            m_vote.End();
        }

        this.ReportLeaderboard();

        DMArena arena = m_arenaRotation.CurrentArena();

        if (m_event)
        {
            m_event.OnRoundEnd(arena);
        }

        DMBlamedPlayer.Clear();

        DMInfected.OnRoundEnd();

        string bestInfo = this.ReportPlayerStats();

        this.NotifyAllPlayers(
            "The round has ended!", bestInfo, "set:ccgui_enforce image:MapUserMarker");

        this.KillAllPlayers();

        m_mission.OnRoundEnd(arena);

        if (this.ServerShouldShutdown())
        {
            Print("Max rounds reached -- requesting restart");
            m_shutdownTimer.Run(0.2, this, "ShutDown");
        }
        else
        {
            this.RollForEvent();
            m_roundTimer = new Timer();
            m_roundTimer.Run(0.2, this, "CleanupObjectsAndStartRound");
        }

        Print("Done ending round");
    }

    private void RestartRound()
    {
        this.EndRound();
    }

    private void EndRoundCountdown(int duration)
    {
        if (m_vote)
        {
            m_vote.OnRoundEnding();
        }

        if (duration <= 0)
        {
            this.RestartRound();
        }
        else
        {
            string detail;

            if (this.ServerShouldShutdown())
            {
                detail = "Server will restart";
            }

            this.NotifyAllPlayers(
                string.Format("Round ends in %1 seconds", duration), detail,
                "set:ccgui_enforce image:MapUserMarker");

            m_arenaRotation.CurrentArena().WarningSounds();

            m_roundTimer = new Timer();
            m_roundTimer.Run(
                    COUNTDOWN_INTERVAL, this, "EndRoundCountdown",
                    new Param1<int>(duration - COUNTDOWN_INTERVAL));
        }
    }

    private void PrepareArena(DMArena arena)
    {
        Print("Preparing arena");

        array<Object> objects = new array<Object>();
        arena.FindObjectsToClean(objects);

        foreach (Object obj : objects)
        {
            Building building = Building.Cast(obj);
            if (building != null)
            {
                int doors = g_Game.ConfigGetChildrenCount(
                        string.Format("CfgVehicles %1 Doors", building.GetType()));
                for (int door = 0; door < doors; door++)
                {
                    building.CloseDoor(door);
                }
            }

            PASBroadcaster pas = PASBroadcaster.Cast(obj);
            if (pas)
            {
                pas.GetInventory().CreateAttachment("CarBattery");
            }
        }

        if (m_event)
        {
            m_event.SpawnChristmas(arena, m_clutter, m_settings);
        }
        else if (m_settings.christmas)
        {
            DMChristmas.Spawn(arena, m_clutter);
        }

        arena.Prepare(m_clutter);
    }

    private void StartRound()
    {
        m_num_rounds++;

        PrintFormat("Starting round %1", m_num_rounds);

        if (m_eventSettings)
        {
            m_arenaRotation.PickNextArena(m_Identities.Count(), m_eventSettings.arenas);
        }
        else
        {
            m_arenaRotation.PickNextArena(m_Identities.Count());
        }

        m_round_ending = false;

        m_scoreboard.Reset();

        DMArena arena = m_arenaRotation.CurrentArena();

        this.PrepareArena(arena);

        if (!m_settings.disableWalls)
        {
            arena.Enclose(m_clutter);
        }

        int roundMinutes;
        if (m_eventSettings)
        {
            roundMinutes = m_eventSettings.roundMinutes;
        }
        else
        {
            roundMinutes = m_settings.roundMinutes;
        }

        m_round_end = g_Game.GetTickTime() + (roundMinutes * 60);

        float delay = (roundMinutes * 60) - COUNTDOWN_DURATION;
        m_roundTimer = new Timer();
        m_roundTimer.Run(delay, this, "EndRoundCountdown", new Param1<int>(COUNTDOWN_DURATION));

        DMCrates.SpawnCrates(m_settings, arena, m_event);

        g_Game.GetWorld().ProcessMarkedObjectsForPathgraphUpdate();

        DMInfected.Spawn(arena, m_Identities.Count(), m_settings, m_event);

        if (m_event && m_event.HasGreeting())
        {
            m_eventAnnounceTimer.Run(10, this, "NotifyAllPlayers", m_event.GetGreeting());
        }

        if (m_event)
        {
            m_event.OnRoundStart(arena);
        }

        m_mission.OnRoundStart(arena);

        Print("Done starting round");
    }

    private void CleanupObjectsAndStartRound()
    {
        this.CleanupObjects();

        this.StartRound();
    }

    private void EquipPlayerForSurvival(PlayerBase player)
    {
        HumanInventory inventory = player.GetHumanInventory();

        EntityAI bandage = inventory.CreateInInventory("BandageDressing");
        player.SetQuickBarEntityShortcut(bandage, 3);

        inventory.CreateInInventory("SalineBagIV");
        inventory.CreateInInventory("Morphine");
        inventory.CreateInInventory("Heatpack");

        float roll = Math.RandomFloat01();
        if (roll < 0.01)
        {
            Print("EASTER EGG!!!");
            EasterEgg egg = EasterEgg.Cast(inventory.CreateInInventory("EasterEgg"));
            egg.DMPrepare();
            player.SetQuickBarEntityShortcut(egg, 4);
        }
    }

    private void StartFedAndWatered(PlayerBase player)
    {
        player.GetStatWater().Set(player.GetStatWater().GetMax());
        player.GetStatEnergy().Set(player.GetStatEnergy().GetMax());
    }

    private void PlayerNightVisionReminder(PlayerBase player, EntityAI nvgs)
    {
        if (m_arenaRotation.CurrentArena().IsDark())
        {
            this.NotifyPlayer(
                player.GetIdentity(), "Use your Night Vision Goggles!", "",
                "set:dayz_gui image:icon_flag");
        }
    }

    private bool IsNight()
    {
        return !m_settings.ignoreNight && g_Game.GetWorld().IsNight();
    }

    void StartingEquipSetup(PlayerBase player)
    {
        if (m_arenaRotation.CurrentArena().IsDark() || this.IsNight())
        {
            HumanInventory inventory = player.GetHumanInventory();
            EntityAI headstrap = inventory.CreateAttachment("NVGHeadstrap");
            EntityAI nvgs = headstrap.GetInventory().CreateAttachment("NVGoggles");
            nvgs.GetInventory().CreateAttachment("Battery9V");
            this.PlayerNightVisionReminder(player, nvgs);
        }

        EntityAI sheath = m_clothes.EquipPlayerClothes(player, m_event);
        this.EquipPlayerForSurvival(player);
        m_weapons.EquipPlayerWeapons(player, sheath, m_event);
        this.StartFedAndWatered(player);

        if (m_event)
        {
            m_event.EquipPlayer(player, player.GetHumanInventory());
        }
    }

    private void ReportTimeLeftInRound(notnull PlayerIdentity identity)
    {
        string text = this.GetTimeRemaining("");
        if (text == "")
        {
            // We're about to show the 10-second countdown, so don't spam the user with extra
            // notifications.
            return;
        }

        if (m_event)
        {
            this.NotifyPlayer(
                    identity, string.Format("%1 ends in %2", m_event.GetDisplayName(), text), "",
                    "set:ccgui_enforce image:MapUserMarker");
        }
        else
        {
            this.NotifyPlayer(
                identity, string.Format("Round ends in %1", text), "",
                "set:ccgui_enforce image:MapUserMarker");
        }
    }

    private void PlayerTrackingMessage(string message, int playerCount, string icon)
    {
        string details = string.Format("player count: %1", playerCount);

        switch (m_settings.killFeed)
        {
            case 1:
                this.NotifyAllPlayers(message, details, icon);
                break;
            case 2:
                DMChat.Announce(string.Format("%1 (%2)", message, details));
                break;
        }

        if (m_settings.killFeedWebhook.playerTracking) {
            m_webhook.PostMessage(string.Format("%1 (%2)", message, details));
        }
    }

    vector GetRandomPlayerSpawnPosition()
    {
        return m_arenaRotation.CurrentArena().GetRandomPlayerSpawnPosition();
    }

    void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        m_arenaRotation.CurrentArena().SetDirectionToCenter(player);

        string uid = identity.GetId();

        m_idleStart.Remove(uid);

        // Unfortunately, InvokeOnConnect gets called when players respawn, so we have to keep
        // track of connects and disconnects in order to know if this is being called for a
        // player's initial spawn.
        if (!m_Identities.Contains(uid))
        {
            string name = identity.GetName();

            m_Identities.Set(uid, name);

            m_autoRespawn.InitializeID(uid);
            m_scoreboard.ResetPlayer(identity);

            this.PlayerTrackingMessage(
                    string.Format("%1 entered the arena", name),
                    DMUtils.GetPlayerCount(), "set:dayz_gui image:notification_friend");

            this.ReportTimeLeftInRound(identity);

            DMChat.MessagePlayerDelayed(
                player,
                string.Format(
                    "Type %1help in chat for a list of available commands",
                    m_settings.commandCharacter),
                30);

            if (m_settings.IsAdmin(identity))
            {
                string latestVersion;

                if (DMVersionChecker.UpgradeAvailable(latestVersion))
                {
                    NotificationSystem.SendNotificationToPlayerIdentityExtended(
                        identity, 30, "Upgrade available!",
                        string.Format(
                            "Please upgrade your server to CrimsonZamboniDeathmatch version %1",
                            latestVersion));
                }
            }
        }
    }

    private int GetPlayerCountExcluding(string id)
    {
        int count = 0;

        array<PlayerIdentity> ids = new array<PlayerIdentity>();
        g_Game.GetPlayerIndentities(ids);

        foreach (PlayerIdentity player : ids)
        {
            if (player.GetId() != id)
            {
                count++;
            }
        }

        return count;
    }

    void PlayerDisconnected(PlayerIdentity identity, string uid)
    {
        PrintFormat("PlayerDisconnected(%1, %2) -- %3", identity, uid, m_Identities.Count());

        m_idleStart.Remove(uid);

        string name;
        if (m_Identities.Find(uid, name))
        {
            m_Identities.Remove(uid);

            this.PlayerTrackingMessage(
                    string.Format("%1 exited the arena", name),
                    this.GetPlayerCountExcluding(uid), "set:dayz_gui image:open");
        }
        else
        {
            PrintFormat("Unable to find %1 in saved identities (%2)", uid, m_Identities.Count());
        }
    }

    void HandleBody(PlayerBase player)
    {
        player.DropAllItems();
        // Kill character so that players start fresh every time they connect
        player.SetHealth(0.0);
    }

    private void KillFeedMessage(string title, string detail, string icon)
    {
        switch (m_settings.killFeed)
        {
            case 1:
                this.NotifyAllPlayers(title, detail, icon);
                break;
            case 2:
                DMChat.Announce(string.Format("%1 %2", title, detail));
                break;
        }

        m_webhook.PostMessage(string.Format("%1 %2", title, detail));
    }

    private DMAttackRecord GetAttackRecordForPlayer(Man player)
    {
        if (player.m_KillerData)
        {
            return DMAttackRecord.FromKillerData(player, player.m_KillerData);
        }
        else
        {
            PlayerBase playerBase = PlayerBase.Cast(player);

            if (playerBase && playerBase.m_attackRecordForUncon)
            {
                return playerBase.m_attackRecordForUncon;
            }
        }

        return null;
    }

    private PlayerIdentity IdentifyKiller(KillerData data)
    {
        Man killer = Man.Cast(data.m_Killer);
        if (killer)
        {
            return killer.GetIdentity();
        }

        return null;
    }

    void OnPlayerDeath(Man player)
    {
        if (m_round_ending)
        {
            return;
        }

        PlayerIdentity victim = player.GetIdentity();
        if (victim)
        {
            m_scoreboard.AddDeath(victim);

            string name = victim.GetName();

            string killTitle;
            string killDetails;
            string icon = "set:dayz_gui image:iconSkull";

            DMAttackRecord record = this.GetAttackRecordForPlayer(player);
            if (record)
            {
                if (record.WasByInfected())
                {
                    killTitle = name + " was kissed by a zombie";
                    killDetails = "and liked it!";
                }
                else if (record.WasSelfInflicted())
                {
                    killTitle = string.Format(SelfInflictedReasons.GetRandomElement(), name);
                    if (record.HaveNonFistsWeapon())
                    {
                        killDetails = string.Format(
                            "(killed self using %1)", record.GetWeaponName());
                    }
                }
                else if (record.HaveAttacker())
                {
                    icon = "set:dayz_gui image:get_in_gunner";

                    PlayerIdentity murderer = record.GetAttackerIdentity(m_settings);
                    if (murderer)
                    {
                        m_scoreboard.AddKill(murderer);

                        string killer = murderer.GetName();
                        killTitle = killer + " killed " + name;

                        if (record.HaveWeapon())
                        {
                            killDetails = "using " + record.GetWeaponName();
                        }

                        if (record.HaveDistance())
                        {
                            killDetails = string.Format(
                                    "%1 from %2m", killDetails, record.GetDistance());

                            if (record.GetDistance() == 69)
                            {
                                killDetails += " (nice)";
                            }
                        }
                    }
                }

                if (killTitle == "")
                {
                    killTitle = name + " was killed";

                    if (record.HaveWeapon())
                    {
                        killTitle += string.Format(" by %1", record.GetWeaponName());
                    }
                }
            }
            else
            {
                PrintFormat("Player %1 was killed without any killer data", player);

                PlayerBase pb = PlayerBase.Cast(player);
                if (pb && pb.CommitedSuicide())
                {
                    killTitle = string.Format(SuicideReasons.GetRandomElement(), name);
                }
                else
                {
                    killTitle = string.Format("%1 has died", name);
                }
            }

            this.KillFeedMessage(killTitle, killDetails, icon);
        }
        else
        {
            Print("Someone (" + player + ") died but I don't know who that is");
        }
    }

    void ResetVote()
    {
        m_vote = null;
    }

    void ForceEndRound()
    {
        m_roundTimer.Stop();
        g_Game.GameScript.CallFunctionParams(
            this, "EndRoundCountdown", null, new Param1<int>(COUNTDOWN_DURATION));
    }

    private void ShutDown()
    {
        g_Game.RequestExit(0);
    }

    private int GetSecondsRemaining()
    {
        return m_round_end - g_Game.GetTickTime();
    }

    private string GetTimeRemaining(string underThirtySeconds = "less than 30 seconds")
    {
        int remaining = GetSecondsRemaining();
        string text;

        if (remaining > 120)
        {
            text = (remaining / 60).ToString() + " minutes";
        }
        else if (remaining > 30)
        {
            text = remaining.ToString() + " seconds";
        }
        else
        {
            text = underThirtySeconds;
        }

        return text;
    }
};

// vim:ft=enforce
