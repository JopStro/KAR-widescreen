#include "hoshi/settings.h"

#define MOD_NAME "Widescreen Support"

int is_widescreen = 0; // variable that is updated by the player in the in-game settings menu via the ModMenu defined below.

char ModName[] = MOD_NAME;      // Name of the mod.
char ModAuthor[] = "JoStro"; // Creator of the mod.
char ModVersion[] = "v0.1";     // Version of the mod.


// Creates a menu that appears in the in-game Settings menu.
// Menus may be nested by setting the OptionDesc::kind to OPTKIND_MENU
OptionDesc ModSettings = {
    .name = "Aspect Ratio",
    .description = "Change The Game's Aspect Ratio For Modern Displays!",
    .kind = OPTKIND_VALUE,
    .val = &is_widescreen,
    .value_num = 2,
    .value_names = (char *[]){
        "4:3",
        "16:9",
    }
}

// Runs immediately after the mod file is loaded.
// Calls to HSD_MemAlloc in THIS function specifically wil persist throughout the entire runtime of the game.
// All calls to HSD_MemAlloc from elsewhere will return an allocation that exists only within the current scene.
void OnBoot()
{
}

// Runs on boot when hoshi creates save data for the mod.
// Initialize default save file values here.
void OnSaveInit()
{
}

// Runs on startup after any save data is loaded into memory.
// This callback is executed regardless of if a memory card is inserted or contained existing save data.
void OnSaveLoaded()
{
}

// Runs when entering the main menu.
void OnMainMenuLoad()
{
}

// Runs when entering the player select menu.
// Currently only executes when entering city trial player select.
void OnPlayerSelectLoad()
{
}

// Runs upon entering a 3D game. Can be either Air Ride or City Trial. Must be explicity checked using Gm_IsInCity().
// Players, riders, their machines, and the map have all been instantiated by the time this is executed.
void On3DLoad()
{
}

// Runs when pausing the match. The index of the pausing player is passed in as an argument.
void On3DPause(int pause_ply)
{
}

// Runs when unpausing the match.
void On3DUnpause()
{
}

// Runs when exiting a match.
void On3DExit()
{
}

// Runs every scene change.
// The memory heap is destroyed and recreated every scene change, meaning HSD objects
// such as CObj's (camera) and JObj's (models) will not persist across them.
// This hook can be used to recreate processes/objects that should always be running.
void OnSceneChange()
{
}

// Runs every game tick, even when the game is paused normally or via debug mode.
void OnFrame()
{
}
