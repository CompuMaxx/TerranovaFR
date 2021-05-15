#include "global.h"
#include "gflib.h"
#include "decompress.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_setup.h"
#include "scanline_effect.h"
#include "task.h"
#include "trig.h"

static EWRAM_DATA u16 sBgCnt = 0;

extern const u8 gBattleAnimRegOffsBgCnt[];
extern const u8 gBattleIntroRegOffsBgCnt[];
extern u8 GetBattleTerrainOverride(void);

struct BattleBackground
{
    const void *tileset;
    const void *tilemap;
    const void *entryTileset;
    const void *entryTilemap;
    const void *palette;
};

extern const struct BattleBackground sBattleTerrainTable[];

static void BattleIntroSlide(u8 taskId);

void SetAnimBgAttribute(u8 bgId, u8 attributeId, u8 value)
{
    if (bgId < 4)
    {
        sBgCnt = GetGpuReg(gBattleAnimRegOffsBgCnt[bgId]);
        switch (attributeId)
        {
        case BG_ANIM_SCREEN_SIZE:
            ((struct BgCnt *)&sBgCnt)->screenSize = value;
            break;
        case BG_ANIM_AREA_OVERFLOW_MODE:
            ((struct BgCnt *)&sBgCnt)->areaOverflowMode = value;
            break;
        case BG_ANIM_MOSAIC:
            ((struct BgCnt *)&sBgCnt)->mosaic = value;
            break;
        case BG_ANIM_CHAR_BASE_BLOCK:
            ((struct BgCnt *)&sBgCnt)->charBaseBlock = value;
            break;
        case BG_ANIM_PRIORITY:
            ((struct BgCnt *)&sBgCnt)->priority = value;
            break;
        case BG_ANIM_PALETTES_MODE:
            ((struct BgCnt *)&sBgCnt)->palettes = value;
            break;
        case BG_ANIM_SCREEN_BASE_BLOCK:
            ((struct BgCnt *)&sBgCnt)->screenBaseBlock = value;
            break;
        }
        SetGpuReg(gBattleAnimRegOffsBgCnt[bgId], sBgCnt);
    }
}

s32 GetAnimBgAttribute(u8 bgId, u8 attributeId)
{
    u16 bgCnt;

    if (bgId < 4)
    {
        bgCnt = GetGpuReg(gBattleIntroRegOffsBgCnt[bgId]);
        switch (attributeId)
        {
        case BG_ANIM_SCREEN_SIZE:
            return ((struct BgCnt *)&bgCnt)->screenSize;
        case BG_ANIM_AREA_OVERFLOW_MODE:
            return ((struct BgCnt *)&bgCnt)->areaOverflowMode;
        case BG_ANIM_MOSAIC:
            return ((struct BgCnt *)&bgCnt)->mosaic;
        case BG_ANIM_CHAR_BASE_BLOCK:
            return ((struct BgCnt *)&bgCnt)->charBaseBlock;
        case BG_ANIM_PRIORITY:
            return ((struct BgCnt *)&bgCnt)->priority;
        case BG_ANIM_PALETTES_MODE:
            return ((struct BgCnt *)&bgCnt)->palettes;
        case BG_ANIM_SCREEN_BASE_BLOCK:
            return ((struct BgCnt *)&bgCnt)->screenBaseBlock;
        }
    }
    return 0;
}

void HandleIntroSlide(u8 terrain)
{
    u8 taskId;

    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        taskId = CreateTask(BattleIntroSlide, 0);
    }
    else if ((gBattleTypeFlags & BATTLE_TYPE_KYOGRE_GROUDON) && gGameVersion != VERSION_RUBY)
    {
        terrain = BATTLE_TERRAIN_UNDERWATER;
        taskId = CreateTask(BattleIntroSlide, 0);
    }
    else
    {
        taskId = CreateTask(BattleIntroSlide, 0);
    }
    gTasks[taskId].data[0] = 0;
    gTasks[taskId].data[1] = terrain;
    gTasks[taskId].data[2] = 0;
    gTasks[taskId].data[3] = 0;
    gTasks[taskId].data[4] = 0;
    gTasks[taskId].data[5] = 0;
    gTasks[taskId].data[6] = 0;
}

