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

class DMEvent
{
    ref DMEventConfig m_config;

    static DMEvent CreateFromConfig(DMEventConfig config)
    {
        DMEvent result = new DMEvent();
        result.m_config = config;
        return result;
    }

    void DMEvent()
    {
    }

    string GetName()
    {
        return m_config.name;
    }

    string GetDisplayName()
    {
        return m_config.displayName;
    }

    bool HasGreeting()
    {
        return m_config.greeting && m_config.greeting.title != "" && m_config.greeting.details != "";
    }

    Param GetGreeting()
    {
        return new Param3<string, string, string>(
            m_config.greeting.title, m_config.greeting.details,
            "set:ccgui_enforce image:MapUserMarker");
    }

    EntityAI CreatePrimaryWeapon(HumanInventory inventory)
    {
        DMWeaponConfig config = m_config.primaryWeapons.GetRandomElement();

        if (!config) return null;

        string type = config.variants.GetRandomElement();
        EntityAI weapon = inventory.CreateInHands(type);
        if (weapon)
        {
            return DMWeapons.EquipWeaponAccessories(inventory, config, weapon);
        }
        else
        {
            Error(string.Format("Failed to create weapon %1 in hands", type));
            return null;
        }
    }

    EntityAI CreateSecondaryWeapon(HumanInventory inventory)
    {
        DMWeaponConfig config = m_config.secondaryWeapons.GetRandomElement();

        if (!config) return null;

        string type = config.variants.GetRandomElement();
        EntityAI weapon = inventory.CreateInInventory(type);
        if (weapon)
        {
            return DMWeapons.EquipWeaponAccessories(inventory, config, weapon);
        }
        else
        {
            Error(string.Format("Failed to create weapon %1 in inventory", type));
            return null;
        }
    }

    string GetRandomKnife()
    {
        return m_config.knives.GetRandomElement();
    }

    int GetCrateCount()
    {
        int result = 0;
        if (this.HaveWeaponCrate()) result++;
        if (this.HaveArmorCrate()) result++;
        if (this.HaveOtherCrate()) result++;
        return result;
    }

    bool HaveWeaponCrate()
    {
        return m_config.crateWeapons && m_config.crateWeapons.Count() > 0;
    }

    void FillWeaponCrate(GameInventory inventory)
    {
        DMWeaponConfig config = m_config.crateWeapons.GetRandomElement();
        if (config)
        {
            DMCrates.FillWeaponCrate(inventory, config);
        }
    }

    bool HaveArmorCrate()
    {
        return m_config.crateArmor && m_config.crateArmor.Count() > 0;
    }

    void FillArmorCrate(GameInventory inventory)
    {
        DMCrates.FillItemCrate(inventory, m_config.crateArmor);
    }

    bool HaveOtherCrate()
    {
        return m_config.crateOther && m_config.crateOther.Count() > 0;
    }

    void FillOtherCrate(GameInventory inventory)
    {
        DMCrates.FillItemCrate(inventory, m_config.crateOther);
    }

    TDMOutfit GetRandomOutfit()
    {
        return m_config.outfits.GetRandomElement();
    }

    bool HaveInfectedTypes()
    {
        return m_config.infectedTypes && m_config.infectedTypes.Count() > 0;
    }

    string GetRandomInfectedType()
    {
        return m_config.infectedTypes.GetRandomElement();
    }

    void EquipPlayer(PlayerBase player, HumanInventory inventory)
    {
    }

    void EquipInfected(EntityAI infected)
    {
    }

    void SpawnChristmas(DMArena arena, array<Object> clutter, DeathmatchSettings settings)
    {
        if (settings.christmas)
        {
            DMChristmas.Spawn(arena, clutter);
        }
    }

    void OnRoundStart(DMArena arena)
    {
    }

    void OnRoundEnd(DMArena arena)
    {
    }
};

// vim:ft=enforce
