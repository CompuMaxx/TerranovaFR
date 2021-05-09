#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_bg.h"
#include "battle_message.h"
#include "decompress.h"
#include "graphics.h"
#include "link.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "text_window.h"
#include "trig.h"
#include "constants/maps.h"
#include "constants/songs.h"
#include "constants/trainer_classes.h"

#define TAG_VS_LETTERS 10000

struct BattleBackground
{
    const void *tileset;
    const void *tilemap;
    const void *entryTileset;
    const void *entryTilemap;
    const void *palette;
};

static u8 GetBattleTerrainOverride(void);

static const u8 gUnknown_824829C[] = {1, 2};

static const struct OamData gOamData_82482A0 = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0x000,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct OamData gOamData_82482A8 = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0x040,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AffineAnimCmd gUnknown_82482B0[] = {
    AFFINEANIMCMD_FRAME(0x80, 0x80, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd gUnknown_82482C0[] = {
    AFFINEANIMCMD_FRAME(0x80, 0x80, 0, 0),
    AFFINEANIMCMD_FRAME(0x18, 0x18, 0x0, 0x80),
    AFFINEANIMCMD_FRAME(0x18, 0x18, 0x0, 0x80),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const gAffineAnimTable_82482E0[] = {
    gUnknown_82482B0,
    gUnknown_82482C0
};

static const struct SpriteTemplate sVsLetter_V_SpriteTemplate = {
    .tileTag = TAG_VS_LETTERS,
    .paletteTag = TAG_VS_LETTERS,
    .oam = &gOamData_82482A0,
    .anims = gDummySpriteAnimTable,
    .affineAnims = gAffineAnimTable_82482E0,
    .callback = SpriteCB_VsLetterDummy
};

static const struct SpriteTemplate sVsLetter_S_SpriteTemplate = {
    .tileTag = TAG_VS_LETTERS,
    .paletteTag = TAG_VS_LETTERS,
    .oam = &gOamData_82482A8,
    .anims = gDummySpriteAnimTable,
    .affineAnims = gAffineAnimTable_82482E0,
    .callback = SpriteCB_VsLetterDummy
};

static const struct CompressedSpriteSheet sVsLettersSpriteSheet = {
    gVsLettersGfx,
    0x1000,
    TAG_VS_LETTERS
};

const struct BgTemplate gBattleBgTemplates[4] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 24,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }, {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 28,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }, {
        .bg = 2,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0x000
    }, {
        .bg = 3,
        .charBaseIndex = 2,
        .mapBaseIndex = 26,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0x000
    }
};

static const struct WindowTemplate gUnknown_8248330[] = { //Fix baseBlock
    {	//Intro Texts
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 200
    }, { //What will\n{B_ACTIVE_NAME_WITH_PREFIX} do?
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 35,
        .width = 14,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 312
    }, { //Fight Bag\nPokémon Run
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    }, { //1st Move
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 55,
        .width = 10,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 368
    }, { //2nd Move
        .bg = 0,
        .tilemapLeft = 11,
        .tilemapTop = 55,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 388
    }, { //3th Move
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 57,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 408
    }, { //4th Move
        .bg = 0,
        .tilemapLeft = 11,
        .tilemapTop = 57,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 428
    }, { //PP Text
        .bg = 0,
        .tilemapLeft = 23,
        .tilemapTop = 55,
        .width = 2,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 448
    }, { //PP Count
        .bg = 0,
        .tilemapLeft = 25,
        .tilemapTop = 55,
        .width = 4,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 452
    }, { //Type Icon
        .bg = 0,
        .tilemapLeft = 23,
        .tilemapTop = 57,
        .width = 4,
        .height = 2,
        .paletteNum = 13,
        .baseBlock = 460
    }, { //Category Icon
        .bg = 0,
        .tilemapLeft = 27,
        .tilemapTop = 57,
        .width = 2,
        .height = 2,
        .paletteNum = 13,
        .baseBlock = 468
    }, {
		.bg = 0,
        .tilemapLeft = 0, //21,
        .tilemapTop = 0, //55,
        .width = 0, //8,
        .height = 0, //4,
        .paletteNum = 0,//5,
        .baseBlock = 0,//0x2b0
    }, {// Stats al subir nivel
        .bg = 1,
        .tilemapLeft = 19,
        .tilemapTop = 8,
        .width = 10,
        .height = 11,
        .paletteNum = 15,
        .baseBlock = 472
    }, {
        .bg = 2,
        .tilemapLeft = 18,
        .tilemapTop = 0,
        .width = 12,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 578
    }, { //Yes/No
        .bg = 0,
        .tilemapLeft = 25,
        .tilemapTop = 9,
        .width = 4,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 472
    }, {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 3,
        .width = 7,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x020
    }, {
        .bg = 2,
        .tilemapLeft = 2,
        .tilemapTop = 3,
        .width = 7,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x040
    }, {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 2,
        .width = 7,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x020
    }, {
        .bg = 2,
        .tilemapLeft = 2,
        .tilemapTop = 2,
        .width = 7,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x040
    }, {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 6,
        .width = 7,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x060
    }, {
        .bg = 2,
        .tilemapLeft = 2,
        .tilemapTop = 6,
        .width = 7,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x080
    }, {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 200
    }, {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 200
    }, {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 216
    }, { // Tutorial Texts
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 200
    }, DUMMY_WIN_TEMPLATE
};

static const u32 sBattleTerrainPalette_Grass[] = INCBIN_U32("graphics/battle/unk_8248400.gbapal.lz");
static const u32 sBattleTerrainTiles_Grass[] = INCBIN_U32("graphics/battle/unk_824844C.4bpp.lz");
static const u32 sBattleTerrainTilemap_Grass[] = INCBIN_U32("graphics/battle/unk_82489A8.bin.lz");
static const u32 sBattleTerrainAnimTiles_Grass[] = INCBIN_U32("graphics/battle/unk_8248C68.4bpp.lz");
static const u32 sBattleTerrainAnimTilemap_Grass[] = INCBIN_U32("graphics/battle/unk_8248F58.bin.lz");
static const u32 sBattleTerrainPalette_LongGrass[] = INCBIN_U32("graphics/battle/unk_8249074.gbapal.lz");
static const u32 sBattleTerrainTiles_LongGrass[] = INCBIN_U32("graphics/battle/unk_82490C4.4bpp.lz");
static const u32 sBattleTerrainTilemap_LongGrass[] = INCBIN_U32("graphics/battle/unk_8249620.bin.lz");
static const u32 sBattleTerrainAnimTiles_LongGrass[] = INCBIN_U32("graphics/battle/unk_82498DC.4bpp.lz");
static const u32 sBattleTerrainAnimTilemap_LongGrass[] = INCBIN_U32("graphics/battle/unk_8249E10.bin.lz");
static const u32 sBattleTerrainPalette_Sand[] = INCBIN_U32("graphics/battle/unk_8249F98.gbapal.lz");
static const u32 sBattleTerrainTiles_Sand[] = INCBIN_U32("graphics/battle/unk_8249FE4.4bpp.lz");
static const u32 sBattleTerrainTilemap_Sand[] = INCBIN_U32("graphics/battle/unk_824A37C.bin.lz");
static const u32 sBattleTerrainAnimTiles_Sand[] = INCBIN_U32("graphics/battle/unk_824A618.4bpp.lz");
static const u32 sBattleTerrainAnimTilemap_Sand[] = INCBIN_U32("graphics/battle/unk_824A844.bin.lz");
static const u32 sBattleTerrainPalette_Underwater[] = INCBIN_U32("graphics/battle/unk_824A940.gbapal.lz");
static const u32 sBattleTerrainTiles_Underwater[] = INCBIN_U32("graphics/battle/unk_824A990.4bpp.lz");
static const u32 sBattleTerrainTilemap_Underwater[] = INCBIN_U32("graphics/battle/unk_824ACD0.bin.lz");
static const u32 sBattleTerrainAnimTiles_Underwater[] = INCBIN_U32("graphics/battle/unk_824AF70.4bpp.lz");
static const u32 sBattleTerrainAnimTilemap_Underwater[] = INCBIN_U32("graphics/battle/unk_824B0DC.bin.lz");
static const u32 sBattleTerrainPalette_Water[] = INCBIN_U32("graphics/battle/unk_824B19C.gbapal.lz");
static const u32 sBattleTerrainTiles_Water[] = INCBIN_U32("graphics/battle/unk_824B1EC.4bpp.lz");
static const u32 sBattleTerrainTilemap_Water[] = INCBIN_U32("graphics/battle/unk_824B608.bin.lz");
static const u32 sBattleTerrainAnimTiles_Water[] = INCBIN_U32("graphics/battle/unk_824B8A8.4bpp.lz");
static const u32 sBattleTerrainAnimTilemap_Water[] = INCBIN_U32("graphics/battle/unk_824BBE0.bin.lz");
static const u32 sBattleTerrainPalette_Pond[] = INCBIN_U32("graphics/battle/unk_824BCE0.gbapal.lz");
static const u32 sBattleTerrainTiles_Pond[] = INCBIN_U32("graphics/battle/unk_824BD38.4bpp.lz");
static const u32 sBattleTerrainTilemap_Pond[] = INCBIN_U32("graphics/battle/unk_824C07C.bin.lz");
static const u32 sBattleTerrainAnimTiles_Pond[] = INCBIN_U32("graphics/battle/unk_824C314.4bpp.lz");
static const u32 sBattleTerrainAnimTilemap_Pond[] = INCBIN_U32("graphics/battle/unk_824C520.bin.lz");
static const u32 sBattleTerrainPalette_Mountain[] = INCBIN_U32("graphics/battle/unk_824C5D8.gbapal.lz");
static const u32 sBattleTerrainTiles_Mountain[] = INCBIN_U32("graphics/battle/unk_824C624.4bpp.lz");
static const u32 sBattleTerrainTilemap_Mountain[] = INCBIN_U32("graphics/battle/unk_824C958.bin.lz");
static const u32 sBattleTerrainAnimTiles_Mountain[] = INCBIN_U32("graphics/battle/unk_824CBF8.4bpp.lz");
static const u32 sBattleTerrainAnimTilemap_Mountain[] = INCBIN_U32("graphics/battle/unk_824CEC8.bin.lz");
static const u32 sBattleTerrainPalette_Cave[] = INCBIN_U32("graphics/battle/unk_824CF98.gbapal.lz");
static const u32 sBattleTerrainTiles_Cave[] = INCBIN_U32("graphics/battle/unk_824CFEC.4bpp.lz");
static const u32 sBattleTerrainTilemap_Cave[] = INCBIN_U32("graphics/battle/unk_824D418.bin.lz");
static const u32 sBattleTerrainAnimTiles_Cave[] = INCBIN_U32("graphics/battle/unk_824D6B8.4bpp.lz");
static const u32 sBattleTerrainAnimTilemap_Cave[] = INCBIN_U32("graphics/battle/unk_824DC98.bin.lz");
static const u32 sBattleTerrainPalette_Building[] = INCBIN_U32("graphics/battle/unk_824DDF0.gbapal.lz");
static const u32 sBattleTerrainTiles_Building[] = INCBIN_U32("graphics/battle/unk_824DE34.4bpp.lz");
static const u32 sBattleTerrainTilemap_Building[] = INCBIN_U32("graphics/battle/unk_824E16C.bin.lz");
static const u32 sBattleTerrainAnimTiles_Building[] = INCBIN_U32("graphics/battle/unk_824E410.4bpp.lz");
static const u32 sBattleTerrainAnimTilemap_Building[] = INCBIN_U32("graphics/battle/unk_824E490.bin.lz");
static const u32 sBattleTerrainPalette_Link[] = INCBIN_U32("graphics/battle/unk_824E528.gbapal.lz");
static const u32 sBattleTerrainPalette_Gym[] = INCBIN_U32("graphics/battle/unk_824E56C.gbapal.lz");
static const u32 sBattleTerrainPalette_Leader[] = INCBIN_U32("graphics/battle/unk_824E5B8.gbapal.lz");
static const u32 sBattleTerrainPalette_Indoor2[] = INCBIN_U32("graphics/battle/unk_824E604.gbapal.lz");
static const u32 sBattleTerrainPalette_Indoor1[] = INCBIN_U32("graphics/battle/unk_824E650.gbapal.lz");
static const u32 sBattleTerrainPalette_Lorelei[] = INCBIN_U32("graphics/battle/unk_824E6A4.gbapal.lz");
static const u32 sBattleTerrainPalette_Bruno[] = INCBIN_U32("graphics/battle/unk_824E6F0.gbapal.lz");
static const u32 sBattleTerrainPalette_Agatha[] = INCBIN_U32("graphics/battle/unk_824E740.gbapal.lz");
static const u32 sBattleTerrainPalette_Lance[] = INCBIN_U32("graphics/battle/unk_824E78C.gbapal.lz");
static const u32 sBattleTerrainPalette_Champion[] = INCBIN_U32("graphics/battle/unk_824E7DC.gbapal.lz");
static const u32 sBattleTerrainPalette_Plain[] = INCBIN_U32("graphics/battle/unk_824E81C.gbapal.lz");
static const u32 sBattleTerrainTiles_Indoor2[] = INCBIN_U32("graphics/battle/unk_824E858.4bpp.lz");
static const u32 sBattleTerrainTilemap_Indoor2[] = INCBIN_U32("graphics/battle/unk_824EB90.bin.lz");

static const struct BattleBackground sBattleTerrainTable[] = {
    [BATTLE_TERRAIN_GRASS] =
    {
        .tileset = sBattleTerrainTiles_Grass,
        .tilemap = sBattleTerrainTilemap_Grass,
        .entryTileset = sBattleTerrainAnimTiles_Grass,
        .entryTilemap = sBattleTerrainAnimTilemap_Grass,
        .palette = sBattleTerrainPalette_Grass
    },
    [BATTLE_TERRAIN_LONG_GRASS] =
    {
        .tileset = sBattleTerrainTiles_LongGrass,
        .tilemap = sBattleTerrainTilemap_LongGrass,
        .entryTileset = sBattleTerrainAnimTiles_LongGrass,
        .entryTilemap = sBattleTerrainAnimTilemap_LongGrass,
        .palette = sBattleTerrainPalette_LongGrass
    },
    [BATTLE_TERRAIN_SAND] =
    {
        .tileset = sBattleTerrainTiles_Sand,
        .tilemap = sBattleTerrainTilemap_Sand,
        .entryTileset = sBattleTerrainAnimTiles_Sand,
        .entryTilemap = sBattleTerrainAnimTilemap_Sand,
        .palette = sBattleTerrainPalette_Sand
    },
    [BATTLE_TERRAIN_UNDERWATER] =
    {
        .tileset = sBattleTerrainTiles_Underwater,
        .tilemap = sBattleTerrainTilemap_Underwater,
        .entryTileset = sBattleTerrainAnimTiles_Underwater,
        .entryTilemap = sBattleTerrainAnimTilemap_Underwater,
        .palette = sBattleTerrainPalette_Underwater
    },
    [BATTLE_TERRAIN_WATER] =
    {
        .tileset = sBattleTerrainTiles_Water,
        .tilemap = sBattleTerrainTilemap_Water,
        .entryTileset = sBattleTerrainAnimTiles_Water,
        .entryTilemap = sBattleTerrainAnimTilemap_Water,
        .palette = sBattleTerrainPalette_Water
    },
    [BATTLE_TERRAIN_POND] =
    {
        .tileset = sBattleTerrainTiles_Pond,
        .tilemap = sBattleTerrainTilemap_Pond,
        .entryTileset = sBattleTerrainAnimTiles_Pond,
        .entryTilemap = sBattleTerrainAnimTilemap_Pond,
        .palette = sBattleTerrainPalette_Pond
    },
    [BATTLE_TERRAIN_MOUNTAIN] =
    {
        .tileset = sBattleTerrainTiles_Mountain,
        .tilemap = sBattleTerrainTilemap_Mountain,
        .entryTileset = sBattleTerrainAnimTiles_Mountain,
        .entryTilemap = sBattleTerrainAnimTilemap_Mountain,
        .palette = sBattleTerrainPalette_Mountain
    },
    [BATTLE_TERRAIN_CAVE] =
    {
        .tileset = sBattleTerrainTiles_Cave,
        .tilemap = sBattleTerrainTilemap_Cave,
        .entryTileset = sBattleTerrainAnimTiles_Cave,
        .entryTilemap = sBattleTerrainAnimTilemap_Cave,
        .palette = sBattleTerrainPalette_Cave
    },
    [BATTLE_TERRAIN_BUILDING] =
    {
        .tileset = sBattleTerrainTiles_Building,
        .tilemap = sBattleTerrainTilemap_Building,
        .entryTileset = sBattleTerrainAnimTiles_Building,
        .entryTilemap = sBattleTerrainAnimTilemap_Building,
        .palette = sBattleTerrainPalette_Building
    },
    [BATTLE_TERRAIN_PLAIN] =
    {
        .tileset = sBattleTerrainTiles_Building,
        .tilemap = sBattleTerrainTilemap_Building,
        .entryTileset = sBattleTerrainAnimTiles_Building,
        .entryTilemap = sBattleTerrainAnimTilemap_Building,
        .palette = sBattleTerrainPalette_Plain
    },
    [BATTLE_TERRAIN_LINK] =
    {
        .tileset = sBattleTerrainTiles_Building,
        .tilemap = sBattleTerrainTilemap_Building,
        .entryTileset = sBattleTerrainAnimTiles_Building,
        .entryTilemap = sBattleTerrainAnimTilemap_Building,
        .palette = sBattleTerrainPalette_Link
    },
    [BATTLE_TERRAIN_GYM] =
    {
        .tileset = sBattleTerrainTiles_Building,
        .tilemap = sBattleTerrainTilemap_Building,
        .entryTileset = sBattleTerrainAnimTiles_Building,
        .entryTilemap = sBattleTerrainAnimTilemap_Building,
        .palette = sBattleTerrainPalette_Gym
    },
    [BATTLE_TERRAIN_LEADER] =
    {
        .tileset = sBattleTerrainTiles_Building,
        .tilemap = sBattleTerrainTilemap_Building,
        .entryTileset = sBattleTerrainAnimTiles_Building,
        .entryTilemap = sBattleTerrainAnimTilemap_Building,
        .palette = sBattleTerrainPalette_Leader
    },
    [BATTLE_TERRAIN_INDOOR_2] =
    {
        .tileset = sBattleTerrainTiles_Indoor2,
        .tilemap = sBattleTerrainTilemap_Indoor2,
        .entryTileset = sBattleTerrainAnimTiles_Building,
        .entryTilemap = sBattleTerrainAnimTilemap_Building,
        .palette = sBattleTerrainPalette_Indoor2
    },
    [BATTLE_TERRAIN_INDOOR_1] =
    {
        .tileset = sBattleTerrainTiles_Indoor2,
        .tilemap = sBattleTerrainTilemap_Indoor2,
        .entryTileset = sBattleTerrainAnimTiles_Building,
        .entryTilemap = sBattleTerrainAnimTilemap_Building,
        .palette = sBattleTerrainPalette_Indoor1
    },
    [BATTLE_TERRAIN_LORELEI] =
    {
        .tileset = sBattleTerrainTiles_Indoor2,
        .tilemap = sBattleTerrainTilemap_Indoor2,
        .entryTileset = sBattleTerrainAnimTiles_Building,
        .entryTilemap = sBattleTerrainAnimTilemap_Building,
        .palette = sBattleTerrainPalette_Lorelei
    },
    [BATTLE_TERRAIN_BRUNO] =
    {
        .tileset = sBattleTerrainTiles_Indoor2,
        .tilemap = sBattleTerrainTilemap_Indoor2,
        .entryTileset = sBattleTerrainAnimTiles_Building,
        .entryTilemap = sBattleTerrainAnimTilemap_Building,
        .palette = sBattleTerrainPalette_Bruno
    },
    [BATTLE_TERRAIN_AGATHA] =
    {
        .tileset = sBattleTerrainTiles_Indoor2,
        .tilemap = sBattleTerrainTilemap_Indoor2,
        .entryTileset = sBattleTerrainAnimTiles_Building,
        .entryTilemap = sBattleTerrainAnimTilemap_Building,
        .palette = sBattleTerrainPalette_Agatha
    },
    [BATTLE_TERRAIN_LANCE] =
    {
        .tileset = sBattleTerrainTiles_Indoor2,
        .tilemap = sBattleTerrainTilemap_Indoor2,
        .entryTileset = sBattleTerrainAnimTiles_Building,
        .entryTilemap = sBattleTerrainAnimTilemap_Building,
        .palette = sBattleTerrainPalette_Lance
    },
    [BATTLE_TERRAIN_CHAMPION] =
    {
        .tileset = sBattleTerrainTiles_Indoor2,
        .tilemap = sBattleTerrainTilemap_Indoor2,
        .entryTileset = sBattleTerrainAnimTiles_Building,
        .entryTilemap = sBattleTerrainAnimTilemap_Building,
        .palette = sBattleTerrainPalette_Champion
    }
};

static const struct {
    u8 mapScene;
    u8 battleTerrain;
} sMapBattleSceneMapping[] = {
    {MAP_BATTLE_SCENE_GYM,      BATTLE_TERRAIN_GYM},
    {MAP_BATTLE_SCENE_INDOOR_1, BATTLE_TERRAIN_INDOOR_1},
    {MAP_BATTLE_SCENE_INDOOR_2, BATTLE_TERRAIN_INDOOR_2},
    {MAP_BATTLE_SCENE_LORELEI,  BATTLE_TERRAIN_LORELEI},
    {MAP_BATTLE_SCENE_BRUNO,    BATTLE_TERRAIN_BRUNO},
    {MAP_BATTLE_SCENE_AGATHA,   BATTLE_TERRAIN_AGATHA},
    {MAP_BATTLE_SCENE_LANCE,    BATTLE_TERRAIN_LANCE},
    {MAP_BATTLE_SCENE_LINK,     BATTLE_TERRAIN_LINK}
};

static u8 GetBattleTerrainByMapScene(u8 mapBattleScene)
{
    int i;
    for (i = 0; i < NELEMS(sMapBattleSceneMapping); i++)
    {
        if (mapBattleScene == sMapBattleSceneMapping[i].mapScene)
            return sMapBattleSceneMapping[i].battleTerrain;
    }
    return 9;
}

static void LoadBattleTerrainGfx(u16 terrain)
{
    if (terrain >= NELEMS(sBattleTerrainTable))
        terrain = 9;
    // Copy to bg3
    LZDecompressVram(sBattleTerrainTable[terrain].tileset, (void *)BG_CHAR_ADDR(2));
    LZDecompressVram(sBattleTerrainTable[terrain].tilemap, (void *)BG_SCREEN_ADDR(26));
    LoadCompressedPalette(sBattleTerrainTable[terrain].palette, 0x20, 0x60);
}

static void LoadBattleTerrainEntryGfx(u16 terrain)
{
    if (terrain >= NELEMS(sBattleTerrainTable))
        terrain = 9;
    // Copy to bg1
    LZDecompressVram(sBattleTerrainTable[terrain].entryTileset, (void *)BG_CHAR_ADDR(1));
    LZDecompressVram(sBattleTerrainTable[terrain].entryTilemap, (void *)BG_SCREEN_ADDR(28));
}

void sub_800F324(void)
{
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, gBattleBgTemplates, NELEMS(gBattleBgTemplates));
    InitWindows(gUnknown_8248330);
    DeactivateAllTextPrinters();
}

