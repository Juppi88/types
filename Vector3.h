/**********************************************************************
 *
 * PROJECT:		Types library
 * FILE:		Vector3.h
 * LICENCE:		See Licence.txt
 * PURPOSE:		A 3D vector structure and functions to manipulate them.
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#pragma once
#ifndef __MYLLY_VECTOR3_H
#define __MYLLY_VECTOR3_H

#include "Types/Types.h"

typedef struct vector3_s
{
	float		x;
	float		y;
	float		z;
} vector3_t;

__inline float vector_length( const vector3_t* vec )
{
	return sqrtf( POW2(vec->x) + POW2(vec->y) + POW2(vec->z) );
}

__inline float vector_length_fast( const vector3_t* vec )
{
	return POW2(vec->x) + POW2(vec->y) + POW2(vec->z);
}

__inline float vector_dist( const vector3_t* vec1, const vector3_t* vec2 )
{
	return sqrtf( POW2( vec2->x - vec1->x ) + POW2( vec2->y - vec1->y ) + POW2( vec2->z - vec1->z ) );
}

__inline float vector_dist_fast( const vector3_t* vec1, const vector3_t* vec2 )
{
	return POW2( vec2->x - vec1->x ) + POW2( vec2->y - vec1->y ) + POW2( vec2->z - vec1->z );
}

#endif /* __MYLLY_VECTOR3_H */
