/**********************************************************************
 *
 * PROJECT:		Types library
 * FILE:		List.h
 * LICENCE:		See Licence.txt
 * PURPOSE:		A basic C linked list implementation for Mylly.
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#pragma once
#ifndef __MYLLY_LIST_H
#define __MYLLY_LIST_H

#include "stdtypes.h"

typedef struct node_t {
	struct node_t*	next;	// The next node on this list.
	struct node_t*	prev;	// The previous node.
	void*			data;	// Pointer to data
} node_t;

typedef struct {
	uint32			size;		// The size of the list
	struct node_t	sentinel;	// Sentinel node
} list_t;

/* Some macros to shorten often used function names */
#define list_begin(list)			list->sentinel.next
#define list_end(list)				&list->sentinel
#define list_first(list)			list->sentinel.next
#define list_last(list)				&list->sentinel.prev
#define list_push(list,node)		list_push_back(list,node)
#define list_pop(list)				list_pop_back(list)
#define list_data_push(list,data)	list_data_push_back(list,data)
#define list_data_pop_data(list)	list_data_pop_back(list)

#define list_empty(list)			( list->size == 0 )

/*
 * list_foreach - A macro to loop through every node
 * @list: The list to loop through
 * @node: A node_t loop variable
 */
#define list_foreach(list,node)       \
	for ( node = list->sentinel.next; \
	      node != &list->sentinel;    \
	      node = node->next )         \

/*
 * list_foreach_r - A macro to loop through every node in reversed order
 * @list: The list to loop through
 * @node: A node_t loop variable
 */
#define list_foreach_r(list,node)     \
	for ( node = list->sentinel.prev; \
	      node != &list->sentinel;    \
	      node = node->prev )         \

/*
 * list_foreach_safe - A macro to loop through every node using a temp var
 * @list: The list to loop through
 * @node: A node_t loop variable
 */
#define list_foreach_safe(list,node,tmp)  \
	for ( node = list->sentinel.next,     \
          tmp = node->next;               \
	      node != &list->sentinel;        \
	      node = tmp, tmp = node->next )  \

__BEGIN_DECLS

MYLLY_API list_t*			list_create					( void );
MYLLY_API void				list_destroy				( list_t* list );

MYLLY_API void				list_push_back				( list_t* list, node_t* node );
MYLLY_API void				list_push_front				( list_t* list, node_t* node );
MYLLY_API void				list_insert					( list_t* list, node_t* node, node_t* position );
MYLLY_API node_t*			list_pop_back				( list_t* list );
MYLLY_API node_t*			list_pop_front				( list_t* list );
MYLLY_API void				list_remove					( list_t* list, node_t* node );

MYLLY_API node_t*			list_data_push_back			( list_t* list, void* data );
MYLLY_API node_t*			list_data_push_front		( list_t* list, void* data );
MYLLY_API node_t*			list_data_insert			( list_t* list, void* data, node_t* position );
MYLLY_API void*				list_data_pop_back			( list_t* list );
MYLLY_API void*				list_data_pop_front			( list_t* list );
MYLLY_API void				list_data_remove			( list_t* list, void* data );

MYLLY_API void				list_move_backward			( list_t* list, node_t* node );
MYLLY_API void				list_move_forward			( list_t* list, node_t* node );
MYLLY_API void				list_send_to_back			( list_t* list, node_t* node );
MYLLY_API void				list_send_to_front			( list_t* list, node_t* node );

__END_DECLS

#endif /* __MYLLY_LIST_H */
