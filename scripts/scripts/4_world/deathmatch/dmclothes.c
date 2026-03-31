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

class DMClothes
{
    static private ref TStringArray ChristmasTops = {
        "Blouse_Green", "HikingJacket_Green", "HikingJacket_Red", "Hoodie_Green", "Hoodie_Red",
        "JumpsuitJacket_Green", "JumpsuitJacket_Red", "MedicalScrubsShirt_Green",
        "MiniDress_GreenChecker", "MiniDress_RedChecker", "ParamedicJacket_Crimson",
        "ParamedicJacket_Green", "QuiltedJacket_Green", "QuiltedJacket_Red", "Raincoat_Green",
        "Raincoat_Red", "Shirt_GreenCheck", "Shirt_RedCheck", "Sweater_Green", "Sweater_Red",
        "TShirt_Green", "TShirt_Red", "TShirt_RedBlackStripes", "TrackSuitJacket_Green",
        "TrackSuitJacket_Red", "WoolCoat_Green", "WoolCoat_Red", "WoolCoat_RedCheck"
    };
    static private ref TStringArray ChristmasBottoms = {
        "Breeches_Green", "Breeches_Red", "CanvasPantsMidi_Red", "CanvasPants_Red",
        "CargoPants_Green", "Jeans_Green", "JumpsuitPants_Green", "JumpsuitPants_Red",
        "MedicalScrubsPants_Green", "ParamedicPants_Crimson", "ParamedicPants_Green",
        "ShortJeans_Green", "ShortJeans_Red", "Skirt_Red", "TrackSuitPants_Green",
        "TrackSuitPants_Red"
    };
    static private ref TStringArray ChristmasShoes = {
        "AthleticShoes_Green", "JoggingShoes_Red", "JungleBoots_Green", "MilitaryBoots_Redpunk",
        "Sneakers_Green", "Sneakers_Red", "WorkingBoots_Green"
    };
    static private ref TStringArray ChristmasVests = {
        "UKAssVest_Camo", "UKAssVest_Olive", "SmershVest"
    };
    static private ref TStringArray ChristmasGlasses = {
        "TacticalGoggles", "ThickFramesGlasses", "ThinFramesGlasses"
    };

    private bool m_christmas;
    private TDMOutfitArray m_outfits;

    void DMClothes(DeathmatchSettings settings)
    {
        m_christmas = settings.christmas;
        m_outfits = settings.outfits;
    }

    EntityAI EquipPlayerClothes(PlayerBase player, DMEvent dmEvent)
    {
        HumanInventory inventory = player.GetHumanInventory();

        if (dmEvent)
        {
            this.EquipOutfit(inventory, dmEvent.GetRandomOutfit());
        }
        else if (m_christmas)
        {
            this.EquipChristmas(inventory);
        }
        else
        {
            this.EquipNormal(inventory);
        }

        EntityAI belt = inventory.FindAttachment(InventorySlots.GetSlotIdFromString("Hips"));
        if (belt)
        {
            return belt.GetInventory().CreateAttachment("NylonKnifeSheath");
        }

        return null;
    }

    private void EquipNormal(HumanInventory inventory)
    {
        this.EquipOutfit(inventory, m_outfits.GetRandomElement());
    }

    private void EquipOutfit(HumanInventory inventory, TDMOutfit outfit)
    {
        if (!outfit) return;

        foreach (DMClothesConfig config : outfit)
        {
            if (config.chance < 1 || Math.RandomInt(0, 100) < config.chance)
            {
                inventory.CreateAttachment(config.choices.GetRandomElement());
            }
        }
    }

    private void EquipChristmas(HumanInventory inventory)
    {
        inventory.CreateAttachment(ChristmasTops.GetRandomElement());
        inventory.CreateAttachment(ChristmasBottoms.GetRandomElement());
        inventory.CreateAttachment(ChristmasShoes.GetRandomElement());
        inventory.CreateAttachment(ChristmasVests.GetRandomElement());
        inventory.CreateAttachment("SantasBeard");
        inventory.CreateAttachment("SantasHat");
        inventory.CreateAttachment("CivilianBelt");
        if (Math.RandomInt(0, 2))
        {
            inventory.CreateAttachment(ChristmasGlasses.GetRandomElement());
        }
    }
}

// vim:ft=enforce
