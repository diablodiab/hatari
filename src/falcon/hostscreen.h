/*
  Hatari - hostscreen.h

  This file is distributed under the GNU Public License, version 2 or at
  your option any later version. Read the file gpl.txt for details.
*/

#ifndef HATARI_HOSTSCREEN_H
#define HATARI_HOSTSCREEN_H

extern void HostScreen_Init(void);
extern void HostScreen_UnInit(void);
extern void HostScreen_toggleFullScreen(void);
extern bool HostScreen_renderBegin(void);
extern void HostScreen_renderEnd(void);
extern void HostScreen_update1(bool forced);
extern Uint32 HostScreen_getBpp(void);	/* Bytes per pixel */
extern Uint32 HostScreen_getPitch(void);
extern Uint32 HostScreen_getWidth(void);
extern Uint32 HostScreen_getHeight(void);
extern Uint8 * HostScreen_getVideoramAddress(void);
extern void HostScreen_setPaletteColor(Uint8 idx, Uint8 red, Uint8 green, Uint8 blue);
extern Uint32 HostScreen_getPaletteColor(Uint8 idx);
extern void HostScreen_updatePalette(Uint16 colorCount);
extern Uint32 HostScreen_getColor(Uint8 red, Uint8 green, Uint8 blue);
extern void HostScreen_setWindowSize(int width, int height, int bpp);

#endif
