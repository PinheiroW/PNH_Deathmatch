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

class DMToxicEvent : DMEvent
{
    static private ref TStringArray TOPS = {"NBCJacketGray", "NBCJacketYellow"};
    static private ref TStringArray BOTTOMS = {"NBCPantsGray", "NBCPantsYellow"};
    static private ref TStringArray SHOES = {"NBCBootsGray", "NBCBootsYellow"};
    static private ref TStringArray GLOVES = {"NBCGlovesGray", "NBCGlovesYellow"};
    static private ref TStringArray HEADGEAR = {"NBCHoodGray", "NBCHoodYellow"};
    static private ref TStringArray MASKS = {"GP5GasMask", "GasMask"};

    private EffectArea m_effect;

    void DMToxicEvent()
    {
        m_config = new DMEventConfig();
        m_config.name = "toxic";
        m_config.displayName = "Toxic round";
        m_config.greeting.title = "### TOXIC ROUND ###";
        m_config.greeting.details = "*cough* *cough* *wheeze*";

        m_config.outfits.Insert(DMToxicEvent.RandomNBCOutfit());

        DMDefaultWeaponConfigs.SetPrimaryWeapons(m_config.primaryWeapons);
        DMDefaultWeaponConfigs.SetSecondaryWeapons(m_config.secondaryWeapons);
        DMDefaultWeaponConfigs.SetKnives(m_config.knives);

        DMDefaultWeaponConfigs.SetCrateWeapons(m_config.crateWeapons);
        DMDefaultWeaponConfigs.SetCrateArmor(m_config.crateArmor);
        DMDefaultWeaponConfigs.SetCrateOther(m_config.crateOther);

        DMToxicEvent.SetInfectedTypes(m_config.infectedTypes);
    }

    override void EquipPlayer(PlayerBase player, HumanInventory inventory)
    {
        EntityAI mask = inventory.FindAttachment(InventorySlots.GetSlotIdFromString("Mask"));
        mask.GetInventory().CreateAttachment("GasMask_Filter");
    }

    override void OnRoundStart(DMArena arena)
    {
        vector center = arena.GetCenter();
        EffectAreaParams params = new EffectAreaParams();
        params.m_ParamTriggerType = "ContaminatedTrigger";
        params.m_ParamRadius = Math.Max(100, arena.GetRadius() / 2);
        params.m_ParamPosHeight = 2;
        params.m_ParamNegHeight = 4;
        params.m_ParamInnerRings = 2;
        params.m_ParamInnerSpace = 2;
        params.m_ParamOuterSpace = 40;
        params.m_ParamOuterOffset = 0;
        params.m_ParamPartId = ParticleList.CONTAMINATED_AREA_GAS_BIGASS;
        params.m_ParamAroundPartId = ParticleList.CONTAMINATED_AREA_GAS_AROUND;
        params.m_ParamTinyPartId = ParticleList.CONTAMINATED_AREA_GAS_TINY;
        params.m_ParamPpeRequesterType = "PPERequester_ContaminatedAreaTint";
        m_effect = EffectArea.Cast(
            g_Game.CreateObjectEx(
                "ContaminatedArea_Static",
                Vector(
                    center[0],
                    Math.Max(0, g_Game.SurfaceRoadY(center[0], center[2])),
                    center[2]),
                ECE_PLACE_ON_SURFACE));
        m_effect.SetupZoneData(params);
    }

    override void OnRoundEnd(DMArena arena)
    {
        g_Game.ObjectDelete(m_effect);
        m_effect = null;
    }

    private static TDMOutfit RandomNBCOutfit()
    {
        TDMOutfit outfit = new TDMOutfit();
        outfit.Insert(DMClothesConfig.FromArray(DMToxicEvent.TOPS));
        outfit.Insert(DMClothesConfig.FromArray(DMToxicEvent.BOTTOMS));
        outfit.Insert(DMClothesConfig.FromArray(DMToxicEvent.SHOES));
        outfit.Insert(DMClothesConfig.FromArray(DMToxicEvent.GLOVES));
        outfit.Insert(DMClothesConfig.FromArray(DMToxicEvent.HEADGEAR));
        outfit.Insert(DMClothesConfig.FromArray(DMToxicEvent.MASKS));
        outfit.Insert(DMDefaultClothesConfigs.Vests());
        outfit.Insert(DMDefaultClothesConfigs.Belts());
        return outfit;
    }

    private static void SetInfectedTypes(TStringArray infected)
    {
        TStringArray all = DMDefaultInfectedConfig.AllInfectedTypes();
        foreach (string type : all)
        {
            if (type.Contains("NBC"))
            {
                infected.Insert(type);
            }
        }
    }
};

// vim:ft=enforce
