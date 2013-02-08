/**********************************************************************
 *
 * PROJECT:		Types library
 * FILE:		List.c
 * LICENCE:		See Licence.txt
 * PURPOSE:		A basic C linked list implementation for Mylly.
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#include "Types/List.h"
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>

/*
 * __list_alloc - An allocator func with further error
 * checking. Exits the app if allocating fails.
 * @arg size: Size to be allocated in bytes
 * @returns: A pointer to the allocated memory block
 */
static void* __list_alloc( size_t size )
{
	void* ptr;

	ptr = malloc( size );

	assert( ptr != NULL ); // If we're in debug mode trigger the assertion
	if ( ptr ) return ptr;
	
	exit( EXIT_FAILURE ); // Otherwise exit the application just in case
}

/*
 * __list_free - A memory freeing func.
 * @arg ptr: Memory block to be freed.
 */
static void __list_free( const void* ptr )
{
	free( (void*)ptr );
}

/*
 * list_create - Create and initialize a linked list.
 * @returns: the created linked list
 */
list_t* list_create( void )
{
	list_t* list;
	node_t* end;

	list = (list_t*)__list_alloc( sizeof(*list) );
	end = (node_t*)__list_alloc( sizeof(*end) );

	end->next = end;
	end->prev = end;
	end->data = NULL;

	list->begin = end;
	list->end = end;
	list->size = 0;

	return list;
}

/*
 * list_destroy - Destroy a previously created list.
 * @list: A list to be destroyed
 */
void list_destroy( list_t* list )
{
	node_t* node;
	node_t* tmp;

	assert( list != NULL );

	list_foreach_safe( list, node, tmp )
	{
		node->prev = NULL;
		node->next = NULL;

		if ( node->data )
			__list_free( node );
	}

	__list_free( list->end );
	__list_free( list );
}

/*
 * __list_add - Add a new node to the list.
 * @list: The list to manipulate
 * @node: The new node
 * @next: Next node
 * @prev: Previous node
 */
static __inline void __list_add( list_t* list, node_t* node,
								node_t* prev, node_t* next )
{
	node->next = next;
	node->prev = prev;
	next->prev = node;
	prev->next = node;

	if ( list->begin == list->end || list->begin == next )
		list->begin = node;

	list->size++;
}

/*
 * list_push_back - Add a new node to the end of the list.
 * @list: The list to manipulate
 * @node: The new node to be added
 */
void list_push_back( list_t* list, node_t* node )
{
	assert( list != NULL );
	assert( node != NULL );

	__list_add( list, node, list->end->prev, list->end );
}

/*
 * list_push_front - Add a new node to the beginning of the list.
 * @list: The list to manipulate
 * @node: The new node to be added
 */
void list_push_front( list_t* list, node_t* node )
{
	assert( list != NULL );
	assert( node != NULL );

	__list_add( list, node, list->end, list->begin );
}

/*
 * list_insert - Insert a new node before 'position'.
 * @list: The list to manipulate
 * @node: The new node to be added
 * @position: The node before which the new node should be inserted
 */
void list_insert( list_t* list, node_t* node, node_t* position )
{
	assert( list != NULL );
	assert( node != NULL );

	if ( position == NULL ) position = list->end;

	__list_add( list, node, position->prev, position );
}

/*
 * __list_remove - Cleans up the nodes links
 * @prev: Previous node
 * @next: Next node
 */
static __inline node_t* __list_remove( list_t* list, node_t* node,
								   node_t* prev, node_t* next )
{
	next->prev = prev;
	prev->next = next;
	node->prev = NULL;
	node->next = NULL;

	if ( node == list->begin )
		list->begin = next;

	list->size--;

	if ( node->data )
	{
		__list_free( node );
		node = NULL;
	}

	return node;
}

/*
 * list_pop_back - Removes a node from the end of the list.
 * @list: The list to manipulate
 * @returns: The removed node, or NULL if there was nothing to remove
 */
node_t* list_pop_back( list_t* list )
{
	node_t* node;

	assert( list != NULL );

	if ( list_empty(list) ) return NULL;

	node = list->end->prev;

	return __list_remove( list, node, node->prev, node->next );
}

/*
 * list_pop_front - Removes a node from the beginning of the list.
 * @list: The list to manipulate
 * @returns: The removed node, or NULL if there was nothing to remove
 */
node_t* list_pop_front( list_t* list )
{
	node_t* node;

	assert( list != NULL );

	if ( list_empty(list) ) return NULL;

	node = list->begin;

	return __list_remove( list, node, node->prev, node->next );
}

/*
 * list_remove - Removes an arbitrary node from the list.
 * @list: The list to manipulate
 * @node: The node to be removed
 */
void list_remove( list_t* list, node_t* node )
{
	assert( list != NULL );
	assert( node != NULL );

	if ( list_empty(list) ) return;

	__list_remove( list, node, node->prev, node->next );
}

