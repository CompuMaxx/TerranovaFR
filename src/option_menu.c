#include "global.h"
#include "gflib.h"
#include "scanline_effect.h"
#include "text_window_graphics.h"
#include "menu.h"
#include "task.h"
#include "overworld.h"
#include "help_system.h"
#include "window.h"
#include "text.h"
#include "text_window.h"
#include "strings.h"
#include "graphics.h"
#include "string_util.h"
#include "field_fadetransition.h"
#include "gba/m4a_internal.h"
#include "constants/songs.h"

// can't include the one in menu_helpers.h since Task_OptionMenu needs bool32 for matching
bool32 MenuHelpers_CallLinkSomething(void);

// Task data
enum
{
    TD_MENUSELECTION,
    TD_TEXTSPEED,
    TD_BATTLESCENE,
    TD_BATTLESTYLE,
    TD_SOUND,
    TD_BUTTONMODE,
    TD_FRAMETYPE,
    TD_RUN,
    TD_LANGUAGE,
    TD_COUNTER,
	TD_TIMER,
};

// Menu items
enum
{
    MENUITEM_TEXTSPEED = 0,
    MENUITEM_BATTLESCENE,
    MENUITEM_BATTLESTYLE,
    MENUITEM_SOUND,
    MENUITEM_BUTTONMODE,
    MENUITEM_FRAMETYPE,
    MENUITEM_RUN,
    MENUITEM_LANGUAGE,
    MENUITEM_COUNT
};

// Window Ids
enum
{
    WIN_TEXT_OPTION = 0,
    WIN_DESCRIPTION,
};

enum
{
    DESCRIPTION,
    FRAME,
	TEXT,
};

//Function Declarataions
static void CB2_InitOptionMenu(void);
static void VBlankCB_OptionMenu(void);
static void OptionMenu_InitCallbacks(void);
static void OptionMenu_SetVBlankCallback(void);
static void CB2_OptionMenu(void);
static void Task_OptionMenuFadeIn(u8 taskId);
static void Task_OptionMenuProcessInput(u8 taskId);
static void OptionMenu_ClearWindow(u8 option);
static void Task_OptionMenuSave(u8 taskId);
static void Task_OptionMenuCancel(u8 taskId);
static void Task_OptionMenuFadeOut(u8 taskId);
static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style);
static void SetDescription(u8 selection, u8 language);
static void ShowDescription(const u8 *text, u8 selection);
static u8 TextSpeed_ProcessInput(u8 selection);
static void TextSpeed_DrawChoices(u8 selection, u8 language);
static u8 BattleScene_ProcessInput(u8 selection);
static void BattleScene_DrawChoices(u8 selection, u8 language);
static u8 BattleStyle_ProcessInput(u8 selection);
static void BattleStyle_DrawChoices(u8 selection, u8 language);
static u8 Sound_ProcessInput(u8 selection);
static void Sound_DrawChoices(u8 selection, u8 language);
static u8 ButtonMode_ProcessInput(u8 selection);
static void ButtonMode_DrawChoices(u8 selection, u8 language);
static u8 FrameType_ProcessInput(u8 selection);
static void FrameType_DrawChoices(u8 selection, u8 language);
static u8 AutoRun_ProcessInput(u8 selection);
static void AutoRun_DrawChoices(u8 selection, u8 language);
static u8 Language_ProcessInput(u8 selection);
static void Language_DrawChoices(u8 selection, u8 language);
static void OptionMenuDrawChoices(u8 taskId);
static void DrawOptionMenuTexts(u8 language);

