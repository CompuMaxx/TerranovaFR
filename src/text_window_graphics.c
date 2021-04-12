#include "global.h"
#include "text_window_graphics.h"

static const u16 gUserFrameTiles0[] = INCBIN_U16("graphics/text_window/user_frame_0.4bpp");
static const u16 gUserFrameTiles1[] = INCBIN_U16("graphics/text_window/user_frame_1.4bpp");
static const u16 gUserFrameTiles2[] = INCBIN_U16("graphics/text_window/user_frame_2.4bpp");
static const u16 gUserFrameTiles3[] = INCBIN_U16("graphics/text_window/user_frame_3.4bpp");
static const u16 gUserFrameTiles4[] = INCBIN_U16("graphics/text_window/user_frame_4.4bpp");
static const u16 gUserFrameTiles5[] = INCBIN_U16("graphics/text_window/user_frame_5.4bpp");
static const u16 gUserFrameTiles6[] = INCBIN_U16("graphics/text_window/user_frame_6.4bpp");
static const u16 gUserFrameTiles7[] = INCBIN_U16("graphics/text_window/user_frame_7.4bpp");
static const u16 gUserFrameTiles8[] = INCBIN_U16("graphics/text_window/user_frame_8.4bpp");
static const u16 gUserFrameTiles9[] = INCBIN_U16("graphics/text_window/user_frame_9.4bpp");

static const u16 gUserFramePal0[] = INCBIN_U16("graphics/text_window/user_frame_0.gbapal");
static const u16 gUserFramePal1[] = INCBIN_U16("graphics/text_window/user_frame_1.gbapal");
static const u16 gUserFramePal2[] = INCBIN_U16("graphics/text_window/user_frame_2.gbapal");
static const u16 gUserFramePal3[] = INCBIN_U16("graphics/text_window/user_frame_3.gbapal");
static const u16 gUserFramePal4[] = INCBIN_U16("graphics/text_window/user_frame_4.gbapal");
static const u16 gUserFramePal5[] = INCBIN_U16("graphics/text_window/user_frame_5.gbapal");
static const u16 gUserFramePal6[] = INCBIN_U16("graphics/text_window/user_frame_6.gbapal");
static const u16 gUserFramePal7[] = INCBIN_U16("graphics/text_window/user_frame_7.gbapal");
static const u16 gUserFramePal8[] = INCBIN_U16("graphics/text_window/user_frame_8.gbapal");
static const u16 gUserFramePal9[] = INCBIN_U16("graphics/text_window/user_frame_9.gbapal");

const u16 gSingPostFrame[] = INCBIN_U16("graphics/text_window/sing_post_frame.4bpp");
const u16 gStdFrame0[] = INCBIN_U16("graphics/text_window/standard_frame_0.4bpp");
const u16 gStdFrame1[] = INCBIN_U16("graphics/text_window/standard_frame_1.4bpp");

const u16 gUnknown_8471DEC[][16] = {
    INCBIN_U16("graphics/text_window/dialog_frame.gbapal"),
    INCBIN_U16("graphics/text_window/sing_post_frame.gbapal"),
    INCBIN_U16("graphics/text_window/stdpal_2.gbapal"),
    INCBIN_U16("graphics/text_window/standard_frame_0.gbapal"),
    INCBIN_U16("graphics/text_window/stdpal_4.gbapal")
};

const struct TextWindowGraphics gUserFrames[] = {
    {gUserFrameTiles0, gUserFramePal0},
    {gUserFrameTiles1, gUserFramePal1},
    {gUserFrameTiles2, gUserFramePal2},
    {gUserFrameTiles3, gUserFramePal3},
    {gUserFrameTiles4, gUserFramePal4},
    {gUserFrameTiles5, gUserFramePal5},
    {gUserFrameTiles6, gUserFramePal6},
    {gUserFrameTiles7, gUserFramePal7},
    {gUserFrameTiles8, gUserFramePal8},
    {gUserFrameTiles9, gUserFramePal9}
}; // NELEMS = 10

const struct TextWindowGraphics * GetUserFrameGraphicsInfo(u8 idx)
{
    if (idx >= 20) // if (idx >= NELEMS(gUserFrames))
        return &gUserFrames[0];
    else
        return &gUserFrames[idx];
}
