 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Events
  * These are best for low-frequency events. Having too many of them,
  * or using them for events that occur too frequently, can cause massive
  * slowdown.
  *
  * Copyright 1995-1998 Bernd Schmidt
  */

#ifndef HATARI_EVENT_H
#define HATARI_EVENT_H

#undef EVENT_DEBUG

#define do_cycles do_cycles_slow

//#include "machdep/rpt.h"
#include "rpt.h"

extern volatile frame_time_t vsynctime, vsyncmintime;
extern void reset_frame_rate_hack (void);
extern int rpt_available;
extern frame_time_t syncbase;

extern void compute_vsynctime (void);
extern void init_eventtab (void);
extern void do_cycles_ce (long cycles);
extern void do_cycles_ce020 (int clocks);
extern void do_cycles_ce020_mem (int clocks);
extern void do_cycles_ce000 (int clocks);
extern int is_cycle_ce (void);
extern int current_hpos (void);
static int get_cycles (void);

static void events_schedule (void);

extern unsigned long currcycle, nextevent, is_lastline;
typedef void (*evfunc)(void);
typedef void (*evfunc2)(uae_u32);

typedef unsigned long int evt;

struct ev
{
    int active;
    evt evtime, oldcycles;
    evfunc handler;
};

struct ev2
{
    int active;
    evt evtime;
    uae_u32 data;
    evfunc2 handler;
};

enum {
    ev_hsync, ev_audio, ev_cia, ev_misc,
    ev_max
};

enum {
    ev2_blitter, ev2_disk, ev2_misc,
    ev2_max = 12
};

extern struct ev eventtab[ev_max];
extern struct ev2 eventtab2[ev2_max];

extern void event2_newevent (int, evt, uae_u32);
extern void event2_newevent2 (evt, uae_u32, evfunc2);
extern void event2_remevent (int);

/*#if 0
#ifdef JIT
#include "events_jit.h"
#else
#include "events_normal.h"
#endif
#else
#include "events_jit.h"
#endif
*/


STATIC_INLINE bool cycles_in_range (unsigned long endcycles)
{
	signed long c = get_cycles ();
	return (signed long)endcycles - c > 0;
}

STATIC_INLINE void cycles_do_special (void)
{
}
STATIC_INLINE void set_cycles (int c)
{
}

static int get_cycles (void)
{
	return currcycle;
}

STATIC_INLINE void do_cycles_slow (unsigned long cycles_to_add)
{
	if (is_lastline && eventtab[ev_hsync].evtime - currcycle <= cycles_to_add
		&& (long int)(read_processor_time () - vsyncmintime) < 0)
		return;

	while ((nextevent - currcycle) <= cycles_to_add) {
		int i;
		cycles_to_add -= (nextevent - currcycle);
		currcycle = nextevent;

		for (i = 0; i < ev_max; i++) {
			if (eventtab[i].active && eventtab[i].evtime == currcycle) {
				(*eventtab[i].handler)();
			}
		}
		events_schedule();
	}
	currcycle += cycles_to_add;
}

static void events_schedule (void)
{
	int i;

	unsigned long int mintime = ~0L;
	for (i = 0; i < ev_max; i++) {
		if (eventtab[i].active) {
			unsigned long int eventtime = eventtab[i].evtime - currcycle;
			if (eventtime < mintime)
				mintime = eventtime;
		}
	}
	nextevent = currcycle + mintime;
}

#endif //HATARI_EVENT_H
