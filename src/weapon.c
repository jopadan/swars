/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file weapon.c
 *     Weapons related functions.
 * @par Purpose:
 *     Implement functions for handling weapons.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "weapon.h"

#include "bfmemory.h"
#include "bfendian.h"
#include "bffile.h"
#include "bfini.h"
#include "bfutility.h"
#include "ssampply.h"

#include "bigmap.h"
#include "network.h"
#include "thing.h"
#include "player.h"
#include "game.h"
#include "game_data.h"
#include "game_speed.h"
#include "research.h"
#include "thing_search.h"
#include "wadfile.h"
#include "sound.h"
#include "vehicle.h"
#include "swlog.h"
/******************************************************************************/

struct WeaponDef weapon_defs[] = {
    { 0,    0,  0,  0,   0,  0, 0, 0, WEPDFLG_None,          0, 0,     0,     0,  0},
    { 5,   50,  4,  5,   8, 10, 1, 1, WEPDFLG_CanPurchease, 16, 1,    40,    40, 10},
    { 8,  100,  3,  8,  20, 10, 1, 1, WEPDFLG_CanPurchease, 17, 1,   120,   120, 10},
    { 9,  200,  5,  8,   2, 10, 2, 2, WEPDFLG_CanPurchease|0x01|0x02, 19, 7,   370,   370, 10},
    { 9,  200,  5,  8,   2, 10, 2, 2, WEPDFLG_CanPurchease|0x01|0x02, 20, 7,   420,   420, 10},
    {12,  800, 30,  8, 300, 10, 1, 1, WEPDFLG_CanPurchease,  24, 1,   750,   750, 10},
    { 6,  800, 40,  8, 600, 10, 1, 1, WEPDFLG_CanPurchease,  21, 3,  1000,  1000, 10},
    { 1,    0, 10,  8,   2, 10, 1, 1, WEPDFLG_CanPurchease,  15, 9,   150,   150, 10},
    { 4,   80,  1, 16,   6,  5, 0, 0, WEPDFLG_CanPurchease,  23, 2,   160,   160, 10},
    { 0, 9999,  3, 16,   4,  5, 0, 0, WEPDFLG_CanPurchease,  29, 7,   200,   200, 10},
    { 6,  800, 40,  8, 600, 10, 0, 0, WEPDFLG_CanPurchease,  33, 2,   100,   100, 10},
    { 6,  800, 40,  8, 600, 10, 0, 0, WEPDFLG_CanPurchease,  30, 2,    80,    80, 10},
    { 4,  300,  5, 16,   4,  5, 0, 0, WEPDFLG_CanPurchease,  25, 7,   620,   620, 10},
    { 4, 9999, 40, 16, 200,  5, 0, 0, WEPDFLG_CanPurchease,  27, 1,   480,   480, 10},
    { 4, 9999,  5, 16,   4,  5, 0, 0, WEPDFLG_None,          25, 2,   400,   400, 10},
    {16, 1300, 80,  8, 200, 10, 1, 1, WEPDFLG_CanPurchease,  28, 1,   300,   300, 10},
    { 4, 9999,  5, 16,   4,  5, 0, 0, WEPDFLG_CanPurchease,  35, 3,  1050,  1050, 10},
    {10,  550,  2, 16,   4,  5, 0, 0, WEPDFLG_CanPurchease,  26,10,  2000,  2000, 10},
    {10,   50,  1,  8,   4,  5, 0, 0, WEPDFLG_CanPurchease,  18, 1,   100,   100, 10},
    { 4, 9999,  1, 16,   4,  5, 0, 0, WEPDFLG_None,          24, 6,   850,   850, 10},
    {11, 1500, 10, 16,   4,  5, 0, 0, WEPDFLG_CanPurchease,  38, 6, 65535, 65535, 10},
    { 2,    0, 10,  8,   2, 10, 1, 1, WEPDFLG_CanPurchease,  41, 9,   450,   450, 10},
    { 8, 9999, 20, 16, 200,  5, 0, 0, WEPDFLG_CanPurchease,  39, 6,  1100,  1100, 10},
    { 0, 9999,  1, 16,   4,  5, 0, 0, WEPDFLG_None,          14,10,    80,    80, 10},
    { 4, 9999,  1, 16, 200,  5, 0, 0, WEPDFLG_CanPurchease,  37, 6,  1200,  1200, 10},
    { 8,   25, 80, 16, 600,  5, 0, 0, WEPDFLG_CanPurchease,  40, 6,  1300,  1300, 10},
    { 2,  500, 50, 16, 600,  5, 0, 0, WEPDFLG_CanPurchease,  36, 6,   650,   650, 10},
    { 1, 9999,  1, 16,  60,  5, 0, 0, WEPDFLG_CanPurchease,  22, 5,    30,    30, 10},
    { 1, 9999,  1, 16,  60,  5, 0, 0, WEPDFLG_CanPurchease,  31, 5,   130,   130, 10},
    {10,   50,  1,  8,   4,  5, 0, 0, WEPDFLG_CanPurchease,  32, 1,   950,   950, 10},
    { 4, 9999,  1, 16,   4,  5, 0, 0, WEPDFLG_CanPurchease,  34, 5,  1000,  1000, 10},
    { 0,    0,  0, 16,   4,  5, 0, 0, WEPDFLG_None,          72, 5,  3000,    10,  8},
    { 0,    0,  0, 16,   4,  5, 0, 0, WEPDFLG_None,          72, 5,  3000,    10,  8},
};

ubyte weapon_tech_level[33] = {
  0, 1, 1, 3, 3, 5, 6, 2, 4, 3, 3, 2, 4, 4, 255, 5, 7, 8, 1, 255, 9, 6, 6, 255, 8, 7, 5, 2, 6, 7, 5, 255, 255,
};

struct WeaponDefAdd weapon_defs_a[33] = {0};
struct TbNamedEnum weapon_names[33] = {0};

short persuaded_person_weapons_sell_cost_permil = 0;

short low_energy_alarm_level = 50;

enum WeaponsConfigCmd {
    CCWep_WeaponsCount = 1,
    CCWep_Name,
    CCWep_Category,
    CCWep_RangeBlocks,
    CCWep_HitDamage,
    CCWep_ReFireDelay,
    CCWep_Weight,
    CCWep_EnergyUsed,
    CCWep_Accuracy,
    CCWep_TargetType,
    CCWep_TargetMode,
    CCWep_Sprite,
    CCWep_Cost,
    CCWep_Funding,
    CCWep_PercentPerDay,
};

const struct TbNamedEnum weapons_conf_common_cmds[] = {
  {"WeaponsCount",	CCWep_WeaponsCount},
  {NULL,		0},
};

const struct TbNamedEnum weapons_conf_weapon_cmds[] = {
  {"Name",			CCWep_Name},
  {"Category",	    CCWep_Category},
  {"RangeBlocks",	CCWep_RangeBlocks},
  {"HitDamage",		CCWep_HitDamage},
  {"ReFireDelay",	CCWep_ReFireDelay},
  {"Weight",		CCWep_Weight},
  {"EnergyUsed",	CCWep_EnergyUsed},
  {"Accuracy",		CCWep_Accuracy},
  {"TargetType",	CCWep_TargetType},
  {"TargetMode",	CCWep_TargetMode},
  {"Sprite",		CCWep_Sprite},
  {"Cost",			CCWep_Cost},
  {"ResearchFunding",		CCWep_Funding},
  {"ResearchPercentPerDay",	CCWep_PercentPerDay},
  {NULL,		0},
};

void read_weapons_conf_file(void)
{
    char conf_fname[DISKPATH_SIZE];
    PathInfo *pinfo;
    TbFileHandle conf_fh;
    TbBool done;
    int i;
    s32 k;
    int cmd_num;
    char *conf_buf;
    struct TbIniParser parser;
    int conf_len;
    int weapons_count, wtype;

    pinfo = &game_dirs[DirPlace_Config];
    snprintf(conf_fname, DISKPATH_SIZE-1, "%s/weapons.ini", pinfo->directory);
    conf_fh = LbFileOpen(conf_fname, Lb_FILE_MODE_READ_ONLY);
    if (conf_fh != INVALID_FILE) {
        conf_len = LbFileLengthHandle(conf_fh);
        if (conf_len > 1024*1024)
            conf_len = 1024*1024;
        conf_buf = LbMemoryAlloc(conf_len+16);
        conf_len = LbFileRead(conf_fh, conf_buf, conf_len);
        LOGSYNC("Processing %s file, %d bytes", conf_fname, conf_len);
        LbFileClose(conf_fh);
    } else {
        LOGERR("Could not open weapons config file, going with defaults.");
        conf_buf = LbMemoryAlloc(16);
        conf_len = 0;
    }
    conf_buf[conf_len] = '\0';
    LbIniParseStart(&parser, conf_buf, conf_len);
#define CONFWRNLOG(format,args...) LOGWARN("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
#define CONFDBGLOG(format,args...) LOGDBG("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
    weapons_count = 0;
    // Parse the [common] section of loaded file
    if (LbIniFindSection(&parser, "common") != Lb_SUCCESS) {
        CONFWRNLOG("Could not find \"[%s]\" section, file skipped.", "common");
        LbIniParseEnd(&parser);
        LbMemoryFree(conf_buf);
        return;
    }
    done = false;
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(weapons_conf_common_cmds,cmd_num)
    while (!done)
    {
        // Finding command number in this line
        i = 0;
        cmd_num = LbIniRecognizeKey(&parser, weapons_conf_common_cmds);
        // Now store the config item in correct place
        switch (cmd_num)
        {
        case CCWep_WeaponsCount:
            i = LbIniValueGetLongInt(&parser, &k);
            if (i <= 0) {
                CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            weapons_count = k;
            CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), weapons_count);
            break;
        case 0: // comment
            break;
        case -1: // end of buffer
        case -3: // end of section
            done = true;
            break;
        default:
            CONFWRNLOG("Unrecognized command.");
            break;
        }
        LbIniSkipToNextLine(&parser);
    }