// Data Definitions
static const struct WindowTemplate sOptionMenuWinTemplates[] =
{
    [WIN_TEXT_OPTION] = {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 2,
        .width = 30,
        .height = 16,
        .paletteNum = 15,
        .baseBlock = 0
    },
	[WIN_DESCRIPTION] = {
        .bg = 2,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 20,
        .paletteNum = 15,
        .baseBlock = 0
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sOptionMenuBgTemplates[] =
{
   { //Text Options
       .bg = 0,
       .charBaseIndex = 0,
       .mapBaseIndex = 31,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 2,
       .baseTile = 0	   
   },
   { //BG
       .bg = 1,
       .charBaseIndex = 3,
       .mapBaseIndex = 28,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 3,
       .baseTile = 0
   },
   { //Instructions & Description
       .bg = 2,
       .charBaseIndex = 1,
       .mapBaseIndex = 30,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 0,
       .baseTile = 0
   },
   { //Buttons & Frame
       .bg = 3,
       .charBaseIndex = 3,
       .mapBaseIndex = 29,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 1,
       .baseTile = 0
   }
};

static const u8 *const sOptionMenuItemsNames[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED]   = gText_TextSpeed,
    [MENUITEM_BATTLESCENE] = gText_BattleScene,
    [MENUITEM_BATTLESTYLE] = gText_BattleStyle,
    [MENUITEM_SOUND]       = gText_Sound,
    [MENUITEM_BUTTONMODE]  = gText_ButtonMode,
    [MENUITEM_FRAMETYPE]   = gText_Frame,
    [MENUITEM_RUN]   	   = gText_Run,
    [MENUITEM_LANGUAGE]    = gText_Language,
};

static const u8 *const sOptionMenuDescriptions[] =
{
    gText_TextSpeedDescription,
    gText_BattleSceneDescription,
    gText_BattleStyleDescription,
    gText_SoundDescription,
    gText_ButtonModeDescription,
    gText_FrameDescription,
    gText_RunDescription,
    gText_LanguageDescription,
	gText_ExitWithSave,
	gText_ExitWithoutSave,
};

static const u8 *const sOptionMenuItemsNamesSpa[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED]   = gText_TextSpeedSpa,
    [MENUITEM_BATTLESCENE] = gText_BattleSceneSpa,
    [MENUITEM_BATTLESTYLE] = gText_BattleStyleSpa,
    [MENUITEM_SOUND]       = gText_SoundSpa,
    [MENUITEM_BUTTONMODE]  = gText_ButtonModeSpa,
    [MENUITEM_FRAMETYPE]   = gText_FrameSpa,
    [MENUITEM_RUN]   	   = gText_RunSpa,
    [MENUITEM_LANGUAGE]    = gText_LanguageSpa,
};

static const u8 *const sOptionMenuDescriptionsSpa[] =
{
    gText_TextSpeedDescriptionSpa,
    gText_BattleSceneDescriptionSpa,
    gText_BattleStyleDescriptionSpa,
    gText_SoundDescriptionSpa,
    gText_ButtonModeDescriptionSpa,
    gText_FrameDescriptionSpa,
    gText_RunDescriptionSpa,
    gText_LanguageDescriptionSpa,
	gText_ExitWithSaveSpa,
	gText_ExitWithoutSaveSpa,
};

