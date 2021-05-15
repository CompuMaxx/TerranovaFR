#include "global.h"
#include "gflib.h"
#include "battle_string_ids.h"
#include "battle.h"
#include "battle_anim.h"
#include "strings.h"
#include "battle_message.h"
#include "link.h"
#include "event_scripts.h"
#include "event_data.h"
#include "item.h"
#include "battle_tower.h"
#include "trainer_tower.h"
#include "battle_setup.h"
#include "field_specials.h"
#include "new_menu_helpers.h"
#include "battle_controllers.h"
#include "graphics.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/trainer_classes.h"

struct BattleWindowText
{
    u8 fillValue;
    u8 fontId;
    u8 x;
    u8 y;
    u8 letterSpacing;
    u8 lineSpacing;
    u8 speed;
    u8 fgColor;
    u8 bgColor;
    u8 shadowColor;
};

static EWRAM_DATA u8 sBattlerAbilities[MAX_BATTLERS_COUNT] = {};
static EWRAM_DATA struct BattleMsgData *sBattleMsgDataPtr = NULL;

static void ChooseMoveUsedParticle(u8 *textPtr);
static void ChooseTypeOfMoveUsedString(u8 *textPtr);
static void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst);

static const u8 sText_Empty1[] = _("");
static const u8 sText_Trainer1LoseText[] = _("{B_TRAINER1_LOSE_TEXT}");
static const u8 sText_Trainer2Class[] = _("{B_TRAINER2_CLASS}");
static const u8 sText_Trainer1RecallPkmn1[] = _("{B_TRAINER1_NAME}: {B_OPPONENT_MON1_NAME}, come back!");
static const u8 sText_Trainer1WinText[] = _("{B_TRAINER1_WIN_TEXT}");
static const u8 sText_Trainer1RecallPkmn2[] = _("{B_TRAINER1_NAME}: {B_OPPONENT_MON2_NAME}, come back!");
static const u8 sText_Trainer1RecallBoth[] = _("{B_TRAINER1_NAME}: {B_OPPONENT_MON1_NAME} and\n{B_OPPONENT_MON2_NAME}, come back!");
static const u8 sText_Trainer2Name[] = _("{B_TRAINER2_NAME}");
static const u8 sText_PkmnGainedEXP[] = _("{B_BUFF1} gained{B_BUFF2}\n{B_TRAINER2_LOSE_TEXT} Exp. Points!\p");
static const u8 sText_EmptyString4[] = _("");
static const u8 sText_ABoosted[] = _(" a boosted");
static const u8 sText_PkmnGrewToLv[] = _("{B_BUFF1} grew to\nLV. {B_BUFF2}!{WAIT_SE}\p");
static const u8 sText_PkmnLearnedMove[] = _("{B_BUFF1} learned\n{B_BUFF2}!{WAIT_SE}\p");
static const u8 sText_TryToLearnMove1[] = _("{B_BUFF1} is trying to\nlearn {B_BUFF2}.\p");
static const u8 sText_TryToLearnMove2[] = _("But, {B_BUFF1} can't learn\nmore than four moves.\p");
static const u8 sText_TryToLearnMove3[] = _("Delete a move to make\nroom for {B_BUFF2}?");
static const u8 sText_PkmnForgotMove[] = _("{B_BUFF1} forgot\n{B_BUFF2}.\p");
static const u8 sText_StopLearningMove[] = _("{PAUSE 32}Stop learning\n{B_BUFF2}?");
static const u8 sText_DidNotLearnMove[] = _("{B_BUFF1} did not learn\n{B_BUFF2}.\p");
static const u8 sText_UseNextPkmn[] = _("Use next Pokémon?");
static const u8 sText_AttackMissed[] = _("{B_ATK_NAME_WITH_PREFIX}'s\nattack missed!");
static const u8 sText_PkmnProtectedItself[] = _("{B_DEF_NAME_WITH_PREFIX}\nprotected itself!");
static const u8 sText_AvoidedDamage[] = _("{B_DEF_NAME_WITH_PREFIX} avoided\ndamage with {B_DEF_ABILITY}!");
static const u8 sText_PkmnMakesGroundMiss[] = _("{B_DEF_NAME_WITH_PREFIX} makes Ground\nmoves miss with {B_DEF_ABILITY}!");
static const u8 sText_PkmnAvoidedAttack[] = _("{B_DEF_NAME_WITH_PREFIX} avoided\nthe attack!");
static const u8 sText_ItDoesntAffect[] = _("It doesn't affect\n{B_DEF_NAME_WITH_PREFIX}…");
static const u8 sText_AttackerFainted[] = _("{B_ATK_NAME_WITH_PREFIX}\nfainted!\p");
static const u8 sText_TargetFainted[] = _("{B_DEF_NAME_WITH_PREFIX}\nfainted!\p");
static const u8 sText_PlayerGotMoney[] = _("{B_PLAYER_NAME} got ¥{B_BUFF1}\nfor winning!\p");
static const u8 sText_PlayerWhiteout[] = _("{B_PLAYER_NAME} is out of\nusable Pokémon!\p");
static const u8 sText_PlayerPanicked[] = _("{B_PLAYER_NAME} panicked and lost ¥{B_BUFF1}…\p… … … …\p{B_PLAYER_NAME} whited out!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PlayerWhiteoutAgainstTrainer[] = _("{B_PLAYER_NAME} is out of\nusable Pokémon!\pPlayer lost against\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PlayerPaidAsPrizeMoney[] = _("{B_PLAYER_NAME} paid ¥{B_BUFF1} as the prize\nmoney…\p… … … …\p{B_PLAYER_NAME} whited out!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PlayerWhiteout2[] = _("{B_PLAYER_NAME} whited out!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PreventsEscape[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} prevents\nescape with {B_SCR_ACTIVE_ABILITY}!\p");
static const u8 sText_CantEscape2[] = _("Can't escape!\p");
static const u8 sText_AttackerCantEscape[] = _("{B_ATK_NAME_WITH_PREFIX} can't escape!");
static const u8 sText_HitXTimes[] = _("Hit {B_BUFF1} time(s)!");
static const u8 sText_PkmnFellAsleep[] = _("{B_EFF_NAME_WITH_PREFIX}\nfell asleep!");
static const u8 sText_PkmnMadeSleep[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nmade {B_EFF_NAME_WITH_PREFIX} sleep!");
static const u8 sText_PkmnAlreadyAsleep[] = _("{B_DEF_NAME_WITH_PREFIX} is\nalready asleep!");
static const u8 sText_PkmnAlreadyAsleep2[] = _("{B_ATK_NAME_WITH_PREFIX} is\nalready asleep!");
static const u8 sText_PkmnWasntAffected[] = _("{B_DEF_NAME_WITH_PREFIX}\nwasn't affected!");
static const u8 sText_PkmnWasPoisoned[] = _("{B_EFF_NAME_WITH_PREFIX}\nwas poisoned!");
static const u8 sText_PkmnPoisonedBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\npoisoned {B_EFF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnHurtByPoison[] = _("{B_ATK_NAME_WITH_PREFIX} is hurt\nby poison!");
static const u8 sText_PkmnAlreadyPoisoned[] = _("{B_DEF_NAME_WITH_PREFIX} is already\npoisoned.");
static const u8 sText_PkmnBadlyPoisoned[] = _("{B_EFF_NAME_WITH_PREFIX} is badly\npoisoned!");
static const u8 sText_PkmnEnergyDrained[] = _("{B_DEF_NAME_WITH_PREFIX} had its\nenergy drained!");
static const u8 sText_PkmnWasBurned[] = _("{B_EFF_NAME_WITH_PREFIX} was burned!");
static const u8 sText_PkmnBurnedBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nburned {B_EFF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnHurtByBurn[] = _("{B_ATK_NAME_WITH_PREFIX} is hurt\nby its burn!");
static const u8 sText_PkmnAlreadyHasBurn[] = _("{B_DEF_NAME_WITH_PREFIX} already\nhas a burn.");
static const u8 sText_PkmnWasFrozen[] = _("{B_EFF_NAME_WITH_PREFIX} was\nfrozen solid!");
static const u8 sText_PkmnFrozenBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nfroze {B_EFF_NAME_WITH_PREFIX} solid!");
static const u8 sText_PkmnIsFrozen[] = _("{B_ATK_NAME_WITH_PREFIX} is\nfrozen solid!");
static const u8 sText_PkmnWasDefrosted[] = _("{B_DEF_NAME_WITH_PREFIX} was\ndefrosted!");
static const u8 sText_PkmnWasDefrosted2[] = _("{B_ATK_NAME_WITH_PREFIX} was\ndefrosted!");
static const u8 sText_PkmnWasDefrostedBy[] = _("{B_ATK_NAME_WITH_PREFIX} was\ndefrosted by {B_CURRENT_MOVE}!");
static const u8 sText_PkmnWasParalyzed[] = _("{B_EFF_NAME_WITH_PREFIX} is paralyzed!\nIt may be unable to move!");
static const u8 sText_PkmnWasParalyzedBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nparalyzed {B_EFF_NAME_WITH_PREFIX}!\lIt may be unable to move!");
static const u8 sText_PkmnIsParalyzed[] = _("{B_ATK_NAME_WITH_PREFIX} is paralyzed!\nIt can't move!");
static const u8 sText_PkmnIsAlreadyParalyzed[] = _("{B_DEF_NAME_WITH_PREFIX} is\nalready paralyzed!");
static const u8 sText_PkmnHealedParalysis[] = _("{B_DEF_NAME_WITH_PREFIX} was\nhealed of paralysis!");
static const u8 sText_PkmnDreamEaten[] = _("{B_DEF_NAME_WITH_PREFIX}'s\ndream was eaten!");
static const u8 sText_StatsWontIncrease[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1}\nwon't go higher!");
static const u8 sText_StatsWontDecrease[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}\nwon't go lower!");
static const u8 sText_TeamStoppedWorking[] = _("Your team's {B_BUFF1}\nstopped working!");
static const u8 sText_FoeStoppedWorking[] = _("The foe's {B_BUFF1}\nstopped working!");
static const u8 sText_PkmnIsConfused[] = _("{B_ATK_NAME_WITH_PREFIX} is\nconfused!");
static const u8 sText_PkmnHealedConfusion[] = _("{B_ATK_NAME_WITH_PREFIX} snapped\nout of confusion!");
static const u8 sText_PkmnWasConfused[] = _("{B_EFF_NAME_WITH_PREFIX} became\nconfused!");
static const u8 sText_PkmnAlreadyConfused[] = _("{B_DEF_NAME_WITH_PREFIX} is\nalready confused!");
static const u8 sText_PkmnFellInLove[] = _("{B_DEF_NAME_WITH_PREFIX}\nfell in love!");
static const u8 sText_PkmnInLove[] = _("{B_ATK_NAME_WITH_PREFIX} is in love\nwith {B_SCR_ACTIVE_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnImmobilizedByLove[] = _("{B_ATK_NAME_WITH_PREFIX} is\nimmobilized by love!");
static const u8 sText_PkmnBlownAway[] = _("{B_DEF_NAME_WITH_PREFIX} was\nblown away!");
static const u8 sText_PkmnChangedType[] = _("{B_ATK_NAME_WITH_PREFIX} transformed\ninto the {B_BUFF1} type!");
static const u8 sText_PkmnFlinched[] = _("{B_ATK_NAME_WITH_PREFIX} flinched!");
static const u8 sText_PkmnRegainedHealth[] = _("{B_DEF_NAME_WITH_PREFIX} regained\nhealth!");
static const u8 sText_PkmnHPFull[] = _("{B_DEF_NAME_WITH_PREFIX}'s\nHP is full!");
static const u8 sText_PkmnRaisedSpDef[] = _("{B_ATK_PREFIX2}'s {B_CURRENT_MOVE}\nraised Sp. Def!");
static const u8 sText_PkmnRaisedSpDefALittle[] = _("{B_ATK_PREFIX2}'s {B_CURRENT_MOVE}\nraised Sp. Def a little!");
static const u8 sText_PkmnRaisedDef[] = _("{B_ATK_PREFIX2}'s {B_CURRENT_MOVE}\nraised Defense!");
static const u8 sText_PkmnRaisedDefALittle[] = _("{B_ATK_PREFIX2}'s {B_CURRENT_MOVE}\nraised Defense a little!");
static const u8 sText_PkmnCoveredByVeil[] = _("{B_ATK_PREFIX2}'s party is covered\nby a veil!");
static const u8 sText_PkmnUsedSafeguard[] = _("{B_DEF_NAME_WITH_PREFIX}'s party is protected\nby Safeguard!");
static const u8 sText_PkmnSafeguardExpired[] = _("{B_ATK_PREFIX3}'s party is no longer\nprotected by Safeguard!");
static const u8 sText_PkmnWentToSleep[] = _("{B_ATK_NAME_WITH_PREFIX} went\nto sleep!");
static const u8 sText_PkmnSleptHealthy[] = _("{B_ATK_NAME_WITH_PREFIX} slept and\nbecame healthy!");
static const u8 sText_PkmnWhippedWhirlwind[] = _("{B_ATK_NAME_WITH_PREFIX} whipped\nup a whirlwind!");
static const u8 sText_PkmnTookSunlight[] = _("{B_ATK_NAME_WITH_PREFIX} took\nin sunlight!");
static const u8 sText_PkmnLoweredHead[] = _("{B_ATK_NAME_WITH_PREFIX} lowered\nits head!");
static const u8 sText_PkmnIsGlowing[] = _("{B_ATK_NAME_WITH_PREFIX} is glowing!");
static const u8 sText_PkmnFlewHigh[] = _("{B_ATK_NAME_WITH_PREFIX} flew\nup high!");
static const u8 sText_PkmnDugHole[] = _("{B_ATK_NAME_WITH_PREFIX} dug a hole!");
static const u8 sText_PkmnHidUnderwater[] = _("{B_ATK_NAME_WITH_PREFIX} hid\nunderwater!");
static const u8 sText_PkmnSprangUp[] = _("{B_ATK_NAME_WITH_PREFIX} sprang up!");
static const u8 sText_PkmnSqueezedByBind[] = _("{B_DEF_NAME_WITH_PREFIX} was squeezed by\n{B_ATK_NAME_WITH_PREFIX}'s Bind!");
static const u8 sText_PkmnTrappedInVortex[] = _("{B_DEF_NAME_WITH_PREFIX} was trapped\nin the vortex!");
static const u8 sText_PkmnTrappedBySandTomb[] = _("{B_DEF_NAME_WITH_PREFIX} was trapped\nby Sand Tomb!");
static const u8 sText_PkmnWrappedBy[] = _("{B_DEF_NAME_WITH_PREFIX} was Wrapped by\n{B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnClamped[] = _("{B_ATK_NAME_WITH_PREFIX} Clamped\n{B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnHurtBy[] = _("{B_ATK_NAME_WITH_PREFIX} is hurt\nby {B_BUFF1}!");
static const u8 sText_PkmnFreedFrom[] = _("{B_ATK_NAME_WITH_PREFIX} was freed\nfrom {B_BUFF1}!");
static const u8 sText_PkmnCrashed[] = _("{B_ATK_NAME_WITH_PREFIX} kept going\nand crashed!");
const u8 gBattleText_MistShroud[] = _("{B_ATK_PREFIX2} became\nshrouded in Mist!");
static const u8 sText_PkmnProtectedByMist[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is protected\nby Mist!");
const u8 gBattleText_GetPumped[] = _("{B_ATK_NAME_WITH_PREFIX} is getting\npumped!");
static const u8 sText_PkmnHitWithRecoil[] = _("{B_ATK_NAME_WITH_PREFIX} is hit\nwith recoil!");
static const u8 sText_PkmnProtectedItself2[] = _("{B_ATK_NAME_WITH_PREFIX} protected\nitself!");
static const u8 sText_PkmnBuffetedBySandstorm[] = _("{B_ATK_NAME_WITH_PREFIX} is buffeted\nby the sandstorm!");
static const u8 sText_PkmnPeltedByHail[] = _("{B_ATK_NAME_WITH_PREFIX} is pelted\nby Hail!");
static const u8 sText_PkmnsXWoreOff[] = _("{B_ATK_PREFIX1}'s {B_BUFF1}\nwore off!");
static const u8 sText_PkmnSeeded[] = _("{B_DEF_NAME_WITH_PREFIX} was seeded!");
static const u8 sText_PkmnEvadedAttack[] = _("{B_DEF_NAME_WITH_PREFIX} evaded\nthe attack!");
static const u8 sText_PkmnSappedByLeechSeed[] = _("{B_ATK_NAME_WITH_PREFIX}'s health is\nsapped by Leech Seed!");
static const u8 sText_PkmnFastAsleep[] = _("{B_ATK_NAME_WITH_PREFIX} is fast\nasleep.");
static const u8 sText_PkmnWokeUp[] = _("{B_ATK_NAME_WITH_PREFIX} woke up!");
static const u8 sText_PkmnUproarKeptAwake[] = _("But {B_SCR_ACTIVE_NAME_WITH_PREFIX}'s Uproar\nkept it awake!");
static const u8 sText_PkmnWokeUpInUproar[] = _("{B_ATK_NAME_WITH_PREFIX} woke up\nin the Uproar!");
static const u8 sText_PkmnCausedUproar[] = _("{B_ATK_NAME_WITH_PREFIX} caused\nan Uproar!");
static const u8 sText_PkmnMakingUproar[] = _("{B_ATK_NAME_WITH_PREFIX} is making\nan Uproar!");
static const u8 sText_PkmnCalmedDown[] = _("{B_ATK_NAME_WITH_PREFIX} calmed down.");
static const u8 sText_PkmnCantSleepInUproar[] = _("But {B_DEF_NAME_WITH_PREFIX} can't\nsleep in an Uproar!");
static const u8 sText_PkmnStockpiled[] = _("{B_ATK_NAME_WITH_PREFIX} StockpileD\n{B_BUFF1}!");
static const u8 sText_PkmnCantStockpile[] = _("{B_ATK_NAME_WITH_PREFIX} can't\nStockpile any more!");
static const u8 sText_PkmnCantSleepInUproar2[] = _("But {B_DEF_NAME_WITH_PREFIX} can't\nsleep in an Uproar!");
static const u8 sText_UproarKeptPkmnAwake[] = _("But the Uproar kept\n{B_DEF_NAME_WITH_PREFIX} awake!");
static const u8 sText_PkmnStayedAwakeUsing[] = _("{B_DEF_NAME_WITH_PREFIX} stayed awake\nusing its {B_DEF_ABILITY}!");
static const u8 sText_PkmnStoringEnergy[] = _("{B_ATK_NAME_WITH_PREFIX} is storing\nenergy!");
static const u8 sText_PkmnUnleashedEnergy[] = _("{B_ATK_NAME_WITH_PREFIX} unleashed\nenergy!");
static const u8 sText_PkmnFatigueConfusion[] = _("{B_ATK_NAME_WITH_PREFIX} became\nconfused due to fatigue!");
static const u8 sText_PkmnPickedUpItem[] = _("{B_PLAYER_NAME} picked up\n¥{B_BUFF1}!\p");
static const u8 sText_PkmnUnaffected[] = _("{B_DEF_NAME_WITH_PREFIX} is\nunaffected!");
static const u8 sText_PkmnTransformedInto[] = _("{B_ATK_NAME_WITH_PREFIX} transformed\ninto {B_BUFF1}!");
static const u8 sText_PkmnMadeSubstitute[] = _("{B_ATK_NAME_WITH_PREFIX} made\na Substitute!");
static const u8 sText_PkmnHasSubstitute[] = _("{B_ATK_NAME_WITH_PREFIX} already\nhas a Substitute!");
static const u8 sText_SubstituteDamaged[] = _("The Substitute took damage\nfor {B_DEF_NAME_WITH_PREFIX}!\p");
static const u8 sText_PkmnSubstituteFaded[] = _("{B_DEF_NAME_WITH_PREFIX}'s\nSubstitute faded!\p");
static const u8 sText_PkmnMustRecharge[] = _("{B_ATK_NAME_WITH_PREFIX} must\nrecharge!");
static const u8 sText_PkmnRageBuilding[] = _("{B_DEF_NAME_WITH_PREFIX}'s Rage\nis building!");
static const u8 sText_PkmnMoveWasDisabled[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}\nwas disabled!");
static const u8 sText_PkmnMoveDisabledNoMore[] = _("{B_ATK_NAME_WITH_PREFIX} is disabled\nno more!");
static const u8 sText_PkmnGotEncore[] = _("{B_DEF_NAME_WITH_PREFIX} got\nan Encore!");
static const u8 sText_PkmnEncoreEnded[] = _("{B_ATK_NAME_WITH_PREFIX}'s Encore\nended!");
static const u8 sText_PkmnTookAim[] = _("{B_ATK_NAME_WITH_PREFIX} took aim\nat {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnSketchedMove[] = _("{B_ATK_NAME_WITH_PREFIX} Sketched\n{B_BUFF1}!");
static const u8 sText_PkmnTryingToTakeFoe[] = _("{B_ATK_NAME_WITH_PREFIX} is trying\nto take its foe with it!");
static const u8 sText_PkmnTookFoe[] = _("{B_DEF_NAME_WITH_PREFIX} took\n{B_ATK_NAME_WITH_PREFIX} with it!");
static const u8 sText_PkmnReducedPP[] = _("Reduced {B_DEF_NAME_WITH_PREFIX}'s\n{B_BUFF1} by {B_BUFF2}!");
static const u8 sText_PkmnStoleItem[] = _("{B_ATK_NAME_WITH_PREFIX} stole\n{B_DEF_NAME_WITH_PREFIX}'s {B_LAST_ITEM}!");
static const u8 sText_TargetCantEscapeNow[] = _("{B_DEF_NAME_WITH_PREFIX} can't\nescape now!");
static const u8 sText_PkmnFellIntoNightmare[] = _("{B_DEF_NAME_WITH_PREFIX} fell into\na Nightmare!");
static const u8 sText_PkmnLockedInNightmare[] = _("{B_ATK_NAME_WITH_PREFIX} is locked\nin a Nightmare!");
static const u8 sText_PkmnLaidCurse[] = _("{B_ATK_NAME_WITH_PREFIX} cut its own HP and\nlaid a Curse on {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnAfflictedByCurse[] = _("{B_ATK_NAME_WITH_PREFIX} is afflicted\nby the Curse!");
static const u8 sText_SpikesScattered[] = _("Spikes were scattered all around\nthe opponent's side!");
static const u8 sText_PkmnHurtBySpikes[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is hurt\nby Spikes!");
static const u8 sText_PkmnIdentified[] = _("{B_ATK_NAME_WITH_PREFIX} identified\n{B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnPerishCountFell[] = _("{B_ATK_NAME_WITH_PREFIX}'s Perish count\nfell to {B_BUFF1}!");
static const u8 sText_PkmnBracedItself[] = _("{B_ATK_NAME_WITH_PREFIX} braced\nitself!");
static const u8 sText_PkmnEnduredHit[] = _("{B_DEF_NAME_WITH_PREFIX} Endured\nthe hit!");
static const u8 sText_MagnitudeStrength[] = _("MAGNITUDE {B_BUFF1}!");
static const u8 sText_PkmnCutHPMaxedAttack[] = _("{B_ATK_NAME_WITH_PREFIX} cut its own HP\nand maximized Attack!");
static const u8 sText_PkmnCopiedStatChanges[] = _("{B_ATK_NAME_WITH_PREFIX} copied\n{B_DEF_NAME_WITH_PREFIX}'s stat changes!");
static const u8 sText_PkmnGotFree[] = _("{B_ATK_NAME_WITH_PREFIX} got free of\n{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}!");
static const u8 sText_PkmnShedLeechSeed[] = _("{B_ATK_NAME_WITH_PREFIX} shed\nLeench Seed!");
static const u8 sText_PkmnBlewAwaySpikes[] = _("{B_ATK_NAME_WITH_PREFIX} blew away\nSpikes!");
static const u8 sText_PkmnFledFromBattle[] = _("{B_ATK_NAME_WITH_PREFIX} fled from\nbattle!");
static const u8 sText_PkmnForesawAttack[] = _("{B_ATK_NAME_WITH_PREFIX} foresaw\nan attack!");
static const u8 sText_PkmnTookAttack[] = _("{B_DEF_NAME_WITH_PREFIX} took the\n{B_BUFF1} attack!");
static const u8 sText_PkmnChoseXAsDestiny[] = _("{B_ATK_NAME_WITH_PREFIX} chose\n{B_CURRENT_MOVE} as its destiny!");
static const u8 sText_PkmnAttack[] = _("{B_BUFF1}'s attack!");
static const u8 sText_PkmnCenterAttention[] = _("{B_ATK_NAME_WITH_PREFIX} became the\ncenter of attention!");
static const u8 sText_PkmnChargingPower[] = _("{B_ATK_NAME_WITH_PREFIX} began\ncharging power!");
static const u8 sText_NaturePowerTurnedInto[] = _("NATURE POWER turned into\n{B_CURRENT_MOVE}!");
static const u8 sText_PkmnStatusNormal[] = _("{B_ATK_NAME_WITH_PREFIX}'s status\nreturned to normal!");
static const u8 sText_PkmnSubjectedToTorment[] = _("{B_DEF_NAME_WITH_PREFIX} was subjected\nto Torment!");
static const u8 sText_PkmnTighteningFocus[] = _("{B_ATK_NAME_WITH_PREFIX} is tightening\nits focus!");
static const u8 sText_PkmnFellForTaunt[] = _("{B_DEF_NAME_WITH_PREFIX} fell for\nthe Taunt!");
static const u8 sText_PkmnReadyToHelp[] = _("{B_ATK_NAME_WITH_PREFIX} is ready to\nhelp {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnSwitchedItems[] = _("{B_ATK_NAME_WITH_PREFIX} switched\nitems with its opponent!");
static const u8 sText_PkmnObtainedX[] = _("{B_ATK_NAME_WITH_PREFIX} obtained\n{B_BUFF1}.");
static const u8 sText_PkmnObtainedX2[] = _("{B_DEF_NAME_WITH_PREFIX} obtained\n{B_BUFF2}.");
static const u8 sText_PkmnObtainedXYObtainedZ[] = _("{B_ATK_NAME_WITH_PREFIX} obtained\n{B_BUFF1}.\p{B_DEF_NAME_WITH_PREFIX} obtained\n{B_BUFF2}.");
static const u8 sText_PkmnCopiedFoe[] = _("{B_ATK_NAME_WITH_PREFIX} copied\n{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}!");
static const u8 sText_PkmnMadeWish[] = _("{B_ATK_NAME_WITH_PREFIX} made a Wish!");
static const u8 sText_PkmnWishCameTrue[] = _("{B_BUFF1}'s Wish\ncame true!");
static const u8 sText_PkmnPlantedRoots[] = _("{B_ATK_NAME_WITH_PREFIX} planted its roots!");
static const u8 sText_PkmnAbsorbedNutrients[] = _("{B_ATK_NAME_WITH_PREFIX} absorbed\nnutrients with its roots!");
static const u8 sText_PkmnAnchoredItself[] = _("{B_DEF_NAME_WITH_PREFIX} anchored\nitself with its roots!");
static const u8 sText_PkmnWasMadeDrowsy[] = _("{B_ATK_NAME_WITH_PREFIX} made\n{B_DEF_NAME_WITH_PREFIX} drowsy!");
static const u8 sText_PkmnKnockedOff[] = _("{B_ATK_NAME_WITH_PREFIX} knocked off\n{B_DEF_NAME_WITH_PREFIX}'s {B_LAST_ITEM}!");
static const u8 sText_PkmnSwappedAbilities[] = _("{B_ATK_NAME_WITH_PREFIX} swapped abilities\nwith its opponent!");
static const u8 sText_PkmnSealedOpponentMove[] = _("{B_ATK_NAME_WITH_PREFIX} sealed the\nopponent's moveシsス!");
static const u8 sText_PkmnWantsGrudge[] = _("{B_ATK_NAME_WITH_PREFIX} wants the\nopponent to bear a Grudge!");
static const u8 sText_PkmnLostPPGrudge[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1} lost\nall its PP due to the Grudge!");
static const u8 sText_PkmnShroudedItself[] = _("{B_ATK_NAME_WITH_PREFIX} shrouded\nitself in {B_CURRENT_MOVE}!");
static const u8 sText_PkmnMoveBounced[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE}\nwas bounced back by Magic Coat!");
static const u8 sText_PkmnWaitsForTarget[] = _("{B_ATK_NAME_WITH_PREFIX} waits for its foe\nto make a move!");
static const u8 sText_PkmnSnatchedMove[] = _("{B_DEF_NAME_WITH_PREFIX} Snatched\n{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s move!");
static const u8 sText_ElectricityWeakened[] = _("Electricity's power was\nweakened!");
static const u8 sText_FireWeakened[] = _("Fire's power was\nweakened!");
static const u8 sText_XFoundOneY[] = _("{B_ATK_NAME_WITH_PREFIX} found\none {B_LAST_ITEM}!");
static const u8 sText_SoothingAroma[] = _("A soothing aroma wafted\nthrough the area!");
static const u8 sText_ItemsCantBeUsedNow[] = _("Items can't be used now.{PAUSE 64}");
static const u8 sText_ForXCommaYZ[] = _("For {B_SCR_ACTIVE_NAME_WITH_PREFIX},\n{B_LAST_ITEM} {B_BUFF1}");
static const u8 sText_PkmnUsedXToGetPumped[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} used\n{B_LAST_ITEM} to hustle!");
static const u8 sText_PkmnLostFocus[] = _("{B_ATK_NAME_WITH_PREFIX} lost its\nfocus and couldn't move!");
static const u8 sText_PkmnWasDraggedOut[] = _("{B_DEF_NAME_WITH_PREFIX} was\ndragged out!\p");
static const u8 sText_TheWallShattered[] = _("The wall shattered!");
static const u8 sText_ButNoEffect[] = _("But it had no effect!");
static const u8 sText_PkmnHasNoMovesLeft[] = _("{B_ACTIVE_NAME_WITH_PREFIX} has no\nmoves left!\p");
static const u8 sText_PkmnMoveIsDisabled[] = _("{B_ACTIVE_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE}\nis disabled!\p");
static const u8 sText_PkmnCantUseMoveTorment[] = _("{B_ACTIVE_NAME_WITH_PREFIX} can't use the same\nmove in a row due to the Torment!\p");
static const u8 sText_PkmnCantUseMoveTaunt[] = _("{B_ACTIVE_NAME_WITH_PREFIX} can't use\n{B_CURRENT_MOVE} after the Taunt!\p");
static const u8 sText_PkmnCantUseMoveSealed[] = _("{B_ACTIVE_NAME_WITH_PREFIX} can't use the\nsealed {B_CURRENT_MOVE}!\p");
static const u8 sText_PkmnMadeItRain[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nmade it rain!");
static const u8 sText_PkmnRaisedSpeed[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nraised its Speed!");
static const u8 sText_PkmnProtectedBy[] = _("{B_DEF_NAME_WITH_PREFIX} was protected\nby {B_DEF_ABILITY}!");
static const u8 sText_PkmnPreventsUsage[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents {B_ATK_NAME_WITH_PREFIX}\lfrom using {B_CURRENT_MOVE}!");
static const u8 sText_PkmnRestoredHPUsing[] = _("{B_DEF_NAME_WITH_PREFIX} restored HP\nusing its {B_DEF_ABILITY}!");
static const u8 sText_PkmnsXMadeYUseless[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nmade {B_CURRENT_MOVE} useless!");
static const u8 sText_PkmnChangedTypeWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nmade it the {B_BUFF1} type!");
static const u8 sText_PkmnPreventsParalysisWith[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents paralysis!");
static const u8 sText_PkmnPreventsRomanceWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents romance!");
static const u8 sText_PkmnPreventsPoisoningWith[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents poisoning!");
static const u8 sText_PkmnPreventsConfusionWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents confusion!");
static const u8 sText_PkmnRaisedFirePowerWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nraised its FIRE power!");
static const u8 sText_PkmnAnchorsItselfWith[] = _("{B_DEF_NAME_WITH_PREFIX} anchors\nitself with {B_DEF_ABILITY}!");
static const u8 sText_PkmnCutsAttackWith[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\ncuts {B_DEF_NAME_WITH_PREFIX}'s Attack!");
static const u8 sText_PkmnPreventsStatLossWith[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nprevents stat loss!");
static const u8 sText_PkmnHurtsWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nhurt {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnTraced[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} TRACED\n{B_BUFF1}'s {B_BUFF2}!");
static const u8 sText_PkmnsXPreventsBurns[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_EFF_ABILITY}\nprevents burns!");
static const u8 sText_PkmnsXBlocksY[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nblocks {B_CURRENT_MOVE}!");
static const u8 sText_PkmnsXBlocksY2[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nblocks {B_CURRENT_MOVE}!");
static const u8 sText_PkmnsXRestoredHPALittle2[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\nrestored its HP a little!");
static const u8 sText_PkmnsXWhippedUpSandstorm[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nwhipped up a sandstorm!");
static const u8 sText_PkmnsXIntensifiedSun[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nintensified the sun's rays!");
static const u8 sText_PkmnsXPreventsYLoss[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nprevents {B_BUFF1} loss!");
static const u8 sText_PkmnsXInfatuatedY[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\ninfatuated {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnsXMadeYIneffective[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nmade {B_CURRENT_MOVE} ineffective!");
static const u8 sText_PkmnsXCuredYProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\ncured its {B_BUFF1} problem!");
static const u8 sText_ItSuckedLiquidOoze[] = _("It sucked up the\nLiquid Ooze!");
static const u8 sText_PkmnTransformed[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} transformed!");
static const u8 sText_PkmnsXTookAttack[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\ntook the attack!");
const u8 gText_PkmnsXPreventsSwitching[] = _("{B_BUFF1}'s {B_LAST_ABILITY}\nprevents switching!\p");
static const u8 sText_PreventedFromWorking[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevented {B_SCR_ACTIVE_NAME_WITH_PREFIX}'s\l{B_BUFF1} from working!");
static const u8 sText_PkmnsXMadeItIneffective[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nmade it ineffective!");
static const u8 sText_PkmnsXPreventsFlinching[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_EFF_ABILITY}\nprevents flinching!");
static const u8 sText_PkmnsXPreventsYsZ[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\nprevents {B_DEF_NAME_WITH_PREFIX}'s\l{B_DEF_ABILITY} from working!");
static const u8 sText_PkmnsXCuredItsYProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\ncured its {B_BUFF1} problem!");
static const u8 sText_PkmnsXHadNoEffectOnY[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nhad no effect on {B_EFF_NAME_WITH_PREFIX}!");
static const u8 sText_TooScaredToMove[] = _("{B_ATK_NAME_WITH_PREFIX} is too scared to move!");
static const u8 sText_GetOutGetOut[] = _("Ghost: Get out…… Get out……");
static const u8 sText_StatSharply[] = _("sharply ");
const u8 gBattleText_Rose[] = _("rose!");
static const u8 sText_StatHarshly[] = _("harshly ");
static const u8 sText_StatFell[] = _("fell!");
static const u8 sText_PkmnsStatChanged[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1}\n{B_BUFF2}");
const u8 gText_PkmnsStatChanged2[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}\n{B_BUFF2}");
static const u8 sText_UsingXTheYOfZN[] = _("Using {B_LAST_ITEM}, the {B_BUFF1}\nof {B_SCR_ACTIVE_NAME_WITH_PREFIX} {B_BUFF2}");
static const u8 sText_PkmnsStatChanged3[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1}\n{B_BUFF2}");
static const u8 sText_PkmnsStatChanged4[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}\n{B_BUFF2}");
static const u8 sText_StatsWontIncrease2[] = _("{B_ATK_NAME_WITH_PREFIX}'s stats won't\ngo any higher!");
static const u8 sText_StatsWontDecrease2[] = _("{B_DEF_NAME_WITH_PREFIX}'s stats won't\ngo any lower!");
static const u8 sText_CriticalHit[] = _("A critical hit!");
static const u8 sText_OneHitKO[] = _("It's a one-hit KO!");
static const u8 sText_123Poof[] = _("{PAUSE 32}1, {PAUSE 15}2, and{PAUSE 15}… {PAUSE 15}… {PAUSE 15}… {PAUSE 15}{PLAY_SE SE_BALL_BOUNCE_1}Poof!\p");
static const u8 sText_AndEllipsis[] = _("And…\p");
static const u8 sText_HMMovesCantBeForgotten[] = _("HM moves can't be\nforgotten now.\p");
static const u8 sText_NotVeryEffective[] = _("It's not very effective…");
static const u8 sText_SuperEffective[] = _("It's super effective!");
static const u8 sText_GotAwaySafely[] = _("{PLAY_SE SE_FLEE}Got away safely!\p");
static const u8 sText_PkmnFledUsingIts[] = _("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX} fled\nusing its {B_LAST_ITEM}!\p");
static const u8 sText_PkmnFledUsing[] = _("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX} fled\nusing {B_ATK_ABILITY}!\p");
static const u8 sText_WildPkmnFled[] = _("{PLAY_SE SE_FLEE}Wild {B_BUFF1} fled!");
static const u8 sText_PlayerDefeatedLinkTrainer[] = _("Player defeated\n{B_LINK_OPPONENT1_NAME}!");
static const u8 sText_TwoLinkTrainersDefeated[] = _("Player beat {B_LINK_OPPONENT1_NAME}\nand {B_LINK_OPPONENT2_NAME}!");
static const u8 sText_PlayerLostAgainstLinkTrainer[] = _("Player lost against\n{B_LINK_OPPONENT1_NAME}!");
static const u8 sText_PlayerLostToTwo[] = _("Player lost to {B_LINK_OPPONENT1_NAME}\nand {B_LINK_OPPONENT2_NAME}!");
static const u8 sText_PlayerBattledToDrawLinkTrainer[] = _("Player battled to a draw against\n{B_LINK_OPPONENT1_NAME}!");
static const u8 sText_PlayerBattledToDrawVsTwo[] = _("Player battled to a draw against\n{B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!");
static const u8 sText_WildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} fled!");
static const u8 sText_TwoWildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} and\n{B_LINK_OPPONENT2_NAME} fled!");
static const u8 sText_NoRunningFromTrainers[] = _("No! There's no running\nfrom a Trainer battle!\p");
static const u8 sText_CantEscape[] = _("Can't escape!\p");
static const u8 sText_EmptyString5[] = _("");   //sText_DontLeaveBirch in pokeem
static const u8 sText_ButNothingHappened[] = _("But nothing happened!");
static const u8 sText_ButItFailed[] = _("But it failed!");
static const u8 sText_ItHurtConfusion[] = _("It hurt itself in its\nconfusion!");
static const u8 sText_MirrorMoveFailed[] = _("The Mirror Move failed!");
static const u8 sText_StartedToRain[] = _("It started to rain!");
static const u8 sText_DownpourStarted[] = _("A downpour started!");
static const u8 sText_RainContinues[] = _("Rain continues to fall.");
static const u8 sText_DownpourContinues[] = _("The downpour continues.");
static const u8 sText_RainStopped[] = _("The rain stopped.");
static const u8 sText_SandstormBrewed[] = _("A sandstorm brewed!");
static const u8 sText_SandstormRages[] = _("The sandstorm rages.");
static const u8 sText_SandstormSubsided[] = _("The sandstorm subsided.");
static const u8 sText_SunlightGotBright[] = _("The sunlight got bright!");
static const u8 sText_SunlightStrong[] = _("The sunlight is strong.");
static const u8 sText_SunlightFaded[] = _("The sunlight faded.");
static const u8 sText_StartedHail[] = _("It started to Hail!");
static const u8 sText_HailContinues[] = _("Hail continues to fall.");
static const u8 sText_HailStopped[] = _("The Hail stopped.");
static const u8 sText_FailedToSpitUp[] = _("But it failed to Split Up\na thing!");
static const u8 sText_FailedToSwallow[] = _("But it failed to Swallow\na thing!");
static const u8 sText_WindBecameHeatWave[] = _("The wind turned into a\nHeat Wave!");
static const u8 sText_StatChangesGone[] = _("All stat changes were\neliminated!");
static const u8 sText_CoinsScattered[] = _("Coins scattered everywhere!");
static const u8 sText_TooWeakForSubstitute[] = _("It was too weak to make\na Substitute!");
static const u8 sText_SharedPain[] = _("The battlers shared\ntheir pain!");
static const u8 sText_BellChimed[] = _("A bell chimed!");
static const u8 sText_FaintInThree[] = _("All affected Pokémon will\nfaint in three turns!");
static const u8 sText_NoPPLeft[] = _("There's no PP left for\nthis move!\p");
static const u8 sText_ButNoPPLeft[] = _("But there was no PP left\nfor the move!");
static const u8 sText_PkmnIgnoresAsleep[] = _("{B_ATK_NAME_WITH_PREFIX} ignored\norders while asleep!");
static const u8 sText_PkmnIgnoredOrders[] = _("{B_ATK_NAME_WITH_PREFIX} ignored\norders!");
static const u8 sText_PkmnBeganToNap[] = _("{B_ATK_NAME_WITH_PREFIX} began to nap!");
static const u8 sText_PkmnLoafing[] = _("{B_ATK_NAME_WITH_PREFIX} is\nloafing around!");
static const u8 sText_PkmnWontObey[] = _("{B_ATK_NAME_WITH_PREFIX} won't\nobey!");
static const u8 sText_PkmnTurnedAway[] = _("{B_ATK_NAME_WITH_PREFIX} turned away!");
static const u8 sText_PkmnPretendNotNotice[] = _("{B_ATK_NAME_WITH_PREFIX} pretended\nnot to notice!");
static const u8 sText_EnemyAboutToSwitchPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} is\nabout to use {B_BUFF2}.\pWill {B_PLAYER_NAME} change\nPokémon?");
static const u8 sText_PkmnLearnedMove2[] = _("{B_ATK_NAME_WITH_PREFIX} learned\n{B_BUFF1}!");
static const u8 sText_PlayerDefeatedLinkTrainerTrainer1[] = _("Player defeated\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!\p");
static const u8 sText_ThrewARock[] = _("{B_PLAYER_NAME} threw a Rock\nat the {B_OPPONENT_MON1_NAME}!");
static const u8 sText_ThrewSomeBait[] = _("{B_PLAYER_NAME} threw some Bait\nat the {B_OPPONENT_MON1_NAME}!");
static const u8 sText_PkmnWatchingCarefully[] = _("{B_OPPONENT_MON1_NAME} is watching\ncarefully!");
static const u8 sText_PkmnIsAngry[] = _("{B_OPPONENT_MON1_NAME} is angry!");
static const u8 sText_PkmnIsEating[] = _("{B_OPPONENT_MON1_NAME} is eating!");
static const u8 sText_OutOfSafariBalls[] = _("{PLAY_SE SE_DING_DONG}Announcer: You're out of\nSafari Balls! Game over!\p");
static const u8 sText_WildPkmnAppeared[] = _("Wild {B_OPPONENT_MON1_NAME} appeared!\p");
static const u8 sText_WildPkmnAppeared2[] = _("Wild {B_OPPONENT_MON1_NAME} appeared!\p");
static const u8 sText_WildPkmnAppearedPause[] = _("Wild {B_OPPONENT_MON1_NAME} appeared!{PAUSE 127}");
static const u8 sText_TwoWildPkmnAppeared[] = _("Wild {B_OPPONENT_MON1_NAME} and\n{B_OPPONENT_MON2_NAME} appeared!\p");
static const u8 sText_GhostAppearedCantId[] = _("The Ghost appeared!\pDarn!\nThe Ghost can't be ID'd!\p");
static const u8 sText_TheGhostAppeared[] = _("The Ghost appeared!\p");
static const u8 sText_SilphScopeUnveil[] = _("Silph Scope unveiled the Ghost's\nidentity!");
static const u8 sText_TheGhostWas[] = _("The Ghost was Marowak!\p\n");
static const u8 sText_Trainer1WantsToBattle[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}\nwould like to battle!\p");
static const u8 sText_LinkTrainerWantsToBattle[] = _("{B_LINK_OPPONENT1_NAME}\nwants to battle!");
static const u8 sText_TwoLinkTrainersWantToBattle[] = _("{B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}\nwant to battle!");
static const u8 sText_Trainer1SentOutPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} sent\nout {B_OPPONENT_MON1_NAME}!{PAUSE 60}");
static const u8 sText_Trainer1SentOutTwoPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} sent\nout {B_OPPONENT_MON1_NAME} and {B_OPPONENT_MON2_NAME}!{PAUSE 60}");
static const u8 sText_Trainer1SentOutPkmn2[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} sent\nout {B_BUFF1}!");
static const u8 sText_LinkTrainerSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out\n{B_OPPONENT_MON1_NAME}!");
static const u8 sText_LinkTrainerSentOutTwoPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out\n{B_OPPONENT_MON1_NAME} and {B_OPPONENT_MON2_NAME}!");
static const u8 sText_TwoLinkTrainersSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out {B_LINK_OPPONENT_MON1_NAME}!\n{B_LINK_OPPONENT2_NAME} sent out {B_LINK_OPPONENT_MON2_NAME}!");
static const u8 sText_LinkTrainerSentOutPkmn2[] = _("{B_LINK_OPPONENT1_NAME} sent out\n{B_BUFF1}!");
static const u8 sText_LinkTrainerMultiSentOutPkmn[] = _("{B_LINK_SCR_TRAINER_NAME} sent out\n{B_BUFF1}!");
static const u8 sText_GoPkmn[] = _("Go! {B_PLAYER_MON1_NAME}!");
static const u8 sText_GoTwoPkmn[] = _("Go! {B_PLAYER_MON1_NAME} and\n{B_PLAYER_MON2_NAME}!");
static const u8 sText_GoPkmn2[] = _("Go! {B_BUFF1}!");
static const u8 sText_DoItPkmn[] = _("Do it! {B_BUFF1}!");
static const u8 sText_GoForItPkmn[] = _("Go for it, {B_BUFF1}!");
static const u8 sText_YourFoesWeakGetEmPkmn[] = _("Your foe's weak!\nGet 'em, {B_BUFF1}!");
static const u8 sText_LinkPartnerSentOutPkmnGoPkmn[] = _("{B_LINK_PARTNER_NAME} sent out {B_LINK_PLAYER_MON2_NAME}!\nGo! {B_LINK_PLAYER_MON1_NAME}!");
static const u8 sText_PkmnThatsEnough[] = _("{B_BUFF1}, that's enough!\nCome back!");
static const u8 sText_PkmnComeBack[] = _("{B_BUFF1}, come back!");
static const u8 sText_PkmnOkComeBack[] = _("{B_BUFF1}, OK!\nCome back!");
static const u8 sText_PkmnGoodComeBack[] = _("{B_BUFF1}, good!\nCome back!");
static const u8 sText_Trainer1WithdrewPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}\nwithdrew {B_BUFF1}!");
static const u8 sText_LinkTrainer1WithdrewPkmn[] = _("{B_LINK_OPPONENT1_NAME} withdrew\n{B_BUFF1}!");
static const u8 sText_LinkTrainer2WithdrewPkmn[] = _("{B_LINK_SCR_TRAINER_NAME} withdrew\n{B_BUFF1}!");
static const u8 sText_WildPkmnPrefix[] = _("Wild ");
static const u8 sText_FoePkmnPrefix[] = _("Foe ");
static const u8 sText_FoePkmnPrefix2[] = _("Foe");
static const u8 sText_AllyPkmnPrefix[] = _("Ally");
static const u8 sText_FoePkmnPrefix3[] = _("Foe");
static const u8 sText_AllyPkmnPrefix2[] = _("Ally");
static const u8 sText_FoePkmnPrefix4[] = _("Foe");
static const u8 sText_AllyPkmnPrefix3[] = _("Ally");
static const u8 sText_AttackerUsedX[] = _("{B_ATK_NAME_WITH_PREFIX} used\n{B_BUFF2}");
static const u8 sText_ExclamationMark[] = _("!");
static const u8 sText_ExclamationMark2[] = _("!");
static const u8 sText_ExclamationMark3[] = _("!");
static const u8 sText_ExclamationMark4[] = _("!");
static const u8 sText_ExclamationMark5[] = _("!");

static const u8 sText_HP2[] = _("HP");
static const u8 sText_Attack2[] = _("Attack");
static const u8 sText_Defense2[] = _("Defense");
static const u8 sText_Speed[] = _("Speed");
static const u8 sText_SpAtk2[] = _("Sp. Atk");
static const u8 sText_SpDef2[] = _("Sp. Def");
static const u8 sText_Accuracy[] = _("Accuracy");
static const u8 sText_Evasiveness[] = _("Evasiveness");
//SPANISH
static const u8 sText_Empty1Spa[] = _("");
static const u8 sText_Trainer1LoseTextSpa[] = _("{B_TRAINER1_LOSE_TEXT}");
static const u8 sText_Trainer2ClassSpa[] = _("{B_TRAINER2_CLASS}");
static const u8 sText_Trainer1RecallPkmn1Spa[] = _("{B_TRAINER1_NAME}: ¡{B_OPPONENT_MON1_NAME}, ven aquí!");
static const u8 sText_Trainer1WinTextSpa[] = _("{B_TRAINER1_WIN_TEXT}");
static const u8 sText_Trainer1RecallPkmn2Spa[] = _("{B_TRAINER1_NAME}: ¡{B_OPPONENT_MON2_NAME}, ven aquí!");
static const u8 sText_Trainer1RecallBothSpa[] = _("{B_TRAINER1_NAME}: ¡{B_OPPONENT_MON1_NAME} y\n{B_OPPONENT_MON2_NAME}, vengan aquí!");
static const u8 sText_Trainer2NameSpa[] = _("{B_TRAINER2_NAME}");
static const u8 sText_PkmnGainedEXPSpa[] = _("¡{B_BUFF1} ganó {B_TRAINER2_LOSE_TEXT}\npuntos de Exp.{B_BUFF2}!\p");
static const u8 sText_EmptyString4Spa[] = _("");
static const u8 sText_ABoostedSpa[] = _(" adicionales");
static const u8 sText_PkmnGrewToLvSpa[] = _("{B_BUFF1} subió al\nnivel {B_BUFF2}!{WAIT_SE}\p");
static const u8 sText_PkmnLearnedMoveSpa[] = _("{B_BUFF1} aprendió\n{B_BUFF2}!{WAIT_SE}\p");
static const u8 sText_TryToLearnMove1Spa[] = _("{B_BUFF1} intenta\naprender {B_BUFF2}.\p");
static const u8 sText_TryToLearnMove2Spa[] = _("Pero {B_BUFF1} no puede aprender\nmás de cuatro movimientos.\p");
static const u8 sText_TryToLearnMove3Spa[] = _("¿Quieres sustituir uno de esos\nmovimientos por {B_BUFF2}?");
static const u8 sText_PkmnForgotMoveSpa[] = _("{B_BUFF1} olvidó\n{B_BUFF2}.\p");
static const u8 sText_StopLearningMoveSpa[] = _("{PAUSE 32}¿Deja de aprender\n{B_BUFF2}?");
static const u8 sText_DidNotLearnMoveSpa[] = _("{B_BUFF1} no aprendió\n{B_BUFF2}.\p");
static const u8 sText_UseNextPkmnSpa[] = _("¿Usas el siguiente Pokémon?");
static const u8 sText_AttackMissedSpa[] = _("¡El ataque de\n{B_ATK_NAME_WITH_PREFIX} falló!");
static const u8 sText_PkmnProtectedItselfSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX}\nse ha protegido!");
static const u8 sText_AvoidedDamageSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} se\nprotegió con {B_DEF_ABILITY}!");
static const u8 sText_PkmnMakesGroundMissSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} impide ataques\nde Tierra con {B_DEF_ABILITY}!");
static const u8 sText_PkmnAvoidedAttackSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} evitó\nel ataque!");
static const u8 sText_ItDoesntAffectSpa[] = _("No afecta a\n{B_DEF_NAME_WITH_PREFIX}...");
static const u8 sText_AttackerFaintedSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX}\nse debilitó!\p");
static const u8 sText_TargetFaintedSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX}\nse debilitó!\p");
static const u8 sText_PlayerGotMoneySpa[] = _("¡{B_PLAYER_NAME} ganó\n{B_BUFF1}¥!\p");
static const u8 sText_PlayerWhiteoutSpa[] = _("¡A {B_PLAYER_NAME} no le\nquedan Pokémon!\p");
static const u8 sText_PlayerPanickedSpa[] = _("¡{B_PLAYER_NAME} se ha asustado\ny ha perdido {B_BUFF1}¥!...\p... ... ... ...\p¡{B_PLAYER_NAME} está fuera\nde combate!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PlayerWhiteoutAgainstTrainerSpa[] = _("¡A {B_PLAYER_NAME} no le\nquedan Pokémon!\p¡Has perdido contra\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PlayerPaidAsPrizeMoneySpa[] = _("¡{B_PLAYER_NAME} tuvo que pagar\n{B_BUFF1}¥!...\p... ... ... ...\p¡{B_PLAYER_NAME} está fuera\nde combate!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PlayerWhiteout2Spa[] = _("¡{B_PLAYER_NAME} está fuera\nde combate!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PreventsEscapeSpa[] = _("¡{B_SCR_ACTIVE_NAME_WITH_PREFIX} impide la\nhuida con {B_SCR_ACTIVE_ABILITY}!\p");
static const u8 sText_CantEscape2Spa[] = _("¡No puedes huir!\p");
static const u8 sText_AttackerCantEscapeSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX}\nno puede escapar!");
static const u8 sText_HitXTimesSpa[] = _("N.º de golpes: {B_BUFF1}.");
static const u8 sText_PkmnFellAsleepSpa[] = _("¡{B_EFF_NAME_WITH_PREFIX}\nse durmió!");
static const u8 sText_PkmnMadeSleepSpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\ndurmió a {B_EFF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnAlreadyAsleepSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX}\nestá dormido!");
static const u8 sText_PkmnAlreadyAsleep2Spa[] = _("¡{B_ATK_NAME_WITH_PREFIX}\nestá dormido!");
static const u8 sText_PkmnWasntAffectedSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} no se\nvio afectado!");
static const u8 sText_PkmnWasPoisonedSpa[] = _("¡{B_EFF_NAME_WITH_PREFIX}\nfue envenenado!");
static const u8 sText_PkmnPoisonedBySpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nenvenenó a {B_EFF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnHurtByPoisonSpa[] = _("¡El veneno resta salud\na {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnAlreadyPoisonedSpa[] = _("{B_DEF_NAME_WITH_PREFIX}\nestá envenenado.");
static const u8 sText_PkmnBadlyPoisonedSpa[] = _("¡{B_EFF_NAME_WITH_PREFIX} fue \ngravemente envenenado!");
static const u8 sText_PkmnEnergyDrainedSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} ha perdido\nenergía!");
static const u8 sText_PkmnWasBurnedSpa[] = _("¡{B_EFF_NAME_WITH_PREFIX} se ha quemado!");
static const u8 sText_PkmnBurnedBySpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nquemó a {B_EFF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnHurtByBurnSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se resiente\nde la quemadura!");
static const u8 sText_PkmnAlreadyHasBurnSpa[] = _("{B_DEF_NAME_WITH_PREFIX} \nya está quemado.");
static const u8 sText_PkmnWasFrozenSpa[] = _("¡{B_EFF_NAME_WITH_PREFIX} fue\ncongelado!");
static const u8 sText_PkmnFrozenBySpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\ncongeló a {B_EFF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnIsFrozenSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} está\ncongelado!");
static const u8 sText_PkmnWasDefrostedSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} ya\nno está congelado!");
static const u8 sText_PkmnWasDefrosted2Spa[] = _("¡{B_ATK_NAME_WITH_PREFIX} ya\nno está congelado!");
static const u8 sText_PkmnWasDefrostedBySpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} descongeló\na {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnWasParalyzedSpa[] = _("¡{B_EFF_NAME_WITH_PREFIX} está paralizado!\n¡Quizás no se mueva!");
static const u8 sText_PkmnWasParalyzedBySpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nparalizó a {B_EFF_NAME_WITH_PREFIX}!\l¡Quizás no se mueva!");
static const u8 sText_PkmnIsParalyzedSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} está paralizado!\n¡No se puede mover!");
static const u8 sText_PkmnIsAlreadyParalyzedSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX}\nestá paralizado!");
static const u8 sText_PkmnHealedParalysisSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} ya\nno está paralizado!");
static const u8 sText_PkmnDreamEatenSpa[] = _("¡Se comió el sueño\nde {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_StatsWontIncreaseSpa[] = _("¡{B_BUFF1} de {B_ATK_NAME_WITH_PREFIX}\nno subirá más!");
static const u8 sText_StatsWontDecreaseSpa[] = _("¡{B_BUFF1} de {B_DEF_NAME_WITH_PREFIX}\nno bajará más!");
static const u8 sText_TeamStoppedWorkingSpa[] = _("¡{B_BUFF1} no funciona\nen tu equipo!");
static const u8 sText_FoeStoppedWorkingSpa[] = _("¡{B_BUFF1} no funciona\nen el equipo rival!");
static const u8 sText_PkmnIsConfusedSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} está\nconfuso!");
static const u8 sText_PkmnHealedConfusionSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} ya\nno está confuso!");
static const u8 sText_PkmnWasConfusedSpa[] = _("¡{B_EFF_NAME_WITH_PREFIX} se\nencuentra confuso!");
static const u8 sText_PkmnAlreadyConfusedSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} ya\nestá confuso!");
static const u8 sText_PkmnFellInLoveSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} se ha\nenamorado!");
static const u8 sText_PkmnInLoveSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se ha enamorado\nde {B_SCR_ACTIVE_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnImmobilizedByLoveSpa[] = _("¡El amor impide que\n{B_ATK_NAME_WITH_PREFIX} ataque!");
static const u8 sText_PkmnBlownAwaySpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} fue\narrastrado!");
static const u8 sText_PkmnChangedTypeSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} ha cambiado \nal tipo {B_BUFF1}!");
static const u8 sText_PkmnFlinchedSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} retrocedió!");
static const u8 sText_PkmnRegainedHealthSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} recuperó\nsalud!");
static const u8 sText_PkmnHPFullSpa[] = _("¡Los PS de {B_DEF_NAME_WITH_PREFIX}\nestán al máximo!");
static const u8 sText_PkmnRaisedSpDefSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} de Pokémon\n{B_CURRENT_MOVE} subió la Def. Esp.!");
static const u8 sText_PkmnRaisedSpDefALittleSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} de Pokémon\n{B_CURRENT_MOVE} subió algo Def. Esp.!");
static const u8 sText_PkmnRaisedDefSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} de Pokémon\n{B_CURRENT_MOVE} subió la Defensa!");
static const u8 sText_PkmnRaisedDefALittleSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} de Pokémon\n{B_CURRENT_MOVE} subió algo Defensa!");
static const u8 sText_PkmnCoveredByVeilSpa[] = _("¡Pokémon {B_CURRENT_MOVE} se \nprotegió con Velo Sagrado!");
static const u8 sText_PkmnUsedSafeguardSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} está protegido\npor Velo Sagrado!");
static const u8 sText_PkmnSafeguardExpiredSpa[] = _("¡VELO SAGRADO de Pokémon\n{B_ATK_PREFIX3} dejó de hacer efecto!");
static const u8 sText_PkmnWentToSleepSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se fue\na dormir!");
static const u8 sText_PkmnSleptHealthySpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} recuperó\nsalud durmiendo!");
static const u8 sText_PkmnWhippedWhirlwindSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} provocó \nun remolino!");
static const u8 sText_PkmnTookSunlightSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} absorbió \nluz solar!");
static const u8 sText_PkmnLoweredHeadSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} bajó \nla cabeza!");
static const u8 sText_PkmnIsGlowingSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} está rodeado\nde una luz brillante!");
static const u8 sText_PkmnFlewHighSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} voló\nmuy alto!");
static const u8 sText_PkmnDugHoleSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se\nocultó en un agujero!");
static const u8 sText_PkmnHidUnderwaterSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se ocultó\nbajo el agua!");
static const u8 sText_PkmnSprangUpSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} dio un\nsalto tremendo!");
static const u8 sText_PkmnSqueezedByBindSpa[] = _("¡Atadura de {B_ATK_NAME_WITH_PREFIX}\noprime a {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnTrappedInVortexSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} fue atrapado\nen el torbellino!");
static const u8 sText_PkmnTrappedBySandTombSpa[] = _("¡Bucle Arena atrapó\na {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnWrappedBySpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} fue atrapado por\nREPETICIÓN de {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnClampedSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} Atenazó\na {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnHurtBySpa[] = _("¡{B_BUFF1} ha herido\na {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnFreedFromSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se liberó \nde {B_BUFF1}!");
static const u8 sText_PkmnCrashedSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} falló\ny se cayó!");
const u8 gBattleText_MistShroudSpa[] = _("¡Neblina ha cubierto a\nPokémon {B_CURRENT_MOVE}!");
static const u8 sText_PkmnProtectedByMistSpa[] = _("¡{B_SCR_ACTIVE_NAME_WITH_PREFIX} se ha\nprotegido con Neblina!");
const u8 gBattleText_GetPumpedSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se está\npreparando para luchar!");
static const u8 sText_PkmnHitWithRecoilSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} también\nse ha hecho daño!");
static const u8 sText_PkmnProtectedItself2Spa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se\nestá protegiendo!");
static const u8 sText_PkmnBuffetedBySandstormSpa[] = _("¡Tormenta Arena zarandea\na {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnPeltedByHailSpa[] = _("¡Granizo golpea\na {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnsXWoreOffSpa[] = _("¡{B_BUFF1} de Pokémon \n{B_SCR_ACTIVE_NAME_WITH_PREFIX} se agotó!");
static const u8 sText_PkmnSeededSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} fue infectado!");
static const u8 sText_PkmnEvadedAttackSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} esquivó\nel ataque!");
static const u8 sText_PkmnSappedByLeechSeedSpa[] = _("¡Drenadoras restó  \nsalud a {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnFastAsleepSpa[] = _("{B_ATK_NAME_WITH_PREFIX} está\ndormido como un tronco.");
static const u8 sText_PkmnWokeUpSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se despertó!");
static const u8 sText_PkmnUproarKeptAwakeSpa[] = _("¡Pero el Alboroto de \n{B_SCR_ACTIVE_NAME_WITH_PREFIX} no le deja dormir!");
static const u8 sText_PkmnWokeUpInUproarSpa[] = _("¡Alboroto despertó a\n{B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnCausedUproarSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX}\nmontó un Alboroto!");
static const u8 sText_PkmnMakingUproarSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} está\nmontando un Alboroto!");
static const u8 sText_PkmnCalmedDownSpa[] = _("{B_ATK_NAME_WITH_PREFIX} se tranquilizó.");
static const u8 sText_PkmnCantSleepInUproarSpa[] = _("¡Pero {B_DEF_NAME_WITH_PREFIX} no puede\ndormir con el Alboroto!");
static const u8 sText_PkmnStockpiledSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} Reservó\n{B_BUFF1}!");
static const u8 sText_PkmnCantStockpileSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} no puede\nReservar más!");
static const u8 sText_PkmnCantSleepInUproar2Spa[] = _("¡Pero {B_DEF_NAME_WITH_PREFIX} no puede \ndormir con el Alboroto!");
static const u8 sText_UproarKeptPkmnAwakeSpa[] = _("¡Pero el Alboroto mantuvo\ndespierto a {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnStayedAwakeUsingSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} se mantuvo\ndespierto con {B_DEF_ABILITY}!");
static const u8 sText_PkmnStoringEnergySpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} está \nacumulando energía!");
static const u8 sText_PkmnUnleashedEnergySpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} liberó \nenergía!");
static const u8 sText_PkmnFatigueConfusionSpa[] = _("¡El cansancio terminó \nconfundiendo a {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnPickedUpItemSpa[] = _("¡{B_PLAYER_NAME} recogió\n{B_BUFF1}¥!\p");
static const u8 sText_PkmnUnaffectedSpa[] = _("¡No afectó a\n{B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnTransformedIntoSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se transformó \nen {B_BUFF1}!");
static const u8 sText_PkmnMadeSubstituteSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} creó\nun Sustituto!");
static const u8 sText_PkmnHasSubstituteSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} ya tiene\nun Sustituto!");
static const u8 sText_SubstituteDamagedSpa[] = _("¡El Sustituto recibe el daño\nen lugar de {B_DEF_NAME_WITH_PREFIX}!\p");
static const u8 sText_PkmnSubstituteFadedSpa[] = _("¡El Sustituto de\n{B_DEF_NAME_WITH_PREFIX} se debilitó!\p");
static const u8 sText_PkmnMustRechargeSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} necesita\nrecuperarse de su ataque!");
static const u8 sText_PkmnRageBuildingSpa[] = _("¡La Furia de {B_DEF_NAME_WITH_PREFIX}\nestá creciendo!");
static const u8 sText_PkmnMoveWasDisabledSpa[] = _("¡{B_BUFF1} de {B_DEF_NAME_WITH_PREFIX}\nfue desactivado con Anulación!");
static const u8 sText_PkmnMoveDisabledNoMoreSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se\nha liberado de Anulación!");
static const u8 sText_PkmnGotEncoreSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} sufrió los\nefectos de Otra Vez!");
static const u8 sText_PkmnEncoreEndedSpa[] = _("¡Otra Vez de {B_ATK_NAME_WITH_PREFIX}\nya no hace efecto!");
static const u8 sText_PkmnTookAimSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} apuntó\na {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnSketchedMoveSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} usó Esquema\nen {B_BUFF1}!");
static const u8 sText_PkmnTryingToTakeFoeSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} intenta\nllevarse a su rival!");
static const u8 sText_PkmnTookFoeSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} se llevó\na {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnReducedPPSpa[] = _("¡Redujo {B_BUFF1}\nde {B_DEF_NAME_WITH_PREFIX} en {B_BUFF2}!");
static const u8 sText_PkmnStoleItemSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} robó\n{B_LAST_ITEM} de {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_TargetCantEscapeNowSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} no \npuede escapar!");
static const u8 sText_PkmnFellIntoNightmareSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} cae en \nuna Pesadilla!");
static const u8 sText_PkmnLockedInNightmareSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} está inmerso\nen una Pesadilla!");
static const u8 sText_PkmnLaidCurseSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} reduce sus PS\ny Maldice a {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnAfflictedByCurseSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} es víctima \nde una Maldición!");
static const u8 sText_SpikesScatteredSpa[] = _("¡El equipo Pokémon {B_ATK_NAME_WITH_PREFIX}\nha sido rodeado de Púas!");
static const u8 sText_PkmnHurtBySpikesSpa[] = _("¡{B_SCR_ACTIVE_NAME_WITH_PREFIX} ha sido\nherido por Púas!");
static const u8 sText_PkmnIdentifiedSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} identificó\na {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnPerishCountFellSpa[] = _("¡Contador de salud de\n{B_ATK_NAME_WITH_PREFIX} bajó a {B_BUFF1}!");
static const u8 sText_PkmnBracedItselfSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se ha\nfortalecido!");
static const u8 sText_PkmnEnduredHitSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} Aguantó\nel golpe!");
static const u8 sText_MagnitudeStrengthSpa[] = _("¡MAGNITUD {B_BUFF1}!");
static const u8 sText_PkmnCutHPMaxedAttackSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} redujo sus PS\ny mejoró su Ataque!");
static const u8 sText_PkmnCopiedStatChangesSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} copió las nuevas \ncaracteríst. de {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnGotFreeSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se liberó de\n{B_BUFF1} de {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnShedLeechSeedSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se\nliberó de Drenadoras!");
static const u8 sText_PkmnBlewAwaySpikesSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se\nliberó de Púas!");
static const u8 sText_PkmnFledFromBattleSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} huyó del\ncombate!");
static const u8 sText_PkmnForesawAttackSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} previó\nun ataque!");
static const u8 sText_PkmnTookAttackSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} tomó \nel ataque de {B_BUFF1}!");
static const u8 sText_PkmnChoseXAsDestinySpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} usará\n{B_ATK_NAME_WITH_PREFIX} más adelante!");
static const u8 sText_PkmnAttackSpa[] = _("¡Ataque de {B_BUFF1}!");
static const u8 sText_PkmnCenterAttentionSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se convirtió en\nel centro de atención!");
static const u8 sText_PkmnChargingPowerSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} comenzó \na cargar energía!");
static const u8 sText_NaturePowerTurnedIntoSpa[] = _("¡Adaptación se convirtió \nen {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnStatusNormalSpa[] = _("¡El estado de {B_ATK_NAME_WITH_PREFIX}\nregresó a la normalidad!");
static const u8 sText_PkmnSubjectedToTormentSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} es víctima\nde Tormento!");
static const u8 sText_PkmnTighteningFocusSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} está reforzando\nsu concentración!");
static const u8 sText_PkmnFellForTauntSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} se dejó\nengañar por Mofa!");
static const u8 sText_PkmnReadyToHelpSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} está listo para\nayudar a {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnSwitchedItemsSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} provocó \nun cambio de objeto!");
static const u8 sText_PkmnObtainedXSpa[] = _("{B_ATK_NAME_WITH_PREFIX} obtuvo\n{B_BUFF1}.");
static const u8 sText_PkmnObtainedX2Spa[] = _("{B_DEF_NAME_WITH_PREFIX} obtuvo\n{B_BUFF2}.");
static const u8 sText_PkmnObtainedXYObtainedZSpa[] = _("{B_ATK_NAME_WITH_PREFIX} obtuvo\n{B_BUFF1}.\p{B_DEF_NAME_WITH_PREFIX} obtuvo\n{B_BUFF2}.");
static const u8 sText_PkmnCopiedFoeSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} copió\n{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnMadeWishSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} pidió un Deseo!");
static const u8 sText_PkmnWishCameTrueSpa[] = _("¡El Deseo de {B_BUFF1}\nse hizo realidad!");
static const u8 sText_PkmnPlantedRootsSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} echó raíces!");
static const u8 sText_PkmnAbsorbedNutrientsSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} absorbió\nnutrientes con las raíces!");
static const u8 sText_PkmnAnchoredItselfSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} se ancló\ncon las raíces!");
static const u8 sText_PkmnWasMadeDrowsySpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} adormeció\na {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnKnockedOffSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} echó abajo\n{B_LAST_ITEM} de {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnSwappedAbilitiesSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} intercambió\nsu habilidad especial!");
static const u8 sText_PkmnSealedOpponentMoveSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} selló\nmovimientos del oponente!");
static const u8 sText_PkmnWantsGrudgeSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} provoca\nRabia a su rival!");
static const u8 sText_PkmnLostPPGrudgeSpa[] = _("¡La Rabia hizo que {B_BUFF1} de\n{B_ATK_NAME_WITH_PREFIX} se quedara sin PP!");
static const u8 sText_PkmnShroudedItselfSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se cubrió\ncon {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnMoveBouncedSpa[] = _("¡Capa Mágica hizo rebotar\n{B_ATK_NAME_WITH_PREFIX} de {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnWaitsForTargetSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} espera a que su\nrival haga algún movimiento!");
static const u8 sText_PkmnSnatchedMoveSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} Robó el \nmovimiento de {B_SCR_ACTIVE_NAME_WITH_PREFIX}!");
static const u8 sText_ElectricityWeakenedSpa[] = _("¡Se han debilitado los\nataques Eléctricos!");
static const u8 sText_FireWeakenedSpa[] = _("¡Se han debilitado los\nataques de Fuego!");
static const u8 sText_XFoundOneYSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} encontró una\nunidad de {B_LAST_ITEM}!");
static const u8 sText_SoothingAromaSpa[] = _("¡Un balsámico aroma \nimpregnó la zona!");
static const u8 sText_ItemsCantBeUsedNowSpa[] = _("Aquí no se pueden usar objetos.{PAUSE 64}");
static const u8 sText_ForXCommaYZSpa[] = _("¡{B_SCR_ACTIVE_NAME_WITH_PREFIX} encuentra\n{B_LAST_ITEM} {B_BUFF1}");
static const u8 sText_PkmnUsedXToGetPumpedSpa[] = _("¡{B_SCR_ACTIVE_NAME_WITH_PREFIX} usa {B_LAST_ITEM}\npara potenciar los golpes críticos!");
static const u8 sText_PkmnLostFocusSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se desconcentró\ny quedó inmovilizado!");
static const u8 sText_PkmnWasDraggedOutSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} fue\narrastrado al combate!\p");
static const u8 sText_TheWallShatteredSpa[] = _("¡Destruyó la protección!");
static const u8 sText_ButNoEffectSpa[] = _("¡Pero no surtió efecto!");
static const u8 sText_PkmnHasNoMovesLeftSpa[] = _("¡A {B_ACTIVE_NAME_WITH_PREFIX} no le quedan\nmás movimientos!\p");
static const u8 sText_PkmnMoveIsDisabledSpa[] = _("¡{B_ACTIVE_NAME_WITH_PREFIX} no puede usar\n{B_CURRENT_MOVE} por culpa de Anulación!\p");
static const u8 sText_PkmnCantUseMoveTormentSpa[] = _("¡{B_ACTIVE_NAME_WITH_PREFIX} no puede usar el \nmismo movimiento dos veces \lseguidas debido a Tormento!\p");
static const u8 sText_PkmnCantUseMoveTauntSpa[] = _("¡{B_ACTIVE_NAME_WITH_PREFIX} no puede usar\n{B_CURRENT_MOVE} debido a Mofa!\p");
static const u8 sText_PkmnCantUseMoveSealedSpa[] = _("¡{B_ACTIVE_NAME_WITH_PREFIX} no puede usar\n{B_CURRENT_MOVE} porque está sellado!\p");
static const u8 sText_PkmnMadeItRainSpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nhizo llover!");
static const u8 sText_PkmnRaisedSpeedSpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nsubió su Velocidad!");
static const u8 sText_PkmnProtectedBySpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} estaba protegido\npor {B_DEF_ABILITY}!");
static const u8 sText_PkmnPreventsUsageSpa[] = _("¡{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX}\nimpide que {B_CURRENT_MOVE}\lutilice {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnRestoredHPUsingSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} restauró PS\nusando su {B_DEF_ABILITY}!");
static const u8 sText_PkmnsXMadeYUselessSpa[] = _("¡{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX}\ninutilizó {B_CURRENT_MOVE}!");
static const u8 sText_PkmnChangedTypeWithSpa[] = _("¡{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX}\nlo convirtió en \lel tipo {B_BUFF1}!");
static const u8 sText_PkmnPreventsParalysisWithSpa[] = _("¡{B_DEF_ABILITY} de {B_EFF_NAME_WITH_PREFIX}\nimpide que se quede paralizado!");
static const u8 sText_PkmnPreventsRomanceWithSpa[] = _("¡{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX}\nimpide que se enamore!");
static const u8 sText_PkmnPreventsPoisoningWithSpa[] = _("¡{B_DEF_ABILITY} de {B_EFF_NAME_WITH_PREFIX}\nimpide que se envenene!");
static const u8 sText_PkmnPreventsConfusionWithSpa[] = _("¡{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX}\nimpide que quede confuso!");
static const u8 sText_PkmnRaisedFirePowerWithSpa[] = _("¡{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX}\nreforzó sus ataques de Fuego!");
static const u8 sText_PkmnAnchorsItselfWithSpa[] = _("¡{B_DEF_NAME_WITH_PREFIX} se aferra\nal suelo con {B_DEF_ABILITY}!");
static const u8 sText_PkmnCutsAttackWithSpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\npara el Ataque de {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnPreventsStatLossWithSpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nimpide que pierda características!");
static const u8 sText_PkmnHurtsWithSpa[] = _("¡{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX}\nhirió a {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnTracedSpa[] = _("¡{B_SCR_ACTIVE_NAME_WITH_PREFIX} Rastreó\n{B_BUFF2} de {B_BUFF1}!");
static const u8 sText_PkmnsXPreventsBurnsSpa[] = _("¡{B_EFF_ABILITY} de {B_EFF_NAME_WITH_PREFIX}\nimpide que se queme!");
static const u8 sText_PkmnsXBlocksYSpa[] = _("¡{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX}\nbloquea {B_CURRENT_MOVE}!");
static const u8 sText_PkmnsXBlocksY2Spa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nbloquea {B_CURRENT_MOVE}!");
static const u8 sText_PkmnsXRestoredHPALittle2Spa[] = _("¡{B_ATK_NAME_WITH_PREFIX} restauró algo\nsus PS con {B_ATK_ABILITY}!");
static const u8 sText_PkmnsXWhippedUpSandstormSpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nlevantó una tormenta de arena!");
static const u8 sText_PkmnsXIntensifiedSunSpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nintensificó los rayos del sol!");
static const u8 sText_PkmnsXPreventsYLossSpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nimpide que pierda {B_BUFF1}!");
static const u8 sText_PkmnsXInfatuatedYSpa[] = _("¡{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX}\nenamoró a {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnsXMadeYIneffectiveSpa[] = _("¡{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX}\nanuló {B_CURRENT_MOVE}!");
static const u8 sText_PkmnsXCuredYProblemSpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\ncuró su problema de {B_BUFF1}!");
static const u8 sText_ItSuckedLiquidOozeSpa[] = _("¡Absorbió el \nLodo Líquido!");
static const u8 sText_PkmnTransformedSpa[] = _("¡{B_SCR_ACTIVE_NAME_WITH_PREFIX} se transformó!");
static const u8 sText_PkmnsXTookAttackSpa[] = _("¡{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX}\nrecibió el ataque!");
const u8 gText_PkmnsXPreventsSwitchingSpa[] = _("¡{B_LAST_ABILITY} de {B_BUFF1}\nimpide el cambio!\p");
static const u8 sText_PreventedFromWorkingSpa[] = _("¡{B_DEF_ABILITY} de {B_DEF_NAME_WITH_PREFIX}\nimpidió que {B_BUFF1}\lde {B_SCR_ACTIVE_NAME_WITH_PREFIX} funcionase!");
static const u8 sText_PkmnsXMadeItIneffectiveSpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nlo hizo ineficaz!");
static const u8 sText_PkmnsXPreventsFlinchingSpa[] = _("¡{B_EFF_ABILITY} de {B_EFF_NAME_WITH_PREFIX} \nimpide la retirada!");
static const u8 sText_PkmnsXPreventsYsZSpa[] = _("¡{B_ATK_ABILITY} de {B_ATK_NAME_WITH_PREFIX}\nimpide que {B_DEF_ABILITY}\lde {B_DEF_NAME_WITH_PREFIX} funcione!");
static const u8 sText_PkmnsXCuredItsYProblemSpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\ncuró su problema de {B_BUFF1}!");
static const u8 sText_PkmnsXHadNoEffectOnYSpa[] = _("¡{B_SCR_ACTIVE_ABILITY} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\nno ha afectado a {B_EFF_NAME_WITH_PREFIX}!");
static const u8 sText_TooScaredToMoveSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} está tan asustado que\nno puede moverse!");
static const u8 sText_GetOutGetOutSpa[] = _("Ghost: Fuera... Fuera...");
static const u8 sText_StatSharplySpa[] = _("subió mucho");
const u8 gBattleText_RoseSpa[] = _("subió");
static const u8 sText_StatHarshlySpa[] = _("bajó mucho");
static const u8 sText_StatFellSpa[] = _("bajó");
static const u8 sText_PkmnsStatChangedSpa[] = _("¡{B_BUFF1} de {B_ATK_NAME_WITH_PREFIX}\n{B_BUFF2}!");
const u8 gText_PkmnsStatChanged2Spa[] = _("¡{B_BUFF1} de {B_DEF_NAME_WITH_PREFIX}\n{B_BUFF2}!");
static const u8 sText_UsingXTheYOfZNSpa[] = _("¡{B_BUFF1} de {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n{B_BUFF2} con {B_LAST_ITEM}!");
static const u8 sText_PkmnsStatChanged3Spa[] = _("¡{B_BUFF1} de {B_ATK_NAME_WITH_PREFIX}\n{B_BUFF2}!");
static const u8 sText_PkmnsStatChanged4Spa[] = _("¡{B_BUFF1} de {B_DEF_NAME_WITH_PREFIX}\n{B_BUFF2}!");
static const u8 sText_StatsWontIncrease2Spa[] = _("¡Las características de\n{B_ATK_NAME_WITH_PREFIX} no subirán más!");
static const u8 sText_StatsWontDecrease2Spa[] = _("¡Las características de\n{B_DEF_NAME_WITH_PREFIX} no bajarán más!");
static const u8 sText_CriticalHitSpa[] = _("¡Un golpe crítico!");
static const u8 sText_OneHitKOSpa[] = _("¡K.O. en 1 golpe!");
static const u8 sText_123PoofSpa[] = _("{PAUSE 32}1, {PAUSE 15}2 y{PAUSE 15}... {PAUSE 15}... {PAUSE 15}... {PAUSE 15}{PLAY_SE SE_BALL_BOUNCE_1}¡puf!\p");
static const u8 sText_AndEllipsisSpa[] = _("Y...\p");
static const u8 sText_HMMovesCantBeForgottenSpa[] = _("Los movimientos MO no\nse pueden olvidar así.\p");
static const u8 sText_NotVeryEffectiveSpa[] = _("No es muy eficaz...");
static const u8 sText_SuperEffectiveSpa[] = _("¡Es muy eficaz!");
static const u8 sText_GotAwaySafelySpa[] = _("{PLAY_SE SE_FLEE}¡Escapaste sin problemas!\p");
static const u8 sText_PkmnFledUsingItsSpa[] = _("{PLAY_SE SE_FLEE}¡{B_ATK_NAME_WITH_PREFIX} escapó\nusando su {B_LAST_ITEM}!\p");
static const u8 sText_PkmnFledUsingSpa[] = _("{PLAY_SE SE_FLEE}¡{B_ATK_NAME_WITH_PREFIX} escapó\nusando su {B_ATK_ABILITY}!\p");
static const u8 sText_WildPkmnFledSpa[] = _("{PLAY_SE SE_FLEE}¡{B_BUFF1} salvaje ha huido!");
static const u8 sText_PlayerDefeatedLinkTrainerSpa[] = _("¡Derrotaste a\n{B_LINK_OPPONENT1_NAME}!");
static const u8 sText_TwoLinkTrainersDefeatedSpa[] = _("¡Derrotaste a\n{B_LINK_OPPONENT2_NAME} y {B_LINK_OPPONENT1_NAME}!");
static const u8 sText_PlayerLostAgainstLinkTrainerSpa[] = _("¡Has perdido contra\n{B_LINK_OPPONENT1_NAME}!");
static const u8 sText_PlayerLostToTwoSpa[] = _("¡Has perdido contra\n{B_LINK_OPPONENT2_NAME} y {B_LINK_OPPONENT1_NAME}!");
static const u8 sText_PlayerBattledToDrawLinkTrainerSpa[] = _("¡{B_LINK_OPPONENT1_NAME} ha empatado\ncontigo!");
static const u8 sText_PlayerBattledToDrawVsTwoSpa[] = _("¡{B_LINK_OPPONENT2_NAME} y {B_LINK_OPPONENT1_NAME} han\nempatado contigo!");
static const u8 sText_WildFledSpa[] = _("{PLAY_SE SE_FLEE}¡{B_LINK_OPPONENT1_NAME} ha huido!");
static const u8 sText_TwoWildFledSpa[] = _("{PLAY_SE SE_FLEE}¡{B_LINK_OPPONENT1_NAME} y\n{B_LINK_OPPONENT2_NAME} han huido!");
static const u8 sText_NoRunningFromTrainersSpa[] = _("¡No puedes huir de un\ncombate contra un Entrenador!\p");
static const u8 sText_CantEscapeSpa[] = _("¡No puedes huir!\p");
static const u8 sText_EmptyString5Spa[] = _("");   //sText_DontLeaveBirch in pokeem
static const u8 sText_ButNothingHappenedSpa[] = _("¡Pero no tuvo ningún efecto!");
static const u8 sText_ButItFailedSpa[] = _("¡Pero falló!");
static const u8 sText_ItHurtConfusionSpa[] = _("¡Está tan confuso que se\nhirió a sí mismo!");
static const u8 sText_MirrorMoveFailedSpa[] = _("¡El Mov. Espejo ha fallado!");
static const u8 sText_StartedToRainSpa[] = _("¡Ha empezado a llover!");
static const u8 sText_DownpourStartedSpa[] = _("¡Comenzó a caer un chaparrón!");
static const u8 sText_RainContinuesSpa[] = _("Sigue lloviendo...");
static const u8 sText_DownpourContinuesSpa[] = _("El chaparrón continúa...");
static const u8 sText_RainStoppedSpa[] = _("Ha dejado de llover.");
static const u8 sText_SandstormBrewedSpa[] = _("¡Se acerca una tormenta de arena!");
static const u8 sText_SandstormRagesSpa[] = _("La tormenta de arena arrecia...");
static const u8 sText_SandstormSubsidedSpa[] = _("La tormenta de arena amainó.");
static const u8 sText_SunlightGotBrightSpa[] = _("¡El sol está brillando!");
static const u8 sText_SunlightStrongSpa[] = _("Hace mucho sol...");
static const u8 sText_SunlightFadedSpa[] = _("Se ha ido el sol.");
static const u8 sText_StartedHailSpa[] = _("¡Ha empezado a granizar!");
static const u8 sText_HailContinuesSpa[] = _("Sigue granizando...");
static const u8 sText_HailStoppedSpa[] = _("Ha dejado de granizar.");
static const u8 sText_FailedToSpitUpSpa[] = _("¡Pero no consiguió \nEscupir energía!");
static const u8 sText_FailedToSwallowSpa[] = _("¡Pero no consiguió \nTragar energía!");
static const u8 sText_WindBecameHeatWaveSpa[] = _("¡El viento se convirtió en\nuna Onda Ígnea!");
static const u8 sText_StatChangesGoneSpa[] = _("¡Se han eliminado todos los\ncambios en las características!");
static const u8 sText_CoinsScatteredSpa[] = _("¡Hay monedas por todas partes!");
static const u8 sText_TooWeakForSubstituteSpa[] = _("¡Estaba demasiado débil para\ncrear un Sustituto!");
static const u8 sText_SharedPainSpa[] = _("¡Los combatientes comparten\nel daño sufrido!");
static const u8 sText_BellChimedSpa[] = _("¡Ha repicado una campana!");
static const u8 sText_FaintInThreeSpa[] = _("¡Los Pokémon de ambas partes se\ndebilitarán dentro de 3 turnos!");
static const u8 sText_NoPPLeftSpa[] = _("¡No quedan PP para \neste movimiento!\p");
static const u8 sText_ButNoPPLeftSpa[] = _("¡Pero no quedan PP para\neste movimiento!");
static const u8 sText_PkmnIgnoresAsleepSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} ignoró las\nórdenes porque está dormido!");
static const u8 sText_PkmnIgnoredOrdersSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} ignoró\nlas órdenes!");
static const u8 sText_PkmnBeganToNapSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se fue a dormir!");
static const u8 sText_PkmnLoafingSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} está \nholgazaneando!");
static const u8 sText_PkmnWontObeySpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} no\nquiere obedecer!");
static const u8 sText_PkmnTurnedAwaySpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} se alejó!");
static const u8 sText_PkmnPretendNotNoticeSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} fingió\nno darse cuenta!");
static const u8 sText_EnemyAboutToSwitchPkmnSpa[] = _("{B_BUFF2} será el próximo Pokémon \nde {B_TRAINER1_CLASS} {B_TRAINER1_NAME}.\p{B_PLAYER_NAME}, ¿quieres \ncambiar de Pokémon?");
static const u8 sText_PkmnLearnedMove2Spa[] = _("¡{B_ATK_NAME_WITH_PREFIX} ha aprendido\n{B_BUFF1}!");
static const u8 sText_PlayerDefeatedLinkTrainerTrainer1Spa[] = _("¡Derrotaste a\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!\p");
static const u8 sText_ThrewARockSpa[] = _("{B_PLAYER_NAME} lanzó\nuna Roca a {B_OPPONENT_MON1_NAME}.");
static const u8 sText_ThrewSomeBaitSpa[] = _("{B_PLAYER_NAME} lanzó\nalgo de Cebo a {B_OPPONENT_MON1_NAME}.");
static const u8 sText_PkmnWatchingCarefullySpa[] = _("¡{B_OPPONENT_MON1_NAME} te observa\natentamente!");
static const u8 sText_PkmnIsAngrySpa[] = _("¡{B_OPPONENT_MON1_NAME} está enfadado!");
static const u8 sText_PkmnIsEatingSpa[] = _("¡{B_OPPONENT_MON1_NAME} está comiendo!");
static const u8 sText_OutOfSafariBallsSpa[] = _("{PLAY_SE SE_DING_DONG}Encargado: ¡No te quedan más\nSafari Balls! ¡Se acabó el tiempo!\p");
static const u8 sText_WildPkmnAppearedSpa[] = _("¡Un {B_OPPONENT_MON1_NAME} salvaje!\p");
static const u8 sText_WildPkmnAppeared2Spa[] = _("¡Un {B_OPPONENT_MON1_NAME} salvaje!\p");
static const u8 sText_WildPkmnAppearedPauseSpa[] = _("¡Un {B_OPPONENT_MON1_NAME} salvaje!{PAUSE 127}");
static const u8 sText_TwoWildPkmnAppearedSpa[] = _("¡Un {B_OPPONENT_MON2_NAME} y un\n{B_OPPONENT_MON1_NAME} salvajes!\p");
static const u8 sText_GhostAppearedCantIdSpa[] = _("¡El Ghost apareció!\p¡Maldición!\n¡El Ghost no puede ser identificado!\p");
static const u8 sText_TheGhostAppearedSpa[] = _("¡El Ghost apareció!\p");
static const u8 sText_SilphScopeUnveilSpa[] = _("¡El Scope Silph reveló la\nidentidad del Ghost!");
static const u8 sText_TheGhostWasSpa[] = _("¡Ese Ghost era el perturbado\nespíritu de la madre de Cubone!\p");
static const u8 sText_Trainer1WantsToBattleSpa[] = _("¡A luchar contra\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!\p");
static const u8 sText_LinkTrainerWantsToBattleSpa[] = _("¡{B_LINK_OPPONENT1_NAME}\nquiere luchar!");
static const u8 sText_TwoLinkTrainersWantToBattleSpa[] = _("¡{B_LINK_OPPONENT1_NAME} y {B_LINK_OPPONENT2_NAME}\nquieren luchar!");
static const u8 sText_Trainer1SentOutPkmnSpa[] = _("¡{B_OPPONENT_MON1_NAME} es el Pokémon enviado \npor {B_TRAINER1_CLASS} {B_TRAINER1_NAME}!{PAUSE 60}");
static const u8 sText_Trainer1SentOutTwoPkmnSpa[] = _("¡{B_OPPONENT_MON1_NAME} y {B_OPPONENT_MON2_NAME} son la\nopción de {B_TRAINER1_CLASS} {B_TRAINER1_NAME}!{PAUSE 60}");
static const u8 sText_Trainer1SentOutPkmn2Spa[] = _("¡{B_BUFF1} es el Pokémon enviado\npor {B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");
static const u8 sText_LinkTrainerSentOutPkmnSpa[] = _("¡{B_LINK_OPPONENT1_NAME} envió\na {B_OPPONENT_MON1_NAME}!");
static const u8 sText_LinkTrainerSentOutTwoPkmnSpa[] = _("¡{B_LINK_OPPONENT1_NAME} envió a\n{B_OPPONENT_MON1_NAME} y {B_OPPONENT_MON2_NAME}!");
static const u8 sText_TwoLinkTrainersSentOutPkmnSpa[] = _("¡{B_LINK_OPPONENT1_NAME} envió a\n{B_LINK_OPPONENT_MON1_NAME}!\p¡{B_LINK_OPPONENT2_NAME} envió a\n{B_LINK_OPPONENT2_NAME}!");
static const u8 sText_LinkTrainerSentOutPkmn2Spa[] = _("¡{B_LINK_OPPONENT1_NAME} envió\na {B_BUFF1}!");
static const u8 sText_LinkTrainerMultiSentOutPkmnSpa[] = _("¡{B_LINK_SCR_TRAINER_NAME} envió\na {B_BUFF1}!");
static const u8 sText_GoPkmnSpa[] = _("¡Adelante, {B_PLAYER_MON1_NAME}!");
static const u8 sText_GoTwoPkmnSpa[] = _("¡Adelante, {B_PLAYER_MON1_NAME} y\n{B_PLAYER_MON2_NAME}!");
static const u8 sText_GoPkmn2Spa[] = _("¡Adelante, {B_BUFF1}!");
static const u8 sText_DoItPkmnSpa[] = _("¡Vamos, {B_BUFF1}!");
static const u8 sText_GoForItPkmnSpa[] = _("¡Tú puedes, {B_BUFF1}!");
static const u8 sText_YourFoesWeakGetEmPkmnSpa[] = _("¡Tu enemigo está débil!\n¡A por él, {B_BUFF1}!");
static const u8 sText_LinkPartnerSentOutPkmnGoPkmnSpa[] = _("¡{B_LINK_PARTNER_NAME} envió a\n{B_LINK_OPPONENT1_NAME}!\p¡Adelante, {B_LINK_PLAYER_MON1_NAME}!");
static const u8 sText_PkmnThatsEnoughSpa[] = _("¡Ya vale, {B_BUFF1}!\n¡Ven aquí!");
static const u8 sText_PkmnComeBackSpa[] = _("¡{B_BUFF1}, ven aquí!");
static const u8 sText_PkmnOkComeBackSpa[] = _("¡Bien hecho, {B_BUFF1}!\n¡Ven aquí!");
static const u8 sText_PkmnGoodComeBackSpa[] = _("¡Muy bien, {B_BUFF1}!\n¡Ven aquí!");
static const u8 sText_Trainer1WithdrewPkmnSpa[] = _("¡Retirada de {B_BUFF1} por \n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");
static const u8 sText_LinkTrainer1WithdrewPkmnSpa[] = _("¡{B_LINK_OPPONENT1_NAME} retiró a\n{B_BUFF1}!");
static const u8 sText_LinkTrainer2WithdrewPkmnSpa[] = _("¡{B_LINK_SCR_TRAINER_NAME} retiró a\n{B_BUFF1}!");
static const u8 sText_WildPkmnPrefixSpa[] = _(" salvaje");
static const u8 sText_FoePkmnPrefixSpa[] = _(" enemigo");
static const u8 sText_FoePkmnPrefix2Spa[] = _("enemigo");
static const u8 sText_AllyPkmnPrefixSpa[] = _("amigo");
static const u8 sText_FoePkmnPrefix3Spa[] = _("enemigo");
static const u8 sText_AllyPkmnPrefix2Spa[] = _("amigo");
static const u8 sText_FoePkmnPrefix4Spa[] = _("enemigo");
static const u8 sText_AllyPkmnPrefix3Spa[] = _("amigo");
static const u8 sText_AttackerUsedXSpa[] = _("¡{B_ATK_NAME_WITH_PREFIX} usó\n{B_BUFF2}");
static const u8 sText_ExclamationMarkSpa[] = _("!");
static const u8 sText_ExclamationMark2Spa[] = _("!");
static const u8 sText_ExclamationMark3Spa[] = _("!");
static const u8 sText_ExclamationMark4Spa[] = _("!");
static const u8 sText_ExclamationMark5Spa[] = _("!");

