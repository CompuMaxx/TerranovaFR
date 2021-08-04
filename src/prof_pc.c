#include "global.h"
#include "event_data.h"
#include "pokedex.h"
#include "field_message_box.h"

extern const u8 PokedexRating_Text_LessThan10[];
extern const u8 PokedexRating_Text_LessThan20[];
extern const u8 PokedexRating_Text_LessThan30[];
extern const u8 PokedexRating_Text_LessThan40[];
extern const u8 PokedexRating_Text_LessThan50[];
extern const u8 PokedexRating_Text_LessThan60[];
extern const u8 PokedexRating_Text_LessThan70[];
extern const u8 PokedexRating_Text_LessThan80[];
extern const u8 PokedexRating_Text_LessThan90[];
extern const u8 PokedexRating_Text_LessThan100[];
extern const u8 PokedexRating_Text_LessThan110[];
extern const u8 PokedexRating_Text_LessThan120[];
extern const u8 PokedexRating_Text_LessThan130[];
extern const u8 PokedexRating_Text_LessThan140[];
extern const u8 PokedexRating_Text_LessThan150[];
extern const u8 PokedexRating_Text_Complete[];
extern const u8 PokedexRating_Text_LessThan10Spa[];
extern const u8 PokedexRating_Text_LessThan20Spa[];
extern const u8 PokedexRating_Text_LessThan30Spa[];
extern const u8 PokedexRating_Text_LessThan40Spa[];
extern const u8 PokedexRating_Text_LessThan50Spa[];
extern const u8 PokedexRating_Text_LessThan60Spa[];
extern const u8 PokedexRating_Text_LessThan70Spa[];
extern const u8 PokedexRating_Text_LessThan80Spa[];
extern const u8 PokedexRating_Text_LessThan90Spa[];
extern const u8 PokedexRating_Text_LessThan100Spa[];
extern const u8 PokedexRating_Text_LessThan110Spa[];
extern const u8 PokedexRating_Text_LessThan120Spa[];
extern const u8 PokedexRating_Text_LessThan130Spa[];
extern const u8 PokedexRating_Text_LessThan140Spa[];
extern const u8 PokedexRating_Text_LessThan150Spa[];
extern const u8 PokedexRating_Text_CompleteSpa[];

u16 GetPokedexCount(void)
{
    if (gSpecialVar_0x8004 == 0)
    {
        gSpecialVar_0x8005 = GetKantoPokedexCount(0);
        gSpecialVar_0x8006 = GetKantoPokedexCount(1);
    }
    else
    {
        gSpecialVar_0x8005 = GetNationalPokedexCount(0);
        gSpecialVar_0x8006 = GetNationalPokedexCount(1);
    }
    return IsNationalPokedexEnabled();
}

static const u8 *GetProfOaksRatingMessageByCount(u16 count)
{
    gSpecialVar_Result = FALSE;

    if (count < 10)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan10;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan10Spa;
	}

    if (count < 20)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan20;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan20Spa;
	}

    if (count < 30)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan30;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan30Spa;
	}

    if (count < 40)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan40;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan40Spa;
	}

    if (count < 50)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan50;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan50Spa;
	}

    if (count < 60)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan60;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan60Spa;
	}

    if (count < 70)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan70;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan70Spa;
	}

    if (count < 80)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan80;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan80Spa;
	}

    if (count < 90)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan90;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan90Spa;
	}

    if (count < 100)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan100;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan100Spa;
	}

    if (count < 110)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan110;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan110Spa;
	}

    if (count < 120)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan120;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan120Spa;
	}

    if (count < 130)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan130;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan130Spa;
	}

    if (count < 140)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan140;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan140Spa;
	}

    if (count < 150)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_LessThan150;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_LessThan150Spa;
	}

    if (count == 150)
    {
        // Mew doesn't count for completing the pokedex
        if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(SPECIES_MEW), 1))
		{
			if (gSaveBlock2Ptr->optionsLanguage == ENG)
				return PokedexRating_Text_LessThan150;
			if (gSaveBlock2Ptr->optionsLanguage == SPA)
				return PokedexRating_Text_LessThan150Spa;
		}

        gSpecialVar_Result = TRUE;
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_Complete;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_CompleteSpa;
    }

    if (count == 151)
    {
        gSpecialVar_Result = TRUE;
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			return PokedexRating_Text_Complete;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			return PokedexRating_Text_CompleteSpa;
    }

    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		return PokedexRating_Text_LessThan10;
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		return PokedexRating_Text_LessThan10Spa;
}

void GetProfOaksRatingMessage(void)
{
    ShowFieldMessage(GetProfOaksRatingMessageByCount(gSpecialVar_0x8004));
}
