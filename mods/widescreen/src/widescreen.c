#include "game.h"
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

CODEPATCH_HOOKCREATE(0x80114cec, "mr 3, 31\n\tli 4, 0\n\t", Wide_CreateHudElementHook, "", 0)
CODEPATCH_HOOKCREATE(0x80114b8c, "mr 3, 30\n\tli 4, 0\n\tli 5, 0\n\t", Wide_ShiftHudElement, "mr 3, 31\n\t", 0) // Health Bar and Boost Charge
CODEPATCH_HOOKCREATE(0x80119a8c, "mr 3, 25\n\tli 4, 0\n\tli 5, 0\n\t", Wide_ShiftHudElement, "", 0) // Copy Abilities
CODEPATCH_HOOKCREATE(0x8012f39c, "mr 3, 31\n\tli 4, 1\n\tli 5, 1\n\t", Wide_ShiftHudElement, "", 0) // Stadium Points Splash
CODEPATCH_HOOKCREATE(0x80115a08, "", Wide_IndicatorCOBJHOOK, "", 0)
CODEPATCH_HOOKCREATE(0x80115aac, "", Wide_MapDotsCOBJHOOK, "", 0)
CODEPATCH_HOOKCREATE(0x8040313c, "", Wide_COBJLoadAdjustAspect, "cmplwi 30, 0\n\t", 0)
CODEPATCH_HOOKCREATE(0x80125d88, "mr 3, 29\n\t", Wide_PlyViewPosCreateHook, "", 0)
CODEPATCH_HOOKCREATE(0x8011faa0, "mr 3, 31\n\taddi 4, 1, 0x34\n\t", Wide_IndicatorGXHook, "", 0) // Players
CODEPATCH_HOOKCREATE(0x8012137c, "mr 3, 31\n\taddi 4, 1, 0x14\n\t", Wide_IndicatorGXHook, "", 0) // Enemies
CODEPATCH_HOOKCREATE(0x80120cb0, "mr 3, 30\n\taddi 4, 1, 0x38\n\t", Wide_IndicatorGXHook, "", 0) // PlIcons

void OnBoot() {
    default_aspect = *stc_aspect;
    
    CODEPATCH_REPLACECALL(0x8044f774, Wide_CanvasSetOrtho);
    CODEPATCH_REPLACECALL(0x80064594, Wide_CorrectPerspectiveMTX);
    CODEPATCH_REPLACECALL(0x80385668, Wide_CorrectPerspectiveMTX);
    
    CODEPATCH_HOOKAPPLY(0x80114cec);
    CODEPATCH_HOOKAPPLY(0x80114b8c);
    CODEPATCH_HOOKAPPLY(0x80119a8c);
    CODEPATCH_HOOKAPPLY(0x80115a08);
    CODEPATCH_HOOKAPPLY(0x80115aac);
    CODEPATCH_HOOKAPPLY(0x8040313c);
    CODEPATCH_HOOKAPPLY(0x80125d88);
    CODEPATCH_HOOKAPPLY(0x8011faa0);
    CODEPATCH_HOOKAPPLY(0x8012137c);    
    CODEPATCH_HOOKAPPLY(0x80120cb0);
    CODEPATCH_HOOKAPPLY(0x8012f39c);

    HSD_Archive *archive = Archive_LoadFile("IfBorderAll");
    border_jobj = Archive_GetPublicAddress(archive, "IfBorderAll");
       
    return;
}

void OnSceneChange() {
    MinorKind mnrkind = Scene_GetCurrentMinor();
    if (mnrkind == MNRKIND_3D || mnrkind == MNRKIND_19 || mnrkind == MNRKIND_DEBUGMENU) return;
    GOBJ *gobj = GOBJ_EZCreator(0, 0, 0, 0, 0, 
                                HSD_OBJKIND_JOBJ, border_jobj, 
                                0, 0, 
                                JObj_GX, 63, 4);
    return;
}

