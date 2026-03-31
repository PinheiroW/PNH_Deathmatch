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

class DMBambiEvent : DMEvent
{
    static private ref TStringArray QUIPS = {
        "You wouldn't kill a fresh spawn, would you?",
        "Are you friendly?",
        "Don't shoot! I'm friendly!"
    };
    static private ref TStringArray SECONDARIES = {
        "FirefighterAxe", "FirefighterAxe_Black", "FirefighterAxe_Green", "WoodAxe",
        "Pickaxe", "SledgeHammer", "Pipe", "Shovel", "FarmingHoe",
        "SpearStone", "SpearBone", "Pitchfork", "Cleaver", "MeatTenderizer"
    };
    static private ref TStringArray FRUIT = {"Apple", "Pear", "Plum"};
    static private ref TStringArray BACKPACKS = {
        "ChildBag_Blue", "ChildBag_Green", "ChildBag_Red", "CourierBag", "FurCourierBag"
    };
    static private ref TStringArray VESTS = {
        "ReflexVest", "UKAssVest_Black", "UKAssVest_Camo", "UKAssVest_Khaki", "UKAssVest_Olive",
        "ChestHolster"
    };
    static private ref TStringArray HELMETS = {
        "ConstructionHelmet_Blue", "ConstructionHelmet_Lime", "ConstructionHelmet_Orange",
        "ConstructionHelmet_Red", "ConstructionHelmet_White", "ConstructionHelmet_Yellow",
        "DarkMotoHelmet_Black", "DarkMotoHelmet_Blue", "DarkMotoHelmet_Green",
        "DarkMotoHelmet_Grey", "DarkMotoHelmet_Lime", "DarkMotoHelmet_Red", "DarkMotoHelmet_White",
        "DirtBikeHelmet_Black", "DirtBikeHelmet_Blue", "DirtBikeHelmet_Chernarus",
        "DirtBikeHelmet_Green", "DirtBikeHelmet_Khaki", "DirtBikeHelmet_Police",
        "DirtBikeHelmet_Red", "FirefightersHelmet_Red", "FirefightersHelmet_White",
        "FirefightersHelmet_Yellow", "HockeyHelmet_Black", "HockeyHelmet_Blue", "HockeyHelmet_Red",
        "HockeyHelmet_White", "MotoHelmet_Black", "MotoHelmet_Blue", "MotoHelmet_Green",
        "MotoHelmet_Grey", "MotoHelmet_Lime", "MotoHelmet_Red", "MotoHelmet_White",
        "SkateHelmet_Black", "SkateHelmet_Blue", "SkateHelmet_Gray", "SkateHelmet_Green",
        "SkateHelmet_Red"
    };
    static private ref TStringArray GLOVES = {
        "SurgicalGloves_Blue", "SurgicalGloves_Green", "SurgicalGloves_LightBlue",
        "SurgicalGloves_White", "WorkingGloves_Beige", "WorkingGloves_Black", "WorkingGloves_Brown",
        "WorkingGloves_Yellow"
    };
    static private ref TStringArray SUPPRESSORS = {"PistolSuppressor", "ImprovisedSuppressor"};
    static private ref TStringArray MEDICAL = {
        "BandageDressing", "Rag", "SalineBagIV", "Morphine", "Epinephrine", "Splint"
    };

