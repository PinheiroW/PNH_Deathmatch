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

class DMCowboyEvent : DMEvent
{
    static private ref TStringArray QUIPS = {
        "Giddy-up, pardner!",
        "Yee-haw!",
        "Round 'em up! Get on back!"
    };
    static private ref TStringArray TOPS = {
        "Shirt_BlueCheck", "Shirt_BlueCheckBright", "Shirt_GreenCheck", "Shirt_RedCheck",
        "Shirt_WhiteCheck"
    };
    static private ref TStringArray BOTTOMS = {
        "Jeans_Black", "Jeans_Brown", "Jeans_Green", "Jeans_Blue", "Jeans_BlueDark", "Jeans_Grey"
    };
    static private ref TStringArray SHOES = {
        "CombatBoots_Beige", "CombatBoots_Brown", "HikingBoots_Brown", "JungleBoots_Brown",
        "MilitaryBoots_Brown", "Wellies_Brown", "WorkingBoots_Beige", "WorkingBoots_Brown"
    };
    static private ref TStringArray HATS = {
        "CowboyHat_Brown", "CowboyHat_black", "CowboyHat_darkBrown", "CowboyHat_green"
    };
    static private ref TStringArray BELTS = {"CivilianBelt"};
    static private ref TStringArray MASKS = {
        "BandanaMask_BlackPattern", "BandanaMask_CamoPattern", "BandanaMask_GreenPattern",
        "BandanaMask_PolkaPattern", "BandanaMask_RedPattern"
    };

    void DMCowboyEvent()
    {
        m_config = new DMEventConfig();
        m_config.name = "cowboy";
        m_config.displayName = "Cowboy round";
        m_config.greeting.title = "-=- COWBOY ROUND -=-";

        TDMOutfit outfit = new TDMOutfit();
        outfit.Insert(DMClothesConfig.FromArray(DMCowboyEvent.TOPS));
        outfit.Insert(DMClothesConfig.FromArray(DMCowboyEvent.BOTTOMS));
        outfit.Insert(DMClothesConfig.FromArray(DMCowboyEvent.SHOES));
        outfit.Insert(DMClothesConfig.FromArray(DMCowboyEvent.HATS));
        outfit.Insert(DMClothesConfig.FromArray(DMCowboyEvent.BELTS));
        outfit.Insert(DMClothesConfig.FromArray(DMCowboyEvent.MASKS));
        m_config.outfits.Insert(outfit);

        m_config.crateWeapons = null;
        m_config.crateArmor = null;
        m_config.crateOther = null;

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

    override EntityAI CreatePrimaryWeapon(HumanInventory inventory)
    {
        auto holster = inventory.CreateInInventory("ChestHolster");
        auto weapon = Weapon_Base.Cast(inventory.CreateInHands("Magnum"));
        weapon.SpawnAmmo("Ammo_357", Weapon_Base.SAMF_DEFAULT);
        inventory.CreateInInventory("Ammo_357");
        inventory.CreateInInventory("Ammo_357");
        inventory.CreateInInventory("Ammo_357");
        return weapon;
    }

    override EntityAI CreateSecondaryWeapon(HumanInventory inventory)
    {
        auto weapon = Weapon_Base.Cast(inventory.CreateInInventory("Repeater"));
        weapon.SpawnAmmo("Ammo_357", Weapon_Base.SAMF_DEFAULT);
        inventory.CreateInInventory("Ammo_357");
        inventory.CreateInInventory("Ammo_357");
        inventory.CreateInInventory("Ammo_357");
        return weapon;
    }

    override string GetRandomKnife()
    {
        return "HuntingKnife";
    }

    override void EquipInfected(EntityAI infected)
    {
        GameInventory inventory = infected.GetInventory();
        inventory.CreateAttachment(DMCowboyEvent.HATS.GetRandomElement());
    }
};

// vim:ft=enforce
