/* Copyright (c) 2022-2025 The Crimson Zamboni
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

modded class ActionTriggerRemotely
{
    override void OnExecuteServer(ActionData action_data)
    {
        auto rdt = RemoteDetonatorTrigger.Cast(action_data.m_MainItem);
        if (rdt && rdt.IsConnected())
        {
            auto controlledDevice = ItemBase.Cast(rdt.GetControlledDevice());
            if (controlledDevice && !controlledDevice.IsRuined())
            {
                DMBlamedPlayer.Set(controlledDevice, action_data.m_Player);
            }
        }

        super.OnExecuteServer(action_data);
    }
};

// vim:ft=enforce
