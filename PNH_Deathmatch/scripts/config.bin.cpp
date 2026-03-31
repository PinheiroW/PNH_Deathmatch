class CfgPatches
{
	class CrimsonZamboniDeathmatch
	{
		requiredAddons[]=
		{
			"DZ_Scripts"
		};
	};
};
class CfgMods
{
	class CrimsonZamboniDeathmatch
	{
		type="mod";
		defines[]=
		{
			"CRIMSON_ZAMBONI_DEATHMATCH"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"crimsonzambonideathmatch/scripts/3_game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"crimsonzambonideathmatch/scripts/4_world"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"crimsonzambonideathmatch/scripts/5_mission"
				};
			};
		};
	};
};