// Functions
static void CB2_InitOptionMenu(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_OptionMenu(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_OptionsMenuFromStartMenu(void)
{
   u8 i;
    
    if (gMain.savedCallback == NULL)
        gMain.savedCallback = CB2_ReturnToFieldWithOpenMenu;
    SetHelpContext(HELPCONTEXT_OPTIONS);
    SetMainCallback2(CB2_OptionMenu); 
}

static void CB2_OptionMenu(void)
{
    u8 taskId;
	
	switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
		SetHBlankCallback(NULL);
		DmaFill16(3, 0, (void *)VRAM, VRAM_SIZE);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
		ResetBgsAndClearDma3BusyFlags(0);
		InitBgsFromTemplates(0, sOptionMenuBgTemplates, NELEMS(sOptionMenuBgTemplates));
        ChangeBgX(0, 0, 0);
        ChangeBgY(0, 0, 0);
        ChangeBgX(1, 0, 0);
		ChangeBgY(1, 0, 0);
        ChangeBgX(2, 0, 0);
        ChangeBgY(2, 0, 0);
        ChangeBgX(3, 0, 0);
        ChangeBgY(3, 0, 0);
    	InitWindows(sOptionMenuWinTemplates);
		DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, 0);
        SetGpuReg(REG_OFFSET_WINOUT, 0);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
	    ShowBg(0);
        ShowBg(1);
		ShowBg(2);
		ShowBg(3);
        ResetSpriteData();
		ResetPaletteFade();
		FreeAllSpritePalettes();
     	ResetTasks();
        ScanlineEffect_Stop();
        gMain.state++;
        break;
    case 1:
        LoadBgTiles(3, GetUserFrameGraphicsInfo(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, 24);
        LoadPalette(GetUserFrameGraphicsInfo(gSaveBlock2Ptr->optionsWindowFrameType)->palette, 0xE0, 0x20);
		LZ77UnCompVram(gTilesOptionMenu, (void *)(VRAM + 0xC000));
		LZ77UnCompVram(gMapOptionMenuBG, (void *)(VRAM + 0xE000)); 
		LZ77UnCompVram(gMapOptionMenuButtons, (void *)(VRAM + 0xE800)); 
		LZ77UnCompVram(gMapOptionMenuFrame, (void *)(VRAM + 0xEB80)); 
		LoadPalette(gPalOptionMenuButtons, 0x00, 0x20);
		LoadPalette(gPalOptionMenuNoSel, 0x10, 0x100);
		LoadPalette(gPalOptionMenuSel, 0x10, 0x20);
		LoadPalette(gPalOptionMenuText, 0xF0, 0x20);
        gMain.state++;
        break;
    case 2:
		PutWindowTilemap(WIN_TEXT_OPTION);
		PutWindowTilemap(WIN_DESCRIPTION);
        gMain.state++;
        break;
    case 3:
        taskId = CreateTask(Task_OptionMenuFadeIn, 0);

        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0x10, 0, 0);
        SetVBlankCallback(VBlankCB_OptionMenu);
        SetMainCallback2(CB2_InitOptionMenu);

        gTasks[taskId].data[TD_MENUSELECTION] = 0;
        gTasks[taskId].data[TD_TEXTSPEED] = gSaveBlock2Ptr->optionsTextSpeed;
        gTasks[taskId].data[TD_BATTLESCENE] = gSaveBlock2Ptr->optionsBattleSceneOff;
        gTasks[taskId].data[TD_BATTLESTYLE] = gSaveBlock2Ptr->optionsBattleStyle;
        gTasks[taskId].data[TD_SOUND] = gSaveBlock2Ptr->optionsSound;
        gTasks[taskId].data[TD_BUTTONMODE] = gSaveBlock2Ptr->optionsButtonMode;
        gTasks[taskId].data[TD_FRAMETYPE] = gSaveBlock2Ptr->optionsWindowFrameType;
        gTasks[taskId].data[TD_RUN] = gSaveBlock2Ptr->optionsAutorun;
        gTasks[taskId].data[TD_LANGUAGE] = gSaveBlock2Ptr->optionsLanguage;
		OptionMenu_ClearWindow(TEXT);
		DrawOptionMenuTexts(gTasks[taskId].data[TD_LANGUAGE]);
        Language_DrawChoices(gTasks[taskId].data[TD_LANGUAGE], gTasks[taskId].data[TD_LANGUAGE]);
		OptionMenuDrawChoices(taskId);
		SetDescription(0, gTasks[taskId].data[TD_LANGUAGE]);
        break;
    }
}

static void Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_OptionMenuProcessInput;
}