    void DMBambiEvent()
    {
        m_config = new DMEventConfig();
        m_config.name = "bambi";
        m_config.displayName = "Bambi round";
        m_config.greeting.title = "Bambi Round :^)";

        TDMOutfit outfit = new TDMOutfit();
        outfit.Insert(
                DMClothesConfig.FromArray(
                    DefaultCharacterCreationMethods.GetConfigAttachmentTypes(InventorySlots.BODY)));
        outfit.Insert(
                DMClothesConfig.FromArray(
                    DefaultCharacterCreationMethods.GetConfigAttachmentTypes(InventorySlots.LEGS)));
        outfit.Insert(
                DMClothesConfig.FromArray(
                    DefaultCharacterCreationMethods.GetConfigAttachmentTypes(InventorySlots.FEET)));
        outfit.Insert(DMClothesConfig.FromArray(DMBambiEvent.BACKPACKS));
        m_config.outfits.Insert(outfit);

        m_config.primaryWeapons.Insert(this.Makarov());
        m_config.primaryWeapons.Insert(this.CZ75());
        m_config.primaryWeapons.Insert(this.Glock19());
        m_config.primaryWeapons.Insert(this.Amphibia());
        m_config.primaryWeapons.Insert(this.P1());
        m_config.primaryWeapons.Insert(this.SawedoffMagnum());
        m_config.primaryWeapons.Insert(this.Derringer());

        m_config.crateWeapons.Insert(this.Sporter());
        m_config.crateWeapons.Insert(this.IZH18());
        m_config.crateWeapons.Insert(this.Skorpion());
        m_config.crateWeapons.Insert(this.Repeater());
        m_config.crateWeapons.Insert(this.Shotgun());
        m_config.crateWeapons.Insert(this.Mosin());
        m_config.crateWeapons.Insert(this.Longhorn());
        m_config.crateWeapons.Insert(this.Magnum());
        m_config.crateWeapons.Insert(this.SawedoffB95());

        m_config.knives.Insert("SteakKnife");
        m_config.knives.Insert("StoneKnife");
        m_config.knives.Insert("BoneKnife");

        m_config.crateArmor.Insert(VESTS);
        m_config.crateArmor.Insert(HELMETS);
        m_config.crateArmor.Insert(GLOVES);
        m_config.crateArmor.Insert(SUPPRESSORS);

        m_config.crateOther.Insert(MEDICAL);
        m_config.crateOther.Insert(MEDICAL);
        m_config.crateOther.Insert(MEDICAL);
        m_config.crateOther.Insert(MEDICAL);

        m_config.infectedTypes = null;
    }

    override bool HasGreeting()
    {
        return true;
    }

    override Param GetGreeting()
    {
        return new Param3<string, string, string>(
            m_config.greeting.title, QUIPS.GetRandomElement(),
            "set:ccgui_enforce image:MapUserMarker");
    }

    override EntityAI CreateSecondaryWeapon(HumanInventory inventory)
    {
        EntityAI weapon = inventory.CreateInInventory(SECONDARIES.GetRandomElement());
        inventory.CreateInInventory(FRUIT.GetRandomElement());
        return weapon;
    }

    override void SpawnChristmas(DMArena arena, array<Object> clutter, DeathmatchSettings settings)
    {
        TVectorArray positions = arena.GetChristmasTreePositions();

        foreach (vector pos : positions)
        {
            clutter.Insert(DMUtils.SpawnProp("Land_Wreck_hb01_aban1_police", pos + "0 1 0", "0 0 0"));

            this.SpawnLoot("M4_Suppressor", pos + "-0.4 0.65 0", "90 90 0");
            this.SpawnLoot("PistolSuppressor", pos + "0.2 0.6 1.6", "0 90 0");
            this.SpawnLoot("Mag_Scout_5Rnd", pos + "0.5 0.7 0", "45 90 0");

            auto vsd = Weapon_Base.Cast(this.SpawnLoot("Scout", pos + "0.3 1.05 -0.9", "0 90 0"));
            vsd.SpawnAmmo("Mag_Scout_5Rnd", Weapon_Base.SAMF_DEFAULT);
            vsd.GetInventory().CreateAttachment("ACOGOptic");
        }
    }

    private EntityAI SpawnLoot(string type, vector position, vector orientation)
    {
            auto result = EntityAI.Cast(DMUtils.SpawnProp(type, position, orientation));
            result.SetLifetime(7200);
            return result;
    }

