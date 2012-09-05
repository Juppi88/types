/**********************************************************************
 *
 * PROJECT:		Types library
 * FILE:		VectorScreen.h
 * LICENCE:		See Licence.txt
 * PURPOSE:		A vector structure for an absolute point on screen.
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#pragma once
#ifndef __MYLLY_VECTORSCREEN_H
#define __MYLLY_VECTORSCREEN_H

#include "Types/Types.h"
#include <math.h>

typedef struct vectorscreen_s
{
	uint16		x;
	uint16		y;
} vectorscreen_t;

__inline float vectorscreen_length( const vectorscreen_t* vec )
{
	return sqrtf( (float)( POW2(vec->x) + POW2(vec->y) ) );
}

__inline float vectorscreen_length_fast( const vectorscreen_t* vec )
{
	return (float)( POW2(vec->x) + POW2(vec->y) );
}

__inline float vectorscreen_dist( const vectorscreen_t* vec1, const vectorscreen_t* vec2 )
{
	return sqrtf( (float)( POW2( vec2->x - vec1->x ) + POW2( vec2->y - vec1->y ) ) );
}

__inline float vectorscreen_dist_fast( const vectorscreen_t* vec1, const vectorscreen_t* vec2 )
{
	return (float)( POW2( vec2->x - vec1->x ) + POW2( vec2->y - vec1->y ) );
}

#endif /* __MYLLY_VECTORSCREEN_H */
