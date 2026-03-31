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

class DMBlamedPlayer
{
    private static ref map<EntityAI, string> s_blames = new map<EntityAI, string>();

    static void Clear()
    {
        s_blames.Clear();
    }

    static void Set(EntityAI entity, Man man)
    {
        if (man && man.GetIdentity())
        {
            s_blames.Set(entity, man.GetIdentity().GetId());
        }
        else
        {
            DMBlamedPlayer.Remove(entity);
        }
    }

    static void Set(EntityAI entity, string id)
    {
        if (id != "")
        {
            s_blames.Set(entity, id);
        }
        else
        {
            DMBlamedPlayer.Remove(entity);
        }
    }

    static void Remove(EntityAI entity)
    {
        s_blames.Remove(entity);
    }

    static string Get(EntityAI entity)
    {
        return s_blames.Get(entity);
    }
};

// vim:ft=enforce
