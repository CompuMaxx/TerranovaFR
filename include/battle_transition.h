#ifndef GUARD_BATTLE_TRANSITION_H
#define GUARD_BATTLE_TRANSITION_H

#include "global.h"

enum
{
    MUGSHOT_LORELEI,
    MUGSHOT_BRUNO,
    MUGSHOT_AGATHA,
    MUGSHOT_LANCE,
    MUGSHOT_BLUE,
    MUGSHOTS_COUNT
};

/*
#define	BLAINE			0
#define	BROCK			1
#define	BUGSY			2
#define	CHUCK			3
#define	CLAIR			4
#define	ERIKA			5
#define	FALKNER			6
#define	GARY			7
#define	JANINE			8
#define	JASMINE			9
#define	LT_SURGE		10
#define	MISTY			11
#define	MORTY			12
#define	PRYCE			13
#define	SABRINA			14
#define	WHITNEY			15
#define	BYRON			16
#define	CANDICE			17
#define	CRASHER_WAKE	18
#define	FANTINA			19
#define	GARDENIA		20
#define	MAYLENE			21
#define	ROARK			22
#define	VOLKNER			23
#define	ARGENTA			24
#define	DAHLIA			25
#define	DARACH			26
#define	PALMER			27
#define	THORTON			28
#define	BRUNO			29
#define	KAREN			30
#define	KOGA			31
#define	LANCE			32
#define	WILL			33
#define	AARON			34
#define	BERTHA			35
#define	CYNTHIA			36
#define	FLINT			37
#define	LUCIAN			38
#define	ARCHER			39
#define	ARIANA			40
#define	GIOVANNI		41
#define	PETREL			42
#define	PROTON			43
#define	SILVER			44

#define	GYM_HGSS		16
#define	GYM_PT			24
#define	PIRAMID			29
#define	ELITE_4_HGSS	34
#define	ELITE_4_PT		39
#define	TEAM_ROCKET		44 */

#define B_TRANSITION_BLUR 0
#define B_TRANSITION_DISTORTED_WAVE 1
#define B_TRANSITION_HORIZONTAL_CORRUGATE 2
#define B_TRANSITION_BIG_POKEBALL 3
#define B_TRANSITION_SLIDING_POKEBALLS 4
#define B_TRANSITION_CLOCKWISE_BLACKFADE 5
#define B_TRANSITION_FULLSCREEN_WAVE 6
#define B_TRANSITION_BLACK_WAVE_TO_RIGHT 7
#define B_TRANSITION_SLICED_SCREEN 8
#define B_TRANSITION_WHITEFADE_IN_STRIPES 9
#define B_TRANSITION_GRID_SQUARES 10
#define B_TRANSITION_BLACK_DOODLES 11
#define B_TRANSITION_LORELEI 12
#define B_TRANSITION_BRUNO 13
#define B_TRANSITION_AGATHA 14
#define B_TRANSITION_LANCE 15
#define B_TRANSITION_BLUE 16
#define B_TRANSITION_ANTI_CLOCKWISE_SPIRAL 17
#define B_TRANSITION_VS_GYM_HGSS 18
#define B_TRANSITION_VS_GYM_PT 19
#define B_TRANSITION_VS_PIRAMID 20
#define B_TRANSITION_VS_ELITE_4_HGSS 21
#define B_TRANSITION_VS_ELITE_4_PT 22
#define B_TRANSITION_VS_TEAM_ROCKET 23

extern const struct SpritePalette sSpritePalette_SlidingPokeball;

bool8 BT_IsDone(void);
void BT_StartOnField(u8 transitionId);
bool8 FldEff_Pokeball(void);

#endif // GUARD_BATTLE_TRANSITION_H