#undef COMMAND_TEXT

    for (wtype = 0; wtype < weapons_count; wtype++)
    {
        char sect_name[32];
        struct WeaponDef *wdef;
        struct WeaponDefAdd *wdefa;

        // Parse the [weaponN] sections of loaded file
        sprintf(sect_name, "weapon%d", wtype);
        wdef = &weapon_defs[wtype];
        wdefa = &weapon_defs_a[wtype];
        if (LbIniFindSection(&parser, sect_name) != Lb_SUCCESS) {
            CONFWRNLOG("Could not find \"[%s]\" section.", sect_name);
            continue;
        }
        done = false;
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(weapons_conf_weapon_cmds,cmd_num)
        while (!done)
        {
            // Finding command number in this line
            i = 0;
            cmd_num = LbIniRecognizeKey(&parser, weapons_conf_weapon_cmds);
            // Now store the config item in correct place
            switch (cmd_num)
            {
            case CCWep_Name:
                i = LbIniValueGetStrWord(&parser, wdefa->Name, sizeof(wdefa->Name));
                if (i <= 0) {
                    CONFWRNLOG("Couldn't read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                CONFDBGLOG("%s \"%s\"", COMMAND_TEXT(cmd_num), (int)wdefa->Name);
                break;
            case CCWep_Category:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                if ((k < 0) || (k > WEP_CATEGORIES_COUNT)) {
                    CONFWRNLOG("Outranged value of \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    k = WEP_CATEGORIES_COUNT;
                }
                wdef->Category = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)(wdef->Category));
                break;
            case CCWep_RangeBlocks:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                if ((k < 0) || (k > WEAPON_RANGE_BLOCKS_LIMIT)) {
                    CONFWRNLOG("Outranged value of \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    k = WEAPON_RANGE_BLOCKS_LIMIT;
                }
                wdef->RangeBlocks = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->RangeBlocks);
                break;
            case CCWep_HitDamage:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->HitDamage = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->HitDamage);
                break;
            case CCWep_ReFireDelay:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->ReFireDelay = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->ReFireDelay);
                break;
            case CCWep_Weight:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->Weight = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->Weight);
                break;
            case CCWep_EnergyUsed:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->EnergyUsed = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->EnergyUsed);
                break;
            case CCWep_Accuracy:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->Accuracy = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->Accuracy);
                break;
            case CCWep_TargetType:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->TargetType = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->TargetType);
                break;
            case CCWep_TargetMode:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->TargetMode = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->TargetMode);
                break;
            case CCWep_Sprite:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->Sprite = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)(wdef->Sprite));
                break;
            case CCWep_Cost:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->Cost = k / 100;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->Cost);
                break;
            case CCWep_Funding:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->Funding = k / 100;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->Funding);
                break;
            case CCWep_PercentPerDay:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                wdef->PercentPerDay = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)wdef->PercentPerDay);
                break;
            case 0: // comment
                break;
            case -1: // end of buffer
            case -3: // end of section
                done = true;
                break;
            default:
                CONFWRNLOG("Unrecognized command.");
                break;
            }
            LbIniSkipToNextLine(&parser);
        }
#undef COMMAND_TEXT
    }

#undef CONFDBGLOG
#undef CONFWRNLOG
    LbIniParseEnd(&parser);
    LbMemoryFree(conf_buf);

    i = 0;
    for (wtype = 1; wtype < weapons_count; wtype++)
    {
        struct WeaponDefAdd *wdefa;

        wdefa = &weapon_defs_a[wtype];
        if (strlen(wdefa->Name) > 0) {
            weapon_names[i].name = wdefa->Name;
            weapon_names[i].num = wtype;
            i++;
        }
    }
    weapon_names[i].name = NULL;
    weapon_names[i].num = 0;
}

const char *weapon_codename(ushort wtype)
{
    struct WeaponDefAdd *wdefa;

    if (wtype >= WEP_TYPES_COUNT)
        return "";

    wdefa = &weapon_defs_a[wtype];
    return wdefa->Name;
}

void init_weapon_text(void)
{
    char locstr[512];
    int weptxt_pos;
    int totlen;
    char *s;
    int i, n;

    totlen = load_file_alltext("textdata/wms.txt", weapon_text);
    if (totlen == Lb_FAIL)
        return;
    if (totlen >= weapon_text_len) {
        LOGERR("Insufficient memory for weapon_text - %d instead of %d", weapon_text_len, totlen);
        totlen = weapon_text_len - 1;
    }

    // TODO change the format to use our INI parser
    s = weapon_text;
    weapon_text[totlen] = '\0';

    // section_start = s;
    weptxt_pos = 0;

    s = strchr(s, '[');
    s++;
    s = strchr(s, ']'); // position at start of WEAPONS section
    s++;

    s += 2;
    while (1)
    {
        if (*s == '[')
            break;

        // Read weapon name
        n = 0;
        while ((*s != '\r') && (*s != '\n'))
        {
            locstr[n] = *s++;
            n++;
        }
        locstr[n] = '\0';
        s += 2;

        // Recognize the weapon name
        for (i = 1; i < WEP_TYPES_COUNT; i++)
        {
            const char *codename;
            codename = weapon_codename(i);
            if (strcmp(codename, locstr) == 0) {
                i--;
                break;
            }
        }
        if (i < WEP_TYPES_COUNT)
        {
            weapon_text_index[i] = weptxt_pos;

            while ((*s != '\r') && (*s != '\n')) {
                weapon_text[weptxt_pos] = *s++;
                weptxt_pos++;
            }
            weapon_text[weptxt_pos] = '\0';
            weptxt_pos++;
            s += 2;

            n = weapon_text_index[i];
            my_preprocess_text(&weapon_text[n]);
        } else {
            LOGERR("Weapon name not recognized: \"%s\"", locstr);
            if (s) s = strpbrk(s, "\r\n");
            if (s) s += 2;
        }
    }

    s = strchr(s, '[');
    s++;
    s = strchr(s, ']'); // position at start of MODS section
    s++;

    s += 2;
    while (1)
    {
        if ((*s == '[') || (*s == '\0'))
            break;

        // Read mod name
        n = 0;
        while ((*s != '\r') && (*s != '\n') && (*s != '\0'))
        {
            locstr[n] = *s++;
            n++;
        }
        locstr[n] = '\0';
        s += 2;

        for (i = 1; i < MOD_TYPES_COUNT; i++)
        {
            const char *codename;
            codename = cybmod_codename(i);
            if (strcmp(codename, locstr) == 0) {
                i--;
                break;
            }
        }
        if (i < MOD_TYPES_COUNT)
        {
            cybmod_text_index[i] = weptxt_pos;

            while ((*s != '\r') && (*s != '\n') && (*s != '\0')) {
                weapon_text[weptxt_pos] = *s++;
                weptxt_pos++;
            }
            weapon_text[weptxt_pos] = '\0';
            weptxt_pos++;
            s += 2;

            n = cybmod_text_index[i];
            my_preprocess_text(&weapon_text[n]);
        } else {
            LOGERR("Cyb Mod name not recognized: \"%s\"", locstr);
            if (s) s = strpbrk(s, "\r\n");
            if (s) s += 2;
        }
    }
}

ushort weapon_sprite_index(ushort wtype, TbBool enabled)
{
    ushort sprid;
    struct WeaponDef *wdef;

    if (wtype >= WEP_TYPES_COUNT)
        return 0;

    wdef = &weapon_defs[wtype];
    // Block of normal weapon sprites is 15..41, lighter are 42..68.
    // Sprites added after that have normal image and ligher image adjacent.
    if (enabled) {
        if (wdef->Sprite < 42)
            sprid = 27 + wdef->Sprite;
        else
            sprid = wdef->Sprite + 1;
    } else {
        sprid = 0 + wdef->Sprite;
    }
    return sprid;
}

TbBool weapon_is_deployed_at_wielder_pos(ushort wtype)
{
    return (wtype == WEP_ELEMINE) || (wtype == WEP_EXPLMINE) ||
      (wtype == WEP_AIRSTRIKE) || (wtype == WEP_CEREBUSIFF) ||
      (wtype == WEP_RAZORWIRE) || (wtype == WEP_EXPLWIRE);
}

TbBool weapon_is_for_throwing(ushort weptype)
{
    return (weptype == WEP_NUCLGREN) || (weptype == WEP_CRAZYGAS) || (weptype == WEP_KOGAS);
}

TbBool weapon_is_breaking_will(ushort weptype)
{
    return (weptype == WEP_PERSUADRTRN) || (weptype == WEP_PERSUADER2);
}

TbBool weapon_is_self_affecting(ushort weptype)
{
    return (weptype == WEP_MEDI1) || (weptype == WEP_MEDI2) || (weptype == WEP_CLONESHLD);
}

ushort weapon_fourpack_index(ushort wtype)
{
    switch (wtype)
    {
    case WEP_NUCLGREN:
        return WFRPK_NUCLGREN;
    case WEP_ELEMINE:
        return WFRPK_ELEMINE;
    case WEP_EXPLMINE:
        return WFRPK_EXPLMINE;
    case WEP_KOGAS:
        return WFRPK_KOGAS;
    case WEP_CRAZYGAS:
        return WFRPK_CRAZYGAS;
    }
    return WFRPK_COUNT;
}

TbBool weapon_is_consumable(ushort wtype)
{
    ushort fp;

    if ((wtype == WEP_AIRSTRIKE) || (wtype == WEP_CEREBUSIFF)
      || (wtype == WEP_MEDI1) || (wtype == WEP_MEDI2))
        return true;

    fp = weapon_fourpack_index(wtype);
    if (fp < WFRPK_COUNT)
        return true;

    return false;
}

TbBool weapon_has_targetting(ushort wtype)
{
    return (wtype == WEP_RAP);
}

TbBool weapon_can_be_charged(ushort wtype)
{
    return (wtype == WEP_LASER) || (wtype == WEP_ELLASER)
      || (wtype == WEP_BEAM) || (wtype == WEP_QDEVASTATOR);
}

TbBool weapons_has_weapon(u32 weapons, ushort wtype)
{
    u32 wepflg = 1 << (wtype-1);
    return (weapons & wepflg) != 0;
}

/** Returns weapon set in given flags with index below last.
 */
ushort weapons_prev_weapon(u32 weapons, ushort last_wtype)
{
    ushort wtype;

    if (last_wtype < 2)
        return 0;

    for (wtype = last_wtype - 1; wtype > 0; wtype--)
    {
        u32 wepflg = 1 << (wtype-1);
        if ((weapons & wepflg) != 0)
            return wtype;
    }
    return 0;
}

void weapons_remove_weapon(u32 *p_weapons, struct WeaponsFourPack *p_fourpacks, ushort wtype)
{
    ushort fp;

    *p_weapons &= ~(1 << (wtype-1));

    fp = weapon_fourpack_index(wtype);
    if (fp < WFRPK_COUNT)
        p_fourpacks->Amount[fp] = 0;
}

TbBool weapons_remove_one_from_npc(u32 *p_weapons, ushort wtype)
{
    ushort fp;
    TbBool was_last;

    if ((*p_weapons & (1 << (wtype-1))) == 0)
        return false;

    was_last = true;
    fp = weapon_fourpack_index(wtype);
    if (fp < WFRPK_COUNT) {
        was_last = false;
    }
    if (was_last)
        *p_weapons &= ~(1 << (wtype-1));
    return true;
}

TbBool weapons_remove_one(u32 *p_weapons, struct WeaponsFourPack *p_fourpacks, ushort wtype)
{
    ushort fp;
    TbBool was_last;

    if ((*p_weapons & (1 << (wtype-1))) == 0)
        return false;

    was_last = true;
    fp = weapon_fourpack_index(wtype);
    if (fp < WFRPK_COUNT) {
        was_last = (p_fourpacks->Amount[fp] <= 1);
        p_fourpacks->Amount[fp]--;
    }
    if (was_last)
        *p_weapons &= ~(1 << (wtype-1));
    return true;
}

/** Remove one weapon from player-controlled person in-game.
 * Player struct contains dumb own array rather than uniform WeaponsFourPack, so it requires
 * this special function. To be removed when possible.
 */
TbBool weapons_remove_one_for_player(u32 *p_weapons,
  ubyte p_plfourpacks[][4], ushort plagent, ushort wtype)
{
    ushort fp;
    TbBool was_last;

    if ((*p_weapons & (1 << (wtype-1))) == 0)
        return false;

    was_last = true;
    fp = weapon_fourpack_index(wtype);
    if (fp < WFRPK_COUNT) {
        was_last = (p_plfourpacks[fp][plagent] <= 1);
        p_plfourpacks[fp][plagent]--;
    }
    if (was_last)
        *p_weapons &= ~(1 << (wtype-1));
    return true;
}

