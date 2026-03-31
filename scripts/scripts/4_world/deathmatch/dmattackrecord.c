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

class DMAttackRecord
{
    private static const string FISTS = "fists";

    private string m_attackerId;
    private string m_weaponName;
    private int m_distance;
    private bool m_unreliableDistance;
    private bool m_attackerWasInfected;
    private bool m_selfInflicted;

    static DMAttackRecord FromAttackHit(Man victim, Man attacker, EntityAI weapon)
    {
        if (!victim || !victim.GetIdentity()) return NULL;

        DMAttackRecord result = new DMAttackRecord();
        string victimId = victim.GetIdentity().GetId();

        if (attacker && attacker.GetIdentity())
        {
            result.m_attackerId = attacker.GetIdentity().GetId();

            result.m_distance = vector.Distance(victim.GetPosition(), attacker.GetPosition());
        }

        if (weapon)
        {
            if (DayZInfected.Cast(weapon))
            {
                result.m_attackerWasInfected = true;
            }

            string weaponName = weapon.GetDisplayName();

            if (Man.Cast(weapon))
            {
                result.m_weaponName = FISTS;
            }
            // Work-around for https://feedback.bistudio.com/T158596
            else if (FuelStation.Cast(weapon) && weaponName == "House")
            {
                result.m_weaponName = "a fuel pump";
            }
            else
            {
                result.m_weaponName = weaponName;
            }

            // If the attacker is null or the same as the victim, check for trap, grenade, etc.
            if (!attacker || (victim == attacker))
            {
                string blamed = DMBlamedPlayer.Get(weapon);

                if (blamed != "")
                {
                    result.m_attackerId = blamed;
                    result.m_unreliableDistance = true;
                }
            }
        }

        if (victimId == result.m_attackerId)
        {
            result.m_selfInflicted = true;
        }

        return result;
    }

    static DMAttackRecord FromAttackHit(Man victim, EntityAI source)
    {
        if (!source)
        {
            return null;
        }

        return FromAttackHit(victim, source.GetHierarchyRootPlayer(), source);
    }

    static DMAttackRecord FromKillerData(Man victim, KillerData data)
    {
        return FromAttackHit(victim, Man.Cast(data.m_Killer), data.m_MurderWeapon);
    }

    private void DMAttackRecord()
    {
    }

    bool HaveAttacker()
    {
        return m_attackerId != "";
    }

    bool HaveWeapon()
    {
        return m_weaponName != "";
    }

    bool HaveNonFistsWeapon()
    {
        return m_weaponName != "" && m_weaponName != FISTS;
    }

    bool HaveDistance()
    {
        return !m_unreliableDistance;
    }

    bool WasByInfected()
    {
        return m_attackerWasInfected;
    }

    bool WasSelfInflicted()
    {
        return m_selfInflicted;
    }

    PlayerIdentity GetAttackerIdentity(DeathmatchSettings settings)
    {
        DMPlayer player = DMPlayer.GetByPlayerId(settings, m_attackerId);

        if (player)
        {
            return player.GetIdentity();
        }

        return null;
    }

    string GetWeaponName()
    {
        return m_weaponName;
    }

    int GetDistance()
    {
        return m_distance;
    }

    string ToHumanReadable()
    {
        return string.Format(
                "DMAttackRecord(attackerId=%1, weaponName=%2, distance=%3, ud=%4, i=%5, si=%6)",
                m_attackerId, m_weaponName, m_distance, m_unreliableDistance,
                m_attackerWasInfected, m_selfInflicted);
    }
};

// vim:ft=enforce
