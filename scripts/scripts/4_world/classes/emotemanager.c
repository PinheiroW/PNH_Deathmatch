#ifdef DEBUG_DEATHMATCH_ARENA
modded class EmoteManager
{
    private static DMArenaRotation s_rotation;
    private string m_lastTestSpawnArena;
    private int m_lastTestSpawnIndex = -1;
    private int m_positionType = 0;

    static void SetArenaRotation(DMArenaRotation rotation)
    {
        s_rotation = rotation;
    }

    override bool PlayEmote(int id)
    {
        bool result = super.PlayEmote(id);

        if (!result) {
            return result;
        }

        DMArena arena = s_rotation.CurrentArena();

        if (id == EmoteConstants.ID_EMOTE_GREETING)
        {
            ServerRequestEmoteCancel();

            if (arena.GetName() == m_lastTestSpawnArena)
            {
                m_lastTestSpawnIndex++;
            }
            else
            {
                m_lastTestSpawnArena = arena.GetName();
                m_lastTestSpawnIndex = 0;
            }

            MovePlayer(arena);
        }
        else if (id == EmoteConstants.ID_EMOTE_SOS)
        {
            ServerRequestEmoteCancel();

            MovePlayer(arena);
        }
        else if (id == EmoteConstants.ID_EMOTE_HEART)
        {
            ServerRequestEmoteCancel();

            vector position = m_Player.GetPosition();

            TStringArray pos = new TStringArray();
            position.ToString(false).Split(" ", pos);

            PrintFormat("        [%1, %2, %3],", pos[0], pos[1], pos[2]);

            NotificationSystem.SendNotificationToPlayerIdentityExtended(
                NULL, 1, "Recorded position", position.ToString(false));
        }
        else if (id == EmoteConstants.ID_EMOTE_TAUNT)
        {
            ServerRequestEmoteCancel();

            m_positionType++;

            if (m_positionType > 3)
            {
                m_positionType = 0;
            }

            NotificationSystem.SendNotificationToPlayerIdentityExtended(
                NULL, 1, string.Format("Switched to position type %1", m_positionType));

            m_lastTestSpawnArena = arena.GetName();
            m_lastTestSpawnIndex = 0;

            MovePlayer(arena);
        }

        return result;
    }

    private TVectorArray GetPositions(DMArena arena)
    {
        switch (m_positionType)
        {
            case 0:
                return arena.GetPlayerSpawnPositions();
                break;
            case 1:
                return arena.GetCratePositions();
                break;
            case 2:
                return arena.GetInfectedPositions();
                break;
            case 3:
                return arena.GetChristmasTreePositions();
                break;
        }

        return new TVectorArray();
    }

    private void MovePlayer(DMArena arena)
    {
        TVectorArray positions = GetPositions(arena);

        if (m_lastTestSpawnIndex >= positions.Count())
        {
            m_lastTestSpawnIndex = 0;
        }

        vector position = positions[m_lastTestSpawnIndex];

        m_Player.SetPosition(position);
        arena.SetDirectionToCenter(m_Player);

        DMChat.MessagePlayer(
            m_Player,
            string.Format(
                "Spawn position %1/%2: %3",
                m_lastTestSpawnIndex, positions.Count(), position.ToString(false)));
    }
};
#endif

// vim:ft=enforce