TbBool weapons_add_one(u32 *p_weapons, struct WeaponsFourPack *p_fourpacks, ushort wtype)
{
    ushort fp;
    TbBool is_first;

    if (number_of_set_bits(*p_weapons) >= WEAPONS_CARRIED_MAX_COUNT)
        return false;

    is_first = ((*p_weapons & (1 << (wtype-1))) == 0);

    fp = weapon_fourpack_index(wtype);
    if (fp < WFRPK_COUNT) {
        if ((!is_first) && (p_fourpacks->Amount[fp] > 3))
            return false;

        if (is_first)
            p_fourpacks->Amount[fp] = 1;
        else
            p_fourpacks->Amount[fp]++;
    } else {
        if (!is_first)
            return false;
    }

    if (is_first)
        *p_weapons |= (1 << (wtype-1));

    return true;
}

/** Add one weapon to player-controlled person in-game.
 * Player struct contains dumb own array rather than uniform WeaponsFourPack, so it requires
 * this special function. To be removed when possible.
 */
TbBool weapons_add_one_for_player(u32 *p_weapons,
  ubyte p_plfourpacks[][4], ushort plagent, ushort wtype)
{
    ushort fp;
    TbBool is_first;

    if (number_of_set_bits(*p_weapons) >= WEAPONS_CARRIED_MAX_COUNT)
        return false;

    is_first = ((*p_weapons & (1 << (wtype-1))) == 0);

    fp = weapon_fourpack_index(wtype);
    if (fp < WFRPK_COUNT) {
        if ((!is_first) && (p_plfourpacks[fp][plagent] > 3))
            return false;

        if (is_first)
            p_plfourpacks[fp][plagent] = 1;
        else
            p_plfourpacks[fp][plagent]++;
    } else {
        if (!is_first)
            return false;
    }

    if (is_first)
        *p_weapons |= (1 << (wtype-1));

    return true;
}

void sanitize_weapon_quantities(u32 *p_weapons, struct WeaponsFourPack *p_fourpacks)
{
    ushort wtype;
    ushort n_weapons;

    n_weapons = 0;
    for (wtype = WEP_TYPES_COUNT-1; wtype > 0; wtype--)
    {
        ushort fp, n;
        TbBool has_weapon;

        has_weapon = weapons_has_weapon(*p_weapons, wtype);

        if (has_weapon && (n_weapons > WEAPONS_CARRIED_MAX_COUNT)) {
            weapons_remove_weapon(p_weapons, p_fourpacks, wtype);
            has_weapon = false;
        }

        fp = weapon_fourpack_index(wtype);
        if (fp >= WFRPK_COUNT)
            continue;

        n = p_fourpacks->Amount[fp];

        if (!has_weapon)
            n = 0;
        else if (n < 1)
            n = 1;
        else if (n > 4)
            n = 4;
        p_fourpacks->Amount[fp] = n;
    }
}

short get_hand_weapon_range(struct Thing *p_person, ushort weptype)
{
    struct WeaponDef *wdef;
    short range;

    if (weptype >= WEP_TYPES_COUNT)
        return 0;

    wdef = &weapon_defs[weptype];

    range = TILE_TO_MAPCOORD(wdef->RangeBlocks, 0);

    if (weapon_is_for_throwing(weptype))
        range = (85 * range * (3 + cybmod_arms_level(&p_person->U.UPerson.UMod)) + range) >> 8;

    if (weapon_is_breaking_will(weptype))
        range = (85 * range * (3 + cybmod_brain_level(&p_person->U.UPerson.UMod)) + range) >> 8;

    return range;
}

short current_hand_weapon_range(struct Thing *p_person)
{
#if 0
    short ret;
    asm volatile ("call ASM_current_hand_weapon_range\n"
        : "=r" (ret) : "a" (p_person));
    return ret;
#endif
    return get_hand_weapon_range(p_person, p_person->U.UPerson.CurrentWeapon);
}

int get_weapon_range(struct Thing *p_person)
{
    int ret;
    asm volatile ("call ASM_get_weapon_range\n"
        : "=r" (ret) : "a" (p_person));
    return ret;
}

TbBool current_weapon_has_targetting(struct Thing *p_person)
{
    ushort weptype;

    weptype = p_person->U.UPerson.CurrentWeapon;
    if (weptype >= WEP_TYPES_COUNT)
        return false;

    return weapon_has_targetting(weptype);
}

ubyte find_nth_weapon_held(ushort index, ubyte n)
{
    char ret;
    asm volatile ("call ASM_find_nth_weapon_held\n"
        : "=r" (ret) : "a" (index), "d" (n));
    return ret;
}

u32 person_carried_weapons_pesuaded_sell_value(struct Thing *p_person)
{
    u32 credits;
    ushort weptype;

    credits = 0;
    for (weptype = WEP_TYPES_COUNT-1; weptype > 0; weptype--)
    {
        struct WeaponDef *wdef;

        if (!person_carries_weapon(p_person, weptype))
            continue;

        wdef = &weapon_defs[weptype];
        credits += wdef->Cost * persuaded_person_weapons_sell_cost_permil / 1000;
    }
    return credits;
}

void do_weapon_quantities_net_to_player(struct Thing *p_person)
{
    ushort plyr, plagent;
    ushort weptype;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    plagent = p_person->U.UPerson.ComCur & 3;

    for (weptype = WEP_TYPES_COUNT-1; weptype > 0; weptype--)
    {
        ushort fp, n;

        fp = weapon_fourpack_index(weptype);
        if (fp >= WFRPK_COUNT)
            continue;

        if (person_carries_weapon(p_person, weptype))
            n = net_agents__FourPacks[plyr][plagent].Amount[fp];
        else
            n = 0;
        players[plyr].FourPacks[fp][plagent] = n;
    }
}

void do_weapon_quantities_player_to_net(struct Thing *p_person)
{
    ushort plyr, plagent;
    ushort weptype;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    plagent = p_person->U.UPerson.ComCur & 3;

    for (weptype = WEP_TYPES_COUNT-1; weptype > 0; weptype--)
    {
        ushort fp, n;

        fp = weapon_fourpack_index(weptype);
        if (fp >= WFRPK_COUNT)
            continue;

        if (person_carries_weapon(p_person, weptype))
            n = players[plyr].FourPacks[fp][plagent];
        else
            n = 0;
        net_agents__FourPacks[plyr][plagent].Amount[fp] = n;
    }
}

void do_weapon_quantities_cryo_to_player(struct Thing *p_person)
{
    ushort plyr, plagent;
    ushort weptype;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    plagent = p_person->U.UPerson.ComCur & 3;

    for (weptype = WEP_TYPES_COUNT-1; weptype > 0; weptype--)
    {
        ushort fp, n;

        fp = weapon_fourpack_index(weptype);
        if (fp >= WFRPK_COUNT)
            continue;

        if (person_carries_weapon(p_person, weptype))
            n = cryo_agents.FourPacks[plagent].Amount[fp];
        else
            n = 0;
        players[plyr].FourPacks[fp][plagent] = n;
    }
}

void do_weapon_quantities_max_to_player(struct Thing *p_person)
{
    ushort plyr, plagent;
    ushort weptype;

    plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
    plagent = p_person->U.UPerson.ComCur & 3;

    for (weptype = WEP_TYPES_COUNT-1; weptype > 0; weptype--)
    {
        ushort fp, n;

        fp = weapon_fourpack_index(weptype);
        if (fp >= WFRPK_COUNT)
            continue;

        if (person_carries_weapon(p_person, weptype))
            n = 4;
        else
            n = 0;
        players[plyr].FourPacks[fp][plagent] = n;
    }
}

void do_weapon_quantities1(struct Thing *p_person)
{
    if (in_network_game)
    {
        // No action
    }
    else
    {
        do_weapon_quantities_max_to_player(p_person);
    }
}

void do_weapon_quantities_proper1(struct Thing *p_person)
{
    if (in_network_game)
    {
        do_weapon_quantities_player_to_net(p_person);
    }
    else
    {
        do_weapon_quantities_cryo_to_player(p_person);
    }
}

void init_laser(struct Thing *p_person, ushort timer)
{
    asm volatile ("call ASM_init_laser\n"
        : : "a" (p_person), "d" (timer));
}

void init_laser_6shot(struct Thing *p_person, ushort timer)
{
    struct Thing *p_target;
    struct Thing *p_thing;
    ThingIdx thing;
    ushort group, n_targets;

    p_target = p_person->PTarget;
    if (p_target == NULL)
        return;

    group = p_target->U.UPerson.EffectiveGroup & 0x1F;
    init_laser(p_person, timer);

    n_targets = 0;
    thing = same_type_head[256 + group];
    for (; thing != 0; thing = p_thing->LinkSameGroup)
    {
        if (n_targets >= 5)
            break;
        p_thing = &things[thing];
        if ((p_thing->Flag & TngF_Destroyed) == 0)
        {
            int dist_x, dist_z;
            dist_x = abs(PRCCOORD_TO_MAPCOORD(p_thing->X - p_person->X));
            dist_z = abs(PRCCOORD_TO_MAPCOORD(p_thing->Z - p_person->Z));
            // Simplification to avoid multiplication and square root to get proper distance
            if (dist_x <= dist_z)
                dist_x >>= 1;
            else
                dist_z >>= 1;
            if (dist_x + dist_z + 128 < TILE_TO_MAPCOORD(weapon_defs[WEP_LASER].RangeBlocks,0))
            {
                n_targets++;
                p_person->PTarget = p_thing;
                init_laser(p_person, timer);
            }
        }
    }
    p_person->PTarget = p_target;
}

short init_taser(struct Thing *p_person)
{
    short ret;
    asm volatile ("call ASM_init_taser\n"
        : "=r" (ret) : "a" (p_person));
    return ret;
}

void init_rocket(struct Thing *p_owner)
{
    asm volatile ("call ASM_init_rocket\n"
        : : "a" (p_owner));
}

void init_razor_wire(struct Thing *p_person, ubyte flag)
{
    asm volatile ("call ASM_init_razor_wire\n"
        : : "a" (p_person), "d" (flag));
}

void finalise_razor_wire(struct Thing *p_person)
{
    asm volatile ("call ASM_finalise_razor_wire\n"
        : : "a" (p_person));
}

void init_lay_razor(struct Thing *p_thing, short x, short y, short z, int flag)
{
    asm volatile (
      "push %4\n"
      "call ASM_init_lay_razor\n"
        : : "a" (p_thing), "d" (x), "b" (y), "c" (z), "g" (flag));
}

void update_razor_wire(struct Thing *p_person)
{
    asm volatile ("call ASM_update_razor_wire\n"
        : : "a" (p_person));
}

void init_laser_beam(struct Thing *p_owner, ushort size, ubyte type)
{
    asm volatile ("call ASM_init_laser_beam\n"
        : : "a" (p_owner), "d" (size), "b" (type));
}

void init_laser_guided(struct Thing *p_owner, ushort size)
{
    asm volatile ("call ASM_init_laser_guided\n"
        : : "a" (p_owner), "d" (size));
}

