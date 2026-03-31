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

modded class FuelStation
{
    override void EEKilled(Object killer)
    {
        super.EEKilled(killer);

        EntityAI entity = EntityAI.Cast(killer);
        if (entity)
        {
            DMBlamedPlayer.Set(this, entity.GetHierarchyRootPlayer());
        }
    }
};

// vim:ft=enforce
