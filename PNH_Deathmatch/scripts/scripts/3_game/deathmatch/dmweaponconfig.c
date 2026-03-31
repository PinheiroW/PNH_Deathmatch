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

class DMWeaponConfig
{
    ref TStringArray variants = new TStringArray();
    ref array<ref TStringArray> attachments = new array<ref TStringArray>();
    ref TStringArray accessories = new TStringArray();
}

class DMDefaultWeaponConfigs
{
    // Attachments
    private static const ref TStringArray HUNTING_OPTICS = {"HuntingOptic", "SportingOptic"};
    private static const ref TStringArray PU_OPTICS = {"PUScopeOptic"};
    private static const ref TStringArray PSO_OPTICS = {"PSO1Optic", "PSO11Optic", "PSO6Optic"};
    private static const ref TStringArray NATO_OPTICS = {
        "M68Optic", "M4_T3NRDSOptic", "ReflexOptic"
    };
    private static const ref TStringArray ACOG_OPTICS = {
        "ACOGOptic", "ACOGOptic_6x", "MK4Optic_Black", "MK4Optic_Green", "MK4Optic_Tan"
    };
    private static const ref TStringArray MP5_HANDGUARDS = {
        "MP5_PlasticHndgrd", "MP5_RailHndgrd"
    };
    private static const ref TStringArray MP5_BUTTSTOCKS = {"MP5k_StockBttstck"};
    private static const ref TStringArray M4_HANDGUARDS = {
        "M4_MPHndgrd", "M4_PlasticHndgrd", "M4_RISHndgrd"
    };
    private static const ref TStringArray M4_BUTTSTOCKS = {
        "M4_CQBBttstck", "M4_MPBttstck", "M4_OEBttstck"
    };
    private static const ref TStringArray AK_OPTICS = {"KobraOptic"};
    private static const ref TStringArray AK_HANDGUARDS = {
        "AK_PlasticHndgrd", "AK_RailHndgrd", "AK_RailHndgrd_Black", "AK_RailHndgrd_Green",
        "AK_WoodHndgrd", "AK_WoodHndgrd_Black", "AK_WoodHndgrd_Camo", "AK74_Hndgrd"
    };
    private static const ref TStringArray AK_BUTTSTOCKS = {
        "AK_FoldingBttstck", "AK_FoldingBttstck_Black", "AK_FoldingBttstck_Green",
        "AK_PlasticBttstck", "AK_PlasticBttstck_Black", "AK_PlasticBttstck_Green",
        "AK_WoodBttstck", "AK_WoodBttstck_Black", "AK_WoodBttstck_Camo", "AK74_WoodBttstck",
        "AKS74U_Bttstck"
    };
    private static const ref TStringArray PP19_BUTTSTOCKS = {"PP19_Bttstck"};
    private static const ref TStringArray SAIGA_BUTTSTOCKS = {"Saiga_Bttstck"};
    private static const ref TStringArray FAL_BUTTSTOCKS = {"Fal_FoldingBttstck", "Fal_OeBttstck"};
    private static const ref TStringArray IMPROVISED_SUPPRESSOR = {"ImprovisedSuppressor"};
    private static const ref TStringArray AK_SUPPRESSOR = {"AK_Suppressor"};

