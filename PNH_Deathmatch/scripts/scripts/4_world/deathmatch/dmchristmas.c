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

class DMChristmas
{
    private static const ref TStringArray LARGE = {
        "GiftBox_Large_1", "GiftBox_Large_2", "GiftBox_Large_3", "GiftBox_Large_4"
    };
    private static const ref TStringArray MEDIUM = {
        "GiftBox_Medium_1", "GiftBox_Medium_2", "GiftBox_Medium_3", "GiftBox_Medium_4"
    };
    private static const ref TStringArray SMALL = {
        "GiftBox_Small_1", "GiftBox_Small_2", "GiftBox_Small_3", "GiftBox_Small_4"
    };

    static array<Object> Spawn(DMArena arena, array<Object> clutter)
    {
        arena.SpawnChristmasTrees(clutter);

        TVectorArray positions = arena.GetChristmasTreePositions();
        foreach (vector position : positions)
        {
            DMChristmas.SpawnGiftsAt(position);
        }

        return clutter;
    }

    static void SpawnGiftsAt(vector position)
    {
        PrintFormat("Spawning Christmas gifts at %1", position.ToString());

        GameInventory box1 = DMChristmas.SpawnGiftBox(LARGE, position, 0).GetInventory();
        GameInventory megaphone = box1.CreateInInventory("Megaphone").GetInventory();
        megaphone.CreateAttachment("Battery9V");

        GameInventory box2 = DMChristmas.SpawnGiftBox(MEDIUM, position, 36).GetInventory();
        box2.CreateInInventory("AK_Suppressor");

        GameInventory box3 = DMChristmas.SpawnGiftBox(MEDIUM, position, 72).GetInventory();
        box3.CreateInInventory("M4_Suppressor");

        GameInventory box4 = DMChristmas.SpawnGiftBox(MEDIUM, position, 108).GetInventory();
        box4.CreateInInventory("PistolSuppressor");

        GameInventory box5 = DMChristmas.SpawnGiftBox(MEDIUM, position, 144).GetInventory();
        box5.CreateInInventory("ImprovisedSuppressor");

        GameInventory box6 = DMChristmas.SpawnGiftBox(LARGE, position, 180).GetInventory();
        GameInventory deagle = box6.CreateInInventory("Deagle_Gold").GetInventory();
        deagle.CreateAttachment("PistolOptic");
        deagle.CreateAttachment("PistolSuppressor");
        box6.CreateInInventory("Mag_Deagle_9rnd");
        box6.CreateInInventory("Mag_Deagle_9rnd");
        box6.CreateInInventory("Mag_Deagle_9rnd");
        box6.CreateInInventory("Mag_Deagle_9rnd");

        GameInventory box7 = DMChristmas.SpawnGiftBox(SMALL, position, 216).GetInventory();
        box7.CreateInInventory("M67Grenade");

        GameInventory box8 = DMChristmas.SpawnGiftBox(SMALL, position, 252).GetInventory();
        box8.CreateInInventory("RGD5Grenade");

        GameInventory box9 = DMChristmas.SpawnGiftBox(SMALL, position, 288).GetInventory();
        box9.CreateInInventory("M18SmokeGrenade_Red");

        GameInventory box10 = DMChristmas.SpawnGiftBox(SMALL, position, 324).GetInventory();
        box10.CreateInInventory("M18SmokeGrenade_Green");
    }

    private static ItemBase SpawnGiftBox(TStringArray boxTypes, vector center, int angle)
    {
        ItemBase box = ItemBase.Cast(
            DMUtils.SpawnProp(
                boxTypes.GetRandomElement(),
                DMChristmas.GiftBoxPosition(center, angle),
                DMUtils.RandomYaw()));

        box.SetLifetime(7200);

        return box;
    }

    private static vector GiftBoxPosition(vector center, int angle)
    {
        vector vec = Vector(angle, 0, 0).AnglesToVector();
        vector position = center + (vec * 5);
        position[1] = Math.Max(position[1], g_Game.SurfaceRoadY(position[0], position[2]));
        return position;
    }
};

// vim:ft=enforce