static const u8 sText_HP2Spa[] = _("PS");
static const u8 sText_Attack2Spa[] = _("Ataque");
static const u8 sText_Defense2Spa[] = _("Defensa");
static const u8 sText_SpeedSpa[] = _("Velocid.");
static const u8 sText_SpAtk2Spa[] = _("At. Esp.");
static const u8 sText_SpDef2Spa[] = _("Def. Esp.");
static const u8 sText_AccuracySpa[] = _("Presición");
static const u8 sText_EvasivenessSpa[] = _("Evasión");

const u8 *const gStatNamesTable[] = {
    sText_HP2,
    sText_Attack2,
    sText_Defense2,
    sText_Speed,
    sText_SpAtk2,
    sText_SpDef2,
    sText_Accuracy,
    sText_Evasiveness
};

const u8 *const gStatNamesTableSpa[] = {
    sText_HP2Spa,
    sText_Attack2Spa,
    sText_Defense2Spa,
    sText_SpeedSpa,
    sText_SpAtk2Spa,
    sText_SpDef2Spa,
    sText_AccuracySpa,
    sText_EvasivenessSpa,
};

static const u8 sText_PokeblockWasTooSpicy[] = _("was too spicy!"); //
static const u8 sText_PokeblockWasTooDry[] = _("was too dry!");
static const u8 sText_PokeblockWasTooSweet[] = _("was too sweet!");
static const u8 sText_PokeblockWasTooBitter[] = _("was too bitter!");
static const u8 sText_PokeblockWasTooSour[] = _("was too sour!");
//SPANISH
static const u8 sText_PokeblockWasTooSpicySpa[] = _("demasiado picante!"); //
static const u8 sText_PokeblockWasTooDrySpa[] = _("demasiado seca!");
static const u8 sText_PokeblockWasTooSweetSpa[] = _("demasiado dulce!");
static const u8 sText_PokeblockWasTooBitterSpa[] = _("demasiado amarga!");
static const u8 sText_PokeblockWasTooSourSpa[] = _("demasiado ácida!");