/*
 * __list_create_node - Creates a new node as a container for the specified data.
 * @data: Data to be stored.
 * @returns: Pointer to the new node.
 */
static __inline node_t* __list_create_node( void* data )
{
	node_t* node;

	node = (node_t*)__list_alloc( sizeof(*node) );
	node->prev = NULL;
	node->next = NULL;
	node->data = data;

	return node;
}

/*
 * list_data_push_back - Create and add a new node to the end of the list.
 * @list: The list to manipulate
 * @data: The data to be added
 * @returns: List node for the pushed data
 */
node_t* list_data_push_back( list_t* list, void* data )
{
	node_t* node;

	assert( list != NULL );
	assert( data != NULL );

	node = __list_create_node( data );

	__list_add( list, node, list->end->prev, list->end );

	return node;
}

/*
 * list_data_push_front - Create and add a new node to the beginning of the list.
 * @data: The data to be added
 * @returns: List node for the pushed data
 */
node_t* list_data_push_front( list_t* list, void* data )
{
	node_t* node;

	assert( list != NULL );
	assert( data != NULL );

	node = __list_create_node( data );

	__list_add( list, node, list->end, list->begin );

	return node;
}

/*
 * list_data_insert - Insert a new data node before 'position'.
 * @list: The list to manipulate
 * @data: The data to be added
 * @position: The node before which the new node should be inserted
 * @returns: List node for the pushed data
 */
node_t* list_data_insert( list_t* list, void* data, node_t* position )
{
	node_t* node;

	assert( list != NULL );
	assert( data != NULL );

	if ( position == NULL ) position = list->end;

	node = __list_create_node( data );

	__list_add( list, node, position->prev, position );

	return node;
}

/*
 * list_data_pop_back - Removes a node from the end of the list
 * and returns the contained data.
 * @list: The list to manipulate
 * @returns: Data contained by the node, or NULL if there was nothing to remove.
 */
void* list_data_pop_back( list_t* list )
{
	node_t* node;
	void* data;

	assert( list != NULL );

	if ( list_empty(list) ) return NULL;

	node = list->end->prev;
	data = node->data;

	__list_remove( list, node, node->prev, node->next );

	return data;
}

/*
 * list_data_pop_front - Removes a node from the beginning of the list
 * and returns the contained data.
 * @list: The list to manipulate
 * @returns: Data contained by the node, or NULL if there was nothing to remove.
 */
void* list_data_pop_front( list_t* list )
{
	node_t* node;
	void* data;

	assert( list != NULL );

	if ( list_empty(list) ) return NULL;

	node = list->begin;
	data = node->data;

	__list_remove( list, node, node->prev, node->next );

	return data;
}

/*
 * list_data_remove - Removes an arbitrary node from the list.
 * @list: The list to manipulate
 * @data: Data which should be removed from the list.
 */
void list_data_remove( list_t* list, void* data )
{
	node_t* node;

	assert( list != NULL );

	if ( list_empty(list) ) return;
	if ( data == NULL ) return;

	list_foreach( list, node )
	{
		if ( node->data == data ) break;
	}

	if ( node != list->end )
		__list_remove( list, node, node->prev, node->next );
}

/*
 * list_move_backward - Moves a node on a list backward (towards the end).
 * @list: The list to manipulate
 * @node: The node to be moved
 */
void list_move_backward( list_t* list, node_t* node )
{
	node_t *tmp, *tmp2;

	assert( list != NULL );
	assert( node != NULL );

	tmp = node->next;
	if ( tmp == list->end ) return;

	tmp->prev = node->prev;
	tmp2 = tmp->next;
	tmp->next = node;
	node->prev = tmp;
	node->next = tmp2;
}

/*
 * list_move_forward - Moves a node on a list forward (towards the beginning).
 * @list: The list to manipulate
 * @node: The node to be moved
 */
void list_move_forward( list_t* list, node_t* node )
{
	node_t *tmp, *tmp2;

	assert( list != NULL );
	assert( node != NULL );

	tmp = node->prev;
	if ( tmp == list->end ) return;

	tmp->next = node->next;
	tmp2 = tmp->prev;
	tmp->prev = node;
	node->next = tmp;
	node->prev = tmp2;
}

/*
 * list_send_to_back - Sends a node to the back of the list
 * @list: The list to manipulate
 * @node: The node to be moved
 */
void list_send_to_back( list_t* list, node_t* node )
{
	assert( list != NULL );
	assert( node != NULL );

	__list_remove( list, node, node->prev, node->next );
	__list_add( list, node, list->end->next, list->end );
}

/*
 * list_send_to_front - Sends a node to the beginning of the list
 * @list: The list to manipulate
 * @node: The node to be moved
 */
void list_send_to_front( list_t* list, node_t* node )
{
	assert( list != NULL );
	assert( node != NULL );

	__list_remove( list, node, node->prev, node->next );
	__list_add( list, node, list->end, list->begin );
}
