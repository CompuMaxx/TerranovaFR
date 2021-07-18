#include "global.h"
#include "gflib.h"
#include "bag.h"
#include "battle_controllers.h"
#include "berry_pouch.h"
#include "decompress.h"
#include "event_scripts.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "graphics.h"
#include "help_system.h"
#include "item.h"
#include "item_menu.h"
#include "item_menu_icons.h"
#include "item_use.h"
#include "link.h"
#include "list_menu.h"
#include "load_save.h"
#include "mail_data.h"
#include "map_name_popup.h"
#include "menu.h"
#include "menu_indicators.h"
#include "money.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokemon_storage_system.h"
#include "scanline_effect.h"
#include "script.h"
#include "shop.h"
#include "strings.h"
#include "teachy_tv.h"
#include "tm_case.h"
#include "constants/items.h"
#include "constants/songs.h"

#define FREE_IF_SET(ptr) ({ if (ptr) Free(ptr); })

struct BagMenuAlloc
{
    MainCallback exitCB;
    u8 itemOriginalLocation;
    u8 pocketSwitchMode:4;
    u8 itemMenuIcon:2;
    u8 inhibitItemDescriptionPrint:2;
    u16 contextMenuSelectedItem;
    u8 pocketScrollArrowsTask;
    u8 pocketSwitchArrowsTask;
    u8 nItems[3];
    u8 maxShowed[3];
    u8 data[4];
	u8 lastSortingAlphabetically:1;
	u8 lastSortingByAmount:1;
	u8 lastSortingByType:1;
	u8 lastSortingByImportance:1;
	u8 lastSortingByQuality:1;
};

struct BagSlots
{
    struct ItemSlot bagPocket_Items[BAG_ITEMS_COUNT];
    struct ItemSlot bagPocket_KeyItems[BAG_KEYITEMS_COUNT];
    struct ItemSlot bagPocket_PokeBalls[BAG_POKEBALLS_COUNT];
    u16 itemsAbove[3];
    u16 cursorPos[3];
    u16 registeredItem;
    u16 pocket;
};

EWRAM_DATA struct BagStruct gBagMenuState = {};
static EWRAM_DATA struct BagMenuAlloc * sBagMenuDisplay = NULL;
static EWRAM_DATA void * sBagBgTilemapBuffer = NULL;
static EWRAM_DATA struct ListMenuItem * sListMenuItems = NULL;
static EWRAM_DATA u8 (*sListMenuItemStrings)[19] = NULL;
static EWRAM_DATA u8 sContextMenuItemsBuffer[5] = {};
static EWRAM_DATA const u8 *sContextMenuItemsPtr = NULL;
static EWRAM_DATA u8 sContextMenuNumItems = 0;
static EWRAM_DATA struct BagSlots * sBackupPlayerBag = NULL;
EWRAM_DATA u16 gSpecialVar_ItemId = ITEM_NONE;

static void CB2_OpenBagMenu(void);
static bool8 LoadBagMenuGraphics(void);
static void FadeOutOfBagMenu(void);
static void Task_WaitFadeOutOfBagMenu(u8 taskId);
static void NullBagMenuBufferPtrs(void);
static bool8 BagMenuInitBgsAndAllocTilemapBuffer(void);
static bool8 DoLoadBagGraphics(void);
static u8 CreateBagInputHandlerTask(u8 location);
static bool8 TryAllocListMenuBuffers(void);
static void Bag_BuildListMenuTemplate(u8 pocket);
static void BagListMenuGetItemNameColored(u8 *dest, u16 itemId);
static void BagListMenuMoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static void BagListMenuItemPrintFunc(u8 windowId, s32 itemId, u8 y);
static void bag_menu_print_cursor(u8 y, u8 colorIdx);
static void PrintBagPocketName(void);
static void PrintItemDescriptionOnMessageWindow(s32 itemIndex);
static void CreatePocketScrollArrowPair(void);
static void CreatePocketSwitchArrowPair(void);
static void BagDestroyPocketSwitchArrowPair(void);
static void CalculateInitialCursorPosAndItemsAbove(void);
static void UpdatePocketScrollPositions(void);
static void DestroyBagMenuResources(void);
static void Task_ItemMenu_WaitFadeAndSwitchToExitCallback(u8 taskId);
static void Task_AnimateWin0v(u8 taskId);
static void ShowBagOrBeginWin0OpenTask(void);
static void All_CalculateNItemsAndMaxShowed(void);
static void Task_BagMenu_HandleInput(u8 taskId);
static void Task_ItemContextMenuByLocation(u8 taskId);
static void Bag_FillMessageBoxWithPalette(u32 a0);
static u8 ProcessPocketSwitchInput(u8 taskId, u8 pocketId);
static void SwitchPockets(u8 taskId, s16 direction, bool16 a2);
static void Task_AnimateSwitchPockets(u8 taskId);
static void BeginMovingItemInPocket(u8 taskId, s16 itemIndex);
static void Task_MoveItemInPocket_HandleInput(u8 taskId);
static void ExecuteMoveItemInPocket(u8 taskId, u32 itemIndex);
static void AbortMovingItemInPocket(u8 taskId, u32 itemIndex);
static void CopyBagListBgTileRowToTilemapBuffer(u8 a0);
static void Task_ItemContext_FieldOrBattle(u8 taskId);
static void Task_FieldItemContextMenuHandleInput(u8 taskId);
static void Task_ItemMenuAction_Use(u8 taskId);
static void Task_ItemMenuAction_Toss(u8 taskId);
static void Task_ConfirmTossItems(u8 taskId);
static void Task_TossItem_No(u8 taskId);
static void Task_SelectQuantityToToss(u8 taskId);
static void Task_TossItem_Yes(u8 taskId);
static void Task_WaitAB_RedrawAndReturnToBag(u8 taskId);
static void Task_ItemMenuAction_ToggleSelect(u8 taskId);
static void Task_ItemMenuAction_Give(u8 taskId);
static void Task_PrintThereIsNoPokemon(u8 taskId);
static void Task_ItemMenuAction_Cancel(u8 taskId);
static void Task_PrintItemCantBeHeld(u8 taskId);
static void Task_WaitAButtonAndCloseContextMenu(u8 taskId);
static void Task_ItemMenuAction_BattleUse(u8 taskId);
static void Task_ItemContext_FieldGive(u8 taskId);
static void GoToTMCase_Give(void);
static void GoToBerryPouch_Give(void);
static void ReturnToBagMenuFromSubmenu_Give(void);
static void Task_ItemContext_PcBoxGive(u8 taskId);
static void GoToTMCase_PCBox(void);
static void GoToBerryPouch_PCBox(void);
static void ReturnToBagMenuFromSubmenu_PCBox(void);
static void Task_ItemContext_Sell(u8 taskId);
static void GoToTMCase_Sell(void);
static void GoToBerryPouch_Sell(void);
static void ReturnToBagMenuFromSubmenu_Sell(void);
static void Task_PrintSaleConfirmationText(u8 taskId);
static void Task_ShowSellYesNoMenu(u8 taskId);
static void Task_SellItem_No(u8 taskId);
static void Task_InitSaleQuantitySelectInterface(u8 taskId);
static void UpdateSalePriceDisplay(s32 price);
static void Task_SelectQuantityToSell(u8 taskId);
static void Task_SellItem_Yes(u8 taskId);
static void Task_FinalizeSaleToShop(u8 taskId);
static void Task_WaitPressAB_AfterSell(u8 taskId);
static void Task_ItemContext_Deposit(u8 taskId);
static void Task_SelectQuantityToDeposit(u8 taskId);
static void Task_TryDoItemDeposit(u8 taskId);
static bool8 BagIsTutorial(void);
static void Task_Bag_OldManTutorial(u8 taskId);
static void Task_Pokedude_FadeFromBag(u8 taskId);
static void Task_Pokedude_WaitFadeAndExitBag(u8 taskId);
static void Task_Bag_TeachyTvRegister(u8 taskId);
static void Task_Bag_TeachyTvCatching(u8 taskId);
static void Task_Bag_TeachyTvStatus(u8 taskId);
static void Task_Bag_TeachyTvTMs(u8 taskId);
//bag sort
static void Task_LoadBagSortOptions(u8 taskId);
static void ItemMenu_SortByName(u8 taskId);
static void ItemMenu_SortByType(u8 taskId);
static void ItemMenu_SortByImportance(u8 taskId);
static void ItemMenu_SortByQuality(u8 taskId);
static void ItemMenu_SortByAmount(u8 taskId);
static void SortBagItems(u8 taskId);
static void Task_SortFinish(u8 taskId);
static void SortItemsInBag(u8 pocket, u8 type);
static void MergeSort(struct ItemSlot* array, u32 low, u32 high, s8 (*comparator)(struct ItemSlot*, struct ItemSlot*));
static void Merge(struct ItemSlot* array, u32 low, u32 mid, u32 high, s8 (*comparator)(struct ItemSlot*, struct ItemSlot*));
static s8 CompareItemsAlphabetically(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2);
static s8 CompareItemsByMost(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2);
static s8 CompareItemsByType(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2);
static s8 CompareItemsByImportance(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2);
static s8 CompareItemsByQuality(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2);
void GoToBagMenu(u8 location, u8 pocket, MainCallback bagCallback);

static const struct BgTemplate sBgTemplates[2] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }, {
        .bg = 1,
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0x000
    }
};

static const u8 *const sPocketNames[] = {
    gText_Items2,
    gText_KeyItems2,
    gText_PokeBalls2
};

static const u8 *const sPocketNamesSpa[] = {
    gText_Items2Spa,
    gText_KeyItems2Spa,
    gText_PokeBalls2Spa
};

static const u16 sBagListBgTiles[][18] = {
    INCBIN_U16("graphics/item_menu/bagmap_0.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_1.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_2.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_3.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_4.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_5.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_6.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_7.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_8.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_9.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_A.bin"),
    INCBIN_U16("graphics/item_menu/bagmap_B.bin")
};

static const u8 sMenuText_ByName[] = _("Name");
static const u8 sMenuText_ByImportance[] = _("Value");
static const u8 sMenuText_ByQuality[] = _("Quality");
static const u8 sMenuText_ByType[] = _("Type");
static const u8 sMenuText_ByAmount[] = _("Amount");
static const u8 sMenuText_ByNumber[] = _("Number");
static const u8 sText_NothingToSort[] = _("There's nothing to sort!");

static const u8 sMenuText_ByNameSpa[] = _("Nombre");
static const u8 sMenuText_ByImportanceSpa[] = _("Interés");
static const u8 sMenuText_ByQualitySpa[] = _("Calidad");
static const u8 sMenuText_ByTypeSpa[] = _("Tipo");
static const u8 sMenuText_ByAmountSpa[] = _("Cantidad");
static const u8 sMenuText_ByNumberSpa[] = _("Número");
static const u8 sText_NothingToSortSpa[] = _("¡Aquí no hay nada que ordenar!");

static const struct MenuAction sItemMenuContextActions[] = {
    [ITEMMENUACTION_USE] = {gText_Use, {.void_u8 = Task_ItemMenuAction_Use}},
    [ITEMMENUACTION_TOSS] = {gText_Toss, {.void_u8 = Task_ItemMenuAction_Toss}},
    [ITEMMENUACTION_REGISTER] = {gText_Register, {.void_u8 = Task_ItemMenuAction_ToggleSelect}},
    [ITEMMENUACTION_GIVE] = {gText_Give, {.void_u8 = Task_ItemMenuAction_Give}},
    [ITEMMENUACTION_CANCEL] = {gText_FameChecker_Cancel, {.void_u8 = Task_ItemMenuAction_Cancel}},
    [ITEMMENUACTION_BATTLE_USE] = {gText_Use, {.void_u8 = Task_ItemMenuAction_BattleUse}},
    [ITEMMENUACTION_CHECK] = {gText_Check, {.void_u8 = Task_ItemMenuAction_Use}},
    [ITEMMENUACTION_OPEN] = {gText_Open, {.void_u8 = Task_ItemMenuAction_Use}},
    [ITEMMENUACTION_OPEN_BERRIES] = {gText_Open, {.void_u8 = Task_ItemMenuAction_BattleUse}},
    [ITEMMENUACTION_WALK] = {gText_Walk, {.void_u8 = Task_ItemMenuAction_Use}},
    [ITEMMENUACTION_DESELECT] = {gText_Deselect, {.void_u8 = Task_ItemMenuAction_ToggleSelect}},
    [ITEMMENUACTION_BY_NAME] = {sMenuText_ByName, ItemMenu_SortByName},
    [ITEMMENUACTION_BY_TYPE] = {sMenuText_ByType, ItemMenu_SortByType},
    [ITEMMENUACTION_BY_IMPORTANCE] = {sMenuText_ByImportance, ItemMenu_SortByImportance},
    [ITEMMENUACTION_BY_QUALITY] = {sMenuText_ByQuality, ItemMenu_SortByQuality},
    [ITEMMENUACTION_BY_AMOUNT] = {sMenuText_ByAmount, ItemMenu_SortByAmount},
    [ITEMMENUACTION_DUMMY] = {gText_StringDummy, {.void_u8 = NULL}}
};

static const struct MenuAction sItemMenuContextActionsSpa[] = {
    [ITEMMENUACTION_USE] = {gText_UseSpa, {.void_u8 = Task_ItemMenuAction_Use}},
    [ITEMMENUACTION_TOSS] = {gText_TossSpa, {.void_u8 = Task_ItemMenuAction_Toss}},
    [ITEMMENUACTION_REGISTER] = {gText_AsignarSpa, {.void_u8 = Task_ItemMenuAction_ToggleSelect}},
    [ITEMMENUACTION_GIVE] = {gText_GiveSpa, {.void_u8 = Task_ItemMenuAction_Give}},
    [ITEMMENUACTION_CANCEL] = {gText_FameChecker_CancelSpa, {.void_u8 = Task_ItemMenuAction_Cancel}},
    [ITEMMENUACTION_BATTLE_USE] = {gText_UseSpa, {.void_u8 = Task_ItemMenuAction_BattleUse}},
    [ITEMMENUACTION_CHECK] = {gText_CheckSpa, {.void_u8 = Task_ItemMenuAction_Use}},
    [ITEMMENUACTION_OPEN] = {gText_OpenSpa, {.void_u8 = Task_ItemMenuAction_Use}},
    [ITEMMENUACTION_OPEN_BERRIES] = {gText_OpenSpa, {.void_u8 = Task_ItemMenuAction_BattleUse}},
    [ITEMMENUACTION_WALK] = {gText_WalkSpa, {.void_u8 = Task_ItemMenuAction_Use}},
    [ITEMMENUACTION_DESELECT] = {gText_DeselectSpa, {.void_u8 = Task_ItemMenuAction_ToggleSelect}},
    [ITEMMENUACTION_BY_NAME] = {sMenuText_ByNameSpa, ItemMenu_SortByName},
    [ITEMMENUACTION_BY_TYPE] = {sMenuText_ByTypeSpa, ItemMenu_SortByType},
    [ITEMMENUACTION_BY_IMPORTANCE] = {sMenuText_ByImportanceSpa, ItemMenu_SortByImportance},
    [ITEMMENUACTION_BY_QUALITY] = {sMenuText_ByQualitySpa, ItemMenu_SortByQuality},
    [ITEMMENUACTION_BY_AMOUNT] = {sMenuText_ByAmountSpa, ItemMenu_SortByAmount},
    [ITEMMENUACTION_DUMMY] = {gText_StringDummySpa, {.void_u8 = NULL}}
};

static const u8 sContextMenuItems_Field[][4] = {
    {
        ITEMMENUACTION_USE,
        ITEMMENUACTION_GIVE,
        ITEMMENUACTION_TOSS,
        ITEMMENUACTION_CANCEL
    }, {
        ITEMMENUACTION_USE,
        ITEMMENUACTION_REGISTER,
        ITEMMENUACTION_CANCEL,
        ITEMMENUACTION_DUMMY
    }, {
        ITEMMENUACTION_GIVE,
        ITEMMENUACTION_TOSS,
        ITEMMENUACTION_CANCEL,
        ITEMMENUACTION_DUMMY
    },
};

static const u8 sContextMenuItems_CheckGiveTossCancel[] = {
    ITEMMENUACTION_CHECK,
    ITEMMENUACTION_GIVE,
    ITEMMENUACTION_TOSS,
    ITEMMENUACTION_CANCEL
};

static const u8 sContextMenuItems_GiveIfNotKeyItemPocket[][2] = {
    {
        ITEMMENUACTION_GIVE,
        ITEMMENUACTION_CANCEL
    }, {
        ITEMMENUACTION_CANCEL,
        ITEMMENUACTION_DUMMY
    }, {
        ITEMMENUACTION_GIVE,
        ITEMMENUACTION_CANCEL
    }
};

static const u8 sContextMenuItems_Open[] = {
    ITEMMENUACTION_OPEN,
    ITEMMENUACTION_CANCEL
};

static const u8 sContextMenuItems_BattleUse[] = {
    ITEMMENUACTION_BATTLE_USE,
    ITEMMENUACTION_CANCEL
};

static const u8 sContextMenuItems_Cancel[] = {
    ITEMMENUACTION_CANCEL,
    ITEMMENUACTION_DUMMY
};

static const TaskFunc sItemContextTaskFuncs[] = {
    [ITEMMENULOCATION_FIELD]  = Task_ItemContext_FieldOrBattle,
    [ITEMMENULOCATION_PARTY]  = Task_ItemContext_FieldGive,
    [ITEMMENULOCATION_SHOP]   = Task_ItemContext_Sell,
    [ITEMMENULOCATION_ITEMPC] = Task_ItemContext_Deposit,
    [ITEMMENULOCATION_PCBOX]  = Task_ItemContext_PcBoxGive,
    [ITEMMENULOCATION_BATTLE] = Task_ItemContext_FieldOrBattle,
    [ITEMMENULOCATION_OLD_MAN] = NULL
};

static const struct YesNoFuncTable sYesNoMenu_Toss = {
    Task_TossItem_Yes,
    Task_TossItem_No
};

static const struct YesNoFuncTable sYesNoMenu_Sell = {
    Task_SellItem_Yes,
    Task_SellItem_No
};

static const u8 sListItemTextColor_RegularItem[] = _("{COLOR_HIGHLIGHT_SHADOW DARK_GRAY TRANSPARENT LIGHT_GRAY}");

static const u8 sListItemTextColor_TmCase_BerryPouch[] = _("{COLOR_HIGHLIGHT_SHADOW BLUE TRANSPARENT LIGHT_BLUE}");

static const struct ScrollArrowsTemplate sPocketSwitchArrowPairTemplate = {
    .firstArrowType = SCROLL_ARROW_LEFT,
    .firstX = 8,
    .firstY = 72,
    .secondArrowType = SCROLL_ARROW_RIGHT,
    .secondX = 72,
    .secondY = 72,
    .fullyUpThreshold = 0,
    .fullyDownThreshold = 2,
    .tileTag = 111,
    .palTag = 111,
    .palNum = 0,
};

static const u8 sBlit_SelectButton[] = INCBIN_U8("graphics/interface/select_button.4bpp");

