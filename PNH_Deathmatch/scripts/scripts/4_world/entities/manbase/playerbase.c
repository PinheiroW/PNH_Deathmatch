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

modded class PlayerBase
{
    ref Timer m_prepForCleanupTimer;
    ref DMAttackRecord m_attackRecordForUncon;

    EntityAI m_lastHitSource;

    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
    {
        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

        m_lastHitSource = source;
    }

    override void EEKilled(Object killer)
    {
        super.EEKilled(killer);

        if (!GetIdentity())
        {
            return;
        }

        if (!DMAutoRespawn.Instance().IsEnabledForID(GetIdentity().GetId()))
        {
            return;
        }

        auto mission = MissionBaseWorld.Cast(g_Game.GetMission());
        if (mission)
        {
            g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(
                mission.DMRespawnPlayer, 3000, false, this);
        }
    }

    override void OnUnconsciousStart()
    {
        m_attackRecordForUncon = DMAttackRecord.FromAttackHit(this, m_lastHitSource);

        super.OnUnconsciousStart();
    }

    override void OnUnconsciousStop(int pCurrentCommandID)
    {
        if (pCurrentCommandID != DayZPlayerConstants.COMMANDID_DEATH)
        {
            m_attackRecordForUncon = null;
        }

        super.OnUnconsciousStop(pCurrentCommandID);
    }

    void DMPrepForCleanup()
    {
        m_prepForCleanupTimer = new Timer();
        m_prepForCleanupTimer.Run(1, this, "DMFinalCleanup");
    }

    private void DMFinalCleanup()
    {
        Print("Final cleanup for " + this);
        vector position = this.GetPosition();
        position[1] = -1000;
        this.SetPosition(position);
    }

    private bool DMIsErect()  // hu hu hu
    {
        return this.IsPlayerInStance(
            DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_RAISEDERECT);
    }

    private bool DMIsSprinting()  // PlayerBase.IsSprinting appears to be jacked (T161874)
    {
        return m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_SPRINT;
    }

    override void OnTick()
    {
        super.OnTick();

        DeathmatchSettings settings = DeathmatchSettings.Instance();

        if (!settings || !settings.disableSprintLeaning)
        {
            return;
        }

        if (this.IsLeaning() && this.DMIsErect() && this.DMIsSprinting())
        {
            // Oops! You tripped and fell!
            HumanCommandMove hcm = StartCommand_Move();
            hcm = GetCommand_Move();
            if (hcm)
            {
                hcm.ForceStance(DayZPlayerConstants.STANCEIDX_PRONE);
            }
        }
    }
};

// vim:ft=enforce
