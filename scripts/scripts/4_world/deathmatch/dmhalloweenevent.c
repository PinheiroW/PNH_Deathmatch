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

class DMHalloweenEvent : DMEvent
{
    static private ref TStringArray QUIPS = {
        "Everyone's entitled to one good scare.",
        "Be afraid...be very afraid.",
        "The time's come to play, here in my garden of shadows.",
        "Something wicked this way comes.",
        "Whatever you do, don't fall asleep.",
        "I'm the ghost with the most, babe."
    };
    static private ref TStringArray TOPS = {"M65Jacket_Black"};
    static private ref TStringArray BOTTOMS = {"CargoPants_Black"};
    static private ref TStringArray SHOES = {"CombatBoots_Black"};
    static private ref TStringArray GLOVES = {"WorkingGloves_Black"};
    static private ref TStringArray HEADGEAR = {"HockeyMask", "WitchHood_Black", "WitchHat"};
    static private ref TStringArray BLADES = {
        "Machete", "OrientalMachete", "CrudeMachete", "WoodAxe", "FangeKnife"
    };
    static private ref TStringArray WOLVES = {"Animal_CanisLupus_Grey", "Animal_CanisLupus_White"};

    private ref Timer m_wolfTimer;
    private DMArena m_arena;
    private EffectArea m_effect;

    void DMHalloweenEvent()
    {
        m_config = new DMEventConfig();
        m_config.name = "halloween";
        m_config.displayName = "Halloween round";
        m_config.greeting.title = "H A L L O W E E N";

        TDMOutfit outfit = new TDMOutfit();
        outfit.Insert(DMClothesConfig.FromArray(DMHalloweenEvent.TOPS));
        outfit.Insert(DMClothesConfig.FromArray(DMHalloweenEvent.BOTTOMS));
        outfit.Insert(DMClothesConfig.FromArray(DMHalloweenEvent.SHOES));
        outfit.Insert(DMClothesConfig.FromArray(DMHalloweenEvent.GLOVES));
        outfit.Insert(DMClothesConfig.FromArray(DMHalloweenEvent.HEADGEAR));
        m_config.outfits.Insert(outfit);

        m_config.outfits.Insert(DMHalloweenEvent.MummyOutfit());

        DMDefaultWeaponConfigs.SetPrimaryWeapons(m_config.crateWeapons);
        m_config.crateArmor.Insert({"Mosin9130_Black"});
        m_config.crateArmor.Insert({"Ammo_762x54Tracer"});
        m_config.crateArmor.Insert({"Ammo_762x54Tracer"});
        m_config.crateArmor.Insert({"Ammo_762x54Tracer"});
        m_config.crateArmor.Insert({"Ammo_762x54Tracer"});
        m_config.crateOther.Insert({"Winchester70"});
        m_config.crateOther.Insert({"Ammo_308WinTracer"});
        m_config.crateOther.Insert({"Ammo_308WinTracer"});
        m_config.crateOther.Insert({"Ammo_308WinTracer"});
        m_config.crateOther.Insert({"Ammo_308WinTracer"});

        this.SetInfectedTypes();
    }

