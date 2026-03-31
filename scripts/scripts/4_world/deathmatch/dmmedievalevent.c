/* Copyright (c) 2023-2025 The Crimson Zamboni
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

class DMMedievalEvent : DMEvent
{
    static private ref TStringArray QUIPS = {
        "Huzzah!",
        "'Tis but a scratch!",
        "Run away!"
    };
    static private ref TStringArray GLOVES = {
        "WoolGloves_Green",
        "WoolGloves_Black",
        "WoolGloves_Tan",
        "WoolGloves_White",
        "WoolGlovesFingerless_Green",
        "WoolGlovesFingerless_Black",
        "WoolGlovesFingerless_Tan"
    };
    private static const ref TStringArray CROSSBOW = {
        "Crossbow", "Crossbow_Autumn", "Crossbow_Black", "Crossbow_Summer", "Crossbow_Wood"
    };
    private static const ref TStringArray MELEE = {"Sword", "Mace"};
    private static const ref TStringArray HEADGEAR = {"Chainmail_Coif", "GreatHelm", "NorseHelm"};

    void DMMedievalEvent()
    {
        m_config = new DMEventConfig();
        m_config.name = "medieval";
        m_config.displayName = "Medieval round";
        m_config.greeting.title = "MEDIEVAL ROUND";

        TDMOutfit outfit = new TDMOutfit();
        outfit.Insert(DMClothesConfig.FromArray({"Chainmail_Coif"}));
        outfit.Insert(DMClothesConfig.FromArray({"Chainmail"}));
        outfit.Insert(DMClothesConfig.FromArray({"Chainmail_Leggings"}));
        outfit.Insert(DMClothesConfig.FromArray({"MedievalBoots"}));
        outfit.Insert(DMClothesConfig.FromArray({"MilitaryBelt"}));
        outfit.Insert(DMClothesConfig.FromArray(GLOVES));
        m_config.outfits.Insert(outfit);

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
        auto weapon = Weapon_Base.Cast(inventory.CreateInHands(CROSSBOW.GetRandomElement()));
        weapon.SpawnAmmo("Ammo_HuntingBolt", Weapon_Base.SAMF_DEFAULT);
        inventory.CreateInInventory("Ammo_HuntingBolt");
        inventory.CreateInInventory("Ammo_HuntingBolt");
        inventory.CreateInInventory("Ammo_HuntingBolt");
        inventory.CreateInInventory("Ammo_HuntingBolt");
        return weapon;
    }

    override EntityAI CreateSecondaryWeapon(HumanInventory inventory)
    {
        return inventory.CreateInInventory(MELEE.GetRandomElement());
    }

    override string GetRandomKnife()
    {
        return "HuntingKnife";
    }

    override bool HaveWeaponCrate()
    {
        return true;
    }

    override void FillWeaponCrate(GameInventory inventory)
    {
        FillCrate(inventory);
    }

    override bool HaveArmorCrate()
    {
        return true;
    }

    override void FillArmorCrate(GameInventory inventory)
    {
        inventory.CreateInInventory("ChestPlate");
        inventory.CreateInInventory("NorseHelm");
        inventory.CreateInInventory("GreatHelm");
    }

    override bool HaveOtherCrate()
    {
        return true;
    }

    override void FillOtherCrate(GameInventory inventory)
    {
        FillCrate(inventory);
    }

    override void EquipInfected(EntityAI infected)
    {
        if (Math.RandomFloat01() < 0.1)
        {
            infected.GetInventory().CreateAttachment("ChestPlate");
        }

        if (Math.RandomFloat01() < 0.1)
        {
            infected.GetInventory().CreateAttachment(HEADGEAR.GetRandomElement());
        }
    }

    private static void FillCrate(GameInventory inventory)
    {
        inventory.CreateInInventory("BearTrap");
        inventory.CreateInInventory("ACOGOptic");
        for (int i = 0; i < 14; i++) inventory.CreateInInventory("Ammo_HuntingBolt");
    }
};

// vim:ft=enforce
