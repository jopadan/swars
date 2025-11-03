/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file util.c
 *     Routines implementing small utilities.
 * @par Purpose:
 *     Simple utilities for general use.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Oct 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include "util.h"
#include "bffile.h"

/******************************************************************************/
#pragma pack(1)

struct CharacterSubst {
    char OrigChar;
    char NewChar;
};

extern struct CharacterSubst subst_table_lower_to_upper[66];

#pragma pack()
/******************************************************************************/

ubyte chrtoupper(ubyte ch)
{
    // TODO this should be just a simple conversion array, not such convoluted code
    if (ch <= 127)
    {
        ch = toupper(ch);
    }
    else
    {
        int i;

        for (i = 0; subst_table_lower_to_upper[i].OrigChar; i++)
        {
            if (ch == subst_table_lower_to_upper[i].OrigChar)
            {
                ch = subst_table_lower_to_upper[i].NewChar;
                break;
            }
        }
    }
    return ch;
}


void strtolower(char *string)
{
    while (*string != '\0')
    {
        *string = tolower (*string);
        string++;
    }
}

void strtocapwords(char *s)
{
    TbBool word_start = 1;

    while (*s != '\0')
    {
        if (word_start) {
            if (isalpha(*s)) {
                *s = toupper(*s);
                word_start = 0;
            }
        } else {
            *s = tolower(*s);
            if (*s == ' ')
                word_start = 1;
        }
        s++;
    }
}

const char *extract_path_segment(const char *path, char *buffer, size_t size)
{
    const char *sep;
    int seg_len;

    sep = strchr (path, FS_SEP);
    if (sep == NULL)
    {
        snprintf(buffer, size, "%s", path);
        return NULL;
    }

    if (sep == path)
    {
        snprintf(buffer, size, "%s", FS_SEP_STR);
        return (path + 1);
    }

    seg_len = MIN (sep - path, (int) size);
    snprintf(buffer, size, "%.*s", seg_len, path);

    return sep;
}

/******************************************************************************/
