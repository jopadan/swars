/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file guitext.c
 *     GUI text for menu and in-game display.
 * @par Purpose:
 *     Implement functions for handling text for game GUI, both in-game
 *     and in menu.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "guitext.h"

#include "bffile.h"
#include "bfmemory.h"
#include "bfmemut.h"

#include "display.h"
#include "swlog.h"
/******************************************************************************/
char *gui_strings_data;
char *gui_strings_data_end;

void snprint_dh_time_duration(char *out, ulong outlen, long ndays, short nhours)
{
    ulong days_strid;
    ulong hours_strid;
    if (ndays == 1) {
        days_strid = 625;
    } else {
        days_strid = 626;
    }
    if (nhours == 1) {
        hours_strid = 627;
    } else {
        hours_strid = 628;
    }

    snprintf(out, outlen, "%ld %s %hd %s", ndays, gui_strings[days_strid], nhours, gui_strings[hours_strid]);
}

const char *loctext_to_gtext(const char *ltext)
{
    char *gtext;
    u32 len;

    len = strlen(ltext) + 1;
    gtext = (char *)back_buffer + text_buf_pos;
    text_buf_pos += len;
    LbMemoryCopy(gtext, ltext, len);

    return gtext;
}

/** Read file with all the language-specific texts.
 */
void read_strings_file(void)
{
    char *text_fname = "data/text.dat";
    int text_len;

    text_len = LbFileLength(text_fname);
    gui_strings_data = (char *)LbMemoryAlloc(text_len);
    if (gui_strings_data != NULL) {
        gui_strings_data_end = gui_strings_data + text_len;
        LbFileLoadAt(text_fname, gui_strings_data);
    }
}

TbBool create_strings_list(char **strings, char *strings_data, char *strings_data_end)
{
  int text_idx;
  char *text_ptr;
  char **text_arr;
  text_arr = strings;
  text_idx = STRINGS_MAX;
  text_ptr = strings_data;
  while (text_idx >= 0)
  {
      if (text_ptr >= strings_data_end) {
          break;
      }
      *text_arr = text_ptr;
      text_arr++;
      char chr_prev;
      do {
          chr_prev = *text_ptr;
          text_ptr++;
      } while ((chr_prev != '\0') && (text_ptr < strings_data_end));
      text_idx--;
  }
  return (text_idx < STRINGS_MAX);
}

TbBool flashy_draw_text(int x, int y, const char *text, ubyte speed, int top_line, short *textpos, int cyan_flag)
{
    TbBool ret;
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "call ASM_flashy_draw_text\n"
        : "=r" (ret) : "a" (x), "d" (y), "b" (text), "c" (speed), "g" (top_line), "g" (textpos), "g" (cyan_flag));
    return ret;
}

/******************************************************************************/
