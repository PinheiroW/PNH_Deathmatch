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

class DMHotshotEvent : DMEvent
{
    static private ref TStringArray QUIPS = {
        "Is this the mod or the standalone?",
        "Let's camp the camo building!",
        "Spray and pray!"
    };
    static private ref TStringArray SHOES = {
        "CombatBoots_Beige", "CombatBoots_Black", "CombatBoots_Brown", "CombatBoots_Green",
        "CombatBoots_Grey", "JungleBoots_Beige", "JungleBoots_Black", "JungleBoots_Brown",
        "JungleBoots_Green", "JungleBoots_Olive", "MilitaryBoots_Beige", "MilitaryBoots_Black",
        "MilitaryBoots_Bluerock", "MilitaryBoots_Brown", "MilitaryBoots_Redpunk"
    };
    static private ref TStringArray HATS = {
        "BallisticHelmet_BDU", "BallisticHelmet_Black", "BallisticHelmet_Desert",
        "BallisticHelmet_Green", "BallisticHelmet_UN", "BallisticHelmet_Woodland", "Mich2001Helmet",
        "Ssh68Helmet", "ZSh3PilotHelmet"
    };
    static private ref TStringArray VESTS = {
        "PlateCarrierVest", "PlateCarrierVest_Black", "PlateCarrierVest_Green",
        "PlateCarrierVest_Camo"
    };
    static private ref TStringArray BELTS = {"MilitaryBelt"};
    static private ref TStringArray BACKPACKS = {
        "AliceBag_Black", "AliceBag_Camo", "AliceBag_Green", "AssaultBag_Black",
        "AssaultBag_Green", "AssaultBag_Ttsko", "CoyoteBag_Brown", "CoyoteBag_Green", "HuntingBag",
        "SmershBag", "TortillaBag"
    };
    static private ref TStringArray MASKS = {
        "BalaclavaMask_Blackskull", "Balaclava3Holes_Black"
    };
    static private ref TStringArray GLOVES = {
        "OMNOGloves_Brown", "OMNOGloves_Gray", "TacticalGloves_Beige", "TacticalGloves_Black",
        "TacticalGloves_Green"
    };
    static private ref TStringArray KNIVES = {"HuntingKnife", "CombatKnife"};
    static private ref TStringArray FRAGS = {"M67Grenade", "RGD5Grenade"};
    static private ref TStringArray FLASHES = {"FlashGrenade"};
    static private ref TStringArray SMOKES = {
        "M18SmokeGrenade_Green", "M18SmokeGrenade_Purple", "M18SmokeGrenade_Red",
        "M18SmokeGrenade_White", "M18SmokeGrenade_Yellow", "RDG2SmokeGrenade_Black",
        "RDG2SmokeGrenade_White"
    };

    void DMHotshotEvent()
    {
        m_config = new DMEventConfig();
        m_config.name = "hotshot";
        m_config.displayName = "Hotshot round";
        m_config.greeting.title = "((( HOTSHOT ROUND )))";

        m_config.outfits.Insert(DMHotshotEvent.Outfit("BDUJacket", "BDUPants"));
        m_config.outfits.Insert(DMHotshotEvent.Gorka("Autumn"));
        m_config.outfits.Insert(DMHotshotEvent.Gorka("Flat"));
        m_config.outfits.Insert(DMHotshotEvent.Gorka("PautRev"));
        m_config.outfits.Insert(DMHotshotEvent.Gorka("Summer"));
        m_config.outfits.Insert(DMHotshotEvent.Outfit("HuntingJacket_Autumn", "HunterPants_Autumn"));
        m_config.outfits.Insert(DMHotshotEvent.Outfit("HuntingJacket_Brown", "HunterPants_Brown"));
        m_config.outfits.Insert(DMHotshotEvent.Outfit("HuntingJacket_Spring", "HunterPants_Spring"));
        m_config.outfits.Insert(DMHotshotEvent.Outfit("HuntingJacket_Summer", "HunterPants_Summer"));
        m_config.outfits.Insert(DMHotshotEvent.Outfit("HuntingJacket_Winter", "HunterPants_Winter"));
        m_config.outfits.Insert(DMHotshotEvent.Outfit("M65Jacket_Black", "CargoPants_Black"));
        m_config.outfits.Insert(DMHotshotEvent.Outfit("M65Jacket_Khaki", "CargoPants_Beige"));
        m_config.outfits.Insert(DMHotshotEvent.Outfit("M65Jacket_Olive", "CargoPants_Green"));
        m_config.outfits.Insert(DMHotshotEvent.Outfit("M65Jacket_Tan", "CargoPants_Beige"));
        m_config.outfits.Insert(DMHotshotEvent.Outfit("PoliceJacketOrel", "PolicePantsOrel"));
        m_config.outfits.Insert(DMHotshotEvent.TTSKO());
        m_config.outfits.Insert(DMHotshotEvent.Outfit("USMCJacket_Desert", "USMCPants_Desert"));
        m_config.outfits.Insert(DMHotshotEvent.Outfit("USMCJacket_Woodland", "USMCPants_Woodland"));

        m_config.primaryWeapons.Insert(DMDefaultWeaponConfigs.SVD());
        m_config.primaryWeapons.Insert(DMDefaultWeaponConfigs.FAL(false));
        m_config.primaryWeapons.Insert(DMDefaultWeaponConfigs.VSS());
        m_config.primaryWeapons.Insert(DMDefaultWeaponConfigs.ASVAL());
        m_config.primaryWeapons.Insert(DMDefaultWeaponConfigs.AURAX());
        m_config.primaryWeapons.Insert(DMDefaultWeaponConfigs.GrenadeLauncher());
        m_config.primaryWeapons.Insert(DMDefaultWeaponConfigs.Vikhr(false));
        m_config.primaryWeapons.Insert(DMDefaultWeaponConfigs.Rak());
        m_config.secondaryWeapons.Insert(DMHotshotEvent.Winchester());
        m_config.secondaryWeapons.Insert(DMHotshotEvent.Mosin());
        m_config.secondaryWeapons.Insert(DMHotshotEvent.CR550());
        m_config.secondaryWeapons.Insert(DMDefaultWeaponConfigs.DMR(false));
        m_config.secondaryWeapons.Insert(DMDefaultWeaponConfigs.VS89(false));
        m_config.knives.InsertArray(KNIVES);

        this.SetInfectedTypes();
    }

