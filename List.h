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

typedef struct node_s {
	struct node_s*	next;	// The next node on this list.
	struct node_s*	prev;	// The previous node.
} node_t;

typedef struct {
	struct node_s*	begin;	// The first list node
	struct node_s*	end;	// End iterator (one past last node)
	uint32			size;	// The size of the list
} list_t;

/*
 * list_foreach - A macro to loop through every node
 * @list: The list to loop through
 * @node: A node_t loop variable
 */
#define list_foreach(list,node) \
	for ( node = list->begin;   \
	      node != list->end;    \
	      node = node->next )   \

/*
 * list_foreach_r - A macro to loop through every node in reversed order
 * @list: The list to loop through
 * @node: A node_t loop variable
 */
#define list_foreach_r(list,node) \
	for ( node = list->end->prev; \
	      node != list->end;      \
	      node = node->prev )     \

/*
 * list_foreach_safe - A macro to loop through every node using a temp var
 * @list: The list to loop through
 * @node: A node_t loop variable
 */
#define list_foreach_safe(list,node,tmp)        \
	for ( node = list->begin, tmp = node->next; \
	      node != list->end;                    \
	      node = tmp, tmp = node->next )        \


/* Some macros to shorten often used function names */
#define list_push(list,node)	list_push_back(list,node)
#define list_pop(list)			list_pop_back(list)

__BEGIN_DECLS

MYLLY_API list_t*			list_create					( void );
MYLLY_API void				list_destroy				( list_t* list );

MYLLY_API void				list_push_back				( list_t* list, node_t* node );
MYLLY_API void				list_push_front				( list_t* list, node_t* node );
MYLLY_API node_t*			list_pop_back				( list_t* list );
MYLLY_API node_t*			list_pop_front				( list_t* list );
MYLLY_API void				list_remove					( list_t* list, node_t* node );

MYLLY_API void				list_move_backward			( list_t* list, node_t* node );
MYLLY_API void				list_move_forward			( list_t* list, node_t* node );
MYLLY_API void				list_send_to_back			( list_t* list, node_t* node );
MYLLY_API void				list_send_to_front			( list_t* list, node_t* node );

static __inline bool		list_empty					( list_t* list )
{
	return ( list->size == 0 );
}

__END_DECLS

#endif /* __MYLLY_LIST_H */
