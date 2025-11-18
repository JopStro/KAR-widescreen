#ifndef WIDESCREEN_H
#define WIDESCREEN_H

#include "datatypes.h"
#include "structs.h"

#define MOD_NAME "Widescreen Support"
#define RATIO 0.75

static float *stc_aspect = (float*)0x805deb20;
float default_aspect;


static short (*stc_ar_map_viewport)[4] = (void *)0x805d5310;
static short (*stc_ar_map_scissor)[4]  = (void *)0x805d5318;
static short (*stc_ct_map_viewport)[4] = (void *)0x805d5320;
static short (*stc_ct_map_scissor)[4]  = (void *)0x805d5328;

static float *stc_plicon_bound = (float *)0x805dfdd8;

#define NUM_VIDEO_DESCS 3
static COBJDesc *stc_video_descs[NUM_VIDEO_DESCS] = {
  (COBJDesc *)0x80495b38,
  (COBJDesc *)0x80495bb0,
  (COBJDesc *)0x8049732C,
};

int is_widescreen = 0;

static void (*C_MTXPerspective)(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f) = (void*)0x803d1f30;

static void Wide_ChangeSetting(int);
static void Wide_UpdateCObjs(int);
static void Wide_CanvasSetOrtho(COBJ* cobj, float top, float bottom, float left, float right);
static void Wide_CorrectPerspectiveMTX(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f);
static float Wide_COBJLoadAdjustAspect(float);
static void Wide_CreateHudElementHook(JOBJ *obj, int cpy);
static void Wide_ShiftHudElement(JOBJ *obj, int negate, int neg_splitscreen);
static void Wide_IndicatorCOBJHOOK(COBJ *obj);
static void Wide_MapDotsCOBJHOOK(COBJ *obj);
static void Wide_PlyViewPosCreateHook(GOBJ *obj);
static void Wide_IndicatorGXHook(int ply, float *x);
static void Wide_PlIconGXHook(int mode, int ply, float *x);
static void Wide_CreateStatChartHook(JOBJ *obj, int ply);

static void Wide_BorderCamGX(GOBJ *g);

#endif // WIDESCREEN_H