    // Accessories
    private static const ref TStringArray AMMO_308WIN = {
        "Ammo_308Win", "Ammo_308Win", "Ammo_308Win"
    };
    private static const ref TStringArray AMMO_762X54 = {
        "Ammo_762x54", "Ammo_762x54", "Ammo_762x54"
    };
    private static const ref TStringArray AMMO_762X39 = {
        "Ammo_762x39", "Ammo_762x39", "Ammo_762x39"
    };
    private static const ref TStringArray MAG_CZ527_5RND = {
        "Mag_CZ527_5rnd", "Mag_CZ527_5rnd", "Mag_CZ527_5rnd", "Mag_CZ527_5rnd"
    };
    private static const ref TStringArray MAG_SCOUT_5RND = {
        "Mag_Scout_5Rnd", "Mag_Scout_5Rnd", "Mag_Scout_5Rnd", "Mag_Scout_5Rnd"
    };
    private static const ref TStringArray MAG_CZ550_4RND = {
        "Mag_CZ550_4rnd", "Mag_CZ550_4rnd", "Mag_CZ550_4rnd", "Mag_CZ550_4rnd"
    };
    private static const ref TStringArray MAG_SSG82_5RND = {
        "Mag_SSG82_5rnd", "Mag_SSG82_5rnd", "Mag_SSG82_5rnd", "Mag_SSG82_5rnd"
    };
    private static const ref TStringArray MAG_MP5_30RND = {
        "Mag_MP5_30Rnd", "Mag_MP5_30Rnd", "Mag_MP5_30Rnd", "Mag_MP5_30Rnd"
    };
    private static const ref TStringArray MAG_UMP_25RND = {
        "Mag_UMP_25Rnd", "Mag_UMP_25Rnd", "Mag_UMP_25Rnd", "Mag_UMP_25Rnd"
    };
    private static const ref TStringArray MAG_AK74_30RND = {
        "Mag_AK74_30Rnd", "Mag_AK74_30Rnd", "Mag_AK74_30Rnd", "Mag_AK74_30Rnd"
    };
    private static const ref TStringArray MAG_STANAG_30RND = {
        "Mag_STANAG_30Rnd", "Mag_STANAG_30Rnd", "Mag_STANAG_30Rnd", "Mag_STANAG_30Rnd"
    };
    private static const ref TStringArray MAG_FAMAS_25RND = {
        "Mag_FAMAS_25Rnd", "Mag_FAMAS_25Rnd", "Mag_FAMAS_25Rnd", "Mag_FAMAS_25Rnd"
    };
    private static const ref TStringArray MAG_AUG_30RND = {
        "Mag_Aug_30Rnd", "Mag_Aug_30Rnd", "Mag_Aug_30Rnd", "Mag_Aug_30Rnd"
    };
    private static const ref TStringArray MAG_AK101_30RND = {
        "Mag_AK101_30Rnd", "Mag_AK101_30Rnd", "Mag_AK101_30Rnd", "Mag_AK101_30Rnd"
    };
    private static const ref TStringArray MAG_AKM_30RND = {
        "Mag_AKM_30Rnd", "Mag_AKM_30Rnd", "Mag_AKM_30Rnd", "Mag_AKM_30Rnd"
    };
    private static const ref TStringArray AMMO_12GAPELLETS = {
        "Ammo_12gaPellets", "Ammo_12gaPellets", "Ammo_12gaPellets"
    };
    private static const ref TStringArray MAG_SAIGA_8RND = {
        "Mag_Saiga_8Rnd", "Mag_Saiga_8Rnd", "Mag_Saiga_8Rnd", "Mag_Saiga_8Rnd"
    };
    private static const ref TStringArray MAG_CZ61_20RND = {
        "Mag_CZ61_20Rnd", "Mag_CZ61_20Rnd", "Mag_CZ61_20Rnd", "Mag_CZ61_20Rnd"
    };
    private static const ref TStringArray MAG_PP19_64RND = {
        "Mag_PP19_64Rnd", "Mag_PP19_64Rnd", "Mag_PP19_64Rnd", "Mag_PP19_64Rnd"
    };
    private static const ref TStringArray MAG_PM73_25RND = {
        "Mag_PM73_25Rnd", "Mag_PM73_25Rnd", "Mag_PM73_25Rnd", "Mag_PM73_25Rnd"
    };

    // Crate Armor
    static private ref TStringArray VESTS = {
        "PlateCarrierVest", "PlateCarrierVest_Black", "PlateCarrierVest_Green",
        "PlateCarrierVest_Camo", "PressVest_Blue", "PressVest_LightBlue", "PoliceVest"
    };
    static private ref TStringArray HELMETS = {
        "BallisticHelmet_BDU", "BallisticHelmet_Black", "BallisticHelmet_Desert",
        "BallisticHelmet_Green", "BallisticHelmet_UN", "BallisticHelmet_Woodland", "GorkaHelmet",
        "Mich2001Helmet", "Ssh68Helmet", "ZSh3PilotHelmet"
    };
    static private ref TStringArray GLOVES = {
        "TacticalGloves_Beige", "TacticalGloves_Black", "TacticalGloves_Green",
        "SurgicalGloves_Blue", "SurgicalGloves_Green", "SurgicalGloves_LightBlue",
        "SurgicalGloves_White", "NBCGlovesGray", "OMNOGloves_Brown", "OMNOGloves_Gray",
        "WorkingGloves_Beige", "WorkingGloves_Black", "WorkingGloves_Brown", "WorkingGloves_Yellow"
    };

