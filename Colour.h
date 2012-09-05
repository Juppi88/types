/**********************************************************************
 *
 * PROJECT:		Types library
 * FILE:		Colour.h
 * LICENCE:		See Licence.txt
 * PURPOSE:		RGBA colour struct.
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#pragma once
#ifndef __MYLLY_COLOUR_H
#define __MYLLY_COLOUR_H

#include "Defines.h"

typedef struct colour_s
{
	uint8		r;	// Red
	uint8		g;	// Green
	uint8		b;	// Blue
	uint8		a;	// Alpha
} colour_t;

#define RGBACOL(r,g,b,a) \
	(uint32)( (r << 24) | (g << 16) | (b << 8) | a )

#define RGBCOL(r,g,b) \
	RGBACOL(r,g,b,255)

__inline uint32 colour_to_hex( colour_t* col )
{
	return (uint32)( (((((col->r << 8) + col->g) << 8) + col->b) << 8) + col->a );
}

__inline void hex_to_colour( uint32 hex, colour_t* out )
{
	out->a = hex & 0xFF; hex >>= 8;
	out->b = hex & 0xFF; hex >>= 8;
	out->g = hex & 0xFF; hex >>= 8;
	out->r = hex & 0xFF;
}

__inline void colour_copy( colour_t* dest, colour_t* src )
{
	dest->r = src->r;
	dest->g = src->g;
	dest->b = src->b;
	dest->a = src->a;
}

__inline void colour_add_gradient( colour_t* result, const colour_t* begin, float fr, float fg, float fb, uint32 layer )
{
	int32 r, g, b;

	r = begin->r + (int32)( fr * layer );
	g = begin->g + (int32)( fg * layer );
	b = begin->b + (int32)( fb * layer );

	if ( r < 0 ) r = 0; else if ( r > 255 ) r = 255; result->r = (uint8)r;
	if ( g < 0 ) g = 0; else if ( g > 255 ) g = 255; result->g = (uint8)g;
	if ( b < 0 ) b = 0; else if ( b > 255 ) b = 255; result->b = (uint8)b;
}

__inline uint8 colour_clamp( int32 value )
{
	value = value > 255 ? 255 : value;
	return (uint8)( value < 0 ? 0 : value );
}

__inline void colour_add( colour_t* col, int32 value )
{
	col->r = colour_clamp( col->r + value );
	col->g = colour_clamp( col->g + value );
	col->b = colour_clamp( col->b + value );
}

__inline void colour_invert( colour_t* col )
{
	col->r = 255 - col->r;
	col->g = 255 - col->g;
	col->b = 255 - col->b;
}

#endif /* __MYLLY_COLOUR_H */