void InitBattleBgsVideo(void)
{
    EnableInterrupts(INTR_FLAG_VBLANK | INTR_FLAG_VCOUNT | INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL);
    sub_800F324();
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_OBJWIN_ON);
}

void LoadBattleMenuWindowGfx(void)
{
    TextWindow_SetUserSelectedFrame(2, 0x022, 0xC0);
    gPlttBufferUnfaded[0xFC] = RGB( 9,  9,  9);
    gPlttBufferUnfaded[0xFD] = RGB( 9,  9,  9);
    gPlttBufferUnfaded[0xFE] = RGB(31, 31, 31);
    gPlttBufferUnfaded[0xFF] = RGB( 26,  26,  25);
    CpuCopy16(&gPlttBufferUnfaded[0xFC], &gPlttBufferFaded[0xFC], 8);
}

void DrawMainBattleBackground(void)
{
    LoadBattleTerrainGfx(GetBattleTerrainOverride());
}

void LoadBattleTextboxAndBackground(void)
{
    LZDecompressVram(gBattleTiles, (void *)BG_CHAR_ADDR(0));
	CopyToBgTilemapBuffer(0, gBattleFramesTilemap, 0, 0x000);

    if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
		{
			CopyToBgTilemapBuffer(0, gSafariEngTilemap, 0, 68);
		}
		else
		{
			CopyToBgTilemapBuffer(0, gBattleEngTilemap, 0, 68);
		}
	}
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
		{
			CopyToBgTilemapBuffer(0, gSafariSpaTilemap, 0, 68);
		}
		else
		{
			CopyToBgTilemapBuffer(0, gBattleSpaTilemap, 0, 68);
		}
	}
	LoadPalette(gFireRedMenuElements2_Pal, 0xD0, 0x20);
    LoadPalette(gBattleFramesPalette, 0xE0, 0x20);
	LoadPalette(gBattleTextPalette, 0xF0, 0x20);
	CopyBgTilemapBufferToVram(0);
    LoadBattleMenuWindowGfx();
    DrawMainBattleBackground();
}