    // Crate Other Items
    static private ref TStringArray TRAPS = {
        "LandMineTrap", "BearTrap", "TripwireTrap", "ClaymoreMine"
    };
    static private ref TStringArray GRENADES = {
        "FlashGrenade", "M67Grenade", "RGD5Grenade", "Grenade_ChemGas"
    };

    static void SetPrimaryWeapons(array<ref DMWeaponConfig> configs)
    {
        configs.Insert(DMDefaultWeaponConfigs.MP5());
        configs.Insert(DMDefaultWeaponConfigs.UMP());
        configs.Insert(DMDefaultWeaponConfigs.KAS74U());
        configs.Insert(DMDefaultWeaponConfigs.M4A1());
        configs.Insert(DMDefaultWeaponConfigs.KA101());
        configs.Insert(DMDefaultWeaponConfigs.KA74());
        configs.Insert(DMDefaultWeaponConfigs.AKM());
        configs.Insert(DMDefaultWeaponConfigs.Shotgun());
        configs.Insert(DMDefaultWeaponConfigs.Vaiga());
        configs.Insert(DMDefaultWeaponConfigs.R12());
        configs.Insert(DMDefaultWeaponConfigs.M16A2());
        configs.Insert(DMDefaultWeaponConfigs.LEMAS());
        configs.Insert(DMDefaultWeaponConfigs.AURA1());
        configs.Insert(DMDefaultWeaponConfigs.Skorpion());
        configs.Insert(DMDefaultWeaponConfigs.Bizon());
        configs.Insert(DMDefaultWeaponConfigs.Rak());
    }

    static void SetSecondaryWeapons(array<ref DMWeaponConfig> configs)
    {
        configs.Insert(DMDefaultWeaponConfigs.Winchester());
        configs.Insert(DMDefaultWeaponConfigs.Blaze());
        configs.Insert(DMDefaultWeaponConfigs.Mosin());
        configs.Insert(DMDefaultWeaponConfigs.SKS());
        configs.Insert(DMDefaultWeaponConfigs.CZ527());
        configs.Insert(DMDefaultWeaponConfigs.Pioneer());
        configs.Insert(DMDefaultWeaponConfigs.CR550());
        configs.Insert(DMDefaultWeaponConfigs.SSG82Rifle());
    }

    static void SetKnives(TStringArray configs)
    {
        configs.Insert("CombatKnife");
        configs.Insert("HuntingKnife");
        configs.Insert("KitchenKnife");
        configs.Insert("SteakKnife");
    }

    static void SetCrateWeapons(array<ref DMWeaponConfig> configs)
    {
        configs.Insert(DMDefaultWeaponConfigs.VSS());
        configs.Insert(DMDefaultWeaponConfigs.FAL());
        configs.Insert(DMDefaultWeaponConfigs.SVD());
        configs.Insert(DMDefaultWeaponConfigs.ASVAL());
        configs.Insert(DMDefaultWeaponConfigs.AURAX());
        configs.Insert(DMDefaultWeaponConfigs.GrenadeLauncher());
        configs.Insert(DMDefaultWeaponConfigs.DMR());
        configs.Insert(DMDefaultWeaponConfigs.Vikhr());
        configs.Insert(DMDefaultWeaponConfigs.VS89());
    }

    static void SetCrateArmor(array<ref TStringArray> armor)
    {
        armor.Insert(VESTS);
        armor.Insert(HELMETS);
        armor.Insert(GLOVES);
    }

