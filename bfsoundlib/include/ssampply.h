/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ssampply.h
 *     Header file for ssampply.c.
 * @par Purpose:
 *     Sound Sample playback support routines.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFSOUNDLIB_SSAMPPLY_H_
#define BFSOUNDLIB_SSAMPPLY_H_

#include <stdint.h>
#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

typedef void * TbSampleHandle;

/******************************************************************************/

//int PlaySample();

struct SampleInfo *PlaySampleFromAddress(s32 source_id, short smp_id,
  short volume, ushort pan, short pitch, sbyte loop_count,
  ubyte a7, void *address);

TbBool IsSamplePlaying(s32 source_id, short smp_id, TbSampleHandle handle);

void SetSamplePan(s32 source_id, short smp_id, ushort pan);
void SetSamplePitch(s32 source_id, short smp_id, short pitch);
void SetSampleVolume(s32 source_id, short smp_id, short volume);

/** Releases sound sample which is playing in a loop.
 *
 * @param source_id Index of the related thing; note that it is unsigned;
 *        simple thing offsets should be just treated as unsigned here.
 */
void ReleaseLoopedSample(ushort source_id, short smp_id);

void StopSample(s32 source_id, short smp_id);
void StopAllSamples(void);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // BFSOUNDLIB_SSAMPPLY_H_