void GoToBagMenu(u8 location, u8 pocket, MainCallback bagCallback)
{
    u8 i;

    NullBagMenuBufferPtrs();
    sBagMenuDisplay = Alloc(sizeof(struct BagMenuAlloc));
    if (sBagMenuDisplay == NULL)
        SetMainCallback2(bagCallback);
    else
    {
        if (location != ITEMMENULOCATION_LAST)
            gBagMenuState.location = location;
        if (bagCallback != NULL)
            gBagMenuState.bagCallback = bagCallback;
        sBagMenuDisplay->exitCB = NULL;
        sBagMenuDisplay->itemOriginalLocation = 0xFF;
        sBagMenuDisplay->itemMenuIcon = 0;
        sBagMenuDisplay->inhibitItemDescriptionPrint = FALSE;
        sBagMenuDisplay->pocketScrollArrowsTask = 0xFF;
        sBagMenuDisplay->pocketSwitchArrowsTask = 0xFF;
        if (location == ITEMMENULOCATION_ITEMPC)
            sBagMenuDisplay->pocketSwitchMode = 1;
        else if (location == ITEMMENULOCATION_OLD_MAN)
            sBagMenuDisplay->pocketSwitchMode = 2;
        else
            sBagMenuDisplay->pocketSwitchMode = 0;
        for (i = 0; i < 4; i++)
        {
            sBagMenuDisplay->data[i] = 0;
        }
        if (pocket == OPEN_BAG_ITEMS || pocket == OPEN_BAG_KEYITEMS || pocket == OPEN_BAG_POKEBALLS)
            gBagMenuState.pocket = pocket;
        gTextFlags.autoScroll = FALSE;
        gSpecialVar_ItemId = ITEM_NONE;
        SetMainCallback2(CB2_OpenBagMenu);
    }
}

void CB2_BagMenuFromStartMenu(void)
{
    GoToBagMenu(ITEMMENULOCATION_FIELD, OPEN_BAG_LAST, CB2_ReturnToFieldWithOpenMenu);
}

void CB2_BagMenuFromBattle(void)
{
    GoToBagMenu(ITEMMENULOCATION_BATTLE, OPEN_BAG_LAST, SetCB2ToReshowScreenAfterMenu2);
}

static void CB2_BagMenuRun(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB_BagMenuRun(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_OpenBagMenu(void)
{
    while (1)
    {
        if (MenuHelpers_CallLinkSomething() == TRUE)
            break;
        if (LoadBagMenuGraphics() == TRUE)
            break;
        if (MenuHelpers_LinkSomething() == TRUE)
            break;
    }
}

static bool8 LoadBagMenuGraphics(void)
{
    u8 taskId;

    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        gMain.state++;
        break;
    case 2:
        FreeAllSpritePalettes();
        gMain.state++;
        break;
    case 3:
        ResetPaletteFade();
        gPaletteFade.bufferTransferDisabled = TRUE;
        gMain.state++;
        break;
    case 4:
        ResetSpriteData();
        gMain.state++;
        break;
    case 5:
        ResetItemMenuIconState();
        gMain.state++;
        break;
    case 6:
        if (!MenuHelpers_LinkSomething())
        {
            ResetTasks();
        }
        gMain.state++;
        break;
    case 7:
        if (BagMenuInitBgsAndAllocTilemapBuffer())
        {
            sBagMenuDisplay->data[0] = 0;
            gMain.state++;
        }
        else
        {
            FadeOutOfBagMenu();
            return TRUE;
        }
        break;
    case 8:
        if (DoLoadBagGraphics() == TRUE)
            gMain.state++;
        break;
    case 9:
        InitBagWindows();
        gMain.state++;
        break;
    case 10:
        All_CalculateNItemsAndMaxShowed();
        CalculateInitialCursorPosAndItemsAbove();
        UpdatePocketScrollPositions();
        gMain.state++;
        break;
    case 11:
        if (!TryAllocListMenuBuffers())
        {
            FadeOutOfBagMenu();
            return TRUE;
        }
        gMain.state++;
        break;
    case 12:
        Bag_BuildListMenuTemplate(gBagMenuState.pocket);
        gMain.state++;
        break;
    case 13:
        if (gBagMenuState.location != ITEMMENULOCATION_ITEMPC)
            PrintBagPocketName();
        else
            BagDrawDepositItemTextBox();
        gMain.state++;
        break;
    case 14:
        taskId = CreateBagInputHandlerTask(gBagMenuState.location);
        gTasks[taskId].data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagMenuState.cursorPos[gBagMenuState.pocket], gBagMenuState.itemsAbove[gBagMenuState.pocket]);
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[8] = 0;
        gMain.state++;
        break;
    case 15:
        CreateBagOrSatchelSprite(gBagMenuState.pocket);
        gMain.state++;
        break;
    case 16:
        CreatePocketScrollArrowPair();
        CreatePocketSwitchArrowPair();
        gMain.state++;
        break;
    case 17:
        ItemMenuIcons_CreateInsertIndicatorBarHidden();
        gMain.state++;
        break;
    case 18:
        ShowBagOrBeginWin0OpenTask();
        gMain.state++;
        break;
    case 19:
        if (gBagMenuState.location == ITEMMENULOCATION_ITEMPC)
            SetHelpContext(HELPCONTEXT_PLAYERS_PC_ITEMS);
        else
            SetHelpContext(HELPCONTEXT_BAG);
        gPaletteFade.bufferTransferDisabled = FALSE;
        gMain.state++;
        break;
    default:
        SetVBlankCallback(VBlankCB_BagMenuRun);
        SetMainCallback2(CB2_BagMenuRun);
        return TRUE;
    }
    return FALSE;
}

static void FadeOutOfBagMenu(void)
{
    BeginNormalPaletteFade(0xFFFFFFFF, -2, 0, 16, RGB_BLACK);
    CreateTask(Task_WaitFadeOutOfBagMenu, 0);
    SetVBlankCallback(VBlankCB_BagMenuRun);
    SetMainCallback2(CB2_BagMenuRun);
}

static void Task_WaitFadeOutOfBagMenu(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(gBagMenuState.bagCallback);
        DestroyBagMenuResources();
        DestroyTask(taskId);
    }
}

static void NullBagMenuBufferPtrs(void)
{
    sBagMenuDisplay = NULL;
    sBagBgTilemapBuffer = NULL;
    sListMenuItems = NULL;
    sListMenuItemStrings = NULL;
}

static bool8 BagMenuInitBgsAndAllocTilemapBuffer(void)
{
    void **buff;
    ResetAllBgsCoordinatesAndBgCntRegs();
    buff = &sBagBgTilemapBuffer;
    *buff = Alloc(0x800);
    if (*buff == NULL)
        return FALSE;
    memset(*buff, 0, 0x800);
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, sBgTemplates, NELEMS(sBgTemplates));
    SetBgTilemapBuffer(1, *buff);
    ScheduleBgCopyTilemapToVram(1);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
    ShowBg(0);
    ShowBg(1);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    return TRUE;
}

static bool8 DoLoadBagGraphics(void)
{
    switch (sBagMenuDisplay->data[0])
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, gUnknown_8E830CC, 0, 0, 0);
        sBagMenuDisplay->data[0]++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            if (gBagMenuState.location != ITEMMENULOCATION_ITEMPC)
                LZDecompressWram(gUnknown_8E832C0, sBagBgTilemapBuffer);
            else
                LZDecompressWram(gUnknown_8E83444, sBagBgTilemapBuffer);
            sBagMenuDisplay->data[0]++;
        }
        break;
    case 2:
        LoadCompressedPalette(gBagBgPalette, 0x00, 0x60);
        if (!BagIsTutorial() && gSaveBlock2Ptr->playerGender != MALE)
            LoadCompressedPalette(gBagBgPalette_FemaleOverride, 0x00, 0x20);
        sBagMenuDisplay->data[0]++;
        break;
    case 3:
        if (BagIsTutorial() == TRUE || gSaveBlock2Ptr->playerGender == MALE)
            LoadCompressedSpriteSheet(&gSpriteSheet_Backpack);
        else
            LoadCompressedSpriteSheet(&gSpriteSheet_Satchel);
        sBagMenuDisplay->data[0]++;
        break;
    case 4:
        LoadCompressedSpritePalette(&gSpritePalette_BagOrSatchel);
        sBagMenuDisplay->data[0]++;
        break;
    case 5:
        LoadCompressedSpriteSheet(&gBagSwapSpriteSheet);
        sBagMenuDisplay->data[0]++;
        break;
    default:
        LoadCompressedSpritePalette(&gBagSwapSpritePalette);
        sBagMenuDisplay->data[0] = 0;
        return TRUE;
    }

    return FALSE;
}

static u8 CreateBagInputHandlerTask(u8 location)
{
    switch (location)
    {
    case ITEMMENULOCATION_OLD_MAN:
        return CreateTask(Task_Bag_OldManTutorial, 0);
    case ITEMMENULOCATION_TTVSCR_REGISTER:
        return CreateTask(Task_Bag_TeachyTvRegister, 0);
    case ITEMMENULOCATION_TTVSCR_TMS:
        return CreateTask(Task_Bag_TeachyTvTMs, 0);
    case ITEMMENULOCATION_TTVSCR_STATUS:
        return CreateTask(Task_Bag_TeachyTvStatus, 0);
    case ITEMMENULOCATION_TTVSCR_CATCHING:
        return CreateTask(Task_Bag_TeachyTvCatching, 0);
    default:
        return CreateTask(Task_BagMenu_HandleInput, 0);
    }
}

static bool8 TryAllocListMenuBuffers(void)
{
    // The items pocket has the highest capacity, + 1 for CANCEL
    sListMenuItems = Alloc((BAG_ITEMS_COUNT + 1) * sizeof(struct ListMenuItem));
    if (sListMenuItems == NULL)
        return FALSE;
    sListMenuItemStrings = Alloc((BAG_ITEMS_COUNT + 1) * sizeof(*sListMenuItemStrings));
    if (sListMenuItemStrings == NULL)
        return FALSE;
    return TRUE;
}

static void Bag_BuildListMenuTemplate(u8 pocket)
{
    u16 i;
    struct BagPocket * bagPocket = &gBagPockets[pocket];
    for (i = 0; i < sBagMenuDisplay->nItems[pocket]; i++)
    {
        BagListMenuGetItemNameColored(sListMenuItemStrings[i], bagPocket->itemSlots[i].itemId);
        sListMenuItems[i].label = sListMenuItemStrings[i];
        sListMenuItems[i].index = i;
    }
    StringCopy(sListMenuItemStrings[i], sListItemTextColor_RegularItem);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringAppend(sListMenuItemStrings[i], gText_FameChecker_Cancel);
    else
		StringAppend(sListMenuItemStrings[i], gText_FameChecker_CancelSpa);
    sListMenuItems[i].label = sListMenuItemStrings[i];
    sListMenuItems[i].index = i;
    gMultiuseListMenuTemplate.items = sListMenuItems;
    gMultiuseListMenuTemplate.totalItems = sBagMenuDisplay->nItems[pocket] + 1;
    gMultiuseListMenuTemplate.windowId = 0;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 9;
    gMultiuseListMenuTemplate.cursor_X = 1;
    gMultiuseListMenuTemplate.lettersSpacing = 0;
    gMultiuseListMenuTemplate.itemVerticalPadding = 2;
    gMultiuseListMenuTemplate.upText_Y = 2;
    gMultiuseListMenuTemplate.maxShowed = sBagMenuDisplay->maxShowed[pocket];
    gMultiuseListMenuTemplate.fontId = 2;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 0;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.moveCursorFunc = BagListMenuMoveCursorFunc;
    gMultiuseListMenuTemplate.itemPrintFunc = BagListMenuItemPrintFunc;
    gMultiuseListMenuTemplate.cursorKind = 0;
    gMultiuseListMenuTemplate.scrollMultiple = 0;
}

static void BagListMenuGetItemNameColored(u8 *dest, u16 itemId)
{
    if (itemId == ITEM_TM_CASE || itemId == ITEM_BERRY_POUCH)
        StringCopy(dest, sListItemTextColor_TmCase_BerryPouch);
    else
        StringCopy(dest, sListItemTextColor_RegularItem);
    StringAppend(dest, ItemId_GetName(itemId));
}

static void BagListMenuMoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    if (onInit != TRUE)
    {
        PlaySE(SE_BAG_CURSOR);
        sub_8098580();
    }
    if (sBagMenuDisplay->itemOriginalLocation == 0xFF)
    {
        DestroyItemMenuIcon(sBagMenuDisplay->itemMenuIcon ^ 1);
        if (sBagMenuDisplay->nItems[gBagMenuState.pocket] != itemIndex)
            CreateItemMenuIcon(BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, itemIndex), sBagMenuDisplay->itemMenuIcon);
        else
            CreateItemMenuIcon(ITEM_N_A, sBagMenuDisplay->itemMenuIcon);
        sBagMenuDisplay->itemMenuIcon ^= 1;
        if (!sBagMenuDisplay->inhibitItemDescriptionPrint)
            PrintItemDescriptionOnMessageWindow(itemIndex);
    }
}

static void BagListMenuItemPrintFunc(u8 windowId, s32 itemId, u8 y)
{
    u16 bagItemId;
    u16 bagItemQuantity;
    if (sBagMenuDisplay->itemOriginalLocation != 0xFF)
    {
        if (sBagMenuDisplay->itemOriginalLocation == (u8)itemId)
            bag_menu_print_cursor(y, 2);
        else
            bag_menu_print_cursor(y, 0xFF);
    }
    if (itemId != -2 && sBagMenuDisplay->nItems[gBagMenuState.pocket] != itemId)
    {
        bagItemId = BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, itemId);
        bagItemQuantity = BagGetQuantityByPocketPosition(gBagMenuState.pocket + 1, itemId);
        if (gBagMenuState.pocket != POCKET_KEY_ITEMS - 1 && !itemid_is_unique(bagItemId))
        {
            ConvertIntToDecimalStringN(gStringVar1, bagItemQuantity, STR_CONV_MODE_RIGHT_ALIGN, 3);
            StringExpandPlaceholders(gStringVar4, gText_TimesStrVar1);
            BagPrintTextOnWindow(windowId, 0, gStringVar4, 0x6e, y, 0, 0, 0xFF, 1);
        }
        else if (gSaveBlock1Ptr->registeredItem != ITEM_NONE && gSaveBlock1Ptr->registeredItem == bagItemId)
        {
            BlitBitmapToWindow(windowId, sBlit_SelectButton, 0x70, y, 0x18, 0x10);
        }
    }
}

static void bag_menu_print_cursor_(u8 taskId, u8 colorIdx)
{
    bag_menu_print_cursor(ListMenuGetYCoordForPrintingArrowCursor(taskId), colorIdx);
}

static void bag_menu_print_cursor(u8 y, u8 colorIdx)
{
    if (colorIdx == 0xFF)
    {
        FillWindowPixelRect(0, PIXEL_FILL(0), 1, y, GetMenuCursorDimensionByFont(2, 0), GetMenuCursorDimensionByFont(2, 1));
    }
    else
    {
        BagPrintTextOnWindow(0, 2, gText_SelectorArrow2, 1, y, 0, 0, 0, colorIdx);
    }
}

static void PrintBagPocketName(void)
{
    FillWindowPixelBuffer(2, PIXEL_FILL(0));
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		BagPrintTextOnWin1CenteredColor0(sPocketNames[gBagMenuState.pocket], gBagMenuState.pocket);
    else
		BagPrintTextOnWin1CenteredColor0(sPocketNamesSpa[gBagMenuState.pocket], gBagMenuState.pocket);
}

static void PrintItemDescriptionOnMessageWindow(s32 itemIndex)
{
    const u8 *description;
    if (itemIndex != sBagMenuDisplay->nItems[gBagMenuState.pocket])
        description = ItemId_GetDescription(BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, itemIndex));
    else
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			description = gText_CloseBag;
        else
			description = gText_CloseBagSpa;
    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    BagPrintTextOnWindow(1, 2, description, 0, 3, 2, 0, 0, 0);
}



static void CreatePocketScrollArrowPair(void)
{
    sBagMenuDisplay->pocketScrollArrowsTask = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP,
        160,
        8,
        104,
        sBagMenuDisplay->nItems[gBagMenuState.pocket] - sBagMenuDisplay->maxShowed[gBagMenuState.pocket] + 1,
        110,
        110,
        &gBagMenuState.cursorPos[gBagMenuState.pocket]
    );
}

static void CreatePocketSwitchArrowPair(void)
{
    if (sBagMenuDisplay->pocketSwitchMode != 1)
    {
        sBagMenuDisplay->pocketSwitchArrowsTask = AddScrollIndicatorArrowPair(&sPocketSwitchArrowPairTemplate, &gBagMenuState.pocket);
    }
}

static void CreatePocketScrollArrowPair_SellQuantity(void)
{
    sBagMenuDisplay->contextMenuSelectedItem = 1;
    sBagMenuDisplay->pocketScrollArrowsTask = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP,
        152,
        72,
        104,
        2,
        110,
        110,
        &sBagMenuDisplay->contextMenuSelectedItem
    );
}

static void CreateArrowPair_QuantitySelect(void)
{
    sBagMenuDisplay->contextMenuSelectedItem = 1;
    sBagMenuDisplay->pocketScrollArrowsTask = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP,
        212,
        120,
        152,
        2,
        110,
        110,
        &sBagMenuDisplay->contextMenuSelectedItem
    );
}

static void BagDestroyPocketScrollArrowPair(void)
{
    if (sBagMenuDisplay->pocketScrollArrowsTask != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(sBagMenuDisplay->pocketScrollArrowsTask);
        sBagMenuDisplay->pocketScrollArrowsTask = 0xFF;
    }
    BagDestroyPocketSwitchArrowPair();
}

static void BagDestroyPocketSwitchArrowPair(void)
{
    if (sBagMenuDisplay->pocketSwitchArrowsTask != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(sBagMenuDisplay->pocketSwitchArrowsTask);
        sBagMenuDisplay->pocketSwitchArrowsTask = 0xFF;
    }
}

void ResetBagCursorPositions(void)
{
    u8 i;
    gBagMenuState.pocket = POCKET_ITEMS - 1;
    gBagMenuState.bagOpen = FALSE;
    for (i = 0; i < 3; i++)
    {
        gBagMenuState.itemsAbove[i] = 0;
        gBagMenuState.cursorPos[i] = 0;
    }
}

void PocketCalculateInitialCursorPosAndItemsAbove(u8 pocketId)
{
    if (gBagMenuState.cursorPos[pocketId] != 0 && gBagMenuState.cursorPos[pocketId] + sBagMenuDisplay->maxShowed[pocketId] > sBagMenuDisplay->nItems[pocketId] + 1)
    {
        gBagMenuState.cursorPos[pocketId] = (sBagMenuDisplay->nItems[pocketId] + 1) - sBagMenuDisplay->maxShowed[pocketId];
    }
    if (gBagMenuState.cursorPos[pocketId] + gBagMenuState.itemsAbove[pocketId] >= sBagMenuDisplay->nItems[pocketId] + 1)
    {
        if (sBagMenuDisplay->nItems[pocketId] + 1 < 2)
            gBagMenuState.itemsAbove[pocketId] = 0;
        else
            gBagMenuState.itemsAbove[pocketId] = sBagMenuDisplay->nItems[pocketId];
    }
}

static void CalculateInitialCursorPosAndItemsAbove(void)
{
    u8 i;
    for (i = 0; i < 3; i++)
    {
        PocketCalculateInitialCursorPosAndItemsAbove(i);
    }
}

static void UpdatePocketScrollPositions(void)
{
    u8 i;
    u8 j;

    for (i = 0; i < 3; i++)
    {
        if (gBagMenuState.itemsAbove[i] > 3)
        {
            for (j = 0; j <= gBagMenuState.itemsAbove[i] - 3; gBagMenuState.itemsAbove[i]--, gBagMenuState.cursorPos[i]++, j++)
            {
                if (gBagMenuState.cursorPos[i] + sBagMenuDisplay->maxShowed[i] == sBagMenuDisplay->nItems[i] + 1)
                    break;
            }
        }
    }
}

static void DestroyBagMenuResources(void)
{
    FREE_IF_SET(sBagMenuDisplay);
    FREE_IF_SET(sBagBgTilemapBuffer);
    FREE_IF_SET(sListMenuItems);
    FREE_IF_SET(sListMenuItemStrings);
    FreeAllWindowBuffers();
}