void sub_80BC41C(u8 taskId)
{
    DestroyTask(taskId);
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
    gBattle_BG2_X = 0;
    gBattle_BG2_Y = 0;
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR);
}

void BattleIntroSlide(u8 taskId)
{
	s32 i;

    gBattle_BG1_Y = 256;
	
    switch (gTasks[taskId].data[0])
    {
    case 0:
        if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
        {
			gTasks[taskId].data[2] = 1;
            ++gTasks[taskId].data[0];
        }
        break;
    case 1:
        if (--gTasks[taskId].data[2] == 0)
        {
            SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR);
            ++gTasks[taskId].data[0];
        }
        break;
    case 2:
        gBattle_WIN0V -= 0xFF;
        if ((gBattle_WIN0V & 0xFF00) == 0x3000)
        {
            gTasks[taskId].data[2] = 240;
            gTasks[taskId].data[3] = 32;
            gIntroSlideFlags &= ~1;
            ++gTasks[taskId].data[0];
        }
        break;
    case 3:
        if (gTasks[taskId].data[3])
        {
            --gTasks[taskId].data[3];
        }
        if (gBattle_WIN0V & 0xFF00)
            gBattle_WIN0V -= 0x3FC;
        if (gTasks[taskId].data[2])
            gTasks[taskId].data[2] -= 2;
		for (i = 0; i < 80; ++i)
			gScanlineEffectRegBuffers[gScanlineEffect.srcBuffer][i] = gTasks[taskId].data[2];
        while (i < 160)
            gScanlineEffectRegBuffers[gScanlineEffect.srcBuffer][i++] = -gTasks[taskId].data[2];
        if (!gTasks[taskId].data[2])
        {
			gScanlineEffect.state = 3;
			SetGpuReg(REG_OFFSET_BG1VOFS, 0);
			HideBg(3);
			++gTasks[taskId].data[0];
        }
        break;
    case 4:
		SetGpuReg(REG_OFFSET_BG3CNT, BGCNT_PRIORITY(3) | BGCNT_CHARBASE(2) | BGCNT_16COLOR | BGCNT_SCREENBASE(26) | BGCNT_TXT512x256);
		LZDecompressVram(sBattleTerrainTable[GetBattleTerrainOverride()].tileset, (void*)(VRAM + 0x8000));
		LZDecompressVram(sBattleTerrainTable[GetBattleTerrainOverride()].tilemap, (void*)(VRAM + 0xD000));
		ShowBg(3);
		HideBg(1);
		++gTasks[taskId].data[0];
        break;
    case 5:
		SetBgAttribute(1, BG_ATTR_CHARBASEINDEX, 0);
		SetGpuReg(REG_OFFSET_BG1CNT, BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_16COLOR | BGCNT_SCREENBASE(28) | BGCNT_TXT256x512);
		CpuFill32(0, (void *)BG_SCREEN_ADDR(28), 2 * BG_SCREEN_SIZE);
		ShowBg(1);
		SetBgAttribute(2, BG_ATTR_CHARBASEINDEX, 0);
		SetGpuReg(REG_OFFSET_BG2CNT, BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_16COLOR | BGCNT_SCREENBASE(30) | BGCNT_TXT512x256);
		++gTasks[taskId].data[0];
        break;
    case 6:
        sub_80BC41C(taskId);
        break;
    }
}

void CopyBattlerSpriteToBg(s32 bgId, u8 x, u8 y, u8 battlerPosition, u8 palno, u8 *tilesDest, u16 *tilemapDest, u16 tilesOffset)
{
    s32 i, j;
    u8 battler = GetBattlerAtPosition(battlerPosition);
    s32 offset = tilesOffset;

    CpuCopy16(gMonSpritesGfxPtr->sprites[battlerPosition] + BG_SCREEN_SIZE * gBattleMonForms[battler], tilesDest, BG_SCREEN_SIZE);
    LoadBgTiles(bgId, tilesDest, 0x1000, tilesOffset);
    for (i = y; i < y + 8; ++i)
        for (j = x; j < x + 8; ++j)
            tilemapDest[i * 32 + j] = offset++ | (palno << 12);
    LoadBgTilemap(bgId, tilemapDest, BG_SCREEN_SIZE, 0);
}

