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
#include "engindrwlstx.h"
#include "femain.h"
#include "game.h"
#include "guitext.h"
#include "sound.h"
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
    case GOpt_SampleVolume:
        return gui_strings[419];
    case GOpt_DangerVolume:
        return gui_strings[420];
    case GOpt_CDAVolume:
        return gui_strings[516];
    case GOpt_CDATrack:
        return gui_strings[528];
    case GOpt_DangerTrack:
        return gui_strings[529];
    case GOpt_UseMultiMedia:
        return gui_strings[530];
    case GOpt_ScannerPulse:
        return gui_strings[823];
    case GOpt_TranspObjSurfaceColr:
        return gui_strings[824];
    case GOpt_TranspObjLineColr:
        return gui_strings[825];
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
    case GOpt_UseMultiMedia:
        if (ingame.UseMultiMedia == 1)
            ingame.UseMultiMedia = 0;
        else
            ingame.UseMultiMedia = 1;
        break;
    case GOpt_ScannerPulse:
        if ((ingame.Flags & GamF_NoScannerBeep) == 0)
            ingame.Flags |= GamF_NoScannerBeep;
        else
            ingame.Flags &= ~GamF_NoScannerBeep;
        break;
    default:
        LOGERR("Option %d toggle not supported", option_no);
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
    case GOpt_UseMultiMedia:
    case GOpt_ScannerPulse:
        game_option_toggle(option_no);
        break;
    case GOpt_PanelPermutation:
        if (ingame.PanelPermutation < 0)
        {
            if (ingame.PanelPermutation < OPT_PANEL_PERMUT_MAX)
                ingame.PanelPermutation++;
            else
                ingame.PanelPermutation = OPT_PANEL_PERMUT_MIN;
        }
        else
        {
            if (ingame.PanelPermutation > OPT_PANEL_PERMUT_ALPHA_MIN)
                ingame.PanelPermutation--;
            else
                ingame.PanelPermutation = OPT_PANEL_PERMUT_ALPHA_MAX;
        }
        break;
    case GOpt_TrenchcoatPreference:
        if (ingame.TrenchcoatPreference > OPT_TRENCHCOAT_PREF_MIN)
            ingame.TrenchcoatPreference--;
        else
            ingame.TrenchcoatPreference = OPT_TRENCHCOAT_PREF_MAX;
        break;
    case GOpt_SampleVolume:
        if (startscr_samplevol > 0)
            startscr_samplevol--;
        else
            startscr_samplevol = STARTSCR_VOLUME_MAX;
        break;
    case GOpt_DangerVolume:
        if (startscr_midivol > 0)
            startscr_midivol--;
        else
            startscr_midivol = STARTSCR_VOLUME_MAX;
        break;
    case GOpt_CDAVolume:
        if (startscr_cdvolume > 0)
            startscr_cdvolume--;
        else
            startscr_cdvolume = STARTSCR_VOLUME_MAX;
        break;
    case GOpt_CDATrack:
        if (ingame.CDTrack > OPT_CD_TRACK_MIN)
            ingame.CDTrack--;
        else
            ingame.CDTrack = OPT_CD_TRACK_MAX;
        break;
    case GOpt_DangerTrack:
        if (ingame.DangerTrack > OPT_DANGER_TRACK_MIN)
            ingame.DangerTrack--;
        else
            ingame.DangerTrack = OPT_DANGER_TRACK_MAX;
        break;
    case GOpt_TranspObjSurfaceColr:
        deep_radar_surface_col--;
        break;
    case GOpt_TranspObjLineColr:
        deep_radar_line_col--;
        break;
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
    case GOpt_UseMultiMedia:
    case GOpt_ScannerPulse:
        game_option_toggle(option_no);
        break;
    case GOpt_PanelPermutation:
        if (ingame.PanelPermutation < 0)
        {
            if (ingame.PanelPermutation > OPT_PANEL_PERMUT_MIN)
                ingame.PanelPermutation--;
            else
                ingame.PanelPermutation = OPT_PANEL_PERMUT_MAX;
        }
        else
        {
            if (ingame.PanelPermutation < OPT_PANEL_PERMUT_ALPHA_MAX)
                ingame.PanelPermutation++;
            else
                ingame.PanelPermutation = OPT_PANEL_PERMUT_ALPHA_MIN;
        }
        break;
    case GOpt_TrenchcoatPreference:
        if (ingame.TrenchcoatPreference < OPT_TRENCHCOAT_PREF_MAX)
            ingame.TrenchcoatPreference++;
        else
            ingame.TrenchcoatPreference = OPT_TRENCHCOAT_PREF_MIN;
        break;
    case GOpt_SampleVolume:
        if (startscr_samplevol < STARTSCR_VOLUME_MAX)
            startscr_samplevol++;
        else
            startscr_samplevol = 0;
        break;
    case GOpt_DangerVolume:
        if (startscr_midivol < STARTSCR_VOLUME_MAX)
            startscr_midivol++;
        else
            startscr_midivol = 0;
        break;
    case GOpt_CDAVolume:
        if (startscr_cdvolume < STARTSCR_VOLUME_MAX)
            startscr_cdvolume++;
        else
            startscr_cdvolume = 0;
        break;
    case GOpt_CDATrack:
        if (ingame.CDTrack < OPT_CD_TRACK_MAX)
            ingame.CDTrack++;
        else
            ingame.CDTrack = OPT_CD_TRACK_MIN;
        break;
    case GOpt_DangerTrack:
        if (ingame.DangerTrack < OPT_DANGER_TRACK_MAX)
            ingame.DangerTrack++;
        else
            ingame.DangerTrack = OPT_DANGER_TRACK_MIN;
        break;
    case GOpt_TranspObjSurfaceColr:
        deep_radar_surface_col++;
        break;
    case GOpt_TranspObjLineColr:
        deep_radar_line_col++;
        break;
    default:
        break;
    }
}

void set_default_gfx_settings(void)
{
    game_gfx_advanced_lights = 1;
    game_billboard_movies = 1;
    game_gfx_deep_radar = 0;
    game_high_resolution = true;
    game_projector_speed = 0;
    game_perspective = 5;
}

void set_default_visual_prefernces(void)
{
    ingame.PanelPermutation = -2;
    ingame.TrenchcoatPreference = OPT_TRENCHCOAT_PREF_MIN;
    ingame.DetailLevel = 1;
    ingame.UseMultiMedia = 0;
}

void set_default_audio_tracks(void)
{
    ingame.DangerTrack = OPT_DANGER_TRACK_MIN;
    ingame.CDTrack = OPT_CD_TRACK_MIN;
}
/******************************************************************************/