    static void SetCrateOther(array<ref TStringArray> other)
    {
        other.Insert(TRAPS);
        other.Insert(TRAPS);
        other.Insert(TRAPS);
        other.Insert(GRENADES);
        other.Insert(GRENADES);
        other.Insert(GRENADES);
    }

    static DMWeaponConfig Winchester()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("Winchester70");
        config.accessories.InsertArray(AMMO_308WIN);
        config.attachments.Insert(HUNTING_OPTICS);
        return config;
    }

    static DMWeaponConfig Blaze()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("B95");
        config.accessories.InsertArray(AMMO_308WIN);
        config.attachments.Insert(HUNTING_OPTICS);
        return config;
    }

    static DMWeaponConfig Mosin()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("Mosin9130");
        config.variants.Insert("Mosin9130_Black");
        config.variants.Insert("Mosin9130_Camo");
        config.variants.Insert("Mosin9130_Green");
        config.accessories.InsertArray(AMMO_762X54);
        config.attachments.Insert(PU_OPTICS);
        return config;
    }

    static DMWeaponConfig SKS()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("SKS");
        config.accessories.InsertArray(AMMO_762X39);
        config.attachments.Insert(PU_OPTICS);
        return config;
    }

    static DMWeaponConfig CZ527()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("CZ527");
        config.accessories.InsertArray(MAG_CZ527_5RND);
        config.attachments.Insert(HUNTING_OPTICS);
        return config;
    }

    static DMWeaponConfig Pioneer()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("Scout");
        config.variants.Insert("Scout_Chernarus");
        config.variants.Insert("Scout_Livonia");
        config.accessories.InsertArray(MAG_SCOUT_5RND);
        config.attachments.Insert(ACOG_OPTICS);
        return config;
    }

    static DMWeaponConfig CR550()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("CZ550");
        config.accessories.InsertArray(MAG_CZ550_4RND);
        config.attachments.Insert(HUNTING_OPTICS);
        return config;
    }

    static DMWeaponConfig SSG82Rifle()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("SSG82");
        config.accessories.InsertArray(MAG_SSG82_5RND);
        return config;
    }

    static DMWeaponConfig MP5()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("MP5K");
        config.accessories.InsertArray(MAG_MP5_30RND);
        config.attachments.Insert(NATO_OPTICS);
        config.attachments.Insert(MP5_HANDGUARDS);
        config.attachments.Insert(MP5_BUTTSTOCKS);
        return config;
    }

    static DMWeaponConfig UMP()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("UMP45");
        config.accessories.InsertArray(MAG_UMP_25RND);
        config.attachments.Insert(NATO_OPTICS);
        return config;
    }

    static DMWeaponConfig KAS74U()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("AKS74U");
        config.variants.Insert("AKS74U_Black");
        config.variants.Insert("AKS74U_Green");
        config.accessories.InsertArray(MAG_AK74_30RND);
        config.attachments.Insert(AK_BUTTSTOCKS);
        return config;
    }

    static DMWeaponConfig M16A2()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("M16A2");
        config.accessories.InsertArray(MAG_STANAG_30RND);
        return config;
    }

    static DMWeaponConfig LEMAS()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("FAMAS");
        config.variants.Insert("SawedoffFAMAS");
        config.accessories.InsertArray(MAG_FAMAS_25RND);
        return config;
    }

    static DMWeaponConfig AURA1()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("AugShort");
        config.accessories.InsertArray(MAG_AUG_30RND);
        return config;
    }

    static DMWeaponConfig Skorpion()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("CZ61");
        config.accessories.InsertArray(MAG_CZ61_20RND);
        return config;
    }

    static DMWeaponConfig Bizon()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("PP19");
        config.accessories.InsertArray(MAG_PP19_64RND);
        config.attachments.Insert(AK_OPTICS);
        config.attachments.Insert(PP19_BUTTSTOCKS);
        return config;
    }

    static DMWeaponConfig Rak()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("PM73Rak");
        config.accessories.InsertArray(MAG_PM73_25RND);
        return config;
    }

    static DMWeaponConfig M4A1()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("M4A1");
        config.variants.Insert("M4A1_Black");
        config.variants.Insert("M4A1_Green");
        config.accessories.InsertArray(MAG_STANAG_30RND);
        config.attachments.Insert(NATO_OPTICS);
        config.attachments.Insert(M4_HANDGUARDS);
        config.attachments.Insert(M4_BUTTSTOCKS);
        return config;
    }

    static DMWeaponConfig KA101()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("AK101");
        config.variants.Insert("AK101_Black");
        config.variants.Insert("AK101_Green");
        config.accessories.InsertArray(MAG_AK101_30RND);
        config.attachments.Insert(AK_OPTICS);
        config.attachments.Insert(AK_HANDGUARDS);
        config.attachments.Insert(AK_BUTTSTOCKS);
        return config;
    }

    static DMWeaponConfig KA74()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("AK74");
        config.variants.Insert("AK74_Black");
        config.variants.Insert("AK74_Green");
        config.accessories.InsertArray(MAG_AK74_30RND);
        config.attachments.Insert(AK_OPTICS);
        config.attachments.Insert(AK_HANDGUARDS);
        config.attachments.Insert(AK_BUTTSTOCKS);
        return config;
    }

    static DMWeaponConfig AKM()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("AKM");
        config.accessories.InsertArray(MAG_AKM_30RND);
        config.attachments.Insert(AK_OPTICS);
        config.attachments.Insert(AK_HANDGUARDS);
        config.attachments.Insert(AK_BUTTSTOCKS);
        return config;
    }

    static DMWeaponConfig Shotgun()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("Izh43Shotgun");
        config.variants.Insert("SawedoffIzh43Shotgun");
        config.variants.Insert("Mp133Shotgun");
        config.accessories.InsertArray(AMMO_12GAPELLETS);
        return config;
    }

    static DMWeaponConfig Vaiga()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("Saiga");
        config.accessories.InsertArray(MAG_SAIGA_8RND);
        config.attachments.Insert(SAIGA_BUTTSTOCKS);
        return config;
    }

    static DMWeaponConfig R12()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("R12");
        config.accessories.InsertArray(AMMO_12GAPELLETS);
        return config;
    }

    static DMWeaponConfig VSS()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("VSS");
        config.attachments.Insert(PSO_OPTICS);
        config.accessories.Insert("Mag_VSS_10Rnd");
        config.accessories.Insert("Mag_VSS_10Rnd");
        config.accessories.Insert("Mag_VSS_10Rnd");
        config.accessories.Insert("Mag_VSS_10Rnd");
        config.accessories.Insert("Mag_VSS_10Rnd");
        config.accessories.Insert("Mag_VSS_10Rnd");
        return config;
    }

    static DMWeaponConfig ASVAL()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("ASVAL");
        config.attachments.Insert(ACOG_OPTICS);
        config.accessories.Insert("Mag_VAL_20Rnd");
        config.accessories.Insert("Mag_VAL_20Rnd");
        config.accessories.Insert("Mag_VAL_20Rnd");
        config.accessories.Insert("Mag_VAL_20Rnd");
        config.accessories.Insert("Mag_VAL_20Rnd");
        config.accessories.Insert("Mag_VAL_20Rnd");
        return config;
    }

    static DMWeaponConfig FAL(bool extraSuppressors = true)
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("FAL");
        config.attachments.Insert(ACOG_OPTICS);
        config.attachments.Insert(FAL_BUTTSTOCKS);
        config.attachments.Insert(IMPROVISED_SUPPRESSOR);
        config.accessories.Insert("Mag_FAL_20Rnd");
        config.accessories.Insert("Mag_FAL_20Rnd");
        config.accessories.Insert("Mag_FAL_20Rnd");
        config.accessories.Insert("Mag_FAL_20Rnd");
        config.accessories.Insert("Mag_FAL_20Rnd");
        config.accessories.Insert("Mag_FAL_20Rnd");
        if (extraSuppressors)
        {
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
        }
        return config;
    }

    static DMWeaponConfig SVD()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("SVD");
        config.variants.Insert("SVD_Wooden");
        config.attachments.Insert(PSO_OPTICS);
        config.attachments.Insert(AK_SUPPRESSOR);
        config.accessories.Insert("Mag_SVD_10Rnd");
        config.accessories.Insert("Mag_SVD_10Rnd");
        config.accessories.Insert("Mag_SVD_10Rnd");
        config.accessories.Insert("Mag_SVD_10Rnd");
        config.accessories.Insert("Mag_SVD_10Rnd");
        config.accessories.Insert("Mag_SVD_10Rnd");
        return config;
    }

    static DMWeaponConfig AURAX()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("Aug");
        config.accessories.Insert("Mag_Aug_30Rnd");
        config.accessories.Insert("Mag_Aug_30Rnd");
        config.accessories.Insert("Mag_Aug_30Rnd");
        config.accessories.Insert("Mag_Aug_30Rnd");
        config.accessories.Insert("Mag_Aug_30Rnd");
        config.accessories.Insert("Mag_Aug_30Rnd");
        config.attachments.Insert(ACOG_OPTICS);
        config.attachments.Insert({"M4_Suppressor"});
        return config;
    }

    static DMWeaponConfig GrenadeLauncher()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("M79");
        config.accessories.Insert("Ammo_40mm_Explosive");
        config.accessories.Insert("Ammo_40mm_ChemGas");
        config.accessories.Insert("Ammo_40mm_Explosive");
        config.accessories.Insert("Ammo_40mm_ChemGas");
        config.accessories.Insert("Ammo_40mm_Explosive");
        config.accessories.Insert("Ammo_40mm_ChemGas");
        config.accessories.Insert("Ammo_40mm_Explosive");
        config.accessories.Insert("Ammo_40mm_ChemGas");
        config.accessories.Insert("Ammo_40mm_Explosive");
        config.accessories.Insert("Ammo_40mm_ChemGas");
        config.accessories.Insert("Ammo_40mm_Explosive");
        config.accessories.Insert("Ammo_40mm_ChemGas");
        return config;
    }

    static DMWeaponConfig DMR(bool extraSuppressors = true)
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("M14");
        config.attachments.Insert(ACOG_OPTICS);
        config.attachments.Insert(IMPROVISED_SUPPRESSOR);
        config.accessories.Insert("Mag_M14_10Rnd");
        config.accessories.Insert("Mag_M14_10Rnd");
        config.accessories.Insert("Mag_M14_10Rnd");
        config.accessories.Insert("Mag_M14_10Rnd");
        config.accessories.Insert("Mag_M14_10Rnd");
        config.accessories.Insert("Mag_M14_10Rnd");
        if (extraSuppressors)
        {
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
        }
        return config;
    }

    static DMWeaponConfig Vikhr(bool extraSuppressors = true)
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("Vikhr");
        config.attachments.Insert(PSO_OPTICS);
        config.attachments.Insert(IMPROVISED_SUPPRESSOR);
        config.accessories.Insert("Mag_Vikhr_30Rnd");
        config.accessories.Insert("Mag_Vikhr_30Rnd");
        config.accessories.Insert("Mag_Vikhr_30Rnd");
        config.accessories.Insert("Mag_Vikhr_30Rnd");
        config.accessories.Insert("Mag_Vikhr_30Rnd");
        config.accessories.Insert("Mag_Vikhr_30Rnd");
        if (extraSuppressors)
        {
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
        }
        return config;
    }

    static DMWeaponConfig VS89(bool extraSuppressors = true)
    {
        DMWeaponConfig config = new DMWeaponConfig();
        config.variants.Insert("SV98");
        config.attachments.Insert(ACOG_OPTICS);
        config.attachments.Insert(IMPROVISED_SUPPRESSOR);
        config.accessories.Insert("Mag_SV98_10Rnd");
        config.accessories.Insert("Mag_SV98_10Rnd");
        config.accessories.Insert("Mag_SV98_10Rnd");
        config.accessories.Insert("Mag_SV98_10Rnd");
        config.accessories.Insert("Mag_SV98_10Rnd");
        config.accessories.Insert("Mag_SV98_10Rnd");
        if (extraSuppressors)
        {
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
            config.accessories.Insert("ImprovisedSuppressor");
        }
        return config;
    }
}

// vim:ft=enforce
