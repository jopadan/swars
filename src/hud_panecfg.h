/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file hud_panecfg.h
 *     Header file for hud_panecfg.c.
 * @par Purpose:
 *     Ingame panel config loading from file.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     21 Mar 2024 - 11 Jan 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef HUD_PANECFG_H
#define HUD_PANECFG_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/
TbBool read_panel_config(const char *name, ushort styleno, ushort detail);
void size_panels_for_detail(short detail);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif