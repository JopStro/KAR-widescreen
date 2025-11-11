#include "hoshi/settings.h"

#define MOD_NAME "Widescreen Support"

int is_widescreen = 0;

char ModName[] = MOD_NAME;      // Name of the mod.
char ModAuthor[] = "JoStro"; // Creator of the mod.
char ModVersion[] = "v0.1";     // Version of the mod.


OptionDesc ModSettings = {
    .name = "Aspect Ratio",
    .description = "Change the game's aspect ratio for modern displays!",
    .kind = OPTKIND_VALUE,
    .val = &is_widescreen,
    .value_num = 2,
    .value_names = (char *[]){
        "4:3",
        "16:9",
    }
};

// Runs on startup after any save data is loaded into memory.
// This callback is executed regardless of if a memory card is inserted or contained existing save data.
void OnSaveLoaded()
{
}