void ItemMenu_StartFadeToExitCallback(u8 taskId)
{
    BeginNormalPaletteFade(0xFFFFFFFF, -2, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_ItemMenu_WaitFadeAndSwitchToExitCallback;
}

static void Task_ItemMenu_WaitFadeAndSwitchToExitCallback(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!gPaletteFade.active && FuncIsActiveTask(Task_AnimateWin0v) != TRUE)
    {
        DestroyListMenuTask(data[0], &gBagMenuState.cursorPos[gBagMenuState.pocket], &gBagMenuState.itemsAbove[gBagMenuState.pocket]);
        if (sBagMenuDisplay->exitCB != NULL)
            SetMainCallback2(sBagMenuDisplay->exitCB);
        else
            SetMainCallback2(gBagMenuState.bagCallback);
        BagDestroyPocketScrollArrowPair();
        DestroyBagMenuResources();
        DestroyTask(taskId);
    }
}

static void ShowBagOrBeginWin0OpenTask(void)
{
    u16 paldata = RGB_BLACK;
    u8 taskId;

    LoadPalette(&paldata, 0x00, 0x02);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR);
    BlendPalettes(0xFFFFFFFF, 16, RGB_BLACK);
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
    if (gBagMenuState.bagOpen == TRUE)
    {
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 240));
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, 0));
    }
    else
    {
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 240));
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, 160));
        taskId = CreateTask(Task_AnimateWin0v, 0);
        gTasks[taskId].data[0] = 192;
        gTasks[taskId].data[1] = -16;
        gBagMenuState.bagOpen = TRUE;
    }
}

void Bag_BeginCloseWin0Animation(void)
{

    u8 taskId = CreateTask(Task_AnimateWin0v, 0);
    gTasks[taskId].data[0] = -16;
    gTasks[taskId].data[1] =  16;
    gBagMenuState.bagOpen = FALSE;
}

void CB2_SetUpReshowBattleScreenAfterMenu(void)
{
    gBagMenuState.bagOpen = FALSE;
}

static void Task_AnimateWin0v(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    data[0] += data[1];
    if (data[0] > 160)
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, 160));
    else
        SetGpuReg(REG_OFFSET_WIN0V, data[0]);
    if ((data[1] == 16 && data[0] == 160) || (data[1] == -16 && data[0] == 0))
        DestroyTask(taskId);
}

void MoveItemSlotInList(struct ItemSlot * itemSlots_, u32 from, u32 to_)
{
    // dumb assignments needed to match
    struct ItemSlot *itemSlots = itemSlots_;
    u32 to = to_;

    if (from != to)
    {
        s16 i, count;
        struct ItemSlot firstSlot = itemSlots[from];

        if (to > from)
        {
            to--;
            for (i = from, count = to; i < count; i++)
                itemSlots[i] = itemSlots[i + 1];
        }
        else
        {
            for (i = from, count = to; i > count; i--)
                itemSlots[i] = itemSlots[i - 1];
        }
        itemSlots[to] = firstSlot;
    }
}

void Pocket_CalculateNItemsAndMaxShowed(u8 pocketId)
{
    u16 i;
    struct BagPocket * pocket = &gBagPockets[pocketId];
    BagPocketCompaction(pocket->itemSlots, pocket->capacity);
    sBagMenuDisplay->nItems[pocketId] = 0;
    for (i = 0; i < pocket->capacity; i++)
    {
        if (pocket->itemSlots[i].itemId == ITEM_NONE)
            break;
        sBagMenuDisplay->nItems[pocketId]++;
    }
    if (sBagMenuDisplay->nItems[pocketId] + 1 > 6)
        sBagMenuDisplay->maxShowed[pocketId] = 6;
    else
        sBagMenuDisplay->maxShowed[pocketId] = sBagMenuDisplay->nItems[pocketId] + 1;
}

static void All_CalculateNItemsAndMaxShowed(void)
{
    u8 i;
    for (i = 0; i < 3; i++)
        Pocket_CalculateNItemsAndMaxShowed(i);
}

void DisplayItemMessageInBag(u8 taskId, u8 fontId, const u8 * string, TaskFunc followUpFunc)
{
    s16 *data = gTasks[taskId].data;
    data[10] = OpenBagWindow(5);
    FillWindowPixelBuffer(data[10], PIXEL_FILL(1));
    DisplayMessageAndContinueTask(taskId, data[10], 0x06D, 0x0D, fontId, GetTextSpeedSetting(), string, followUpFunc);
    ScheduleBgCopyTilemapToVram(0);
}

void ItemMenu_SetExitCallback(MainCallback cb)
{
    sBagMenuDisplay->exitCB = cb;
}

static u8 GetSelectedItemIndex(u8 pocket)
{
    return gBagMenuState.cursorPos[pocket] + gBagMenuState.itemsAbove[pocket];
}

static void Task_BagMenu_HandleInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 cursorPos;
    u16 itemsAbove;
    s32 input;

    if (gPaletteFade.active)
        return;
    if (FuncIsActiveTask(Task_AnimateWin0v) == TRUE)
        return;
    if (MenuHelpers_CallLinkSomething() == TRUE)
        return;
    switch (ProcessPocketSwitchInput(taskId, gBagMenuState.pocket))
    {
    case 1:
        SwitchPockets(taskId, -1, FALSE);
        return;
    case 2:
        SwitchPockets(taskId,  1, FALSE);
        return;
    default:
        if (JOY_NEW(SELECT_BUTTON) && gBagMenuState.location == ITEMMENULOCATION_FIELD)
        {
            ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
            if (cursorPos + itemsAbove != sBagMenuDisplay->nItems[gBagMenuState.pocket])
            {
                PlaySE(SE_SELECT);
                BeginMovingItemInPocket(taskId, cursorPos + itemsAbove);
                return;
            }
        }
        else if (JOY_NEW(START_BUTTON))
        {
			if ((sBagMenuDisplay->nItems[gBagMenuState.pocket]) <= 1) //can't sort with 0 or 1 item in bag
			{
				PlaySE(SE_FAILURE);
				if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DisplayItemMessageInBag(taskId, 1, sText_NothingToSort, Task_WaitAButtonAndCloseContextMenu);
				if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DisplayItemMessageInBag(taskId, 1, sText_NothingToSortSpa, Task_WaitAButtonAndCloseContextMenu);
                break;
			}
            data[1] = ListMenu_ProcessInput(data[0]);
            data[2] = BagGetQuantityByPocketPosition(gBagMenuState.pocket + 1, data[1]);
            if (cursorPos + itemsAbove != sBagMenuDisplay->nItems[gBagMenuState.pocket])
                gSpecialVar_ItemId = BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, data[1]);
            else
                break;

            PlaySE(SE_SELECT);
            BagDestroyPocketScrollArrowPair();
            bag_menu_print_cursor_(data[0], 2);
            ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
            gTasks[taskId].func = Task_LoadBagSortOptions;
        }
        else
        {
            input = ListMenu_ProcessInput(data[0]);
            ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
            switch (input)
            {
                case LIST_NOTHING_CHOSEN:
                    return;
                case LIST_CANCEL:
                    // if BERRY CRUSH
                    //    PlaySE(SE_FAILURE);
                    //    break;
                    PlaySE(SE_SELECT);
                    gSpecialVar_ItemId = ITEM_NONE;
                    gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
                default:
                    PlaySE(SE_SELECT);
                    BagDestroyPocketScrollArrowPair();
                    bag_menu_print_cursor_(data[0], 2);
                    data[1] = input;
                    data[2] = BagGetQuantityByPocketPosition(gBagMenuState.pocket + 1, input);
                    gSpecialVar_ItemId = BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, input);
                    gTasks[taskId].func = Task_ItemContextMenuByLocation;
                    break;
            }
        }
        break;
    }
    input = ListMenu_ProcessInput(data[0]);
    ListMenuGetScrollAndRow(data[0], &gBagMenuState.cursorPos[gBagMenuState.pocket], &gBagMenuState.itemsAbove[gBagMenuState.pocket]);
    switch (input)
    {
    case LIST_NOTHING_CHOSEN:
        return;
    case LIST_CANCEL:
        PlaySE(SE_SELECT);
        gSpecialVar_ItemId = ITEM_NONE;
        Bag_BeginCloseWin0Animation();
        gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
        break;
    default:
        PlaySE(SE_SELECT);
        if (input == sBagMenuDisplay->nItems[gBagMenuState.pocket])
        {
            gSpecialVar_ItemId = ITEM_NONE;
            Bag_BeginCloseWin0Animation();
            gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
        }
        else
        {
            BagDestroyPocketScrollArrowPair();
            bag_menu_print_cursor_(data[0], 2);
            data[1] = input;
            data[2] = BagGetQuantityByPocketPosition(gBagMenuState.pocket + 1, input);
            gSpecialVar_ItemId = BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, input);
            gTasks[taskId].func = Task_ItemContextMenuByLocation;
        }
        break;
    }
}

static void Task_ItemContextMenuByLocation(u8 taskId)
{
    Bag_FillMessageBoxWithPalette(1);
    sItemContextTaskFuncs[gBagMenuState.location](taskId);
}

static void Task_RedrawArrowsAndReturnToBagMenuSelect(u8 taskId)
{
    Bag_FillMessageBoxWithPalette(0);
    CreatePocketScrollArrowPair();
    CreatePocketSwitchArrowPair();
    gTasks[taskId].func = Task_BagMenu_HandleInput;
}

static void Bag_FillMessageBoxWithPalette(u32 a0)
{
    SetBgTilemapPalette(1, 0, 14, 30, 6, a0 + 1);
    ScheduleBgCopyTilemapToVram(1);
}

static u8 ProcessPocketSwitchInput(u8 taskId, u8 pocketId)
{
    u8 lrState;
    if (sBagMenuDisplay->pocketSwitchMode != 0)
        return 0;
    lrState = GetLRKeysState();
    if (JOY_NEW(DPAD_LEFT) || lrState == 1)
    {
        if (pocketId == POCKET_ITEMS - 1)
            return 0;
        PlaySE(SE_BAG_POCKET);
        return 1;
    }
    if (JOY_NEW(DPAD_RIGHT) || lrState == 2)
    {
        if (pocketId >= POCKET_POKE_BALLS - 1)
            return 0;
        PlaySE(SE_BAG_POCKET);
        return 2;
    }
    return 0;
}

static void SwitchPockets(u8 taskId, s16 direction, bool16 a2)
{
    s16 *data = gTasks[taskId].data;
    data[13] = 0;
    data[12] = 0;
    data[11] = direction;
    if (!a2)
    {
        ClearWindowTilemap(0);
        ClearWindowTilemap(1);
        ClearWindowTilemap(2);
        DestroyListMenuTask(data[0], &gBagMenuState.cursorPos[gBagMenuState.pocket], &gBagMenuState.itemsAbove[gBagMenuState.pocket]);
        ScheduleBgCopyTilemapToVram(0);
        DestroyItemMenuIcon(sBagMenuDisplay->itemMenuIcon ^ 1);
        BagDestroyPocketScrollArrowPair();
    }
    FillBgTilemapBufferRect_Palette0(1, 0x02D, 11, 1, 18, 12);
    ScheduleBgCopyTilemapToVram(1);
    sub_8098528(gBagMenuState.pocket + direction);
    SetTaskFuncWithFollowupFunc(taskId, Task_AnimateSwitchPockets, gTasks[taskId].func);
}

static void Task_AnimateSwitchPockets(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!MenuHelpers_LinkSomething() && !BagIsTutorial())
    {
        switch (ProcessPocketSwitchInput(taskId, gBagMenuState.pocket + data[11]))
        {
        case 1:
            gBagMenuState.pocket += data[11];
            SwitchTaskToFollowupFunc(taskId);
            SwitchPockets(taskId, -1, TRUE);
            return;
        case 2:
            gBagMenuState.pocket += data[11];
            SwitchTaskToFollowupFunc(taskId);
            SwitchPockets(taskId,  1, TRUE);
            return;
        }
    }
    switch (data[13])
    {
    case 0:
        if (data[12] != 0x7FFF)
        {
            data[12]++;
            CopyBagListBgTileRowToTilemapBuffer(data[12]);
            if (data[12] == 12)
                data[12] = 0x7FFF;
        }
        if (data[12] == 0x7FFF)
            data[13]++;
        break;
    case 1:
        gBagMenuState.pocket += data[11];
        PrintBagPocketName();
        Bag_BuildListMenuTemplate(gBagMenuState.pocket);
        data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagMenuState.cursorPos[gBagMenuState.pocket], gBagMenuState.itemsAbove[gBagMenuState.pocket]);
        PutWindowTilemap(1);
        PutWindowTilemap(2);
        ScheduleBgCopyTilemapToVram(0);
        CreatePocketScrollArrowPair();
        CreatePocketSwitchArrowPair();
        SwitchTaskToFollowupFunc(taskId);
        break;
    }
}

static void BeginMovingItemInPocket(u8 taskId, s16 itemIndex)
{
    u16 itemsAbove;
    u16 cursorPos;
    s16 *data = gTasks[taskId].data;
    ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
    ListMenuSetUnkIndicatorsStructField(data[0], 0x10, 1);
    data[1] = itemIndex;
    sBagMenuDisplay->itemOriginalLocation = itemIndex;
    StringCopy(gStringVar1, ItemId_GetName(BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, data[1])));
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringExpandPlaceholders(gStringVar4, gText_WhereShouldTheStrVar1BePlaced);
    else
		StringExpandPlaceholders(gStringVar4, gText_WhereShouldTheStrVar1BePlacedSpa);
    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    BagPrintTextOnWindow(1, 2, gStringVar4, 0, 3, 2, 0, 0, 0);
    ItemMenuIcons_MoveInsertIndicatorBar(0, ListMenuGetYCoordForPrintingArrowCursor(data[0]));
    ItemMenuIcons_ToggleInsertIndicatorBarVisibility(FALSE);
    BagDestroyPocketSwitchArrowPair();
    bag_menu_print_cursor_(data[0], 2);
    gTasks[taskId].func = Task_MoveItemInPocket_HandleInput;
}

static void Task_MoveItemInPocket_HandleInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s32 input;
    u16 itemsAbove;
    u16 cursorPos;
    if (MenuHelpers_CallLinkSomething() == TRUE)
        return;
    input = ListMenu_ProcessInput(data[0]);
    ListMenuGetScrollAndRow(data[0], &gBagMenuState.cursorPos[gBagMenuState.pocket], &gBagMenuState.itemsAbove[gBagMenuState.pocket]);
    ItemMenuIcons_MoveInsertIndicatorBar(0, ListMenuGetYCoordForPrintingArrowCursor(data[0]));
    if (JOY_NEW(SELECT_BUTTON))
    {
        PlaySE(SE_SELECT);
        sBagMenuDisplay->itemOriginalLocation = 0xFF;
        ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
        ExecuteMoveItemInPocket(taskId, cursorPos + itemsAbove);
        return;
    }
    switch (input)
    {
    case LIST_NOTHING_CHOSEN:
        return;
    case LIST_CANCEL:
        PlaySE(SE_SELECT);
        sBagMenuDisplay->itemOriginalLocation = 0xFF;
        ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
        AbortMovingItemInPocket(taskId, cursorPos + itemsAbove);
        break;
    default:
        PlaySE(SE_SELECT);
        sBagMenuDisplay->itemOriginalLocation = 0xFF;
        ExecuteMoveItemInPocket(taskId, input);
        break;
    }
}

static void ExecuteMoveItemInPocket(u8 taskId, u32 itemIndex)
{
    s16 *data = gTasks[taskId].data;
    if (data[1] == itemIndex || data[1] == itemIndex - 1)
    {
        AbortMovingItemInPocket(taskId, itemIndex);
    }
    else
    {
        MoveItemSlotInList(gBagPockets[gBagMenuState.pocket].itemSlots, data[1], itemIndex);
        DestroyListMenuTask(data[0], &gBagMenuState.cursorPos[gBagMenuState.pocket], &gBagMenuState.itemsAbove[gBagMenuState.pocket]);
        if (data[1] < itemIndex)
            gBagMenuState.itemsAbove[gBagMenuState.pocket]--;
        Bag_BuildListMenuTemplate(gBagMenuState.pocket);
        data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagMenuState.cursorPos[gBagMenuState.pocket], gBagMenuState.itemsAbove[gBagMenuState.pocket]);
        ItemMenuIcons_ToggleInsertIndicatorBarVisibility(TRUE);
        CreatePocketSwitchArrowPair();
        gTasks[taskId].func = Task_BagMenu_HandleInput;
    }
}

static void AbortMovingItemInPocket(u8 taskId, u32 itemIndex)
{
    s16 *data = gTasks[taskId].data;
    DestroyListMenuTask(data[0], &gBagMenuState.cursorPos[gBagMenuState.pocket], &gBagMenuState.itemsAbove[gBagMenuState.pocket]);
    if (data[1] < itemIndex)
        gBagMenuState.itemsAbove[gBagMenuState.pocket]--;
    Bag_BuildListMenuTemplate(gBagMenuState.pocket);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagMenuState.cursorPos[gBagMenuState.pocket], gBagMenuState.itemsAbove[gBagMenuState.pocket]);
    ItemMenuIcons_ToggleInsertIndicatorBarVisibility(TRUE);
    CreatePocketSwitchArrowPair();
    gTasks[taskId].func = Task_BagMenu_HandleInput;
}

static void InitQuantityToTossOrDeposit(u16 cursorPos, const u8 *str)
{
    u8 r4;
    u8 r5 = ShowBagWindow(6, 2);
    CopyItemName(BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, cursorPos), gStringVar1);
    StringExpandPlaceholders(gStringVar4, str);
    BagPrintTextOnWindow(r5, 2, gStringVar4, 0, 2, 1, 0, 0, 1);
    r4 = ShowBagWindow(0, 0);
    ConvertIntToDecimalStringN(gStringVar1, 1, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringExpandPlaceholders(gStringVar4, gText_TimesStrVar1);
    BagPrintTextOnWindow(r4, 0, gStringVar4, 4, 10, 1, 0, 0, 1);
    CreateArrowPair_QuantitySelect();
}

static void UpdateQuantityToTossOrDeposit(s16 value, u8 ndigits)
{
    u8 r6 = GetBagWindow(0);
    FillWindowPixelBuffer(r6, PIXEL_FILL(1));
    ConvertIntToDecimalStringN(gStringVar1, value, STR_CONV_MODE_LEADING_ZEROS, ndigits);
    StringExpandPlaceholders(gStringVar4, gText_TimesStrVar1);
    BagPrintTextOnWindow(r6, 0, gStringVar4, 4, 10, 1, 0, 0, 1);
}

static void CopyBagListBgTileRowToTilemapBuffer(u8 frame)
{
    CopyToBgTilemapBufferRect(1, sBagListBgTiles[12 - frame], 11, 13 - frame, 18, 1);
    ScheduleBgCopyTilemapToVram(1);
}

