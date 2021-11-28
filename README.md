## Note [Important]
This is not a cheat or hack. Do not attempt to use this on the latest version of the game.

## Mod
 * Detours-based Visual Studio project for hooking and modding the game engine.

This project allows you to run APEX Legends with mods, by running the embedded server
and loading custom scripts/global compile lists into the scripting VM.
This allows you to create custom gamemodes, levels, weapons and more.

Target supported versions are:
 * R5pc_r5launch_N52A_CL399039_2019_03_12_03_21_PM  (S1) (almost ready)
 * R5pc_r5launch_N191_CL429743_2019_07_11_01_04_PM  (S2) (almost ready)
 * R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM (S3) (ready)

The game is based on Valve's Source Engine (heavily modified), the static linked server lib supports multiplayer with up to 128 players.
To connect to a server, use the `connect <ip-address:port>` command (omit `<>`)
 * To activate the in-game console frontend, use the tilde (`~`) key.
 * see https://developer.valvesoftware.com/wiki/Console_Command_List for extra details regarding commands.

You can also use the in-game server browser frontend to host games or connect to ongoing games, which is considered more user-friendly.
 * To activate the server browser frontend, use the `F10` or `F12` key.

Information about this repo:
This repo is a version of several separate branches, centralized into a single branch.
Something seemed to went wrong with a few commit dates in which i have added extra information
at the top of some of the more interesting/large files to match the dates with our official repository at https://github.com/Mauler125/detours_r5.

If i find enough time, i will try to support Titanfall 2 as well
This will most likely require a near-full rewrite as most of the structs and function signatures are different.
Titanfall 2 is compiled using a different compiler (Visual Studio 2012 Update 4) which is less aggressive on optimizations and inlining 
compared to Visual Studio 2017.

The `dedicated` server project is currently work-in-progress.
 * note: this is a console application. No game window will pop up.

## Instructions

1. Clone the solution to the game installation directory. Example: `<gamedir>\platform`
	* `<gamedir>` is where `r5apex.exe` is located.
	* The results should be `<gamedir>\platform\r5apexsdk\detours.sln`
2. Build the solution. If step 1 is performed correctly, the executables will be automatically copied over to their correct locations.
3. Run `r5reloaded.exe` and launch in desired mode.

## Information
* Upon launch, a debug terminal will open in which you can forward your commands directly to the source-engine console backend.
* The Squirrel VM output can be enabled or disabled with the `sq_showvmoutput` ConVar
* The Squirrel VM scripts are loaded from the `platform\` folder. If these are not present, the engine will fall back to the in-memory VPK structures.
