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

class DMCratePositions
{
    private ref TVectorArray m_positions = new TVectorArray();
    private int m_crateIndex = 0;

    void DMCratePositions(DMArena arena)
    {
        m_positions.InsertArray(arena.GetCratePositions());

        m_positions.ShuffleArray();
    }

    vector GetRandomPosition()
    {
        return m_positions.Get(m_crateIndex++);
    }

    int Count()
    {
        return m_positions.Count();
    }
}


class DMCrates
{
    private static ref array<autoptr Timer> timers = new array<autoptr Timer>();

    static void SpawnCrates(DeathmatchSettings settings, DMArena arena, DMEvent dmEvent)
    {
        timers.Clear();

        if (dmEvent)
        {
#ifdef DEBUG_DEATHMATCH_ARENA
            TVectorArray positions = arena.GetCratePositions();
            foreach (vector pos : positions)
            {
                g_Game.CreateObject("SeaChest", pos, false, false, false);
            }
#else
            DMCratePositions positions = new DMCratePositions(arena);
            int remaining = Math.Min(positions.Count(), dmEvent.GetCrateCount());

            if (dmEvent.HaveWeaponCrate() && remaining > 0)
            {
                dmEvent.FillWeaponCrate(DMCrates.SpawnCrate(positions));
                remaining--;
            }

            if (dmEvent.HaveArmorCrate() && remaining > 0)
            {
                dmEvent.FillArmorCrate(DMCrates.SpawnCrate(positions));
                remaining--;
            }

            if (dmEvent.HaveOtherCrate() && remaining > 0)
            {
                dmEvent.FillOtherCrate(DMCrates.SpawnCrate(positions));
            }
#endif
        }
        else
        {
            DMCrates.SpawnStandardCrates(settings, arena);
        }
    }

    static private void SpawnStandardCrates(DeathmatchSettings settings, DMArena arena)
    {
#ifdef DEBUG_DEATHMATCH_ARENA
        TVectorArray positions = arena.GetCratePositions();
        foreach (vector pos : positions)
        {
            g_Game.CreateObject("SeaChest", pos, false, false, false);
        }
#else
        DMCratePositions positions = new DMCratePositions(arena);

        if (positions.Count() < 1) return;
        DMCrates.SpawnWeaponCrate(positions, settings.crateWeapons.GetRandomElement());

        if (positions.Count() < 2) return;
        DMCrates.SpawnItemCrate(positions, settings.crateArmor);

        if (positions.Count() < 3) return;
        DMCrates.SpawnItemCrate(positions, settings.crateOther);
#endif
    }

    static private GameInventory SpawnCrate(DMCratePositions positions)
    {
        vector position = positions.GetRandomPosition();

        Print("Spawning crate at " + position);
        ItemBase chest = ItemBase.Cast(
            DMUtils.SpawnProp("SeaChest", position, DMUtils.RandomYaw()));
        chest.SetLifetime(7200);

#ifdef DAYZ_1_20_BROKE_SMOKE_GRENADES
        SmokeGrenadeBase grenade = SmokeGrenadeBase.Cast(
                g_Game.CreateObject("M18SmokeGrenade_Red", position, false, false, false));
        grenade.SetPosition(position);

        // Wait to unpin to give players more of a chance to see the smoke
        Timer timer = new Timer();
        timer.Run(30, grenade, "Unpin");
        timers.Insert(timer);
#endif

        return chest.GetInventory();
    }

    static private void SpawnWeaponCrate(DMCratePositions positions, DMWeaponConfig config)
    {
        GameInventory inventory = DMCrates.SpawnCrate(positions);

        DMCrates.FillWeaponCrate(inventory, config);
    }

    static void FillWeaponCrate(GameInventory inventory, DMWeaponConfig config)
    {
        EntityAI weapon = inventory.CreateInInventory(config.variants.GetRandomElement());
        DMWeapons.EquipWeaponAccessories(inventory, config, weapon);
    }

    static private void SpawnItemCrate(DMCratePositions positions, array<ref TStringArray> slots)
    {
        GameInventory inventory = DMCrates.SpawnCrate(positions);

        DMCrates.FillItemCrate(inventory, slots);
    }

    static void FillItemCrate(GameInventory inventory, array<ref TStringArray> slots)
    {
        foreach (TStringArray slot : slots)
        {
            string thing = slot.GetRandomElement();
            inventory.CreateInInventory(thing);
        }
    }
}

// vim:ft=enforce
