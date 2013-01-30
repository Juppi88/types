/**********************************************************************
 *
 * PROJECT:		Types library
 * FILE:		HashMap.c
 * LICENCE:		See Licence.txt
 * PURPOSE:		A hash map implementation written in C.
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#include "Types/HashMap.h"
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define HASHMAP_INITIAL_CAPACITY	(100)
#define HASHMAP_MAX_LOAD_FACTOR		(0.75f)
#define HASHMAP_EXPANSION_FACTOR	(1.5f)

/*
 * __hashmap_alloc - An allocator func with further error
 * checking. Exits the app if allocating fails.
 * @arg size: Size to be allocated in bytes
 * @returns: A pointer to the allocated memory block
 */
static void* __hashmap_alloc( size_t size )
{
	void* ptr;

	ptr = malloc( size );

	assert( ptr != NULL ); // If we're in debug mode trigger the assertion
	if ( ptr ) return ptr;
	
	exit( EXIT_FAILURE ); // Otherwise exit the application just in case
}

/*
 * __hashmap_free - A memory freeing func.
 * @arg ptr: Memory block to be freed.
 */
static void __hashmap_free( const void* ptr )
{
	free( (void*)ptr );
}

/*
 * __hashmap_hash - Default hasher function.
 * djb2 hash from http://www.cse.yorku.ca/~oz/hash.html
 * @arg key: Pointer to key
 * @returns: Calculated hash
 */
static uint32 __hashmap_hash( const void* key )
{
	register char* s;
	register size_t hash = 5381;

	for ( s = (char*)key; *s; s++ )
	{
		hash = ( (hash << 5) + hash ) + *s;
	}

	return hash;
}

/*
 * __hashmap_key_equal - Default key comparison func.
 * Checks whether the two text keys equal.
 * @arg key1: First key.
 * @arg key2: Second key.
 * @returns: true if the keys are equal, false otherwise.
 */
static bool __hashmap_key_equal( const void* key1, const void* key2 )
{
	register const char *s1, *s2;
	s1 = key1; s2 = key2;

	while ( *s1 == *s2++ )
	{
		if ( !*s1++ ) return true;
	}

	return false;
}

/*
 * __hashmap_key_dup - Default key duplication function.
 * Duplicates a string key.
 * @arg key: The key to be duplicated.
 * @returns: A pointer to the duplicated key.
 */
static void* __hashmap_key_dup( const void* key )
{
	size_t len;
	char* str;

	len = strlen( (const char*)key );
	str = __hashmap_alloc( len+1 );

	strcpy( str, (const char*)key );

	return (void*)str;
}

/*
 * hashmap_create - Create and initialize a hashmap.
 * @returns: The created hashmap
 */
hashmap_t* hashmap_create( uint32 size )
{
	hashmap_t* map;

	map = __hashmap_alloc( sizeof(*map) );

	map->size = 0;
	map->bucket_count = size ? size : HASHMAP_INITIAL_CAPACITY;
	map->load_factor = 0;
	map->max_load_factor = HASHMAP_MAX_LOAD_FACTOR;
	map->key_hash = __hashmap_hash;
	map->key_equals = __hashmap_key_equal;
	map->key_dup = __hashmap_key_dup;
	map->data_destroy = NULL;

	map->nodes = __hashmap_alloc( sizeof(hashnode_t*) * map->bucket_count );
	memset( map->nodes, 0, sizeof(hashnode_t*) * map->bucket_count );

	return map;
}

/*
 * hashmap_destroy - Destroy and free a hashmap.
 * @arg map: Pointer to the hashmap to be destroyed.
 */
void hashmap_destroy( hashmap_t* map )
{
	assert( map != NULL );
	assert( map->nodes != NULL );

	hashmap_clear( map );

	//__hashmap_free( map->nodes );
	//__hashmap_free( map );
}

/*
 * __hashmap_node_create - A helper func to create a new hashnode.
 * @arg map: Hashmap.
 * @arg key: Pointer to the key value.
 * @arg data: The data to be stored.
 * @returns: Pointer to the new hash node
 */
static hashnode_t* __hashmap_node_create( hashmap_t* map, const void* key, const void* data )
{
	hashnode_t* node;

	node = __hashmap_alloc( sizeof(*node) );
	node->key = map->key_dup( key );
	node->data = data;
	node->next = NULL;
	
	map->size++;
	map->load_factor = (float)map->size / map->bucket_count;

	return node;
}

/*
 * hashmap_insert - Insert a new key-data pair to the table.
 * If a key exists already, the previous data will be overwritten.
 * @arg map: Hashmap.
 * @arg key: Pointer to the key value.
 * @arg data: The data to be stored.
 * @returns: Previous data assigned to this key, NULL if nothing was stored
 */
