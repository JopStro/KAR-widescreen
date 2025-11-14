#ifndef WIDESCREEN_H
#define WIDESCREEN_H

#include "datatypes.h"
#include "structs.h"
#define MOD_NAME "Widescreen Support"
#define RATIO 0.75

static float *stc_aspect = (float*)0x805deb20;
float default_aspect;

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
static COBJ *Wide_TopRideCOBJLoadDesc(COBJDesc *desc);
static void Wide_CreateHudElementHook(JOBJ *obj);

#endif // WIDESCREEN_H