static void DrawLinkBattleParticipantPokeballs(u8 taskId, u8 multiplayerId, u8 bgId, u8 destX, u8 destY)
{
    s32 i;
    u16 pokeballStatuses = 0;
    u16 tiles[6];

    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if (gTasks[taskId].data[5] != 0)
        {
            switch (multiplayerId)
            {
            case 0:
                pokeballStatuses = 0x3F & gTasks[taskId].data[3];
                break;
            case 1:
                pokeballStatuses = (0xFC0 & gTasks[taskId].data[4]) >> 6;
                break;
            case 2:
                pokeballStatuses = (0xFC0 & gTasks[taskId].data[3]) >> 6;
                break;
            case 3:
                pokeballStatuses = 0x3F & gTasks[taskId].data[4];
                break;
            }
        }
        else
        {
            switch (multiplayerId)
            {
            case 0:
                pokeballStatuses = 0x3F & gTasks[taskId].data[3];
                break;
            case 1:
                pokeballStatuses = 0x3F & gTasks[taskId].data[4];
                break;
            case 2:
                pokeballStatuses = (0xFC0 & gTasks[taskId].data[3]) >> 6;
                break;
            case 3:
                pokeballStatuses = (0xFC0 & gTasks[taskId].data[4]) >> 6;
                break;
            }
        }

        for (i = 0; i < 3; i++)
            tiles[i] = ((pokeballStatuses & (3 << (i * 2))) >> (i * 2)) + 0x6001;

        CopyToBgTilemapBufferRect_ChangePalette(bgId, tiles, destX, destY, 3, 1, 0x11);
        CopyBgTilemapBufferToVram(bgId);
    }
    else
    {
        if (multiplayerId == gBattleStruct->multiplayerId)
            pokeballStatuses = gTasks[taskId].data[3];
        else
            pokeballStatuses = gTasks[taskId].data[4];

        for (i = 0; i < 6; i++)
            tiles[i] = ((pokeballStatuses & (3 << (i * 2))) >> (i * 2)) + 0x6001;

        CopyToBgTilemapBufferRect_ChangePalette(bgId, tiles, destX, destY, 6, 1, 0x11);
        CopyBgTilemapBufferToVram(bgId);
    }
}