void Wide_ChangeSetting(int val) {
    if (val) {
        *stc_aspect = default_aspect / RATIO;
        *stc_plicon_bound = (640 / RATIO - *stc_plicon_bound) - 320;
        (*stc_ar_map_viewport)[0] = 552;
        (*stc_ar_map_viewport)[1] = 632;
        (*stc_ct_map_viewport)[1] = 120;
        (*stc_ar_map_scissor)[0] = 552;
        (*stc_ar_map_scissor)[1] = 632;
        (*stc_ct_map_scissor)[1] = 120;
    } else {
        *stc_aspect = default_aspect;
        *stc_plicon_bound = 160;
        (*stc_ar_map_viewport)[0] = 523;
        (*stc_ar_map_viewport)[1] = 630;
        (*stc_ct_map_viewport)[1] = 160;
        (*stc_ar_map_scissor)[0] = 523;
        (*stc_ar_map_scissor)[1] = 630;
        (*stc_ct_map_scissor)[1] = 160;
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
    if (is_widescreen) {
        C_MTXPerspective(m, fovY, aspect * RATIO, n, f);
    } else {
        C_MTXPerspective(m, fovY, aspect, n, f);
    }
}

float Wide_COBJLoadAdjustAspect(float aspect) {
    if (is_widescreen)
        return aspect / 0.75;
    return aspect;
}

void Wide_CreateHudElementHook(JOBJ *obj, int cpy) {
    if (!is_widescreen) return;
    float shift_by = Gm_GetPlyViewNum() <= 2 ? 10 : 5;
    if (cpy || !obj->child) {
        obj->trans.X += shift_by;
        return;
    }
    
    int left = 0, right = 0;
    for (JOBJ* m = obj->child; m; m = m->sibling) {
        if (m->trans.X > 0) {
            right = 1;
        } else if (m->trans.X < 0) {
            left = 1;
        }
    }
    if (left && right) return;
    obj->trans.X += left ? -shift_by : right ? shift_by : 0;
    return;
}

void Wide_ShiftHudElement(JOBJ *obj, int negate, int neg_splitscreen) {
    if (!is_widescreen) return;
    float shift_by = Gm_GetPlyViewNum() <= 2 ? 10 : 5;
    shift_by *= negate ? -1 : 1;
    if (Gm_GetPlyViewNum() > 1 && neg_splitscreen) shift_by *= -1;
    obj->trans.X += shift_by;
}

void Wide_IndicatorCOBJHOOK(COBJ *obj) {
    if (is_widescreen) {
        float adjustment = ((64 / 0.75) - 64) / 2;
        obj->projection_param.ortho.left -= adjustment;
        obj->projection_param.ortho.right += adjustment;
    }
}

void Wide_MapDotsCOBJHOOK(COBJ *obj) {
    if (!is_widescreen) return;
    if (Gm_GetPlyViewNum() > 2) {
        if (Gm_GetPlyViewNum() == 4) return;
        s8 *views = Gm_Get3dData()->plyview_lookup;
        int i = 0;
        while (i < 4) {
            if (views[i] == -1) {
                break;
            }
            i++;
        }
        if (i < 2) {
            obj->viewport_right -= 160 - 41;
            obj->viewport_left += 41;
        } else {
            obj->viewport_left += 160 - 41;
            obj->viewport_right -= 41;
        }
    } else if (Gm_IsInCity()) {
        obj->viewport_right -= 160 - 12;
        obj->viewport_left += 12;
    } else {
        obj->viewport_left += 160 - 12;
        obj->viewport_right -= 12;
    }
}

void Wide_PlyViewPosCreateHook(GOBJ *obj) {
    if (!is_widescreen) return;
    PlyViewPosData *data = obj->userdata;
    int num = Gm_GetPlyViewNum();
    if (num <= 2) return;
    data->plyview_center_pos[0].X -= 5;
    data->plyview_center_pos[1].X -= 5;
    data->plyview_center_pos[2].X += 5;
    data->plyview_center_pos[3].X += 5;
}

void Wide_IndicatorGXHook(IndicatorData *data, float *x) {
    if (!is_widescreen) return;
    if (Gm_GetPlyViewNum() <= 2) return;
    float adjustment = (640 / 0.75 - 640) / 4;
    int ply = data->viewdata >> 28;
    if (Ply_GetViewIndex(ply) < 2) {
        *x -= adjustment;
    } else {
        *x += adjustment;
    }
}