void* hashmap_insert( hashmap_t* map, const void* key, const void* data )
{
	hashnode_t *node, *newnode;
	void* old;
	uint32 hash;

	assert( map != NULL );
	assert( map->nodes != NULL );

	hash = map->key_hash( key ) % map->bucket_count;
	node = map->nodes[hash];

	if ( node )
	{
		for ( ; node; node = node->next )
		{
			if ( map->key_equals( key, node->key ) )
			{
				old = (void*)node->data;
				node->data = data;
				
				if ( map->data_destroy )
				{
					map->data_destroy( old );
					return NULL;
				}

				return old;
			}
		}
	}

	newnode = __hashmap_node_create( map, key, data );
	newnode->next = map->nodes[hash];

	map->nodes[hash] = newnode;
	
	if ( map->load_factor > map->max_load_factor )
		hashmap_rehash( map, (uint32)( HASHMAP_EXPANSION_FACTOR * map->bucket_count ) + 1 );

	return NULL;
}

/*
 * hashmap_erase - Remove a key-data pair from the table.
 * @arg map: The hashmap to remove from.
 * @arg key: Pointer to the key value.
 * @returns: Removed data, NULL if nothing was stored or a destructor was called
 */
void* hashmap_erase( hashmap_t* map, const void* key )
{
	hashnode_t *node, *prev, *first;
	void* data;
	uint32 hash;

	assert( map != NULL );
	assert( map->nodes != NULL );

	hash = map->key_hash( key ) % map->bucket_count;
	node = map->nodes[hash];

	if ( !node ) return NULL;

	for ( first = node, prev = NULL; node; prev = node, node = node->next )
	{
		if ( map->key_equals( key, node->key ) )
		{
			if ( prev )
				prev->next = node->next;

			if ( node == first )
				map->nodes[hash] = node->next;

			data = (void*)node->data;

			__hashmap_free( node->key );
			__hashmap_free( node );

			map->size--;

			if ( map->data_destroy )
			{
				map->data_destroy( node->data );
				return NULL;
			}

			return data;
		}
	}

	return NULL;
}

/*
 * hashmap_find - Find a data pointer matching the given key.
 * @arg map: Hashmap to look from
 * @arg key: The key
 * @returns: Found data, or NULL if no data was found.
 */
void* hashmap_find( hashmap_t* map, const void* key )
{
	hashnode_t* node;
	uint32 hash;

	assert( map != NULL );
	assert( map->nodes != NULL );

	hash = map->key_hash( key ) % map->bucket_count;
	node = map->nodes[hash];

	if ( node )
	{
		for ( ; node; node = node->next )
		{
			if ( map->key_equals( key, node->key ) )
			{
				return (void*)node->data;
			}
		}
	}

	return NULL;
}

/*
 * hashmap_clear - Remove all nodes from a hash table.
 * @arg map: The hash map to be cleared
 * @returns: -
 */
void hashmap_clear( hashmap_t* map )
{
	uint32 i;
	hashnode_t *node, *tmp;

	assert( map != NULL );
	assert( map->nodes != NULL );

	for ( i = 0; i < map->bucket_count; i++ )
	{
		for ( node = map->nodes[i]; node; node = tmp )
		{
			tmp = node->next;

			if ( map->data_destroy )
				map->data_destroy( node->data );

			__hashmap_free( node->key );
			__hashmap_free( node );
		}
	}

	map->size = 0;
}

/*
 * __hashmap_rehash_insert - A helper func to insert rehashed
 * nodes back into the hashmap.
 * @arg map: The hashmap that was rehashed.
 * @arg node: The node to be re-inserted
 * @returns: -
 */
static void __hashmap_rehash_insert( hashmap_t* map, hashnode_t* node )
{
	uint32 hash;
	hashnode_t* bucket;
	
	hash = map->key_hash( node->key ) % map->bucket_count;
	bucket = map->nodes[hash];

	if ( !bucket )
		node->next = NULL;
	else
		node->next = bucket;
	
	map->nodes[hash] = node;
}

/*
 * hashmap_rehash - Rehash the map and allocate extra space if needed.
 * @arg map: The hash map to be rehashed
 * @arg buckets: How many buckets the rehashed map should have
 * @returns: -
 */
void hashmap_rehash( hashmap_t* map, uint32 buckets )
{
	hashnode_t** nodes;
	hashnode_t *node, *next;
	uint32 i, old_buckets;

	assert( map != NULL );
	assert( map->nodes != NULL );

	if ( buckets == 0 ) return;

	nodes = map->nodes;
	old_buckets = map->bucket_count;

	map->nodes = __hashmap_alloc( sizeof(hashnode_t*) * buckets );
	map->bucket_count = buckets;

	memset( map->nodes, 0, sizeof(hashnode_t*) * buckets );

	for ( i = 0; i < old_buckets; i++ )
	{
		for ( node = nodes[i]; node; node = next )
		{
			next = node->next;

			__hashmap_rehash_insert( map, node );
		}
	}

	__hashmap_free( nodes );
}
