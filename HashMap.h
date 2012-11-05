/**********************************************************************
 *
 * PROJECT:		Types library
 * FILE:		HashMap.h
 * LICENCE:		See Licence.txt
 * PURPOSE:		A hash map implementation written in C.
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#pragma once
#ifndef __MYLLY_HASHMAP_H
#define __MYLLY_HASHMAP_H

#include "Defines.h"

typedef uint32	( *hash_func_t )	( const void* );
typedef bool	( *key_func_t )		( const void*, const void* );
typedef void*	( *key_dup_func_t )	( const void* );
typedef void	( *data_destruct_t )( const void* );

typedef struct hnode_s {
	const void*		data;			// Stored data
	const void*		key;			// Key assigned to this data node
	struct hnode_s*	next;			// Pointer to next data node
} hashnode_t;

typedef struct {
	uint32			size;			// Number of elements stored
	uint32			bucket_count;	// Number of buckets in the hashmap
	float			load_factor;	// Current load factor
	float			max_load_factor;// Maximum load factor
	hashnode_t**	nodes;			// The data nodes (buckets)
	hash_func_t		key_hash;		// Hash function
	key_func_t		key_equals;		// Key comparison function
	key_dup_func_t	key_dup;		// Function to duplicate a key
	data_destruct_t	data_destroy;	// A custom destructor for the saved data
} hashmap_t;

__BEGIN_DECLS

MYLLY_API hashmap_t*	hashmap_create			( uint32 size );
MYLLY_API void			hashmap_destroy			( hashmap_t* map );

MYLLY_API void*			hashmap_insert			( hashmap_t* map, const void* key, const void* data );
MYLLY_API void*			hashmap_erase			( hashmap_t* map, const void* key );
MYLLY_API void*			hashmap_find			( hashmap_t* map, const void* key );

MYLLY_API void			hashmap_clear			( hashmap_t* map );
MYLLY_API void			hashmap_rehash			( hashmap_t* map, uint32 buckets );

__END_DECLS

#endif