static void OpenContextMenu(u8 taskId)
{
    u8 r6;
    u8 r4;
    switch (gBagMenuState.location)
    {
    case ITEMMENULOCATION_BATTLE:
    case ITEMMENULOCATION_TTVSCR_STATUS:
        if (gSpecialVar_ItemId == ITEM_BERRY_POUCH)
        {
            sContextMenuItemsBuffer[0] = ITEMMENUACTION_OPEN_BERRIES;
            sContextMenuItemsBuffer[1] = ITEMMENUACTION_CANCEL;
            sContextMenuItemsPtr = sContextMenuItemsBuffer;
            sContextMenuNumItems = 2;
        }
        else if (ItemId_GetBattleUsage(gSpecialVar_ItemId))
        {
            sContextMenuItemsPtr = sContextMenuItems_BattleUse;
            sContextMenuNumItems = 2;
        }
        else
        {
            sContextMenuItemsPtr = sContextMenuItems_Cancel;
            sContextMenuNumItems = 1;
        }
        break;
    case ITEMMENULOCATION_OLD_MAN:
    case ITEMMENULOCATION_TTVSCR_CATCHING:
        sContextMenuItemsPtr = sContextMenuItems_BattleUse;
        sContextMenuNumItems = 2;
        break;
    default:
        if (MenuHelpers_LinkSomething() == TRUE || InUnionRoom() == TRUE)
        {
            if (gSpecialVar_ItemId == ITEM_TM_CASE || gSpecialVar_ItemId == ITEM_BERRY_POUCH)
            {
                sContextMenuItemsPtr = sContextMenuItems_Open;
                sContextMenuNumItems = 2;
            }
            else
            {
                if (gBagMenuState.pocket == POCKET_KEY_ITEMS - 1)
                    sContextMenuNumItems = 1;
                else
                    sContextMenuNumItems = 2;
                sContextMenuItemsPtr = sContextMenuItems_GiveIfNotKeyItemPocket[gBagMenuState.pocket];
            }
        }
        else
        {
            switch (gBagMenuState.pocket)
            {
            case OPEN_BAG_ITEMS:
                sContextMenuNumItems = 4;
                if (ItemIsMail(gSpecialVar_ItemId) == TRUE)
                    sContextMenuItemsPtr = sContextMenuItems_CheckGiveTossCancel;
                else
                    sContextMenuItemsPtr = sContextMenuItems_Field[gBagMenuState.pocket];
                break;
            case OPEN_BAG_KEYITEMS:
                sContextMenuItemsPtr = sContextMenuItemsBuffer;
                sContextMenuNumItems = 3;
                sContextMenuItemsBuffer[2] = ITEMMENUACTION_CANCEL;
                if (gSaveBlock1Ptr->registeredItem == gSpecialVar_ItemId)
                    sContextMenuItemsBuffer[1] = ITEMMENUACTION_DESELECT;
                else
                    sContextMenuItemsBuffer[1] = ITEMMENUACTION_REGISTER;
                if (gSpecialVar_ItemId == ITEM_TM_CASE || gSpecialVar_ItemId == ITEM_BERRY_POUCH)
                    sContextMenuItemsBuffer[0] = ITEMMENUACTION_OPEN;
                else if (gSpecialVar_ItemId == ITEM_BICYCLE && TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ACRO_BIKE | PLAYER_AVATAR_FLAG_MACH_BIKE))
                    sContextMenuItemsBuffer[0] = ITEMMENUACTION_WALK;
                else
                    sContextMenuItemsBuffer[0] = ITEMMENUACTION_USE;
                break;
            case OPEN_BAG_POKEBALLS:
                sContextMenuItemsPtr = sContextMenuItems_Field[gBagMenuState.pocket];
                sContextMenuNumItems = 3;
                break;
            }
        }
    }
    r6 = ShowBagWindow(10, sContextMenuNumItems - 1);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		AddItemMenuActionTextPrinters(r6, 2, GetMenuCursorDimensionByFont(2, 0), 2, GetFontAttribute(2, FONTATTR_LETTER_SPACING), GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT) + 2, sContextMenuNumItems, sItemMenuContextActions, sContextMenuItemsPtr);
    else
		AddItemMenuActionTextPrinters(r6, 2, GetMenuCursorDimensionByFont(2, 0), 2, GetFontAttribute(2, FONTATTR_LETTER_SPACING), GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT) + 2, sContextMenuNumItems, sItemMenuContextActionsSpa, sContextMenuItemsPtr);
    Menu_InitCursor(r6, 2, 0, 2, GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT) + 2, sContextMenuNumItems, 0);
    r4 = ShowBagWindow(6, 0);
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringExpandPlaceholders(gStringVar4, gText_Var1IsSelected);
    else
		StringExpandPlaceholders(gStringVar4, gText_Var1IsSelectedSpa);
    BagPrintTextOnWindow(r4, 2, gStringVar4, 0, 2, 1, 0, 0, 1);
}

static void Task_ItemContext_FieldOrBattle(u8 taskId)
{
    OpenContextMenu(taskId);
    gTasks[taskId].func = Task_FieldItemContextMenuHandleInput;
}

static void Task_FieldItemContextMenuHandleInput(u8 taskId)
{
    s8 input;
    if (MenuHelpers_CallLinkSomething() != TRUE)
    {
        input = Menu_ProcessInputNoWrapAround();
        switch (input)
        {
        case -1:
            PlaySE(SE_SELECT);
            sItemMenuContextActions[ITEMMENUACTION_CANCEL].func.void_u8(taskId);
            break;
        case -2:
            break;
        default:
            PlaySE(SE_SELECT);
            sItemMenuContextActions[sContextMenuItemsPtr[input]].func.void_u8(taskId);
            break;
        }
    }
}

static void Task_ItemMenuAction_Use(u8 taskId)
{
    if (ItemId_GetFieldFunc(gSpecialVar_ItemId) != NULL)
    {
        HideBagWindow(10);
        HideBagWindow(6);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        if (CalculatePlayerPartyCount() == 0 && ItemId_GetType(gSpecialVar_ItemId) == 1)
            Task_PrintThereIsNoPokemon(taskId);
        else
            ItemId_GetFieldFunc(gSpecialVar_ItemId)(taskId);
    }
}

static void Task_ItemMenuAction_Toss(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    ClearWindowTilemap(GetBagWindow(10));
    ClearWindowTilemap(GetBagWindow(6));
    HideBagWindow(10);
    HideBagWindow(6);
    PutWindowTilemap(0);
    data[8] = 1;
    if (data[2] == 1)
    {
        Task_ConfirmTossItems(taskId);
    }
    else
    {
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			InitQuantityToTossOrDeposit(data[1], gText_TossOutHowManyStrVar1s);
        else
			InitQuantityToTossOrDeposit(data[1], gText_TossOutHowManyStrVar1sSpa);
        gTasks[taskId].func = Task_SelectQuantityToToss;
    }
}

static void Task_ConfirmTossItems(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    ConvertIntToDecimalStringN(gStringVar2, data[8], STR_CONV_MODE_LEFT_ALIGN, 3);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringExpandPlaceholders(gStringVar4, gText_ThrowAwayStrVar2OfThisItemQM);
    else
		StringExpandPlaceholders(gStringVar4, gText_ThrowAwayStrVar2OfThisItemQMSpa);
    BagPrintTextOnWindow(ShowBagWindow(6, 1), 2, gStringVar4, 0, 2, 1, 0, 0, 1);
    BagCreateYesNoMenuBottomRight(taskId, &sYesNoMenu_Toss);
}

static void Task_TossItem_No(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    HideBagWindow(6);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    bag_menu_print_cursor_(data[0], 1);
    Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
}

static void Task_SelectQuantityToToss(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (AdjustQuantityAccordingToDPadInput(&data[8], data[2]) == TRUE)
    {
        UpdateQuantityToTossOrDeposit(data[8], 3);
    }
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        ClearWindowTilemap(GetBagWindow(6));
        HideBagWindow(6);
        HideBagWindow(0);
        ScheduleBgCopyTilemapToVram(0);
        BagDestroyPocketScrollArrowPair();
        Task_ConfirmTossItems(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideBagWindow(6);
        HideBagWindow(0);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        bag_menu_print_cursor_(data[0], 1);
        BagDestroyPocketScrollArrowPair();
        Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
    }
}

static void Task_TossItem_Yes(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    HideBagWindow(6);
    CopyItemName(BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, data[1]), gStringVar1);
    ConvertIntToDecimalStringN(gStringVar2, data[8], STR_CONV_MODE_LEFT_ALIGN, 3);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringExpandPlaceholders(gStringVar4, gText_ThrewAwayStrVar2StrVar1s);
    else
		StringExpandPlaceholders(gStringVar4, gText_ThrewAwayStrVar2StrVar1sSpa);
    BagPrintTextOnWindow(ShowBagWindow(6, 3), 2, gStringVar4, 0, 2, 1, 0, 0, 1);
    gTasks[taskId].func = Task_WaitAB_RedrawAndReturnToBag;
}

static void Task_WaitAB_RedrawAndReturnToBag(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        RemoveBagItem(gSpecialVar_ItemId, data[8]);
        HideBagWindow(6);
        DestroyListMenuTask(data[0], &gBagMenuState.cursorPos[gBagMenuState.pocket], &gBagMenuState.itemsAbove[gBagMenuState.pocket]);
        Pocket_CalculateNItemsAndMaxShowed(gBagMenuState.pocket);
        PocketCalculateInitialCursorPosAndItemsAbove(gBagMenuState.pocket);
        Bag_BuildListMenuTemplate(gBagMenuState.pocket);
        data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagMenuState.cursorPos[gBagMenuState.pocket], gBagMenuState.itemsAbove[gBagMenuState.pocket]);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        bag_menu_print_cursor_(data[0], 1);
        Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
    }
}

static void Task_ItemMenuAction_ToggleSelect(u8 taskId)
{
    u16 itemId;
    s16 *data = gTasks[taskId].data;
    itemId = BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, data[1]);
    if (gSaveBlock1Ptr->registeredItem == itemId)
        gSaveBlock1Ptr->registeredItem = ITEM_NONE;
    else
        gSaveBlock1Ptr->registeredItem = itemId;

    DestroyListMenuTask(data[0], &gBagMenuState.cursorPos[gBagMenuState.pocket], &gBagMenuState.itemsAbove[gBagMenuState.pocket]);
    Bag_BuildListMenuTemplate(gBagMenuState.pocket);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagMenuState.cursorPos[gBagMenuState.pocket], gBagMenuState.itemsAbove[gBagMenuState.pocket]);
    CopyWindowToVram(0, COPYWIN_MAP);
    Task_ItemMenuAction_Cancel(taskId);
}

static void Task_ItemMenuAction_Give(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 itemId = BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, data[1]);
    HideBagWindow(10);
    HideBagWindow(6);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    CopyWindowToVram(0, COPYWIN_MAP);
    if (!CanWriteMailHere(itemId))
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			DisplayItemMessageInBag(taskId, 2, gText_CantWriteMailHere, Task_WaitAButtonAndCloseContextMenu);
        else
			DisplayItemMessageInBag(taskId, 2, gText_CantWriteMailHere, Task_WaitAButtonAndCloseContextMenu);
	}
    else if (!itemid_is_unique(itemId))
    {
        if (CalculatePlayerPartyCount() == 0)
        {
            Task_PrintThereIsNoPokemon(taskId);
        }
        else
        {
            sBagMenuDisplay->exitCB = CB2_ChooseMonToGiveItem;
            gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
        }
    }
    else
        Task_PrintItemCantBeHeld(taskId);
}

static void Task_PrintThereIsNoPokemon(u8 taskId)
{
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		DisplayItemMessageInBag(taskId, 2, gText_ThereIsNoPokemon, Task_WaitAButtonAndCloseContextMenu);
    else
		DisplayItemMessageInBag(taskId, 2, gText_ThereIsNoPokemonSpa, Task_WaitAButtonAndCloseContextMenu);
}

static void Task_PrintItemCantBeHeld(u8 taskId)
{
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringExpandPlaceholders(gStringVar4, gText_ItemCantBeHeld);
    else
		StringExpandPlaceholders(gStringVar4, gText_ItemCantBeHeldSpa);
    DisplayItemMessageInBag(taskId, 2, gStringVar4, Task_WaitAButtonAndCloseContextMenu);
}

static void Task_WaitAButtonAndCloseContextMenu(u8 taskId)
{
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        Task_ReturnToBagFromContextMenu(taskId);
    }
}

void Task_ReturnToBagFromContextMenu(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    CloseBagWindow(5);
    DestroyListMenuTask(data[0], &gBagMenuState.cursorPos[gBagMenuState.pocket], &gBagMenuState.itemsAbove[gBagMenuState.pocket]);
    Pocket_CalculateNItemsAndMaxShowed(gBagMenuState.pocket);
    PocketCalculateInitialCursorPosAndItemsAbove(gBagMenuState.pocket);
    Bag_BuildListMenuTemplate(gBagMenuState.pocket);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagMenuState.cursorPos[gBagMenuState.pocket], gBagMenuState.itemsAbove[gBagMenuState.pocket]);
    ScheduleBgCopyTilemapToVram(0);
    bag_menu_print_cursor_(data[0], 1);
    Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
}

static void unref_sub_810A288(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 itemsAbove;
    u16 cursorPos;
    ListMenuGetScrollAndRow(data[0], &cursorPos, &itemsAbove);
    PrintItemDescriptionOnMessageWindow(cursorPos + itemsAbove);
    PutWindowTilemap(0);
    ScheduleBgCopyTilemapToVram(0);
    bag_menu_print_cursor_(data[0], 1);
    Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
}

static void Task_ItemMenuAction_Cancel(u8 taskId)
{
    HideBagWindow(10);
    HideBagWindow(6);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    bag_menu_print_cursor_(gTasks[taskId].data[0], 1);
    Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
}

static void Task_ItemMenuAction_BattleUse(u8 taskId)
{
    if (ItemId_GetBattleFunc(gSpecialVar_ItemId) != NULL)
    {
        HideBagWindow(10);
        HideBagWindow(6);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        CopyWindowToVram(0, COPYWIN_MAP);
        ItemId_GetBattleFunc(gSpecialVar_ItemId)(taskId);
    }
}

static void Task_ItemContext_FieldGive(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 itemId = BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, data[1]);
    if (!CanWriteMailHere(itemId))
    {
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			DisplayItemMessageInBag(taskId, 2, gText_CantWriteMailHere, Task_WaitAButtonAndCloseContextMenu);
        else
			DisplayItemMessageInBag(taskId, 2, gText_CantWriteMailHereSpa, Task_WaitAButtonAndCloseContextMenu);
    }
    else if (itemId == ITEM_TM_CASE)
    {
        ItemMenu_SetExitCallback(GoToTMCase_Give);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else if (itemId == ITEM_BERRY_POUCH)
    {
        ItemMenu_SetExitCallback(GoToBerryPouch_Give);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else if (gBagMenuState.pocket != POCKET_KEY_ITEMS - 1 && !itemid_is_unique(itemId))
    {
        Bag_BeginCloseWin0Animation();
        gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
    }
    else
    {
        Task_PrintItemCantBeHeld(taskId);
    }
}

static void GoToTMCase_Give(void)
{
    InitTMCase(TMCASE_FROMPARTYGIVE, ReturnToBagMenuFromSubmenu_Give, FALSE);
}

static void GoToBerryPouch_Give(void)
{
    InitBerryPouch(BERRYPOUCH_FROMPARTYGIVE, ReturnToBagMenuFromSubmenu_Give, FALSE);
}

static void ReturnToBagMenuFromSubmenu_Give(void)
{
    CB2_SelectBagItemToGive();
}

static void Task_ItemContext_PcBoxGive(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 itemId = BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, data[1]);
    if (ItemIsMail(itemId) == TRUE)
    {
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			DisplayItemMessageInBag(taskId, 2, gText_CantWriteMailHere, Task_WaitAButtonAndCloseContextMenu);
        else
			DisplayItemMessageInBag(taskId, 2, gText_CantWriteMailHereSpa, Task_WaitAButtonAndCloseContextMenu);
    }
    else if (itemId == ITEM_TM_CASE)
    {
        ItemMenu_SetExitCallback(GoToTMCase_PCBox);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else if (itemId == ITEM_BERRY_POUCH)
    {
        ItemMenu_SetExitCallback(GoToBerryPouch_PCBox);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else if (gBagMenuState.pocket != POCKET_KEY_ITEMS - 1 && !itemid_is_unique(itemId))
    {
        Bag_BeginCloseWin0Animation();
        gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
    }
    else
    {
        Task_PrintItemCantBeHeld(taskId);
    }
}

static void GoToTMCase_PCBox(void)
{
    InitTMCase(TMCASE_FROMPOKEMONSTORAGEPC, ReturnToBagMenuFromSubmenu_PCBox, FALSE);
}

static void GoToBerryPouch_PCBox(void)
{
    InitBerryPouch(BERRYPOUCH_FROMPOKEMONSTORAGEPC, ReturnToBagMenuFromSubmenu_PCBox, FALSE);
}

static void ReturnToBagMenuFromSubmenu_PCBox(void)
{
    GoToBagMenu(ITEMMENULOCATION_PCBOX, OPEN_BAG_LAST, Cb2_ReturnToPSS);
}

static void Task_ItemContext_Sell(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (gSpecialVar_ItemId == ITEM_TM_CASE)
    {
        ItemMenu_SetExitCallback(GoToTMCase_Sell);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else if (gSpecialVar_ItemId == ITEM_BERRY_POUCH)
    {
        ItemMenu_SetExitCallback(GoToBerryPouch_Sell);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else if (itemid_get_market_price(gSpecialVar_ItemId) == 0)
    {
        CopyItemName(gSpecialVar_ItemId, gStringVar1);
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			StringExpandPlaceholders(gStringVar4, gText_OhNoICantBuyThat);
        else
			StringExpandPlaceholders(gStringVar4, gText_OhNoICantBuyThatSpa);
        DisplayItemMessageInBag(taskId, GetDialogBoxFontId(), gStringVar4, Task_ReturnToBagFromContextMenu);
    }
    else
    {
        data[8] = 1;
        if (data[2] == 1)
        {
            BagPrintMoneyAmount();
            Task_PrintSaleConfirmationText(taskId);
        }
        else
        {
            if (data[2] > 999)
                data[2] = 999;
            CopyItemName(gSpecialVar_ItemId, gStringVar1);
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				StringExpandPlaceholders(gStringVar4, gText_HowManyWouldYouLikeToSell);
            else
				StringExpandPlaceholders(gStringVar4, gText_HowManyWouldYouLikeToSellSpa);
            DisplayItemMessageInBag(taskId, GetDialogBoxFontId(), gStringVar4, Task_InitSaleQuantitySelectInterface);
        }
    }
}

static void GoToTMCase_Sell(void)
{
    InitTMCase(TMCASE_FROMMARTSELL, ReturnToBagMenuFromSubmenu_Sell, FALSE);
}

static void GoToBerryPouch_Sell(void)
{
    InitBerryPouch(BERRYPOUCH_FROMMARTSELL, ReturnToBagMenuFromSubmenu_Sell, FALSE);
}

static void ReturnToBagMenuFromSubmenu_Sell(void)
{
    GoToBagMenu(ITEMMENULOCATION_SHOP, OPEN_BAG_LAST, CB2_ReturnToField);
}

static void Task_PrintSaleConfirmationText(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    ConvertIntToDecimalStringN(gStringVar3, itemid_get_market_price(BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, data[1])) / 2 * data[8], STR_CONV_MODE_LEFT_ALIGN, 8);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringExpandPlaceholders(gStringVar4, gText_ICanPayThisMuch_WouldThatBeOkay);
    else
		StringExpandPlaceholders(gStringVar4, gText_ICanPayThisMuch_WouldThatBeOkaySpa);
    DisplayItemMessageInBag(taskId, GetDialogBoxFontId(), gStringVar4, Task_ShowSellYesNoMenu);
}

static void Task_ShowSellYesNoMenu(u8 taskId)
{
    BagCreateYesNoMenuTopRight(taskId, &sYesNoMenu_Sell);
}

static void Task_SellItem_No(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    HideBagWindow(2);
    CloseBagWindow(5);
    PutWindowTilemap(2);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    bag_menu_print_cursor_(data[0], 1);
    Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
}

static void Task_InitSaleQuantitySelectInterface(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 r4 = ShowBagWindow(0, 1);
    ConvertIntToDecimalStringN(gStringVar1, 1, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringExpandPlaceholders(gStringVar4, gText_TimesStrVar1);
    BagPrintTextOnWindow(r4, 0, gStringVar4, 4, 10, 1, 0, 0xFF, 1);
    UpdateSalePriceDisplay(itemid_get_market_price(BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, data[1])) / 2 * data[8]);
    BagPrintMoneyAmount();
    CreatePocketScrollArrowPair_SellQuantity();
    gTasks[taskId].func = Task_SelectQuantityToSell;
}

static void UpdateSalePriceDisplay(s32 amount)
{
    PrintMoneyAmount(GetBagWindow(0), 50, 10, amount, 0);
}

static void Task_SelectQuantityToSell(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (AdjustQuantityAccordingToDPadInput(&data[8], data[2]) == TRUE)
    {
        UpdateQuantityToTossOrDeposit(data[8], 3);
        UpdateSalePriceDisplay(itemid_get_market_price(BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, data[1])) / 2 * data[8]);
    }
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideBagWindow(0);
        PutWindowTilemap(0);
        ScheduleBgCopyTilemapToVram(0);
        BagDestroyPocketScrollArrowPair();
        Task_PrintSaleConfirmationText(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideBagWindow(0);
        HideBagWindow(2);
        CloseBagWindow(5);
        PutWindowTilemap(2);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        BagDestroyPocketScrollArrowPair();
        bag_menu_print_cursor_(data[0], 1);
        Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
    }
}

static void Task_SellItem_Yes(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    PutWindowTilemap(0);
    ScheduleBgCopyTilemapToVram(0);
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    ConvertIntToDecimalStringN(gStringVar3, itemid_get_market_price(BagGetItemIdByPocketPosition(gBagMenuState.pocket + 1, data[1])) / 2 * data[8], STR_CONV_MODE_LEFT_ALIGN, 8);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringExpandPlaceholders(gStringVar4, gText_TurnedOverItemsWorthYen);
    else
		StringExpandPlaceholders(gStringVar4, gText_TurnedOverItemsWorthYenSpa);
    DisplayItemMessageInBag(taskId, 2, gStringVar4, Task_FinalizeSaleToShop);
}

static void Task_FinalizeSaleToShop(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    PlaySE(SE_SHOP);
    RemoveBagItem(gSpecialVar_ItemId, data[8]);
    AddMoney(&gSaveBlock1Ptr->money, itemid_get_market_price(gSpecialVar_ItemId) / 2 * data[8]);
    RecordItemPurchase(gSpecialVar_ItemId, data[8], 2);
    DestroyListMenuTask(data[0], &gBagMenuState.cursorPos[gBagMenuState.pocket], &gBagMenuState.itemsAbove[gBagMenuState.pocket]);
    Pocket_CalculateNItemsAndMaxShowed(gBagMenuState.pocket);
    PocketCalculateInitialCursorPosAndItemsAbove(gBagMenuState.pocket);
    sBagMenuDisplay->inhibitItemDescriptionPrint = TRUE;
    Bag_BuildListMenuTemplate(gBagMenuState.pocket);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagMenuState.cursorPos[gBagMenuState.pocket], gBagMenuState.itemsAbove[gBagMenuState.pocket]);
    bag_menu_print_cursor_(data[0], 2);
    BagDrawTextBoxOnWindow(GetBagWindow(2));
    PrintMoneyAmountInMoneyBox(GetBagWindow(2), GetMoney(&gSaveBlock1Ptr->money), 0);
    gTasks[taskId].func = Task_WaitPressAB_AfterSell;
}

static void Task_WaitPressAB_AfterSell(u8 taskId)
{
    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideBagWindow(2);
        PutWindowTilemap(2);
        sBagMenuDisplay->inhibitItemDescriptionPrint = FALSE;
        Task_ReturnToBagFromContextMenu(taskId);
    }
}

