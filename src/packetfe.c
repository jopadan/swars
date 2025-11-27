/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file packetfe.c
 *     Handling of front-end packets for syncing player data.
 * @par Purpose:
 *     Besides in-mission packets, some front-end data exchange is also require
 *     to start a network game. This module stores functions for handling
 *     such packets.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Nov 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "packetfe.h"

#include <string.h>

#include "game.h"
#include "network.h"
#include "player.h"
#include "swlog.h"
/******************************************************************************/
#pragma pack(1)

#pragma pack()
/******************************************************************************/

TbBool net_local_player_hosts_the_game(void)
{
    int plyr;

    plyr = LbNetworkPlayerNumber();
    return login_control__State == LognCt_Unkn6 || plyr == net_host_player_no;
}

void net_players_all_set_unkn17(void)
{
    int plyr;

    for (plyr = 0; plyr < PLAYERS_LIMIT; plyr++)
    {
        network_players[plyr].Type = NPAct_Unkn17;
    }
}

void net_schedule_local_player_logout(void)
{
    int plyr;

    plyr = LbNetworkPlayerNumber();
    network_players[plyr].Type = NPAct_PlyrLogOut;
}

void net_schedule_local_player_reset(void)
{
    int plyr;

    plyr = LbNetworkPlayerNumber();
    network_players[plyr].Type = NPAct_NetReset;
}

void net_schedule_player_eject_sync(void)
{
    int plyr;

    plyr = LbNetworkPlayerNumber();
    network_players[plyr].Type = NPAct_PlyrEject;
}

void net_schedule_player_cryo_equip_sync(void)
{
    int plyr;

    plyr = LbNetworkPlayerNumber();
    network_players[plyr].Type = NPAct_PlyrWeapModsSync;
}

void net_schedule_player_equip_fourpack_sync(void)
{
    int plyr;

    plyr = LbNetworkPlayerNumber();
    network_players[plyr].Type = NPAct_PlyrFourPackSync;
}

void net_schedule_player_city_choice_sync(void)
{
    int plyr;

    plyr = LbNetworkPlayerNumber();
    network_players[plyr].Type = NPAct_SetCity;
}

void net_schedule_game_options_sync(void)
{
    int plyr;

    plyr = LbNetworkPlayerNumber();
    network_players[plyr].Type = NPAct_SetGameOptions;
}

void net_schedule_player_faction_change_sync(void)
{
    int plyr;

    plyr = LbNetworkPlayerNumber();
    network_players[plyr].Type = NPAct_SetFaction;
}

void net_schedule_player_team_change_sync(void)
{
    int plyr;

    plyr = LbNetworkPlayerNumber();
    network_players[plyr].Type = NPAct_SetTeam;
}

void net_schedule_player_chat_message_sync(const char *msg_text)
{
    int plyr;

    plyr = LbNetworkPlayerNumber();
    network_players[plyr].Type = NPAct_ChatMsg;
    strncpy(network_players[plyr].U.Text, msg_text,
      sizeof(network_players[0].U.Text));
}

void net_schedule_player_grpaint_action_sync(ubyte action, short pos_x, short pos_y, TbPixel colour)
{
    int plyr;

    plyr = LbNetworkPlayerNumber();
    network_players[plyr].Type = action;
    network_players[plyr].U.Progress.npfield_8 = pos_x;
    network_players[plyr].U.Progress.npfield_A = pos_y;
    network_players[plyr].U.Progress.npfield_12 = colour;
}

void net_schedule_player_grpaint_clear_sync(void)
{
    int plyr;

    plyr = LbNetworkPlayerNumber();
    network_players[plyr].Type = NPAct_GrPaintClear;
}



/******************************************************************************/
