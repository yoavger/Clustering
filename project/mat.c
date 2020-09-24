/*
 * mat.c
 *
 *	DOCUMENTATION OF THE STRUCTURE MAT IN H FILE "mat.h"
 *
 *  Created on: Aug 12, 2020
 *      Author: yoavger
 */

#include <stdio.h>
#include <stdlib.h>

#include "mat.h"

void insert_node(int ind,link_list *list);

void add_row_list(struct mat *A, const int *row, int i, int numOfItems);

void free_A(struct mat *A);


/* allocate a sparse matrix A using link_list */
mat* mat_allocate_list(int n){

	mat 		*A;
	int 		*vectorK;
	link_list 	**rowList;

	/* allocate a new sparse matrix */
	A = (mat*) malloc(sizeof(mat));
	if(A == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	/* array of n link list */
	rowList = (link_list**) malloc(n * sizeof(link_list*));
	if(rowList == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	/* allocate a vector of size n with degree of all graph vertices */
	vectorK = (int*) malloc (n * sizeof(int));
	if(vectorK == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	/* initialize sparse matrix */
	A -> n = n;
	A -> add_row = add_row_list;
	A -> private = rowList;
	A -> free = free_A;
	A -> vectorK = vectorK;
	A -> m = 0;

	return A;
}
/* add row to matrix A */
void add_row_list(struct mat *A, const int *row, int i, int numOfItems){

	int 		j = 0;
	int 		item = 0;
	link_list 	*list;

	/* allocate a new link list */
	list = (link_list*)malloc(sizeof(link_list));
	if(list == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	list -> head = NULL;
	list -> tail = NULL;

	/* add all non zero row values to the link list */
	for (j = 0; j < numOfItems; j++){
		item = *(row+j);

		insert_node(item, list);

	}
	/* add the link list to the sparse matrix */
	*(((link_list**)(A -> private)) + i) = list;
}
/* insert a new node to the link list */
void insert_node(int index, link_list *list){

	/* create a new node */
	node *newNode = (node*)malloc(sizeof(node));
	if(newNode == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}
	/* insert data */
    newNode -> index = index;
    newNode -> next = NULL;

	/* if link list is empty insert at start */
    if ((list -> head == NULL) && (list -> tail == NULL)) {
       list -> head = newNode;
       list -> tail = newNode;
       }
    /* else insert to the end */
    else {
       list -> tail -> next = newNode;
       list -> tail = newNode;
    }
}
/* free all resources used by A */
void free_A(struct mat *A){

	int 		i = 0;
	node 		*cur, *temp;
	link_list 	*list;

	for (i = 0 ; i < A -> n ; i++){
		list = *(((link_list**)(A->private))+i);
		if (list -> head != NULL){
			cur = list -> head;
			while(cur -> next != NULL){
				temp = cur -> next;
				free(cur);
				cur = temp;
			}
			free(cur);
		}
		free(list);
	}
	free(A -> private);
	free(A -> vectorK);
	free(A);
}
