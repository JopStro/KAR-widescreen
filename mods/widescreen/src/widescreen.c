#include "hsd.h"
#include "inline.h"
#include "structs.h"
#include "text.h"
#include "obj.h"
#include "scene.h"
#include "hoshi/settings.h"
#include "code_patch/code_patch.h"

#include "widescreen.h"

char ModName[] = MOD_NAME;      // Name of the mod.
char ModAuthor[] = "JoStro"; // Creator of the mod.
char ModVersion[] = "v0.1";     // Version of the mod.

OptionDesc ModSettings = {
    .name = "Aspect Ratio",
    .description = "Change the display width.",
    .kind = OPTKIND_VALUE,
    .on_change = &Wide_ChangeSetting,
    .val = &is_widescreen,
    .value_num = 2,
    .value_names = (char *[]){
        "Standard",
        "Wide",
    }
};

JOBJDesc *border_jobj;

CODEPATCH_HOOKCREATE(0x80114be8, "", Wide_CreateHudElementHook, "", 0);
CODEPATCH_HOOKCREATE(0x80114a84, "", Wide_CreateHudElementHook, "", 0);
CODEPATCH_HOOKCREATE(0x80119960, "", Wide_CreateHudElementHook, "", 0);

void OnBoot() {
    default_aspect = *stc_aspect;
    
    CODEPATCH_REPLACECALL(0x8044f774, Wide_CanvasSetOrtho);
    CODEPATCH_REPLACECALL(0x80064594, Wide_CorrectPerspectiveMTX);
    CODEPATCH_REPLACECALL(0x80385668, Wide_CorrectPerspectiveMTX);
    CODEPATCH_REPLACECALL(0x8037a220, Wide_TopRideCOBJLoadDesc);
    CODEPATCH_REPLACECALL(0x802cad88, Wide_TopRideCOBJLoadDesc);
    CODEPATCH_REPLACECALL(0x8037a0fc, Wide_TopRideCOBJLoadDesc);
    CODEPATCH_REPLACECALL(0x8038250c, Wide_TopRideCOBJLoadDesc);
    CODEPATCH_REPLACECALL(0x8037199c, Wide_TopRideCOBJLoadDesc);
    
    CODEPATCH_HOOKAPPLY(0x80114be8);
    CODEPATCH_HOOKAPPLY(0x80114a84);
    CODEPATCH_HOOKAPPLY(0x80119960);
    
    HSD_Archive *archive = Archive_LoadFile("IfBorderAll");
    border_jobj = Archive_GetPublicAddress(archive, "IfBorderAll");
       
    return;
}

void OnSceneChange() {
    MinorKind mnrkind = Scene_GetCurrentMinor();
    if (mnrkind == MNRKIND_3D || mnrkind == MNRKIND_19) return;
    GOBJ *gobj = GOBJ_EZCreator(0, 0, 0, 0, 0, 
                                HSD_OBJKIND_JOBJ, border_jobj, 
                                0, 0, 
                                JObj_GX, 63, 4);
    return;
}

void Wide_ChangeSetting(int val) {
    if (val) {
        *stc_aspect = default_aspect / RATIO;
    } else {
        *stc_aspect = default_aspect;
    }
    
    for (int i = 0;i < NUM_VIDEO_DESCS;i++) {
        if (val) {
            stc_video_descs[i]->viewport_left = 80;
            stc_video_descs[i]->viewport_right = 560;
        } else {
            stc_video_descs[i]->viewport_left = 0;
            stc_video_descs[i]->viewport_right = 640;
        }
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
            if (c->projection_type == PROJ_PERSPECTIVE && val)
                c->projection_param.perspective.aspect /= RATIO;
            else
                c->projection_param.perspective.aspect *= RATIO;
        }
        m = m->next;
    }
    TextCanvas *canvas = *stc_textcanvas_first;
    while (canvas) {
        if (canvas->cam_gobj) {
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

void Wide_CanvasSetOrtho(COBJ* cobj, float top, float bottom, float left, float right) {
    if (is_widescreen) {
        float adjustment = ((right - left) / RATIO - (right - left)) / 2;
        CObj_SetOrtho(cobj, top, bottom, left - adjustment, right + adjustment);
    } else {
        CObj_SetOrtho(cobj, top, bottom, left, right);
    }
}


void Wide_CorrectPerspectiveMTX(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f) {
    if (is_widescreen && Scene_GetCurrentMinor() != MNRKIND_3D) {
        C_MTXPerspective(m, fovY, aspect * RATIO, n, f);
    } else {
        C_MTXPerspective(m, fovY, aspect, n, f);
    }
}

COBJ *Wide_TopRideCOBJLoadDesc(COBJDesc *desc) {
    COBJ *obj;
    if (is_widescreen) {
        desc->projection_param.perspective.aspect /= RATIO;
        obj = COBJ_LoadDesc(desc);
        desc->projection_param.perspective.aspect *= RATIO;
    } else {
        obj = COBJ_LoadDesc(desc);
    }
    return obj;
}

void Wide_CreateHudElementHook(JOBJ *obj) {
    if (!is_widescreen || Scene_GetCurrentMinor() != MNRKIND_3D) return;
    if (!obj->child) {
        obj->trans.X += 10;
        return;
    }
    JOBJ *root; // Workaround for the needle copy ability
    if (!obj->child->sibling && obj->child->child) {
        root = obj->child;
    } else {
        root = obj;
    }
    
    int left = 0, right = 0;
    for (JOBJ* m = root->child; m; m = m->sibling) {
        if (m->trans.X > 0) {
            right = 1;
        } else if (m->trans.X < 0) {
            left = 1;
        }
    }
    if (left && right) return;
    obj->trans.X += left ? -10 : right ? 10 : 0;
    return;
}