static void DrawLinkBattleVsScreenOutcomeText(void)
{
    if (gBattleOutcome == B_OUTCOME_DREW)
    {
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			BattlePutTextOnWindow(gText_Draw, 0x15);
        else
			BattlePutTextOnWindow(gText_DrawSpa, 0x15);
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			if (gBattleOutcome == B_OUTCOME_WON)
			{
				switch (gLinkPlayers[gBattleStruct->multiplayerId].id)
				{
				case 0:
					BattlePutTextOnWindow(gText_Win, 0x16);
					BattlePutTextOnWindow(gText_Loss, 0x17);
					break;
				case 1:
					BattlePutTextOnWindow(gText_Win, 0x17);
					BattlePutTextOnWindow(gText_Loss, 0x16);
					break;
				case 2:
					BattlePutTextOnWindow(gText_Win, 0x16);
					BattlePutTextOnWindow(gText_Loss, 0x17);
					break;
				case 3:
					BattlePutTextOnWindow(gText_Win, 0x17);
					BattlePutTextOnWindow(gText_Loss, 0x16);
					break;
            }
        }
        else
        {
            switch (gLinkPlayers[gBattleStruct->multiplayerId].id)
            {
            case 0:
                BattlePutTextOnWindow(gText_Win, 0x17);
                BattlePutTextOnWindow(gText_Loss, 0x16);
                break;
            case 1:
                BattlePutTextOnWindow(gText_Win, 0x16);
                BattlePutTextOnWindow(gText_Loss, 0x17);
                break;
            case 2:
                BattlePutTextOnWindow(gText_Win, 0x17);
                BattlePutTextOnWindow(gText_Loss, 0x16);
                break;
            case 3:
                BattlePutTextOnWindow(gText_Win, 0x16);
                BattlePutTextOnWindow(gText_Loss, 0x17);
                break;
            }
        }
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			if (gBattleOutcome == B_OUTCOME_WON)
			{
				switch (gLinkPlayers[gBattleStruct->multiplayerId].id)
				{
				case 0:
					BattlePutTextOnWindow(gText_WinSpa, 0x16);
					BattlePutTextOnWindow(gText_LossSpa, 0x17);
					break;
				case 1:
					BattlePutTextOnWindow(gText_WinSpa, 0x17);
					BattlePutTextOnWindow(gText_LossSpa, 0x16);
					break;
				case 2:
					BattlePutTextOnWindow(gText_WinSpa, 0x16);
					BattlePutTextOnWindow(gText_LossSpa, 0x17);
					break;
				case 3:
					BattlePutTextOnWindow(gText_WinSpa, 0x17);
					BattlePutTextOnWindow(gText_LossSpa, 0x16);
					break;
				}
			}
			else
			{
				switch (gLinkPlayers[gBattleStruct->multiplayerId].id)
				{
				case 0:
					BattlePutTextOnWindow(gText_WinSpa, 0x17);
					BattlePutTextOnWindow(gText_LossSpa, 0x16);
					break;
				case 1:
					BattlePutTextOnWindow(gText_WinSpa, 0x16);
					BattlePutTextOnWindow(gText_LossSpa, 0x17);
					break;
				case 2:
					BattlePutTextOnWindow(gText_WinSpa, 0x17);
					BattlePutTextOnWindow(gText_LossSpa, 0x16);
					break;
				case 3:
					BattlePutTextOnWindow(gText_WinSpa, 0x16);
					BattlePutTextOnWindow(gText_LossSpa, 0x17);
					break;
				}
			}
		}
    }
    else if (gBattleOutcome == B_OUTCOME_WON)
    {
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			if (gLinkPlayers[gBattleStruct->multiplayerId].id != 0)
			{
				BattlePutTextOnWindow(gText_Win, 0x17);
				BattlePutTextOnWindow(gText_Loss, 0x16);
			}
			else
			{
				BattlePutTextOnWindow(gText_Win, 0x16);
				BattlePutTextOnWindow(gText_Loss, 0x17);
			}
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			if (gLinkPlayers[gBattleStruct->multiplayerId].id != 0)
			{
				BattlePutTextOnWindow(gText_WinSpa, 0x17);
				BattlePutTextOnWindow(gText_LossSpa, 0x16);
			}
			else
			{
				BattlePutTextOnWindow(gText_WinSpa, 0x16);
				BattlePutTextOnWindow(gText_LossSpa, 0x17);
			}
		}
	}
    else
    {
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			if (gLinkPlayers[gBattleStruct->multiplayerId].id != 0)
        {
            BattlePutTextOnWindow(gText_Win, 0x16);
            BattlePutTextOnWindow(gText_Loss, 0x17);
        }
        else
        {
            BattlePutTextOnWindow(gText_Win, 0x17);
            BattlePutTextOnWindow(gText_Loss, 0x16);
        }
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			if (gLinkPlayers[gBattleStruct->multiplayerId].id != 0)
        {
            BattlePutTextOnWindow(gText_WinSpa, 0x16);
            BattlePutTextOnWindow(gText_LossSpa, 0x17);
        }
        else
        {
            BattlePutTextOnWindow(gText_WinSpa, 0x17);
            BattlePutTextOnWindow(gText_LossSpa, 0x16);
        }
		}
    }
}