void init_laser_elec(struct Thing *p_owner, ushort size)
{
    asm volatile ("call ASM_init_laser_elec\n"
        : : "a" (p_owner), "d" (size));
}

void init_laser_q_sep(struct Thing *p_owner, ushort size)
{
    init_laser_guided(p_owner, size);
    init_laser_guided(p_owner, size);
    init_laser_guided(p_owner, size);
    init_laser_guided(p_owner, size);
    init_laser_guided(p_owner, size);
    init_laser_beam(p_owner, size, 0x14u);
}

void init_uzi(struct Thing *p_owner)
{
    asm volatile ("call ASM_init_uzi\n"
        : : "a" (p_owner));
}

void init_minigun(struct Thing *p_owner)
{
    asm volatile ("call ASM_init_minigun\n"
        : : "a" (p_owner));
}

void init_flamer(struct Thing *p_owner)
{
    asm volatile ("call ASM_init_flamer\n"
        : : "a" (p_owner));
}

void init_s32_range(struct Thing *p_owner)
{
    asm volatile ("call ASM_init_s32_range\n"
        : : "a" (p_owner));
}

void init_air_strike(struct Thing *p_owner)
{
    asm volatile ("call ASM_init_air_strike\n"
        : : "a" (p_owner));
}

void init_stasis_gun(struct Thing *p_owner)
{
    asm volatile ("call ASM_init_stasis_gun\n"
        : : "a" (p_owner));
}

void init_time_gun(struct Thing *p_owner)
{
    asm volatile ("call ASM_init_time_gun\n"
        : : "a" (p_owner));
}

void init_c_iff(struct Thing *p_owner)
{
    asm volatile ("call ASM_init_c_iff\n"
        : : "a" (p_owner));
}

void init_grenade(struct Thing *p_owner, ushort gtype)
{
    asm volatile ("call ASM_init_grenade\n"
        : : "a" (p_owner), "d" (gtype));
}

void init_v_rocket(struct Thing *p_owner)
{
    asm volatile ("call ASM_init_v_rocket\n"
        : : "a" (p_owner));
}

void init_mech_rocket(struct Thing *p_owner, struct Thing *p_mech, int x, int y, int z)
{
    asm volatile (
      "push %4\n"
      "call ASM_init_mech_rocket\n"
        : : "a" (p_owner), "d" (p_mech), "b" (x), "c" (y), "g" (z));
}

void give_take_me_weapon(struct Thing *p_person, int item, int giveortake, short id)
{
    asm volatile (
      "call ASM_give_take_me_weapon\n"
        : : "a" (p_person), "d" (item), "b" (giveortake), "c" (id));
}

ushort set_player_weapon_turn(struct Thing *p_person, ushort time)
{
    ushort ret;
    asm volatile ("call ASM_set_player_weapon_turn\n"
        : "=r" (ret) : "a" (p_person), "d" (time));
    return ret;
}

void init_fire_weapon(struct Thing *p_person)
{
#if 0
    asm volatile ("call ASM_init_fire_weapon\n"
        : : "a" (p_person));
#else
    struct WeaponDef *wdef;
    int plagent;
    struct Thing *p_target;
    ushort wtype;
    short i;

    wtype = p_person->U.UPerson.CurrentWeapon;
    wdef = &weapon_defs[wtype];
    if ((p_person->Flag & TngF_Unkn0001) != 0)
        return;

    if (((gameturn + p_person->ThingOffset) & 0x7F) == 0)
        process_random_speech(p_person, 1);

    if ((p_person->Type != TT_MINE) && ((p_person->State == PerSt_WAIT) || (p_person->State == PerSt_NONE)))
    {
        p_person->U.UPerson.AnimMode = gun_out_anim(p_person, 1);
        reset_person_frame(p_person);
    }
    if (p_person->U.UPerson.Energy <= wdef->EnergyUsed)
    {
        if (wtype == WEP_MINIGUN) {
            plagent = p_person->U.UPerson.ComCur & 3;
            play_dist_sample(p_person, 88 + plagent, 0x7Fu, 0x40u, 100, 0, 2);
        }
    }
    else
    {
        if (!in_network_game && !is_single_game && (p_person->Flag & TngF_PlayerAgent) != 0
          && !is_research_weapon_completed(wtype) && (LbRandomAnyShort() % 17 < 4))
        {
            // Deplete energy to the level which is barely enough for the shot
            // After the shot takes its energy, the level remaining will be near zero
            p_person->U.UPerson.Energy = wdef->EnergyUsed + 1;
        }

        if (weapon_can_be_charged(wtype) || weapon_has_targetting(wtype)) {
            p_person->Flag |= TngF_WepCharging;
            p_person->U.UPerson.WeaponTimer = 0;
        }

        switch (wtype)
        {
        case WEP_UZI:
            p_person->U.UPerson.Energy -= wdef->EnergyUsed;
            p_person->Flag &= ~TngF_WepCharging;
            if ((p_person->Flag & TngF_PlayerAgent) != 0)
            {
                p_target = p_person->PTarget;
                if (p_target != NULL)
                {
                    if ((p_target->Flag & TngF_Destroyed) == 0)
                        p_person->Flag2 |= TngF_Unkn00200000;
                }
            }
            p_person->U.UPerson.FrameId.Version[4] = 1;
            init_uzi(p_person);
            p_person->U.UPerson.WeaponTurn = wdef->ReFireDelay;
            if ((p_person->Flag2 & TgF2_Unkn0200) == 0)
            {
                if ((p_person->Flag & TngF_PlayerAgent) != 0)
                    i = 0;
                else
                    i = 10;

                if ((p_person->Flag & TngF_PlayerAgent) != 0)
                    plagent = p_person->U.UPerson.ComCur & 3;
                else
                    plagent = 0;

                play_dist_sample(p_person, 80 + plagent, 0x7Fu, 0x40u, 100 + i, -1, 3);
            }
            p_person->Flag2 |= TgF2_Unkn0200;
            break;
        case WEP_MINIGUN:
            p_person->U.UPerson.Energy -= wdef->EnergyUsed;
            p_person->Flag &= ~TngF_WepCharging;
            if ((p_person->Flag & TngF_PlayerAgent) != 0)
            {
                p_target = p_person->PTarget;
                if (p_target != NULL)
                {
                    if ((p_target->Flag & TngF_Destroyed) == 0)
                        p_person->Flag2 |= TngF_Unkn00200000;
                }
            }
            p_person->U.UPerson.FrameId.Version[4] = 3;
            init_minigun(p_person);
            p_person->U.UPerson.WeaponTurn = wdef->ReFireDelay;
            if ((p_person->Flag2 & TgF2_Unkn0200) == 0)
            {
                if ((p_person->Flag & TngF_PlayerAgent) != 0)
                    plagent = p_person->U.UPerson.ComCur & 3;
                else
                    plagent = 0;

                play_dist_sample(p_person, 84 + plagent, 0x7Fu, 0x40u, 100, -1, 3);
            }
            p_person->Flag2 |= TgF2_Unkn0200;
            break;
        case WEP_LASER:
            if ((p_person->Flag & TngF_PlayerAgent) != 0)
            {
                p_target = p_person->PTarget;
                if (p_target != NULL)
                {
                    if ((p_target->Flag & TngF_Destroyed) == 0)
                        p_person->Flag2 |= TngF_Unkn00200000;
                }
            }
            if ((p_person->Flag & TngF_Unkn1000) != 0)
                play_dist_sample(p_person, 7u, 0x7Fu, 0x40u, 100, -1, 3);
            break;
        case WEP_ELLASER:
            if ((p_person->Flag & TngF_PlayerAgent) != 0)
            {
                p_target = p_person->PTarget;
                if (p_target != NULL)
                {
                    if ((p_target->Flag & TngF_Destroyed) == 0)
                        p_person->Flag2 |= TngF_Unkn00200000;
                }
            }
            if (((p_person->Flag2 & TgF2_ExistsOffMap) == 0)
              && ((p_person->Flag & TngF_Unkn1000) != 0))
                play_dist_sample(p_person, 7u, 0x7Fu, 0x40u, 100, -1, 3);
            break;
        case WEP_RAP:
            // Targetted shots are created somewhere else
            break;
        case WEP_NUCLGREN:
            if ((p_person->Flag & TngF_PlayerAgent) != 0)
            {
                person_weapons_remove_one(p_person, wtype);
            }
            p_person->U.UPerson.Energy -= wdef->EnergyUsed;
            init_grenade(p_person, 3);
            play_dist_sample(p_person, 0x24u, 0x7Fu, 0x40u, 100, 0, 2);
            p_person->U.UPerson.WeaponTurn = wdef->ReFireDelay;
            if ((p_person->Flag & TngF_PlayerAgent) == 0)
                choose_best_weapon_for_range(p_person, 1280);
            break;
        case WEP_FLAMER:
            p_person->U.UPerson.Energy -= wdef->EnergyUsed;
            init_flamer(p_person);
            if (p_person->U.UPerson.WeaponTurn == 0)
                p_person->U.UPerson.WeaponTurn = 10;
            break;
        case WEP_CRAZYGAS:
            if ((p_person->Flag & TngF_PlayerAgent) != 0)
            {
                person_weapons_remove_one(p_person, wtype);
            }
            p_person->U.UPerson.Energy -= wdef->EnergyUsed;
            init_grenade(p_person, 4);
            play_dist_sample(p_person, 0x24u, 0x7Fu, 0x40u, 100, 0, 2);
            p_person->U.UPerson.WeaponTurn = wdef->ReFireDelay;
            if ((p_person->Flag & TngF_PlayerAgent) == 0)
                choose_best_weapon_for_range(p_person, 1280);
            break;
        case WEP_KOGAS:
            if ((p_person->Flag & TngF_PlayerAgent) != 0)
            {
                person_weapons_remove_one(p_person, wtype);
            }
            p_person->U.UPerson.Energy -= wdef->EnergyUsed;
            init_grenade(p_person, 5);
            play_dist_sample(p_person, 0x24u, 0x7Fu, 0x40u, 100, 0, 2);
            p_person->U.UPerson.WeaponTurn = wdef->ReFireDelay;
            if ((p_person->Flag & TngF_PlayerAgent) == 0)
                choose_best_weapon_for_range(p_person, 1280);
            break;
        case WEP_ELEMINE:
        case WEP_EXPLMINE:
            if ((p_person->Flag2 & TgF2_Unkn00800000) == 0)
            {
                p_person->Flag2 |= TgF2_Unkn00800000;
                person_init_drop(p_person, wtype);
                p_person->U.UPerson.Energy -= wdef->EnergyUsed;
                p_person->U.UPerson.WeaponTurn = wdef->ReFireDelay;
            }
            break;
        case WEP_LONGRANGE:
            p_person->Flag &= ~TngF_WepCharging;
            p_person->U.UPerson.Energy -= wdef->EnergyUsed;
            if ((p_person->Flag & TngF_PlayerAgent) != 0)
            {
                p_target = p_person->PTarget;
                if (p_target != NULL)
                {
                    if ((p_target->Flag & TngF_Destroyed) == 0)
                        p_person->Flag2 |= TngF_Unkn00200000;
                }
            }
            p_person->U.UPerson.FrameId.Version[4] = 1;
            init_s32_range(p_person);
            p_person->U.UPerson.WeaponTurn = wdef->ReFireDelay;
            play_dist_sample(p_person, 0x21u, 0x7Fu, 0x40u, 100, 0, 3);
            break;
        case WEP_AIRSTRIKE:
            give_take_me_weapon(p_person, wtype, -1, p_person->ThingOffset);
            init_air_strike(p_person);
            break;
        case WEP_BEAM:
            if ((p_person->Flag & TngF_PlayerAgent) != 0)
            {
                p_target = p_person->PTarget;
                if (p_target != NULL)
                {
                    if ((p_target->Flag & TngF_Destroyed) == 0)
                        p_person->Flag2 |= TngF_Unkn00200000;
                }
            }
            if ((p_person->Flag & TngF_Unkn1000) != 0)
                play_dist_sample(p_person, 7u, 0x7Fu, 0x40u, 100, -1, 3);
            break;
        case WEP_QDEVASTATOR:
            if ((p_person->Flag & TngF_PlayerAgent) != 0)
            {
                p_target = p_person->PTarget;
                if (p_target != NULL)
                {
                    if ((p_target->Flag & TngF_Destroyed) == 0)
                        p_person->Flag2 |= TngF_Unkn00200000;
                }
            }
            if ((p_person->Flag & TngF_Unkn1000) != 0)
                play_dist_sample(p_person, 7u, 0x7Fu, 0x40u, 100, -1, 3);
            break;
        case WEP_STASISFLD:
            p_person->U.UPerson.Energy -= wdef->EnergyUsed;
            p_person->Flag &= ~TngF_WepCharging;
            p_person->U.UPerson.FrameId.Version[4] = 1;
            init_stasis_gun(p_person);
            p_person->U.UPerson.WeaponTurn = wdef->ReFireDelay;
            break;
        case WEP_TIMEGUN:
            p_person->U.UPerson.Energy -= wdef->EnergyUsed;
            p_person->Flag &= ~TngF_WepCharging;
            p_person->U.UPerson.FrameId.Version[4] = 1;
            init_time_gun(p_person);
            p_person->U.UPerson.WeaponTurn = wdef->ReFireDelay;
            break;
        case WEP_CEREBUSIFF:
            //TODO all other consumables are kept if NPC uses them; why do we break the rule for IFF?
            { // Remove weapon even if it is NPC
                person_weapons_remove_one(p_person, WEP_CEREBUSIFF);
            }
            p_person->U.UPerson.Energy -= wdef->EnergyUsed;
            p_person->Flag &= ~TngF_WepCharging;
            p_person->U.UPerson.FrameId.Version[4] = 1;
            init_c_iff(p_person);
            p_person->U.UPerson.FrameId.Version[4] = 0;
            p_person->U.UPerson.WeaponTurn = wdef->ReFireDelay;
            break;
        default:
          break;
        }
    }
    if (((p_person->Flag & TngF_WepCharging) == 0) && (p_person->U.UPerson.CurrentWeapon != WEP_FLAMER))
        p_person->Flag &= ~TngF_Unkn20000000;
    if ((p_person->Flag & TngF_PlayerAgent) != 0)
        set_player_weapon_turn(p_person, p_person->U.UPerson.WeaponTurn);
#endif
}