static void Task_OptionMenuProcessInput(u8 taskId)
{
	if (gMain.newKeys & A_BUTTON)
    {
		PlaySE(SE_SELECT);
		OptionMenu_ClearWindow(DESCRIPTION);
        gTasks[taskId].data[TD_TIMER] = 20;
		gTasks[taskId].func = Task_OptionMenuSave;
    }
	else if (gMain.newKeys & B_BUTTON)
    {
		PlaySE(SE_SELECT);
		OptionMenu_ClearWindow(DESCRIPTION);
        gTasks[taskId].data[TD_TIMER] = 20;
		gTasks[taskId].func = Task_OptionMenuCancel;
    }
    else if (gMain.newKeys & DPAD_UP)
    {
        PlaySE(SE_SELECT);
		if (gTasks[taskId].data[TD_MENUSELECTION] == TD_BATTLESTYLE - 1)
		{
			if (GetGpuReg(REG_OFFSET_BG0VOFS) > 16)
			{
				SetGpuReg(REG_OFFSET_BG0VOFS, 16);
				SetGpuReg(REG_OFFSET_BG1VOFS, 16);
			}
			gTasks[taskId].data[TD_MENUSELECTION]--;
		}
        else if (gTasks[taskId].data[TD_MENUSELECTION] == TD_BATTLESCENE - 1)
		{
			SetGpuReg(REG_OFFSET_BG0VOFS, 0);
			SetGpuReg(REG_OFFSET_BG1VOFS, 0);
			gTasks[taskId].data[TD_MENUSELECTION]--;
		}
        else if (gTasks[taskId].data[TD_MENUSELECTION] > 0)
            gTasks[taskId].data[TD_MENUSELECTION]--;
		LoadPalette(gPalOptionMenuNoSel, 0x10, 0x100);
		LoadPalette(gPalOptionMenuSel, (gTasks[taskId].data[TD_MENUSELECTION] + 1) * 0x10, 0x20);
		OptionMenu_ClearWindow(DESCRIPTION);
		SetDescription(gTasks[taskId].data[TD_MENUSELECTION], gTasks[taskId].data[TD_LANGUAGE]);
    }
    else if (gMain.newKeys & DPAD_DOWN)
    {
        PlaySE(SE_SELECT);
		if (gTasks[taskId].data[TD_MENUSELECTION] == TD_FRAMETYPE - 1)
		{
			if (GetGpuReg(REG_OFFSET_BG0VOFS) < 16)
			{
				SetGpuReg(REG_OFFSET_BG0VOFS, 16);
				SetGpuReg(REG_OFFSET_BG1VOFS, 16);
			}
			gTasks[taskId].data[TD_MENUSELECTION]++;
		}
        else if (gTasks[taskId].data[TD_MENUSELECTION] == TD_RUN - 1)
		{
			SetGpuReg(REG_OFFSET_BG0VOFS, 32);
			SetGpuReg(REG_OFFSET_BG1VOFS, 32);
			gTasks[taskId].data[TD_MENUSELECTION]++;
		}
        else if (gTasks[taskId].data[TD_MENUSELECTION] < TD_FRAMETYPE - 1)
		{
            gTasks[taskId].data[TD_MENUSELECTION]++;
		}

		LoadPalette(gPalOptionMenuNoSel, 0x10, 0x100);
		LoadPalette(gPalOptionMenuSel, (gTasks[taskId].data[TD_MENUSELECTION] + 1) * 0x10, 0x20);
		OptionMenu_ClearWindow(DESCRIPTION);
		SetDescription(gTasks[taskId].data[TD_MENUSELECTION], gTasks[taskId].data[TD_LANGUAGE]);
    }
    else
    {
		u8 previousOption;

        switch (gTasks[taskId].data[TD_MENUSELECTION])
        {
        case MENUITEM_TEXTSPEED:
			previousOption = gTasks[taskId].data[TD_TEXTSPEED];
            gTasks[taskId].data[TD_TEXTSPEED] = TextSpeed_ProcessInput(gTasks[taskId].data[TD_TEXTSPEED]);
			
            if (previousOption != gTasks[taskId].data[TD_TEXTSPEED])
			{
				PlaySE(SE_SELECT);
                TextSpeed_DrawChoices(gTasks[taskId].data[TD_TEXTSPEED], gTasks[taskId].data[TD_LANGUAGE]);
			}
            break;
        case MENUITEM_BATTLESCENE:
            previousOption = gTasks[taskId].data[TD_BATTLESCENE];
            gTasks[taskId].data[TD_BATTLESCENE] = BattleScene_ProcessInput(gTasks[taskId].data[TD_BATTLESCENE]);

            if (previousOption != gTasks[taskId].data[TD_BATTLESCENE])
			{
				PlaySE(SE_SELECT);
                BattleScene_DrawChoices(gTasks[taskId].data[TD_BATTLESCENE], gTasks[taskId].data[TD_LANGUAGE]);
			}
			break;
        case MENUITEM_BATTLESTYLE:
            previousOption = gTasks[taskId].data[TD_BATTLESTYLE];
            gTasks[taskId].data[TD_BATTLESTYLE] = BattleStyle_ProcessInput(gTasks[taskId].data[TD_BATTLESTYLE]);

            if (previousOption != gTasks[taskId].data[TD_BATTLESTYLE])
			{
				PlaySE(SE_SELECT);
				BattleStyle_DrawChoices(gTasks[taskId].data[TD_BATTLESTYLE], gTasks[taskId].data[TD_LANGUAGE]);
			}
            break;
        case MENUITEM_SOUND:
            previousOption = gTasks[taskId].data[TD_SOUND];
            gTasks[taskId].data[TD_SOUND] = Sound_ProcessInput(gTasks[taskId].data[TD_SOUND]);

            if (previousOption != gTasks[taskId].data[TD_SOUND])
			{
				PlaySE(SE_SELECT);
				Sound_DrawChoices(gTasks[taskId].data[TD_SOUND], gTasks[taskId].data[TD_LANGUAGE]);
			}
            break;
        case MENUITEM_BUTTONMODE:
            previousOption = gTasks[taskId].data[TD_BUTTONMODE];
            gTasks[taskId].data[TD_BUTTONMODE] = ButtonMode_ProcessInput(gTasks[taskId].data[TD_BUTTONMODE]);

            if (previousOption != gTasks[taskId].data[TD_BUTTONMODE])
			{
				PlaySE(SE_SELECT);
                ButtonMode_DrawChoices(gTasks[taskId].data[TD_BUTTONMODE], gTasks[taskId].data[TD_LANGUAGE]);
			}
			break;
        case MENUITEM_FRAMETYPE:
            previousOption = gTasks[taskId].data[TD_FRAMETYPE];
            gTasks[taskId].data[TD_FRAMETYPE] = FrameType_ProcessInput(gTasks[taskId].data[TD_FRAMETYPE]);

            if (previousOption != gTasks[taskId].data[TD_FRAMETYPE])
			{
				PlaySE(SE_SELECT);
				FrameType_DrawChoices(gTasks[taskId].data[TD_FRAMETYPE], gTasks[taskId].data[TD_LANGUAGE]);
			}
            break;
        case MENUITEM_RUN:
            previousOption = gTasks[taskId].data[TD_RUN];
            gTasks[taskId].data[TD_RUN] = AutoRun_ProcessInput(gTasks[taskId].data[TD_RUN]);

            if (previousOption != gTasks[taskId].data[TD_RUN])
			{
				PlaySE(SE_SELECT);
				AutoRun_DrawChoices(gTasks[taskId].data[TD_RUN], gTasks[taskId].data[TD_LANGUAGE]);
			}
            break;
        case MENUITEM_LANGUAGE:
            previousOption = gTasks[taskId].data[TD_LANGUAGE];
            gTasks[taskId].data[TD_LANGUAGE] = Language_ProcessInput(gTasks[taskId].data[TD_LANGUAGE]);

            if (previousOption != gTasks[taskId].data[TD_LANGUAGE])
			{
				PlaySE(SE_SELECT);
				Language_DrawChoices(gTasks[taskId].data[TD_LANGUAGE], gTasks[taskId].data[TD_LANGUAGE]);
			}
            break;
        default:
            return;
        }
    }
}