const u8 *const gPokeblockWasTooXStringTable[] = {
    sText_PokeblockWasTooSpicy,
    sText_PokeblockWasTooDry,
    sText_PokeblockWasTooSweet,
    sText_PokeblockWasTooBitter,
    sText_PokeblockWasTooSour
};

const u8 *const gPokeblockWasTooXStringTableSpa[] = {
    sText_PokeblockWasTooSpicySpa,
    sText_PokeblockWasTooDrySpa,
    sText_PokeblockWasTooSweetSpa,
    sText_PokeblockWasTooBitterSpa,
    sText_PokeblockWasTooSourSpa,
};

static const u8 sText_PlayerUsedItem[] = _("{B_PLAYER_NAME} used\n{B_LAST_ITEM}!");
static const u8 sText_OldManUsedItem[] = _("The old man used\n{B_LAST_ITEM}!");
static const u8 sText_PokedudeUsedItem[] = _("The Poké Dude used\n{B_LAST_ITEM}!");
static const u8 sText_Trainer1UsedItem[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}\nused {B_LAST_ITEM}!");
static const u8 sText_TrainerBlockedBall[] = _("The Trainer blocked the Ball!");
static const u8 sText_DontBeAThief[] = _("Don't be a thief!");
static const u8 sText_ItDodgedBall[] = _("It dodged the thrown Ball!\nThis Pokémon can't be caught!");
static const u8 sText_YouMissedPkmn[] = _("You missed the Pokémon!");
static const u8 sText_PkmnBrokeFree[] = _("Oh, no!\nThe Pokémon broke free!");
static const u8 sText_ItAppearedCaught[] = _("Aww!\nIt appeared to be caught!");
static const u8 sText_AarghAlmostHadIt[] = _("Aargh!\nAlmost had it!");
static const u8 sText_ShootSoClose[] = _("Shoot!\nIt was so close, too!");
static const u8 gUnknown_83FD78A[] = _("よけられた!\nこいつは つかまりそうにないぞ!");
static const u8 sText_GotchaPkmnCaught[] = _("Gotcha!\n{B_OPPONENT_MON1_NAME} was caught!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}\p");
static const u8 sText_GotchaPkmnCaught2[] = _("Gotcha!\n{B_OPPONENT_MON1_NAME} was caught!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}{PAUSE 127}");
static const u8 sText_GiveNicknameCaptured[] = _("Give a nickname to the\ncaptured {B_OPPONENT_MON1_NAME}?");
static const u8 sText_PkmnSentToPC[] = _("{B_OPPONENT_MON1_NAME} was sent to\n{B_PC_CREATOR_NAME} PC.");
static const u8 sText_Someones[] = _("someone's");
static const u8 sText_Bills[] = _("Bill's");
static const u8 sText_PkmnDataAddedToDex[] = _("{B_OPPONENT_MON1_NAME}'s data was\nadded to the Pokédex.\p");
static const u8 sText_ItIsRaining[] = _("It is raining.");
static const u8 sText_SandstormIsRaging[] = _("A sandstorm is raging.");
static const u8 sText_BoxIsFull[] = _("The Box is full!\nYou can't catch any more!\p");
static const u8 sText_EnigmaBerry[] = _("Enigma Berry");
static const u8 sText_BerrySuffix[] = _(" Berry");
static const u8 gUnknown_83FD8B6[] = _("ナゾ");
static const u8 sText_PkmnsItemCuredParalysis[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ncured paralysis!");
static const u8 sText_PkmnsItemCuredPoison[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ncured poison!");
static const u8 sText_PkmnsItemHealedBurn[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nhealed its burn!");
static const u8 sText_PkmnsItemDefrostedIt[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ndefrosted it!");
static const u8 sText_PkmnsItemWokeIt[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nwoke it from its sleep!");
static const u8 sText_PkmnsItemSnappedOut[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nsnapped it out of confusion!");
static const u8 sText_PkmnsItemCuredProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ncured its {B_BUFF1} problem!");
static const u8 sText_PkmnsItemNormalizedStatus[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nnormalized its status!");
static const u8 sText_PkmnsItemRestoredHealth[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nrestored health!");
static const u8 sText_PkmnsItemRestoredPP[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nrestored {B_BUFF1}'s PP!");
static const u8 sText_PkmnsItemRestoredStatus[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nrestored its status!");
static const u8 sText_PkmnsItemRestoredHPALittle[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nrestored its HP a little!");
static const u8 sText_ItemAllowsOnlyYMove[] = _("{B_LAST_ITEM}'s effect allows only\n{B_CURRENT_MOVE} to be used!\p");
static const u8 sText_PkmnHungOnWithX[] = _("{B_DEF_NAME_WITH_PREFIX} hung on\nusing its {B_LAST_ITEM}!");
const u8 gText_EmptyString3[] = _("");
static const u8 sText_PlayedFluteCatchyTune[] = _("{B_PLAYER_NAME} played the {B_LAST_ITEM}.\pNow, that's a catchy tune!");
static const u8 sText_PlayedThe[] = _("{B_PLAYER_NAME} played the\n{B_LAST_ITEM}.");
static const u8 sText_PkmnHearingFluteAwoke[] = _("The Pokémon hearing the Flute\nawoke!");
static const u8 sText_YouThrowABallNowRight[] = _("You throw a Ball now, right?\nI… I'll do my best!");

const u8 gText_ForPetesSake[] = _("Oak: Oh, for Pete's sake…\nSo pushy, as always.\p{B_PLAYER_NAME}.\pYou've never had a Pokémon battle\nbefore, have you?\pA Pokémon battle is when Trainers\npit their Pokémon against each\lother.\p");
const u8 gText_TheTrainerThat[] = _("The Trainer that makes the other\nTRAINER's Pokémon faint by lowering\ltheir HP to “0,” wins.\p");
const u8 gText_TryBattling[] = _("But rather than talking about it,\nyou'll learn more from experience.\pTry battling and see for yourself.\p");
const u8 gText_InflictingDamageIsKey[] = _("Oak: Inflicting damage on the foe\nis the key to any battle.\p");
const u8 gText_LoweringStats[] = _("Oak: Lowering the foe's stats\nwill put you at an advantage.\p");
const u8 gText_KeepAnEyeOnHP[] = _("Oak: Keep your eyes on your\nPokémon's HP.\pIt will faint if the HP drops to\n“0.”\p");
const u8 gText_OakNoRunningFromATrainer[] = _("Oak: No! There's no running away\nfrom a Trainer Pokémon battle!\p");
const u8 gText_WinEarnsPrizeMoney[] = _("Oak: Hm! Excellent!\pIf you win, you earn prize money,\nand your Pokémon will grow!\pBattle other Trainers and make\nyour Pokémon strong!\p");
const u8 gText_HowDissapointing[] = _("Oak: Hm…\nHow disappointing…\pIf you win, you earn prize money,\nand your Pokémon grow.\pBut if you lose, {B_PLAYER_NAME}, you end\nup paying prize money…\pHowever, since you had no warning\nthis time, I'll pay for you.\pBut things won't be this way once\nyou step outside these doors.\pThat's why you must strengthen your\nPokémon by battling wild Pokémon.\p");
//SPANISH
static const u8 sText_PlayerUsedItemSpa[] = _("¡{B_PLAYER_NAME} usó\n{B_LAST_ITEM}!");
static const u8 sText_OldManUsedItemSpa[] = _("El señor mayor usó\nuna {B_LAST_ITEM}.");
static const u8 sText_PokedudeUsedItemSpa[] = _("El Pokélotodo usó\nuna {B_LAST_ITEM}.");
static const u8 sText_Trainer1UsedItemSpa[] = _("¡Uso de {B_LAST_ITEM} por\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");
static const u8 sText_TrainerBlockedBallSpa[] = _("¡El Entrenador bloqueó la Ball!");
static const u8 sText_DontBeAThiefSpa[] = _("¡Robar está mal!");
static const u8 sText_ItDodgedBallSpa[] = _("¡Esquivó la Ball! ¡No has podido\natrapar a este Pokémon!");
static const u8 sText_YouMissedPkmnSpa[] = _("¡Has apuntado mal al Pokémon!");
static const u8 sText_PkmnBrokeFreeSpa[] = _("¡Oh, no!\n¡El Pokémon se ha escapado!");
static const u8 sText_ItAppearedCaughtSpa[] = _("¡Vaya! ¡Parecía que lo\nhabías atrapado!");
static const u8 sText_AarghAlmostHadItSpa[] = _("¡Qué pena!\n¡Te faltó poco!");
static const u8 sText_ShootSoCloseSpa[] = _("¡Uy!\n¡Casi lo consigues!");
static const u8 gUnknown_83FD78ASpa[] = _("よけられた!\nこいつは つかまりそうにないぞ!");
static const u8 sText_GotchaPkmnCaughtSpa[] = _("¡Ya está!\n¡{B_OPPONENT_MON1_NAME} atrapado!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}\p");
static const u8 sText_GotchaPkmnCaught2Spa[] = _("¡Ya está!\n¡{B_OPPONENT_MON1_NAME} atrapado!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}{PAUSE 127}");
static const u8 sText_GiveNicknameCapturedSpa[] = _("¿Quieres ponerle un \nmote a {B_OPPONENT_MON1_NAME}?");
static const u8 sText_PkmnSentToPCSpa[] = _("{B_OPPONENT_MON1_NAME} fue enviado \nal PC de {B_PC_CREATOR_NAME}.");
static const u8 sText_SomeonesSpa[] = _("alguien");
static const u8 sText_BillsSpa[] = _("Bill");
static const u8 sText_PkmnDataAddedToDexSpa[] = _("Registrados en la Pokédex\nlos datos de {B_OPPONENT_MON1_NAME}.\p");
static const u8 sText_ItIsRainingSpa[] = _("Está lloviendo...");
static const u8 sText_SandstormIsRagingSpa[] = _("La tormenta de arena cobra fuerza...");
static const u8 sText_BoxIsFullSpa[] = _("¡Las Cajas están llenas!\n¡No puedes atrapar ninguno más!\p");
static const u8 sText_EnigmaBerrySpa[] = _("Baya Enigma");
static const u8 sText_BerrySuffixSpa[] = _("Baya ");
static const u8 gUnknown_83FD8B6Spa[] = _("ナゾ");
static const u8 sText_PkmnsItemCuredParalysisSpa[] = _("¡({B_SCR_ACTIVE_NAME_WITH_PREFIX} se curó de la\nparálisis con {B_LAST_ITEM}!");
static const u8 sText_PkmnsItemCuredPoisonSpa[] = _("¡({B_SCR_ACTIVE_NAME_WITH_PREFIX} se curó del\nenvenenamiento con {B_LAST_ITEM}!");
static const u8 sText_PkmnsItemHealedBurnSpa[] = _("¡({B_SCR_ACTIVE_NAME_WITH_PREFIX} se curó las \nquemaduras con {B_LAST_ITEM}!");
static const u8 sText_PkmnsItemDefrostedItSpa[] = _("¡({B_SCR_ACTIVE_NAME_WITH_PREFIX} se descongeló\ncon {B_LAST_ITEM}!");
static const u8 sText_PkmnsItemWokeItSpa[] = _("¡({B_SCR_ACTIVE_NAME_WITH_PREFIX} se despertó\ncon {B_LAST_ITEM}!");
static const u8 sText_PkmnsItemSnappedOutSpa[] = _("¡({B_SCR_ACTIVE_NAME_WITH_PREFIX} se libró de la\nconfusión con {B_LAST_ITEM}!");
static const u8 sText_PkmnsItemCuredProblemSpa[] = _("¡{B_LAST_ITEM} de ({B_SCR_ACTIVE_NAME_WITH_PREFIX}\ncuró su problema de {B_BUFF1}!");
static const u8 sText_PkmnsItemNormalizedStatusSpa[] = _("¡({B_SCR_ACTIVE_NAME_WITH_PREFIX} normalizó su\nestado con {B_LAST_ITEM}!");
static const u8 sText_PkmnsItemRestoredHealthSpa[] = _("¡({B_SCR_ACTIVE_NAME_WITH_PREFIX} restauró \nsalud con {B_LAST_ITEM}!");
static const u8 sText_PkmnsItemRestoredPPSpa[] = _("¡{B_LAST_ITEM} de ({B_SCR_ACTIVE_NAME_WITH_PREFIX}\nrestauró los PP de {B_BUFF1}!");
static const u8 sText_PkmnsItemRestoredStatusSpa[] = _("¡{B_LAST_ITEM} de ({B_SCR_ACTIVE_NAME_WITH_PREFIX}\nrestauró sus características!");
static const u8 sText_PkmnsItemRestoredHPALittleSpa[] = _("¡({B_SCR_ACTIVE_NAME_WITH_PREFIX} restauró algo\nsus PS con {B_LAST_ITEM}!");
static const u8 sText_ItemAllowsOnlyYMoveSpa[] = _("¡{B_LAST_ITEM} sólo permite el\nuso de {B_CURRENT_MOVE}!\p");
static const u8 sText_PkmnHungOnWithXSpa[] = _("¡({B_DEF_NAME_WITH_PREFIX} resistió\nusando su {B_LAST_ITEM}!");
const u8 gText_EmptyString3Spa[] = _("");
static const u8 sText_PlayedFluteCatchyTuneSpa[] = _("{B_PLAYER_NAME} ha tocado la {B_LAST_ITEM}.\p¡Es una melodía muy pegadiza!");
static const u8 sText_PlayedTheSpa[] = _("{B_PLAYER_NAME} tocó la {B_LAST_ITEM}.");
static const u8 sText_PkmnHearingFluteAwokeSpa[] = _("¡El Pokémon se ha despertado\nal oír la Flauta!");
static const u8 sText_YouThrowABallNowRightSpa[] = _("Ahora debo lanzar una Ball, ¿no?\n¡Intentaré hacerlo lo mejor posible!");

const u8 gText_ForPetesSakeSpa[] = _("Oak: ¡Pero bueno!...\n¡Qué impaciente es este chico!\p{B_PLAYER_NAME}, no te asustes.\pTú nunca has participado\nen un combate Pokémon, ¿no?\pEn ellos se enfrentan Entrenadores\ncon sus Pokémon.\p");
const u8 gText_TheTrainerThatSpa[] = _("Y gana el Entrenador que consiga\ndebilitar los Pokémon de su rival\lreduciendo sus PS a 0.\p");
const u8 gText_TryBattlingSpa[] = _("Bueno, no me enrollo más.\nLo mejor será que luches\ly aprendas con la práctica.\p¡Venga, que no se diga!\p");
const u8 gText_InflictingDamageIsKeySpa[] = _("Oak: La clave está en causar daño.\p");
const u8 gText_LoweringStatsSpa[] = _("Oak: Si reduces las características\nde tu rival, lo tendrás más fácil.\p");
const u8 gText_KeepAnEyeOnHPSpa[] = _("Oak: No pierdas de vista los PS\nde tu Pokémon.\pSi llegan a 0, se debilitará.\p");
const u8 gText_OakNoRunningFromATrainerSpa[] = _("Oak: ¡Eh! ¡No puedes escapar\ndel combate con un Entrenador!\p");
const u8 gText_WinEarnsPrizeMoneySpa[] = _("Oak: ¡Muy bien! ¡Excelente!\pSi vences, ganas dinero;\ny tu Pokémon, experiencia.\p¡Lucha con otros Entrenadores\npara hacerte más fuerte todavía!\p");
const u8 gText_HowDissapointingSpa[] = _("Oak: Oh...\nVaya chasco...\pSi vences, ganas dinero;\ny tu Pokémon, experiencia.\pPero si te derrotan, {B_PLAYER_NAME},\ndebes pagar una cantidad...\pBueno, como es la primera vez\ny no lo sabías, yo pagaré por ti.\pPero piensa que las cosas no\nson así fuera del laboratorio.\pDebes luchar con muchos Pokémon\nsalvajes; debes hacerte fuerte.\p");

const u8 *const gBattleStringsTable[] = {
    [STRINGID_TRAINER1LOSETEXT - 12]              = sText_Trainer1LoseText,
    [STRINGID_PKMNGAINEDEXP - 12]                 = sText_PkmnGainedEXP,
    [STRINGID_PKMNGREWTOLV - 12]                  = sText_PkmnGrewToLv,
    [STRINGID_PKMNLEARNEDMOVE - 12]               = sText_PkmnLearnedMove,
    [STRINGID_TRYTOLEARNMOVE1 - 12]               = sText_TryToLearnMove1,
    [STRINGID_TRYTOLEARNMOVE2 - 12]               = sText_TryToLearnMove2,
    [STRINGID_TRYTOLEARNMOVE3 - 12]               = sText_TryToLearnMove3,
    [STRINGID_PKMNFORGOTMOVE - 12]                = sText_PkmnForgotMove,
    [STRINGID_STOPLEARNINGMOVE - 12]              = sText_StopLearningMove,
    [STRINGID_DIDNOTLEARNMOVE - 12]               = sText_DidNotLearnMove,
    [STRINGID_PKMNLEARNEDMOVE2 - 12]              = sText_PkmnLearnedMove2,
    [STRINGID_ATTACKMISSED - 12]                  = sText_AttackMissed,
    [STRINGID_PKMNPROTECTEDITSELF - 12]           = sText_PkmnProtectedItself,
    [STRINGID_STATSWONTINCREASE2 - 12]            = sText_StatsWontIncrease2,
    [STRINGID_AVOIDEDDAMAGE - 12]                 = sText_AvoidedDamage,
    [STRINGID_ITDOESNTAFFECT - 12]                = sText_ItDoesntAffect,
    [STRINGID_ATTACKERFAINTED - 12]               = sText_AttackerFainted,
    [STRINGID_TARGETFAINTED - 12]                 = sText_TargetFainted,
    [STRINGID_PLAYERGOTMONEY - 12]                = sText_PlayerGotMoney,
    [STRINGID_PLAYERWHITEOUT - 12]                = sText_PlayerWhiteout,
    [STRINGID_PLAYERWHITEOUT2 - 12]               = sText_PlayerPanicked,
    [STRINGID_PREVENTSESCAPE - 12]                = sText_PreventsEscape,
    [STRINGID_HITXTIMES - 12]                     = sText_HitXTimes,
    [STRINGID_PKMNFELLASLEEP - 12]                = sText_PkmnFellAsleep,
    [STRINGID_PKMNMADESLEEP - 12]                 = sText_PkmnMadeSleep,
    [STRINGID_PKMNALREADYASLEEP - 12]             = sText_PkmnAlreadyAsleep,
    [STRINGID_PKMNALREADYASLEEP2 - 12]            = sText_PkmnAlreadyAsleep2,
    [STRINGID_PKMNWASNTAFFECTED - 12]             = sText_PkmnWasntAffected,
    [STRINGID_PKMNWASPOISONED - 12]               = sText_PkmnWasPoisoned,
    [STRINGID_PKMNPOISONEDBY - 12]                = sText_PkmnPoisonedBy,
    [STRINGID_PKMNHURTBYPOISON - 12]              = sText_PkmnHurtByPoison,
    [STRINGID_PKMNALREADYPOISONED - 12]           = sText_PkmnAlreadyPoisoned,
    [STRINGID_PKMNBADLYPOISONED - 12]             = sText_PkmnBadlyPoisoned,
    [STRINGID_PKMNENERGYDRAINED - 12]             = sText_PkmnEnergyDrained,
    [STRINGID_PKMNWASBURNED - 12]                 = sText_PkmnWasBurned,
    [STRINGID_PKMNBURNEDBY - 12]                  = sText_PkmnBurnedBy,
    [STRINGID_PKMNHURTBYBURN - 12]                = sText_PkmnHurtByBurn,
    [STRINGID_PKMNWASFROZEN - 12]                 = sText_PkmnWasFrozen,
    [STRINGID_PKMNFROZENBY - 12]                  = sText_PkmnFrozenBy,
    [STRINGID_PKMNISFROZEN - 12]                  = sText_PkmnIsFrozen,
    [STRINGID_PKMNWASDEFROSTED - 12]              = sText_PkmnWasDefrosted,
    [STRINGID_PKMNWASDEFROSTED2 - 12]             = sText_PkmnWasDefrosted2,
    [STRINGID_PKMNWASDEFROSTEDBY - 12]            = sText_PkmnWasDefrostedBy,
    [STRINGID_PKMNWASPARALYZED - 12]              = sText_PkmnWasParalyzed,
    [STRINGID_PKMNWASPARALYZEDBY - 12]            = sText_PkmnWasParalyzedBy,
    [STRINGID_PKMNISPARALYZED - 12]               = sText_PkmnIsParalyzed,
    [STRINGID_PKMNISALREADYPARALYZED - 12]        = sText_PkmnIsAlreadyParalyzed,
    [STRINGID_PKMNHEALEDPARALYSIS - 12]           = sText_PkmnHealedParalysis,
    [STRINGID_PKMNDREAMEATEN - 12]                = sText_PkmnDreamEaten,
    [STRINGID_STATSWONTINCREASE - 12]             = sText_StatsWontIncrease,
    [STRINGID_STATSWONTDECREASE - 12]             = sText_StatsWontDecrease,
    [STRINGID_TEAMSTOPPEDWORKING - 12]            = sText_TeamStoppedWorking,
    [STRINGID_FOESTOPPEDWORKING - 12]             = sText_FoeStoppedWorking,
    [STRINGID_PKMNISCONFUSED - 12]                = sText_PkmnIsConfused,
    [STRINGID_PKMNHEALEDCONFUSION - 12]           = sText_PkmnHealedConfusion,
    [STRINGID_PKMNWASCONFUSED - 12]               = sText_PkmnWasConfused,
    [STRINGID_PKMNALREADYCONFUSED - 12]           = sText_PkmnAlreadyConfused,
    [STRINGID_PKMNFELLINLOVE - 12]                = sText_PkmnFellInLove,
    [STRINGID_PKMNINLOVE - 12]                    = sText_PkmnInLove,
    [STRINGID_PKMNIMMOBILIZEDBYLOVE - 12]         = sText_PkmnImmobilizedByLove,
    [STRINGID_PKMNBLOWNAWAY - 12]                 = sText_PkmnBlownAway,
    [STRINGID_PKMNCHANGEDTYPE - 12]               = sText_PkmnChangedType,
    [STRINGID_PKMNFLINCHED - 12]                  = sText_PkmnFlinched,
    [STRINGID_PKMNREGAINEDHEALTH - 12]            = sText_PkmnRegainedHealth,
    [STRINGID_PKMNHPFULL - 12]                    = sText_PkmnHPFull,
    [STRINGID_PKMNRAISEDSPDEF - 12]               = sText_PkmnRaisedSpDef,
    [STRINGID_PKMNRAISEDDEF - 12]                 = sText_PkmnRaisedDef,
    [STRINGID_PKMNCOVEREDBYVEIL - 12]             = sText_PkmnCoveredByVeil,
    [STRINGID_PKMNUSEDSAFEGUARD - 12]             = sText_PkmnUsedSafeguard,
    [STRINGID_PKMNSAFEGUARDEXPIRED - 12]          = sText_PkmnSafeguardExpired,
    [STRINGID_PKMNWENTTOSLEEP - 12]               = sText_PkmnWentToSleep,
    [STRINGID_PKMNSLEPTHEALTHY - 12]              = sText_PkmnSleptHealthy,
    [STRINGID_PKMNWHIPPEDWHIRLWIND - 12]          = sText_PkmnWhippedWhirlwind,
    [STRINGID_PKMNTOOKSUNLIGHT - 12]              = sText_PkmnTookSunlight,
    [STRINGID_PKMNLOWEREDHEAD - 12]               = sText_PkmnLoweredHead,
    [STRINGID_PKMNISGLOWING - 12]                 = sText_PkmnIsGlowing,
    [STRINGID_PKMNFLEWHIGH - 12]                  = sText_PkmnFlewHigh,
    [STRINGID_PKMNDUGHOLE - 12]                   = sText_PkmnDugHole,
    [STRINGID_PKMNSQUEEZEDBYBIND - 12]            = sText_PkmnSqueezedByBind,
    [STRINGID_PKMNTRAPPEDINVORTEX - 12]           = sText_PkmnTrappedInVortex,
    [STRINGID_PKMNWRAPPEDBY - 12]                 = sText_PkmnWrappedBy,
    [STRINGID_PKMNCLAMPED - 12]                   = sText_PkmnClamped,
    [STRINGID_PKMNHURTBY - 12]                    = sText_PkmnHurtBy,
    [STRINGID_PKMNFREEDFROM - 12]                 = sText_PkmnFreedFrom,
    [STRINGID_PKMNCRASHED - 12]                   = sText_PkmnCrashed,
    [STRINGID_PKMNSHROUDEDINMIST - 12]            = gBattleText_MistShroud,
    [STRINGID_PKMNPROTECTEDBYMIST - 12]           = sText_PkmnProtectedByMist,
    [STRINGID_PKMNGETTINGPUMPED - 12]             = gBattleText_GetPumped,
    [STRINGID_PKMNHITWITHRECOIL - 12]             = sText_PkmnHitWithRecoil,
    [STRINGID_PKMNPROTECTEDITSELF2 - 12]          = sText_PkmnProtectedItself2,
    [STRINGID_PKMNBUFFETEDBYSANDSTORM - 12]       = sText_PkmnBuffetedBySandstorm,
    [STRINGID_PKMNPELTEDBYHAIL - 12]              = sText_PkmnPeltedByHail,
    [STRINGID_PKMNSEEDED - 12]                    = sText_PkmnSeeded,
    [STRINGID_PKMNEVADEDATTACK - 12]              = sText_PkmnEvadedAttack,
    [STRINGID_PKMNSAPPEDBYLEECHSEED - 12]         = sText_PkmnSappedByLeechSeed,
    [STRINGID_PKMNFASTASLEEP - 12]                = sText_PkmnFastAsleep,
    [STRINGID_PKMNWOKEUP - 12]                    = sText_PkmnWokeUp,
    [STRINGID_PKMNUPROARKEPTAWAKE - 12]           = sText_PkmnUproarKeptAwake,
    [STRINGID_PKMNWOKEUPINUPROAR - 12]            = sText_PkmnWokeUpInUproar,
    [STRINGID_PKMNCAUSEDUPROAR - 12]              = sText_PkmnCausedUproar,
    [STRINGID_PKMNMAKINGUPROAR - 12]              = sText_PkmnMakingUproar,
    [STRINGID_PKMNCALMEDDOWN - 12]                = sText_PkmnCalmedDown,
    [STRINGID_PKMNCANTSLEEPINUPROAR - 12]         = sText_PkmnCantSleepInUproar,
    [STRINGID_PKMNSTOCKPILED - 12]                = sText_PkmnStockpiled,
    [STRINGID_PKMNCANTSTOCKPILE - 12]             = sText_PkmnCantStockpile,
    [STRINGID_PKMNCANTSLEEPINUPROAR2 - 12]        = sText_PkmnCantSleepInUproar2,
    [STRINGID_UPROARKEPTPKMNAWAKE - 12]           = sText_UproarKeptPkmnAwake,
    [STRINGID_PKMNSTAYEDAWAKEUSING - 12]          = sText_PkmnStayedAwakeUsing,
    [STRINGID_PKMNSTORINGENERGY - 12]             = sText_PkmnStoringEnergy,
    [STRINGID_PKMNUNLEASHEDENERGY - 12]           = sText_PkmnUnleashedEnergy,
    [STRINGID_PKMNFATIGUECONFUSION - 12]          = sText_PkmnFatigueConfusion,
    [STRINGID_PKMNPICKEDUPITEM - 12]              = sText_PkmnPickedUpItem,
    [STRINGID_PKMNUNAFFECTED - 12]                = sText_PkmnUnaffected,
    [STRINGID_PKMNTRANSFORMEDINTO - 12]           = sText_PkmnTransformedInto,
    [STRINGID_PKMNMADESUBSTITUTE - 12]            = sText_PkmnMadeSubstitute,
    [STRINGID_PKMNHASSUBSTITUTE - 12]             = sText_PkmnHasSubstitute,
    [STRINGID_SUBSTITUTEDAMAGED - 12]             = sText_SubstituteDamaged,
    [STRINGID_PKMNSUBSTITUTEFADED - 12]           = sText_PkmnSubstituteFaded,
    [STRINGID_PKMNMUSTRECHARGE - 12]              = sText_PkmnMustRecharge,
    [STRINGID_PKMNRAGEBUILDING - 12]              = sText_PkmnRageBuilding,
    [STRINGID_PKMNMOVEWASDISABLED - 12]           = sText_PkmnMoveWasDisabled,
    [STRINGID_PKMNMOVEISDISABLED - 12]            = sText_PkmnMoveIsDisabled,
    [STRINGID_PKMNMOVEDISABLEDNOMORE - 12]        = sText_PkmnMoveDisabledNoMore,
    [STRINGID_PKMNGOTENCORE - 12]                 = sText_PkmnGotEncore,
    [STRINGID_PKMNENCOREENDED - 12]               = sText_PkmnEncoreEnded,
    [STRINGID_PKMNTOOKAIM - 12]                   = sText_PkmnTookAim,
    [STRINGID_PKMNSKETCHEDMOVE - 12]              = sText_PkmnSketchedMove,
    [STRINGID_PKMNTRYINGTOTAKEFOE - 12]           = sText_PkmnTryingToTakeFoe,
    [STRINGID_PKMNTOOKFOE - 12]                   = sText_PkmnTookFoe,
    [STRINGID_PKMNREDUCEDPP - 12]                 = sText_PkmnReducedPP,
    [STRINGID_PKMNSTOLEITEM - 12]                 = sText_PkmnStoleItem,
    [STRINGID_TARGETCANTESCAPENOW - 12]           = sText_TargetCantEscapeNow,
    [STRINGID_PKMNFELLINTONIGHTMARE - 12]         = sText_PkmnFellIntoNightmare,
    [STRINGID_PKMNLOCKEDINNIGHTMARE - 12]         = sText_PkmnLockedInNightmare,
    [STRINGID_PKMNLAIDCURSE - 12]                 = sText_PkmnLaidCurse,
    [STRINGID_PKMNAFFLICTEDBYCURSE - 12]          = sText_PkmnAfflictedByCurse,
    [STRINGID_SPIKESSCATTERED - 12]               = sText_SpikesScattered,
    [STRINGID_PKMNHURTBYSPIKES - 12]              = sText_PkmnHurtBySpikes,
    [STRINGID_PKMNIDENTIFIED - 12]                = sText_PkmnIdentified,
    [STRINGID_PKMNPERISHCOUNTFELL - 12]           = sText_PkmnPerishCountFell,
    [STRINGID_PKMNBRACEDITSELF - 12]              = sText_PkmnBracedItself,
    [STRINGID_PKMNENDUREDHIT - 12]                = sText_PkmnEnduredHit,
    [STRINGID_MAGNITUDESTRENGTH - 12]             = sText_MagnitudeStrength,
    [STRINGID_PKMNCUTHPMAXEDATTACK - 12]          = sText_PkmnCutHPMaxedAttack,
    [STRINGID_PKMNCOPIEDSTATCHANGES - 12]         = sText_PkmnCopiedStatChanges,
    [STRINGID_PKMNGOTFREE - 12]                   = sText_PkmnGotFree,
    [STRINGID_PKMNSHEDLEECHSEED - 12]             = sText_PkmnShedLeechSeed,
    [STRINGID_PKMNBLEWAWAYSPIKES - 12]            = sText_PkmnBlewAwaySpikes,
    [STRINGID_PKMNFLEDFROMBATTLE - 12]            = sText_PkmnFledFromBattle,
    [STRINGID_PKMNFORESAWATTACK - 12]             = sText_PkmnForesawAttack,
    [STRINGID_PKMNTOOKATTACK - 12]                = sText_PkmnTookAttack,
    [STRINGID_PKMNATTACK - 12]                    = sText_PkmnAttack,
    [STRINGID_PKMNCENTERATTENTION - 12]           = sText_PkmnCenterAttention,
    [STRINGID_PKMNCHARGINGPOWER - 12]             = sText_PkmnChargingPower,
    [STRINGID_NATUREPOWERTURNEDINTO - 12]         = sText_NaturePowerTurnedInto,
    [STRINGID_PKMNSTATUSNORMAL - 12]              = sText_PkmnStatusNormal,
    [STRINGID_PKMNHASNOMOVESLEFT - 12]            = sText_PkmnHasNoMovesLeft,
    [STRINGID_PKMNSUBJECTEDTOTORMENT - 12]        = sText_PkmnSubjectedToTorment,
    [STRINGID_PKMNCANTUSEMOVETORMENT - 12]        = sText_PkmnCantUseMoveTorment,
    [STRINGID_PKMNTIGHTENINGFOCUS - 12]           = sText_PkmnTighteningFocus,
    [STRINGID_PKMNFELLFORTAUNT - 12]              = sText_PkmnFellForTaunt,
    [STRINGID_PKMNCANTUSEMOVETAUNT - 12]          = sText_PkmnCantUseMoveTaunt,
    [STRINGID_PKMNREADYTOHELP - 12]               = sText_PkmnReadyToHelp,
    [STRINGID_PKMNSWITCHEDITEMS - 12]             = sText_PkmnSwitchedItems,
    [STRINGID_PKMNCOPIEDFOE - 12]                 = sText_PkmnCopiedFoe,
    [STRINGID_PKMNMADEWISH - 12]                  = sText_PkmnMadeWish,
    [STRINGID_PKMNWISHCAMETRUE - 12]              = sText_PkmnWishCameTrue,
    [STRINGID_PKMNPLANTEDROOTS - 12]              = sText_PkmnPlantedRoots,
    [STRINGID_PKMNABSORBEDNUTRIENTS - 12]         = sText_PkmnAbsorbedNutrients,
    [STRINGID_PKMNANCHOREDITSELF - 12]            = sText_PkmnAnchoredItself,
    [STRINGID_PKMNWASMADEDROWSY - 12]             = sText_PkmnWasMadeDrowsy,
    [STRINGID_PKMNKNOCKEDOFF - 12]                = sText_PkmnKnockedOff,
    [STRINGID_PKMNSWAPPEDABILITIES - 12]          = sText_PkmnSwappedAbilities,
    [STRINGID_PKMNSEALEDOPPONENTMOVE - 12]        = sText_PkmnSealedOpponentMove,
    [STRINGID_PKMNCANTUSEMOVESEALED - 12]         = sText_PkmnCantUseMoveSealed,
    [STRINGID_PKMNWANTSGRUDGE - 12]               = sText_PkmnWantsGrudge,
    [STRINGID_PKMNLOSTPPGRUDGE - 12]              = sText_PkmnLostPPGrudge,
    [STRINGID_PKMNSHROUDEDITSELF - 12]            = sText_PkmnShroudedItself,
    [STRINGID_PKMNMOVEBOUNCED - 12]               = sText_PkmnMoveBounced,
    [STRINGID_PKMNWAITSFORTARGET - 12]            = sText_PkmnWaitsForTarget,
    [STRINGID_PKMNSNATCHEDMOVE - 12]              = sText_PkmnSnatchedMove,
    [STRINGID_PKMNMADEITRAIN - 12]                = sText_PkmnMadeItRain,
    [STRINGID_PKMNRAISEDSPEED - 12]               = sText_PkmnRaisedSpeed,
    [STRINGID_PKMNPROTECTEDBY - 12]               = sText_PkmnProtectedBy,
    [STRINGID_PKMNPREVENTSUSAGE - 12]             = sText_PkmnPreventsUsage,
    [STRINGID_PKMNRESTOREDHPUSING - 12]           = sText_PkmnRestoredHPUsing,
    [STRINGID_PKMNCHANGEDTYPEWITH - 12]           = sText_PkmnChangedTypeWith,
    [STRINGID_PKMNPREVENTSPARALYSISWITH - 12]     = sText_PkmnPreventsParalysisWith,
    [STRINGID_PKMNPREVENTSROMANCEWITH - 12]       = sText_PkmnPreventsRomanceWith,
    [STRINGID_PKMNPREVENTSPOISONINGWITH - 12]     = sText_PkmnPreventsPoisoningWith,
    [STRINGID_PKMNPREVENTSCONFUSIONWITH - 12]     = sText_PkmnPreventsConfusionWith,
    [STRINGID_PKMNRAISEDFIREPOWERWITH - 12]       = sText_PkmnRaisedFirePowerWith,
    [STRINGID_PKMNANCHORSITSELFWITH - 12]         = sText_PkmnAnchorsItselfWith,
    [STRINGID_PKMNCUTSATTACKWITH - 12]            = sText_PkmnCutsAttackWith,
    [STRINGID_PKMNPREVENTSSTATLOSSWITH - 12]      = sText_PkmnPreventsStatLossWith,
    [STRINGID_PKMNHURTSWITH - 12]                 = sText_PkmnHurtsWith,
    [STRINGID_PKMNTRACED - 12]                    = sText_PkmnTraced,
    [STRINGID_STATSHARPLY - 12]                   = sText_StatSharply,
    [STRINGID_STATROSE - 12]                      = gBattleText_Rose,
    [STRINGID_STATHARSHLY - 12]                   = sText_StatHarshly,
    [STRINGID_STATFELL - 12]                      = sText_StatFell,
    [STRINGID_PKMNSSTATCHANGED - 12]              = sText_PkmnsStatChanged,
    [STRINGID_PKMNSSTATCHANGED2 - 12]             = gText_PkmnsStatChanged2,
    [STRINGID_PKMNSSTATCHANGED3 - 12]             = sText_PkmnsStatChanged3,
    [STRINGID_PKMNSSTATCHANGED4 - 12]             = sText_PkmnsStatChanged4,
    [STRINGID_CRITICALHIT - 12]                   = sText_CriticalHit,
    [STRINGID_ONEHITKO - 12]                      = sText_OneHitKO,
    [STRINGID_123POOF - 12]                       = sText_123Poof,
    [STRINGID_ANDELLIPSIS - 12]                   = sText_AndEllipsis,
    [STRINGID_NOTVERYEFFECTIVE - 12]              = sText_NotVeryEffective,
    [STRINGID_SUPEREFFECTIVE - 12]                = sText_SuperEffective,
    [STRINGID_GOTAWAYSAFELY - 12]                 = sText_GotAwaySafely,
    [STRINGID_WILDPKMNFLED - 12]                  = sText_WildPkmnFled,
    [STRINGID_NORUNNINGFROMTRAINERS - 12]         = sText_NoRunningFromTrainers,
    [STRINGID_CANTESCAPE - 12]                    = sText_CantEscape,
    [STRINGID_DONTLEAVEBIRCH - 12]                = sText_EmptyString5,
    [STRINGID_BUTNOTHINGHAPPENED - 12]            = sText_ButNothingHappened,
    [STRINGID_BUTITFAILED - 12]                   = sText_ButItFailed,
    [STRINGID_ITHURTCONFUSION - 12]               = sText_ItHurtConfusion,
    [STRINGID_MIRRORMOVEFAILED - 12]              = sText_MirrorMoveFailed,
    [STRINGID_STARTEDTORAIN - 12]                 = sText_StartedToRain,
    [STRINGID_DOWNPOURSTARTED - 12]               = sText_DownpourStarted,
    [STRINGID_RAINCONTINUES - 12]                 = sText_RainContinues,
    [STRINGID_DOWNPOURCONTINUES - 12]             = sText_DownpourContinues,
    [STRINGID_RAINSTOPPED - 12]                   = sText_RainStopped,
    [STRINGID_SANDSTORMBREWED - 12]               = sText_SandstormBrewed,
    [STRINGID_SANDSTORMRAGES - 12]                = sText_SandstormRages,
    [STRINGID_SANDSTORMSUBSIDED - 12]             = sText_SandstormSubsided,
    [STRINGID_SUNLIGHTGOTBRIGHT - 12]             = sText_SunlightGotBright,
    [STRINGID_SUNLIGHTSTRONG - 12]                = sText_SunlightStrong,
    [STRINGID_SUNLIGHTFADED - 12]                 = sText_SunlightFaded,
    [STRINGID_STARTEDHAIL - 12]                   = sText_StartedHail,
    [STRINGID_HAILCONTINUES - 12]                 = sText_HailContinues,
    [STRINGID_HAILSTOPPED - 12]                   = sText_HailStopped,
    [STRINGID_FAILEDTOSPITUP - 12]                = sText_FailedToSpitUp,
    [STRINGID_FAILEDTOSWALLOW - 12]               = sText_FailedToSwallow,
    [STRINGID_WINDBECAMEHEATWAVE - 12]            = sText_WindBecameHeatWave,
    [STRINGID_STATCHANGESGONE - 12]               = sText_StatChangesGone,
    [STRINGID_COINSSCATTERED - 12]                = sText_CoinsScattered,
    [STRINGID_TOOWEAKFORSUBSTITUTE - 12]          = sText_TooWeakForSubstitute,
    [STRINGID_SHAREDPAIN - 12]                    = sText_SharedPain,
    [STRINGID_BELLCHIMED - 12]                    = sText_BellChimed,
    [STRINGID_FAINTINTHREE - 12]                  = sText_FaintInThree,
    [STRINGID_NOPPLEFT - 12]                      = sText_NoPPLeft,
    [STRINGID_BUTNOPPLEFT - 12]                   = sText_ButNoPPLeft,
    [STRINGID_PLAYERUSEDITEM - 12]                = sText_PlayerUsedItem,
    [STRINGID_OLDMANUSEDITEM - 12]                = sText_OldManUsedItem,
    [STRINGID_TRAINERBLOCKEDBALL - 12]            = sText_TrainerBlockedBall,
    [STRINGID_DONTBEATHIEF - 12]                  = sText_DontBeAThief,
    [STRINGID_ITDODGEDBALL - 12]                  = sText_ItDodgedBall,
    [STRINGID_YOUMISSEDPKMN - 12]                 = sText_YouMissedPkmn,
    [STRINGID_PKMNBROKEFREE - 12]                 = sText_PkmnBrokeFree,
    [STRINGID_ITAPPEAREDCAUGHT - 12]              = sText_ItAppearedCaught,
    [STRINGID_AARGHALMOSTHADIT - 12]              = sText_AarghAlmostHadIt,
    [STRINGID_SHOOTSOCLOSE - 12]                  = sText_ShootSoClose,
    [STRINGID_GOTCHAPKMNCAUGHT - 12]              = sText_GotchaPkmnCaught,
    [STRINGID_GOTCHAPKMNCAUGHT2 - 12]             = sText_GotchaPkmnCaught2,
    [STRINGID_GIVENICKNAMECAPTURED - 12]          = sText_GiveNicknameCaptured,
    [STRINGID_PKMNSENTTOPC - 12]                  = sText_PkmnSentToPC,
    [STRINGID_PKMNDATAADDEDTODEX - 12]            = sText_PkmnDataAddedToDex,
    [STRINGID_ITISRAINING - 12]                   = sText_ItIsRaining,
    [STRINGID_SANDSTORMISRAGING - 12]             = sText_SandstormIsRaging,
    [STRINGID_CANTESCAPE2 - 12]                   = sText_CantEscape2,
    [STRINGID_PKMNIGNORESASLEEP - 12]             = sText_PkmnIgnoresAsleep,
    [STRINGID_PKMNIGNOREDORDERS - 12]             = sText_PkmnIgnoredOrders,
    [STRINGID_PKMNBEGANTONAP - 12]                = sText_PkmnBeganToNap,
    [STRINGID_PKMNLOAFING - 12]                   = sText_PkmnLoafing,
    [STRINGID_PKMNWONTOBEY - 12]                  = sText_PkmnWontObey,
    [STRINGID_PKMNTURNEDAWAY - 12]                = sText_PkmnTurnedAway,
    [STRINGID_PKMNPRETENDNOTNOTICE - 12]          = sText_PkmnPretendNotNotice,
    [STRINGID_ENEMYABOUTTOSWITCHPKMN - 12]        = sText_EnemyAboutToSwitchPkmn,
    [STRINGID_THREWROCK - 12]                     = sText_ThrewARock,
    [STRINGID_THREWBAIT - 12]                     = sText_ThrewSomeBait,
    [STRINGID_PKMNWATCHINGCAREFULLY - 12]         = sText_PkmnWatchingCarefully,
    [STRINGID_PKMNANGRY - 12]                     = sText_PkmnIsAngry,
    [STRINGID_PKMNEATING - 12]                    = sText_PkmnIsEating,
    [STRINGID_DUMMY288 - 12]                      = sText_Empty1,
    [STRINGID_DUMMY289 - 12]                      = sText_Empty1,
    [STRINGID_OUTOFSAFARIBALLS - 12]              = sText_OutOfSafariBalls,
    [STRINGID_PKMNSITEMCUREDPARALYSIS - 12]       = sText_PkmnsItemCuredParalysis,
    [STRINGID_PKMNSITEMCUREDPOISON - 12]          = sText_PkmnsItemCuredPoison,
    [STRINGID_PKMNSITEMHEALEDBURN - 12]           = sText_PkmnsItemHealedBurn,
    [STRINGID_PKMNSITEMDEFROSTEDIT - 12]          = sText_PkmnsItemDefrostedIt,
    [STRINGID_PKMNSITEMWOKEIT - 12]               = sText_PkmnsItemWokeIt,
    [STRINGID_PKMNSITEMSNAPPEDOUT - 12]           = sText_PkmnsItemSnappedOut,
    [STRINGID_PKMNSITEMCUREDPROBLEM - 12]         = sText_PkmnsItemCuredProblem,
    [STRINGID_PKMNSITEMRESTOREDHEALTH - 12]       = sText_PkmnsItemRestoredHealth,
    [STRINGID_PKMNSITEMRESTOREDPP - 12]           = sText_PkmnsItemRestoredPP,
    [STRINGID_PKMNSITEMRESTOREDSTATUS - 12]       = sText_PkmnsItemRestoredStatus,
    [STRINGID_PKMNSITEMRESTOREDHPALITTLE - 12]    = sText_PkmnsItemRestoredHPALittle,
    [STRINGID_ITEMALLOWSONLYYMOVE - 12]           = sText_ItemAllowsOnlyYMove,
    [STRINGID_PKMNHUNGONWITHX - 12]               = sText_PkmnHungOnWithX,
    [STRINGID_EMPTYSTRING3 - 12]                  = gText_EmptyString3,
    [STRINGID_PKMNSXPREVENTSBURNS - 12]           = sText_PkmnsXPreventsBurns,
    [STRINGID_PKMNSXBLOCKSY - 12]                 = sText_PkmnsXBlocksY,
    [STRINGID_PKMNSXRESTOREDHPALITTLE2 - 12]      = sText_PkmnsXRestoredHPALittle2,
    [STRINGID_PKMNSXWHIPPEDUPSANDSTORM - 12]      = sText_PkmnsXWhippedUpSandstorm,
    [STRINGID_PKMNSXPREVENTSYLOSS - 12]           = sText_PkmnsXPreventsYLoss,
    [STRINGID_PKMNSXINFATUATEDY - 12]             = sText_PkmnsXInfatuatedY,
    [STRINGID_PKMNSXMADEYINEFFECTIVE - 12]        = sText_PkmnsXMadeYIneffective,
    [STRINGID_PKMNSXCUREDYPROBLEM - 12]           = sText_PkmnsXCuredYProblem,
    [STRINGID_ITSUCKEDLIQUIDOOZE - 12]            = sText_ItSuckedLiquidOoze,
    [STRINGID_PKMNTRANSFORMED - 12]               = sText_PkmnTransformed,
    [STRINGID_ELECTRICITYWEAKENED - 12]           = sText_ElectricityWeakened,
    [STRINGID_FIREWEAKENED - 12]                  = sText_FireWeakened,
    [STRINGID_PKMNHIDUNDERWATER - 12]             = sText_PkmnHidUnderwater,
    [STRINGID_PKMNSPRANGUP - 12]                  = sText_PkmnSprangUp,
    [STRINGID_HMMOVESCANTBEFORGOTTEN - 12]        = sText_HMMovesCantBeForgotten,
    [STRINGID_XFOUNDONEY - 12]                    = sText_XFoundOneY,
    [STRINGID_PLAYERDEFEATEDTRAINER1 - 12]        = sText_PlayerDefeatedLinkTrainerTrainer1,
    [STRINGID_SOOTHINGAROMA - 12]                 = sText_SoothingAroma,
    [STRINGID_ITEMSCANTBEUSEDNOW - 12]            = sText_ItemsCantBeUsedNow,
    [STRINGID_FORXCOMMAYZ - 12]                   = sText_ForXCommaYZ,
    [STRINGID_USINGXTHEYOFZN - 12]                = sText_UsingXTheYOfZN,
    [STRINGID_PKMNUSEDXTOGETPUMPED - 12]          = sText_PkmnUsedXToGetPumped,
    [STRINGID_PKMNSXMADEYUSELESS - 12]            = sText_PkmnsXMadeYUseless,
    [STRINGID_PKMNTRAPPEDBYSANDTOMB - 12]         = sText_PkmnTrappedBySandTomb,
    [STRINGID_EMPTYSTRING4 - 12]                  = sText_EmptyString4,
    [STRINGID_ABOOSTED - 12]                      = sText_ABoosted,
    [STRINGID_PKMNSXINTENSIFIEDSUN - 12]          = sText_PkmnsXIntensifiedSun,
    [STRINGID_PKMNMAKESGROUNDMISS - 12]           = sText_PkmnMakesGroundMiss,
    [STRINGID_YOUTHROWABALLNOWRIGHT - 12]         = sText_YouThrowABallNowRight,
    [STRINGID_PKMNSXTOOKATTACK - 12]              = sText_PkmnsXTookAttack,
    [STRINGID_PKMNCHOSEXASDESTINY - 12]           = sText_PkmnChoseXAsDestiny,
    [STRINGID_PKMNLOSTFOCUS - 12]                 = sText_PkmnLostFocus,
    [STRINGID_USENEXTPKMN - 12]                   = sText_UseNextPkmn,
    [STRINGID_PKMNFLEDUSINGITS - 12]              = sText_PkmnFledUsingIts,
    [STRINGID_PKMNFLEDUSING - 12]                 = sText_PkmnFledUsing,
    [STRINGID_PKMNWASDRAGGEDOUT - 12]             = sText_PkmnWasDraggedOut,
    [STRINGID_PREVENTEDFROMWORKING - 12]          = sText_PreventedFromWorking,
    [STRINGID_PKMNSITEMNORMALIZEDSTATUS - 12]     = sText_PkmnsItemNormalizedStatus,
    [STRINGID_TRAINER1USEDITEM - 12]              = sText_Trainer1UsedItem,
    [STRINGID_BOXISFULL - 12]                     = sText_BoxIsFull,
    [STRINGID_PKMNAVOIDEDATTACK - 12]             = sText_PkmnAvoidedAttack,
    [STRINGID_PKMNSXMADEITINEFFECTIVE - 12]       = sText_PkmnsXMadeItIneffective,
    [STRINGID_PKMNSXPREVENTSFLINCHING - 12]       = sText_PkmnsXPreventsFlinching,
    [STRINGID_PKMNALREADYHASBURN - 12]            = sText_PkmnAlreadyHasBurn,
    [STRINGID_STATSWONTDECREASE2 - 12]            = sText_StatsWontDecrease2,
    [STRINGID_PKMNSXBLOCKSY2 - 12]                = sText_PkmnsXBlocksY2,
    [STRINGID_PKMNSXWOREOFF - 12]                 = sText_PkmnsXWoreOff,
    [STRINGID_PKMNRAISEDDEFALITTLE - 12]          = sText_PkmnRaisedDefALittle,
    [STRINGID_PKMNRAISEDSPDEFALITTLE - 12]        = sText_PkmnRaisedSpDefALittle,
    [STRINGID_THEWALLSHATTERED - 12]              = sText_TheWallShattered,
    [STRINGID_PKMNSXPREVENTSYSZ - 12]             = sText_PkmnsXPreventsYsZ,
    [STRINGID_PKMNSXCUREDITSYPROBLEM - 12]        = sText_PkmnsXCuredItsYProblem,
    [STRINGID_ATTACKERCANTESCAPE - 12]            = sText_AttackerCantEscape,
    [STRINGID_PKMNOBTAINEDX - 12]                 = sText_PkmnObtainedX,
    [STRINGID_PKMNOBTAINEDX2 - 12]                = sText_PkmnObtainedX2,
    [STRINGID_PKMNOBTAINEDXYOBTAINEDZ - 12]       = sText_PkmnObtainedXYObtainedZ,
    [STRINGID_BUTNOEFFECT - 12]                   = sText_ButNoEffect,
    [STRINGID_PKMNSXHADNOEFFECTONY - 12]          = sText_PkmnsXHadNoEffectOnY,
    [STRINGID_OAKPLAYERWON - 12]                  = gText_WinEarnsPrizeMoney,
    [STRINGID_OAKPLAYERLOST - 12]                 = gText_HowDissapointing,
    [STRINGID_PLAYERLOSTAGAINSTENEMYTRAINER - 12] = sText_PlayerWhiteoutAgainstTrainer,
    [STRINGID_PLAYERPAIDPRIZEMONEY - 12]          = sText_PlayerPaidAsPrizeMoney,
    [STRINGID_PKMNTRANSFERREDSOMEONESPC - 12]     = gText_MonSentToBoxInSomeonesPC,
    [STRINGID_PKMNTRANSFERREDBILLSPC - 12]        = gText_MonSentToBoxInBillsPC,
    [STRINGID_PKMNBOXSOMEONESPCFULL - 12]         = gText_MonSentToBoxSomeonesBoxFull,
    [STRINGID_PKMNBOXBILLSPCFULL - 12]            = gText_MonSentToBoxBillsBoxFull,
    [STRINGID_POKEDUDEUSED - 12]                  = sText_PokedudeUsedItem,
    [STRINGID_POKEFLUTECATCHY - 12]               = sText_PlayedFluteCatchyTune,
    [STRINGID_POKEFLUTE - 12]                     = sText_PlayedThe,
    [STRINGID_MONHEARINGFLUTEAWOKE - 12]          = sText_PkmnHearingFluteAwoke,
    [STRINGID_TRAINER2CLASS - 12]                 = sText_Trainer2Class,
    [STRINGID_TRAINER2NAME - 12]                  = sText_Trainer2Name,
    [STRINGID_PLAYERWHITEDOUT - 12]               = sText_PlayerWhiteout2,
    [STRINGID_MONTOOSCAREDTOMOVE - 12]            = sText_TooScaredToMove,
    [STRINGID_GHOSTGETOUTGETOUT - 12]             = sText_GetOutGetOut,
    [STRINGID_SILPHSCOPEUNVEILED - 12]            = sText_SilphScopeUnveil,
    [STRINGID_GHOSTWASMAROWAK - 12]               = sText_TheGhostWas,
    [STRINGID_TRAINER1MON1COMEBACK - 12]          = sText_Trainer1RecallPkmn1,
    [STRINGID_TRAINER1WINTEXT - 12]               = sText_Trainer1WinText,
    [STRINGID_TRAINER1MON2COMEBACK - 12]          = sText_Trainer1RecallPkmn2,
    [STRINGID_TRAINER1MON1AND2COMEBACK - 12]      = sText_Trainer1RecallBoth
};

const u8 *const gBattleStringsTableSpa[] = {
    [STRINGID_TRAINER1LOSETEXT - 12]              = sText_Trainer1LoseTextSpa,
    [STRINGID_PKMNGAINEDEXP - 12]                 = sText_PkmnGainedEXPSpa,
    [STRINGID_PKMNGREWTOLV - 12]                  = sText_PkmnGrewToLvSpa,
    [STRINGID_PKMNLEARNEDMOVE - 12]               = sText_PkmnLearnedMoveSpa,
    [STRINGID_TRYTOLEARNMOVE1 - 12]               = sText_TryToLearnMove1Spa,
    [STRINGID_TRYTOLEARNMOVE2 - 12]               = sText_TryToLearnMove2Spa,
    [STRINGID_TRYTOLEARNMOVE3 - 12]               = sText_TryToLearnMove3Spa,
    [STRINGID_PKMNFORGOTMOVE - 12]                = sText_PkmnForgotMoveSpa,
    [STRINGID_STOPLEARNINGMOVE - 12]              = sText_StopLearningMoveSpa,
    [STRINGID_DIDNOTLEARNMOVE - 12]               = sText_DidNotLearnMoveSpa,
    [STRINGID_PKMNLEARNEDMOVE2 - 12]              = sText_PkmnLearnedMove2Spa,
    [STRINGID_ATTACKMISSED - 12]                  = sText_AttackMissedSpa,
    [STRINGID_PKMNPROTECTEDITSELF - 12]           = sText_PkmnProtectedItselfSpa,
    [STRINGID_STATSWONTINCREASE2 - 12]            = sText_StatsWontIncrease2Spa,
    [STRINGID_AVOIDEDDAMAGE - 12]                 = sText_AvoidedDamageSpa,
    [STRINGID_ITDOESNTAFFECT - 12]                = sText_ItDoesntAffectSpa,
    [STRINGID_ATTACKERFAINTED - 12]               = sText_AttackerFaintedSpa,
    [STRINGID_TARGETFAINTED - 12]                 = sText_TargetFaintedSpa,
    [STRINGID_PLAYERGOTMONEY - 12]                = sText_PlayerGotMoneySpa,
    [STRINGID_PLAYERWHITEOUT - 12]                = sText_PlayerWhiteoutSpa,
    [STRINGID_PLAYERWHITEOUT2 - 12]               = sText_PlayerPanickedSpa,
    [STRINGID_PREVENTSESCAPE - 12]                = sText_PreventsEscapeSpa,
    [STRINGID_HITXTIMES - 12]                     = sText_HitXTimesSpa,
    [STRINGID_PKMNFELLASLEEP - 12]                = sText_PkmnFellAsleepSpa,
    [STRINGID_PKMNMADESLEEP - 12]                 = sText_PkmnMadeSleepSpa,
    [STRINGID_PKMNALREADYASLEEP - 12]             = sText_PkmnAlreadyAsleepSpa,
    [STRINGID_PKMNALREADYASLEEP2 - 12]            = sText_PkmnAlreadyAsleep2Spa,
    [STRINGID_PKMNWASNTAFFECTED - 12]             = sText_PkmnWasntAffectedSpa,
    [STRINGID_PKMNWASPOISONED - 12]               = sText_PkmnWasPoisonedSpa,
    [STRINGID_PKMNPOISONEDBY - 12]                = sText_PkmnPoisonedBySpa,
    [STRINGID_PKMNHURTBYPOISON - 12]              = sText_PkmnHurtByPoisonSpa,
    [STRINGID_PKMNALREADYPOISONED - 12]           = sText_PkmnAlreadyPoisonedSpa,
    [STRINGID_PKMNBADLYPOISONED - 12]             = sText_PkmnBadlyPoisonedSpa,
    [STRINGID_PKMNENERGYDRAINED - 12]             = sText_PkmnEnergyDrainedSpa,
    [STRINGID_PKMNWASBURNED - 12]                 = sText_PkmnWasBurnedSpa,
    [STRINGID_PKMNBURNEDBY - 12]                  = sText_PkmnBurnedBySpa,
    [STRINGID_PKMNHURTBYBURN - 12]                = sText_PkmnHurtByBurnSpa,
    [STRINGID_PKMNWASFROZEN - 12]                 = sText_PkmnWasFrozenSpa,
    [STRINGID_PKMNFROZENBY - 12]                  = sText_PkmnFrozenBySpa,
    [STRINGID_PKMNISFROZEN - 12]                  = sText_PkmnIsFrozenSpa,
    [STRINGID_PKMNWASDEFROSTED - 12]              = sText_PkmnWasDefrostedSpa,
    [STRINGID_PKMNWASDEFROSTED2 - 12]             = sText_PkmnWasDefrosted2Spa,
    [STRINGID_PKMNWASDEFROSTEDBY - 12]            = sText_PkmnWasDefrostedBySpa,
    [STRINGID_PKMNWASPARALYZED - 12]              = sText_PkmnWasParalyzedSpa,
    [STRINGID_PKMNWASPARALYZEDBY - 12]            = sText_PkmnWasParalyzedBySpa,
    [STRINGID_PKMNISPARALYZED - 12]               = sText_PkmnIsParalyzedSpa,
    [STRINGID_PKMNISALREADYPARALYZED - 12]        = sText_PkmnIsAlreadyParalyzedSpa,
    [STRINGID_PKMNHEALEDPARALYSIS - 12]           = sText_PkmnHealedParalysisSpa,
    [STRINGID_PKMNDREAMEATEN - 12]                = sText_PkmnDreamEatenSpa,
    [STRINGID_STATSWONTINCREASE - 12]             = sText_StatsWontIncreaseSpa,
    [STRINGID_STATSWONTDECREASE - 12]             = sText_StatsWontDecreaseSpa,
    [STRINGID_TEAMSTOPPEDWORKING - 12]            = sText_TeamStoppedWorkingSpa,
    [STRINGID_FOESTOPPEDWORKING - 12]             = sText_FoeStoppedWorkingSpa,
    [STRINGID_PKMNISCONFUSED - 12]                = sText_PkmnIsConfusedSpa,
    [STRINGID_PKMNHEALEDCONFUSION - 12]           = sText_PkmnHealedConfusionSpa,
    [STRINGID_PKMNWASCONFUSED - 12]               = sText_PkmnWasConfusedSpa,
    [STRINGID_PKMNALREADYCONFUSED - 12]           = sText_PkmnAlreadyConfusedSpa,
    [STRINGID_PKMNFELLINLOVE - 12]                = sText_PkmnFellInLoveSpa,
    [STRINGID_PKMNINLOVE - 12]                    = sText_PkmnInLoveSpa,
    [STRINGID_PKMNIMMOBILIZEDBYLOVE - 12]         = sText_PkmnImmobilizedByLoveSpa,
    [STRINGID_PKMNBLOWNAWAY - 12]                 = sText_PkmnBlownAwaySpa,
    [STRINGID_PKMNCHANGEDTYPE - 12]               = sText_PkmnChangedTypeSpa,
    [STRINGID_PKMNFLINCHED - 12]                  = sText_PkmnFlinchedSpa,
    [STRINGID_PKMNREGAINEDHEALTH - 12]            = sText_PkmnRegainedHealthSpa,
    [STRINGID_PKMNHPFULL - 12]                    = sText_PkmnHPFullSpa,
    [STRINGID_PKMNRAISEDSPDEF - 12]               = sText_PkmnRaisedSpDefSpa,
    [STRINGID_PKMNRAISEDDEF - 12]                 = sText_PkmnRaisedDefSpa,
    [STRINGID_PKMNCOVEREDBYVEIL - 12]             = sText_PkmnCoveredByVeilSpa,
    [STRINGID_PKMNUSEDSAFEGUARD - 12]             = sText_PkmnUsedSafeguardSpa,
    [STRINGID_PKMNSAFEGUARDEXPIRED - 12]          = sText_PkmnSafeguardExpiredSpa,
    [STRINGID_PKMNWENTTOSLEEP - 12]               = sText_PkmnWentToSleepSpa,
    [STRINGID_PKMNSLEPTHEALTHY - 12]              = sText_PkmnSleptHealthySpa,
    [STRINGID_PKMNWHIPPEDWHIRLWIND - 12]          = sText_PkmnWhippedWhirlwindSpa,
    [STRINGID_PKMNTOOKSUNLIGHT - 12]              = sText_PkmnTookSunlightSpa,
    [STRINGID_PKMNLOWEREDHEAD - 12]               = sText_PkmnLoweredHeadSpa,
    [STRINGID_PKMNISGLOWING - 12]                 = sText_PkmnIsGlowingSpa,
    [STRINGID_PKMNFLEWHIGH - 12]                  = sText_PkmnFlewHighSpa,
    [STRINGID_PKMNDUGHOLE - 12]                   = sText_PkmnDugHoleSpa,
    [STRINGID_PKMNSQUEEZEDBYBIND - 12]            = sText_PkmnSqueezedByBindSpa,
    [STRINGID_PKMNTRAPPEDINVORTEX - 12]           = sText_PkmnTrappedInVortexSpa,
    [STRINGID_PKMNWRAPPEDBY - 12]                 = sText_PkmnWrappedBySpa,
    [STRINGID_PKMNCLAMPED - 12]                   = sText_PkmnClampedSpa,
    [STRINGID_PKMNHURTBY - 12]                    = sText_PkmnHurtBySpa,
    [STRINGID_PKMNFREEDFROM - 12]                 = sText_PkmnFreedFromSpa,
    [STRINGID_PKMNCRASHED - 12]                   = sText_PkmnCrashedSpa,
    [STRINGID_PKMNSHROUDEDINMIST - 12]            = gBattleText_MistShroudSpa,
    [STRINGID_PKMNPROTECTEDBYMIST - 12]           = sText_PkmnProtectedByMistSpa,
    [STRINGID_PKMNGETTINGPUMPED - 12]             = gBattleText_GetPumpedSpa,
    [STRINGID_PKMNHITWITHRECOIL - 12]             = sText_PkmnHitWithRecoilSpa,
    [STRINGID_PKMNPROTECTEDITSELF2 - 12]          = sText_PkmnProtectedItself2Spa,
    [STRINGID_PKMNBUFFETEDBYSANDSTORM - 12]       = sText_PkmnBuffetedBySandstormSpa,
    [STRINGID_PKMNPELTEDBYHAIL - 12]              = sText_PkmnPeltedByHailSpa,
    [STRINGID_PKMNSEEDED - 12]                    = sText_PkmnSeededSpa,
    [STRINGID_PKMNEVADEDATTACK - 12]              = sText_PkmnEvadedAttackSpa,
    [STRINGID_PKMNSAPPEDBYLEECHSEED - 12]         = sText_PkmnSappedByLeechSeedSpa,
    [STRINGID_PKMNFASTASLEEP - 12]                = sText_PkmnFastAsleepSpa,
    [STRINGID_PKMNWOKEUP - 12]                    = sText_PkmnWokeUpSpa,
    [STRINGID_PKMNUPROARKEPTAWAKE - 12]           = sText_PkmnUproarKeptAwakeSpa,
    [STRINGID_PKMNWOKEUPINUPROAR - 12]            = sText_PkmnWokeUpInUproarSpa,
    [STRINGID_PKMNCAUSEDUPROAR - 12]              = sText_PkmnCausedUproarSpa,
    [STRINGID_PKMNMAKINGUPROAR - 12]              = sText_PkmnMakingUproarSpa,
    [STRINGID_PKMNCALMEDDOWN - 12]                = sText_PkmnCalmedDownSpa,
    [STRINGID_PKMNCANTSLEEPINUPROAR - 12]         = sText_PkmnCantSleepInUproarSpa,
    [STRINGID_PKMNSTOCKPILED - 12]                = sText_PkmnStockpiledSpa,
    [STRINGID_PKMNCANTSTOCKPILE - 12]             = sText_PkmnCantStockpileSpa,
    [STRINGID_PKMNCANTSLEEPINUPROAR2 - 12]        = sText_PkmnCantSleepInUproar2Spa,
    [STRINGID_UPROARKEPTPKMNAWAKE - 12]           = sText_UproarKeptPkmnAwakeSpa,
    [STRINGID_PKMNSTAYEDAWAKEUSING - 12]          = sText_PkmnStayedAwakeUsingSpa,
    [STRINGID_PKMNSTORINGENERGY - 12]             = sText_PkmnStoringEnergySpa,
    [STRINGID_PKMNUNLEASHEDENERGY - 12]           = sText_PkmnUnleashedEnergySpa,
    [STRINGID_PKMNFATIGUECONFUSION - 12]          = sText_PkmnFatigueConfusionSpa,
    [STRINGID_PKMNPICKEDUPITEM - 12]              = sText_PkmnPickedUpItemSpa,
    [STRINGID_PKMNUNAFFECTED - 12]                = sText_PkmnUnaffectedSpa,
    [STRINGID_PKMNTRANSFORMEDINTO - 12]           = sText_PkmnTransformedIntoSpa,
    [STRINGID_PKMNMADESUBSTITUTE - 12]            = sText_PkmnMadeSubstituteSpa,
    [STRINGID_PKMNHASSUBSTITUTE - 12]             = sText_PkmnHasSubstituteSpa,
    [STRINGID_SUBSTITUTEDAMAGED - 12]             = sText_SubstituteDamagedSpa,
    [STRINGID_PKMNSUBSTITUTEFADED - 12]           = sText_PkmnSubstituteFadedSpa,
    [STRINGID_PKMNMUSTRECHARGE - 12]              = sText_PkmnMustRechargeSpa,
    [STRINGID_PKMNRAGEBUILDING - 12]              = sText_PkmnRageBuildingSpa,
    [STRINGID_PKMNMOVEWASDISABLED - 12]           = sText_PkmnMoveWasDisabledSpa,
    [STRINGID_PKMNMOVEISDISABLED - 12]            = sText_PkmnMoveIsDisabledSpa,
    [STRINGID_PKMNMOVEDISABLEDNOMORE - 12]        = sText_PkmnMoveDisabledNoMoreSpa,
    [STRINGID_PKMNGOTENCORE - 12]                 = sText_PkmnGotEncoreSpa,
    [STRINGID_PKMNENCOREENDED - 12]               = sText_PkmnEncoreEndedSpa,
    [STRINGID_PKMNTOOKAIM - 12]                   = sText_PkmnTookAimSpa,
    [STRINGID_PKMNSKETCHEDMOVE - 12]              = sText_PkmnSketchedMoveSpa,
    [STRINGID_PKMNTRYINGTOTAKEFOE - 12]           = sText_PkmnTryingToTakeFoeSpa,
    [STRINGID_PKMNTOOKFOE - 12]                   = sText_PkmnTookFoeSpa,
    [STRINGID_PKMNREDUCEDPP - 12]                 = sText_PkmnReducedPPSpa,
    [STRINGID_PKMNSTOLEITEM - 12]                 = sText_PkmnStoleItemSpa,
    [STRINGID_TARGETCANTESCAPENOW - 12]           = sText_TargetCantEscapeNowSpa,
    [STRINGID_PKMNFELLINTONIGHTMARE - 12]         = sText_PkmnFellIntoNightmareSpa,
    [STRINGID_PKMNLOCKEDINNIGHTMARE - 12]         = sText_PkmnLockedInNightmareSpa,
    [STRINGID_PKMNLAIDCURSE - 12]                 = sText_PkmnLaidCurseSpa,
    [STRINGID_PKMNAFFLICTEDBYCURSE - 12]          = sText_PkmnAfflictedByCurseSpa,
    [STRINGID_SPIKESSCATTERED - 12]               = sText_SpikesScatteredSpa,
    [STRINGID_PKMNHURTBYSPIKES - 12]              = sText_PkmnHurtBySpikesSpa,
    [STRINGID_PKMNIDENTIFIED - 12]                = sText_PkmnIdentifiedSpa,
    [STRINGID_PKMNPERISHCOUNTFELL - 12]           = sText_PkmnPerishCountFellSpa,
    [STRINGID_PKMNBRACEDITSELF - 12]              = sText_PkmnBracedItselfSpa,
    [STRINGID_PKMNENDUREDHIT - 12]                = sText_PkmnEnduredHitSpa,
    [STRINGID_MAGNITUDESTRENGTH - 12]             = sText_MagnitudeStrengthSpa,
    [STRINGID_PKMNCUTHPMAXEDATTACK - 12]          = sText_PkmnCutHPMaxedAttackSpa,
    [STRINGID_PKMNCOPIEDSTATCHANGES - 12]         = sText_PkmnCopiedStatChangesSpa,
    [STRINGID_PKMNGOTFREE - 12]                   = sText_PkmnGotFreeSpa,
    [STRINGID_PKMNSHEDLEECHSEED - 12]             = sText_PkmnShedLeechSeedSpa,
    [STRINGID_PKMNBLEWAWAYSPIKES - 12]            = sText_PkmnBlewAwaySpikesSpa,
    [STRINGID_PKMNFLEDFROMBATTLE - 12]            = sText_PkmnFledFromBattleSpa,
    [STRINGID_PKMNFORESAWATTACK - 12]             = sText_PkmnForesawAttackSpa,
    [STRINGID_PKMNTOOKATTACK - 12]                = sText_PkmnTookAttackSpa,
    [STRINGID_PKMNATTACK - 12]                    = sText_PkmnAttackSpa,
    [STRINGID_PKMNCENTERATTENTION - 12]           = sText_PkmnCenterAttentionSpa,
    [STRINGID_PKMNCHARGINGPOWER - 12]             = sText_PkmnChargingPowerSpa,
    [STRINGID_NATUREPOWERTURNEDINTO - 12]         = sText_NaturePowerTurnedIntoSpa,
    [STRINGID_PKMNSTATUSNORMAL - 12]              = sText_PkmnStatusNormalSpa,
    [STRINGID_PKMNHASNOMOVESLEFT - 12]            = sText_PkmnHasNoMovesLeftSpa,
    [STRINGID_PKMNSUBJECTEDTOTORMENT - 12]        = sText_PkmnSubjectedToTormentSpa,
    [STRINGID_PKMNCANTUSEMOVETORMENT - 12]        = sText_PkmnCantUseMoveTormentSpa,
    [STRINGID_PKMNTIGHTENINGFOCUS - 12]           = sText_PkmnTighteningFocusSpa,
    [STRINGID_PKMNFELLFORTAUNT - 12]              = sText_PkmnFellForTauntSpa,
    [STRINGID_PKMNCANTUSEMOVETAUNT - 12]          = sText_PkmnCantUseMoveTauntSpa,
    [STRINGID_PKMNREADYTOHELP - 12]               = sText_PkmnReadyToHelpSpa,
    [STRINGID_PKMNSWITCHEDITEMS - 12]             = sText_PkmnSwitchedItemsSpa,
    [STRINGID_PKMNCOPIEDFOE - 12]                 = sText_PkmnCopiedFoeSpa,
    [STRINGID_PKMNMADEWISH - 12]                  = sText_PkmnMadeWishSpa,
    [STRINGID_PKMNWISHCAMETRUE - 12]              = sText_PkmnWishCameTrueSpa,
    [STRINGID_PKMNPLANTEDROOTS - 12]              = sText_PkmnPlantedRootsSpa,
    [STRINGID_PKMNABSORBEDNUTRIENTS - 12]         = sText_PkmnAbsorbedNutrientsSpa,
    [STRINGID_PKMNANCHOREDITSELF - 12]            = sText_PkmnAnchoredItselfSpa,
    [STRINGID_PKMNWASMADEDROWSY - 12]             = sText_PkmnWasMadeDrowsySpa,
    [STRINGID_PKMNKNOCKEDOFF - 12]                = sText_PkmnKnockedOffSpa,
    [STRINGID_PKMNSWAPPEDABILITIES - 12]          = sText_PkmnSwappedAbilitiesSpa,
    [STRINGID_PKMNSEALEDOPPONENTMOVE - 12]        = sText_PkmnSealedOpponentMoveSpa,
    [STRINGID_PKMNCANTUSEMOVESEALED - 12]         = sText_PkmnCantUseMoveSealedSpa,
    [STRINGID_PKMNWANTSGRUDGE - 12]               = sText_PkmnWantsGrudgeSpa,
    [STRINGID_PKMNLOSTPPGRUDGE - 12]              = sText_PkmnLostPPGrudgeSpa,
    [STRINGID_PKMNSHROUDEDITSELF - 12]            = sText_PkmnShroudedItselfSpa,
    [STRINGID_PKMNMOVEBOUNCED - 12]               = sText_PkmnMoveBouncedSpa,
    [STRINGID_PKMNWAITSFORTARGET - 12]            = sText_PkmnWaitsForTargetSpa,
    [STRINGID_PKMNSNATCHEDMOVE - 12]              = sText_PkmnSnatchedMoveSpa,
    [STRINGID_PKMNMADEITRAIN - 12]                = sText_PkmnMadeItRainSpa,
    [STRINGID_PKMNRAISEDSPEED - 12]               = sText_PkmnRaisedSpeedSpa,
    [STRINGID_PKMNPROTECTEDBY - 12]               = sText_PkmnProtectedBySpa,
    [STRINGID_PKMNPREVENTSUSAGE - 12]             = sText_PkmnPreventsUsageSpa,
    [STRINGID_PKMNRESTOREDHPUSING - 12]           = sText_PkmnRestoredHPUsingSpa,
    [STRINGID_PKMNCHANGEDTYPEWITH - 12]           = sText_PkmnChangedTypeWithSpa,
    [STRINGID_PKMNPREVENTSPARALYSISWITH - 12]     = sText_PkmnPreventsParalysisWithSpa,
    [STRINGID_PKMNPREVENTSROMANCEWITH - 12]       = sText_PkmnPreventsRomanceWithSpa,
    [STRINGID_PKMNPREVENTSPOISONINGWITH - 12]     = sText_PkmnPreventsPoisoningWithSpa,
    [STRINGID_PKMNPREVENTSCONFUSIONWITH - 12]     = sText_PkmnPreventsConfusionWithSpa,
    [STRINGID_PKMNRAISEDFIREPOWERWITH - 12]       = sText_PkmnRaisedFirePowerWithSpa,
    [STRINGID_PKMNANCHORSITSELFWITH - 12]         = sText_PkmnAnchorsItselfWithSpa,
    [STRINGID_PKMNCUTSATTACKWITH - 12]            = sText_PkmnCutsAttackWithSpa,
    [STRINGID_PKMNPREVENTSSTATLOSSWITH - 12]      = sText_PkmnPreventsStatLossWithSpa,
    [STRINGID_PKMNHURTSWITH - 12]                 = sText_PkmnHurtsWithSpa,
    [STRINGID_PKMNTRACED - 12]                    = sText_PkmnTracedSpa,
    [STRINGID_STATSHARPLY - 12]                   = sText_StatSharplySpa,
    [STRINGID_STATROSE - 12]                      = gBattleText_RoseSpa,
    [STRINGID_STATHARSHLY - 12]                   = sText_StatHarshlySpa,
    [STRINGID_STATFELL - 12]                      = sText_StatFellSpa,
    [STRINGID_PKMNSSTATCHANGED - 12]              = sText_PkmnsStatChangedSpa,
    [STRINGID_PKMNSSTATCHANGED2 - 12]             = gText_PkmnsStatChanged2Spa,
    [STRINGID_PKMNSSTATCHANGED3 - 12]             = sText_PkmnsStatChanged3Spa,
    [STRINGID_PKMNSSTATCHANGED4 - 12]             = sText_PkmnsStatChanged4Spa,
    [STRINGID_CRITICALHIT - 12]                   = sText_CriticalHitSpa,
    [STRINGID_ONEHITKO - 12]                      = sText_OneHitKOSpa,
    [STRINGID_123POOF - 12]                       = sText_123PoofSpa,
    [STRINGID_ANDELLIPSIS - 12]                   = sText_AndEllipsisSpa,
    [STRINGID_NOTVERYEFFECTIVE - 12]              = sText_NotVeryEffectiveSpa,
    [STRINGID_SUPEREFFECTIVE - 12]                = sText_SuperEffectiveSpa,
    [STRINGID_GOTAWAYSAFELY - 12]                 = sText_GotAwaySafelySpa,
    [STRINGID_WILDPKMNFLED - 12]                  = sText_WildPkmnFledSpa,
    [STRINGID_NORUNNINGFROMTRAINERS - 12]         = sText_NoRunningFromTrainersSpa,
    [STRINGID_CANTESCAPE - 12]                    = sText_CantEscapeSpa,
    [STRINGID_DONTLEAVEBIRCH - 12]                = sText_EmptyString5Spa,
    [STRINGID_BUTNOTHINGHAPPENED - 12]            = sText_ButNothingHappenedSpa,
    [STRINGID_BUTITFAILED - 12]                   = sText_ButItFailedSpa,
    [STRINGID_ITHURTCONFUSION - 12]               = sText_ItHurtConfusionSpa,
    [STRINGID_MIRRORMOVEFAILED - 12]              = sText_MirrorMoveFailedSpa,
    [STRINGID_STARTEDTORAIN - 12]                 = sText_StartedToRainSpa,
    [STRINGID_DOWNPOURSTARTED - 12]               = sText_DownpourStartedSpa,
    [STRINGID_RAINCONTINUES - 12]                 = sText_RainContinuesSpa,
    [STRINGID_DOWNPOURCONTINUES - 12]             = sText_DownpourContinuesSpa,
    [STRINGID_RAINSTOPPED - 12]                   = sText_RainStoppedSpa,
    [STRINGID_SANDSTORMBREWED - 12]               = sText_SandstormBrewedSpa,
    [STRINGID_SANDSTORMRAGES - 12]                = sText_SandstormRagesSpa,
    [STRINGID_SANDSTORMSUBSIDED - 12]             = sText_SandstormSubsidedSpa,
    [STRINGID_SUNLIGHTGOTBRIGHT - 12]             = sText_SunlightGotBrightSpa,
    [STRINGID_SUNLIGHTSTRONG - 12]                = sText_SunlightStrongSpa,
    [STRINGID_SUNLIGHTFADED - 12]                 = sText_SunlightFadedSpa,
    [STRINGID_STARTEDHAIL - 12]                   = sText_StartedHailSpa,
    [STRINGID_HAILCONTINUES - 12]                 = sText_HailContinuesSpa,
    [STRINGID_HAILSTOPPED - 12]                   = sText_HailStoppedSpa,
    [STRINGID_FAILEDTOSPITUP - 12]                = sText_FailedToSpitUpSpa,
    [STRINGID_FAILEDTOSWALLOW - 12]               = sText_FailedToSwallowSpa,
    [STRINGID_WINDBECAMEHEATWAVE - 12]            = sText_WindBecameHeatWaveSpa,
    [STRINGID_STATCHANGESGONE - 12]               = sText_StatChangesGoneSpa,
    [STRINGID_COINSSCATTERED - 12]                = sText_CoinsScatteredSpa,
    [STRINGID_TOOWEAKFORSUBSTITUTE - 12]          = sText_TooWeakForSubstituteSpa,
    [STRINGID_SHAREDPAIN - 12]                    = sText_SharedPainSpa,
    [STRINGID_BELLCHIMED - 12]                    = sText_BellChimedSpa,
    [STRINGID_FAINTINTHREE - 12]                  = sText_FaintInThreeSpa,
    [STRINGID_NOPPLEFT - 12]                      = sText_NoPPLeftSpa,
    [STRINGID_BUTNOPPLEFT - 12]                   = sText_ButNoPPLeftSpa,
    [STRINGID_PLAYERUSEDITEM - 12]                = sText_PlayerUsedItemSpa,
    [STRINGID_OLDMANUSEDITEM - 12]                = sText_OldManUsedItemSpa,
    [STRINGID_TRAINERBLOCKEDBALL - 12]            = sText_TrainerBlockedBallSpa,
    [STRINGID_DONTBEATHIEF - 12]                  = sText_DontBeAThiefSpa,
    [STRINGID_ITDODGEDBALL - 12]                  = sText_ItDodgedBallSpa,
    [STRINGID_YOUMISSEDPKMN - 12]                 = sText_YouMissedPkmnSpa,
    [STRINGID_PKMNBROKEFREE - 12]                 = sText_PkmnBrokeFreeSpa,
    [STRINGID_ITAPPEAREDCAUGHT - 12]              = sText_ItAppearedCaughtSpa,
    [STRINGID_AARGHALMOSTHADIT - 12]              = sText_AarghAlmostHadItSpa,
    [STRINGID_SHOOTSOCLOSE - 12]                  = sText_ShootSoCloseSpa,
    [STRINGID_GOTCHAPKMNCAUGHT - 12]              = sText_GotchaPkmnCaughtSpa,
    [STRINGID_GOTCHAPKMNCAUGHT2 - 12]             = sText_GotchaPkmnCaught2Spa,
    [STRINGID_GIVENICKNAMECAPTURED - 12]          = sText_GiveNicknameCapturedSpa,
    [STRINGID_PKMNSENTTOPC - 12]                  = sText_PkmnSentToPCSpa,
    [STRINGID_PKMNDATAADDEDTODEX - 12]            = sText_PkmnDataAddedToDexSpa,
    [STRINGID_ITISRAINING - 12]                   = sText_ItIsRainingSpa,
    [STRINGID_SANDSTORMISRAGING - 12]             = sText_SandstormIsRagingSpa,
    [STRINGID_CANTESCAPE2 - 12]                   = sText_CantEscape2Spa,
    [STRINGID_PKMNIGNORESASLEEP - 12]             = sText_PkmnIgnoresAsleepSpa,
    [STRINGID_PKMNIGNOREDORDERS - 12]             = sText_PkmnIgnoredOrdersSpa,
    [STRINGID_PKMNBEGANTONAP - 12]                = sText_PkmnBeganToNapSpa,
    [STRINGID_PKMNLOAFING - 12]                   = sText_PkmnLoafingSpa,
    [STRINGID_PKMNWONTOBEY - 12]                  = sText_PkmnWontObeySpa,
    [STRINGID_PKMNTURNEDAWAY - 12]                = sText_PkmnTurnedAwaySpa,
    [STRINGID_PKMNPRETENDNOTNOTICE - 12]          = sText_PkmnPretendNotNoticeSpa,
    [STRINGID_ENEMYABOUTTOSWITCHPKMN - 12]        = sText_EnemyAboutToSwitchPkmnSpa,
    [STRINGID_THREWROCK - 12]                     = sText_ThrewARockSpa,
    [STRINGID_THREWBAIT - 12]                     = sText_ThrewSomeBaitSpa,
    [STRINGID_PKMNWATCHINGCAREFULLY - 12]         = sText_PkmnWatchingCarefullySpa,
    [STRINGID_PKMNANGRY - 12]                     = sText_PkmnIsAngrySpa,
    [STRINGID_PKMNEATING - 12]                    = sText_PkmnIsEatingSpa,
    [STRINGID_DUMMY288 - 12]                      = sText_Empty1Spa,
    [STRINGID_DUMMY289 - 12]                      = sText_Empty1Spa,
    [STRINGID_OUTOFSAFARIBALLS - 12]              = sText_OutOfSafariBallsSpa,
    [STRINGID_PKMNSITEMCUREDPARALYSIS - 12]       = sText_PkmnsItemCuredParalysisSpa,
    [STRINGID_PKMNSITEMCUREDPOISON - 12]          = sText_PkmnsItemCuredPoisonSpa,
    [STRINGID_PKMNSITEMHEALEDBURN - 12]           = sText_PkmnsItemHealedBurnSpa,
    [STRINGID_PKMNSITEMDEFROSTEDIT - 12]          = sText_PkmnsItemDefrostedItSpa,
    [STRINGID_PKMNSITEMWOKEIT - 12]               = sText_PkmnsItemWokeItSpa,
    [STRINGID_PKMNSITEMSNAPPEDOUT - 12]           = sText_PkmnsItemSnappedOutSpa,
    [STRINGID_PKMNSITEMCUREDPROBLEM - 12]         = sText_PkmnsItemCuredProblemSpa,
    [STRINGID_PKMNSITEMRESTOREDHEALTH - 12]       = sText_PkmnsItemRestoredHealthSpa,
    [STRINGID_PKMNSITEMRESTOREDPP - 12]           = sText_PkmnsItemRestoredPPSpa,
    [STRINGID_PKMNSITEMRESTOREDSTATUS - 12]       = sText_PkmnsItemRestoredStatusSpa,
    [STRINGID_PKMNSITEMRESTOREDHPALITTLE - 12]    = sText_PkmnsItemRestoredHPALittleSpa,
    [STRINGID_ITEMALLOWSONLYYMOVE - 12]           = sText_ItemAllowsOnlyYMoveSpa,
    [STRINGID_PKMNHUNGONWITHX - 12]               = sText_PkmnHungOnWithXSpa,
    [STRINGID_EMPTYSTRING3 - 12]                  = gText_EmptyString3Spa,
    [STRINGID_PKMNSXPREVENTSBURNS - 12]           = sText_PkmnsXPreventsBurnsSpa,
    [STRINGID_PKMNSXBLOCKSY - 12]                 = sText_PkmnsXBlocksYSpa,
    [STRINGID_PKMNSXRESTOREDHPALITTLE2 - 12]      = sText_PkmnsXRestoredHPALittle2Spa,
    [STRINGID_PKMNSXWHIPPEDUPSANDSTORM - 12]      = sText_PkmnsXWhippedUpSandstormSpa,
    [STRINGID_PKMNSXPREVENTSYLOSS - 12]           = sText_PkmnsXPreventsYLossSpa,
    [STRINGID_PKMNSXINFATUATEDY - 12]             = sText_PkmnsXInfatuatedYSpa,
    [STRINGID_PKMNSXMADEYINEFFECTIVE - 12]        = sText_PkmnsXMadeYIneffectiveSpa,
    [STRINGID_PKMNSXCUREDYPROBLEM - 12]           = sText_PkmnsXCuredYProblemSpa,
    [STRINGID_ITSUCKEDLIQUIDOOZE - 12]            = sText_ItSuckedLiquidOozeSpa,
    [STRINGID_PKMNTRANSFORMED - 12]               = sText_PkmnTransformedSpa,
    [STRINGID_ELECTRICITYWEAKENED - 12]           = sText_ElectricityWeakenedSpa,
    [STRINGID_FIREWEAKENED - 12]                  = sText_FireWeakenedSpa,
    [STRINGID_PKMNHIDUNDERWATER - 12]             = sText_PkmnHidUnderwaterSpa,
    [STRINGID_PKMNSPRANGUP - 12]                  = sText_PkmnSprangUpSpa,
    [STRINGID_HMMOVESCANTBEFORGOTTEN - 12]        = sText_HMMovesCantBeForgottenSpa,
    [STRINGID_XFOUNDONEY - 12]                    = sText_XFoundOneYSpa,
    [STRINGID_PLAYERDEFEATEDTRAINER1 - 12]        = sText_PlayerDefeatedLinkTrainerTrainer1Spa,
    [STRINGID_SOOTHINGAROMA - 12]                 = sText_SoothingAromaSpa,
    [STRINGID_ITEMSCANTBEUSEDNOW - 12]            = sText_ItemsCantBeUsedNowSpa,
    [STRINGID_FORXCOMMAYZ - 12]                   = sText_ForXCommaYZSpa,
    [STRINGID_USINGXTHEYOFZN - 12]                = sText_UsingXTheYOfZNSpa,
    [STRINGID_PKMNUSEDXTOGETPUMPED - 12]          = sText_PkmnUsedXToGetPumpedSpa,
    [STRINGID_PKMNSXMADEYUSELESS - 12]            = sText_PkmnsXMadeYUselessSpa,
    [STRINGID_PKMNTRAPPEDBYSANDTOMB - 12]         = sText_PkmnTrappedBySandTombSpa,
    [STRINGID_EMPTYSTRING4 - 12]                  = sText_EmptyString4Spa,
    [STRINGID_ABOOSTED - 12]                      = sText_ABoostedSpa,
    [STRINGID_PKMNSXINTENSIFIEDSUN - 12]          = sText_PkmnsXIntensifiedSunSpa,
    [STRINGID_PKMNMAKESGROUNDMISS - 12]           = sText_PkmnMakesGroundMissSpa,
    [STRINGID_YOUTHROWABALLNOWRIGHT - 12]         = sText_YouThrowABallNowRightSpa,
    [STRINGID_PKMNSXTOOKATTACK - 12]              = sText_PkmnsXTookAttackSpa,
    [STRINGID_PKMNCHOSEXASDESTINY - 12]           = sText_PkmnChoseXAsDestinySpa,
    [STRINGID_PKMNLOSTFOCUS - 12]                 = sText_PkmnLostFocusSpa,
    [STRINGID_USENEXTPKMN - 12]                   = sText_UseNextPkmnSpa,
    [STRINGID_PKMNFLEDUSINGITS - 12]              = sText_PkmnFledUsingItsSpa,
    [STRINGID_PKMNFLEDUSING - 12]                 = sText_PkmnFledUsingSpa,
    [STRINGID_PKMNWASDRAGGEDOUT - 12]             = sText_PkmnWasDraggedOutSpa,
    [STRINGID_PREVENTEDFROMWORKING - 12]          = sText_PreventedFromWorkingSpa,
    [STRINGID_PKMNSITEMNORMALIZEDSTATUS - 12]     = sText_PkmnsItemNormalizedStatusSpa,
    [STRINGID_TRAINER1USEDITEM - 12]              = sText_Trainer1UsedItemSpa,
    [STRINGID_BOXISFULL - 12]                     = sText_BoxIsFullSpa,
    [STRINGID_PKMNAVOIDEDATTACK - 12]             = sText_PkmnAvoidedAttackSpa,
    [STRINGID_PKMNSXMADEITINEFFECTIVE - 12]       = sText_PkmnsXMadeItIneffectiveSpa,
    [STRINGID_PKMNSXPREVENTSFLINCHING - 12]       = sText_PkmnsXPreventsFlinchingSpa,
    [STRINGID_PKMNALREADYHASBURN - 12]            = sText_PkmnAlreadyHasBurnSpa,
    [STRINGID_STATSWONTDECREASE2 - 12]            = sText_StatsWontDecrease2Spa,
    [STRINGID_PKMNSXBLOCKSY2 - 12]                = sText_PkmnsXBlocksY2Spa,
    [STRINGID_PKMNSXWOREOFF - 12]                 = sText_PkmnsXWoreOffSpa,
    [STRINGID_PKMNRAISEDDEFALITTLE - 12]          = sText_PkmnRaisedDefALittleSpa,
    [STRINGID_PKMNRAISEDSPDEFALITTLE - 12]        = sText_PkmnRaisedSpDefALittleSpa,
    [STRINGID_THEWALLSHATTERED - 12]              = sText_TheWallShatteredSpa,
    [STRINGID_PKMNSXPREVENTSYSZ - 12]             = sText_PkmnsXPreventsYsZSpa,
    [STRINGID_PKMNSXCUREDITSYPROBLEM - 12]        = sText_PkmnsXCuredItsYProblemSpa,
    [STRINGID_ATTACKERCANTESCAPE - 12]            = sText_AttackerCantEscapeSpa,
    [STRINGID_PKMNOBTAINEDX - 12]                 = sText_PkmnObtainedXSpa,
    [STRINGID_PKMNOBTAINEDX2 - 12]                = sText_PkmnObtainedX2Spa,
    [STRINGID_PKMNOBTAINEDXYOBTAINEDZ - 12]       = sText_PkmnObtainedXYObtainedZSpa,
    [STRINGID_BUTNOEFFECT - 12]                   = sText_ButNoEffectSpa,
    [STRINGID_PKMNSXHADNOEFFECTONY - 12]          = sText_PkmnsXHadNoEffectOnYSpa,
    [STRINGID_OAKPLAYERWON - 12]                  = gText_WinEarnsPrizeMoneySpa,
    [STRINGID_OAKPLAYERLOST - 12]                 = gText_HowDissapointingSpa,
    [STRINGID_PLAYERLOSTAGAINSTENEMYTRAINER - 12] = sText_PlayerWhiteoutAgainstTrainerSpa,
    [STRINGID_PLAYERPAIDPRIZEMONEY - 12]          = sText_PlayerPaidAsPrizeMoneySpa,
    [STRINGID_PKMNTRANSFERREDSOMEONESPC - 12]     = gText_MonSentToBoxInSomeonesPCSpa,
    [STRINGID_PKMNTRANSFERREDBILLSPC - 12]        = gText_MonSentToBoxInBillsPCSpa,
    [STRINGID_PKMNBOXSOMEONESPCFULL - 12]         = gText_MonSentToBoxSomeonesBoxFullSpa,
    [STRINGID_PKMNBOXBILLSPCFULL - 12]            = gText_MonSentToBoxBillsBoxFullSpa,
    [STRINGID_POKEDUDEUSED - 12]                  = sText_PokedudeUsedItemSpa,
    [STRINGID_POKEFLUTECATCHY - 12]               = sText_PlayedFluteCatchyTuneSpa,
    [STRINGID_POKEFLUTE - 12]                     = sText_PlayedTheSpa,
    [STRINGID_MONHEARINGFLUTEAWOKE - 12]          = sText_PkmnHearingFluteAwokeSpa,
    [STRINGID_TRAINER2CLASS - 12]                 = sText_Trainer2ClassSpa,
    [STRINGID_TRAINER2NAME - 12]                  = sText_Trainer2NameSpa,
    [STRINGID_PLAYERWHITEDOUT - 12]               = sText_PlayerWhiteout2Spa,
    [STRINGID_MONTOOSCAREDTOMOVE - 12]            = sText_TooScaredToMoveSpa,
    [STRINGID_GHOSTGETOUTGETOUT - 12]             = sText_GetOutGetOutSpa,
    [STRINGID_SILPHSCOPEUNVEILED - 12]            = sText_SilphScopeUnveilSpa,
    [STRINGID_GHOSTWASMAROWAK - 12]               = sText_TheGhostWasSpa,
    [STRINGID_TRAINER1MON1COMEBACK - 12]          = sText_Trainer1RecallPkmn1Spa,
    [STRINGID_TRAINER1WINTEXT - 12]               = sText_Trainer1WinTextSpa,
    [STRINGID_TRAINER1MON2COMEBACK - 12]          = sText_Trainer1RecallPkmn2Spa,
    [STRINGID_TRAINER1MON1AND2COMEBACK - 12]      = sText_Trainer1RecallBothSpa,
};

const u16 gMissStringIds[] = {
    STRINGID_ATTACKMISSED,
    STRINGID_PKMNPROTECTEDITSELF,
    STRINGID_PKMNAVOIDEDATTACK,
    STRINGID_AVOIDEDDAMAGE,
    STRINGID_PKMNMAKESGROUNDMISS
};

const u16 gNoEscapeStringIds[] = {
    STRINGID_CANTESCAPE,
    STRINGID_DONTLEAVEBIRCH,
    STRINGID_PREVENTSESCAPE,
    STRINGID_CANTESCAPE2,
    STRINGID_ATTACKERCANTESCAPE
};

const u16 gMoveWeatherChangeStringIds[] = {
    STRINGID_STARTEDTORAIN,
    STRINGID_DOWNPOURSTARTED,
    STRINGID_BUTITFAILED,
    STRINGID_SANDSTORMBREWED,
    STRINGID_SUNLIGHTGOTBRIGHT,
    STRINGID_STARTEDHAIL
};

const u16 gSandstormHailContinuesStringIds[] = {
    STRINGID_SANDSTORMRAGES,
    STRINGID_HAILCONTINUES
};

const u16 gSandstormHailDmgStringIds[] = {
    STRINGID_PKMNBUFFETEDBYSANDSTORM,
    STRINGID_PKMNPELTEDBYHAIL
};

const u16 gSandstormHailEndStringIds[] = {
    STRINGID_SANDSTORMSUBSIDED,
    STRINGID_HAILSTOPPED
};

const u16 gRainContinuesStringIds[] = {
    STRINGID_RAINCONTINUES,
    STRINGID_DOWNPOURCONTINUES,
    STRINGID_RAINSTOPPED
};

const u16 gProtectLikeUsedStringIds[] = {
    STRINGID_PKMNPROTECTEDITSELF2,
    STRINGID_PKMNBRACEDITSELF,
    STRINGID_BUTITFAILED
};

const u16 gReflectLightScreenSafeguardStringIds[] = {
    STRINGID_BUTITFAILED,
    STRINGID_PKMNRAISEDDEF,
    STRINGID_PKMNRAISEDDEFALITTLE,
    STRINGID_PKMNRAISEDSPDEF,
    STRINGID_PKMNRAISEDSPDEFALITTLE,
    STRINGID_PKMNCOVEREDBYVEIL
};

const u16 gLeechSeedStringIds[] = {
    STRINGID_PKMNSEEDED,
    STRINGID_PKMNEVADEDATTACK,
    STRINGID_ITDOESNTAFFECT,
    STRINGID_PKMNSAPPEDBYLEECHSEED,
    STRINGID_ITSUCKEDLIQUIDOOZE
};

const u16 gRestUsedStringIds[] = {
    STRINGID_PKMNWENTTOSLEEP,
    STRINGID_PKMNSLEPTHEALTHY
};

const u16 gUproarOverTurnStringIds[] = {
    STRINGID_PKMNMAKINGUPROAR,
    STRINGID_PKMNCALMEDDOWN
};

const u16 gStockpileUsedStringIds[] = {
    STRINGID_PKMNSTOCKPILED,
    STRINGID_PKMNCANTSTOCKPILE
};

const u16 gWokeUpStringIds[] = {
    STRINGID_PKMNWOKEUP,
    STRINGID_PKMNWOKEUPINUPROAR
};

const u16 gSwallowFailStringIds[] = {
    STRINGID_FAILEDTOSWALLOW,
    STRINGID_PKMNHPFULL
};

const u16 gUproarAwakeStringIds[] = {
    STRINGID_PKMNCANTSLEEPINUPROAR2,
    STRINGID_UPROARKEPTPKMNAWAKE,
    STRINGID_PKMNSTAYEDAWAKEUSING
};

const u16 gStatUpStringIds[] = {
    STRINGID_PKMNSSTATCHANGED,
    STRINGID_PKMNSSTATCHANGED2,
    STRINGID_STATSWONTINCREASE,
    STRINGID_EMPTYSTRING3,
    STRINGID_USINGXTHEYOFZN,
    STRINGID_PKMNUSEDXTOGETPUMPED
};

const u16 gStatDownStringIds[] = {
    STRINGID_PKMNSSTATCHANGED3,
    STRINGID_PKMNSSTATCHANGED4,
    STRINGID_STATSWONTDECREASE,
    STRINGID_EMPTYSTRING3
};

const u16 gFirstTurnOfTwoStringIds[] = {
    STRINGID_PKMNWHIPPEDWHIRLWIND,
    STRINGID_PKMNTOOKSUNLIGHT,
    STRINGID_PKMNLOWEREDHEAD,
    STRINGID_PKMNISGLOWING,
    STRINGID_PKMNFLEWHIGH,
    STRINGID_PKMNDUGHOLE,
    STRINGID_PKMNHIDUNDERWATER,
    STRINGID_PKMNSPRANGUP
};

const u16 gWrappedStringIds[] = {
    STRINGID_PKMNSQUEEZEDBYBIND,
    STRINGID_PKMNWRAPPEDBY,
    STRINGID_PKMNTRAPPEDINVORTEX,
    STRINGID_PKMNCLAMPED,
    STRINGID_PKMNTRAPPEDINVORTEX,
    STRINGID_PKMNTRAPPEDBYSANDTOMB
};

const u16 gMistUsedStringIds[] = {
    STRINGID_PKMNSHROUDEDINMIST,
    STRINGID_BUTITFAILED
};

const u16 gFocusEnergyUsedStringIds[] = {
    STRINGID_PKMNGETTINGPUMPED,
    STRINGID_BUTITFAILED
};

const u16 gTransformUsedStringIds[] = {
    STRINGID_PKMNTRANSFORMEDINTO,
    STRINGID_BUTITFAILED
};

const u16 gSubsituteUsedStringIds[] = {
    STRINGID_PKMNMADESUBSTITUTE,
    STRINGID_TOOWEAKFORSUBSTITUTE
};

const u16 gGotPoisonedStringIds[] = {
    STRINGID_PKMNWASPOISONED,
    STRINGID_PKMNPOISONEDBY
};

const u16 gGotParalyzedStringIds[] = {
    STRINGID_PKMNWASPARALYZED,
    STRINGID_PKMNWASPARALYZEDBY
};

const u16 gFellAsleepStringIds[] = {
    STRINGID_PKMNFELLASLEEP,
    STRINGID_PKMNMADESLEEP
};

const u16 gGotBurnedStringIds[] = {
    STRINGID_PKMNWASBURNED,
    STRINGID_PKMNBURNEDBY
};

const u16 gGotFrozenStringIds[] = {
    STRINGID_PKMNWASFROZEN,
    STRINGID_PKMNFROZENBY
};

const u16 gGotDefrostedStringIds[] = {
    STRINGID_PKMNWASDEFROSTED2,
    STRINGID_PKMNWASDEFROSTEDBY
};

const u16 gKOFailedStringIds[] = {
    STRINGID_ATTACKMISSED,
    STRINGID_PKMNUNAFFECTED
};

const u16 gAttractUsedStringIds[] = {
    STRINGID_PKMNFELLINLOVE,
    STRINGID_PKMNSXINFATUATEDY
};

const u16 gLeechSeedDrainStringIds[] = {
    STRINGID_PKMNENERGYDRAINED,
    STRINGID_ITSUCKEDLIQUIDOOZE
};

const u16 gSportsUsedStringIds[] = {
    STRINGID_ELECTRICITYWEAKENED,
    STRINGID_FIREWEAKENED
};

const u16 gPartyStatusHealStringIds[] = {
    STRINGID_BELLCHIMED,
    STRINGID_BELLCHIMED,
    STRINGID_BELLCHIMED,
    STRINGID_BELLCHIMED,
    STRINGID_SOOTHINGAROMA
};

const u16 gFutureMoveUsedStringIds[] = {
    STRINGID_PKMNFORESAWATTACK,
    STRINGID_PKMNCHOSEXASDESTINY
};

const u16 gBallEscapeStringIds[] = {
    STRINGID_PKMNBROKEFREE,
    STRINGID_ITAPPEAREDCAUGHT,
    STRINGID_AARGHALMOSTHADIT,
    STRINGID_SHOOTSOCLOSE
};

const u16 gWeatherContinuesStringIds[] = {
    STRINGID_ITISRAINING, 
	STRINGID_ITISRAINING, 
	STRINGID_ITISRAINING,
    STRINGID_ITISRAINING, 
	STRINGID_STARTEDHAIL, 
	STRINGID_ITISRAINING,
    STRINGID_ITISRAINING, 
	STRINGID_ITISRAINING, 
	STRINGID_SANDSTORMISRAGING,
    STRINGID_ITISRAINING, 
	STRINGID_ITISRAINING, 
	STRINGID_ITISRAINING,
    STRINGID_SUNLIGHTSTRONG, 
	STRINGID_ITISRAINING, 
	STRINGID_ITISRAINING, 
	STRINGID_ITISRAINING,
};

const u16 gInobedientStringIds[] = {
    STRINGID_PKMNLOAFING,
    STRINGID_PKMNWONTOBEY,
    STRINGID_PKMNTURNEDAWAY,
    STRINGID_PKMNPRETENDNOTNOTICE
};

const u16 gSafariPokeblockResultStringIds[] = {
    STRINGID_PKMNWATCHINGCAREFULLY,
    STRINGID_PKMNANGRY,
    STRINGID_PKMNEATING
};

const u16 gTrainerItemCuredStatusStringIds[] = {
    STRINGID_PKMNSITEMSNAPPEDOUT,
    STRINGID_PKMNSITEMCUREDPARALYSIS,
    STRINGID_PKMNSITEMDEFROSTEDIT,
    STRINGID_PKMNSITEMHEALEDBURN,
    STRINGID_PKMNSITEMCUREDPOISON,
    STRINGID_PKMNSITEMWOKEIT
};

const u16 gBerryEffectStringIds[] = {
    STRINGID_PKMNSITEMCUREDPROBLEM,
    STRINGID_PKMNSITEMNORMALIZEDSTATUS
};

const u16 gBRNPreventionStringIds[] = {
    STRINGID_PKMNSXPREVENTSBURNS,
    STRINGID_PKMNSXPREVENTSYSZ,
    STRINGID_PKMNSXHADNOEFFECTONY
};

const u16 gPRLZPreventionStringIds[] = {
    STRINGID_PKMNPREVENTSPARALYSISWITH,
    STRINGID_PKMNSXPREVENTSYSZ,
    STRINGID_PKMNSXHADNOEFFECTONY
};

const u16 gPSNPreventionStringIds[] = {
    STRINGID_PKMNPREVENTSPOISONINGWITH,
    STRINGID_PKMNSXPREVENTSYSZ,
    STRINGID_PKMNSXHADNOEFFECTONY
};

const u16 gItemSwapStringIds[] = {
    STRINGID_PKMNOBTAINEDX,
    STRINGID_PKMNOBTAINEDX2,
    STRINGID_PKMNOBTAINEDXYOBTAINEDZ
};

const u16 gFlashFireStringIds[] = {
    STRINGID_PKMNRAISEDFIREPOWERWITH,
    STRINGID_PKMNSXMADEYINEFFECTIVE
};

const u16 gCaughtMonStringIds[] = {
    STRINGID_PKMNTRANSFERREDSOMEONESPC,
    STRINGID_PKMNTRANSFERREDBILLSPC,
    STRINGID_PKMNBOXSOMEONESPCFULL,
    STRINGID_PKMNBOXBILLSPCFULL
};

const u16 gDoubleBattleRecallStrings[] = {
    STRINGID_TRAINER1MON1COMEBACK,
    STRINGID_TRAINER1MON1COMEBACK,
    STRINGID_TRAINER1MON2COMEBACK,
    STRINGID_TRAINER1MON1AND2COMEBACK
};

const u16 gTrappingMoves[] = {
    MOVE_BIND,
    MOVE_WRAP,
    MOVE_FIRE_SPIN,
    MOVE_CLAMP,
    MOVE_WHIRLPOOL,
    MOVE_SAND_TOMB,
    0xFFFF
};

const u8 gText_PkmnIsEvolving[] = _("What?\n{STR_VAR_1} is evolving!");
const u8 gText_CongratsPkmnEvolved[] = _("Congratulations! Your {STR_VAR_1}\nevolved into {STR_VAR_2}!{WAIT_SE}\p");
const u8 gText_PkmnStoppedEvolving[] = _("Huh? {STR_VAR_1}\nstopped evolving!\p");
const u8 gText_EllipsisQuestionMark[] = _("……?\p");
const u8 gText_WhatWillPkmnDo[] = _("What will\n{B_ACTIVE_NAME_WITH_PREFIX} do?");
const u8 gText_WhatWillPlayerThrow[] = _("What will {B_PLAYER_NAME}\nthrow?");
const u8 gText_WhatWillOldManDo[] = _("What will the\nold man do?");
const u8 gText_LinkStandby[] = _("{PAUSE 16}Link standby…");
const u8 gText_BattleMenu[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Fight{CLEAR_TO 56}Bag\nPokémon{CLEAR_TO 56}Run");
const u8 gText_83FE747[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Ball{CLEAR_TO 56}Bait\nRock{CLEAR_TO 56}Run");
const u8 gText_MoveInterfacePP[] = _("PP");
const u8 gText_MoveInterfaceType[] = _("Type/");
const u8 gText_BattleTextPrefix[] = _("{COLOR 4}");
const u8 gText_83FE779[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}どの わざを\nわすれさせたい?");
const u8 gText_BattleYesNoChoice[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Yes\nNo");
const u8 gText_BattleSwitchWhich[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Switch\nwhich?");
const u8 gText_83FE7B6[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}");
const u8 gText_83FE7BF[] = _("{RIGHT_ARROW_2}");
const u8 gText_83FE7C2[] = _("{PLUS}");
const u8 gText_83FE7C5[] = _("-");
const u8 gText_83FE7C7[] = _("Max HP");
const u8 gText_83FE7D4[] = _("Attack ");
const u8 gText_83FE7DC[] = _("Defense");
const u8 gText_83FE7E4[] = _("Sp. Atk");
const u8 gText_83FE7EC[] = _("Sp. Def");
//SPANISH
const u8 gText_PkmnIsEvolvingSpa[] = _("¡Anda!\n¡{STR_VAR_1} está evolucionando!");
const u8 gText_CongratsPkmnEvolvedSpa[] = _("¡Enhorabuena! ¡Tu {STR_VAR_1}\nse ha convertido en {STR_VAR_2}!{WAIT_SE}\p");
const u8 gText_PkmnStoppedEvolvingSpa[] = _("¿Eh? ¡La evolución de\n{STR_VAR_1} se ha detenido!\p");
const u8 gText_EllipsisQuestionMarkSpa[] = _("... ...?\p");
const u8 gText_WhatWillPkmnDoSpa[] = _("¿Qué debería hacer\n{B_ACTIVE_NAME_WITH_PREFIX}?");
const u8 gText_WhatWillPlayerThrowSpa[] = _("¿Qué hará\n{B_PLAYER_NAME}?");
const u8 gText_WhatWillOldManDoSpa[] = _("¿Qué hará el\nseñor mayor?");
const u8 gText_LinkStandbySpa[] = _("{PAUSE 16}Esperando conexión...");
const u8 gText_BattleMenuSpa[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Lucha{CLEAR_TO 56}Bolsa\nPokémon{CLEAR_TO 56}Huida");
const u8 gText_83FE747Spa[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Ball{CLEAR_TO 56}Cebo\nRoca{CLEAR_TO 56}Huida");
const u8 gText_MoveInterfacePPSpa[] = _("PP ");
const u8 gText_MoveInterfaceTypeSpa[] = _("Tipo/");
const u8 gBattleTextColor[] = _("{COLOR_HIGHLIGHT_SHADOW 2 1 3}");
const u8 gText_83FE779Spa[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}どの わざを\nわすれさせたい?");
const u8 gText_BattleYesNoChoiceSpa[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Sí\nNo");
const u8 gText_BattleSwitchWhichSpa[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}¿Cuál\ncambias?");
const u8 gText_83FE7B6Spa[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}");
const u8 gText_83FE7BFSpa[] = _("{RIGHT_ARROW_2}");
const u8 gText_83FE7C2Spa[] = _("{PLUS}");
const u8 gText_83FE7C5Spa[] = _("-");
const u8 gText_83FE7C7Spa[] = _("PS Máx");
const u8 gText_83FE7D4Spa[] = _("Ataque ");
const u8 gText_83FE7DCSpa[] = _("Defensa");
const u8 gText_83FE7E4Spa[] = _("AT. Esp.");
const u8 gText_83FE7ECSpa[] = _("Def. Esp.");

const u8 *const gText_83FE7F4[] = {
    gText_83FE7C7,
    gText_83FE7E4,
    gText_83FE7D4,
    gText_83FE7EC,
    gText_83FE7DC,
    sText_Speed,
};

const u8 *const gText_83FE7F4Spa[] = {
    gText_83FE7C7Spa,
    gText_83FE7E4Spa,
    gText_83FE7D4Spa,
    gText_83FE7ECSpa,
    gText_83FE7DCSpa,
    sText_SpeedSpa,
};

const u8 gText_SafariBalls[] = _("{HIGHLIGHT 2}Saffari Balls"); //
const u8 gText_HighlightRed_Left[] = _("{HIGHLIGHT 2}Left: ");
const u8 gText_HighlightRed[] = _("{HIGHLIGHT 2}");
const u8 gText_Sleep[] = _("sleep");
const u8 gText_Poison[] = _("poison");
const u8 gText_Burn[] = _("burn");
const u8 gText_Paralysis[] = _("paralysis");
const u8 gText_Ice[] = _("ice");
const u8 gText_Confusion[] = _("confusion");
const u8 gText_Love[] = _("love");
const u8 gText_83FE859[] = _("  ");
const u8 gText_83FE85C[] = _("\n");
const u8 gText_83FE85E[] = _("\n");
const u8 gText_83FE860[] = _(" is");
const u8 gText_83FE864[] = _(" is");
const u8 gText_BadEgg[] = _("Bad Egg");
const u8 gText_83FE870[] = _("ミツル");
const u8 gText_Win[] = _("{HIGHLIGHT 0}Win");
const u8 gText_Loss[] = _("{HIGHLIGHT 0}Loss");
const u8 gText_Draw[] = _("{HIGHLIGHT 0}Draw");
static const u8 sText_SpaceIs[] = _(" is");
static const u8 sText_ApostropheS[] = _("'s");
const u8 gText_83FE892[] = _("a Normal move");
const u8 gText_83FE8A0[] = _("a Fighting move");
const u8 gText_83FE8B0[] = _("a Flying move");
const u8 gText_83FE8BE[] = _("a Poison move");
const u8 gText_83FE8CC[] = _("a Ground move");
const u8 gText_83FE8DA[] = _("a Rock move");
const u8 gText_83FE8E6[] = _("a Bug move");
const u8 gText_83FE8F1[] = _("a Ghost move");
const u8 gText_83FE8FE[] = _("a Steel move");
const u8 gText_83FE90B[] = _("a ??? move");
const u8 gText_83FE916[] = _("a Fire move");
const u8 gText_83FE922[] = _("a Water move");
const u8 gText_83FE92F[] = _("a Grass move");
const u8 gText_83FE93C[] = _("an Electric move");
const u8 gText_83FE94D[] = _("a Psychic move");
const u8 gText_83FE95C[] = _("an Ice move");
const u8 gText_83FE968[] = _("a Dragon move");
const u8 gText_83FE976[] = _("a Dark move");
const u8 gText_TimeBoard[] = _("Time board");
const u8 gText_ClearTime[] = _("Clear Time"); // Unused
const u8 gText_XMinYZSec[] = _("{STR_VAR_1}MIN. {STR_VAR_2}.{STR_VAR_3}SEC.");
const u8 gText_83FE9A9[] = _("1F");
const u8 gText_83FE9AC[] = _("2F");
const u8 gText_83FE9AF[] = _("3F");
const u8 gText_83FE9B2[] = _("4F");
const u8 gText_83FE9B5[] = _("5F");
const u8 gText_83FE9B8[] = _("6F");
const u8 gText_83FE9BB[] = _("7F");
const u8 gText_83FE9BE[] = _("8F");
//Spanish
const u8 gText_SafariBallsSpa[] = _("{HIGHLIGHT 2}Saffari Balls"); //
const u8 gText_HighlightRed_LeftSpa[] = _("{HIGHLIGHT 2}Tienes ");
const u8 gText_HighlightRedSpa[] = _("{HIGHLIGHT 2}");
const u8 gText_SleepSpa[] = _("sueño");
const u8 gText_PoisonSpa[] = _("envenenamiento");
const u8 gText_BurnSpa[] = _("quemaduras");
const u8 gText_ParalysisSpa[] = _("parálisis");
const u8 gText_IceSpa[] = _("congelación");
const u8 gText_ConfusionSpa[] = _("confusión");
const u8 gText_LoveSpa[] = _("enamoramiento");
const u8 gText_83FE859Spa[] = _("  ");
const u8 gText_83FE85CSpa[] = _("\n");
const u8 gText_83FE85ESpa[] = _("\n");
const u8 gText_83FE860Spa[] = _(" no pueden");
const u8 gText_83FE864Spa[] = _(" no pueden");
const u8 gText_BadEggSpa[] = _("Huevo malo");
const u8 gText_83FE870Spa[] = _("ミツル");
const u8 gText_WinSpa[] = _("{HIGHLIGHT 0}Victoria");
const u8 gText_LossSpa[] = _("{HIGHLIGHT 0}Derrota");
const u8 gText_DrawSpa[] = _("{HIGHLIGHT 0}Empate");
static const u8 sText_SpaceIsSpa[] = _(" es");
static const u8 sText_ApostropheSSpa[] = _("de ");
const u8 gText_83FE892Spa[] = _("un mov. Normal");
const u8 gText_83FE8A0Spa[] = _("un mov. Lucha");
const u8 gText_83FE8B0Spa[] = _("un mov. Volador");
const u8 gText_83FE8BESpa[] = _("un mov. Veneno");
const u8 gText_83FE8CCSpa[] = _("un mov. Tierra");
const u8 gText_83FE8DASpa[] = _("un mov. Roca");
const u8 gText_83FE8E6Spa[] = _("un mov. Bicho");
const u8 gText_83FE8F1Spa[] = _("un mov. Fantasma");
const u8 gText_83FE8FESpa[] = _("un mov. Acero");
const u8 gText_83FE90BSpa[] = _("un mov. (?)");
const u8 gText_83FE916Spa[] = _("un mov. Fuego");
const u8 gText_83FE922Spa[] = _("un mov. Agua");
const u8 gText_83FE92FSpa[] = _("un mov. Planta");
const u8 gText_83FE93CSpa[] = _("un mov. Eléctrico");
const u8 gText_83FE94DSpa[] = _("un mov. Psíquico");
const u8 gText_83FE95CSpa[] = _("un mov. Hielo");
const u8 gText_83FE968Spa[] = _("un mov. Dragón");
const u8 gText_83FE976Spa[] = _("un mov. Siniestro");
const u8 gText_TimeBoardSpa[] = _("Marcador");
const u8 gText_ClearTimeSpa[] = _("Tiempo"); // Unused
const u8 gText_XMinYZSecSpa[] = _("{STR_VAR_1} min {STR_VAR_2},{STR_VAR_3} s");
const u8 gText_83FE9A9Spa[] = _("1F");
const u8 gText_83FE9ACSpa[] = _("2F");
const u8 gText_83FE9AFSpa[] = _("3F");
const u8 gText_83FE9B2Spa[] = _("4F");
const u8 gText_83FE9B5Spa[] = _("5F");
const u8 gText_83FE9B8Spa[] = _("6F");
const u8 gText_83FE9BBSpa[] = _("7F");
const u8 gText_83FE9BESpa[] = _("8F");

const u8 *const gTrainerTowerChallengeTypeTexts[NUM_TOWER_CHALLENGE_TYPES] = {
    gText_Single,
    gText_Double,
    gText_Knockout,
    gText_Mixed
};

const u8 gText_83FE9D4[] = _("{PLAY_SE SE_FLEE}{B_TRAINER1_CLASS} {B_TRAINER1_NAME} fled!"); //
static const u8 sText_PlayerLostAgainstTrainer1[] = _("Player lost against\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");
static const u8 sText_PlayerBattledToDrawTrainer1[] = _("Player battled to a draw against\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");

const u8 gText_83FE9D4Spa[] = _("{PLAY_SE SE_FLEE}{B_TRAINER1_CLASS} {B_TRAINER1_NAME} ha huido!"); //
static const u8 sText_PlayerLostAgainstTrainer1Spa[] = _("¡Has perdido contra\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");
static const u8 sText_PlayerBattledToDrawTrainer1Spa[] = _("¡{B_TRAINER1_CLASS} {B_TRAINER1_NAME}\nha empatado contigo!");

static const u8 *const sATypeMove_Table[] = {
    gText_83FE892,
    gText_83FE8A0,
    gText_83FE8B0,
    gText_83FE8BE,
    gText_83FE8CC,
    gText_83FE8DA,
    gText_83FE8E6,
    gText_83FE8F1,
    gText_83FE8FE,
    gText_83FE90B,
    gText_83FE916,
    gText_83FE922,
    gText_83FE92F,
    gText_83FE93C,
    gText_83FE94D,
    gText_83FE95C,
    gText_83FE968,
    gText_83FE976,
};

static const u8 *const sATypeMove_TableSpa[] = {
    gText_83FE892Spa,
    gText_83FE8A0Spa,
    gText_83FE8B0Spa,
    gText_83FE8BESpa,
    gText_83FE8CCSpa,
    gText_83FE8DASpa,
    gText_83FE8E6Spa,
    gText_83FE8F1Spa,
    gText_83FE8FESpa,
    gText_83FE90BSpa,
    gText_83FE916Spa,
    gText_83FE922Spa,
    gText_83FE92FSpa,
    gText_83FE93CSpa,
    gText_83FE94DSpa,
    gText_83FE95CSpa,
    gText_83FE968Spa,
    gText_83FE976Spa,
};

static const u16 sGrammarMoveUsedTable[] = {
    MOVE_SWORDS_DANCE,
    MOVE_STRENGTH,
    MOVE_GROWTH,
    MOVE_HARDEN,
    MOVE_MINIMIZE,
    MOVE_SMOKESCREEN,
    MOVE_WITHDRAW,
    MOVE_DEFENSE_CURL,
    MOVE_EGG_BOMB,
    MOVE_SMOG,
    MOVE_BONE_CLUB,
    MOVE_FLASH,
    MOVE_SPLASH,
    MOVE_ACID_ARMOR,
    MOVE_BONEMERANG,
    MOVE_REST,
    MOVE_SHARPEN,
    MOVE_SUBSTITUTE,
    MOVE_MIND_READER,
    MOVE_SNORE,
    MOVE_PROTECT,
    MOVE_SPIKES,
    MOVE_ENDURE,
    MOVE_ROLLOUT,
    MOVE_SWAGGER,
    MOVE_SLEEP_TALK,
    MOVE_HIDDEN_POWER,
    MOVE_PSYCH_UP,
    MOVE_EXTREME_SPEED,
    MOVE_FOLLOW_ME,
    MOVE_TRICK,
    MOVE_ASSIST,
    MOVE_INGRAIN,
    MOVE_KNOCK_OFF,
    MOVE_CAMOUFLAGE,
    MOVE_ASTONISH,
    MOVE_ODOR_SLEUTH,
    MOVE_GRASS_WHISTLE,
    MOVE_SHEER_COLD,
    MOVE_MUDDY_WATER,
    MOVE_IRON_DEFENSE,
    MOVE_BOUNCE,
    MOVE_NONE,

    MOVE_TELEPORT,
    MOVE_RECOVER,
    MOVE_BIDE,
    MOVE_AMNESIA,
    MOVE_FLAIL,
    MOVE_TAUNT,
    MOVE_BULK_UP,
    MOVE_NONE,

    MOVE_MEDITATE,
    MOVE_AGILITY,
    MOVE_MIMIC,
    MOVE_DOUBLE_TEAM,
    MOVE_BARRAGE,
    MOVE_TRANSFORM,
    MOVE_STRUGGLE,
    MOVE_SCARY_FACE,
    MOVE_CHARGE,
    MOVE_WISH,
    MOVE_BRICK_BREAK,
    MOVE_YAWN,
    MOVE_FEATHER_DANCE,
    MOVE_TEETER_DANCE,
    MOVE_MUD_SPORT,
    MOVE_FAKE_TEARS,
    MOVE_WATER_SPORT,
    MOVE_CALM_MIND,
    MOVE_NONE,

    MOVE_POUND,
    MOVE_SCRATCH,
    MOVE_VICE_GRIP,
    MOVE_WING_ATTACK,
    MOVE_FLY,
    MOVE_BIND,
    MOVE_SLAM,
    MOVE_HORN_ATTACK,
    MOVE_WRAP,
    MOVE_THRASH,
    MOVE_TAIL_WHIP,
    MOVE_LEER,
    MOVE_BITE,
    MOVE_GROWL,
    MOVE_ROAR,
    MOVE_SING,
    MOVE_PECK,
    MOVE_ABSORB,
    MOVE_STRING_SHOT,
    MOVE_EARTHQUAKE,
    MOVE_FISSURE,
    MOVE_DIG,
    MOVE_TOXIC,
    MOVE_SCREECH,
    MOVE_METRONOME,
    MOVE_LICK,
    MOVE_CLAMP,
    MOVE_CONSTRICT,
    MOVE_POISON_GAS,
    MOVE_BUBBLE,
    MOVE_SLASH,
    MOVE_SPIDER_WEB,
    MOVE_NIGHTMARE,
    MOVE_CURSE,
    MOVE_FORESIGHT,
    MOVE_CHARM,
    MOVE_ATTRACT,
    MOVE_ROCK_SMASH,
    MOVE_UPROAR,
    MOVE_SPIT_UP,
    MOVE_SWALLOW,
    MOVE_TORMENT,
    MOVE_FLATTER,
    MOVE_ROLE_PLAY,
    MOVE_ENDEAVOR,
    MOVE_TICKLE,
    MOVE_COVET,
    MOVE_NONE
};

// code
void BufferStringBattle(u16 stringId)
{
    s32 i;
    const u8 *stringPtr = NULL;

    sBattleMsgDataPtr = (struct BattleMsgData*)(&gBattleBufferA[gActiveBattler][4]);
    gLastUsedItem = sBattleMsgDataPtr->lastItem;
    gLastUsedAbility = sBattleMsgDataPtr->lastAbility;
    gBattleScripting.battler = sBattleMsgDataPtr->scrActive;
    *(&gBattleStruct->scriptPartyIdx) = sBattleMsgDataPtr->bakScriptPartyIdx;
    *(&gBattleStruct->hpScale) = sBattleMsgDataPtr->hpScale;
    gPotentialItemEffectBattler = sBattleMsgDataPtr->itemEffectBattler;
    *(&gBattleStruct->stringMoveType) = sBattleMsgDataPtr->moveType;

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        sBattlerAbilities[i] = sBattleMsgDataPtr->abilities[i];
    }
    for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; i++)
    {
        gBattleTextBuff1[i] = sBattleMsgDataPtr->textBuffs[0][i];
        gBattleTextBuff2[i] = sBattleMsgDataPtr->textBuffs[1][i];
        gBattleTextBuff3[i] = sBattleMsgDataPtr->textBuffs[2][i];
    }

    switch (stringId)
    {
    case STRINGID_INTROMSG: // first battle msg
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                {
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_TwoLinkTrainersWantToBattle;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_TwoLinkTrainersWantToBattleSpa;
                }
                else
                {
                    if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
					{
                        if (gSaveBlock2Ptr->optionsLanguage == ENG)
							stringPtr = sText_Trainer1WantsToBattle;
                        if (gSaveBlock2Ptr->optionsLanguage == SPA)
							stringPtr = sText_Trainer1WantsToBattleSpa;
					}
                    else
					{
                        if (gSaveBlock2Ptr->optionsLanguage == ENG)
							stringPtr = sText_LinkTrainerWantsToBattle;
                        if (gSaveBlock2Ptr->optionsLanguage == SPA)
							stringPtr = sText_LinkTrainerWantsToBattleSpa;
					}
                }
            }
            else
            {
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_Trainer1WantsToBattle;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_Trainer1WantsToBattleSpa;
            }
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_GHOST)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_GHOST_UNVEILED)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_TheGhostAppeared;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_TheGhostAppearedSpa;
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_GhostAppearedCantId;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_GhostAppearedCantIdSpa;
				}
            }
            else if (gBattleTypeFlags & BATTLE_TYPE_LEGENDARY)
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_WildPkmnAppeared2;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_WildPkmnAppeared2Spa;
			}
            else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) // interesting, looks like they had something planned for wild double battles
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_TwoWildPkmnAppeared;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_TwoWildPkmnAppearedSpa;
			}
            else if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_WildPkmnAppearedPause;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_WildPkmnAppearedPauseSpa;
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_WildPkmnAppeared;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_WildPkmnAppearedSpa;
			}
        }
        break;
    case STRINGID_INTROSENDOUT: // poke first send-out
        if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_LinkPartnerSentOutPkmnGoPkmn;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_LinkPartnerSentOutPkmnGoPkmnSpa;
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_GoTwoPkmn;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_GoTwoPkmnSpa;
				}
            }
            else
            {
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_GoPkmn;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_GoPkmnSpa;
            }
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_TwoLinkTrainersSentOutPkmn;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_TwoLinkTrainersSentOutPkmnSpa;
				}
                else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_LinkTrainerSentOutTwoPkmn;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_LinkTrainerSentOutTwoPkmnSpa;
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_Trainer1SentOutTwoPkmn;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_Trainer1SentOutTwoPkmnSpa;
				}
            }
            else
            {
                if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_Trainer1SentOutPkmn;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_Trainer1SentOutPkmnSpa;
				}
                else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_Trainer1SentOutPkmn;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_Trainer1SentOutPkmnSpa;
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_LinkTrainerSentOutPkmn;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_LinkTrainerSentOutPkmnSpa;
				}
            }
        }
        break;
    case STRINGID_RETURNMON: // sending poke to ball msg
        if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
        {
            if (*(&gBattleStruct->hpScale) == 0)
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_PkmnThatsEnough;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_PkmnThatsEnoughSpa;
			}
            else if (*(&gBattleStruct->hpScale) == 1 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_PkmnComeBack;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_PkmnComeBackSpa;
			}
            else if (*(&gBattleStruct->hpScale) == 2)
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_PkmnOkComeBack;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_PkmnOkComeBackSpa;
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_PkmnGoodComeBack;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_PkmnGoodComeBackSpa;
			}
        }
        else
        {
            if (gTrainerBattleOpponent_A == TRAINER_LINK_OPPONENT)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_LinkTrainer2WithdrewPkmn;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_LinkTrainer2WithdrewPkmnSpa;
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_LinkTrainer1WithdrewPkmn;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_LinkTrainer1WithdrewPkmnSpa;
				}
            }
            else
            {
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_Trainer1WithdrewPkmn;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_Trainer1WithdrewPkmnSpa;
            }
        }
        break;
    case STRINGID_SWITCHINMON: // switch-in msg
        if (GetBattlerSide(gBattleScripting.battler) == B_SIDE_PLAYER)
        {
            if (*(&gBattleStruct->hpScale) == 0 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
               if (gSaveBlock2Ptr->optionsLanguage == ENG)
				   stringPtr = sText_GoPkmn2;
               if (gSaveBlock2Ptr->optionsLanguage == SPA)
				   stringPtr = sText_GoPkmn2Spa;
			}
            else if (*(&gBattleStruct->hpScale) == 1)
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_DoItPkmn;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_DoItPkmnSpa;
			}
            else if (*(&gBattleStruct->hpScale) == 2)
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_GoForItPkmn;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_GoForItPkmnSpa;
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_YourFoesWeakGetEmPkmn;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_YourFoesWeakGetEmPkmnSpa;
			}
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_LinkTrainerMultiSentOutPkmn;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_LinkTrainerMultiSentOutPkmnSpa;
				}
                else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_Trainer1SentOutPkmn2;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_Trainer1SentOutPkmn2Spa;
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_LinkTrainerSentOutPkmn2;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_LinkTrainerSentOutPkmn2Spa;
				}
            }
            else
            {
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_Trainer1SentOutPkmn2;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_Trainer1SentOutPkmn2Spa;
            }
        }
        break;
    case STRINGID_USEDMOVE: // pokemon used a move msg
        ChooseMoveUsedParticle(gBattleTextBuff1); // buff1 doesn't appear in the string, leftover from japanese move names

        if (sBattleMsgDataPtr->currentMove >= MOVES_COUNT)
		{
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				StringCopy(gBattleTextBuff2, sATypeMove_Table[*(&gBattleStruct->stringMoveType)]);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				StringCopy(gBattleTextBuff2, sATypeMove_TableSpa[*(&gBattleStruct->stringMoveType)]);
		}
        else
		{
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				StringCopy(gBattleTextBuff2, gMoveNames[sBattleMsgDataPtr->currentMove]);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				StringCopy(gBattleTextBuff2, gMoveNamesSpa[sBattleMsgDataPtr->currentMove]);
		}

        ChooseTypeOfMoveUsedString(gBattleTextBuff2);
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			stringPtr = sText_AttackerUsedX;
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			stringPtr = sText_AttackerUsedXSpa;
        break;
    case STRINGID_BATTLEEND: // battle end
        if (gBattleTextBuff1[0] & B_OUTCOME_LINK_BATTLE_RAN)
        {
            gBattleTextBuff1[0] &= ~(B_OUTCOME_LINK_BATTLE_RAN);
            if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTextBuff1[0] == B_OUTCOME_LOST || gBattleTextBuff1[0] == B_OUTCOME_DREW)
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_GotAwaySafely;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_GotAwaySafelySpa;
			}
            else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_TwoWildFled;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_TwoWildFledSpa;
			}
            else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = gText_83FE9D4;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = gText_83FE9D4Spa;
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					stringPtr = sText_WildFled;
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					stringPtr = sText_WildFledSpa;
			}
        }
        else
        {
            if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_TwoLinkTrainersDefeated;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_TwoLinkTrainersDefeatedSpa;
                    break;
                case B_OUTCOME_LOST:
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_PlayerLostToTwo;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_PlayerLostToTwoSpa;
                    break;
                case B_OUTCOME_DREW:
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_PlayerBattledToDrawVsTwo;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_PlayerBattledToDrawVsTwoSpa;
                    break;
                }
            }
            else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_PlayerDefeatedLinkTrainerTrainer1;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_PlayerDefeatedLinkTrainerTrainer1Spa;
                    break;
                case B_OUTCOME_LOST:
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_PlayerLostAgainstTrainer1;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_PlayerLostAgainstTrainer1Spa;
                    break;
                case B_OUTCOME_DREW:
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_PlayerBattledToDrawTrainer1;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_PlayerBattledToDrawTrainer1Spa;
                    break;
                }
            }
            else
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_PlayerDefeatedLinkTrainer;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_PlayerDefeatedLinkTrainerSpa;
                    break;
                case B_OUTCOME_LOST:
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_PlayerLostAgainstLinkTrainer;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_PlayerLostAgainstLinkTrainerSpa;
                    break;
                case B_OUTCOME_DREW:
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						stringPtr = sText_PlayerBattledToDrawLinkTrainer;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						stringPtr = sText_PlayerBattledToDrawLinkTrainerSpa;
                    break;
                }
            }
        }
        break;
    default: // load a string from the table
        if (stringId >= BATTLESTRINGS_COUNT + BATTLESTRINGS_ID_ADDER)
        {
            gDisplayedStringBattle[0] = EOS;
            return;
        }
        else
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				stringPtr = gBattleStringsTable[stringId - BATTLESTRINGS_ID_ADDER];
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				stringPtr = gBattleStringsTableSpa[stringId - BATTLESTRINGS_ID_ADDER];
        }
        break;
    }

    BattleStringExpandPlaceholdersToDisplayedString(stringPtr);
}

