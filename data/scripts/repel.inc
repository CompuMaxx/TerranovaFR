EventScript_RepelWoreOff:: @ 82A4B2A
	checkitem VAR_REPEL_LAST_USED, 1
	compare VAR_RESULT, FALSE
	goto_if_eq EventScript_RepelWoreOff_NoMoreRepels
	goto AskForAnotherRepel
EventScript_RepelWoreOff_Result:
	compare VAR_RESULT, 0
	goto_if_eq EventScript_RepelWoreOff_ChooseNo
	copyvar VAR_0x8004, VAR_REPEL_LAST_USED
	callnative ItemId_GetHoldEffectParam_Script
	copyvar VAR_REPEL_STEP_COUNT, VAR_RESULT
	bufferitemname 1, VAR_REPEL_LAST_USED
	removeitem VAR_REPEL_LAST_USED, 1
	playse SE_REPEL
	checklanguage
	compare VAR_RESULT, 0
	goto_if_eq PrinText_RepelWoreOff_PlayerUsedVar2
	goto PrinText_RepelWoreOff_PlayerUsedVar2Spa
EventScript_RepelWoreOff_ChooseNo:
	goto EndScript
EventScript_RepelWoreOff_NoMoreRepels:
	checklanguage
	compare VAR_RESULT, 0
	goto_if_eq PrinText_RepelWoreOff
	goto PrinText_RepelWoreOffSpa
EndScript:
	closemessage
	release
	end

AskForAnotherRepel:
	checklanguage
	compare VAR_RESULT, 0
	goto_if_eq PrinText_RepelWoreOff_UseAnother
	goto PrinText_RepelWoreOff_UseAnotherSpa
	
PrinText_RepelWoreOff:
	msgbox Text_RepelWoreOff, MSGBOX_SIGN
	goto EndScript
PrinText_RepelWoreOffSpa:
	msgbox Text_RepelWoreOffSpa, MSGBOX_SIGN
	goto EndScript
PrinText_RepelWoreOff_UseAnother:
	msgbox Text_RepelWoreOff_UseAnother, MSGBOX_YESNO
	goto EventScript_RepelWoreOff_Result
PrinText_RepelWoreOff_UseAnotherSpa:
	msgbox Text_RepelWoreOff_UseAnotherSpa, MSGBOX_YESNO
	goto EventScript_RepelWoreOff_Result
PrinText_RepelWoreOff_PlayerUsedVar2:
	msgbox gText_PlayerUsedVar2, MSGBOX_SIGN
	goto EndScript
PrinText_RepelWoreOff_PlayerUsedVar2Spa:
	msgbox gText_PlayerUsedVar2Spa, MSGBOX_SIGN
	goto EndScript


Text_RepelWoreOff: @ 82A4B33
	.string "REPEL's effect wore off…$"
Text_RepelWoreOff_UseAnother:
	.string "REPEL's effect wore off…\nUse another?$"
Text_RepelWoreOffSpa: @ 82A4B33
	.string "El Repelente ha dejado de hacer\nefecto.$"
Text_RepelWoreOff_UseAnotherSpa:
	.string "El Repelente ha dejado de hacer\nefecto. ¿Usar otro?$"

EventScript_PoisonSurvial::
	lock
	checklanguage
	compare VAR_RESULT, 1
	goto_if_eq EventScript_PoisonSurvial_Spa
	msgbox gText_PoisonSurvial
	goto EndScript

EventScript_PoisonSurvial_Spa:
	msgbox gText_PoisonSurvialSpa
	goto EndScript
