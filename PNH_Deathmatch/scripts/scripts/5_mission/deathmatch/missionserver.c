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

modded class MissionServer
{
    autoptr Deathmatch m_deathmatch;

    override void OnInit()
    {
        super.OnInit();

        m_deathmatch = new Deathmatch();
    }

    override void OnEvent(EventType eventTypeId, Param params)
    {
        super.OnEvent(eventTypeId, params);

        if (m_deathmatch)
        {
            m_deathmatch.OnEvent(eventTypeId, params);
        }
    }

    override PlayerBase CreateCharacter(
        PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
    {
        pos = m_deathmatch.GetRandomPlayerSpawnPosition();

        m_player = PlayerBase.Cast(g_Game.CreateObject(characterName, pos));

        g_Game.SelectPlayer(identity, m_player);

        return m_player;
    }

    override void EquipCharacter(MenuDefaultCharacterData char_data)
    {
        m_deathmatch.StartingEquipSetup(m_player);

        StartingEquipSetup(m_player, true);
    }

    override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
    {
        string characterType = g_Game.CreateRandomPlayer();

        auto player = identity.GetPlayer();
        if (player && player.IsAlive())
        {
            PrintFormat(
                "WARNING: Identity %1 already connected to alive player %2", identity, player);
            return m_player;
        }

        if (CreateCharacter(identity, pos, ctx, characterType))
        {
			EquipCharacter(new MenuDefaultCharacterData());
        }

        return m_player;
    }

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        m_deathmatch.InvokeOnConnect(player, identity);

        super.InvokeOnConnect(player, identity);
    }

    override void PlayerDisconnected(PlayerBase player, PlayerIdentity identity, string uid)
    {
        m_deathmatch.PlayerDisconnected(identity, uid);

        super.PlayerDisconnected(player, identity, uid);
    }

    override void HandleBody(PlayerBase player)
    {
        m_deathmatch.HandleBody(player);
    }

    override bool InsertCorpse(Man player)
    {
        m_deathmatch.OnPlayerDeath(player);

        return super.InsertCorpse(player);
    }

    override void DMRespawnPlayer(PlayerBase player)
    {
        if (!player)
        {
            return;
        }

        auto identity = player.GetIdentity();
        if (!identity)
        {
            return;
        }

        auto ctx = new ScriptReadWriteContext();
        auto data = new MenuDefaultCharacterData();
        data.SerializeCharacterData(ctx.GetWriteContext());

        auto params = new ClientNewEventParams(identity, vector.Zero, ctx.GetReadContext());
        OnEvent(ClientNewEventTypeID, params);
    }
};

// vim:ft=enforce