void InitLinkBattleVsScreen(u8 taskId)
{
    struct LinkPlayer *linkPlayer;
    u8 *name;
    s32 i, palId;

    switch (gTasks[taskId].data[0])
    {
    case 0:
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            for (i = 0; i < MAX_BATTLERS_COUNT; i++)
            {
                name = gLinkPlayers[i].name;
                linkPlayer = &gLinkPlayers[i];

                switch (linkPlayer->id)
                {
                case 0:
                    BattlePutTextOnWindow(name, 0x11);
                    DrawLinkBattleParticipantPokeballs(taskId, linkPlayer->id, 1, 2, 4);
                    break;
                case 1:
                    BattlePutTextOnWindow(name, 0x12);
                    DrawLinkBattleParticipantPokeballs(taskId, linkPlayer->id, 2, 2, 4);
                    break;
                case 2:
                    BattlePutTextOnWindow(name, 0x13);
                    DrawLinkBattleParticipantPokeballs(taskId, linkPlayer->id, 1, 2, 8);
                    break;
                case 3:
                    BattlePutTextOnWindow(name, 0x14);
                    DrawLinkBattleParticipantPokeballs(taskId, linkPlayer->id, 2, 2, 8);
                    break;
                }
            }
        }
        else
        {
            u8 playerId = gBattleStruct->multiplayerId;
            u8 opponentId = playerId ^ BIT_SIDE;
            u8 opponentId_copy = opponentId;

            if (gLinkPlayers[playerId].id != 0)
                opponentId = playerId, playerId = opponentId_copy;

            name = gLinkPlayers[playerId].name;
            BattlePutTextOnWindow(name, 0xF);

            name = gLinkPlayers[opponentId].name;
            BattlePutTextOnWindow(name, 0x10);

            DrawLinkBattleParticipantPokeballs(taskId, playerId, 1, 2, 7);
            DrawLinkBattleParticipantPokeballs(taskId, opponentId, 2, 2, 7);
        }
        gTasks[taskId].data[0]++;
        break;
    case 1:
        palId = AllocSpritePalette(TAG_VS_LETTERS);
        gPlttBufferUnfaded[palId * 16 + 0x10F] = gPlttBufferFaded[palId * 16 + 0x10F] = RGB(31, 31, 31);
        gBattleStruct->linkBattleVsSpriteId_V = CreateSprite(&sVsLetter_V_SpriteTemplate, 108, 80, 0);
        gBattleStruct->linkBattleVsSpriteId_S = CreateSprite(&sVsLetter_S_SpriteTemplate, 132, 80, 0);
        gSprites[gBattleStruct->linkBattleVsSpriteId_V].invisible = TRUE;
        gSprites[gBattleStruct->linkBattleVsSpriteId_S].invisible = TRUE;
        gTasks[taskId].data[0]++;
        break;
    case 2:
        if (gTasks[taskId].data[5] != 0)
        {
            gBattle_BG1_X = -(20) - (Sin2(gTasks[taskId].data[1]) / 32);
            gBattle_BG2_X = -(140) - (Sin2(gTasks[taskId].data[2]) / 32);
            gBattle_BG1_Y = -36;
            gBattle_BG2_Y = -36;
        }
        else
        {
            gBattle_BG1_X = -(20) - (Sin2(gTasks[taskId].data[1]) / 32);
            gBattle_BG1_Y = (Cos2(gTasks[taskId].data[1]) / 32) - 164;
            gBattle_BG2_X = -(140) - (Sin2(gTasks[taskId].data[2]) / 32);
            gBattle_BG2_Y = (Cos2(gTasks[taskId].data[2]) / 32) - 164;
        }

        if (gTasks[taskId].data[2] != 0)
        {
            gTasks[taskId].data[2] -= 2;
            gTasks[taskId].data[1] += 2;
        }
        else
        {
            if (gTasks[taskId].data[5] != 0)
                DrawLinkBattleVsScreenOutcomeText();

            PlaySE(SE_M_HARDEN);
            DestroyTask(taskId);
            gSprites[gBattleStruct->linkBattleVsSpriteId_V].invisible = FALSE;
            gSprites[gBattleStruct->linkBattleVsSpriteId_S].invisible = FALSE;
            gSprites[gBattleStruct->linkBattleVsSpriteId_S].oam.tileNum += 0x40;
            gSprites[gBattleStruct->linkBattleVsSpriteId_V].data[0] = 0;
            gSprites[gBattleStruct->linkBattleVsSpriteId_S].data[0] = 1;
            gSprites[gBattleStruct->linkBattleVsSpriteId_V].data[1] = gSprites[gBattleStruct->linkBattleVsSpriteId_V].pos1.x;
            gSprites[gBattleStruct->linkBattleVsSpriteId_S].data[1] = gSprites[gBattleStruct->linkBattleVsSpriteId_S].pos1.x;
            gSprites[gBattleStruct->linkBattleVsSpriteId_V].data[2] = 0;
            gSprites[gBattleStruct->linkBattleVsSpriteId_S].data[2] = 0;
        }
        break;
    }
}

