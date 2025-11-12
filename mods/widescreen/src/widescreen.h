#ifndef WIDESCREEN_H
#define WIDESCREEN_H

#define MOD_NAME "Widescreen Support"

static float *stc_aspect = (float*)0x805deb20;
float default_aspect;

#define NUM_VIDEO_VIEWPORTS 3
// "declare array of pointer to array 4 of short", I hate C type declarations
static short (*stc_video_viewports[NUM_VIDEO_VIEWPORTS])[4] = {
  (short (*)[4])0x80495b40,
  (short (*)[4])0x80495bb8,
  (short (*)[4])0x80497334,
};

int is_widescreen = 0;

#define RATIO 0.75

static void Wide_ChangeSetting(int);
static void Wide_UpdateCObjs(int);

#endif // WIDESCREEN_H
