#pragma once
#include "stdafx.h"

/*-----------------------------------------------------------------------------
 * _mathlib.h
 *-----------------------------------------------------------------------------*/

struct Vector3 // Implement the proper class of this at some point.
{
	float x; // 0x0000
	float y; // 0x0004
	float z; // 0x0008
};

struct QAngle // Implement the proper class of this at some point.
{
	float pitch; // 0x0000
	float yaw;   // 0x0004
	float roll;  // 0x0008
};
