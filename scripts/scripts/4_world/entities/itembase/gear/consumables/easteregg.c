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

modded class EasterEgg
{
    void DMPrepare()
    {
        m_CreatureType = "Animal_UrsusArctos";
        m_CaptureState = eCaptureState.STASIS;
        SetSynchDirty();
        SetQuantity(GetQuantityMax());
    }

	override void OnInventoryExit(Man player)
    {
        super.OnInventoryExit(player);

        DMBlamedPlayer.Set(this, player);
    }

    override void Release(vector pos)
    {
        if (g_Game.IsServer())
        {
            m_CaptureState = eCaptureState.RELEASEFX;
            m_CreatureHash = 0;
            SetSynchDirty();

            PrintFormat("%1 :: Releasing %2 at %3", this, m_CreatureType, pos.ToString(true));
            EntityAI creature = EntityAI.Cast(
                g_Game.CreateObject(m_CreatureType, pos, false, true));
            creature.SetLifetime(7200);
            m_CreatureType = "";

            string blamed = DMBlamedPlayer.Get(this);
            if (creature && blamed != "")
            {
                DMBlamedPlayer.Set(creature, blamed);
            }

            DecreaseHealth("", "", GetMaxHealth() * 0.4);
            SetQuantity(GetQuantityMin(), false);
            SetVelocity(this, CAPTURE_VELOCITY);

            if (!IsAlive())
            {
                if (m_ParCapture)
                {
                    m_ParCapture.Delete();
                }
                Delete();
            }
        }
    }
};

// vim:ft=enforce
