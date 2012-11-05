/**********************************************************************
 *
 * PROJECT:		Types library
 * FILE:		Types.h
 * LICENCE:		See Licence.txt
 * PURPOSE:		Basic types and structures implemented in C.
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#pragma once
#ifndef __MYLLY_TYPES_H
#define __MYLLY_TYPES_H

#include "Defines.h"
#include <math.h>

#ifndef POW2
#define POW2(x) (x) * (x)
#endif

#include "Types/Colour.h"
#include "Types/List.h"
#include "Types/MathDefs.h"
#include "Types/Rectangle.h"
#include "Types/Tree.h"
#include "Types/Vector2.h"
#include "Types/Vector3.h"
#include "Types/VectorScreen.h"

#endif /* __MYLLY_TYPES_H */
