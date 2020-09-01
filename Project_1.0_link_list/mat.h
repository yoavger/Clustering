/*
 * mat.h
 *
 *	STRUCTURE OF A SPARSE MATRIX USING LINK-LIST
 *
 *	This model is a implementation of a Structure sparse matrix
 *	storing all information of matrix A (adjacency matrix) of the input graph
 *
 *  Created on: Aug 12, 2020
 *      Author: yoavger
 */

#ifndef __MAT_H__
#define __MAT_H__

#define TEST 0
#define PRINT 0

#define IS_POSITIVE(X) ((X) > 0.00001)
#define EPS 0.0001

typedef struct mat{

	/* Matrix size (n*n) */
	int		n;

	/*  sum of the degrees of all vertices */
	int 	m;

	/* pointer to a list of all vertex degree */
	int 	*vectorK;

	/* Adds row i the matrix. Called before any other call */
	void	(*add_row)(struct mat *A, const int *row, int i,int numOfItems);

	/* Frees all resources used by A */
	void	(*free)(struct mat *A);

	/* Private field for inner implementation.
	 * Should not be read or modified externally */
	void	*private;
} mat;


/* node of link list structure */
typedef struct node{

	/* index e.g. the index of column of the element in graph A */
	int 	index;

	/* pointer to the next node in the link-list */
    struct 	node *next;

    /* pointer  to a list of type int - for algo3 */
    int 	*g;

} node;

/* link list with pointer for head and tail structure*/
typedef struct link_list{

	/* pointer to head of the link list */
	node 	*head;

	/* pointer to tail of the link list */
	node 	*tail;

} link_list;


/* Allocates a new linked-lists sparse matrix of size n */
mat* mat_allocate_list(int n);

#endif /* MAT_H_ */
