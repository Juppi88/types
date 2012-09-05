/**********************************************************************
 *
 * PROJECT:		Types library
 * FILE:		Vector2.h
 * LICENCE:		See Licence.txt
 * PURPOSE:		A 2D vector structure and functions to manipulate them.
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#pragma once
#ifndef __MYLLY_VECTOR2_H
#define __MYLLY_VECTOR2_H

#include "Types/Types.h"

typedef struct vector2_s
{
	float		x;
	float		y;
} vector2_t;

__inline float vector2_length( const vector2_t* vec )
{
	return sqrtf( POW2(vec->x) + POW2(vec->y) );
}

__inline float vector2_length_fast( const vector2_t* vec )
{
	return POW2(vec->x) + POW2(vec->y);
}

__inline float vector2_dist( const vector2_t* vec1, const vector2_t* vec2 )
{
	return sqrtf( POW2( vec2->x - vec1->x ) + POW2( vec2->y - vec1->y ) );
}

__inline float vector2_dist_fast( const vector2_t* vec1, const vector2_t* vec2 )
{
	return POW2( vec2->x - vec1->x ) + POW2( vec2->y - vec1->y );
}

#endif /* __MYLLY_VECTOR2_H */