u32 BattleStringExpandPlaceholdersToDisplayedString(const u8* src)
{
    BattleStringExpandPlaceholders(src, gDisplayedStringBattle);
}

static const u8* TryGetStatusString(u8 *src)
{
    u32 i;
    u8 status[] = _("$$$$$$$");
    u32 chars1, chars2;
    u8* statusPtr;

    statusPtr = status;
    for (i = 0; i < 8; i++)
    {
        if (*src == EOS)
            break;
        *statusPtr = *src;
        src++;
        statusPtr++;
    }

    chars1 = *(u32*)(&status[0]);
    chars2 = *(u32*)(&status[4]);

    for (i = 0; i < NELEMS(gStatusConditionStringsTable); i++)
    {
        if (chars1 == *(u32*)(&gStatusConditionStringsTable[i][0][0])
            && chars2 == *(u32*)(&gStatusConditionStringsTable[i][0][4]))
            return gStatusConditionStringsTable[i][1];
    }
    return NULL;
}

#define HANDLE_NICKNAME_STRING_CASE(battlerId, monIndex)                	\
    if (gSaveBlock2Ptr->optionsLanguage == ENG)								\
	{																		\
		if (GetBattlerSide(battlerId) != B_SIDE_PLAYER)                     \
		{                                                                   \
			if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     \
				toCpy = sText_FoePkmnPrefix;                                \
			else                                                            \
				toCpy = sText_WildPkmnPrefix;                               \
			while (*toCpy != EOS)                                           \
			{                                                               \
				dst[dstId] = *toCpy;                                        \
				dstId++;                                                    \
				toCpy++;                                                    \
			}                                                               \
			GetMonData(&gEnemyParty[monIndex], MON_DATA_NICKNAME, text);    \
		}                                                                   \
		else                                                                \
		{                                                                   \
			GetMonData(&gPlayerParty[monIndex], MON_DATA_NICKNAME, text);   \
		}                                                                   \
		StringGetEnd10(text);                                               \
		toCpy = text;														\
	}																		\
    if (gSaveBlock2Ptr->optionsLanguage == SPA)								\
	{																		\
		if (GetBattlerSide(battlerId) != B_SIDE_PLAYER)                     \
		{                                                                   \
			GetMonData(&gEnemyParty[monIndex], MON_DATA_NICKNAME, text);    \
			StringGetEnd10(text);                                           \
			toCpy = text;													\
			while (*toCpy != EOS)                                           \
			{                                                               \
				dst[dstId] = *toCpy;                                        \
				dstId++;                                                    \
				toCpy++;                                                    \
			}                                                               \
			if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     \
				toCpy = sText_FoePkmnPrefixSpa;                             \
			else                                                            \
				toCpy = sText_WildPkmnPrefixSpa;                            \
			while (*toCpy != EOS)                                           \
			{                                                               \
				dst[dstId] = *toCpy;                                        \
				dstId++;                                                    \
				toCpy++;                                                    \
			}                                                               \
		}                                                                   \
		else                                                                \
		{                                                                   \
			GetMonData(&gPlayerParty[monIndex], MON_DATA_NICKNAME, text);   \
			StringGetEnd10(text);                                           \
			toCpy = text;													\
		}                                                                   \
	}																		\
	
