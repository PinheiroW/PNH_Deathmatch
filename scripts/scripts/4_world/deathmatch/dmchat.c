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

class DMChatTimer
{
    private autoptr Timer m_timer = new Timer();
    private Man m_player;
    private string m_message;

    void DMChatTimer(Man player, string message, float delay)
    {
        m_player = player;
        m_message = message;
        m_timer.Run(delay, this, "Chat");
    }

    void Chat()
    {
        DMChat.OnDelayedMessagePlayer(this, m_player, m_message);
    }
}

class DMChat
{
    private static ref array<ref DMChatTimer> timers = new array<ref DMChatTimer>();

    static void MessagePlayer(DMPlayer player, string message)
    {
        DMChat.MessagePlayer(player.GetPlayer(), message);
    }

    static void MessagePlayer(Man player, string message)
    {
        PrintFormat("%1 | MESSAGE-TO | %2 | %3", DMGetTimestamp(), player, message);

        DMChat.MessagePlayerImpl(player, new Param1<string>(message));
    }

    static void MessagePlayerDelayed(DMPlayer player, string message, float delay)
    {
        DMChat.MessagePlayerDelayed(player.GetPlayer(), message, delay);
    }

    static void MessagePlayerDelayed(Man player, string message, float delay)
    {
        timers.Insert(new DMChatTimer(player, message, delay));
    }

    static void Announce(string message)
    {
        PrintFormat("%1 | ANNOUNCE | %2 ", DMGetTimestamp(), message);

        Param1<string> announceParams = new Param1<string>(message);

        array<Man> players = new array<Man>();
        g_Game.GetPlayers(players);
        foreach (Man player : players)
        {
            DMChat.MessagePlayerImpl(player, announceParams);
        }
    }

    private static void MessagePlayerImpl(Man player, Param1<string> messageParams)
    {
        g_Game.RPCSingleParam(
                player, ERPCs.RPC_USER_ACTION_MESSAGE, messageParams, true, player.GetIdentity());
    }

    static void OnDelayedMessagePlayer(DMChatTimer timer, Man player, string message)
    {
        int removeIndex = timers.Find(timer);
        if (removeIndex >= 0)
        {
            timers.Remove(removeIndex);
        }

        DMChat.MessagePlayer(player, message);
    }
};

// vim:ft=enforce