static void OptionMenu_ClearWindow(u8 option)
{
	if (option == DESCRIPTION)
	{	
		FillWindowPixelRect(WIN_DESCRIPTION, 0, 8, 120, 224, 32);
	}
	else if (option == FRAME)
	{	
		FillWindowPixelRect(WIN_TEXT_OPTION, 0, 142, 80, 16, 16);
	}
	else if (option == TEXT)
	{	
		FillWindowPixelRect(WIN_TEXT_OPTION, 0, 0, 0, 240, 128);
		FillWindowPixelRect(WIN_DESCRIPTION, 0, 0, 0, 240, 160);
	}
}

static void Task_OptionMenuSave(u8 taskId)
{
	SetDescription(8, gTasks[taskId].data[TD_LANGUAGE]);
	if (gTasks[taskId].data[TD_TIMER])
    {
        gTasks[taskId].data[TD_TIMER]--;
    }
    else
	{
		gSaveBlock2Ptr->optionsTextSpeed = gTasks[taskId].data[TD_TEXTSPEED];
		gSaveBlock2Ptr->optionsBattleSceneOff = gTasks[taskId].data[TD_BATTLESCENE];
		gSaveBlock2Ptr->optionsBattleStyle = gTasks[taskId].data[TD_BATTLESTYLE];
		gSaveBlock2Ptr->optionsSound = gTasks[taskId].data[TD_SOUND];
		gSaveBlock2Ptr->optionsButtonMode = gTasks[taskId].data[TD_BUTTONMODE];
		gSaveBlock2Ptr->optionsWindowFrameType = gTasks[taskId].data[TD_FRAMETYPE];
		gSaveBlock2Ptr->optionsAutorun = gTasks[taskId].data[TD_RUN];
		gSaveBlock2Ptr->optionsLanguage = gTasks[taskId].data[TD_LANGUAGE];

		BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, 0);
		gTasks[taskId].func = Task_OptionMenuFadeOut;
	}
}