static void Task_ItemContext_Deposit(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    data[8] = 1;
    if (data[2] == 1)
    {
        Task_TryDoItemDeposit(taskId);
    }
    else
    {
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			InitQuantityToTossOrDeposit(data[1], gText_DepositHowManyStrVars1);
        else
			InitQuantityToTossOrDeposit(data[1], gText_DepositHowManyStrVars1Spa);
        gTasks[taskId].func = Task_SelectQuantityToDeposit;
    }
}

static void Task_SelectQuantityToDeposit(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (AdjustQuantityAccordingToDPadInput(&data[8], data[2]) == TRUE)
    {
        UpdateQuantityToTossOrDeposit(data[8], 3);
    }
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        ClearWindowTilemap(GetBagWindow(6));
        HideBagWindow(6);
        HideBagWindow(0);
        ScheduleBgCopyTilemapToVram(0);
        BagDestroyPocketScrollArrowPair();
        Task_TryDoItemDeposit(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideBagWindow(6);
        HideBagWindow(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        bag_menu_print_cursor_(data[0], 1);
        BagDestroyPocketScrollArrowPair();
        Task_RedrawArrowsAndReturnToBagMenuSelect(taskId);
    }
}

static void Task_TryDoItemDeposit(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (AddPCItem(gSpecialVar_ItemId, data[8]) == TRUE)
    {
        ItemUse_SetQuestLogEvent(28, 0, gSpecialVar_ItemId, 0xFFFF);
        CopyItemName(gSpecialVar_ItemId, gStringVar1);
        ConvertIntToDecimalStringN(gStringVar2, data[8], STR_CONV_MODE_LEFT_ALIGN, 3);
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			StringExpandPlaceholders(gStringVar4, gText_DepositedStrVar2StrVar1s);
        else
			StringExpandPlaceholders(gStringVar4, gText_DepositedStrVar2StrVar1sSpa);
        BagPrintTextOnWindow(ShowBagWindow(6, 3), 2, gStringVar4, 0, 2, 1, 0, 0, 1);
        gTasks[taskId].func = Task_WaitAB_RedrawAndReturnToBag;
    }
    else
    {
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			DisplayItemMessageInBag(taskId, 2, gText_NoRoomToStoreItems, Task_WaitAButtonAndCloseContextMenu);
        else
			DisplayItemMessageInBag(taskId, 2, gText_NoRoomToStoreItemsSpa, Task_WaitAButtonAndCloseContextMenu);
    }
}

bool8 UseRegisteredKeyItemOnField(void)
{
    u8 taskId;
    if (InUnionRoom() == TRUE)
        return FALSE;
    DismissMapNamePopup();
    ChangeBgY(0, 0, 0);
    if (gSaveBlock1Ptr->registeredItem != ITEM_NONE)
    {
        if (CheckBagHasItem(gSaveBlock1Ptr->registeredItem, 1) == TRUE)
        {
            ScriptContext2_Enable();
            FreezeObjectEvents();
            HandleEnforcedLookDirectionOnPlayerStopMoving();
            StopPlayerAvatar();
            gSpecialVar_ItemId = gSaveBlock1Ptr->registeredItem;
            taskId = CreateTask(ItemId_GetFieldFunc(gSaveBlock1Ptr->registeredItem), 8);
            gTasks[taskId].data[3] = 1;
            return TRUE;
        }
        gSaveBlock1Ptr->registeredItem = ITEM_NONE;
    }
    ScriptContext1_SetupScript(EventScript_BagItemCanBeRegistered);
    return TRUE;
}

static bool8 BagIsTutorial(void)
{
    if (
        gBagMenuState.location == ITEMMENULOCATION_OLD_MAN 
     || gBagMenuState.location == ITEMMENULOCATION_TTVSCR_CATCHING 
     || gBagMenuState.location == ITEMMENULOCATION_TTVSCR_STATUS 
     || gBagMenuState.location == ITEMMENULOCATION_TTVSCR_REGISTER 
     || gBagMenuState.location == ITEMMENULOCATION_TTVSCR_TMS
    )
        return TRUE;
    return FALSE;
}

static void BackUpPlayerBag(void)
{
    u32 i;
    sBackupPlayerBag = AllocZeroed(sizeof(struct BagSlots));
    memcpy(sBackupPlayerBag->bagPocket_Items, gSaveBlock1Ptr->bagPocket_Items, BAG_ITEMS_COUNT * sizeof(struct ItemSlot));
    memcpy(sBackupPlayerBag->bagPocket_KeyItems, gSaveBlock1Ptr->bagPocket_KeyItems, BAG_KEYITEMS_COUNT * sizeof(struct ItemSlot));
    memcpy(sBackupPlayerBag->bagPocket_PokeBalls, gSaveBlock1Ptr->bagPocket_PokeBalls, BAG_POKEBALLS_COUNT * sizeof(struct ItemSlot));
    sBackupPlayerBag->registeredItem = gSaveBlock1Ptr->registeredItem;
    sBackupPlayerBag->pocket = gBagMenuState.pocket;
    for (i = 0; i < 3; i++)
    {
        sBackupPlayerBag->itemsAbove[i] = gBagMenuState.itemsAbove[i];
        sBackupPlayerBag->cursorPos[i] = gBagMenuState.cursorPos[i];
    }
    ClearItemSlots(gSaveBlock1Ptr->bagPocket_Items, BAG_ITEMS_COUNT);
    ClearItemSlots(gSaveBlock1Ptr->bagPocket_KeyItems, BAG_KEYITEMS_COUNT);
    ClearItemSlots(gSaveBlock1Ptr->bagPocket_PokeBalls, BAG_POKEBALLS_COUNT);
    gSaveBlock1Ptr->registeredItem = ITEM_NONE;
    ResetBagCursorPositions();
}

static void RestorePlayerBag(void)
{
    u32 i;
    memcpy(gSaveBlock1Ptr->bagPocket_Items, sBackupPlayerBag->bagPocket_Items, BAG_ITEMS_COUNT * sizeof(struct ItemSlot));
    memcpy(gSaveBlock1Ptr->bagPocket_KeyItems, sBackupPlayerBag->bagPocket_KeyItems, BAG_KEYITEMS_COUNT * sizeof(struct ItemSlot));
    memcpy(gSaveBlock1Ptr->bagPocket_PokeBalls, sBackupPlayerBag->bagPocket_PokeBalls, BAG_POKEBALLS_COUNT * sizeof(struct ItemSlot));
    gSaveBlock1Ptr->registeredItem = sBackupPlayerBag->registeredItem;
    gBagMenuState.pocket = sBackupPlayerBag->pocket;
    for (i = 0; i < 3; i++)
    {
        gBagMenuState.itemsAbove[i] = sBackupPlayerBag->itemsAbove[i];
        gBagMenuState.cursorPos[i] = sBackupPlayerBag->cursorPos[i];
    }
    Free(sBackupPlayerBag);
}

void InitOldManBag(void)
{
    BackUpPlayerBag();
    AddBagItem(ITEM_POTION, 1);
    AddBagItem(ITEM_POKE_BALL, 1);
    GoToBagMenu(ITEMMENULOCATION_OLD_MAN, OPEN_BAG_ITEMS, SetCB2ToReshowScreenAfterMenu2);
}

static void Task_Bag_OldManTutorial(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!gPaletteFade.active)
    {
        switch (data[8])
        {
        case 102:
        case 204:
            PlaySE(SE_BAG_POCKET);
            SwitchPockets(taskId, 1, FALSE);
            break;
        case 306:
            PlaySE(SE_SELECT);
            bag_menu_print_cursor_(data[0], 2);
            Bag_FillMessageBoxWithPalette(1);
            gSpecialVar_ItemId = ITEM_POKE_BALL;
            OpenContextMenu(taskId);
            break;
        case 408:
            PlaySE(SE_SELECT);
            HideBagWindow(10);
            HideBagWindow(6);
            PutWindowTilemap(0);
            PutWindowTilemap(1);
            CopyWindowToVram(0, COPYWIN_MAP);
            DestroyListMenuTask(data[0], NULL, NULL);
            RestorePlayerBag();
            Bag_BeginCloseWin0Animation();
            gTasks[taskId].func = Task_Pokedude_FadeFromBag;
            return;
        }
        data[8]++;
    }
}

static void Task_Pokedude_FadeFromBag(u8 taskId)
{
    BeginNormalPaletteFade(0xFFFFFFFF, -2, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_Pokedude_WaitFadeAndExitBag;
}

static void Task_Pokedude_WaitFadeAndExitBag(u8 taskId)
{
    if (!gPaletteFade.active && FuncIsActiveTask(Task_AnimateWin0v) != TRUE)
    {
        if (sBagMenuDisplay->exitCB != NULL)
            SetMainCallback2(sBagMenuDisplay->exitCB);
        else
            SetMainCallback2(gBagMenuState.bagCallback);
        BagDestroyPocketScrollArrowPair();
        DestroyBagMenuResources();
        DestroyTask(taskId);
    }
}

void InitPokedudeBag(u8 a0)
{
    MainCallback cb2;
    u8 location;
    BackUpPlayerBag();
    AddBagItem(ITEM_POTION, 1);
    AddBagItem(ITEM_ANTIDOTE, 1);
    AddBagItem(ITEM_TEACHY_TV, 1);
    AddBagItem(ITEM_TM_CASE, 1);
    AddBagItem(ITEM_POKE_BALL, 5);
    AddBagItem(ITEM_GREAT_BALL, 1);
    AddBagItem(ITEM_NEST_BALL, 1);
    switch (a0)
    {
    default:
        cb2 = CB2_ReturnToTeachyTV;
        location = a0;
        break;
    case 7:
        cb2 = SetCB2ToReshowScreenAfterMenu2;
        location = ITEMMENULOCATION_TTVSCR_STATUS;
        break;
    case 8:
        cb2 = SetCB2ToReshowScreenAfterMenu2;
        location = ITEMMENULOCATION_TTVSCR_CATCHING;
        break;
    }
    GoToBagMenu(location, OPEN_BAG_ITEMS, cb2);
}

static bool8 Task_BButtonInterruptTeachyTv(u8 taskId)
{
    if (JOY_NEW(B_BUTTON))
    {
        RestorePlayerBag();
        SetTeachyTvControllerModeToResume();
        sBagMenuDisplay->exitCB = CB2_ReturnToTeachyTV;
        gTasks[taskId].func = Task_Pokedude_FadeFromBag;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void Task_Bag_TeachyTvRegister(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!gPaletteFade.active && Task_BButtonInterruptTeachyTv(taskId) != TRUE)
    {
        switch (data[8])
        {
        case 102:
            PlaySE(SE_BAG_POCKET);
            SwitchPockets(taskId, 1, FALSE);
            break;
        case 204:
            PlaySE(SE_SELECT);
            bag_menu_print_cursor_(data[0], 2);
            Bag_FillMessageBoxWithPalette(1);
            gSpecialVar_ItemId = ITEM_TEACHY_TV;
            OpenContextMenu(taskId);
            break;
        case 306:
            PlaySE(SE_SELECT);
            Menu_MoveCursorNoWrapAround(1);
            break;
        case 408:
            PlaySE(SE_SELECT);
            gSaveBlock1Ptr->registeredItem = gSpecialVar_ItemId;
            HideBagWindow(10);
            HideBagWindow(6);
            PutWindowTilemap(0);
            PutWindowTilemap(1);
            DestroyListMenuTask(data[0], &gBagMenuState.cursorPos[gBagMenuState.pocket], &gBagMenuState.itemsAbove[gBagMenuState.pocket]);
            Bag_BuildListMenuTemplate(gBagMenuState.pocket);
            data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagMenuState.cursorPos[gBagMenuState.pocket], gBagMenuState.itemsAbove[gBagMenuState.pocket]);
            Bag_FillMessageBoxWithPalette(0);
            bag_menu_print_cursor_(data[0], 1);
            CopyWindowToVram(0, COPYWIN_MAP);
            break;
        case 510:
        case 612:
            gMain.newKeys = 0;
            gMain.newAndRepeatedKeys = DPAD_DOWN;
            ListMenu_ProcessInput(data[0]);
            break;
        case 714:
            PlaySE(SE_SELECT);
            DestroyListMenuTask(data[0], NULL, NULL);
            RestorePlayerBag();
            Bag_BeginCloseWin0Animation();
            gTasks[taskId].func = Task_Pokedude_FadeFromBag;
            return;
        }
        data[8]++;
    }
}

static void Task_Bag_TeachyTvCatching(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!gPaletteFade.active)
    {
        if (Task_BButtonInterruptTeachyTv(taskId) == TRUE)
        {
            FreeRestoreBattleData();
            LoadPlayerParty();
            return;
        }
        switch (data[8])
        {
        case 102:
        case 204:
            PlaySE(SE_BAG_POCKET);
            SwitchPockets(taskId, 1, FALSE);
            break;
        case 306:
        case 408:
            gMain.newKeys = 0;
            gMain.newAndRepeatedKeys = DPAD_DOWN;
            ListMenu_ProcessInput(data[0]);
            break;
        case 510:
        case 612:
            gMain.newKeys = 0;
            gMain.newAndRepeatedKeys = DPAD_UP;
            ListMenu_ProcessInput(data[0]);
            break;
        case 714:
            PlaySE(SE_SELECT);
            bag_menu_print_cursor_(data[0], 2);
            Bag_FillMessageBoxWithPalette(1);
            gSpecialVar_ItemId = ITEM_POKE_BALL;
            OpenContextMenu(taskId);
            break;
        case 816:
            PlaySE(SE_SELECT);
            HideBagWindow(10);
            HideBagWindow(6);
            PutWindowTilemap(0);
            PutWindowTilemap(1);
            CopyWindowToVram(0, COPYWIN_MAP);
            DestroyListMenuTask(data[0], NULL, NULL);
            RestorePlayerBag();
            Bag_BeginCloseWin0Animation();
            gTasks[taskId].func = Task_Pokedude_FadeFromBag;
            return;
        }
        data[8]++;
    }
}

static void Task_Bag_TeachyTvStatus(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!gPaletteFade.active)
    {
        if (Task_BButtonInterruptTeachyTv(taskId) == TRUE)
        {
            FreeRestoreBattleData();
            LoadPlayerParty();
            return;
        }
        switch (data[8])
        {
        case 102:
            gMain.newKeys = 0;
            gMain.newAndRepeatedKeys = DPAD_DOWN;
            ListMenu_ProcessInput(data[0]);
            break;
        case 204:
            PlaySE(SE_SELECT);
            bag_menu_print_cursor_(data[0], 2);
            Bag_FillMessageBoxWithPalette(1);
            gSpecialVar_ItemId = ITEM_ANTIDOTE;
            OpenContextMenu(taskId);
            break;
        case 306:
            PlaySE(SE_SELECT);
            HideBagWindow(10);
            HideBagWindow(6);
            PutWindowTilemap(0);
            PutWindowTilemap(1);
            CopyWindowToVram(0, COPYWIN_MAP);
            DestroyListMenuTask(data[0], NULL, NULL);
            RestorePlayerBag();
            gItemUseCB = ItemUseCB_MedicineStep;
            ItemMenu_SetExitCallback(Pokedude_ChooseMonForInBattleItem);
            gTasks[taskId].func = Task_Pokedude_FadeFromBag;
            return;
        }
        data[8]++;
    }
}

static void Task_Bag_TeachyTvTMs(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!gPaletteFade.active && Task_BButtonInterruptTeachyTv(taskId) != TRUE)
    {
        switch (data[8])
        {
        case 102:
            PlaySE(SE_BAG_POCKET);
            SwitchPockets(taskId, 1, 0);
            break;
        case 204:
            gMain.newKeys = 0;
            gMain.newAndRepeatedKeys = DPAD_DOWN;
            ListMenu_ProcessInput(data[0]);
            break;
        case 306:
            PlaySE(SE_SELECT);
            bag_menu_print_cursor_(data[0], 2);
            Bag_FillMessageBoxWithPalette(1);
            gSpecialVar_ItemId = ITEM_TM_CASE;
            OpenContextMenu(taskId);
            break;
        case 408:
            PlaySE(SE_SELECT);
            HideBagWindow(10);
            HideBagWindow(6);
            PutWindowTilemap(0);
            PutWindowTilemap(1);
            CopyWindowToVram(0, COPYWIN_MAP);
            DestroyListMenuTask(data[0], NULL, NULL);
            RestorePlayerBag();
            sBagMenuDisplay->exitCB = Pokedude_InitTMCase;
            gTasks[taskId].func = Task_Pokedude_FadeFromBag;
            return;
        }
        data[8]++;
    }
}