    private void SetInfectedTypes()
    {
        TStringArray all = DMDefaultInfectedConfig.AllInfectedTypes();
        foreach (string type : all)
        {
            if (type.Contains("MilkMaidOld") || type.Contains("priest") || type.Contains("Black"))
            {
                m_config.infectedTypes.Insert(type);
            }
        }
        if (g_Game.ConfigIsExisting("CfgVehicles ZmbM_Mummy"))
        {
            m_config.infectedTypes.Insert("ZmbM_Mummy");
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

    override EntityAI CreatePrimaryWeapon(HumanInventory inventory)
    {
        return inventory.CreateInHands(DMHalloweenEvent.BLADES.GetRandomElement());
    }

    override EntityAI CreateSecondaryWeapon(HumanInventory inventory)
    {
        auto mp5 = Weapon_Base.Cast(inventory.CreateInInventory("MP5K"));
        mp5.SpawnAmmo("Mag_MP5_15Rnd", Weapon_Base.SAMF_DEFAULT);
        mp5.GetInventory().CreateAttachment("MP5k_StockBttstck");
        mp5.GetInventory().CreateAttachment("MP5_PlasticHndgrd");
        return mp5;
    }

    override string GetRandomKnife()
    {
        return "FlashGrenade";
    }

    override void EquipInfected(EntityAI infected)
    {
        GameInventory inventory = infected.GetInventory();
        string type = infected.GetType();
        if (type.Contains("MilkMaidOld"))
        {
            inventory.CreateAttachment("WitchHat");
        }
        else if (type.Contains("priest"))
        {
            inventory.CreateAttachment("WitchHood_Black");
        }
        else if (type != "ZmbM_Mummy")
        {
            if (Math.RandomFloat01() > 0.5)
            {
                EntityAI headtorch = inventory.CreateAttachment("Headtorch_Black");
                if (headtorch)
                {
                    headtorch.GetInventory().CreateAttachment("Battery9V");
                }
            }
            else
            {
                inventory.CreateAttachment("PumpkinHelmet");
            }
        }
    }

    static private TDMOutfit MummyOutfit()
    {
        TDMOutfit outfit = new TDMOutfit();
        outfit.Insert(DMClothesConfig.FromArray({"TorsoCover_Improvised"}));
        outfit.Insert(DMClothesConfig.FromArray({"LegsCover_Improvised"}));
        outfit.Insert(DMClothesConfig.FromArray({"FeetCover_Improvised"}));
        outfit.Insert(DMClothesConfig.FromArray({"HandsCover_Improvised"}));
        outfit.Insert(DMClothesConfig.FromArray({"HeadCover_Improvised"}));
        outfit.Insert(DMClothesConfig.FromArray({"FaceCover_Improvised"}));
        return outfit;
    }

    static private vector GiftBoxPosition(vector center, int angle)
    {
        vector vec = Vector(angle, 0, 0).AnglesToVector();
        vector position = center + (vec * 3);
        position[1] = Math.Max(position[1], g_Game.SurfaceRoadY(position[0], position[2]));
        return position;
    }

    private ItemBase SpawnCauldron(vector position)
    {
        auto cauldron = ItemBase.Cast(DMUtils.SpawnProp("Cauldron", position, DMUtils.RandomYaw()));
        cauldron.SetLifetime(7200);
        return cauldron;
    }

    override void SpawnChristmas(DMArena arena, array<Object> clutter, DeathmatchSettings settings)
    {
        TVectorArray positions = arena.GetChristmasTreePositions();

        foreach (vector pos : positions)
        {
            clutter.Insert(DMUtils.SpawnProp("Bonfire", pos + "0 3 0", DMUtils.RandomYaw()));

            GameInventory box1 = SpawnCauldron(
                DMHalloweenEvent.GiftBoxPosition(pos, 0)).GetInventory();
            box1.CreateInInventory("M67Grenade").GetInventory();
            box1.CreateInInventory("M67Grenade").GetInventory();
            box1.CreateInInventory("RGD5Grenade").GetInventory();
            box1.CreateInInventory("RGD5Grenade").GetInventory();

            GameInventory box2 = SpawnCauldron(
                    DMHalloweenEvent.GiftBoxPosition(pos, 60)).GetInventory();
            box2.CreateInInventory("Chemlight_Red");
            box2.CreateInInventory("Chemlight_Red");
            box2.CreateInInventory("Chemlight_Yellow");
            box2.CreateInInventory("Chemlight_Yellow");

            GameInventory box3 = SpawnCauldron(
                    DMHalloweenEvent.GiftBoxPosition(pos, 120)).GetInventory();
            box3.CreateInInventory("Magnum");
            box3.CreateInInventory("Ammo_357");
            box3.CreateInInventory("Ammo_357");
            box3.CreateInInventory("Ammo_357");
            box3.CreateInInventory("Ammo_357");

            GameInventory box4 = SpawnCauldron(
                    DMHalloweenEvent.GiftBoxPosition(pos, 180)).GetInventory();
            box4.CreateInInventory("LandMineTrap");
            box4.CreateInInventory("LandMineTrap");

            GameInventory box5 = SpawnCauldron(
                    DMHalloweenEvent.GiftBoxPosition(pos, 240)).GetInventory();
            box5.CreateInInventory("FlashGrenade");
            box5.CreateInInventory("FlashGrenade");
            box5.CreateInInventory("FlashGrenade");
            box5.CreateInInventory("FlashGrenade");

            GameInventory box6 = SpawnCauldron(
                    DMHalloweenEvent.GiftBoxPosition(pos, 300)).GetInventory();
            box6.CreateInInventory("Magnum");
            box6.CreateInInventory("Ammo_357");
            box6.CreateInInventory("Ammo_357");
            box6.CreateInInventory("Ammo_357");
            box6.CreateInInventory("Ammo_357");
        }
    }

    private void SpawnWolf()
    {
        if (DMUtils.GetPlayerCount() > 0)
        {
            vector position = m_arena.GetRandomPlayerSpawnPosition();
            string type = WOLVES.GetRandomElement();

            Object wolf = g_Game.CreateObjectEx(
                type, position, ECE_INITAI | ECE_PLACE_ON_SURFACE);
            wolf.SetPosition(position);

            PrintFormat("Spawned %1 at %2", type, position.ToString(true));
        }
    }

    override void OnRoundStart(DMArena arena)
    {
        m_arena = arena;

        m_wolfTimer = new Timer();
        m_wolfTimer.Run(60, this, "SpawnWolf", null, true);

        if (arena.GetRadius() >= 200)
        {
            vector center = arena.GetCenter();
            EffectAreaParams params = new EffectAreaParams();
            params.m_ParamTriggerType = "SpookyTrigger";
            params.m_ParamRadius = arena.GetRadius() / 2;
            params.m_ParamPosHeight = 2;
            params.m_ParamNegHeight = 4;
            params.m_ParamInnerRings = 2;
            params.m_ParamInnerSpace = 2;
            params.m_ParamOuterSpace = 40;
            params.m_ParamOuterOffset = 0;
            params.m_ParamPartId = ParticleList.SPOOKY_MIST;
            params.m_ParamAroundPartId = -1;
            params.m_ParamTinyPartId = -1;
            params.m_ParamPpeRequesterType = "";
            m_effect = SpookyArea.Cast(
                g_Game.CreateObjectEx(
                    "SpookyArea",
                    Vector(
                        center[0],
                        Math.Max(0, g_Game.SurfaceRoadY(center[0], center[2])),
                        center[2]),
                    ECE_PLACE_ON_SURFACE));
            m_effect.SetupZoneData(params);
        }
    }

    override void OnRoundEnd(DMArena arena)
    {
        g_Game.ObjectDelete(m_effect);
        m_effect = null;

        if (m_wolfTimer)
        {
            m_wolfTimer.Stop();
        }

        m_arena = null;
    }
};

// vim:ft=enforce