void DrawBattleEntryBackground(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        LZDecompressVram(gFile_graphics_battle_transitions_vs_frame_sheet, (void*)(BG_CHAR_ADDR(1)));
        LZDecompressVram(gVsLettersGfx, (void*)(VRAM + 0x10000));
        LoadCompressedPalette(gFile_graphics_battle_transitions_vs_frame_palette, 0x60, 0x20);
        SetBgAttribute(1, BG_ATTR_SCREENSIZE, 1);
        SetGpuReg(REG_OFFSET_BG1CNT, BGCNT_PRIORITY(0) | BGCNT_CHARBASE(1) | BGCNT_16COLOR | BGCNT_SCREENBASE(28) | BGCNT_TXT512x256);
        CopyToBgTilemapBuffer(1, gFile_graphics_battle_transitions_vs_frame_tilemap, 0, 0);
        CopyToBgTilemapBuffer(2, gFile_graphics_battle_transitions_vs_frame_tilemap, 0, 0);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(2);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ | WININ_WIN0_CLR);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        gBattle_BG1_Y = -164;
        gBattle_BG2_Y = -164;
        LoadCompressedSpriteSheetUsingHeap(&sVsLettersSpriteSheet);
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
    {
        LoadBattleTerrainEntryGfx(BATTLE_TERRAIN_GRASS);
    }
    else if (gBattleTypeFlags & (BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_LINK | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_EREADER_TRAINER))
    {
        LoadBattleTerrainEntryGfx(BATTLE_TERRAIN_BUILDING);
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_KYOGRE_GROUDON)
    {
        if (gGameVersion == VERSION_FIRE_RED)
        {
            LoadBattleTerrainEntryGfx(BATTLE_TERRAIN_CAVE);
        }
        else
        {
            LoadBattleTerrainEntryGfx(BATTLE_TERRAIN_WATER);
        }
    }
    else
    {
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            u8 trainerClass = gTrainers[gTrainerBattleOpponent_A].trainerClass;
            if (trainerClass == CLASS_LEADER_2)
            {
                LoadBattleTerrainEntryGfx(BATTLE_TERRAIN_BUILDING);
                return;
            }
            else if (trainerClass == CLASS_CHAMPION_2)
            {
                LoadBattleTerrainEntryGfx(BATTLE_TERRAIN_BUILDING);
                return;
            }
        }

        if (GetCurrentMapBattleScene() == MAP_BATTLE_SCENE_NORMAL)
        {
            LoadBattleTerrainEntryGfx(gBattleTerrain);
        }
        else
        {
            LoadBattleTerrainEntryGfx(BATTLE_TERRAIN_BUILDING);
        }
    }
}