// bag sorting
enum BagSortOptions
{
    SORT_ALPHABETICALLY,
    SORT_BY_TYPE,
    SORT_BY_IMPORTANCE,
    SORT_BY_QUALITY,
    SORT_BY_AMOUNT, //greatest->least
};
enum ItemSortType
{
	ITEM_TYPE_FIELD_USE,
	ITEM_TYPE_HEALTH_RECOVERY,
	ITEM_TYPE_STATUS_RECOVERY,
	ITEM_TYPE_PP_RECOVERY,
	ITEM_TYPE_STAT_BOOST_DRINK,
	ITEM_TYPE_STAT_BOOST_FEATHER,
	ITEM_TYPE_EVOLUTION_STONE,
	ITEM_TYPE_EVOLUTION_ITEM,
	ITEM_TYPE_BATTLE_ITEM,
	ITEM_TYPE_FLUTE,
	ITEM_TYPE_STAT_BOOST_HELD_ITEM,
	ITEM_TYPE_HELD_ITEM,
	ITEM_TYPE_GEM,
	ITEM_TYPE_PLATE,
	ITEM_TYPE_MEMORY,
	ITEM_TYPE_DRIVE,
	ITEM_TYPE_INCENSE,
	ITEM_TYPE_MEGA_STONE,
	ITEM_TYPE_Z_CRYSTAL,
	ITEM_TYPE_NECTAR,
	ITEM_TYPE_SELLABLE,
	ITEM_TYPE_RELIC,
	ITEM_TYPE_SHARD,
	ITEM_TYPE_FOSSIL,
	ITEM_TYPE_MAIL,
};

static const u8 sText_SortItemsHow[] = _("Sort items how?");
static const u8 sText_Name[] = _("name");
static const u8 sText_Importance[] = _("importance");
static const u8 sText_Quality[] = _("quality");
static const u8 sText_Type[] = _("type");
static const u8 sText_Amount[] = _("amount");
static const u8 sText_Ascending[] = _("ascending");
static const u8 sText_Descending[] = _("descending");
static const u8 sText_ItemsSorted[] = _("Items sorted by {STR_VAR_1} {STR_VAR_2}!");
static const u8 *const sSortTypeStrings[] = 
{
    [SORT_ALPHABETICALLY] = sText_Name,
    [SORT_BY_TYPE] = sText_Type,
    [SORT_BY_IMPORTANCE] = sText_Importance,
    [SORT_BY_QUALITY] = sText_Quality,
    [SORT_BY_AMOUNT] = sText_Amount,
};

static const u8 sText_SortItemsHowSpa[] = _("¿Cómo ordenar los\nelementos?");
static const u8 sText_NameSpa[] = _("nombre");
static const u8 sText_TypeSpa[] = _("tipo");
static const u8 sText_ImportanceSpa[] = _("importancia");
static const u8 sText_QualitySpa[] = _("calidad");
static const u8 sText_AmountSpa[] = _("cantidad");
static const u8 sText_AscendingSpa[] = _("ascendente");
static const u8 sText_DescendingSpa[] = _("descendente");
static const u8 sText_ItemsSortedSpa[] = _("¡Los elementos han sido ordenados por\n{STR_VAR_1} en orden {STR_VAR_2}!");
static const u8 *const sSortTypeStringsSpa[] = 
{
    [SORT_ALPHABETICALLY] = sText_NameSpa,
    [SORT_BY_TYPE] = sText_TypeSpa,
    [SORT_BY_IMPORTANCE] = sText_ImportanceSpa,
    [SORT_BY_QUALITY] = sText_QualitySpa,
    [SORT_BY_AMOUNT] = sText_AmountSpa,
};

static const u8 sBagMenuSortItems[] =
{
    ITEMMENUACTION_BY_NAME,
    ITEMMENUACTION_BY_IMPORTANCE,
    ITEMMENUACTION_BY_TYPE,
    ITEMMENUACTION_BY_AMOUNT,
    ITEMMENUACTION_CANCEL,
};

static const u8 sBagMenuSortKeyItems[] =
{
    ITEMMENUACTION_BY_NAME,
    ITEMMENUACTION_BY_IMPORTANCE,
    ITEMMENUACTION_CANCEL,
};

static const u8 sBagMenuSortPokeBallsBerries[] =
{
    ITEMMENUACTION_BY_NAME,
	ITEMMENUACTION_BY_QUALITY,
    ITEMMENUACTION_BY_AMOUNT,
    ITEMMENUACTION_CANCEL,
};

static const u16 sItemsByType[ITEMS_COUNT] =
{
    [ITEM_REPEL] = ITEM_TYPE_FIELD_USE,
    [ITEM_SUPER_REPEL] = ITEM_TYPE_FIELD_USE,
    [ITEM_MAX_REPEL] = ITEM_TYPE_FIELD_USE,
    [ITEM_ESCAPE_ROPE] = ITEM_TYPE_FIELD_USE,
    [ITEM_HEART_SCALE] = ITEM_TYPE_FIELD_USE,

    [ITEM_POTION] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_FULL_RESTORE] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_MAX_POTION] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_HYPER_POTION] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_SUPER_POTION] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_REVIVE] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_MAX_REVIVE] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_FRESH_WATER] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_SODA_POP] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_LEMONADE] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_MOOMOO_MILK] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_ENERGY_POWDER] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_ENERGY_ROOT] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_REVIVAL_HERB] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_BERRY_JUICE] = ITEM_TYPE_HEALTH_RECOVERY,
    [ITEM_SACRED_ASH] = ITEM_TYPE_HEALTH_RECOVERY,

    [ITEM_ANTIDOTE] = ITEM_TYPE_STATUS_RECOVERY,
    [ITEM_BURN_HEAL] = ITEM_TYPE_STATUS_RECOVERY,
    [ITEM_ICE_HEAL] = ITEM_TYPE_STATUS_RECOVERY,
    [ITEM_AWAKENING] = ITEM_TYPE_STATUS_RECOVERY,
    [ITEM_PARALYZE_HEAL] = ITEM_TYPE_STATUS_RECOVERY,
    [ITEM_FULL_HEAL] = ITEM_TYPE_STATUS_RECOVERY,
    [ITEM_LAVA_COOKIE] = ITEM_TYPE_STATUS_RECOVERY,

    [ITEM_ETHER] = ITEM_TYPE_PP_RECOVERY,
    [ITEM_MAX_ETHER] = ITEM_TYPE_PP_RECOVERY,
    [ITEM_ELIXIR] = ITEM_TYPE_PP_RECOVERY,
    [ITEM_MAX_ELIXIR] = ITEM_TYPE_PP_RECOVERY,

    [ITEM_HP_UP] = ITEM_TYPE_STAT_BOOST_DRINK,
    [ITEM_PROTEIN] = ITEM_TYPE_STAT_BOOST_DRINK,
    [ITEM_IRON] = ITEM_TYPE_STAT_BOOST_DRINK,
    [ITEM_CARBOS] = ITEM_TYPE_STAT_BOOST_DRINK,
    [ITEM_CALCIUM] = ITEM_TYPE_STAT_BOOST_DRINK,
    [ITEM_RARE_CANDY] = ITEM_TYPE_STAT_BOOST_DRINK,
    [ITEM_PP_UP] = ITEM_TYPE_STAT_BOOST_DRINK,
    [ITEM_ZINC] = ITEM_TYPE_STAT_BOOST_DRINK,
    [ITEM_PP_MAX] = ITEM_TYPE_STAT_BOOST_DRINK,

    [ITEM_MACHO_BRACE] = ITEM_TYPE_STAT_BOOST_HELD_ITEM,

    [ITEM_SUN_STONE] = ITEM_TYPE_EVOLUTION_STONE,
    [ITEM_MOON_STONE] = ITEM_TYPE_EVOLUTION_STONE,
    [ITEM_FIRE_STONE] = ITEM_TYPE_EVOLUTION_STONE,
    [ITEM_THUNDER_STONE] = ITEM_TYPE_EVOLUTION_STONE,
    [ITEM_WATER_STONE] = ITEM_TYPE_EVOLUTION_STONE,
    [ITEM_LEAF_STONE] = ITEM_TYPE_EVOLUTION_STONE,

    [ITEM_KINGS_ROCK] = ITEM_TYPE_EVOLUTION_ITEM,
    [ITEM_DEEP_SEA_TOOTH] = ITEM_TYPE_EVOLUTION_ITEM,
    [ITEM_DEEP_SEA_SCALE] = ITEM_TYPE_EVOLUTION_ITEM,
    [ITEM_EVERSTONE] = ITEM_TYPE_EVOLUTION_ITEM,
    [ITEM_METAL_COAT] = ITEM_TYPE_EVOLUTION_ITEM,
    [ITEM_DRAGON_SCALE] = ITEM_TYPE_EVOLUTION_ITEM,
    [ITEM_UP_GRADE] = ITEM_TYPE_EVOLUTION_ITEM,

    [ITEM_GUARD_SPEC] = ITEM_TYPE_BATTLE_ITEM,
    [ITEM_DIRE_HIT] = ITEM_TYPE_BATTLE_ITEM,
    [ITEM_X_ATTACK] = ITEM_TYPE_BATTLE_ITEM,
    [ITEM_X_DEFEND] = ITEM_TYPE_BATTLE_ITEM,
    [ITEM_X_SPEED] = ITEM_TYPE_BATTLE_ITEM,
    [ITEM_X_ACCURACY] = ITEM_TYPE_BATTLE_ITEM,
    [ITEM_X_SPECIAL] = ITEM_TYPE_BATTLE_ITEM,
    [ITEM_POKE_DOLL] = ITEM_TYPE_BATTLE_ITEM,
    [ITEM_FLUFFY_TAIL] = ITEM_TYPE_BATTLE_ITEM,

    [ITEM_BRIGHT_POWDER] = ITEM_TYPE_HELD_ITEM,
    [ITEM_WHITE_HERB] = ITEM_TYPE_HELD_ITEM,
    [ITEM_EXP_SHARE] = ITEM_TYPE_HELD_ITEM,
    [ITEM_QUICK_CLAW] = ITEM_TYPE_HELD_ITEM,
    [ITEM_SOOTHE_BELL] = ITEM_TYPE_HELD_ITEM,
    [ITEM_MENTAL_HERB] = ITEM_TYPE_HELD_ITEM,
    [ITEM_CHOICE_BAND] = ITEM_TYPE_HELD_ITEM,
    [ITEM_SILVER_POWDER] = ITEM_TYPE_HELD_ITEM,
    [ITEM_AMULET_COIN] = ITEM_TYPE_HELD_ITEM,
    [ITEM_CLEANSE_TAG] = ITEM_TYPE_HELD_ITEM,
    [ITEM_SOUL_DEW] = ITEM_TYPE_HELD_ITEM,
    [ITEM_SMOKE_BALL] = ITEM_TYPE_HELD_ITEM,
    [ITEM_FOCUS_BAND] = ITEM_TYPE_HELD_ITEM,
    [ITEM_LUCKY_EGG] = ITEM_TYPE_HELD_ITEM,
    [ITEM_SCOPE_LENS] = ITEM_TYPE_HELD_ITEM,
    [ITEM_LEFTOVERS] = ITEM_TYPE_HELD_ITEM,
    [ITEM_LIGHT_BALL] = ITEM_TYPE_HELD_ITEM,
    [ITEM_SOFT_SAND] = ITEM_TYPE_HELD_ITEM,
    [ITEM_HARD_STONE] = ITEM_TYPE_HELD_ITEM,
    [ITEM_MIRACLE_SEED] = ITEM_TYPE_HELD_ITEM,
    [ITEM_BLACK_GLASSES] = ITEM_TYPE_HELD_ITEM,
    [ITEM_BLACK_BELT] = ITEM_TYPE_HELD_ITEM,
    [ITEM_MAGNET] = ITEM_TYPE_HELD_ITEM,
    [ITEM_MYSTIC_WATER] = ITEM_TYPE_HELD_ITEM,
    [ITEM_SHARP_BEAK] = ITEM_TYPE_HELD_ITEM,
    [ITEM_POISON_BARB] = ITEM_TYPE_HELD_ITEM,
    [ITEM_NEVER_MELT_ICE] = ITEM_TYPE_HELD_ITEM,
    [ITEM_SPELL_TAG] = ITEM_TYPE_HELD_ITEM,
    [ITEM_TWISTED_SPOON] = ITEM_TYPE_HELD_ITEM,
    [ITEM_CHARCOAL] = ITEM_TYPE_HELD_ITEM,
    [ITEM_DRAGON_FANG] = ITEM_TYPE_HELD_ITEM,
    [ITEM_SILK_SCARF] = ITEM_TYPE_HELD_ITEM,
    [ITEM_SHELL_BELL] = ITEM_TYPE_HELD_ITEM,
    [ITEM_LUCKY_PUNCH] = ITEM_TYPE_HELD_ITEM,
    [ITEM_METAL_POWDER] = ITEM_TYPE_HELD_ITEM,
    [ITEM_THICK_CLUB] = ITEM_TYPE_HELD_ITEM,
    [ITEM_STICK] = ITEM_TYPE_HELD_ITEM,

    [ITEM_SEA_INCENSE] = ITEM_TYPE_INCENSE,
    [ITEM_LAX_INCENSE] = ITEM_TYPE_INCENSE,

    [ITEM_RED_ORB] = ITEM_TYPE_MEGA_STONE,
    [ITEM_BLUE_ORB] = ITEM_TYPE_MEGA_STONE,

    [ITEM_BLUE_FLUTE] = ITEM_TYPE_FLUTE,
    [ITEM_YELLOW_FLUTE] = ITEM_TYPE_FLUTE,
    [ITEM_RED_FLUTE] = ITEM_TYPE_FLUTE,
    [ITEM_BLACK_FLUTE] = ITEM_TYPE_FLUTE,
    [ITEM_WHITE_FLUTE] = ITEM_TYPE_FLUTE,

    [ITEM_SHOAL_SALT] = ITEM_TYPE_SELLABLE,
    [ITEM_SHOAL_SHELL] = ITEM_TYPE_SELLABLE,
    [ITEM_TINY_MUSHROOM] = ITEM_TYPE_SELLABLE,
    [ITEM_BIG_MUSHROOM] = ITEM_TYPE_SELLABLE,
    [ITEM_PEARL] = ITEM_TYPE_SELLABLE,
    [ITEM_BIG_PEARL] = ITEM_TYPE_SELLABLE,
    [ITEM_STARDUST] = ITEM_TYPE_SELLABLE,
    [ITEM_STAR_PIECE] = ITEM_TYPE_SELLABLE,
    [ITEM_NUGGET] = ITEM_TYPE_SELLABLE,

    [ITEM_RED_SHARD] = ITEM_TYPE_SHARD,
    [ITEM_BLUE_SHARD] = ITEM_TYPE_SHARD,
    [ITEM_YELLOW_SHARD] = ITEM_TYPE_SHARD,
    [ITEM_GREEN_SHARD] = ITEM_TYPE_SHARD,

    [ITEM_HELIX_FOSSIL] = ITEM_TYPE_FOSSIL,
    [ITEM_DOME_FOSSIL] = ITEM_TYPE_FOSSIL,
    [ITEM_OLD_AMBER] = ITEM_TYPE_FOSSIL,

    [ITEM_ORANGE_MAIL] = ITEM_TYPE_MAIL,
    [ITEM_HARBOR_MAIL] = ITEM_TYPE_MAIL,
    [ITEM_GLITTER_MAIL] = ITEM_TYPE_MAIL,
    [ITEM_MECH_MAIL] = ITEM_TYPE_MAIL,
    [ITEM_WOOD_MAIL] = ITEM_TYPE_MAIL,
    [ITEM_WAVE_MAIL] = ITEM_TYPE_MAIL,
    [ITEM_BEAD_MAIL] = ITEM_TYPE_MAIL,
    [ITEM_SHADOW_MAIL] = ITEM_TYPE_MAIL,
    [ITEM_TROPIC_MAIL] = ITEM_TYPE_MAIL,
    [ITEM_DREAM_MAIL] = ITEM_TYPE_MAIL,
    [ITEM_FAB_MAIL] = ITEM_TYPE_MAIL,
    [ITEM_RETRO_MAIL] = ITEM_TYPE_MAIL,

 /*   #ifdef ITEM_EXPANSION
        [ITEM_HONEY] = ITEM_TYPE_STATUS_RECOVERY,
        [ITEM_BIG_MALASADA] = ITEM_TYPE_STATUS_RECOVERY,
        [ITEM_CASTELIACONE] = ITEM_TYPE_STATUS_RECOVERY,
        [ITEM_LUMIOSE_GALETTE] = ITEM_TYPE_STATUS_RECOVERY,
        [ITEM_RAGE_CANDY_BAR] = ITEM_TYPE_STATUS_RECOVERY,
        [ITEM_SHALOUR_SABLE] = ITEM_TYPE_STATUS_RECOVERY,
        [ITEM_OLD_GATEAU] = ITEM_TYPE_STATUS_RECOVERY,
        [ITEM_HEAL_POWDER] = ITEM_TYPE_STATUS_RECOVERY,

        [ITEM_SWEET_HEART] = ITEM_TYPE_HEALTH_RECOVERY,

        [ITEM_ADAMANT_ORB] = ITEM_TYPE_HELD_ITEM,
        [ITEM_LUSTROUS_ORB] = ITEM_TYPE_HELD_ITEM,
        [ITEM_GRISEOUS_ORB] = ITEM_TYPE_HELD_ITEM,
        [ITEM_EXPERT_BELT] = ITEM_TYPE_HELD_ITEM,
        [ITEM_POWER_HERB] = ITEM_TYPE_HELD_ITEM,
        [ITEM_WIDE_LENS] = ITEM_TYPE_HELD_ITEM,
        [ITEM_ZOOM_LENS] = ITEM_TYPE_HELD_ITEM,
        [ITEM_DESTINY_KNOT] = ITEM_TYPE_HELD_ITEM,
        [ITEM_SMOOTH_ROCK] = ITEM_TYPE_HELD_ITEM,
        [ITEM_DAMP_ROCK] = ITEM_TYPE_HELD_ITEM,
        [ITEM_HEAT_ROCK] = ITEM_TYPE_HELD_ITEM,
        [ITEM_ICY_ROCK] = ITEM_TYPE_HELD_ITEM,
        [ITEM_BIG_ROOT] = ITEM_TYPE_HELD_ITEM,
        [ITEM_LIGHT_CLAY] = ITEM_TYPE_HELD_ITEM,
        [ITEM_SAFETY_GOGGLES] = ITEM_TYPE_HELD_ITEM,
        [ITEM_ROCKY_HELMET] = ITEM_TYPE_HELD_ITEM,
        [ITEM_WEAKNESS_POLICY] = ITEM_TYPE_HELD_ITEM,
        [ITEM_ASSAULT_VEST] = ITEM_TYPE_HELD_ITEM,
        [ITEM_EVIOLITE] = ITEM_TYPE_HELD_ITEM,
        [ITEM_ABSORB_BULB] = ITEM_TYPE_HELD_ITEM,
        [ITEM_AIR_BALLOON] = ITEM_TYPE_HELD_ITEM,
        [ITEM_ADRENALINE_ORB] = ITEM_TYPE_HELD_ITEM,
        [ITEM_BINDING_BAND] = ITEM_TYPE_HELD_ITEM,
        [ITEM_CELL_BATTERY] = ITEM_TYPE_HELD_ITEM,
        [ITEM_EJECT_BUTTON] = ITEM_TYPE_HELD_ITEM,
        [ITEM_FLOAT_STONE] = ITEM_TYPE_HELD_ITEM,
        [ITEM_FOCUS_SASH] = ITEM_TYPE_HELD_ITEM,
        [ITEM_GRIP_CLAW] = ITEM_TYPE_HELD_ITEM,
        [ITEM_IRON_BALL] = ITEM_TYPE_HELD_ITEM,
        [ITEM_LAGGING_TAIL] = ITEM_TYPE_HELD_ITEM,
        [ITEM_LUMINOUS_MOSS] = ITEM_TYPE_HELD_ITEM,
        [ITEM_QUICK_POWDER] = ITEM_TYPE_HELD_ITEM,
        [ITEM_METRONOME] = ITEM_TYPE_HELD_ITEM,
        [ITEM_MUSCLE_BAND] = ITEM_TYPE_HELD_ITEM,
        [ITEM_PROTECTIVE_PADS] = ITEM_TYPE_HELD_ITEM,
        [ITEM_RED_CARD] = ITEM_TYPE_HELD_ITEM,
        [ITEM_RING_TARGET] = ITEM_TYPE_HELD_ITEM,
        [ITEM_SHED_SHELL] = ITEM_TYPE_HELD_ITEM,
        [ITEM_SNOWBALL] = ITEM_TYPE_HELD_ITEM,
        [ITEM_STICKY_BARB] = ITEM_TYPE_HELD_ITEM,
        [ITEM_TERRAIN_EXTENDER] = ITEM_TYPE_HELD_ITEM,
        [ITEM_WISE_GLASSES] = ITEM_TYPE_HELD_ITEM,
        [ITEM_ELECTRIC_SEED] = ITEM_TYPE_HELD_ITEM,
        [ITEM_GRASSY_SEED] = ITEM_TYPE_HELD_ITEM,
        [ITEM_MISTY_SEED] = ITEM_TYPE_HELD_ITEM,
        [ITEM_PSYCHIC_SEED] = ITEM_TYPE_HELD_ITEM,
        [ITEM_LIFE_ORB] = ITEM_TYPE_HELD_ITEM,
        [ITEM_TOXIC_ORB] = ITEM_TYPE_HELD_ITEM,
        [ITEM_FLAME_ORB] = ITEM_TYPE_HELD_ITEM,
        [ITEM_BLACK_SLUDGE] = ITEM_TYPE_HELD_ITEM,
        [ITEM_CHOICE_SPECS] = ITEM_TYPE_HELD_ITEM,
        [ITEM_CHOICE_SCARF] = ITEM_TYPE_HELD_ITEM,
        [ITEM_EJECT_PACK] = ITEM_TYPE_HELD_ITEM,
        [ITEM_ROOM_SERVICE] = ITEM_TYPE_HELD_ITEM,
        [ITEM_BLUNDER_POLICY] = ITEM_TYPE_HELD_ITEM,
        [ITEM_HEAVY_DUTY_BOOTS] = ITEM_TYPE_HELD_ITEM,
        [ITEM_UTILITY_UMBRELLA] = ITEM_TYPE_HELD_ITEM,
        [ITEM_THROAT_SPRAY] = ITEM_TYPE_HELD_ITEM,
        [ITEM_FIST_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_SKY_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_TOXIC_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_EARTH_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_STONE_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_INSECT_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_SPOOKY_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_IRON_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_FLAME_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_SPLASH_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_MEADOW_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_ZAP_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_MIND_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_ICICLE_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_DRACO_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_DREAD_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_PIXIE_PLATE] = ITEM_TYPE_PLATE,
        [ITEM_FIGHTING_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_FLYING_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_POISON_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_GROUND_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_ROCK_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_BUG_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_GHOST_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_STEEL_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_FIRE_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_WATER_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_GRASS_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_ELECTRIC_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_PSYCHIC_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_ICE_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_DRAGON_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_DARK_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_FAIRY_MEMORY] = ITEM_TYPE_MEMORY,
        [ITEM_BURN_DRIVE] = ITEM_TYPE_DRIVE,
        [ITEM_DOUSE_DRIVE] = ITEM_TYPE_DRIVE,
        [ITEM_SHOCK_DRIVE] = ITEM_TYPE_DRIVE,
        [ITEM_CHILL_DRIVE] = ITEM_TYPE_DRIVE,
        [ITEM_NORMAL_GEM] = ITEM_TYPE_GEM,
        [ITEM_FIGHTING_GEM] = ITEM_TYPE_GEM,
        [ITEM_FLYING_GEM] = ITEM_TYPE_GEM,
        [ITEM_POISON_GEM] = ITEM_TYPE_GEM,
        [ITEM_GROUND_GEM] = ITEM_TYPE_GEM,
        [ITEM_ROCK_GEM] = ITEM_TYPE_GEM,
        [ITEM_BUG_GEM] = ITEM_TYPE_GEM,
        [ITEM_GHOST_GEM] = ITEM_TYPE_GEM,
        [ITEM_STEEL_GEM] = ITEM_TYPE_GEM,
        [ITEM_FIRE_GEM] = ITEM_TYPE_GEM,
        [ITEM_WATER_GEM] = ITEM_TYPE_GEM,
        [ITEM_GRASS_GEM] = ITEM_TYPE_GEM,
        [ITEM_ELECTRIC_GEM] = ITEM_TYPE_GEM,
        [ITEM_PSYCHIC_GEM] = ITEM_TYPE_GEM,
        [ITEM_ICE_GEM] = ITEM_TYPE_GEM,
        [ITEM_DRAGON_GEM] = ITEM_TYPE_GEM,
        [ITEM_DARK_GEM] = ITEM_TYPE_GEM,
        [ITEM_FAIRY_GEM] = ITEM_TYPE_GEM,

        [ITEM_LUCK_INCENSE] = ITEM_TYPE_INCENSE,
        [ITEM_FULL_INCENSE] = ITEM_TYPE_INCENSE,
        [ITEM_ODD_INCENSE] = ITEM_TYPE_INCENSE,
        [ITEM_PURE_INCENSE] = ITEM_TYPE_INCENSE,
        [ITEM_ROCK_INCENSE] = ITEM_TYPE_INCENSE,
        [ITEM_ROSE_INCENSE] = ITEM_TYPE_INCENSE,
        [ITEM_WAVE_INCENSE] = ITEM_TYPE_INCENSE,

        [ITEM_VENUSAURITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_CHARIZARDITE_X] = ITEM_TYPE_MEGA_STONE,
        [ITEM_CHARIZARDITE_Y] = ITEM_TYPE_MEGA_STONE,
        [ITEM_BLASTOISINITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_BEEDRILLITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_PIDGEOTITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_ALAKAZITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_SLOWBRONITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_GENGARITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_KANGASKHANITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_PINSIRITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_GYARADOSITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_AERODACTYLITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_MEWTWONITE_X] = ITEM_TYPE_MEGA_STONE,
        [ITEM_MEWTWONITE_Y] = ITEM_TYPE_MEGA_STONE,
        [ITEM_AMPHAROSITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_STEELIXITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_SCIZORITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_HERACRONITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_HOUNDOOMINITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_TYRANITARITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_SCEPTILITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_BLAZIKENITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_SWAMPERTITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_GARDEVOIRITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_SABLENITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_MAWILITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_AGGRONITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_MEDICHAMITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_MANECTITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_SHARPEDONITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_CAMERUPTITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_ALTARIANITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_BANETTITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_ABSOLITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_GLALITITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_SALAMENCITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_METAGROSSITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_LATIASITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_LATIOSITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_LOPUNNITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_GARCHOMPITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_LUCARIONITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_ABOMASITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_GALLADITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_AUDINITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_DIANCITE] = ITEM_TYPE_MEGA_STONE,
        [ITEM_ULTRANECROZIUM_Z] =  ITEM_TYPE_MEGA_STONE,

        [ITEM_NORMALIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_FIGHTINIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_FLYINIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_POISONIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_GROUNDIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_ROCKIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_BUGINIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_GHOSTIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_STEELIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_FIRIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_WATERIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_GRASSIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_ELECTRIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_PSYCHIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_ICIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_DRAGONIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_DARKINIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_FAIRIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_ALORAICHIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_DECIDIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_EEVIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_INCINIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_KOMMONIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_LUNALIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_LYCANIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_MARSHADIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_MEWNIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_MIMIKIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_PIKANIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_PIKASHUNIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_PRIMARIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_SNORLIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_SOLGANIUM_Z] = ITEM_TYPE_Z_CRYSTAL,
        [ITEM_TAPUNIUM_Z] = ITEM_TYPE_Z_CRYSTAL,

        [ITEM_RED_NECTAR] = ITEM_TYPE_NECTAR,
        [ITEM_YELLOW_NECTAR] = ITEM_TYPE_NECTAR,
        [ITEM_PINK_NECTAR] = ITEM_TYPE_NECTAR,
        [ITEM_PURPLE_NECTAR] = ITEM_TYPE_NECTAR,

        [ITEM_ABILITY_CAPSULE] = ITEM_TYPE_STAT_BOOST_DRINK,
        [ITEM_HEALTH_FEATHER] = ITEM_TYPE_STAT_BOOST_FEATHER,
        [ITEM_MUSCLE_FEATHER] = ITEM_TYPE_STAT_BOOST_FEATHER,
        [ITEM_RESIST_FEATHER] = ITEM_TYPE_STAT_BOOST_FEATHER,
        [ITEM_GENIUS_FEATHER] = ITEM_TYPE_STAT_BOOST_FEATHER,
        [ITEM_CLEVER_FEATHER] = ITEM_TYPE_STAT_BOOST_FEATHER,
        [ITEM_SWIFT_FEATHER] = ITEM_TYPE_STAT_BOOST_FEATHER,
        [ITEM_PRETTY_FEATHER] = ITEM_TYPE_STAT_BOOST_FEATHER,

        [ITEM_POWER_BRACER] = ITEM_TYPE_STAT_BOOST_HELD_ITEM,
        [ITEM_POWER_BELT] = ITEM_TYPE_STAT_BOOST_HELD_ITEM,
        [ITEM_POWER_LENS] = ITEM_TYPE_STAT_BOOST_HELD_ITEM,
        [ITEM_POWER_BAND] = ITEM_TYPE_STAT_BOOST_HELD_ITEM,
        [ITEM_POWER_ANKLET] = ITEM_TYPE_STAT_BOOST_HELD_ITEM,
        [ITEM_POWER_WEIGHT] = ITEM_TYPE_STAT_BOOST_HELD_ITEM,

        [ITEM_DAWN_STONE] = ITEM_TYPE_EVOLUTION_STONE,
        [ITEM_DUSK_STONE] = ITEM_TYPE_EVOLUTION_STONE,
        [ITEM_SHINY_STONE] = ITEM_TYPE_EVOLUTION_STONE,
        [ITEM_ICE_STONE] = ITEM_TYPE_EVOLUTION_STONE,
        [ITEM_OVAL_STONE] = ITEM_TYPE_EVOLUTION_STONE,

        [ITEM_PROTECTOR] = ITEM_TYPE_EVOLUTION_ITEM,
        [ITEM_MAGMARIZER] = ITEM_TYPE_EVOLUTION_ITEM,
        [ITEM_PRISM_SCALE] = ITEM_TYPE_EVOLUTION_ITEM,
        [ITEM_SACHET] = ITEM_TYPE_EVOLUTION_ITEM,
        [ITEM_WHIPPED_DREAM] = ITEM_TYPE_EVOLUTION_ITEM,
        [ITEM_RAZOR_CLAW] = ITEM_TYPE_EVOLUTION_ITEM,
        [ITEM_RAZOR_FANG] = ITEM_TYPE_EVOLUTION_ITEM,
        [ITEM_REAPER_CLOTH] = ITEM_TYPE_EVOLUTION_ITEM,
        [ITEM_DUBIOUS_DISC] = ITEM_TYPE_EVOLUTION_ITEM,
        [ITEM_ELECTIRIZER] = ITEM_TYPE_EVOLUTION_ITEM,

        [ITEM_RARE_BONE] = ITEM_TYPE_SELLABLE,
        [ITEM_PEARL_STRING] = ITEM_TYPE_SELLABLE,
        [ITEM_BIG_NUGGET] = ITEM_TYPE_SELLABLE,
        [ITEM_COMET_SHARD] = ITEM_TYPE_SELLABLE,
        [ITEM_BALM_MUSHROOM] = ITEM_TYPE_SELLABLE,
        [ITEM_ODD_KEYSTONE] = ITEM_TYPE_SELLABLE,
        [ITEM_BOTTLE_CAP] = ITEM_TYPE_SELLABLE,
        [ITEM_GOLD_BOTTLE_CAP] = ITEM_TYPE_SELLABLE,
        [ITEM_WISHING_PIECE] = ITEM_TYPE_SELLABLE,
        [ITEM_RELIC_COPPER] = ITEM_TYPE_RELIC,
        [ITEM_RELIC_SILVER] = ITEM_TYPE_RELIC,
        [ITEM_RELIC_GOLD] = ITEM_TYPE_RELIC,
        [ITEM_RELIC_VASE] = ITEM_TYPE_RELIC,
        [ITEM_RELIC_BAND] = ITEM_TYPE_RELIC,
        [ITEM_RELIC_STATUE] = ITEM_TYPE_RELIC,
        [ITEM_RELIC_CROWN] = ITEM_TYPE_RELIC,

        [ITEM_ROOT_FOSSIL] = ITEM_TYPE_FOSSIL,
        [ITEM_CLAW_FOSSIL] = ITEM_TYPE_FOSSIL,
        [ITEM_SKULL_FOSSIL] = ITEM_TYPE_FOSSIL,
        [ITEM_ARMOR_FOSSIL] = ITEM_TYPE_FOSSIL,
        [ITEM_COVER_FOSSIL] = ITEM_TYPE_FOSSIL,
        [ITEM_PLUME_FOSSIL] = ITEM_TYPE_FOSSIL,
        [ITEM_JAW_FOSSIL] = ITEM_TYPE_FOSSIL,
        [ITEM_SAIL_FOSSIL] = ITEM_TYPE_FOSSIL,
    #endif*/
};

