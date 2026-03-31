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

class DMVersionChecker
{
    private static const string LATEST_URL = "https://crimsonzamboni.com/deathmatch/version.txt";
    private static const string STATE_FILE = "$saves:deathmatch-version-check.dat";

    private static string s_upgrade = "";

    static void CheckVersion()
    {
        string datestamp = DMUtils.DateStamp();

        if (DMVersionChecker.ReadState(datestamp))
        {
            DMVersionChecker.WriteState(datestamp);

            Print("Checking for new version...");

            DMVersionCheckerCallback cbx = new DMVersionCheckerCallback();
            RestContext ctx = GetRestApi().GetRestContext(DMVersionChecker.LATEST_URL);
            ctx.GET(cbx, "");
        }
    }

    static void SetLatest(string data)
    {
        int currentMajor;
        int currentMinor;
        int currentPatch;
        DMVersionChecker.ParseVersion(
            CRIMSON_ZAMBONI_DEATHMATCH_VERSION, currentMajor, currentMinor, currentPatch);

        int latestMajor;
        int latestMinor;
        int latestPatch;
        if (!DMVersionChecker.ParseVersion(data, latestMajor, latestMinor, latestPatch))
        {
            PrintFormat("WARNING :: Received invalid latest version: %1", data);
            return;
        }

        string latest = string.Format("%1.%2.%3", latestMajor, latestMinor, latestPatch);

        PrintFormat("Latest CrimsonZamboniDeathmatch version detected: %1", latest);

        DMVersionChecker.WriteState(DMUtils.DateStamp(), latest);

        if (latestMajor > currentMajor)
        {
            s_upgrade = latest;
        }
        else if (latestMajor == currentMajor)
        {
            if (latestMinor > currentMinor)
            {
                s_upgrade = latest;
            }
            else if (latestMinor == currentMinor)
            {
                if (latestPatch > currentPatch)
                {
                    s_upgrade = latest;
                }
            }
        }
    }

    static bool UpgradeAvailable(out string version)
    {
        if (s_upgrade == "")
        {
            return false;
        }

        version = s_upgrade;
        return true;
    }

    static bool ParseVersion(string data, out int major, out int minor, out int patch)
    {
        if (data.Length() < 1)
        {
            return false;
        }

        int i = 0;

        if (data.Substring(0, 1) == "v" || data.Substring(0, 1) == "V")
        {
            i++;
        }

        string version;

        while (i < data.Length())
        {
            string chr = data.Substring(i, 1);
            int c = chr.Hash();
            i++;

            if ((c != 46) && ((c < 48) || (c > 57)))
            {
                break;
            }

            version += chr;
        }

        TStringArray parts = new TStringArray();
        version.Split(".", parts);
        if (parts.Count() != 3)
        {
            return false;
        }

        major = parts[0].ToInt();
        minor = parts[1].ToInt();
        patch = parts[2].ToInt();
        return true;
    }

    private static void WriteState(string datestamp, string latestVersion = "")
    {
        FileHandle fh = OpenFile(DMVersionChecker.STATE_FILE, FileMode.WRITE);
        if (fh == 0)
        {
            return;
        }

        FPrintln(fh, datestamp);
        FPrintln(fh, latestVersion);
        CloseFile(fh);
    }

    private static bool ReadState(string datestamp)
    {
        FileHandle fh = OpenFile(DMVersionChecker.STATE_FILE, FileMode.READ);
        if (fh == 0)
        {
            return true;
        }

        string lastDateStamp;

        if (FGets(fh, lastDateStamp) <= 0)
        {
            CloseFile(fh);
            return true;
        }

        if (lastDateStamp != datestamp)
        {
            CloseFile(fh);
            return true;
        }

        Print("Reading latest version from saved state...");

        string latestVersion;

        FGets(fh, latestVersion);

        DMVersionChecker.SetLatest(latestVersion);

        CloseFile(fh);
        return false;
    }
};

class DMVersionCheckerCallback : RestCallback
{
    override void OnSuccess(string data, int dataSize)
    {
        DMVersionChecker.SetLatest(data);
    }

    override void OnError(int errorCode)
    {
        PrintFormat("WARNING :: Check for latest version failed: %1", errorCode);
    }

    override void OnTimeout()
    {
        Print("WARNING :: Check for latest version timed out");
    }
};

// vim:ft=enforce
