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

class DMSavedArenaOrder : Managed
{
    ref TStringArray rotation = new TStringArray();
    int position = 0;
};

class DMArenaRotation
{
    private DMArenaRegistry m_registry;

    private bool m_error = false;

    private bool m_debugArena;

    private ref TStringArray m_arenaRotation;
    private ref TStringArray m_excludeArenas;
    private int m_arenaRotationPosition;

    private string m_arenaName;
    private DMArena m_arena;

    private string m_nextArena;

    void DMArenaRotation(DeathmatchSettings settings)
    {
        m_registry = DMArenaRegistry.Instance();

        m_debugArena = settings._debugArena;

        m_arenaRotation = settings.arenaRotation;
        m_excludeArenas = settings.excludeArenas;
        m_arenaRotationPosition = 0;

        m_registry.Freeze();

        if (m_registry.IsEmpty())
        {
            Error(string.Format("Unsupported map (%1) detected!", g_Game.GetWorldName()));
            return;
        }

        bool validationError = false;

        this.ValidateArenas(m_arenaRotation, "arenaRotation");

        foreach (DMEventSettings type : settings.events.types)
        {
            this.ValidateArenas(type.arenas, string.Format("events.types.%1.arenas", type.name));
        }

        if (this.HasError()) return;

        this.RestoreSavedOrder();
    }

    DMArena CurrentArena()
    {
        return m_arena;
    }

    bool HasError()
    {
        return m_error;
    }

    private void Error(string message)
    {
        m_error = true;
        Error2("DMArenaRotation error", message);
        g_Game.RequestExit(1);
    }

    private bool ValidateArenas(TStringArray arenas, string source)
    {
        foreach (string arena : arenas)
        {
            if (m_registry.GetArena(arena) == null)
            {
                Error(string.Format("Invalid arena name (%1) in %2", arena, source));
                return false;
            }
        }
        return true;
    }

    private void RestoreSavedOrder()
    {
        if (m_arenaRotation.Count() < 1) return;

        DMSavedArenaOrder savedOrder = new DMSavedArenaOrder();

        DMJSONLoader<DMSavedArenaOrder>.LoadFile("$saves:deathmatch-arena-order.json", savedOrder);

        if (savedOrder.rotation.Count() != m_arenaRotation.Count()) return;

        for (int i = 0; i < m_arenaRotation.Count(); i++)
        {
            if (savedOrder.rotation.Get(i) != m_arenaRotation.Get(i)) return;
        }

        m_arenaRotationPosition = savedOrder.position;

        Print("Restored saved arena order");
    }

    private void SaveOrder()
    {
        DMSavedArenaOrder savedOrder = new DMSavedArenaOrder();
        savedOrder.rotation.Copy(m_arenaRotation);
        savedOrder.position = m_arenaRotationPosition;

        JsonFileLoader<DMSavedArenaOrder>.JsonSaveFile(
                "$saves:deathmatch-arena-order.json", savedOrder);
    }

    bool SetNextArena(string arenaName)
    {
        if (m_registry.GetArena(arenaName) != null)
        {
            m_nextArena = arenaName;
            return true;
        }

        return false;
    }

    private TStringArray GetArenaNamesForRandomRotation(int playerCount)
    {
        TStringArray result = new TStringArray();
        TStringArray supported = m_registry.GetSupportedArenaNames(playerCount);

        foreach (string arena : supported)
        {
            if (m_excludeArenas.Find(arena) < 0)
            {
                result.Insert(arena);
            }
        }

        return result;
    }

    void PickNextArena(int playerCount, TStringArray arenas = null)
    {
        if (m_nextArena != "")
        {
            m_arenaName = m_nextArena;
            m_nextArena = "";
        }
        else if (arenas && arenas.Count() > 0)
        {
            m_arenaName = this.NextArenaFromNames(playerCount, arenas);
        }
        else if (m_arenaRotation.Count() > 0)
        {
            m_arenaName = m_arenaRotation.Get(m_arenaRotationPosition);
            m_arenaRotationPosition = (m_arenaRotationPosition + 1) % m_arenaRotation.Count();

            this.SaveOrder();
        }
        else
        {
            TStringArray supportedArenas = this.GetArenaNamesForRandomRotation(playerCount);

            if (supportedArenas.Count() > 1)
            {
                // Prevent the same arena from being picked twice in a row
                supportedArenas.RemoveItem(m_arenaName);
            }

            if (supportedArenas.Count() > 0)
            {
                m_arenaName = supportedArenas.GetRandomElement();
            }
            else
            {
                Error(string.Format("No arenas support the current player count (%1)", playerCount));
                return;
            }
        }

        PrintFormat("Picked arena: %1", m_arenaName);

        m_arena = m_registry.GetArena(m_arenaName);

        if (m_debugArena)
        {
            m_arena.DebugPlayerSpawns();
        }
    }

    private string NextArenaFromNames(int playerCount, TStringArray arenas)
    {
        TStringArray supportedArenas = m_registry.GetSupportedArenaNames(playerCount, arenas);

        if (supportedArenas.Count() > 0)
        {
            return supportedArenas.GetRandomElement();
        }

        return arenas.GetRandomElement();
    }

    private ref Param1<string> m_msg = new Param1<string>("");

    void CheckPlayerPosition(Man player)
    {
        if (m_debugArena)
        {
            float distance = m_arena.DistanceFromCenter(player.GetPosition());
            DMChat.MessagePlayer(player, string.Format("Distance: %1", distance));
        }

        DMArenaPositionStatus status = m_arena.GetPositionStatus(player);
        if (status == DMArenaPositionStatus.INSTANT_KILL)
        {
            NotificationSystem.SendNotificationToPlayerExtended(
                player, 5.0, "You are outside the zone!", "",
                "set:ccgui_enforce image:MapUserMarker");
            player.SetHealth(0.0);
        }
        else if (status == DMArenaPositionStatus.TAKING_DAMAGE)
        {
            NotificationSystem.SendNotificationToPlayerExtended(
                player, 5.0, "You are outside the zone!",
                "You will continue to lose health until you return to the zone.",
                "set:ccgui_enforce image:MapUserMarker");
            player.SetHealth(player.GetHealth() - 33);
        }
    }
};

// vim:ft=enforce
