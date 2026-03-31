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

class DMWeapons
{
    private DeathmatchSettings m_settings;

    void DMWeapons(DeathmatchSettings settings)
    {
        m_settings = settings;
    }

    void EquipPlayerWeapons(PlayerBase player, EntityAI sheath, DMEvent dmEvent)
    {
        HumanInventory inventory = player.GetHumanInventory();
        EntityAI primary = this.EquipPrimaryWeapon(inventory, dmEvent);
        EntityAI secondary = this.EquipSecondaryWeapon(inventory, dmEvent);
        EntityAI melee = this.EquipKnife(inventory, sheath, dmEvent);

        player.SetQuickBarEntityShortcut(primary, 0);
        player.SetQuickBarEntityShortcut(secondary, 1);
        player.SetQuickBarEntityShortcut(melee, 2);
    }

    private EntityAI EquipPrimaryWeapon(HumanInventory inventory, DMEvent dmEvent)
    {
        if (dmEvent)
        {
            return dmEvent.CreatePrimaryWeapon(inventory);
        }
        else
        {
            DMWeaponConfig config = m_settings.primaryWeapons.GetRandomElement();
            EntityAI weapon = inventory.CreateInHands(config.variants.GetRandomElement());
            return DMWeapons.EquipWeaponAccessories(inventory, config, weapon);
        }
    }

    private EntityAI EquipSecondaryWeapon(HumanInventory inventory, DMEvent dmEvent)
    {
        if (dmEvent)
        {
            return dmEvent.CreateSecondaryWeapon(inventory);
        }
        else
        {
            DMWeaponConfig config = m_settings.secondaryWeapons.GetRandomElement();
            EntityAI weapon = inventory.CreateInInventory(config.variants.GetRandomElement());
            return DMWeapons.EquipWeaponAccessories(inventory, config, weapon);
        }
    }

    static EntityAI EquipWeaponAccessories(
            GameInventory inventory, DMWeaponConfig config, EntityAI weapon)
    {
        if (weapon)
        {
            GameInventory weaponInventory = weapon.GetInventory();

            foreach (TStringArray attachments : config.attachments)
            {
                EntityAI attachment = DMWeapons.CreateAttachment(
                        weaponInventory, attachments.GetRandomElement());

                if (attachment != null)
                {
                    // Connect battery if attachment takes one. Usually only applies to optics.
                    attachment.GetInventory().CreateAttachment("Battery9V");
                }
            }

            bool triedToSpawnAmmo = false;

            foreach (string accessory : config.accessories)
            {
                if (!triedToSpawnAmmo)
                {
                    triedToSpawnAmmo = true;

                    auto wb = Weapon_Base.Cast(weapon);
                    if (wb && wb.SpawnAmmo(accessory, Weapon_Base.SAMF_DEFAULT))
                    {
                        if (!g_Game.IsKindOf(accessory, "Ammunition_Base"))
                        {
                            continue;
                        }
                    }
                }

                inventory.CreateInInventory(accessory);
            }
        }

        return weapon;
    }

    private static EntityAI CreateAttachment(GameInventory inventory, string attachment)
    {
        if (attachment == "") return null;

        return inventory.CreateAttachment(attachment);
    }

    private EntityAI EquipKnife(GameInventory inventory, EntityAI sheath, DMEvent dmEvent)
    {
        string knife;

        if (dmEvent)
        {
            knife = dmEvent.GetRandomKnife();
        }
        else
        {
            knife = m_settings.knives.GetRandomElement();
        }

        if (sheath)
        {
            return sheath.GetInventory().CreateAttachment(knife);
        }

        return inventory.CreateInInventory(knife);
    }
}

// vim:ft=enforce