    private void SetInfectedTypes()
    {
        TStringArray all = DMDefaultInfectedConfig.AllInfectedTypes();
        foreach (string type : all)
        {
            if (type.Contains("Soldier") || type.Contains("Patrol") || type.Contains("Police"))
            {
                m_config.infectedTypes.Insert(type);
            }
        }
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

    override bool HaveWeaponCrate()
    {
        return true;
    }

    override void FillWeaponCrate(GameInventory inventory)
    {
        int i;
        for (i = 0; i < 32; i++) inventory.CreateInInventory("Ammo_308WinTracer");
        for (i = 0; i < 32; i++) inventory.CreateInInventory("Ammo_762x54Tracer");
        for (i = 0; i < 32; i++) inventory.CreateInInventory("Ammo_9x39AP");
    }

    override bool HaveArmorCrate()
    {
        return true;
    }

    override void FillArmorCrate(GameInventory inventory)
    {
        inventory.CreateInInventory("GhillieSuit_Woodland");
        inventory.CreateInInventory("GhillieTop_Woodland");
        inventory.CreateInInventory("GhillieAtt_Woodland");
        inventory.CreateInInventory("GhillieBushrag_Woodland");
        inventory.CreateInInventory("GhillieHood_Woodland");
    }

    override bool HaveOtherCrate()
    {
        return true;
    }

    override void FillOtherCrate(GameInventory inventory)
    {
        int i;
        for (i = 0; i < 4; i++) inventory.CreateInInventory("Splint");
        for (i = 0; i < 8; i++) inventory.CreateInInventory("SalineBagIV");
        for (i = 0; i < 8; i++) inventory.CreateInInventory("Bandage");
        for (i = 0; i < 8; i++) inventory.CreateInInventory("Morphine");
        for (i = 0; i < 8; i++) inventory.CreateInInventory("Epinephrine");
    }

    override void EquipPlayer(PlayerBase player, HumanInventory inventory)
    {
        EntityAI vest = inventory.FindAttachment(InventorySlots.VEST);
        if (vest)
        {
            vest.GetInventory().CreateAttachment(DMHotshotEvent.FRAGS.GetRandomElement());
            vest.GetInventory().CreateAttachment(DMHotshotEvent.FLASHES.GetRandomElement());
            vest.GetInventory().CreateAttachment(DMHotshotEvent.SMOKES.GetRandomElement());
        }

        EntityAI rangefinder = inventory.CreateInInventory("Rangefinder");
        rangefinder.GetInventory().CreateAttachment("Battery9V");
    }

    override void SpawnChristmas(DMArena arena, array<Object> clutter, DeathmatchSettings settings)
    {
        TVectorArray positions = arena.GetChristmasTreePositions();

        foreach (vector pos : positions)
        {
            clutter.Insert(DMUtils.SpawnProp("Land_Wreck_Uaz", pos + "0 0.85 0", vector.Zero));

            EntityAI.Cast(
                DMUtils.SpawnProp(
                    "RGD5Grenade", pos + "0.85 0.8 1.6", vector.Zero)).SetLifetime(7200);
            EntityAI.Cast(
                DMUtils.SpawnProp(
                    "RGD5Grenade", pos + "-0.5 0.8 1.6", vector.Zero)).SetLifetime(7200);
            EntityAI.Cast(
                DMUtils.SpawnProp(
                    "RGD5Grenade", pos + "0.2 0.6 1.6", vector.Zero)).SetLifetime(7200);
            EntityAI.Cast(
                DMUtils.SpawnProp(
                    "RGD5Grenade", pos + "0 1.2 -1.5", vector.Zero)).SetLifetime(7200);

            auto vsd = Weapon_Base.Cast(
                DMUtils.SpawnProp("SVD", pos + "0.5 1.13 -0.9", "174.5 81 160.5"));
            vsd.SetLifetime(7200);
            vsd.SpawnAmmo("Mag_SVD_10Rnd", Weapon_Base.SAMF_DEFAULT);
            vsd.GetInventory().CreateAttachment("AK_Suppressor");
            vsd.GetInventory().CreateAttachment("GhillieAtt_Tan");
            vsd.GetInventory().CreateAttachment("PSO11Optic").GetInventory().CreateAttachment("Battery9V");
        }
    }

    private static TDMOutfit Gorka(string suffix)
    {
        TDMOutfit outfit = new TDMOutfit();
        outfit.Insert(DMClothesConfig.FromArray({"GorkaEJacket_" + suffix}));
        outfit.Insert(DMClothesConfig.FromArray({"GorkaPants_" + suffix}));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.SHOES));
        outfit.Insert(DMClothesConfig.FromArray({"GorkaHelmet"}));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.VESTS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.BELTS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.BACKPACKS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.MASKS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.GLOVES));
        return outfit;
    }

    private static TDMOutfit TTSKO()
    {
        TDMOutfit outfit = new TDMOutfit();
        outfit.Insert(DMClothesConfig.FromArray({"TTsKOJacket_Camo"}));
        outfit.Insert(DMClothesConfig.FromArray({"TTSKOPants"}));
        outfit.Insert(DMClothesConfig.FromArray({"TTSKOBoots"}));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.HATS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.VESTS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.BELTS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.BACKPACKS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.MASKS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.GLOVES));
        return outfit;
    }

    private static TDMOutfit Outfit(string top, string bottom)
    {
        TDMOutfit outfit = new TDMOutfit();
        outfit.Insert(DMClothesConfig.FromArray({top}));
        outfit.Insert(DMClothesConfig.FromArray({bottom}));
        DMHotshotEvent.AddCommonClothes(outfit);
        return outfit;
    }

    private static void AddCommonClothes(TDMOutfit outfit)
    {
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.SHOES));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.HATS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.VESTS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.BELTS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.BACKPACKS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.MASKS));
        outfit.Insert(DMClothesConfig.FromArray(DMHotshotEvent.GLOVES));
    }

    private static DMWeaponConfig Winchester()
    {
        DMWeaponConfig config = DMDefaultWeaponConfigs.Winchester();
        config.accessories.Insert("Ammo_308Win");
        config.accessories.Insert("Ammo_308Win");
        config.accessories.Insert("Ammo_308Win");
        return config;
    }

    private static DMWeaponConfig Mosin()
    {
        DMWeaponConfig config = DMDefaultWeaponConfigs.Mosin();
        config.accessories.Insert("Ammo_762x54");
        config.accessories.Insert("Ammo_762x54");
        config.accessories.Insert("Ammo_762x54");
        return config;
    }

    private static DMWeaponConfig CR550()
    {
        DMWeaponConfig config = DMDefaultWeaponConfigs.CR550();
        config.accessories.Insert("Mag_CZ550_4rnd");
        config.accessories.Insert("Mag_CZ550_4rnd");
        config.accessories.Insert("Mag_CZ550_4rnd");
        return config;
    }

    private static DMWeaponConfig ExtraAmmo()
    {
        DMWeaponConfig config = new DMWeaponConfig();
        int i;
        for (i = 0; i < 32; i++) config.accessories.Insert("Ammo_308WinTracer");
        for (i = 0; i < 32; i++) config.accessories.Insert("Ammo_762x54Tracer");
        for (i = 0; i < 32; i++) config.accessories.Insert("Ammo_9x39AP");
        return config;
    }
};

// vim:ft=enforce
