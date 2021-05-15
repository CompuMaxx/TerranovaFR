#ifndef GUARD_BATTLE_BG_H
#define GUARD_BATTLE_BG_H

#include "bg.h"

extern const struct BgTemplate gBattleBgTemplates[];

void InitBattleBgsVideo(void);
void DrawBattleEntryBackground(void);
void InitLinkBattleVsScreen(u8 taskId);
void LoadBattleMenuWindowGfx(void);
void LoadBattleTextboxAndBackground(void);
void LoadBattleTextboxAndBackground2(void);
void sub_800F324(void);
void DrawMainBattleBackground(void);
void DrawMainBattleBackground2(void);

#endif // GUARD_BATTLE_BG_H
