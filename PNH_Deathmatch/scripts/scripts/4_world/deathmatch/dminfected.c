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

class DMInfected
{
    static private ref TStringArray CHRISTMAS_TYPES;

    static private ref TStringArray JOURNALIST_VESTS = {"PressVest_Blue", "PressVest_LightBlue"};
    static private ref TStringArray POLICE_VESTS = {"PoliceVest", "ReflexVest"};
    static private ref TStringArray MILITARY_VESTS = {
        "HighCapacityVest_Black", "HighCapacityVest_Olive", "PlateCarrierVest",
        "PlateCarrierVest_Black", "PlateCarrierVest_Green", "PlateCarrierVest_Camo", "SmershVest",
        "UKAssVest_Black", "UKAssVest_Camo", "UKAssVest_Khaki", "UKAssVest_Olive"
    };
    static private ref TStringArray MILITARY_HATS = {
        "MilitaryBeret_CDF", "MilitaryBeret_ChDKZ", "MilitaryBeret_NZ", "MilitaryBeret_Red",
        "MilitaryBeret_UN", "OfficerHat", "Ssh68Helmet"
    };

    static private const int RESPAWN_INTERVAL = 30000;

    static int infectedRoll = 101;
    static int aliveCount = 0;
    static int nextRespawn = 0;

    static private TStringArray GetChristmasTypes()
    {
        if (!CHRISTMAS_TYPES)
        {
            CHRISTMAS_TYPES = new TStringArray();

            if (g_Game.ConfigIsExisting("CfgVehicles ZmbM_Santa"))
            {
                CHRISTMAS_TYPES.Insert("ZmbM_Santa");
                CHRISTMAS_TYPES.Insert("ZmbM_Santa");
                CHRISTMAS_TYPES.Insert("ZmbM_Santa");
                CHRISTMAS_TYPES.Insert("ZmbM_Santa");
                CHRISTMAS_TYPES.Insert("ZmbM_Santa");
            }

            TStringArray types = DMDefaultInfectedConfig.AllInfectedTypes();

            foreach (string type : types)
            {
                if (type.Length() > 4)
                {
                    string suffix = type.Substring(type.Length() - 4, 4);
                    suffix.ToLower();
                    if (suffix == "_red")
                    {
                        CHRISTMAS_TYPES.Insert(type);
                    }
                }
            }
            CHRISTMAS_TYPES.ShuffleArray();

            PrintFormat("Found %1 Christmas infected types", CHRISTMAS_TYPES.Count());
        }

        return CHRISTMAS_TYPES;
    }

    static string InfectedType(DeathmatchSettings settings)
    {
        if (settings.christmas)
        {
            return DMInfected.GetChristmasTypes().GetRandomElement();
        }

        return settings.infectedTypes.GetRandomElement();
    }

    static void OnRoundEnd()
    {
        infectedRoll = 101;
        aliveCount = 0; // assumes infected are cleaned up elsewhere
    }

    static void OnInfectedDeath()
    {
        if (aliveCount <= 0) return;

        aliveCount--;
    }

    static private int CalculateInfectedCount(
            DMArena arena, int playerCount, DeathmatchSettings settings)
    {
        if ((playerCount >= settings.forceInfectedPlayerLimit) && (settings.infectedChance <= 0 || infectedRoll >= settings.infectedChance))
        {
            return 0;
        }

        int infectedCount = playerCount * settings.infectedPlayerFactor;

        infectedCount = Math.Max(infectedCount, settings.minimumInfected);
        infectedCount = Math.Min(infectedCount, settings.maximumInfected);

        infectedCount = Math.Min(infectedCount, arena.GetInfectedPositionCount());

        return infectedCount;
    }

    static void OnTick(DMArena arena, int playerCount, DeathmatchSettings settings, DMEvent dmEvent)
    {
#ifndef DEBUG_DEATHMATCH_ARENA
        if (g_Game.GetTime() < nextRespawn) return;

        int desiredCount = DMInfected.CalculateInfectedCount(arena, playerCount, settings);

        if (aliveCount >= desiredCount) return;

        nextRespawn = g_Game.GetTime() + RESPAWN_INTERVAL;

        vector position = arena.GetRandomInfectedPosition();

        CEApi ce = GetCEApi();
        if (!ce.AvoidPlayer(position, 20)) return;

        DMInfected.SpawnInfected(position, settings, dmEvent);
#endif
    }

    static void Spawn(DMArena arena, int playerCount, DeathmatchSettings settings, DMEvent dmEvent)
    {
#ifdef DEBUG_DEATHMATCH_ARENA
        TVectorArray positions = arena.GetInfectedPositions();
        foreach (vector pos : positions)
        {
            g_Game.CreateObjectEx("ZmbF_VillagerOld_Red", pos, ECE_PLACE_ON_SURFACE);
        }
#else
        if (!arena.HaveInfectedPositions())
        {
            Print("Infected not spawned because no infected spawn points are configured!");
            return;
        }

        infectedRoll = Math.RandomInt(0, 100);
        PrintFormat("Infected roll: %1", infectedRoll);

        int desiredCount = DMInfected.CalculateInfectedCount(arena, playerCount, settings);

        PrintFormat("Spawning %1 infected", desiredCount);
        if (desiredCount == arena.GetInfectedPositionCount())
        {
            TVectorArray positions = new TVectorArray();
            positions.InsertArray(arena.GetInfectedPositions());
            foreach (vector pos : positions)
            {
                DMInfected.SpawnInfected(pos, settings, dmEvent);
            }
        }
        else
        {
            for (int i = 0; i < desiredCount; i++)
            {
                vector position = arena.GetRandomInfectedPosition();

                DMInfected.SpawnInfected(position, settings, dmEvent);
            }
        }
        Print("Done spawning infected");
#endif
    }

    static private void SpawnInfected(vector position, DeathmatchSettings settings, DMEvent dmEvent)
    {
        string infectedType;
        if (dmEvent && dmEvent.HaveInfectedTypes())
        {
            infectedType = dmEvent.GetRandomInfectedType();
        }
        else
        {
            infectedType = InfectedType(settings);
        }

        Object obj = g_Game.CreateObjectEx(infectedType, position, ECE_INITAI | ECE_PLACE_ON_SURFACE);
        if (!obj)
        {
            Error(
                string.Format(
                    "Failed to spawn infected '%1' at %2", infectedType, position.ToString(true)));
            return;
        }

        EntityAI infected = EntityAI.Cast(obj);
        aliveCount++;
        GameInventory inventory = infected.GetInventory();
        if (dmEvent)
        {
            dmEvent.EquipInfected(infected);
        }
        else if (settings.christmas)
        {
            inventory.CreateAttachment("SantasHat");
        }
        // Random attachment presets for infected types are not honored, so emulate the behavior
        // with code, instead.
        else if (infectedType.Contains("Journalist"))
        {
            inventory.CreateAttachment(JOURNALIST_VESTS.GetRandomElement());
        }
        else if (infectedType.Contains("Police"))
        {
            inventory.CreateAttachment(POLICE_VESTS.GetRandomElement());
            inventory.CreateAttachment("PoliceCap");
        }
        else if (infectedType.Contains("Patrol") || infectedType.Contains("Soldier"))
        {
            inventory.CreateAttachment(MILITARY_VESTS.GetRandomElement());
            inventory.CreateAttachment(MILITARY_HATS.GetRandomElement());
        }
        else if (infectedType.Contains("Hunter"))
        {
            inventory.CreateAttachment("HuntingVest");
        }
        Print("Spawned " + infectedType + " at " + position);
    }
}

// vim:ft=enforce
