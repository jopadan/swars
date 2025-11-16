/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file game_options.h
 *     Header file for game_options.c.
 * @par Purpose:
 *     Game options storage and manipulation.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     10 Feb 2024 - 02 Nov 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

#include "bftypes.h"
#include "game_bstype.h"
#include "scanner.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#define WEP_ROCKETS_FIRED_LIMIT 15

struct InGame {
    short GameMode;
    short DisplayMode;
    short fld_unk7D8;
    ubyte fld_unk7DA;
    ubyte CDTrack;
    ubyte DangerTrack;
    ubyte UseMultiMedia;
    ubyte AutoResearch;
    ubyte GameOver;
    struct Scanner Scanner; // offset=0x0C
    long Credits; // offset=0x473 (on original Scanner struct)
    ulong fld_unkC4B;	// offset=0x477
    short fld_unkC4F;
    short MissionStatus;
    long Flags;
    ushort fld_unkC57;
    short fld_unkC59;
    short draw_unknprop_01;
    short Rocket1[WEP_ROCKETS_FIRED_LIMIT];	// offset=0x489
    short NextRocket;
    short TrainMode;
    short MyGroup;
    short CurrentMission;
    ubyte fld_unk4AF;
    ubyte DetailLevel;
    short UserZoom;
    short cmdln_param_a;
    short LowerMemoryUse;
    short fld_unkC8B;
    short fld_unkC8D[2];
    long fld_unkC91;
    ushort TrackX;
    ushort TrackZ;
    short TrackThing;
    ubyte fld_unkC98[5];
    short fld_unkCA0;
    ubyte fld_unkCA2;
    sbyte PanelPermutation;
    ubyte TrenchcoatPreference;
    ubyte MissionNo;
    short fld_unkCA6;	// offset=0x4D2
    short fld_unkCA8;
    ubyte fld_unkCAA;
    ubyte PalType;
    short FlameCount;
    ubyte LastTmap;
    short SoundThing;
    ubyte fld_unkCB1;
    ubyte fld_unkCB2;
    short VisibleBillboardThing;
    short fld_unkCB5;
    short fld_unkCB7;
    /** Flags representing active human players, in both net and local games. */
    ubyte InNetGame_UNSURE;
    ubyte fld_unkCBA[5];
    ushort UserFlags;
    long CashAtStart;
    long Expenditure;
};

#pragma pack()
/******************************************************************************/




/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