static const u16 sItemsByImportance[ITEMS_COUNT] =
{
    [ITEM_POTION] = 0,
    [ITEM_BERRY_JUICE] = 1,
    [ITEM_SUPER_POTION] = 2,
    [ITEM_ENERGY_POWDER] = 3,
    [ITEM_FRESH_WATER] = 4,
    [ITEM_SODA_POP] = 5,
    [ITEM_LEMONADE] = 6,
    [ITEM_MOOMOO_MILK] = 7,
    [ITEM_ENERGY_ROOT] = 8,
    [ITEM_HYPER_POTION] = 9,
    [ITEM_MAX_POTION] = 10,
    [ITEM_FULL_RESTORE] = 11,
    [ITEM_REVIVE] = 12,
    [ITEM_REVIVAL_HERB] = 13,
    [ITEM_MAX_REVIVE] = 14,
    [ITEM_SACRED_ASH] = 15,

    [ITEM_ANTIDOTE] = 16,
    [ITEM_BURN_HEAL] = 17,
    [ITEM_ICE_HEAL] = 18,
    [ITEM_AWAKENING] = 19,
    [ITEM_PARALYZE_HEAL] = 20,
    [ITEM_FULL_HEAL] = 21,
    [ITEM_LAVA_COOKIE] = 22,

    [ITEM_ETHER] = 23,
    [ITEM_MAX_ETHER] = 24,
    [ITEM_ELIXIR] = 25,
    [ITEM_MAX_ELIXIR] = 26,

    [ITEM_REPEL] = 27,
    [ITEM_SUPER_REPEL] = 28,
    [ITEM_MAX_REPEL] = 29,
    [ITEM_ESCAPE_ROPE] = 30,
    [ITEM_HEART_SCALE] = 31,

    [ITEM_HP_UP] = 29,
    [ITEM_PROTEIN] = 30,
    [ITEM_IRON] = 31,
    [ITEM_CARBOS] = 32,
    [ITEM_ZINC] = 33,
    [ITEM_CALCIUM] = 34,
    [ITEM_PP_UP] = 35,
    [ITEM_PP_MAX] = 36,
    [ITEM_RARE_CANDY] = 37,

    [ITEM_SUN_STONE] = 38,
    [ITEM_MOON_STONE] = 39,
    [ITEM_FIRE_STONE] = 40,
    [ITEM_THUNDER_STONE] = 41,
    [ITEM_WATER_STONE] = 42,
    [ITEM_LEAF_STONE] = 43,

    [ITEM_EVERSTONE] = 40,
    [ITEM_KINGS_ROCK] = 41,
    [ITEM_DEEP_SEA_TOOTH] = 42,
    [ITEM_DEEP_SEA_SCALE] = 43,
    [ITEM_METAL_COAT] = 44,
    [ITEM_DRAGON_SCALE] = 45,
    [ITEM_UP_GRADE] = 46,

    [ITEM_GUARD_SPEC] = 47,
    [ITEM_DIRE_HIT] = 48,
    [ITEM_X_ATTACK] = 49,
    [ITEM_X_DEFEND] = 50,
    [ITEM_X_SPEED] = 51,
    [ITEM_X_ACCURACY] = 52,
    [ITEM_X_SPECIAL] = 53,
    [ITEM_POKE_DOLL] = 54,
    [ITEM_FLUFFY_TAIL] = 55,

    [ITEM_MACHO_BRACE] = 56,

    [ITEM_BRIGHT_POWDER] = 57,
    [ITEM_WHITE_HERB] = 58,
    [ITEM_EXP_SHARE] = 59,
    [ITEM_QUICK_CLAW] = 60,
    [ITEM_SOOTHE_BELL] = 61,
    [ITEM_MENTAL_HERB] = 62,
    [ITEM_CHOICE_BAND] = 63,
    [ITEM_SILVER_POWDER] = 64,
    [ITEM_AMULET_COIN] = 65,
    [ITEM_CLEANSE_TAG] = 66,
    [ITEM_SOUL_DEW] = 67,
    [ITEM_FOCUS_BAND] = 68,
    [ITEM_LUCKY_EGG] = 69,
    [ITEM_SCOPE_LENS] = 70,
    [ITEM_LEFTOVERS] = 71,
    [ITEM_LIGHT_BALL] = 72,
    [ITEM_SOFT_SAND] = 73,
    [ITEM_HARD_STONE] = 74,
    [ITEM_MIRACLE_SEED] = 75,
    [ITEM_BLACK_GLASSES] = 76,
    [ITEM_BLACK_BELT] = 77,
    [ITEM_MAGNET] = 78,
    [ITEM_MYSTIC_WATER] = 79,
    [ITEM_SHARP_BEAK] = 80,
    [ITEM_POISON_BARB] = 81,
    [ITEM_NEVER_MELT_ICE] = 82,
    [ITEM_SPELL_TAG] = 83,
    [ITEM_TWISTED_SPOON] = 84,
    [ITEM_CHARCOAL] = 85,
    [ITEM_DRAGON_FANG] = 86,
    [ITEM_SILK_SCARF] = 87,
    [ITEM_SHELL_BELL] = 88,
    [ITEM_LUCKY_PUNCH] = 89,
    [ITEM_METAL_POWDER] = 90,
    [ITEM_THICK_CLUB] = 91,
    [ITEM_STICK] = 92,

    [ITEM_SEA_INCENSE] = 93,
    [ITEM_LAX_INCENSE] = 94,

    [ITEM_YELLOW_FLUTE] = 95,
    [ITEM_BLUE_FLUTE] = 96,
    [ITEM_RED_FLUTE] = 97,
    [ITEM_BLACK_FLUTE] = 98,
    [ITEM_WHITE_FLUTE] = 99,

    [ITEM_SHOAL_SALT] = 100,
    [ITEM_SHOAL_SHELL] = 101,
    [ITEM_TINY_MUSHROOM] = 102,
    [ITEM_BIG_MUSHROOM] = 103,
    [ITEM_PEARL] = 104,
    [ITEM_BIG_PEARL] = 105,
    [ITEM_STARDUST] = 106,
    [ITEM_STAR_PIECE] = 107,
    [ITEM_NUGGET] = 108,

    [ITEM_YELLOW_SHARD] = 109,
    [ITEM_BLUE_SHARD] = 110,
    [ITEM_RED_SHARD] = 111,
    [ITEM_GREEN_SHARD] = 112,

    [ITEM_ORANGE_MAIL] = 113,
    [ITEM_HARBOR_MAIL] = 114,
    [ITEM_GLITTER_MAIL] = 115,
    [ITEM_MECH_MAIL] = 116,
    [ITEM_WOOD_MAIL] = 117,
    [ITEM_WAVE_MAIL] = 118,
    [ITEM_BEAD_MAIL] = 119,
    [ITEM_SHADOW_MAIL] = 120,
    [ITEM_TROPIC_MAIL] = 121,
    [ITEM_DREAM_MAIL] = 122,
    [ITEM_FAB_MAIL] = 123,
    [ITEM_RETRO_MAIL] = 124,

    [ITEM_SMOKE_BALL] = 125,
    [ITEM_POKE_BALL] = 126,
    [ITEM_GREAT_BALL] = 127,
    [ITEM_SAFARI_BALL] = 128,
    [ITEM_NET_BALL] = 129,
    [ITEM_DIVE_BALL] = 130,
    [ITEM_REPEAT_BALL] = 131,
    [ITEM_TIMER_BALL] = 132,
    [ITEM_LUXURY_BALL] = 133,
    [ITEM_PREMIER_BALL] = 134,
    [ITEM_MASTER_BALL] = 135,

    [ITEM_OAKS_PARCEL] = 136,
    [ITEM_TOWN_MAP] = 137,
    [ITEM_TM_CASE] = 138,
    [ITEM_BERRY_POUCH] = 139,
    [ITEM_POKEBLOCK_CASE] = 140,
    [ITEM_COIN_CASE] = 141,
    [ITEM_OLD_ROD] = 142,
    [ITEM_GOOD_ROD] = 143,
    [ITEM_SUPER_ROD] = 144,
    [ITEM_BICYCLE] = 145,
    [ITEM_MACH_BIKE] = 146,
    [ITEM_ACRO_BIKE] = 147,
    [ITEM_BIKE_VOUCHER] = 148,
    [ITEM_SS_TICKET] = 149,
    [ITEM_EON_TICKET] = 150,
    [ITEM_TRI_PASS] = 151,
    [ITEM_RAINBOW_PASS] = 152,
    [ITEM_MYSTIC_TICKET] = 153,
    [ITEM_AURORA_TICKET] = 154,
    [ITEM_CONTEST_PASS] = 155,
    [ITEM_VS_SEEKER] = 156,
    [ITEM_FAME_CHECKER] = 157,
    [ITEM_TEACHY_TV] = 158,
    [ITEM_ITEMFINDER] = 159,
    [ITEM_BASEMENT_KEY] = 160,
    [ITEM_ROOM_1_KEY] = 161,
    [ITEM_ROOM_2_KEY] = 162,
    [ITEM_ROOM_4_KEY] = 163,
    [ITEM_ROOM_6_KEY] = 164,
    [ITEM_STORAGE_KEY] = 165,
    [ITEM_CARD_KEY] = 166,
    [ITEM_LIFT_KEY] = 167,
    [ITEM_SECRET_KEY] = 168,
    [ITEM_POKE_FLUTE] = 169,
    [ITEM_GOLD_TEETH] = 170,
    [ITEM_TEA] = 171,
    [ITEM_LETTER] = 172,
    [ITEM_WAILMER_PAIL] = 173,
    [ITEM_DEVON_GOODS] = 174,
    [ITEM_SCANNER] = 175,
    [ITEM_GO_GOGGLES] = 176,
    [ITEM_DEVON_SCOPE] = 177,
    [ITEM_SILPH_SCOPE] = 178,
    [ITEM_METEORITE] = 179,
    [ITEM_SOOT_SACK] = 180,
    [ITEM_POWDER_JAR] = 181,
    [ITEM_RED_ORB] = 182,
    [ITEM_BLUE_ORB] = 183,
    [ITEM_RUBY] = 184,
    [ITEM_SAPPHIRE] = 185,
    [ITEM_MAGMA_EMBLEM] = 186,
    [ITEM_OLD_SEA_MAP] = 187,
    [ITEM_HELIX_FOSSIL] = 188,
    [ITEM_DOME_FOSSIL] = 189,
    [ITEM_OLD_AMBER] = 190,
};