static void Task_OptionMenuCancel(u8 taskId)
{
	SetDescription(9, gTasks[taskId].data[TD_LANGUAGE]);
	if (gTasks[taskId].data[TD_TIMER])
    {
        gTasks[taskId].data[TD_TIMER]--;
    }
    else
	{
		BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, 0);
		gTasks[taskId].func = Task_OptionMenuFadeOut;
	}
}

static void Task_OptionMenuFadeOut(u8 taskId)
{
    u8 i;
	if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style)
{
    u8 dst[160];
    u16 i;

    for (i = 0; *text != EOS && i <= 160; i++)
        dst[i] = *(text++);
	
	dst[2] = style;
    dst[i] = EOS;
    AddTextPrinterParameterized(WIN_TEXT_OPTION, 1, dst, x, y + 1, TEXT_SPEED_FF, NULL);
	CopyWindowToVram(WIN_TEXT_OPTION, 2);
}

static void SetDescription(u8 selection, u8 language)
{
	if (language == ENG)
		ShowDescription(sOptionMenuDescriptions[selection], selection);
	if (language == SPA)
		ShowDescription(sOptionMenuDescriptionsSpa[selection], selection);
}

static void ShowDescription(const u8 *text, u8 selection)
{
	u8 dst[160];
    u16 i;

    for (i = 0; *text != EOS && i <= 160; i++)
        dst[i] = *(text++);
	
	dst[2] = 5;
    dst[i] = EOS;
	AddTextPrinterParameterized4(WIN_DESCRIPTION, 1, 12, 120, 0, 0, 0, 0, dst);
}

static u8 TextSpeed_ProcessInput(u8 selection)
{
    if (gMain.newKeys & DPAD_RIGHT)
    {
        if (selection < 2)
            selection++;
        else
            selection = 0;

    }
    if (gMain.newKeys & DPAD_LEFT)
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;

    }
    return selection;
}

static void TextSpeed_DrawChoices(u8 selection, u8 language)
{
    u8 styles[3];

    styles[0] = 5;
    styles[1] = 5;
    styles[2] = 5;
    styles[selection] = 1;

    if (language == ENG)
	{
		DrawOptionMenuChoice(gText_TextSpeedSlow, 112, 0, styles[0]);
		DrawOptionMenuChoice(gText_TextSpeedMid,  150, 0, styles[1]);
		DrawOptionMenuChoice(gText_TextSpeedFast, 184, 0, styles[2]);
	}
    if (language == SPA)
	{
		DrawOptionMenuChoice(gText_TextSpeedSlowSpa, 112, 0, styles[0]);
		DrawOptionMenuChoice(gText_TextSpeedMidSpa,  154, 0, styles[1]);
		DrawOptionMenuChoice(gText_TextSpeedFastSpa, 196, 0, styles[2]);
	}
}

static u8 BattleScene_ProcessInput(u8 selection)
{
    if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
    }

    return selection;
}

static void BattleScene_DrawChoices(u8 selection, u8 language)
{
    u8 styles[2];

    styles[0] = 5;
    styles[1] = 5;
    styles[selection] = 1;

    if (language == ENG)
	{
		DrawOptionMenuChoice(gText_BattleSceneOn,  112, 16, styles[0]);
		DrawOptionMenuChoice(gText_BattleSceneOff, 162, 16, styles[1]);
	}
    if (language == SPA)
	{
		DrawOptionMenuChoice(gText_BattleSceneOnSpa,  112, 16, styles[0]);
		DrawOptionMenuChoice(gText_BattleSceneOffSpa, 162, 16, styles[1]);
	}
}

static u8 BattleStyle_ProcessInput(u8 selection)
{
    if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
    }

    return selection;
}

static void BattleStyle_DrawChoices(u8 selection, u8 language)
{
    u8 styles[2];

    styles[0] = 5;
    styles[1] = 5;
    styles[selection] = 1;

    if (language == ENG)
	{
		DrawOptionMenuChoice(gText_BattleStyleShift, 112, 32, styles[0]);
		DrawOptionMenuChoice(gText_BattleStyleSet,   162, 32, styles[1]);
	}
    if (language == SPA)
	{
		DrawOptionMenuChoice(gText_BattleStyleShiftSpa, 112, 32, styles[0]);
		DrawOptionMenuChoice(gText_BattleStyleSetSpa,   162, 32, styles[1]);
	}
}

