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

class DMWebhook
{
    static DMWebhook Create(DMWebhookSettings settings)
    {
        if (settings.type == "")
        {
            return new DMNullWebhook();
        }
        else if (settings.type == "discord")
        {
            return new DMDiscordWebhook(settings.url);
        }

        Error2("DMWebhook error", "Unsupported webhook type: " + settings.type);
        return new DMNullWebhook();
    }

    void DMWebhook()
    {
        CreateRestApi();
    }

    void PostMessage(string message) {}

    void PostLeaderboard(TDMScoreArray leaderboard) {}
};

class DMNullWebhook : DMWebhook
{
};

class DMDiscordWebhookCallback : RestCallback
{
    override void OnError(int errorCode)
    {
        if (errorCode == 8)
        {
            // Error code 8 seems to be normal for Discord webhook responses
            return;
        }

        PrintFormat("ERROR :: Discord webhook returned %1", errorCode);
    }
};

class DMDiscordWebhook : DMWebhook
{
    string m_url;

    void DMDiscordWebhook(string url)
    {
        m_url = url;
    }

    override void PostMessage(string message)
    {
        JsonObject data = new JsonObject();
        data.AddString("content", message);
        data.AddInt("flags", 1 << 2);  // SUPPRESS_EMBEDS

        DMDiscordWebhookCallback cbx = new DMDiscordWebhookCallback();
        RestContext ctx = GetRestApi().GetRestContext(m_url);
        ctx.SetHeader("application/json");
        ctx.POST(cbx, "", data.GetJson());
    }

    override void PostLeaderboard(TDMScoreArray leaderboard)
    {
        string message = "**Leaderboard:**\\n";

        message += "```Kills | Deaths | Name\\n------+--------+-------------------------------\\n";
        foreach (DMScore score : leaderboard)
        {
            message += string.Format(
                    "%1 | %2 | %3\\n",
                    DMUtils.PadLeft(score.m_kills.ToString(), 5),
                    DMUtils.PadLeft(score.m_deaths.ToString(), 6),
                    score.m_name);
        }
        message += "```";

        this.PostMessage(message);
    }
};

// vim:ft=enforce