static void AddBagSortSubMenu(void)
{
    u8 windowId = 0;

    switch (gBagMenuState.pocket + 1)
    {
        case POCKET_KEY_ITEMS:
            sContextMenuItemsPtr = sBagMenuSortKeyItems;
            memcpy(&sContextMenuItemsBuffer, &sBagMenuSortKeyItems, NELEMS(sBagMenuSortKeyItems));
            sContextMenuNumItems = NELEMS(sBagMenuSortKeyItems);
            break;
        case POCKET_POKE_BALLS:
            sContextMenuItemsPtr = sBagMenuSortPokeBallsBerries;
            memcpy(&sContextMenuItemsBuffer, &sBagMenuSortPokeBallsBerries, NELEMS(sBagMenuSortPokeBallsBerries));
            sContextMenuNumItems = NELEMS(sBagMenuSortPokeBallsBerries);
            break;
        case POCKET_BERRY_POUCH:
        case POCKET_TM_CASE:
        default:
            sContextMenuItemsPtr = sBagMenuSortItems;
            memcpy(&sContextMenuItemsBuffer, &sBagMenuSortItems, NELEMS(sBagMenuSortItems));
            sContextMenuNumItems = NELEMS(sBagMenuSortItems);
            break;
    }

    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringExpandPlaceholders(gStringVar4, sText_SortItemsHow);
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		StringExpandPlaceholders(gStringVar4, sText_SortItemsHowSpa);
	BagPrintTextOnWindow(ShowBagWindow(6, 0), 2, gStringVar4, 0, 2, 1, 0, 0, 1);
    windowId = ShowBagWindow(10, sContextMenuNumItems -1);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		AddItemMenuActionTextPrinters(windowId, 2, GetMenuCursorDimensionByFont(2, 0), 2, GetFontAttribute(2, FONTATTR_LETTER_SPACING), GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT) + 2, sContextMenuNumItems, sItemMenuContextActions, sContextMenuItemsPtr);
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		AddItemMenuActionTextPrinters(windowId, 2, GetMenuCursorDimensionByFont(2, 0), 2, GetFontAttribute(2, FONTATTR_LETTER_SPACING), GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT) + 2, sContextMenuNumItems, sItemMenuContextActionsSpa, sContextMenuItemsPtr);
    Menu_InitCursor(windowId, 2, 0, 2, GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT) + 2, sContextMenuNumItems, 0);
}


static void Task_LoadBagSortOptions(u8 taskId)
{
    AddBagSortSubMenu();
    gTasks[taskId].func = Task_FieldItemContextMenuHandleInput;
}

#define tSortType data[2]
static void ItemMenu_SortByName(u8 taskId)
{
    gTasks[taskId].tSortType = SORT_ALPHABETICALLY;
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringCopy(gStringVar1, sSortTypeStrings[SORT_ALPHABETICALLY]);
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		StringCopy(gStringVar1, sSortTypeStringsSpa[SORT_ALPHABETICALLY]);
    gTasks[taskId].func = SortBagItems;
}
static void ItemMenu_SortByImportance(u8 taskId)
{
    gTasks[taskId].tSortType = SORT_BY_IMPORTANCE;
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringCopy(gStringVar1, sSortTypeStrings[SORT_BY_IMPORTANCE]);
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		StringCopy(gStringVar1, sSortTypeStringsSpa[SORT_BY_IMPORTANCE]);
    gTasks[taskId].func = SortBagItems;
}
static void ItemMenu_SortByQuality(u8 taskId)
{
    gTasks[taskId].tSortType = SORT_BY_QUALITY;
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringCopy(gStringVar1, sSortTypeStrings[SORT_BY_QUALITY]);
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		StringCopy(gStringVar1, sSortTypeStringsSpa[SORT_BY_QUALITY]);
    gTasks[taskId].func = SortBagItems;
}
static void ItemMenu_SortByType(u8 taskId)
{
    gTasks[taskId].tSortType = SORT_BY_TYPE;
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringCopy(gStringVar1, sSortTypeStrings[SORT_BY_TYPE]);
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		StringCopy(gStringVar1, sSortTypeStringsSpa[SORT_BY_TYPE]);
    gTasks[taskId].func = SortBagItems;
}
static void ItemMenu_SortByAmount(u8 taskId)
{
    gTasks[taskId].tSortType = SORT_BY_AMOUNT; //greatest->least
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringCopy(gStringVar1, sSortTypeStrings[SORT_BY_AMOUNT]);
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		StringCopy(gStringVar1, sSortTypeStringsSpa[SORT_BY_AMOUNT]);
    gTasks[taskId].func = SortBagItems;
}

static void SortBagItems(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    SortItemsInBag(gBagMenuState.pocket, tSortType);
    CloseBagWindow(10);
    CloseBagWindow(6);
    Bag_BuildListMenuTemplate(gBagMenuState.pocket);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, gBagMenuState.cursorPos[gBagMenuState.pocket], gBagMenuState.itemsAbove[gBagMenuState.pocket]);
    DestroyListMenuTask(data[0], &gBagMenuState.cursorPos[gBagMenuState.pocket], &gBagMenuState.itemsAbove[gBagMenuState.pocket]);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		switch (tSortType)
		{
		case SORT_ALPHABETICALLY:
			sBagMenuDisplay->lastSortingAlphabetically == 1 ? StringCopy(gStringVar2, sText_Ascending) : StringCopy(gStringVar2, sText_Descending);
			break;
		case SORT_BY_AMOUNT:
			sBagMenuDisplay->lastSortingByAmount == 1 ? StringCopy(gStringVar2, sText_Ascending) : StringCopy(gStringVar2, sText_Descending);
			break;
		case SORT_BY_TYPE:
			sBagMenuDisplay->lastSortingByType == 1 ? StringCopy(gStringVar2, sText_Ascending) : StringCopy(gStringVar2, sText_Descending);
			break;
		case SORT_BY_IMPORTANCE:
			sBagMenuDisplay->lastSortingByImportance == 1 ? StringCopy(gStringVar2, sText_Ascending) : StringCopy(gStringVar2, sText_Descending);
			break;
		case SORT_BY_QUALITY:
			sBagMenuDisplay->lastSortingByQuality == 1 ? StringCopy(gStringVar2, sText_Ascending) : StringCopy(gStringVar2, sText_Descending);
			break;
		}
		StringCopy(gStringVar1, sSortTypeStrings[tSortType]);
		StringExpandPlaceholders(gStringVar4, sText_ItemsSorted);
    }    
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		switch (tSortType)
		{
		case SORT_ALPHABETICALLY:
			sBagMenuDisplay->lastSortingAlphabetically == 1 ? StringCopy(gStringVar2, sText_AscendingSpa) : StringCopy(gStringVar2, sText_DescendingSpa);
			break;
		case SORT_BY_AMOUNT:
			sBagMenuDisplay->lastSortingByAmount == 1 ? StringCopy(gStringVar2, sText_AscendingSpa) : StringCopy(gStringVar2, sText_DescendingSpa);
			break;
		case SORT_BY_TYPE:
			sBagMenuDisplay->lastSortingByType == 1 ? StringCopy(gStringVar2, sText_AscendingSpa) : StringCopy(gStringVar2, sText_DescendingSpa);
			break;
		case SORT_BY_IMPORTANCE:
			sBagMenuDisplay->lastSortingByImportance == 1 ? StringCopy(gStringVar2, sText_AscendingSpa) : StringCopy(gStringVar2, sText_DescendingSpa);
			break;
		case SORT_BY_QUALITY:
			sBagMenuDisplay->lastSortingByQuality == 1 ? StringCopy(gStringVar2, sText_AscendingSpa) : StringCopy(gStringVar2, sText_DescendingSpa);
			break;
		}
		StringCopy(gStringVar1, sSortTypeStringsSpa[tSortType]);
		StringExpandPlaceholders(gStringVar4, sText_ItemsSortedSpa);
    }
	DisplayItemMessageInBag(taskId, 1, gStringVar4, Task_SortFinish);
}
#undef tSortType

static void Task_SortFinish(u8 taskId)
{
    s16* data = gTasks[taskId].data;

    if (gMain.newKeys & (A_BUTTON | B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Task_ReturnToBagFromContextMenu(taskId);
    }
}

static void SortItemsInBag(u8 pocket, u8 type)
{
    struct ItemSlot* itemMem;
    u16 itemAmount;
    s8 (*func)(struct ItemSlot*, struct ItemSlot*);
    switch (pocket)
    {
    case (POCKET_ITEMS - 1):
        itemMem = gSaveBlock1Ptr->bagPocket_Items;
        itemAmount = BAG_ITEMS_COUNT;
        break;
    case (POCKET_KEY_ITEMS - 1):
        itemMem = gSaveBlock1Ptr->bagPocket_KeyItems;
        itemAmount = BAG_KEYITEMS_COUNT;
        break;
    case (POCKET_POKE_BALLS - 1):
        itemMem = gSaveBlock1Ptr->bagPocket_PokeBalls;
        itemAmount = BAG_POKEBALLS_COUNT;
        break;
    case (POCKET_BERRY_POUCH - 1):
        itemMem = gSaveBlock1Ptr->bagPocket_Berries;
        itemAmount = BAG_BERRIES_COUNT;
        break;
    case (POCKET_TM_CASE - 1):
        itemMem = gSaveBlock1Ptr->bagPocket_TMHM;
        itemAmount = BAG_TMHM_COUNT;
        break;
    default:
        return;
    }

    switch (type)
    {
    case SORT_ALPHABETICALLY:
        sBagMenuDisplay->lastSortingAlphabetically ^= 1;
        MergeSort(itemMem, 0, itemAmount - 1, CompareItemsAlphabetically);
        break;
    case SORT_BY_AMOUNT:
        sBagMenuDisplay->lastSortingByAmount ^= 1;
        MergeSort(itemMem, 0, itemAmount - 1, CompareItemsByMost);
        break;
    case SORT_BY_IMPORTANCE:
        sBagMenuDisplay->lastSortingByImportance ^= 1;
        MergeSort(itemMem, 0, itemAmount - 1, CompareItemsByImportance);
        break;
    case SORT_BY_QUALITY:
        sBagMenuDisplay->lastSortingByQuality ^= 1;
        MergeSort(itemMem, 0, itemAmount - 1, CompareItemsByQuality);
        break;
    case SORT_BY_TYPE:
        sBagMenuDisplay->lastSortingByType ^= 1;
        MergeSort(itemMem, 0, itemAmount - 1, CompareItemsByType);
        break;
    default:
		MergeSort(itemMem, 0, itemAmount - 1, CompareItemsByType);
        break;
    }
}

static void MergeSort(struct ItemSlot* array, u32 low, u32 high, s8 (*comparator)(struct ItemSlot*, struct ItemSlot*))
{
    u32 mid;

    if (high <= low)
        return;

    mid = low + (high - low) / 2;
    MergeSort(array, low, mid, comparator); //Sort left half.
    MergeSort(array, mid + 1, high, comparator); //Sort right half.
    Merge(array, low, mid, high, comparator); //Merge results.
}

static void Merge(struct ItemSlot* array, u32 low, u32 mid, u32 high, s8 (*comparator)(struct ItemSlot*, struct ItemSlot*))
{
    u32 i = low;
    u32 j = mid + 1;
    u32 k;
    struct ItemSlot aux[high + 1];

    for (k = low; k <= high; ++k)
        aux[k] = array[k];

    for (k = low; k <= high; ++k)
    { //Merge back to a[low..high]
        if (i > mid)
            array[k] = aux[j++];
        else if (j > high)
            array[k] = aux[i++];
        else if (comparator(&aux[j], &aux[i]) < 0)
            array[k] = aux[j++];
        else
            array[k] = aux[i++];
    }
}

static s8 CompareItemsAlphabetically(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2)
{
    u16 item1 = itemSlot1->itemId;
    u16 item2 = itemSlot2->itemId;
    int i;
    const u8 *name1;
    const u8 *name2;

    if (item1 == ITEM_NONE)
        return 1;
    else if (item2 == ITEM_NONE)
        return -1;

    name1 = ItemId_GetName(item1);
    name2 = ItemId_GetName(item2);

    for (i = 0; ; ++i)
    {
        if (name1[i] == EOS && name2[i] != EOS)
            return -1;
        else if (name1[i] != EOS && name2[i] == EOS)
            return 1;
        else if (name1[i] == EOS && name2[i] == EOS)
            return 0;

        if (sBagMenuDisplay->lastSortingAlphabetically == 1)
		{
			if (name1[i] < name2[i])
				return -1;
			else if (name1[i] > name2[i])
				return 1;
		}
        else
		{
			if (name1[i] < name2[i])
				return 1;
			else if (name1[i] > name2[i])
				return -1;
		}
    }

    return 0; //Will never be reached
}

static s8 CompareItemsByMost(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2)
{
    u16 quantity1 = GetBagItemQuantity(&itemSlot1->quantity);
    u16 quantity2 = GetBagItemQuantity(&itemSlot2->quantity);

    if (itemSlot1->itemId == ITEM_NONE)
        return 1;
    else if (itemSlot2->itemId == ITEM_NONE)
        return -1;

    if (sBagMenuDisplay->lastSortingByAmount == 0)
	{
		if (quantity1 < quantity2)
			return 1;
		else if (quantity1 > quantity2)
			return -1;
	}
    else
	{
		if (quantity1 < quantity2)
			return -1;
		else if (quantity1 > quantity2)
			return 1;
	}
}

static s8 CompareItemsByType(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2)
{
    //Null items go last
    u8 type1 = (itemSlot1->itemId == ITEM_NONE) ? 0xFF : sItemsByType[itemSlot1->itemId];
    u8 type2 = (itemSlot2->itemId == ITEM_NONE) ? 0xFF : sItemsByType[itemSlot2->itemId];

    if (itemSlot1->itemId == ITEM_NONE)
        return 1;
    else if (itemSlot2->itemId == ITEM_NONE)
        return -1;

    if (sBagMenuDisplay->lastSortingByType == 1)
	{
		if (type1 < type2)
			return -1;
		else if (type1 > type2)
			return 1;
	}
    else
	{
		if (type1 < type2)
			return 1;
		else if (type1 > type2)
			return -1;
	}
}

static s8 CompareItemsByImportance(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2)
{
    //Null items go last
    u8 type1 = (itemSlot1->itemId == ITEM_NONE) ? 0xFF : sItemsByImportance[itemSlot1->itemId];
    u8 type2 = (itemSlot2->itemId == ITEM_NONE) ? 0xFF : sItemsByImportance[itemSlot2->itemId];

    if (itemSlot1->itemId == ITEM_NONE)
        return 1;
    else if (itemSlot2->itemId == ITEM_NONE)
        return -1;

    if (sBagMenuDisplay->lastSortingByImportance == 1)
	{
		if (type1 < type2)
			return -1;
		else if (type1 > type2)
			return 1;
	}
    else
	{
		if (type1 < type2)
			return 1;
		else if (type1 > type2)
			return -1;
	}
}

static s8 CompareItemsByQuality(struct ItemSlot* itemSlot1, struct ItemSlot* itemSlot2)
{
    //Null items go last
    u8 type1 = (itemSlot1->itemId == ITEM_NONE) ? 0xFF : sItemsByImportance[itemSlot1->itemId];
    u8 type2 = (itemSlot2->itemId == ITEM_NONE) ? 0xFF : sItemsByImportance[itemSlot2->itemId];

    if (itemSlot1->itemId == ITEM_NONE)
        return 1;
    else if (itemSlot2->itemId == ITEM_NONE)
        return -1;

    if (sBagMenuDisplay->lastSortingByQuality == 1)
	{
		if (type1 < type2)
			return -1;
		else if (type1 > type2)
			return 1;
	}
    else
	{
		if (type1 < type2)
			return 1;
		else if (type1 > type2)
			return -1;
	}
}

