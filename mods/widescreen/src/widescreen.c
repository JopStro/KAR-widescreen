#include "text.h"
#include "obj.h"
#include "scene.h"

#include "hoshi/settings.h"

#include "widescreen.h"

char ModName[] = MOD_NAME;      // Name of the mod.
char ModAuthor[] = "JoStro"; // Creator of the mod.
char ModVersion[] = "v0.1";     // Version of the mod.

OptionDesc ModSettings = {
    .name = "Aspect Ratio",
    .description = "Change the game's aspect ratio for modern displays!",
    .kind = OPTKIND_VALUE,
    .on_change = &Wide_ChangeSetting,
    .val = &is_widescreen,
    .value_num = 2,
    .value_names = (char *[]){
        "4:3",
        "16:9",
    }
};

void OnBoot() {
    default_aspect = *stc_aspect;
    return;
}

// Runs on startup after any save data is loaded into memory.
// This callback is executed regardless of if a memory card is inserted or contained existing save data.
void OnSaveLoaded()
{
    // if (is_widescreen) {
    //     *stc_aspect /= RATIO;
    // }
    return;
}

void Wide_ChangeSetting(int val) {
    if (val) {
        *stc_aspect = default_aspect / RATIO;
    } else {
        *stc_aspect = default_aspect;
    }
    if (Scene_GetCurrentMajor() == MJRKIND_MENU) {
        Wide_UpdateCObjs(val);
    }
    return;
}

void Wide_UpdateCObjs(int val) {
    GOBJ *m = stc_scene_menu_common->cam_gobj;
    while (m) {
        if (m->obj_kind == HSD_OBJKIND_COBJ) {
            COBJ *c = m->hsd_object;
            if (c->projection_type == PROJ_PERSPECTIVE) {
                if (val)
                    c->projection_param.perspective.aspect /= RATIO;
                else
                    c->projection_param.perspective.aspect *= RATIO;
            }
        }
        m = m->next;
    }
    TextCanvas *canvas = *stc_textcanvas_first;
    while (canvas) {
        if (canvas->cam_gobj && canvas->cam_gobj->obj_kind == HSD_OBJKIND_COBJ) {
            COBJ *c = canvas->cam_gobj->hsd_object;
            if (val) {
                float adjustment = (640 / RATIO - 640) / 2;
                CObj_SetOrtho(c, 0, -480, -adjustment, 640 + adjustment);
            } else {
                CObj_SetOrtho(c, 0, -480, 0, 640);
            }
        }
        canvas = canvas->next;
    }

    return;
}
