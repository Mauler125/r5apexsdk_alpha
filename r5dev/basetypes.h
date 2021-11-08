#pragma once

/*-----------------------------------------------------------------------------
 * _basetypes.h
 *-----------------------------------------------------------------------------*/

//#define GAMEDLL_S0 //[r]
//#define GAMEDLL_S1 //[r]
//#define GAMEDLL_S2 //[i]
//#define GAMEDLL_S3 //[r]
#define GAMEDLL_S3 //[i]

#define MAX_SPLITSCREEN_CLIENT_BITS 2 // Max 2 player splitscreen in portal (don't merge this back), saves a bunch of memory [8/31/2010 tom]
#define MAX_SPLITSCREEN_CLIENTS	( 1 << MAX_SPLITSCREEN_CLIENT_BITS ) // 4 // this should == MAX_JOYSTICKS in InputEnums.h

#define MAX_PLAYERS 128 // Max R5 players.