void init_clone_disguise(struct Thing *p_person)
{
    if ((p_person->Flag2 & TgF2_Unkn00400000) != 0)
        return;

    p_person->U.UPerson.AnimMode = 0;
    p_person->U.UPerson.OldSubType = p_person->SubType;
    switch (LbRandomAnyShort() & 3)
    {
    case 0:
        p_person->SubType = SubTT_PERS_BRIEFCASE_M;
        break;
    case 1:
        p_person->SubType = SubTT_PERS_WHITE_BRUN_F;
        break;
    case 2:
        p_person->SubType = SubTT_PERS_WHIT_BLOND_F;
        break;
    case 3:
        p_person->SubType = SubTT_PERS_LETH_JACKT_M;
        break;
    default:
        break;
    }
    reset_person_frame(p_person);
    p_person->Flag2 |= TgF2_Unkn00400000;
}

int gun_out_anim(struct Thing *p_person, ubyte shoot_flag)
{
    int ret;
    asm volatile ("call ASM_gun_out_anim\n"
        : "=r" (ret) : "a" (p_person), "d" (shoot_flag));
    return ret;
}

ushort player_weapon_time(struct Thing *p_person)
{
    ushort ret;
    asm volatile ("call ASM_player_weapon_time\n"
        : "=r" (ret) : "a" (p_person));
    return ret;
}

ThingIdx person_is_non_selected_agent(struct Thing *p_person)
{
    return ((p_person->Flag & TngF_PlayerAgent) != 0) && ((p_person->Flag & TngF_Unkn1000) == 0);
}

ThingIdx person_is_selected_agent(struct Thing *p_person)
{
    return ((p_person->Flag & TngF_PlayerAgent) != 0) && ((p_person->Flag & TngF_Unkn1000) != 0);
}

ThingIdx get_vehicle_passenger_in_player_control(struct Thing *p_vehicle)
{
    ThingIdx passener;

    passener = p_vehicle->U.UVehicle.PassengerHead;
    while (passener != 0)
    {
        struct Thing *p_passenger;

        p_passenger = &things[passener];
        if (p_passenger->Type == TT_PERSON)
        {
            if (person_is_selected_agent(p_passenger))
                return passener;
        }
        passener = p_passenger->U.UPerson.LinkPassenger;
    }
    return 0;
}

void process_vehicle_weapon(struct Thing *p_vehicle, struct Thing *p_person)
{
#if 0
    asm volatile ("call ASM_process_vehicle_weapon\n"
        : : "a" (p_vehicle), "d" (p_person));
#else
    // If another passenger controls the weapon of this vehicle, no action
    if (person_is_non_selected_agent(p_person)
      && (get_vehicle_passenger_in_player_control(p_vehicle) != 0))
        return;

    if ((p_person->Flag & TngF_Unkn20000000) != 0)
    {
        short tdx, tdy, tdz;

        if ((p_person->Flag & TngF_PlayerAgent) != 0)
        {
            PlayerInfo *p_player;
            PlayerIdx plyr;
            ushort plagent;

            plyr = p_person->U.UPerson.ComCur >> 2;
            plagent = p_person->U.UPerson.ComCur & 3;
            p_player = &players[plyr];

            tdx = p_player->UserVX[plagent];
            tdz = p_player->UserVZ[plagent];
            if (p_player->UserVY[plagent] != 0)
            {
                tdy = p_player->UserVY[plagent];
                p_player->UserVY[plagent] = 0;
            }
            else
            {
                tdy = (alt_at_point(tdx, tdz) >> 8) + 20;
            }
        }
        else
        {
            tdx = p_vehicle->U.UVehicle.TargetDX;
            tdy = p_vehicle->U.UVehicle.TargetDY;
            tdz = p_vehicle->U.UVehicle.TargetDZ;
        }

        if (p_vehicle->U.UVehicle.TargetDX != tdx || p_vehicle->U.UVehicle.TargetDZ != tdz)
            p_vehicle->OldTarget = 20000;
        p_vehicle->PTarget = NULL;
        p_vehicle->U.UVehicle.TargetDX = tdx;
        p_vehicle->U.UVehicle.TargetDZ = tdz;
        p_vehicle->U.UVehicle.TargetDY = tdy;
        p_vehicle->Flag |= TngF_Unkn20000000;
    }
    else if (p_person->PTarget != NULL)
    {
        struct Thing *p_target;

        p_vehicle->Flag &= ~TngF_Unkn20000000;
        p_target = p_person->PTarget;
        if (p_vehicle->PTarget != p_target)
        {
            p_vehicle->OldTarget = 2000;
            p_vehicle->PTarget = p_target;
        }
    }

    if (((p_person->Flag & 0x800) != 0) && (p_person->U.UPerson.WeaponTurn == 0)
      && (p_vehicle->OldTarget < 24)
      && ((p_vehicle->PTarget != NULL && p_person->PTarget != NULL)
      || (p_vehicle->Flag & TngF_Unkn20000000) != 0))
    {
        init_v_rocket(p_person);
        p_person->U.UPerson.WeaponTurn = 20;
    }
#endif
}

void mech_unkn_func_05(int owntng, int a2, int a3)
{
    asm volatile ("call ASM_mech_unkn_func_05\n"
        : : "a" (owntng), "d" (a2), "b" (a3));
}

void process_mech_weapon(struct Thing *p_vehicle, struct Thing *p_person)
{
#if 0
    asm volatile ("call ASM_process_mech_weapon\n"
        : : "a" (p_vehicle), "d" (p_person));
#else
    if ((p_person->Flag & TngF_Unkn20000000) != 0)
    {
        short tdx, tdy, tdz;

        p_vehicle->PTarget = NULL;
        p_vehicle->Flag |= TngF_Unkn20000000;
        if ((p_person->Flag & TngF_PlayerAgent) != 0)
        {
            PlayerInfo *p_player;
            PlayerIdx plyr;
            ushort plagent;

            plyr = p_person->U.UPerson.ComCur >> 2;
            plagent = p_person->U.UPerson.ComCur & 3;
            p_player = &players[plyr];

            tdx = p_player->UserVX[plagent];
            tdz = p_player->UserVZ[plagent];
            if (p_player->UserVY[plagent] != 0)
            {
                tdy = p_player->UserVY[plagent];
                p_player->UserVY[plagent] = 0;
            }
            else
            {
                tdy = (alt_at_point(tdx, tdz) >> 8) + 20;
            }
        }
        else
        {
            tdy = p_person->VY;
            tdz = p_person->VZ;
            tdx = p_person->VX;
        }
        if ((p_vehicle->U.UVehicle.TargetDX != tdx) || (p_vehicle->U.UVehicle.TargetDZ != tdz))
        {
            p_vehicle->OldTarget = 20000;
            p_vehicle->U.UVehicle.TargetDY = tdy;
            p_vehicle->U.UVehicle.TargetDZ = tdz;
            p_vehicle->U.UVehicle.TargetDX = tdx;
        }
        p_vehicle->PTarget = NULL;
    }
    else if (p_person->PTarget != NULL)
    {
        struct Thing *p_target;

        p_vehicle->Flag &= ~TngF_Unkn20000000;
        p_target = p_person->PTarget;
        if (p_vehicle->PTarget != p_target)
        {
            p_vehicle->OldTarget = 20000;
            p_vehicle->PTarget = p_target;
        }
    }

    if ((p_person->Flag & TngF_PlayerAgent) == 0)
        p_person->Flag |= TngF_Unkn0800;

    if (((p_person->Flag & 0x0800) != 0) && ((p_vehicle->U.UVehicle.TNode & 0x0004) != 0)
      && (p_person->U.UPerson.WeaponTurn == 0)
      && ((p_vehicle->PTarget != NULL && p_person->PTarget != NULL) || (p_vehicle->Flag & 0x20000000) != 0))
    {
        p_person->U.UVehicle.WeaponTurn = 10;
        if (p_vehicle->OldTarget < 24)
        {
            int val;

            if ((LbRandomAnyShort() & 0x1F) != 0)
                val = ((gameturn & 1) != 0) + 5;
            else
                val = 7;
            mech_unkn_func_05(p_vehicle->Owner, 0, val);
            p_vehicle->U.UVehicle.TNode &= ~0x0004;
        }
    }
    if ((p_vehicle->U.UVehicle.TNode & 0x0001) != 0)
    {
        p_vehicle->U.UVehicle.TNode &= ~0x0001;
        init_mech_rocket(p_person, p_vehicle, mech_unkn_dw_1DC880, mech_unkn_dw_1DC884, mech_unkn_dw_1DC888);
    }
    if ((p_vehicle->U.UVehicle.TNode & 0x0002) != 0)
    {
        p_vehicle->U.UVehicle.TNode &= ~0x0002;
        init_mech_rocket(p_person, p_vehicle, mech_unkn_dw_1DC88C, mech_unkn_dw_1DC890, mech_unkn_dw_1DC894);
    }
    if ((p_vehicle->U.UVehicle.TNode & 0x0008) != 0)
    {
        p_vehicle->U.UVehicle.TNode &= ~0x0008;
        init_laser_guided(p_person, 32);
        init_laser_guided(p_person, 32);
        init_laser_guided(p_person, 32);
        init_laser_guided(p_person, 32);
    }
#endif
}

