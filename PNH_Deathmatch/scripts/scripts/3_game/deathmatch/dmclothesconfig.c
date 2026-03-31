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

class DMClothesConfig
{
    int chance = 100;

    ref TStringArray choices = new TStringArray();

    static DMClothesConfig FromArray(TStringArray items)
    {
        DMClothesConfig config = new DMClothesConfig();
        config.choices.InsertArray(items);
        return config;
    }
};

typedef array<ref DMClothesConfig> TDMOutfit;
typedef array<ref TDMOutfit> TDMOutfitArray;

class DMDefaultClothesConfigs
{
    static private ref TStringArray TOPS = {
        "BDUJacket", "Blouse_Green", "BomberJacket_Black", "BomberJacket_Brown",
        "BomberJacket_Blue", "BomberJacket_Grey", "BomberJacket_Maroon", "BomberJacket_Olive",
        "BomberJacket_SkyBlue", "DenimJacket", "HikingJacket_Black", "HikingJacket_Blue",
        "HikingJacket_Red", "HikingJacket_Green", "Hoodie_Blue", "Hoodie_Black", "Hoodie_Brown",
        "Hoodie_Green", "Hoodie_Grey", "Hoodie_Red", "HuntingJacket_Autumn", "HuntingJacket_Brown",
        "HuntingJacket_Spring", "HuntingJacket_Summer", "HuntingJacket_Winter", "LabCoat",
        "M65Jacket_Black", "M65Jacket_Olive", "ParamedicJacket_Blue", "ParamedicJacket_Crimson",
        "ParamedicJacket_Green", "PrisonUniformJacket", "QuiltedJacket_Black",
        "QuiltedJacket_Green", "QuiltedJacket_Blue", "QuiltedJacket_Red", "QuiltedJacket_Grey",
        "QuiltedJacket_Orange", "QuiltedJacket_Yellow", "QuiltedJacket_Violet",
        "TacticalShirt_Grey", "TacticalShirt_Black", "TacticalShirt_Olive", "TacticalShirt_Tan",
        "WoolCoat_Black", "WoolCoat_Red", "WoolCoat_Blue", "WoolCoat_Green", "WoolCoat_Beige",
        "WoolCoat_RedCheck", "WoolCoat_BlackCheck", "WoolCoat_BlueCheck", "WoolCoat_GreyCheck",
        "WoolCoat_BrownCheck"
    };
    static private ref TStringArray BOTTOMS = {
        "BDUPants", "CargoPants_Beige", "CargoPants_Black", "CargoPants_Blue", "CargoPants_Green",
        "CargoPants_Grey", "HunterPants_Autumn", "HunterPants_Brown", "HunterPants_Spring",
        "HunterPants_Summer", "HunterPants_Winter", "Jeans_Black", "Jeans_Blue", "Jeans_Brown",
        "Jeans_Green", "Jeans_Grey", "Jeans_BlueDark", "JumpsuitPants_Blue", "JumpsuitPants_Green",
        "JumpsuitPants_Grey", "JumpsuitPants_Red", "ParamedicPants_Blue", "ParamedicPants_Crimson",
        "ParamedicPants_Green", "PrisonUniformPants", "ShortJeans_Black", "ShortJeans_Blue",
        "ShortJeans_Brown", "ShortJeans_Darkblue", "ShortJeans_Green", "ShortJeans_Red",
        "SlacksPants_Beige", "SlacksPants_Black", "SlacksPants_Blue", "SlacksPants_Brown",
        "SlacksPants_DarkGrey", "SlacksPants_Khaki", "SlacksPants_LightGrey", "SlacksPants_White"
    };
    static private ref TStringArray SHOES = {
        "AthleticShoes_Blue", "AthleticShoes_Black", "AthleticShoes_Brown", "AthleticShoes_Green",
        "AthleticShoes_Grey", "CombatBoots_Beige", "CombatBoots_Black", "CombatBoots_Brown",
        "CombatBoots_Green", "CombatBoots_Grey", "HikingBoots_Brown", "HikingBoots_Black",
        "HikingBootsLow_Black", "HikingBootsLow_Blue", "HikingBootsLow_Beige",
        "HikingBootsLow_Grey", "JoggingShoes_Black", "JoggingShoes_Blue", "JoggingShoes_Red",
        "JoggingShoes_Violet", "JoggingShoes_White", "JungleBoots_Beige", "JungleBoots_Black",
        "JungleBoots_Brown", "JungleBoots_Green", "MilitaryBoots_Beige", "MilitaryBoots_Black",
        "MilitaryBoots_Bluerock", "MilitaryBoots_Brown", "Sneakers_Black", "Sneakers_Gray",
        "Sneakers_Green", "Sneakers_Red", "Sneakers_White", "Wellies_Black", "Wellies_Brown",
        "Wellies_Green", "Wellies_Grey", "WorkingBoots_Grey", "WorkingBoots_Brown",
        "WorkingBoots_Green", "WorkingBoots_Yellow", "WorkingBoots_Beige"
    };
    static private ref TStringArray VESTS = {
        "HuntingVest", "HuntingVest_Winter", "SmershVest", "UKAssVest_Black", "UKAssVest_Camo",
        "UKAssVest_Khaki", "UKAssVest_Olive", "UKAssVest_Winter"
    };
    static private ref TStringArray HATS = {
        "BaseballCap_Beige", "BaseballCap_Black", "BaseballCap_Blue", "BaseballCap_CMMG_Black",
        "BaseballCap_CMMG_Pink", "BaseballCap_Camo", "BaseballCap_Olive", "BaseballCap_Pink",
        "BaseballCap_Red", "BeanieHat_Beige", "BeanieHat_Black", "BeanieHat_Blue",
        "BeanieHat_Brown", "BeanieHat_Green", "BeanieHat_Grey", "BeanieHat_Pink", "BeanieHat_Red",
        "BoonieHat_Black", "BoonieHat_Blue", "BoonieHat_DPM", "BoonieHat_Dubok",
        "BoonieHat_Flecktran", "BoonieHat_NavyBlue", "BoonieHat_Olive", "BoonieHat_Orange",
        "BoonieHat_Red", "BoonieHat_Tan", "BoonieHat_Winter", "CowboyHat_Brown", "CowboyHat_black",
        "CowboyHat_darkBrown", "CowboyHat_green", "FlatCap_Black", "FlatCap_Blue", "FlatCap_Red",
        "FlatCap_Brown", "FlatCap_Grey", "FlatCap_BrownCheck", "FlatCap_GreyCheck",
        "FlatCap_BlackCheck", "MedicalScrubsHat_Blue", "MedicalScrubsHat_White",
        "MedicalScrubsHat_Green", "MilitaryBeret_Red", "MilitaryBeret_UN", "MilitaryBeret_NZ",
        "MilitaryBeret_ChDKZ", "MilitaryBeret_CDF", "OfficerHat", "PrisonerCap", "RadarCap_Black",
        "RadarCap_Blue", "RadarCap_Brown", "RadarCap_Green", "RadarCap_Red", "Ushanka_Black",
        "Ushanka_Blue", "Ushanka_Green", "ZmijovkaCap_Black", "ZmijovkaCap_Blue",
        "ZmijovkaCap_Brown", "ZmijovkaCap_Green", "ZmijovkaCap_Red"
    };
    static private ref TStringArray GLASSES = {
        "AviatorGlasses", "EyeMask_Valentines", "EyeMask_NewYears", "EyeMask_Christmas",
        "EyeMask_Dead", "EyeMask_Black", "EyeMask_Blue", "EyeMask_Red", "EyeMask_Yellow",
        "DesignerGlasses", "SkiGoggles_WhiteClear", "SkiGoggles_WhiteDark",
        "SkiGoggles_BlackYellow", "SportGlasses_Black", "SportGlasses_Blue", "SportGlasses_Green",
        "SportGlasses_Orange", "ThickFramesGlasses", "ThinFramesGlasses"
    };
    static private ref TStringArray BELTS = {"CivilianBelt", "MilitaryBelt"};
    static private ref TStringArray PPE = {"NioshFaceMask", "SurgicalMask"};

