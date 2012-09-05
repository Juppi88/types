/**********************************************************************
 *
 * PROJECT:		Types library
 * FILE:		Rectangle.h
 * LICENCE:		See Licence.txt
 * PURPOSE:		A 2D rectangle.
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#pragma once
#ifndef __MYLLY_RECTANGLE_H
#define __MYLLY_RECTANGLE_H

#include "Types/Types.h"

typedef struct rectangle_s
{
	uint16		x;
	uint16		y;
	uint16		w;
	uint16		h;
} rectangle_t;

__inline bool point_in_rect( const rectangle_t* r, uint16 x, uint16 y )
{
	return ( x >= r->x &&
			y >= r->y &&
			x <= r->x + r->w &&
			y <= r->y + r->h ) ? true : false;
}

__inline bool rect_is_in( const rectangle_t* rect1, const rectangle_t* rect2 )
{
	return ( rect1->x <= rect2->x &&
		rect1->y <= rect2->y &&
		(rect1->x+rect1->w) >= (rect2->x+rect2->w) &&
		(rect1->y+rect1->h) >= (rect2->y+rect2->h) ) ? true : false;
}

__inline void rect_copy( rectangle_t* dest, const rectangle_t* src )
{
	dest->x = src->x;
	dest->y = src->y;
	dest->w = src->w;
	dest->h = src->h;
}

__inline bool rect_equals( const rectangle_t* r1, const rectangle_t* r2 )
{
	return ( r1->x == r2->x && r1->y == r2->y && r1->w == r2->w && r1->h == r2->h );
}

#endif /* __MYLLY_RECTANGLE_H */