ushort persuade_power_required(ThingIdx victim)
{
    struct Thing *p_victim;
    ushort ptype;
    ushort persd_pwr_rq;
    short brain_lv;

    if (victim <= 0)
        return 9999;

    p_victim = &things[victim];
    if ((p_victim->Flag2 & TgF2_Unkn00400000) != 0)
        ptype = p_victim->U.UPerson.OldSubType;
    else
        ptype = p_victim->SubType;

    persd_pwr_rq = peep_type_stats[ptype].PersuadeReqd;
    brain_lv = cybmod_brain_level(&p_victim->U.UPerson.UMod);
    if (brain_lv == 5)
        return 9999;
    if (brain_lv == 4)
        persd_pwr_rq = peep_type_stats[SubTT_PERS_AGENT].PersuadeReqd;

    return persd_pwr_rq;
}

TbBool person_can_be_persuaded_now(ThingIdx attacker, ThingIdx target,
  short weapon_range, ubyte target_select, ushort *energy_reqd)
{
    struct Thing *p_target;
    struct Thing *p_attacker;
    short target_persd_pwr_rq;

    if ((target <= 0) || (attacker <= 0))
        return false;

    p_attacker = &things[attacker];

    p_target = &things[target];
    if (p_target->Type != TT_PERSON)
        return false;

    {
        int dist_y;
        short cntr_cor_y;

        cntr_cor_y = PRCCOORD_TO_YCOORD(p_attacker->Y);
        dist_y = abs(PRCCOORD_TO_YCOORD(p_target->Y) - cntr_cor_y);
        if (dist_y >= 1240)
            return false;
    }

    if (((target_select == PTargSelect_Persuader) || (target_select == PTargSelect_PersuadeAdv)) &&
      ((p_target->Flag & (TngF_Unkn40000000|TngF_Persuaded|TngF_Destroyed)) != 0))
        return false;

    // If already harvested the soul
    if ((target_select == PTargSelect_SoulCollect) && ((p_target->Flag2 & TgF2_SoulDepleted) != 0))
        return false;

    // Cannot persuade people from own group
    if (((target_select == PTargSelect_Persuader) || (target_select == PTargSelect_PersuadeAdv)) &&
      (p_target->U.UPerson.EffectiveGroup == p_attacker->U.UPerson.EffectiveGroup))
        return false;

    // Holding a taser prevents both persuasion and soul harvest
    if (p_target->U.UPerson.CurrentWeapon == WEP_H2HTASER)
        return false;

    // Self-affecting not allowed for both persuasion and soul harvest
    if (target == attacker)
        return false;

    // Some people can only be affected by advanced persuader
    if ((target_select == PTargSelect_Persuader) &&
      person_only_affected_by_adv_persuader(target))
        return false;

    target_persd_pwr_rq = persuade_power_required(target);

    // Check if we have enough persuade power to overwhelm the target
    if (((target_select == PTargSelect_Persuader) || (target_select == PTargSelect_PersuadeAdv)) &&
      (target_persd_pwr_rq > p_attacker->U.UPerson.PersuadePower))
        return false;

    // Only player agents require energy to persuade
    *energy_reqd = 0;
    if ((p_attacker->Flag & TngF_PlayerAgent) != 0)
        *energy_reqd = 30 * (target_persd_pwr_rq + 1);
    if (*energy_reqd > 600)
        *energy_reqd = 600;

    // Check if we have enough weapon energy
    if (*energy_reqd > p_attacker->U.UPerson.Energy)
        return false;

    // If under commands to persuade a specific person, accept only that person ignoring anyone else
    if (((target_select == PTargSelect_Persuader) || (target_select == PTargSelect_PersuadeAdv)) &&
      (p_attacker->State == PerSt_PERSUADE_PERSON) && (target != p_attacker->GotoThingIndex))
        return false;

    return true;
}

short process_persuadertron(struct Thing *p_person, ubyte target_select, ushort *energy_reqd)
{
#if 0
    short ret;
    asm volatile ("call ASM_process_persuadertron\n"
        : "=r" (ret) : "a" (p_person), "d" (target_select), "b" (energy_reqd));
    return ret;
#endif
    short cor_x, cor_z;
    short weapon_range;

    cor_x = PRCCOORD_TO_MAPCOORD(p_person->X);
    cor_z = PRCCOORD_TO_MAPCOORD(p_person->Z);

    switch (target_select)
    {
    case PTargSelect_Persuader:
    default:
        weapon_range = get_hand_weapon_range(p_person, WEP_PERSUADRTRN);
        break;
    case PTargSelect_PersuadeAdv:
        weapon_range = get_hand_weapon_range(p_person, WEP_PERSUADER2);
        break;
    case PTargSelect_SoulCollect:
        weapon_range = get_hand_weapon_range(p_person, WEP_SOULGUN);
        break;
    }

    return find_person_which_can_be_persuaded_now(cor_x, cor_z, weapon_range,
      p_person->ThingOffset, target_select, energy_reqd);
}

void get_soul(struct Thing *p_dead, struct Thing *p_person)
{
    asm volatile ("call ASM_get_soul\n"
        : : "a" (p_dead), "d" (p_person));
}

void choose_best_weapon_for_range(struct Thing *p_person, int dist)
{
    asm volatile ("call ASM_choose_best_weapon_for_range\n"
        : : "a" (p_person), "d" (dist));
}

void process_weapon_recoil(struct Thing *p_person)
{
    if (((p_person->Flag2 & TgF2_Unkn0800) == 0) &&
      ((p_person->Flag & (TngF_Unkn20000000|TngF_PlayerAgent)) == (TngF_Unkn20000000|TngF_PlayerAgent)))
    {
        PlayerInfo *p_player;
        ushort plyr, plagent;

        plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
        plagent = p_person->U.UPerson.ComCur & 3;
        p_player = &players[plyr];
        p_person->U.UPerson.ComTimer = -1;
        p_person->VX = p_player->UserVX[plagent];
        p_person->VY = p_player->UserVY[plagent];
        p_person->VZ = p_player->UserVZ[plagent];
    }
    else if ((p_person->Flag & (TngF_Unkn20000000|TngF_Persuaded)) == (TngF_Unkn20000000|TngF_Persuaded))
    {
        PlayerInfo *p_player;
        struct Thing *p_owner;
        ushort plyr;
        short cor_x, cor_y, cor_z;
        short src_x, src_y, src_z;
        int range;

        p_owner = &things[p_person->Owner];
        plyr = (p_owner->U.UPerson.ComCur & 0x1C) >> 2;
        p_player = &players[plyr];
        cor_x = p_player->SpecialItems[0];
        cor_y = p_player->SpecialItems[1];
        cor_z = p_player->SpecialItems[2];

        range = get_weapon_range(p_person);
        src_x = PRCCOORD_TO_MAPCOORD(p_person->X);
        src_y = PRCCOORD_TO_MAPCOORD(p_person->Y);
        src_z = PRCCOORD_TO_MAPCOORD(p_person->Z);
        map_limit_distance_to_target_fast(src_x, src_y, src_z,
          &cor_x, &cor_y, &cor_z, range);

        p_person->U.UPerson.ComTimer = -1;
        p_person->VX = cor_x;
        p_person->VY = cor_y;
        p_person->VZ = cor_z;
    }
}

static void process_energy_recovery(struct Thing *p_person)
{
    if (p_person->U.UPerson.Energy < p_person->U.UPerson.MaxEnergy)
    {
        if ((p_person->Flag2 & TgF2_ExistsOffMap) != 0)
        {
            p_person->U.UPerson.Energy = p_person->U.UPerson.MaxEnergy;
        }
        else if (p_person->U.UPerson.WeaponTurn == 0)
        {
            p_person->U.UPerson.Energy += 2;
        }
    }
}

static void process_health_recovery(struct Thing *p_person)
{
    u32 mask;

    if ((p_person->Flag & TngF_PlayerAgent) != 0)
        mask = 1;
    else
        mask = 7;
    // Chest level 4 means invincible
    if (cybmod_chest_level(&p_person->U.UPerson.UMod) == 4)
    {
        person_set_helath_to_max_limit(p_person);
    }
    if (((mask & gameturn) == 0) && p_person->Health < p_person->U.UPerson.MaxHealth)
    {
        int i;

        i = 4 * (cybmod_chest_level(&p_person->U.UPerson.UMod) + 1);
        if (abs(p_person->U.UPerson.Mood) > 32)
            i >>= 2;
        else if (abs(p_person->U.UPerson.Mood) > 16)
            i >>= 1;
        if (abs(p_person->U.UPerson.Mood) > 64 && (gameturn & 2) != 0)
            i = 0;
        p_person->Health += i;
    }
}

void set_person_weapon_turn(struct Thing *p_person, short n_turn)
{
    if ((p_person->Flag & TngF_PlayerAgent) == 0)
    {
        p_person->U.UPerson.WeaponTurn = n_turn;
    }
    else if ((p_person->Flag2 & TgF2_Unkn0800) == 0)
    {
        PlayerInfo *p_player;
        ushort plyr, plagent;
        ushort weptype;

        plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
        plagent = p_person->U.UPerson.ComCur & 3;
        p_player = &players[plyr];
        weptype = p_person->U.UPerson.CurrentWeapon;
        p_player->WepDelays[plagent][weptype] = n_turn;
        p_person->U.UPerson.WeaponTurn = n_turn;
    }
}

void weapon_consume_energy(struct Thing *p_person, ushort wtype)
{
    struct WeaponDef *wdef;
    short en_used;

    wdef = &weapon_defs[wtype];

    switch (wtype)
    {
    case WEP_CLONESHLD:
        if (in_network_game)
            en_used = wdef->EnergyUsed >> 1;
        else
            en_used = wdef->EnergyUsed;
        break;
    default:
        en_used = wdef->EnergyUsed;
        break;
    }
    p_person->U.UPerson.Energy -= en_used;
}

void process_clone_disguise(struct Thing *p_person)
{
    if ((p_person->Flag2 & TgF2_Unkn00400000) == 0)
        return;

    weapon_consume_energy(p_person, WEP_CLONESHLD);

    if (((p_person->Flag & TngF_PlayerAgent) != 0) && (p_person->U.UPerson.Energy < 0))
        p_person->U.UPerson.CurrentWeapon = WEP_NULL;

    if (p_person->U.UPerson.CurrentWeapon != WEP_CLONESHLD)
    {
        p_person->Flag2 &= ~TgF2_Unkn00400000;
        p_person->SubType = p_person->U.UPerson.OldSubType;
        reset_person_frame(p_person);
    }
}