static u8 Sound_ProcessInput(u8 selection)
{
    if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        SetPokemonCryStereo(selection);
    }

    return selection;
}

static void Sound_DrawChoices(u8 selection, u8 language)
{
    u8 styles[2];

    styles[0] = 5;
    styles[1] = 5;
    styles[selection] = 1;

    if (language == ENG)
	{
		DrawOptionMenuChoice(gText_SoundMono,   112, 48, styles[0]);
		DrawOptionMenuChoice(gText_SoundStereo, 162, 48, styles[1]);
	}
    if (language == SPA)
	{
		DrawOptionMenuChoice(gText_SoundMonoSpa,   112, 48, styles[0]);
		DrawOptionMenuChoice(gText_SoundStereoSpa, 162, 48, styles[1]);
	}
}

static u8 ButtonMode_ProcessInput(u8 selection)
{
    if (gMain.newKeys & DPAD_RIGHT)
    {
        if (selection <= 1)
            selection++;
        else
            selection = 0;
    }
    if (gMain.newKeys & DPAD_LEFT)
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;
    }
    return selection;
}

static void ButtonMode_DrawChoices(u8 selection, u8 language)
{
    u8 styles[3];

    styles[0] = 5;
    styles[1] = 5;
    styles[2] = 5;
    styles[selection] = 1;

    if (language == ENG)
	{
		DrawOptionMenuChoice(gText_ButtonTypeNormal,   112, 64, styles[0]);
		DrawOptionMenuChoice(gText_ButtonTypeLR,       162, 64, styles[1]);
		DrawOptionMenuChoice(gText_ButtonTypeLEqualsA, 188, 64, styles[2]);
	}
    if (language == SPA)
	{
		DrawOptionMenuChoice(gText_ButtonTypeNormalSpa,   112, 64, styles[0]);
		DrawOptionMenuChoice(gText_ButtonTypeLRSpa,       162, 64, styles[1]);
		DrawOptionMenuChoice(gText_ButtonTypeLEqualsASpa, 188, 64, styles[2]);
	}
}

static u8 FrameType_ProcessInput(u8 selection)
{
    if (gMain.newKeys & DPAD_RIGHT)
    {
        if (selection < 10 - 1)
            selection++;
        else
            selection = 0;
    }
    if (gMain.newKeys & DPAD_LEFT)
    {
        if (selection != 0)
            selection--;
        else
            selection = 10 - 1;
    }
	LoadBgTiles(3, GetUserFrameGraphicsInfo(selection)->tiles, 0x120, 24);
	LoadPalette(GetUserFrameGraphicsInfo(selection)->palette, 0xE0, 0x20);
    return selection;
}

static void FrameType_DrawChoices(u8 selection, u8 language)
{
    u8 text[16];
    u8 n = selection + 1;
    u16 i;

    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];

    // Convert a number to decimal string
    if (n / 10 != 0)
    {
        text[i] = n / 10 + CHAR_0;
        i++;
        text[i] = n % 10 + CHAR_0;
        i++;
        text[i] = CHAR_SPACE;
        i++;
    }
    else
    {
        text[i] = n % 10 + CHAR_0;
        i++;
        text[i] = 0x77;
        i++;
    }

    text[i] = EOS;
	
	OptionMenu_ClearWindow(FRAME);
	if (language == ENG)
		DrawOptionMenuChoice(gText_FrameType, 112, 80, 1);
	if (language == SPA)
		DrawOptionMenuChoice(gText_FrameTypeSpa, 112, 80, 1);
    DrawOptionMenuChoice(text, 142, 80, 1);
}

static u8 AutoRun_ProcessInput(u8 selection)
{
    if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
    }

    return selection;
}

static void AutoRun_DrawChoices(u8 selection, u8 language)
{
    u8 styles[2];

    styles[0] = 5;
    styles[1] = 5;
    styles[selection] = 1;

    if (language == ENG)
	{
		DrawOptionMenuChoice(gText_RunOff,   112, 96, styles[0]);
		DrawOptionMenuChoice(gText_RunOn, 162, 96, styles[1]);
	}
    if (language == SPA)
	{
		DrawOptionMenuChoice(gText_RunOffSpa,   112, 96, styles[0]);
		DrawOptionMenuChoice(gText_RunOnSpa, 162, 96, styles[1]);
	}
}