u32 BattleStringExpandPlaceholders(const u8 *src, u8 *dst)
{
    u32 dstId = 0; // if they used dstId, why not use srcId as well?
    const u8 *toCpy = NULL;
    u8 text[30];
    u8 multiplayerId;
    s32 i;

    multiplayerId = GetMultiplayerId();

    while (*src != EOS)
    {
        if (*src == PLACEHOLDER_BEGIN)
        {
            src++;
            switch (*src)
            {
            case B_TXT_BUFF1:
                if (gBattleTextBuff1[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gStringVar1);
                    toCpy = gStringVar1;
                }
                else
                {
                    toCpy = TryGetStatusString(gBattleTextBuff1);
                    if (toCpy == NULL)
                        toCpy = gBattleTextBuff1;
                }
                break;
            case B_TXT_BUFF2:
                if (gBattleTextBuff2[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff2, gStringVar2);
                    toCpy = gStringVar2;
                }
                else
                    toCpy = gBattleTextBuff2;
                break;
            case B_TXT_BUFF3:
                if (gBattleTextBuff3[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff3, gStringVar3);
                    toCpy = gStringVar3;
                }
                else
                    toCpy = gBattleTextBuff3;
                break;
            case B_TXT_COPY_VAR_1:
                toCpy = gStringVar1;
                break;
            case B_TXT_COPY_VAR_2:
                toCpy = gStringVar2;
                break;
            case B_TXT_COPY_VAR_3:
                toCpy = gStringVar3;
                break;
            case B_TXT_PLAYER_MON1_NAME: // first player poke name
                GetMonData(&gPlayerParty[gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON1_NAME: // first enemy poke name
                GetMonData(&gEnemyParty[gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_PLAYER_MON2_NAME: // second player poke name
                GetMonData(&gPlayerParty[gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON2_NAME: // second enemy poke name
                GetMonData(&gEnemyParty[gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON1_NAME: // link first player poke name
                GetMonData(&gPlayerParty[gBattlerPartyIndexes[gLinkPlayers[multiplayerId].id]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON1_NAME: // link first opponent poke name
                GetMonData(&gEnemyParty[gBattlerPartyIndexes[gLinkPlayers[multiplayerId].id ^ 1]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON2_NAME: // link second player poke name
                GetMonData(&gPlayerParty[gBattlerPartyIndexes[gLinkPlayers[multiplayerId].id ^ 2]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON2_NAME: // link second opponent poke name
                GetMonData(&gEnemyParty[gBattlerPartyIndexes[gLinkPlayers[multiplayerId].id ^ 3]],
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX_MON1: // attacker name with prefix, only battlerId 0/1
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker,
                                            gBattlerPartyIndexes[GetBattlerAtPosition(GET_BATTLER_SIDE(gBattlerAttacker))])
                break;
            case B_TXT_ATK_PARTNER_NAME: // attacker partner name
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
                    GetMonData(
                        &gPlayerParty[gBattlerPartyIndexes[GetBattlerAtPosition(GET_BATTLER_SIDE(gBattlerAttacker)) +
                                                           2]], MON_DATA_NICKNAME, text);
                else
                    GetMonData(
                        &gEnemyParty[gBattlerPartyIndexes[GetBattlerAtPosition(GET_BATTLER_SIDE(gBattlerAttacker)) +
                                                          2]], MON_DATA_NICKNAME, text);

                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX: // attacker name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker])
                break;
            case B_TXT_DEF_NAME_WITH_PREFIX: // target name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerTarget, gBattlerPartyIndexes[gBattlerTarget])
                break;
            case B_TXT_EFF_NAME_WITH_PREFIX: // effect battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gEffectBattler, gBattlerPartyIndexes[gEffectBattler])
                break;
            case B_TXT_ACTIVE_NAME_WITH_PREFIX: // active battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gActiveBattler, gBattlerPartyIndexes[gActiveBattler])
                break;
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX: // scripting active battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting.battler, gBattlerPartyIndexes[gBattleScripting.battler])
                break;
            case B_TXT_CURRENT_MOVE: // current move name
                if (sBattleMsgDataPtr->currentMove >= MOVES_COUNT)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = (const u8 *)&sATypeMove_Table[gBattleStruct->stringMoveType];
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = (const u8 *)&sATypeMove_TableSpa[gBattleStruct->stringMoveType];
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = gMoveNames[sBattleMsgDataPtr->currentMove];
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = gMoveNamesSpa[sBattleMsgDataPtr->currentMove];
				}
                break;
            case B_TXT_LAST_MOVE: // originally used move name
                if (sBattleMsgDataPtr->originallyUsedMove >= MOVES_COUNT)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = (const u8 *)&sATypeMove_Table[gBattleStruct->stringMoveType];
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = (const u8 *)&sATypeMove_TableSpa[gBattleStruct->stringMoveType];
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = gMoveNames[sBattleMsgDataPtr->originallyUsedMove];
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = gMoveNamesSpa[sBattleMsgDataPtr->originallyUsedMove];
				}
                break;
            case B_TXT_LAST_ITEM: // last used item
                if (gBattleTypeFlags & BATTLE_TYPE_LINK)
                {
                    if (gLastUsedItem == ITEM_ENIGMA_BERRY)
                    {
                        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
                        {
                            if ((gBattleStruct->multiplayerId != 0 && (gPotentialItemEffectBattler & BIT_SIDE))
                                || (gBattleStruct->multiplayerId == 0 && !(gPotentialItemEffectBattler & BIT_SIDE)))
                            {
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                if (gSaveBlock2Ptr->optionsLanguage == ENG)
									StringAppend(text, sText_BerrySuffix);
                                if (gSaveBlock2Ptr->optionsLanguage == SPA)
									StringAppend(text, sText_BerrySuffixSpa);
                                toCpy = text;
                            }
                            else
                            {
                                if (gSaveBlock2Ptr->optionsLanguage == ENG)
									toCpy = sText_EnigmaBerry;
                                if (gSaveBlock2Ptr->optionsLanguage == SPA)
									toCpy = sText_EnigmaBerrySpa;
                            }
                        }
                        else
                        {
                            if (gLinkPlayers[gBattleStruct->multiplayerId].id == gPotentialItemEffectBattler)
                            {
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                if (gSaveBlock2Ptr->optionsLanguage == ENG)
									StringAppend(text, sText_BerrySuffix);
                                if (gSaveBlock2Ptr->optionsLanguage == SPA)
									StringAppend(text, sText_BerrySuffixSpa);
                                toCpy = text;
                            }
                            else
							{
                                if (gSaveBlock2Ptr->optionsLanguage == ENG)
									toCpy = sText_EnigmaBerry;
                                if (gSaveBlock2Ptr->optionsLanguage == SPA)
									toCpy = sText_EnigmaBerrySpa;
							}
                        }
                    }
                    else
                    {
                        CopyItemName(gLastUsedItem, text);
                        toCpy = text;
                    }
                }
                else
                {
                    CopyItemName(gLastUsedItem, text);
                    toCpy = text;
                }
                break;
            case B_TXT_LAST_ABILITY: // last used ability
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					toCpy = gAbilityNames[gLastUsedAbility];
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					toCpy = gAbilityNamesSpa[gLastUsedAbility];
                break;
            case B_TXT_ATK_ABILITY: // attacker ability
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					toCpy = gAbilityNames[sBattlerAbilities[gBattlerAttacker]];
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					toCpy = gAbilityNamesSpa[sBattlerAbilities[gBattlerAttacker]];
                break;
            case B_TXT_DEF_ABILITY: // target ability
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					toCpy = gAbilityNames[sBattlerAbilities[gBattlerTarget]];
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					toCpy = gAbilityNamesSpa[sBattlerAbilities[gBattlerTarget]];
                break;
            case B_TXT_SCR_ACTIVE_ABILITY: // scripting active ability
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					toCpy = gAbilityNames[sBattlerAbilities[gBattleScripting.battler]];
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					toCpy = gAbilityNamesSpa[sBattlerAbilities[gBattleScripting.battler]];
                break;
            case B_TXT_EFF_ABILITY: // effect battlerId ability
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					toCpy = gAbilityNames[sBattlerAbilities[gEffectBattler]];
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					toCpy = gAbilityNamesSpa[sBattlerAbilities[gEffectBattler]];
                break;
            case B_TXT_TRAINER1_CLASS: // trainer class name
                if (gTrainerBattleOpponent_A == SECRET_BASE_OPPONENT)
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = gTrainerClassNames[GetSecretBaseTrainerNameIndex()];
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = gTrainerClassNamesSpa[GetSecretBaseTrainerNameIndex()];
                else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = gTrainerClassNames[GetUnionRoomTrainerClass()];
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = gTrainerClassNamesSpa[GetUnionRoomTrainerClass()];
                else if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = gTrainerClassNames[GetBattleTowerTrainerClassNameId()];
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = gTrainerClassNamesSpa[GetBattleTowerTrainerClassNameId()];
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = gTrainerClassNames[GetTrainerTowerOpponentClass()];
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = gTrainerClassNamesSpa[GetTrainerTowerOpponentClass()];
                else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = gTrainerClassNames[GetEreaderTrainerClassId()];
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = gTrainerClassNamesSpa[GetEreaderTrainerClassId()];
                else
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = gTrainerClassNames[gTrainers[gTrainerBattleOpponent_A].trainerClass];
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = gTrainerClassNamesSpa[gTrainers[gTrainerBattleOpponent_A].trainerClass];
                break;
            case B_TXT_TRAINER1_NAME: // trainer1 name
                if (gTrainerBattleOpponent_A == SECRET_BASE_OPPONENT)
                {
                    for (i = 0; i < (s32)NELEMS(gBattleResources->secretBase->trainerName); i++)
                        text[i] = gBattleResources->secretBase->trainerName[i];
                    text[i] = EOS;
                    toCpy = text;
                }
                if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                {
                    toCpy = gLinkPlayers[multiplayerId ^ BIT_SIDE].name;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
                {
                    GetBattleTowerTrainerName(text);
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    GetTrainerTowerOpponentName(text);
                    toCpy = text;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
                {
                    CopyEReaderTrainerName5(text);
                    toCpy = text;
                }
                else
                {
                    if (gTrainers[gTrainerBattleOpponent_A].trainerClass == CLASS_RIVAL
                     || gTrainers[gTrainerBattleOpponent_A].trainerClass == CLASS_RIVAL_2
                     || gTrainers[gTrainerBattleOpponent_A].trainerClass == CLASS_CHAMPION_2)
                        toCpy = GetExpandedPlaceholder(PLACEHOLDER_ID_RIVAL);
                    else
                        toCpy = gTrainers[gTrainerBattleOpponent_A].trainerName;
                }
                break;
            case B_TXT_LINK_PLAYER_NAME: // link player name
                toCpy = gLinkPlayers[multiplayerId].name;
                break;
            case B_TXT_LINK_PARTNER_NAME: // link partner name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_PARTNER(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT1_NAME: // link opponent 1 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT2_NAME: // link opponent 2 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(
                    BATTLE_PARTNER(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id)))].name;
                break;
            case B_TXT_LINK_SCR_TRAINER_NAME: // link scripting active name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(gBattleScripting.battler)].name;
                break;
            case B_TXT_PLAYER_NAME: // player name
                toCpy = gSaveBlock2Ptr->playerName;
                break;
            case B_TXT_TRAINER1_LOSE_TEXT: // trainerA lose text
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    GetTrainerTowerOpponentLoseText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerALoseText();
                }
                break;
            case B_TXT_TRAINER1_WIN_TEXT: // trainerA win text
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    GetTrainerTowerOpponentWinText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerWonSpeech();
                }
                break;
            case B_TXT_TRAINER2_LOSE_TEXT:
                GetTrainerTowerOpponentLoseText(gStringVar4, 1);
                toCpy = gStringVar4;
                break;
            case B_TXT_TRAINER2_WIN_TEXT:
                GetTrainerTowerOpponentWinText(gStringVar4, 1);
                toCpy = gStringVar4;
                break;
            case B_TXT_26: // ?
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting.battler, *(&gBattleStruct->scriptPartyIdx))
                break;
            case B_TXT_PC_CREATOR_NAME: // lanette pc
                if (FlagGet(FLAG_SYS_NOT_SOMEONES_PC))
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_Bills;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_BillsSpa;
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_Someones;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_SomeonesSpa;
				}
                break;
            case B_TXT_ATK_PREFIX2:
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_AllyPkmnPrefix2;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_AllyPkmnPrefix2Spa;
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_FoePkmnPrefix3;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_FoePkmnPrefix3Spa;
				}
                break;
            case B_TXT_DEF_PREFIX2:
                if (GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_AllyPkmnPrefix2;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_AllyPkmnPrefix2Spa;
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_FoePkmnPrefix3;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_FoePkmnPrefix3Spa;
				}
                break;
            case B_TXT_ATK_PREFIX1:
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_AllyPkmnPrefix;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_AllyPkmnPrefixSpa;
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_FoePkmnPrefix2;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_FoePkmnPrefix2Spa;
				}
                break;
            case B_TXT_DEF_PREFIX1:
                if (GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_AllyPkmnPrefix;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_AllyPkmnPrefixSpa;
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_FoePkmnPrefix2;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_FoePkmnPrefix2Spa;
				}
                break;
            case B_TXT_ATK_PREFIX3:
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_AllyPkmnPrefix3;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_AllyPkmnPrefix3Spa;
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_FoePkmnPrefix4;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_FoePkmnPrefix4Spa;
				}
                break;
            case B_TXT_DEF_PREFIX3:
                if (GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_AllyPkmnPrefix3;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_AllyPkmnPrefix3Spa;
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						toCpy = sText_FoePkmnPrefix4;
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						toCpy = sText_FoePkmnPrefix4Spa;
				}
                break;
            }

            // missing if (toCpy != NULL) check
            while (*toCpy != EOS)
            {
                dst[dstId] = *toCpy;
                dstId++;
                toCpy++;
            }
            if (*src == B_TXT_TRAINER1_LOSE_TEXT || *src == B_TXT_TRAINER1_WIN_TEXT
             || *src == B_TXT_TRAINER2_LOSE_TEXT || *src == B_TXT_TRAINER2_WIN_TEXT)
            {
                dst[dstId] = EXT_CTRL_CODE_BEGIN;
                dstId++;
                dst[dstId] = 9;
                dstId++;
            }
        }
        else
        {
            dst[dstId] = *src;
            dstId++;
        }
        src++;
    }

    dst[dstId] = *src;
    dstId++;

    return dstId;
}

static void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst)
{
    u32 srcId = 1;
    u32 value = 0;
    u8 text[12];
    u16 hword;

    *dst = EOS;
    while (src[srcId] != B_BUFF_EOS)
    {
        switch (src[srcId])
        {
        case B_BUFF_STRING: // battle string
            hword = T1_READ_16(&src[srcId + 1]);
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				StringAppend(dst, gBattleStringsTable[hword - BATTLESTRINGS_ID_ADDER]);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				StringAppend(dst, gBattleStringsTableSpa[hword - BATTLESTRINGS_ID_ADDER]);
            srcId += 3;
            break;
        case B_BUFF_NUMBER: // int to string
            switch (src[srcId + 1])
            {
            case 1:
                value = src[srcId + 3];
                break;
            case 2:
                value = T1_READ_16(&src[srcId + 3]);
                break;
            case 4:
                value = T1_READ_32(&src[srcId + 3]);
                break;
            }
            ConvertIntToDecimalStringN(dst, value, STR_CONV_MODE_LEFT_ALIGN, src[srcId + 2]);
            srcId += src[srcId + 1] + 3;
            break;
        case B_BUFF_MOVE: // move name
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				StringAppend(dst, gMoveNames[T1_READ_16(&src[srcId + 1])]);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				StringAppend(dst, gMoveNamesSpa[T1_READ_16(&src[srcId + 1])]);
            srcId += 3;
            break;
        case B_BUFF_TYPE: // type name
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				StringAppend(dst, gTypeNames[src[srcId + 1]]);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				StringAppend(dst, gTypeNamesSpa[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_MON_NICK_WITH_PREFIX: // poke nick with prefix
            if (GetBattlerSide(src[srcId + 1]) == B_SIDE_PLAYER)
            {
                GetMonData(&gPlayerParty[src[srcId + 2]], MON_DATA_NICKNAME, text);
            }
            else
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						StringAppend(dst, sText_FoePkmnPrefix);
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						StringAppend(dst, sText_FoePkmnPrefixSpa);
				}
                else
				{
                    if (gSaveBlock2Ptr->optionsLanguage == ENG)
						StringAppend(dst, sText_WildPkmnPrefix);
                    if (gSaveBlock2Ptr->optionsLanguage == SPA)
						StringAppend(dst, sText_WildPkmnPrefixSpa);
				}

                GetMonData(&gEnemyParty[src[srcId + 2]], MON_DATA_NICKNAME, text);
            }
            StringGetEnd10(text);
            StringAppend(dst, text);
            srcId += 3;
            break;
        case B_BUFF_STAT: // stats
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				StringAppend(dst, gStatNamesTable[src[srcId + 1]]);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				StringAppend(dst, gStatNamesTableSpa[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_SPECIES: // species name
            GetSpeciesName(dst, T1_READ_16(&src[srcId + 1]));
            srcId += 3;
            break;
        case B_BUFF_MON_NICK: // poke nick without prefix
            if (GetBattlerSide(src[srcId + 1]) == B_SIDE_PLAYER)
                GetMonData(&gPlayerParty[src[srcId + 2]], MON_DATA_NICKNAME, dst);
            else
                GetMonData(&gEnemyParty[src[srcId + 2]], MON_DATA_NICKNAME, dst);
            StringGetEnd10(dst);
            srcId += 3;
            break;
        case B_BUFF_NEGATIVE_FLAVOR: // flavor table
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				StringAppend(dst, gPokeblockWasTooXStringTable[src[srcId + 1]]);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				StringAppend(dst, gPokeblockWasTooXStringTableSpa[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_ABILITY: // ability names
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				StringAppend(dst, gAbilityNames[src[srcId + 1]]);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				StringAppend(dst, gAbilityNamesSpa[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_ITEM: // item name
            hword = T1_READ_16(&src[srcId + 1]);
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (hword == ITEM_ENIGMA_BERRY)
                {
                    if (gLinkPlayers[gBattleStruct->multiplayerId].id == gPotentialItemEffectBattler)
                    {
                        StringCopy(dst, gEnigmaBerries[gPotentialItemEffectBattler].name);
                        if (gSaveBlock2Ptr->optionsLanguage == ENG)
							StringAppend(dst, sText_BerrySuffix);
                        if (gSaveBlock2Ptr->optionsLanguage == SPA)
							StringAppend(dst, sText_BerrySuffixSpa);
                    }
                    else
                    {
                        if (gSaveBlock2Ptr->optionsLanguage == ENG)
							StringAppend(dst, sText_EnigmaBerry);
                        if (gSaveBlock2Ptr->optionsLanguage == SPA)
							StringAppend(dst, sText_EnigmaBerrySpa);
                    }
                }
                else
                {
                    CopyItemName(hword, dst);
                }
            }
            else
            {
                CopyItemName(hword, dst);
            }
            srcId += 3;
            break;
        }
    }
}

// Loads one of two text strings into the provided buffer. This is functionally
// unused, since the value loaded into the buffer is not read; it loaded one of
// two particles (either "は" or "の") which works in tandem with ChooseTypeOfMoveUsedString
// below to effect changes in the meaning of the line.
static void ChooseMoveUsedParticle(u8* textBuff)
{
    s32 counter = 0;
    u32 i = 0;

    while (counter != MAX_MON_MOVES)
    {
        if (sGrammarMoveUsedTable[i] == 0)
            counter++;
        if (sGrammarMoveUsedTable[i++] == sBattleMsgDataPtr->currentMove)
            break;
    }

    if (counter >= 0)
    {
        if (counter <= 2)
            StringCopy(textBuff, sText_SpaceIs); // is
        else if (counter <= MAX_MON_MOVES)
            StringCopy(textBuff, sText_ApostropheS); // 's
    }
}

// Appends "!" to the text buffer `dst`. In the original Japanese this looked
// into the table of moves at sGrammarMoveUsedTable and varied the line accordingly.
//
// sText_ExclamationMark was a plain "!", used for any attack not on the list.
// It resulted in the translation "<NAME>'s <Attack>!".
//
// sText_ExclamationMark2 was "を つかった！". This resulted in the translation
// "<NAME> used <Attack>!", which was used for all attacks in English.
//
// sText_ExclamationMark3 was "した！". This was used for those moves whose
// names were verbs, such as Recover, and resulted in translations like "<NAME>
// recovered itself!".
//
// sText_ExclamationMark4 was "を した！" This resulted in a translation of
// "<NAME> did an <Attack>!".
//
// sText_ExclamationMark5 was " こうげき！" This resulted in a translation of
// "<NAME>'s <Attack> attack!".
static void ChooseTypeOfMoveUsedString(u8* dst)
{
    s32 counter = 0;
    s32 i = 0;

    while (*dst != EOS)
        dst++;

    while (counter != MAX_MON_MOVES)
    {
        if (sGrammarMoveUsedTable[i] == MOVE_NONE)
            counter++;
        if (sGrammarMoveUsedTable[i++] == sBattleMsgDataPtr->currentMove)
            break;
    }

    switch (counter)
    {
    case 0:
        StringCopy(dst, sText_ExclamationMark);
        break;
    case 1:
        StringCopy(dst, sText_ExclamationMark2);
        break;
    case 2:
        StringCopy(dst, sText_ExclamationMark3);
        break;
    case 3:
        StringCopy(dst, sText_ExclamationMark4);
        break;
    case 4:
        StringCopy(dst, sText_ExclamationMark5);
        break;
    }
}
//   fillValue fontId x y letterSpacing lineSpacing speed fgColor bgColor shadowColor;
static const struct BattleWindowText sTextOnWindowsInfo_Normal[] = {
    {PIXEL_FILL(0xe), 1, 0x02, 2, 0, 2, 1, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x02, 2, 0, 2, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 2, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x02, 1, 0, 0, 0, 0x2, 0xe, 0x3}, // Move Texts 
    {PIXEL_FILL(0xe), 1, 0x02, 1, 0, 0, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x02, 1, 0, 0, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x02, 1, 0, 0, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x02, 1, 0, 0, 0, 0xc, 0xe, 0xb}, // PP text
    {PIXEL_FILL(0xe), 1, 0x01, 1, 0, 0, 0, 0xc, 0xe, 0xb}, // PP number
    {PIXEL_FILL(0xe), 1, 0x0a, 2, 0, 2, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 2, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 2, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x00, 0, 0, 0, 0, 0x2, 0xe, 0x3}, // Stats
    {PIXEL_FILL(0xe), 1, 0x20, 0, 0, 0, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 1, 2, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 0, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 0, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 0, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 0, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 0, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 0, 0, 0x2, 0xe, 0x3},
    {PIXEL_FILL(0x0), 1, 0x00, 2, 0, 0, 0, 0x1, 0x0, 0x6},
    {PIXEL_FILL(0x0), 1, 0x00, 2, 0, 0, 0, 0x1, 0x0, 0x6},
    {PIXEL_FILL(0x0), 1, 0x00, 2, 0, 0, 0, 0x1, 0x0, 0x6},
    {PIXEL_FILL(0xe), 1, 0x02, 2, 0, 2, 1, 0x2, 0xe, 0x3}, // Tutorial Battle
};

const u8 gUnknown_83FEC90[] = {0x01, 0x01, 0x01, 0x01};

void BattlePutTextOnWindow(const u8 *text, u8 windowId) 
{
    bool32 copyToVram;
    struct TextPrinterTemplate printerTemplate;
    u8 speed;
    int x;
    u8 context;

    u8 textFlags = windowId & 0xC0;
    windowId &= 0x3F;
    if (!(textFlags & 0x80))
        FillWindowPixelBuffer(windowId, sTextOnWindowsInfo_Normal[windowId].fillValue);
    if (textFlags & 0x40) 
	{
        context = ContextNpcGetTextColor();
        printerTemplate.fontId = gUnknown_83FEC90[context];
    }
    else 
	{
        printerTemplate.fontId = sTextOnWindowsInfo_Normal[windowId].fontId;
    }
    switch (windowId)
    {
    case 15 ... 20:
        x = (48 - GetStringWidth(sTextOnWindowsInfo_Normal[windowId].fontId, text,
                                 sTextOnWindowsInfo_Normal[windowId].letterSpacing)) / 2;
        break;
    case 21 ... 23:
        x = (64 - GetStringWidth(sTextOnWindowsInfo_Normal[windowId].fontId, text,
                                 sTextOnWindowsInfo_Normal[windowId].letterSpacing)) / 2;
        break;
    default:
        x = sTextOnWindowsInfo_Normal[windowId].x;
        break;
    }
    if (x < 0)
        x = 0;
    printerTemplate.currentChar = text;
    printerTemplate.windowId = windowId;
    printerTemplate.x = x;
    printerTemplate.y = sTextOnWindowsInfo_Normal[windowId].y;
    printerTemplate.currentX = printerTemplate.x;
    printerTemplate.currentY = printerTemplate.y;
    printerTemplate.letterSpacing = sTextOnWindowsInfo_Normal[windowId].letterSpacing;
    printerTemplate.lineSpacing = sTextOnWindowsInfo_Normal[windowId].lineSpacing;
    printerTemplate.unk = 0;
    printerTemplate.fgColor = sTextOnWindowsInfo_Normal[windowId].fgColor;
    printerTemplate.bgColor = sTextOnWindowsInfo_Normal[windowId].bgColor;
    printerTemplate.shadowColor = sTextOnWindowsInfo_Normal[windowId].shadowColor;
    if (windowId == 24)
        gTextFlags.useAlternateDownArrow = FALSE;
    else
        gTextFlags.useAlternateDownArrow = TRUE;

    if ((gBattleTypeFlags & BATTLE_TYPE_LINK) || ((gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) && windowId != 24))
        gTextFlags.autoScroll = TRUE;
    else
        gTextFlags.autoScroll = FALSE;

    if (windowId == 0 || windowId == 24)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            speed = 1;
        else
            speed = GetTextSpeedSetting();
        gTextFlags.canABSpeedUpPrint = TRUE;
    }
    else
    {
        speed = sTextOnWindowsInfo_Normal[windowId].speed;
        gTextFlags.canABSpeedUpPrint = FALSE;
    }

    AddTextPrinter(&printerTemplate, speed, NULL);
    if (!(textFlags & 0x80))
    {
        PutWindowTilemap(windowId);
        CopyWindowToVram(windowId, COPYWIN_BOTH);
    }
}

bool8 BattleStringShouldBeColored(u16 stringId)
{
    if (stringId == STRINGID_TRAINER1LOSETEXT || stringId == STRINGID_TRAINER2CLASS || stringId == STRINGID_TRAINER1WINTEXT || stringId == STRINGID_TRAINER2NAME)
        return TRUE;
    return FALSE;
}

void SetPpNumbersPaletteInMoveSelection(void) //Carga de paleta de los PP
{
    struct ChooseMoveStruct *chooseMoveStruct = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);
    const u16 *palPtr = gUnknown_8D2FBB4;
    u8 var = GetCurrentPpToMaxPpState(chooseMoveStruct->currentPp[gMoveSelectionCursor[gActiveBattler]],
                                      chooseMoveStruct->maxPp[gMoveSelectionCursor[gActiveBattler]]);

    gPlttBufferUnfaded[0xFC] = palPtr[(var * 2) + 0];
    gPlttBufferUnfaded[0xFB] = palPtr[(var * 2) + 1];

    CpuCopy16(&gPlttBufferUnfaded[0xFC], &gPlttBufferFaded[0xFC], sizeof(u16));
    CpuCopy16(&gPlttBufferUnfaded[0xFB], &gPlttBufferFaded[0xFB], sizeof(u16));
}

u8 GetCurrentPpToMaxPpState(u8 currentPp, u8 maxPp)
{
    if (maxPp == currentPp)
    {
        return 3;
    }
    else if (maxPp <= 2)
    {
        if (currentPp > 1)
            return 3;
        else
            return 2 - currentPp;
    }
    else if (maxPp <= 7)
    {
        if (currentPp > 2)
            return 3;
        else
            return 2 - currentPp;
    }
    else
    {
        if (currentPp == 0)
            return 2;
        if (currentPp <= maxPp / 4)
            return 1;
        if (currentPp > maxPp / 2)
            return 3;
    }

    return 0;
}