TbBool person_weapons_remove_one(struct Thing *p_person, ushort weptype)
{
    PlayerInfo *p_player;
    ushort plagent;
    TbBool done;

    p_player = NULL;
    plagent = 0;
    if ((p_person->Flag & TngF_PlayerAgent) != 0)
    {
        PlayerIdx plyr;
        plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
        plagent = p_person->U.UPerson.ComCur & 3;
        p_player = &players[plyr];
    }

    if (p_player != NULL)
        //TODO replace  with weapons_remove_one() call, when FourPacks have unified format
        done = weapons_remove_one_for_player(&p_person->U.UPerson.WeaponsCarried, p_player->FourPacks, plagent, weptype);
    else
        done = weapons_remove_one_from_npc(&p_person->U.UPerson.WeaponsCarried, weptype);

    // If the weapon got depleted, remove it from all properties
    if (!person_carries_weapon(p_person, weptype)) {
        if (p_person->U.UPerson.CurrentWeapon == weptype)
            p_person->U.UPerson.CurrentWeapon = WEP_NULL;
        if ((p_player != NULL) && (p_player->PrevWeapon[plagent] == weptype))
            person_weapons_reset_previous(p_person);
    }

    return done;
}

void person_weapons_reset_previous(struct Thing *p_person)
{
    PlayerInfo *p_player;
    ushort plagent;

    p_player = NULL;
    plagent = 0;
    if ((p_person->Flag & TngF_PlayerAgent) != 0)
    {
        PlayerIdx plyr;
        plyr = (p_person->U.UPerson.ComCur & 0x1C) >> 2;
        plagent = p_person->U.UPerson.ComCur & 3;
        p_player = &players[plyr];
    }

    if (p_player != NULL) {
        p_player->PrevWeapon[plagent] = find_nth_weapon_held(p_person->ThingOffset, 1);
    }

}

void process_automedkit(struct Thing *p_person)
{
    if (!weapons_has_weapon(p_person->U.UPerson.WeaponsCarried, WEP_MEDI2))
        return;
    if (p_person->Health >= p_person->U.UPerson.MaxHealth / 8)
        return;

    p_person->Health = p_person->U.UPerson.MaxHealth;
    person_weapons_remove_one(p_person, WEP_MEDI2);
    play_dist_sample(p_person, 2, 127, 64, 100, 0, 1);
}

void low_energy_alarm_stop(void)
{
    if (IsSamplePlaying(0, 93, 0))
        stop_sample_using_heap(0, 93);
}

void low_energy_alarm_play(void)
{
    if (!IsSamplePlaying(0, 93, 0))
        play_sample_using_heap(0, 93, 127, 64, 100, -1, 3);
}

void process_energy_alarm(struct Thing *p_person)
{
    ThingIdx dcthing;

    if ((p_person->Flag & TngF_Unkn1000) == 0)
        return;

    dcthing = players[local_player_no].DirectControl[0];
    if (p_person->ThingOffset == dcthing)
    {
        if (p_person->U.UPerson.Energy >= low_energy_alarm_level)
        {
            if ((gameturn & 7) == 0)
                low_energy_alarm_stop();
        }
        else
        {
            low_energy_alarm_play();
        }
    }
}

void process_move_while_firing(struct Thing *p_person)
{
    struct Thing *p_vehicle;
    ubyte currWeapon;

    if ((p_person->Flag & TngF_Unkn0800) == 0)
        return;

    currWeapon = p_person->U.UPerson.CurrentWeapon;
    if (currWeapon != 0 && currWeapon != WEP_RAZORWIRE
        && currWeapon != WEP_EXPLWIRE && (p_person->Flag2 & TgF2_Unkn00080000) != 0)
    {
        p_person->U.UPerson.AnimMode = gun_out_anim(p_person, 0);
        reset_person_frame(p_person);
        p_person->Timer1 = 48;
        p_person->StartTimer1 = 48;
        p_person->Flag2 &= ~TgF2_Unkn00080000;
        p_person->Speed = calc_person_speed(p_person);
    }
    p_vehicle = &things[p_person->U.UPerson.Vehicle];
    if (((p_person->Flag & TngF_InVehicle) != 0) && (p_vehicle->State == VehSt_UNKN_45))
    {
        p_person->Flag &= ~TngF_Unkn0800;
    }
    else
    {
        if ((p_person->Flag & TngF_Unkn20000000) == 0)
            check_persons_target(p_person);
        if (p_person->U.UPerson.Target2 != 0)
        {
            check_persons_target2(p_person);
        }
        else if ((p_person->Type != TT_MINE) && (p_person->Flag & TngF_PlayerAgent) == 0)
        {
            struct Thing *p_target;

            p_target = p_person->PTarget;
            if (p_target != NULL)
            {
                int dt_x, dt_z;
                ushort i;

                dt_x = PRCCOORD_TO_MAPCOORD(p_target->X - p_person->X);
                dt_z = PRCCOORD_TO_MAPCOORD(p_target->Z - p_person->Z);
                i = (arctan(dt_x, -dt_z) + 128) & 0x7FF;
                change_player_angle(p_person, i >> 8);
            }
        }
    }
}

void process_weapon_wind_down(struct Thing *p_person)
{
    ushort plagent;

    if ((p_person->Flag2 & TgF2_Unkn0200) == 0)
        return;

    switch (p_person->U.UPerson.CurrentWeapon)
    {
    case WEP_UZI:
        if ((p_person->Flag & TngF_PlayerAgent) != 0)
            plagent = p_person->U.UPerson.ComCur & 3;
        else
            plagent = 0;
        ReleaseLoopedSample(p_person->ThingOffset, 80 + plagent);
        p_person->Flag2 &= ~TgF2_Unkn0200;
        break;
    case WEP_MINIGUN:
        if ((p_person->Flag & TngF_PlayerAgent) != 0)
            plagent = p_person->U.UPerson.ComCur & 3;
        else
            plagent = 0;
        ReleaseLoopedSample(p_person->ThingOffset, 84 + plagent);
        plagent = p_person->U.UPerson.ComCur & 3;
        play_dist_sample(p_person, 88 + plagent, 0x7F, 0x40, 100, 0, 1);
        p_person->Flag2 &= ~TgF2_Unkn0200;
        break;
    }
}

void process_weapon_continuous_fire(struct Thing *p_person)
{
    struct WeaponDef *wdef;
    short prevWepTurn;

    wdef = &weapon_defs[p_person->U.UPerson.CurrentWeapon];
    prevWepTurn = p_person->U.UPerson.WeaponTurn;
    if ((p_person->Flag & TngF_PlayerAgent) != 0)
        p_person->U.UPerson.WeaponTurn = player_weapon_time(p_person);
    if (prevWepTurn)
    {
        if ((p_person->Flag & TngF_PlayerAgent) == 0)
            p_person->U.UPerson.WeaponTurn--;
        if (p_person->U.UPerson.WeaponTurn == 0)
        {
            int animMode;

            animMode = p_person->U.UPerson.AnimMode;
            if ((animMode == 14 || animMode == 15) && p_person->Type != TT_MINE)
            {
                p_person->U.UPerson.AnimMode = gun_out_anim(p_person, 0);
                reset_person_frame(p_person);
            }
            switch (p_person->U.UPerson.CurrentWeapon)
            {
            case WEP_UZI:
                if ((p_person->U.UPerson.Energy <= wdef->EnergyUsed) ||
                  ((p_person->Flag & TngF_Unkn0800) == 0))
                {
                    ushort plagent;

                    if ((p_person->Flag & TngF_PlayerAgent) != 0)
                        plagent = p_person->U.UPerson.ComCur & 3;
                    else
                        plagent = 0;
                    ReleaseLoopedSample(p_person->ThingOffset, 80 + plagent);
                    p_person->Flag2 &= ~TgF2_Unkn0200;
                }
                break;
            case WEP_MINIGUN:
                if ((p_person->U.UPerson.Energy <= wdef->EnergyUsed) ||
                  ((p_person->Flag & TngF_Unkn0800) == 0))
                {
                    ushort plagent;

                    if ((p_person->Flag & TngF_PlayerAgent) != 0)
                        plagent = p_person->U.UPerson.ComCur & 3;
                    else
                        plagent = 0;
                    ReleaseLoopedSample(p_person->ThingOffset, 84 + plagent);
                    plagent = p_person->U.UPerson.ComCur & 3;
                    play_dist_sample(p_person, 88 + plagent, 0x7F, 0x40, 100, 0, 1);
                    p_person->Flag2 &= ~TgF2_Unkn0200;
                }
                break;
            case WEP_FLAMER:
                if ((p_person->U.UPerson.Energy <= wdef->EnergyUsed) ||
                  ((p_person->Flag & TngF_Unkn0800) == 0))
                {
                    stop_sample_using_heap(p_person->ThingOffset, 14);
                    play_dist_sample(p_person, 15, 0x7F, 0x40, 100, 0, 2);
                    p_person->Flag2 &= ~TgF2_Unkn0200;
                }
                break;
            }
        }
    }
}

void process_weapon_wield_affecting_area(struct Thing *p_person, ushort wtype)
{
    ThingIdx targtng;
    ushort energy_rq;

    switch (wtype)
    {
    case WEP_SOULGUN:
        if ((p_person->Health < 2 * p_person->U.UPerson.MaxHealth) &&
          (((gameturn + p_person->ThingOffset) & 7) == 0))
        {
            targtng = process_persuadertron(p_person, PTargSelect_SoulCollect, &energy_rq);
            if (targtng > 0)
                get_soul(&things[targtng], p_person);
        }
        return;
    case WEP_PERSUADRTRN:
        {
            targtng = process_persuadertron(p_person, PTargSelect_Persuader, &energy_rq);
            if (targtng > 0)
                set_person_persuaded(&things[targtng], p_person, energy_rq);
        }
        return;
    case WEP_PERSUADER2:
        {
            targtng = process_persuadertron(p_person, PTargSelect_PersuadeAdv, &energy_rq);
            if (targtng > 0)
                set_person_persuaded(&things[targtng], p_person, energy_rq);
        }
        return;
    case WEP_H2HTASER:
        init_taser(p_person);
        break;
    case WEP_CLONESHLD:
        init_clone_disguise(p_person);
        break;
    default:
        break;
    }
}