    static void SetOutfits(TDMOutfitArray outfits)
    {
        outfits.Insert(DMDefaultClothesConfigs.Outfit());
    }

    private static TDMOutfit Outfit()
    {
        TDMOutfit outfit = new TDMOutfit();
        outfit.Insert(DMDefaultClothesConfigs.Tops());
        outfit.Insert(DMDefaultClothesConfigs.Bottoms());
        outfit.Insert(DMDefaultClothesConfigs.Shoes());
        outfit.Insert(DMDefaultClothesConfigs.Vests());
        outfit.Insert(DMDefaultClothesConfigs.Hats());
        outfit.Insert(DMDefaultClothesConfigs.Glasses());
        outfit.Insert(DMDefaultClothesConfigs.Belts());
        outfit.Insert(DMDefaultClothesConfigs.Masks());
        return outfit;
    }

    private static DMClothesConfig Tops()
    {
        return DMClothesConfig.FromArray(DMDefaultClothesConfigs.TOPS);
    }

    private static DMClothesConfig Bottoms()
    {
        return DMClothesConfig.FromArray(DMDefaultClothesConfigs.BOTTOMS);
    }

    static DMClothesConfig Shoes()
    {
        return DMClothesConfig.FromArray(DMDefaultClothesConfigs.SHOES);
    }

    static DMClothesConfig Vests()
    {
        return DMClothesConfig.FromArray(DMDefaultClothesConfigs.VESTS);
    }

    static DMClothesConfig Hats()
    {
        DMClothesConfig config = new DMClothesConfig();
        config.chance = 50;
        config.choices.InsertArray(DMDefaultClothesConfigs.HATS);
        return config;
    }

    static DMClothesConfig Glasses()
    {
        DMClothesConfig config = new DMClothesConfig();
        config.chance = 50;
        config.choices.InsertArray(DMDefaultClothesConfigs.GLASSES);
        return config;
    }

    static DMClothesConfig Belts()
    {
        return DMClothesConfig.FromArray(DMDefaultClothesConfigs.BELTS);
    }

    static DMClothesConfig Masks()
    {
        return DMClothesConfig.FromArray(DMDefaultClothesConfigs.PPE);
    }
}

// vim:ft=enforce