static u8 Language_ProcessInput(u8 selection)
{
    if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
    }

    return selection;
}

static void Language_DrawChoices(u8 selection, u8 language)
{
    u8 styles[2];
	
    styles[0] = 5;
    styles[1] = 5;
    styles[selection] = 1;

	OptionMenu_ClearWindow(TEXT);
	DrawOptionMenuTexts(gTasks[0].data[TD_LANGUAGE]);
	OptionMenuDrawChoices(0);
	if (gTasks[0].data[TD_MENUSELECTION] == MENUITEM_LANGUAGE)
		SetDescription(MENUITEM_LANGUAGE, language);
    if (language == ENG)
	{
		DrawOptionMenuChoice(gText_LanguageENG,   112, 112, styles[0]);
		DrawOptionMenuChoice(gText_LanguageSPA, 162, 112, styles[1]);
	}
    if (language == SPA)
	{
		DrawOptionMenuChoice(gText_LanguageENGSpa,   112, 112, styles[0]);
		DrawOptionMenuChoice(gText_LanguageSPASpa, 162, 112, styles[1]);
	}
}

static void OptionMenuDrawChoices(u8 taskId)
{
	TextSpeed_DrawChoices(gTasks[taskId].data[TD_TEXTSPEED], gTasks[taskId].data[TD_LANGUAGE]);
	BattleScene_DrawChoices(gTasks[taskId].data[TD_BATTLESCENE],gTasks[taskId].data[TD_LANGUAGE]);
    BattleStyle_DrawChoices(gTasks[taskId].data[TD_BATTLESTYLE], gTasks[taskId].data[TD_LANGUAGE]);
    Sound_DrawChoices(gTasks[taskId].data[TD_SOUND],gTasks[taskId].data[TD_LANGUAGE]);
    ButtonMode_DrawChoices(gTasks[taskId].data[TD_BUTTONMODE],gTasks[taskId].data[TD_LANGUAGE]);
    FrameType_DrawChoices(gTasks[taskId].data[TD_FRAMETYPE],gTasks[taskId].data[TD_LANGUAGE]);
    AutoRun_DrawChoices(gTasks[taskId].data[TD_RUN],gTasks[taskId].data[TD_LANGUAGE]);
        
	CopyWindowToVram(WIN_TEXT_OPTION, COPYWIN_BOTH);
}

static void DrawOptionMenuTexts(u8 language)
{
    u8 i;

    FillWindowPixelBuffer(WIN_TEXT_OPTION, PIXEL_FILL(0));
	if (language == ENG)
	{
		AddTextPrinterParameterized(WIN_DESCRIPTION, 1, gText_MenuOption, 8, 1, TEXT_SPEED_FF, NULL);
		AddTextPrinterParameterized(WIN_DESCRIPTION, 1, gText_Instructions, 140, 0 , TEXT_SPEED_FF, NULL);

		for (i = 0; i < MENUITEM_COUNT; i++)
		{
			AddTextPrinterParameterized(WIN_TEXT_OPTION, 1, sOptionMenuItemsNames[i], 8, (i * 16) + 1, TEXT_SPEED_FF, NULL);
		}
	}
	if (language == SPA)
	{
		AddTextPrinterParameterized(WIN_DESCRIPTION, 1, gText_MenuOptionSpa, 8, 1, TEXT_SPEED_FF, NULL);
		AddTextPrinterParameterized(WIN_DESCRIPTION, 1, gText_InstructionsSpa, 140, 0 , TEXT_SPEED_FF, NULL);

		for (i = 0; i < MENUITEM_COUNT; i++)
		{
			AddTextPrinterParameterized(WIN_TEXT_OPTION, 1, sOptionMenuItemsNamesSpa[i], 8, (i * 16) + 1, TEXT_SPEED_FF, NULL);
		}
	}
	
	CopyWindowToVram(WIN_TEXT_OPTION, COPYWIN_BOTH);
    CopyWindowToVram(WIN_DESCRIPTION, COPYWIN_BOTH);
}