void weapon_init_shot(struct Thing *p_person, ushort wtype)
{
    switch (wtype)
    {
    case WEP_LASER:
        if (p_person->SubType == SubTT_PERS_MECH_SPIDER)
            init_laser_6shot(p_person, p_person->U.UPerson.WeaponTimer);
        else
            init_laser(p_person, p_person->U.UPerson.WeaponTimer);
        stop_sample_using_heap(p_person->ThingOffset, 7);
        stop_sample_using_heap(p_person->ThingOffset, 52);
        play_dist_sample(p_person, 18, 0x7F, 0x40, 100, 0, 3);
        break;
    case WEP_ELLASER:
        init_laser_elec(p_person, p_person->U.UPerson.WeaponTimer);
        stop_sample_using_heap(p_person->ThingOffset, 7);
        stop_sample_using_heap(p_person->ThingOffset, 52);
        if ((p_person->Flag2 & TgF2_ExistsOffMap) == 0)
            play_dist_sample(p_person, 6, 0x7F, 0x40, 100, 0, 3);
        break;
    case WEP_RAP:
        init_rocket(p_person);
        stop_sample_using_heap(p_person->ThingOffset, 7);
        stop_sample_using_heap(p_person->ThingOffset, 52);
        break;
    case WEP_BEAM:
         init_laser_beam(p_person, p_person->U.UPerson.WeaponTimer, 17);
         stop_sample_using_heap(p_person->ThingOffset, 7);
         stop_sample_using_heap(p_person->ThingOffset, 52);
         play_dist_sample(p_person, 5, 0x7F, 0x40, 100, 0, 3);
        break;
    case WEP_QDEVASTATOR:
        init_laser_q_sep(p_person, p_person->U.UPerson.WeaponTimer);
        stop_sample_using_heap(p_person->ThingOffset, 7);
        stop_sample_using_heap(p_person->ThingOffset, 52);
        play_dist_sample(p_person, 28, 0x7F, 0x40, 100, 0, 3);
        break;
    default:
        break;
    }
}

void process_wielded_weapon_fire(struct Thing *p_person, ushort wtype)
{
    struct WeaponDef *wdef;
    short wepTurn;

    wdef = &weapon_defs[wtype];
    wepTurn = p_person->U.UPerson.WeaponTurn;
    if ((wepTurn == 0) || (wepTurn < wdef->ReFireDelay - 6))
        p_person->U.UPerson.FrameId.Version[4] = 0;

    switch (wtype)
    {
    case WEP_EXPLWIRE:
        if ((p_person->Flag2 & TgF2_Unkn0001) == 0)
        {
            if ((p_person->Flag & TngF_Unkn0800) != 0)
                init_razor_wire(p_person, 1);
        }
        else if (((p_person->Flag & TngF_Unkn0800) != 0)
            || ((p_person->Flag2 & TgF2_Unkn0004) != 0))
        {
            update_razor_wire(p_person);
        }
        else
        {
            finalise_razor_wire(p_person);
        }
        break;
    case WEP_RAZORWIRE:
        if ((p_person->Flag2 & TgF2_Unkn0001) == 0)
        {
            if ((p_person->Flag & TngF_Unkn0800) != 0)
                init_razor_wire(p_person, 0);
        }
        else if (((p_person->Flag & TngF_Unkn0800) != 0)
            || ((p_person->Flag2 & TgF2_Unkn0004) != 0))
        {
            update_razor_wire(p_person);
        }
        else
        {
            finalise_razor_wire(p_person);
        }
        break;
    case WEP_FLAMER:
        if (p_person->U.UPerson.WeaponTurn == 1)
        {
            if ((p_person->Flag & TngF_PlayerAgent) == 0)
                p_person->Flag &= ~TngF_Unkn0800;
        }
        if (p_person->U.UPerson.WeaponTurn <= 1)
        {
            if ((p_person->Flag & TngF_Unkn0800) != 0)
            {
                if ((p_person->U.UPerson.WeaponTurn == 0)
                    && ((p_person->Flag2 & TgF2_Unkn0400) == 0))
                {
                    play_dist_sample(p_person, 14, 0x7F, 0x40, 100, -1, 2);
                    play_dist_sample(p_person, 13, 0x7F, 0x40, 100, 0, 2);
                    p_person->Flag2 |= TgF2_Unkn0200;
                }
                init_fire_weapon(p_person);
                p_person->U.UPerson.WeaponTurn = 5;
            }
            else
            {
                p_person->U.UPerson.WeaponDir = 32
                    * p_person->U.UPerson.Angle;
            }
        }
        else
        {
            init_fire_weapon(p_person);
        }
        break;
    default:
        if (((p_person->Flag & TngF_Unkn0800) != 0) && (p_person->U.UPerson.CurrentWeapon != WEP_NULL)
            && (p_person->U.UPerson.WeaponTurn == 0) && ((p_person->Flag & TngF_WepCharging) == 0))
        {
            process_weapon_recoil(p_person);
            init_fire_weapon(p_person);
        }
        break;
    }
}

void weapon_inc_timer(struct Thing *p_person, ushort wtype)
{
    p_person->U.UPerson.WeaponTimer++;
    if (wtype == WEP_RAP)
        p_person->U.UPerson.WeaponTimer++;
}

void weapon_set_timer_for_refire(struct Thing *p_person, ushort wtype)
{
    struct WeaponDef *wdef;
    int i;
    short reFireShift;

    wdef = &weapon_defs[wtype];

    reFireShift = 0;
    if (p_person->U.UPerson.WeaponTimer >= 5)
    {
        i = p_person->U.UPerson.WeaponTimer - 3;
        set_person_weapon_turn(p_person, i);
    }
    else
    {
        i = p_person->U.UPerson.WeaponTimer;
        set_person_weapon_turn(p_person, i);
        p_person->U.UPerson.WeaponTimer = 5;
        reFireShift = 5 - i;
    }

    switch (wtype)
    {
    case WEP_LASER:
        if (p_person->SubType == SubTT_PERS_MECH_SPIDER)
            p_person->U.UPerson.WeaponTurn = 1;
        else
            p_person->U.UPerson.WeaponTurn = reFireShift + wdef->ReFireDelay;
        break;
    case WEP_RAP:
    case WEP_ELLASER:
    case WEP_BEAM:
    case WEP_QDEVASTATOR:
        p_person->U.UPerson.WeaponTurn = reFireShift + wdef->ReFireDelay;
        break;
    default:
        break;
    }
    // We probably updated WeaponTurn, now do the update properly
    set_person_weapon_turn(p_person, p_person->U.UPerson.WeaponTurn);
}

void process_wielded_weapon(struct Thing *p_person)
{
    ushort wtype;

    wtype = p_person->U.UPerson.CurrentWeapon;

    process_weapon_wield_affecting_area(p_person, wtype);

    process_clone_disguise(p_person);

    process_wielded_weapon_fire(p_person, wtype);

    if ((p_person->Flag & TngF_WepCharging) != 0)
    {
        if (!weapon_has_targetting(wtype))
        {
            if (p_person->U.UPerson.WeaponTimer > 15)
                p_person->U.UPerson.WeaponTimer = 15;
            else
                p_person->U.UPerson.Energy -= 8;
        }

        switch (wtype)
        {
        case WEP_LASER:
            p_person->U.UPerson.FrameId.Version[4] = 2;
            break;
        case WEP_RAP:
            p_person->U.UPerson.FrameId.Version[4] = 0;
            break;
        default:
            p_person->U.UPerson.FrameId.Version[4] = 4;
            break;
        }

        if ((p_person->Flag & TngF_Unkn0800) != 0)
        {
            if (p_person->U.UPerson.WeaponTimer == 14)
            {
                stop_sample_using_heap(p_person->ThingOffset, 7);
                play_dist_sample(p_person, 52, 0x7F, 0x40, 100, -1, 3);
            }
            else
            {
                SetSamplePitch(p_person->ThingOffset, 7,
                    2 * p_person->U.UPerson.WeaponTimer + 100);
            }
            weapon_inc_timer(p_person, wtype);

            if ((p_person->Flag & (TngF_Unkn20000000|TngF_PlayerAgent)) != (TngF_Unkn20000000|TngF_PlayerAgent))
            {
                int resp_time;

                if ((p_person->Health < (p_person->U.UPerson.MaxHealth >> 1)
                    || ((p_person->Flag & TngF_InVehicle) != 0)
                    || (p_person->ThingOffset & 3) != 0)
                    && ((p_person->Flag & TngF_PlayerAgent) == 0))
                {
                    resp_time = 5;
                }
                else
                {
                    resp_time = 4 * (cybmod_brain_level(&p_person->U.UPerson.UMod) + 1);
                    if (resp_time > 15)
                        resp_time = 15;
                }
                if (p_person->U.UPerson.WeaponTimer >= resp_time)
                {
                    if ((p_person->Flag & TngF_Unkn1000) == 0)
                        p_person->Flag &= ~TngF_Unkn0800;
                }
            }
            if (weapon_has_targetting(wtype) && (p_person->PTarget == NULL))
                p_person->U.UPerson.WeaponTimer = 0;
        }

        if ((p_person->Flag & TngF_Unkn0800) == 0)
        {
            process_weapon_recoil(p_person);

            weapon_init_shot(p_person, wtype);

            if (wtype == WEP_RAP)
                weapon_consume_energy(p_person, wtype);

            weapon_set_timer_for_refire(p_person, wtype);

            p_person->Flag &= ~TngF_WepCharging;
            if ((p_person->U.UPerson.WeaponTimer > 5)
              && (wtype != WEP_RAP))
                p_person->Flag |= TngF_StationrSht;
        }
    }
}

void process_weapon(struct Thing *p_person)
{
#if 0
    asm volatile ("call ASM_process_weapon\n"
        : : "a" (p_person));
    return;
#endif
    process_energy_alarm(p_person);

    p_person->U.UPerson.Flag3 &= ~PrsF3_Unkn40;

    process_move_while_firing(p_person);

    if (((p_person->Flag & TngF_Persuaded) != 0) || (p_person->State == PerSt_PROTECT_PERSON))
    {
        struct Thing *p_owner;
        struct Thing *p_target;

        p_owner = &things[p_person->Owner];
        p_target = p_owner->PTarget;
        if (p_target != NULL)
        {
            if ((p_target->State == PerSt_DEAD) && (p_owner->Flag & (TngF_WepCharging|TngF_Unkn0800)) != 0)
            {
                p_person->Flag |= TngF_Unkn0800;
                p_person->PTarget = p_target;
                p_person->Flag &= ~TngF_Unkn20000000;
            }
        }
    }
    process_automedkit(p_person);

    if (p_person->U.UPerson.WeaponTurn == 0)
        process_weapon_wind_down(p_person);

    if (p_person->U.UPerson.MaxStamina != 0)
        process_stamina(p_person);
    process_energy_recovery(p_person);
    process_health_recovery(p_person);

    process_weapon_continuous_fire(p_person);

    if ((p_person->Flag & TngF_Unkn40000000) == 0)
    {
        if ((p_person->Flag & TngF_InVehicle) != 0)
        {
            struct Thing *p_vehicle;

            p_vehicle = &things[p_person->U.UPerson.Vehicle];
            if ((p_vehicle->Flag2 & TgF2_Unkn0002) != 0)
                return;
            switch (p_vehicle->SubType)
            {
            case SubTT_VEH_TANK:
                process_vehicle_weapon(p_vehicle, p_person);
                return;
            case SubTT_VEH_MECH:
                process_mech_weapon(p_vehicle, p_person);
                return;
            default:
                break;
            }
        }
        process_wielded_weapon(p_person);
    }
}

s32 laser_hit_at(s32 x1, s32 y1, s32 z1, s32 *x2, s32 *y2, s32 *z2, struct Thing *p_shot)
{
    s32 ret;
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "call ASM_laser_hit_at\n"
        : "=r" (ret) : "a" (x1), "d" (y1), "b" (z1), "c" (x2), "g" (y2), "g" (z2), "g" (p_shot));
    return ret;
}

/******************************************************************************/
