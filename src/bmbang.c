/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file bmbang.c
 *     On-map explosion effect implementation.
 * @par Purpose:
 *     Implement functions displaying an explosion within the game world.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Sep 2023 - 17 Mar 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bmbang.h"

#include "swlog.h"
/******************************************************************************/

extern ubyte byte_1C4769;
extern struct MapCreater map_craters[128];

void new_bang(int x, int y, int z, int type, int owner, int c)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_new_bang\n"
        : : "a" (x), "d" (y), "b" (z), "c" (type), "g" (owner), "g" (c));
}

void bang_new5(int x, int y, int z, int type, int owner)
{
    new_bang(x, y, z, type, owner, 0);
}

void bang_new4(int x, int y, int z, int type)
{
    new_bang(x, y, z, type, 0, 0);
}

void create_crater(short x, short y, short depth)
{
    int cratr_no;

    LOGSYNC("crater at (%d,%d) depth %d\n", x, y, depth);
    if (x > 0x80 || y > 0x80)
        return;
    cratr_no = byte_1C4769;
    byte_1C4769++;
    if (byte_1C4769 > 127)
        byte_1C4769 = 0;
    map_craters[cratr_no].MapX = x;
    map_craters[cratr_no].MapY = y;
    map_craters[cratr_no].Depth = depth;
    map_craters[cratr_no].Iterations = 0;
}

ubyte unused_func_026(ubyte a1)
{
    ubyte ret;
    asm volatile (
      "call ASM_unused_func_026\n"
        : "=r" (ret) : "a" (a1));
    return ret;
}

void unused_func_025(short a1, short a2, short a3)
{
    asm volatile (
      "call ASM_unused_func_025\n"
        : : "a" (a1), "d" (a2), "b" (a3));
}

/******************************************************************************/
