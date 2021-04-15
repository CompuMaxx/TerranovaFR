#include "global.h"
#include "gflib.h"
#include "menu.h"
#include "new_menu_helpers.h"
#include "pokemon_storage_system_internal.h"
#include "strings.h"
#include "constants/songs.h"

void InitMenu(void)
{
    gPSSData->menuItemsCount = 0;
    gPSSData->menuWidth = 0;
    gPSSData->menuWindow.bg = 0;
    gPSSData->menuWindow.paletteNum = 15;
    gPSSData->menuWindow.baseBlock = 92;
}

static const u8 *const sMenuTexts[] = {
    [PC_TEXT_CANCEL]     = gText_PC_Cancel,
    [PC_TEXT_STORE]      = gText_PC_Store,
    [PC_TEXT_WITHDRAW]   = gText_PC_Withdraw,
    [PC_TEXT_MOVE]       = gText_PC_Move,
    [PC_TEXT_SHIFT]      = gText_PC_Shift,
    [PC_TEXT_PLACE]      = gText_PC_Place,
    [PC_TEXT_SUMMARY]    = gText_PC_Summary,
    [PC_TEXT_RELEASE]    = gText_PC_Release,
    [PC_TEXT_MARK]       = gText_PC_Mark,
    [PC_TEXT_JUMP]       = gText_PC_Jump,
    [PC_TEXT_WALLPAPER]  = gText_PC_Wallpaper,
    [PC_TEXT_NAME]       = gText_PC_Name,
    [PC_TEXT_TAKE]       = gText_PC_Take,
    [PC_TEXT_GIVE]       = gText_PC_Give,
    [PC_TEXT_GIVE2]      = gText_PC_Give,
    [PC_TEXT_SWITCH]     = gText_PC_Switch,
    [PC_TEXT_BAG]        = gText_PC_Bag,
    [PC_TEXT_INFO]       = gText_PC_Info,
    [PC_TEXT_SCENERY1]   = gText_PC_Scenery1,
    [PC_TEXT_SCENERY2]   = gText_PC_Scenery2,
    [PC_TEXT_SCENERY3]   = gText_PC_Scenery3,
    [PC_TEXT_ETCETERA]   = gText_PC_Etcetera,
    [PC_TEXT_FOREST]     = gText_PC_Forest,
    [PC_TEXT_CITY]       = gText_PC_City,
    [PC_TEXT_DESERT]     = gText_PC_Desert,
    [PC_TEXT_SAVANNA]    = gText_PC_Savanna,
    [PC_TEXT_CRAG]       = gText_PC_Crag,
    [PC_TEXT_VOLCANO]    = gText_PC_Volcano,
    [PC_TEXT_SNOW]       = gText_PC_Snow,
    [PC_TEXT_CAVE]       = gText_PC_Cave,
    [PC_TEXT_BEACH]      = gText_PC_Beach,
    [PC_TEXT_SEAFLOOR]   = gText_PC_Seafloor,
    [PC_TEXT_RIVER]      = gText_PC_River,
    [PC_TEXT_SKY]        = gText_PC_Sky,
    [PC_TEXT_POLKADOT]   = gText_PC_PolkaDot,
    [PC_TEXT_POKECENTER] = gText_PC_Pokecenter,
    [PC_TEXT_MACHINE]    = gText_PC_Machine,
    [PC_TEXT_SIMPLE]     = gText_PC_Simple,
};

void SetMenuText(u8 textId)
{
    if (gPSSData->menuItemsCount < MAX_MENU_ITEMS)
    {
        u8 len;
        struct StorageMenu *menu = &gPSSData->menuItems[gPSSData->menuItemsCount];

        menu->text = sMenuTexts[textId];
        menu->textId = textId;
        len = StringLength(menu->text);
        if (len > gPSSData->menuWidth)
            gPSSData->menuWidth = len;

        gPSSData->menuItemsCount++;
    }
}

s8 sub_8094E50(u8 arg0)
{
    if (arg0 >= gPSSData->menuItemsCount)
        return -1;
    else
        return gPSSData->menuItems[arg0].textId;
}

void AddMenu(void)
{
    gPSSData->menuWindow.width = gPSSData->menuWidth + 2;
    gPSSData->menuWindow.height = 2 * gPSSData->menuItemsCount;
    gPSSData->menuWindow.tilemapLeft = 29 - gPSSData->menuWindow.width;
    gPSSData->menuWindow.tilemapTop = 15 - gPSSData->menuWindow.height;
    gPSSData->field_CB0 = AddWindow(&gPSSData->menuWindow);
    ClearWindowTilemap(gPSSData->field_CB0);
    DrawStdFrameWithCustomTileAndPalette(gPSSData->field_CB0, FALSE, 0x00b, 14);
    PrintTextArray(gPSSData->field_CB0, 1, 8, 2, 16, gPSSData->menuItemsCount, (void*)gPSSData->menuItems);
    Menu_InitCursor(gPSSData->field_CB0, 1, 0, 2, 16, gPSSData->menuItemsCount, 0);
    ScheduleBgCopyTilemapToVram(0);
    gPSSData->field_CAE = 0;
}

bool8 sub_8094F90(void)
{
    // Some debug flag?
    return FALSE;
}

s16 sub_8094F94(void)
{
    s32 textId = -2;

    do
    {
        if (JOY_NEW(A_BUTTON))
        {
            textId = Menu_GetCursorPos();
            break;
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            textId = -1;
        }

        if (JOY_NEW(DPAD_UP))
        {
            PlaySE(SE_SELECT);
            Menu_MoveCursor(-1);
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            PlaySE(SE_SELECT);
            Menu_MoveCursor(1);
        }
    } while (0);

    if (textId != -2)
        sub_8095024();

    if (textId >= 0)
        textId = gPSSData->menuItems[textId].textId;

    return textId;
}

void sub_8095024(void)
{
    ClearStdWindowAndFrameToTransparent(gPSSData->field_CB0, TRUE);
    RemoveWindow(gPSSData->field_CB0);
}
