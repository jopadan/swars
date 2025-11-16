/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file game_options.c
 *     Game options storage and manipulation.
 * @par Purpose:
 *     Variables and functions for handling ingame settings.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Feb 2024 - 02 Nov 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "game_options.h"

#include <assert.h>

#include "display.h"
#include "engindrwlstm.h"
#include "femain.h"
#include "game.h"
#include "guitext.h"
#include "swlog.h"

/******************************************************************************/


/******************************************************************************/

const char *game_option_desc(int option_no)
{
    switch (option_no)
    {
    case GOpt_ProjectorSpeed:
        return gui_strings[464];
    case GOpt_HighResolution:
        return gui_strings[469];
    case GOpt_DetailLevel:
        return gui_strings[470];
    case GOpt_CameraPerspective:
        return gui_strings[471];
    case GOpt_AdvancedLights:
        return gui_strings[481];
    case GOpt_BillboardMovies:
        return gui_strings[480];
    case GOpt_DeepRadar:
        return gui_strings[514];
    case GOpt_PanelPermutation:
        return gui_strings[522];
    case GOpt_TrenchcoatPreference:
        return gui_strings[523];
    default:
        return "";
    }
}

void game_option_toggle(int option_no)
{
    switch (option_no)
    {
    case GOpt_ProjectorSpeed:
        if (game_projector_speed == 1)
            game_projector_speed = 0;
        else
            game_projector_speed = 1;
        break;
    case GOpt_HighResolution:
        if (game_high_resolution == 1)
            game_high_resolution = 0;
        else
            game_high_resolution = 1;
        break;
    case GOpt_DetailLevel:
        if (ingame.DetailLevel == 1)
            ingame.DetailLevel = 0;
        else
            ingame.DetailLevel = 1;
        break;
    case GOpt_CameraPerspective:
        if (game_perspective == 5)
            game_perspective = 0;
        else
            game_perspective = 5;
        break;
    case GOpt_AdvancedLights:
        if ((ingame.Flags & GamF_AdvLights) == 0)
            ingame.Flags |= GamF_AdvLights;
        else
            ingame.Flags &= ~GamF_AdvLights;
        break;
    case GOpt_BillboardMovies:
        if ((ingame.Flags & GamF_BillboardMovies) == 0)
            ingame.Flags |= GamF_BillboardMovies;
        else
            ingame.Flags &= ~GamF_BillboardMovies;
        break;
    case GOpt_DeepRadar:
        if ((ingame.Flags & GamF_DeepRadar) == 0)
            ingame.Flags |= GamF_DeepRadar;
        else
            ingame.Flags &= ~GamF_DeepRadar;
        break;
    default:
        break;
    }
}


void game_option_dec(int option_no)
{
    switch (option_no)
    {
    case GOpt_ProjectorSpeed:
    case GOpt_HighResolution:
    case GOpt_DetailLevel:
    case GOpt_CameraPerspective:
    case GOpt_AdvancedLights:
    case GOpt_BillboardMovies:
    case GOpt_DeepRadar:
        game_option_toggle(option_no);
        break;
    case GOpt_PanelPermutation:
        if (ingame.PanelPermutation < 0)
        {
            if (ingame.PanelPermutation < -1)
                ingame.PanelPermutation++;
            else
                ingame.PanelPermutation = -3;
        }
        else
        {
            if (ingame.PanelPermutation > 0)
                ingame.PanelPermutation--;
            else
                ingame.PanelPermutation = 2;
        }
        break;
    case GOpt_TrenchcoatPreference:
        if (ingame.TrenchcoatPreference > 0)
            ingame.TrenchcoatPreference--;
        else
            ingame.TrenchcoatPreference = 5;
    default:
        break;
    }
}

void game_option_inc(int option_no)
{
    switch (option_no)
    {
    case GOpt_ProjectorSpeed:
    case GOpt_HighResolution:
    case GOpt_DetailLevel:
    case GOpt_CameraPerspective:
    case GOpt_AdvancedLights:
    case GOpt_BillboardMovies:
    case GOpt_DeepRadar:
        game_option_toggle(option_no);
        break;
    case GOpt_PanelPermutation:
        if (ingame.PanelPermutation < 0)
        {
            if (ingame.PanelPermutation > -3)
                ingame.PanelPermutation--;
            else
                ingame.PanelPermutation = -1;
        }
        else
        {
            if (ingame.PanelPermutation < 2)
                ingame.PanelPermutation++;
            else
                ingame.PanelPermutation = 0;
        }
        break;
    case GOpt_TrenchcoatPreference:
        if (ingame.TrenchcoatPreference < 5)
            ingame.TrenchcoatPreference++;
        else
            ingame.TrenchcoatPreference = 0;
    default:
        break;
    }
}

/******************************************************************************/
