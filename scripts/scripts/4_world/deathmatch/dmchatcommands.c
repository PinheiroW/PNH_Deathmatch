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

class DMChatCommand
{
    private Class m_target;
    private string m_funcName;
    private bool m_adminOnly;
    private string m_help;

    void DMChatCommand(Class target, string funcName, bool adminOnly, string help)
    {
        m_target = target;
        m_funcName = funcName;
        m_adminOnly = adminOnly;
        m_help = help;
    }

    bool IsAdminOnly()
    {
        return m_adminOnly;
    }

    string GetHelp()
    {
        return m_help;
    }

    void Call(DMPlayer source, string args)
    {
        g_Game.GameScript.CallFunctionParams(
                m_target, m_funcName, NULL, new Param2<DMPlayer, string>(source, args));
    }
};

class DMChatCommands
{
    private ref map<string, ref DMChatCommand> m_commands = new map<string, ref DMChatCommand>();
    string leader;

    void DMChatCommands(DeathmatchSettings settings)
    {
        this.leader = settings.commandCharacter;

        this.AddCommand("help", this, "OnHelp", ":: Show available commands");
    }

    void AddCommand(string command, Class target, string funcName, string help)
    {
        command.ToUpper();

        m_commands.Set(command, new DMChatCommand(target, funcName, false, help));
    }

    void AddAdminCommand(string command, Class target, string funcName, string help)
    {
        command.ToUpper();

        m_commands.Set(command, new DMChatCommand(target, funcName, true, help));
    }

    string ParseCommand(string input, out string args)
    {
        if (input.Length() < 2) return "";
        if (input.Substring(0, 1) != leader) return "";

        int space = input.IndexOf(" ");
        if (space == -1) space = input.Length();
        if (space < 2) return "";

        string command = input.Substring(1, space - 1);
        if (space < input.Length())
        {
            args = input.Substring(space + 1, input.Length() - space - 1);
        }
        else
        {
            args = "";
        }

        return command;
    }

    void OnCommand(DMPlayer source, string command, string args)
    {
        PrintFormat(
                "%1 | COMMAND-FROM | %2 (%3) | %4%5 %6",
                DMGetTimestamp(), source.GetName(), source.GetIdentity().GetId(), this.leader,
                command, args);

        command.ToUpper();

        DMChatCommand cmd;
        if (m_commands.Find(command, cmd))
        {
            if (!cmd.IsAdminOnly() || source.IsAdmin())
            {
                cmd.Call(source, args);
                return;
            }
        }

        DMChat.MessagePlayer(source, string.Format("Invalid command: %1", command));
    }

    private void OnHelp(DMPlayer source, string args)
    {
        DMChat.MessagePlayer(source, "Available commands:");

        float delay = 0;
        for (int i = 0; i < m_commands.Count(); i++)
        {
            DMChatCommand cmd = m_commands.GetElement(i);
            if (!cmd.IsAdminOnly() || source.IsAdmin())
            {
                string command = m_commands.GetKey(i);
                command.ToLower();

                delay += 1;
                DMChat.MessagePlayerDelayed(
                        source, string.Format(
                            " %1%2 %3", this.leader, command, cmd.GetHelp()), delay);
            }
        }
    }
};

// vim:ft=enforce