    private DMWeaponConfig Makarov()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("MakarovIJ70");
        result.accessories.Insert("Mag_IJ70_8Rnd");
        result.accessories.Insert("Mag_IJ70_8Rnd");
        result.accessories.Insert("Mag_IJ70_8Rnd");
        result.accessories.Insert("Mag_IJ70_8Rnd");
        return result;
    }

    private DMWeaponConfig CZ75()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("CZ75");
        result.accessories.Insert("Mag_CZ75_15Rnd");
        result.accessories.Insert("Mag_CZ75_15Rnd");
        result.accessories.Insert("Mag_CZ75_15Rnd");
        result.accessories.Insert("Mag_CZ75_15Rnd");
        return result;
    }

    private DMWeaponConfig Glock19()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("Glock19");
        result.accessories.Insert("Mag_Glock_15Rnd");
        result.accessories.Insert("Mag_Glock_15Rnd");
        result.accessories.Insert("Mag_Glock_15Rnd");
        result.accessories.Insert("Mag_Glock_15Rnd");
        return result;
    }

    private DMWeaponConfig Amphibia()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("MKII");
        result.accessories.Insert("Mag_MKII_10Rnd");
        result.accessories.Insert("Mag_MKII_10Rnd");
        result.accessories.Insert("Mag_MKII_10Rnd");
        result.accessories.Insert("Mag_MKII_10Rnd");
        return result;
    }

    private DMWeaponConfig SawedoffMagnum()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("SawedoffMagnum");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        return result;
    }

    private DMWeaponConfig Derringer()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("Derringer_Black");
        result.variants.Insert("Derringer_Pink");
        result.variants.Insert("Derringer_Grey");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        return result;
    }

    private DMWeaponConfig P1()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("P1");
        result.accessories.Insert("Mag_P1_8Rnd");
        result.accessories.Insert("Mag_P1_8Rnd");
        result.accessories.Insert("Mag_P1_8Rnd");
        result.accessories.Insert("Mag_P1_8Rnd");
        return result;
    }

    private DMWeaponConfig Sporter()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("Ruger1022");
        result.accessories.Insert("Mag_Ruger1022_15Rnd");
        result.accessories.Insert("Mag_Ruger1022_15Rnd");
        result.accessories.Insert("Mag_Ruger1022_15Rnd");
        result.accessories.Insert("Mag_Ruger1022_15Rnd");
        result.accessories.Insert("Mag_Ruger1022_15Rnd");
        result.accessories.Insert("Mag_Ruger1022_15Rnd");
        result.attachments.Insert({"HuntingOptic"});
        return result;
    }

    private DMWeaponConfig IZH18()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("Izh18");
        result.variants.Insert("SawedoffIzh18");
        result.accessories.Insert("Ammo_762x39");
        result.accessories.Insert("Ammo_762x39");
        result.accessories.Insert("Ammo_762x39");
        result.accessories.Insert("Ammo_762x39");
        result.accessories.Insert("Ammo_762x39");
        result.accessories.Insert("Ammo_762x39");
        return result;
    }

    private DMWeaponConfig Skorpion()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("CZ61");
        result.accessories.Insert("Mag_CZ61_20Rnd");
        result.accessories.Insert("Mag_CZ61_20Rnd");
        result.accessories.Insert("Mag_CZ61_20Rnd");
        result.accessories.Insert("Mag_CZ61_20Rnd");
        result.accessories.Insert("Mag_CZ61_20Rnd");
        result.accessories.Insert("Mag_CZ61_20Rnd");
        return result;
    }

    private DMWeaponConfig Repeater()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("Repeater");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        return result;
    }

    private DMWeaponConfig Shotgun()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("Izh43Shotgun");
        result.variants.Insert("SawedoffIzh43Shotgun");
        result.variants.Insert("Izh18Shotgun");
        result.variants.Insert("SawedoffIzh18Shotgun");
        result.accessories.Insert("Ammo_12gaPellets");
        result.accessories.Insert("Ammo_12gaPellets");
        result.accessories.Insert("Ammo_12gaPellets");
        result.accessories.Insert("Ammo_12gaPellets");
        result.accessories.Insert("Ammo_12gaPellets");
        result.accessories.Insert("Ammo_12gaPellets");
        return result;
    }

    private DMWeaponConfig Mosin()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("SawedoffMosin9130");
        result.variants.Insert("SawedoffMosin9130_Black");
        result.variants.Insert("SawedoffMosin9130_Camo");
        result.variants.Insert("SawedoffMosin9130_Green");
        result.accessories.Insert("Ammo_762x54");
        result.accessories.Insert("Ammo_762x54");
        result.accessories.Insert("Ammo_762x54");
        result.accessories.Insert("Ammo_762x54");
        result.accessories.Insert("Ammo_762x54");
        result.accessories.Insert("Ammo_762x54");
        return result;
    }

    private DMWeaponConfig Longhorn()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("Longhorn");
        result.accessories.Insert("Ammo_308Win");
        result.accessories.Insert("Ammo_308Win");
        result.accessories.Insert("Ammo_308Win");
        result.accessories.Insert("Ammo_308Win");
        result.accessories.Insert("Ammo_308Win");
        result.accessories.Insert("Ammo_308Win");
        return result;
    }

    private DMWeaponConfig Magnum()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("Magnum");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        result.accessories.Insert("Ammo_357");
        return result;
    }

    private DMWeaponConfig SawedoffB95()
    {
        DMWeaponConfig result = new DMWeaponConfig();
        result.variants.Insert("SawedoffB95");
        result.accessories.Insert("Ammo_308Win");
        result.accessories.Insert("Ammo_308Win");
        result.accessories.Insert("Ammo_308Win");
        result.accessories.Insert("Ammo_308Win");
        result.accessories.Insert("Ammo_308Win");
        result.accessories.Insert("Ammo_308Win");
        return result;
    }
};

// vim:ft=enforce
