#ifndef WIDESCREEN_H
#define WIDESCREEN_H

#define MOD_NAME "Widescreen Support"

static float *stc_aspect = (float*)0x805deb20;

#define NUM_VIDEO_VIEWPORTS 3
// "declare array of pointer to array 2 of short", I hate C type declarations
static short (*stc_video_viewports[NUM_VIDEO_VIEWPORTS])[2] = {
  (short (*)[2])0x80495b40,
  (short (*)[2])0x80495bb8,
  (short (*)[2])0x80497334,
};

int is_widescreen = 0;

const short pillarbox_viewport[2] = {80, 560};

static void Wide_ChangeSetting(int val);

#endif // WIDESCREEN_H