static u8 GetBattleTerrainOverride(void)
{
    u8 battleScene;
    if (gBattleTypeFlags & (BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_LINK | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_EREADER_TRAINER))
    {
        return BATTLE_TERRAIN_LINK;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
    {
        gBattleTerrain = BATTLE_TERRAIN_GRASS;
        return BATTLE_TERRAIN_GRASS;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        if (gTrainers[gTrainerBattleOpponent_A].trainerClass == CLASS_LEADER_2)
        {
            return BATTLE_TERRAIN_LEADER;
        }
        else if (gTrainers[gTrainerBattleOpponent_A].trainerClass == CLASS_CHAMPION_2)
        {
            return BATTLE_TERRAIN_CHAMPION;
        }
    }
    battleScene = GetCurrentMapBattleScene();
    if (battleScene == MAP_BATTLE_SCENE_NORMAL)
    {
        return gBattleTerrain;
    }
    return GetBattleTerrainByMapScene(battleScene);
}

bool8 LoadChosenBattleElement(u8 caseId)
{
    bool8 ret = FALSE;
    u8 battleScene;
    switch (caseId)
    {
    case 0:
		LoadBattleTextboxAndBackground();
		break;
    case 1:
        battleScene = GetBattleTerrainOverride();
        LZDecompressVram(sBattleTerrainTable[battleScene].tileset, (void *)BG_CHAR_ADDR(2));
        break;
    case 2:
        battleScene = GetBattleTerrainOverride();
        LZDecompressVram(sBattleTerrainTable[battleScene].tilemap, (void *)BG_SCREEN_ADDR(26));
        break;
    case 3:
        battleScene = GetBattleTerrainOverride();
        LoadCompressedPalette(sBattleTerrainTable[battleScene].palette, 0x20, 0x60);
        break;
    case 4:
        LoadBattleMenuWindowGfx();
        break;
    default:
        ret = TRUE;
        break;
    }
    return ret;
}
