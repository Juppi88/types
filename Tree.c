/**********************************************************************
 *
 * PROJECT:		Types library
 * FILE:		Tree.c
 * LICENCE:		See Licence.txt
 * PURPOSE:		AA tree implementation. AA trees are a variation of an
 *				rb-tree, see http://en.wikipedia.org/wiki/AA_tree
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#include "Types/Tree.h"
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>

/*
 * __tree_node_destructor - An empty default destructor
 */
static void __tree_node_destructor( void* ptr )
{
	UNREFERENCED_PARAM(ptr);
}

/*
 * __tree_alloc - An allocator func with further error
 * checking. Exits the app if allocating fails.
 * @arg size: Size to be allocated in bytes
 * @returns: A pointer to the allocated memory block
 */
static void* __tree_alloc( size_t size )
{
	void* ptr;

	ptr = malloc( size );

	assert( ptr != NULL ); // If we're in debug mode trigger the assertion
	if ( ptr ) return ptr;

	exit( EXIT_FAILURE ); // Otherwise exit the application just in case
}

/*
 * __tree_free - A memory freeing func.
 * @arg ptr: Memory block to be freed.
 */
static void __tree_free( const void* ptr )
{
	free( (void*)ptr );
}

/*
 * tree_create - Create a new binary tree.
 * @destructor: A destructor function for the nodes of the tree
 * @returns: The tree pointer
 */
tree_t* tree_create( void (*destructor)( void* ) )
{
	tree_t* tree;
	tree = __tree_alloc( sizeof(*tree) );

	tree->null.level = tree->null.key = 0;
	tree->null.left = tree->null.right = &tree->null;

	tree->root = &tree->null;
	tree->deleted = NULL;
	tree->size = 0;
	tree->destructor = destructor ? destructor : __tree_node_destructor;

	return tree;
}

/*
 * __tree_destroy - A recursive subroutine to destroy all the nodes of a tree.
 * @tree: The tree to be destroyed
 * @node: A node to be removed from the tree
 */
static void __tree_destroy( tree_t* tree, tnode_t* node )
{
	assert( node != NULL );

	if ( node == &tree->null ) return;
	if ( node->left != &tree->null ) __tree_destroy( tree, node->left );
	if ( node->right != &tree->null ) __tree_destroy( tree, node->right );

	tree->destructor( node );
}

/*
 * tree_destroy - Destroy a tree
 * @tree: The tree to be destroyed
 */
void tree_destroy( tree_t* tree )
{
	assert( tree != NULL );
	assert( tree->root != NULL );

	__tree_destroy( tree, tree->root );
	__tree_free( tree );
}

/*
 * __tree_skew - Right rotation needed to restore the balance after tree operations
 * @tree: A tree to be rebalanced
 * @returns: The rebalanced tree
 */
static tnode_t* __tree_skew( tnode_t* node )
{
	tnode_t* tmp;

	assert( node != NULL );
	assert( node->left != NULL );

	if ( node->level != node->left->level ) return node;

	tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;
	node = tmp;

	return node;
}

/*
 * __tree_split - Left rotation needed to restore the balance after tree operations
 * @tree: A tree to be rebalanced
 * @returns: The rebalanced tree
 */
static tnode_t* __tree_split( tnode_t* node )
{
	tnode_t* tmp;

	assert( node != NULL );
	assert( node->right != NULL );
	assert( node->right->right != NULL );

	if ( node->right->right->level != node->level ) return node;

	tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;
	node = tmp;
	node->level++;

	return node;
}

/*
 * __tree_find - A recursive subroutine to look for a matching key
 * @null: Null node
 * @node: Root node
 * @key: Wanted key
 * @returns: The result node, or NULL if no matching key was found
 */
static tnode_t* __tree_find( tnode_t* null, tnode_t* node, uint32 key )
{
	if ( node == null ) return NULL;
	else if ( node->key > key ) return __tree_find( null, node->left, key );
	else if ( node->key < key ) return __tree_find( null, node->right, key );
	else return node;
}

/*
 * tree_find - Find a node with the given key
 * @tree: The tree to look from
 * @key: Wanted key
 * @returns: The result node, or NULL if no matching key was found
 */
tnode_t* tree_find( tree_t* tree, uint32 key )
{
	return __tree_find( &tree->null, tree->root, key );
}

/*
 * __tree_new_node: Set up a new node for the tree
 * @node: The node to be prepared
 * @null: Pointer to the null node
 * @returns: The prepared node
 */
static __inline tnode_t* __tree_new_node( tnode_t* node, tnode_t* null )
{
	node->level = 1;
	node->left = null;
	node->right = null;

	return node;
}

/*
 * __tree_insert - A recursive subroutine to insert a new node
 * @tree: The tree to insert data
 * @root: Root node of the tree
 * @data: The node to be inserted
 * @returns: The new root node
 */
static tnode_t* __tree_insert( tree_t* tree, tnode_t* root, tnode_t* data )
{
	if ( root == &tree->null )
	{
		root = __tree_new_node( data, &tree->null );
		tree->size++;
		return root;
	}

	if ( data->key < root->key )
		root->left = __tree_insert( tree, root->left, data );

	else if ( data->key > root->key )
		root->right = __tree_insert( tree, root->right, data );

	else
		return root;

	root = __tree_skew( root );
	root = __tree_split( root );

	return root;
}

/*
 * tree_insert - Insert a new node to a tree
 * @tree: The tree to insert data
 * @data: The node to be inserted
 * @returns: The new root node
 */
void tree_insert( tree_t* tree, uint32 key, tnode_t* data )
{
	assert( tree != NULL );
	assert( data != NULL );

	data->key = key;
	tree->root = __tree_insert( tree, tree->root, data );
}

/*
 * __tree_remove - A recursive subroutine to remove a node
 * @tree: The tree to remove from
 * @root: The root node of the tree
 * @key: Key matching the node to be removed
 * @returns: The new root node
 */
static tnode_t* __tree_remove( tree_t* tree, tnode_t* root, uint32 key )
{
	if ( root == &tree->null ) return root;

	tree->last = root;

	if ( key < root->key )
	{
		root->left = __tree_remove( tree, root->left, key );
	}
	else if ( key > root->key )
	{
		root->right = __tree_remove( tree, root->right, key );
	}
	else
		tree->deleted = root;

	if ( root == tree->last &&
		 tree->deleted != &tree->null &&
		 key == tree->deleted->key )
	{
		tree->deleted->key = root->key;
		tree->deleted = &tree->null;
		root = root->right;

		tree->destructor( tree->last );
		tree->size--;
	}

	else if ( root->left->level < root->level - 1 ||
		      root->right->level < root->level - 1 )
	{
		root->level--;

		if ( root->right->level > root->level )
			root->right->level = root->level;

		root = __tree_skew( root );
		root->right = __tree_skew( root->right );
		root->right->right = __tree_skew( root->right->right );
		root = __tree_split( root );
		root->right = __tree_split( root->right );
	}

	return root;
}

/*
 * tree_remove - Remove an arbitrary node from the tree
 * @tree: The tree to remove from
 * @key: The key matching the node to be removed
 */
void tree_remove( tree_t* tree, uint32 key )
{
	tree->root = __tree_remove( tree, tree->root, key );
}
