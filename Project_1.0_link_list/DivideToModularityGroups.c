/*
 * DivideToModularityGroups.c
 *
 *	DOCUMENTATION OF EACH FUNCTION IN H FILE "DivideToModularityGroups.h"
 *
 *  Created on: Aug 14, 2020
 *      Author: yoavger
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "DivideToModularityGroups.h"

link_list* algo3 (struct mat *A);

void initialize_P_and_O(int size, int *g, link_list **P , link_list **O);

void divise_g1_g2(int *g ,int **g1, int **g2, double *s, int size);

int countPostive(double *s,int size);

void insertNode_g(link_list *list, int *g);

void deleteHead(link_list *list);

void freeLink_list(link_list *list);

link_list* algo3 (struct mat *A){

	int 		size;
	int 		*g = NULL;
	int 		*g1,*g2;
	double 		*s;
	
	link_list 	*P;
	link_list 	*O;
	bHat 		*bHat_g;


	/* mesurment var */
	clock_t		start, end;
	double 		totalTimeAlgo2 = 0;	
	double 		totalTimeAlgo4 = 0;
 	int 		counter;
 	
	initialize_P_and_O(A -> n,g,&P ,&O);

	/* Repeat until P is empty */
	while((P -> head) != NULL){

		/* Remove the first group from P */
		g = P -> head -> g;
		deleteHead(P);

		/* get size of the group number of vertices */
		size = *(g);
		/* initialize struct bHat_g of the sub group g */
		bHat_g = intizeleize_bHat(A,g+1,size);

		start = clock();
		/* run Algorithm 2 to Divide g into two */
		s = algo2(A, bHat_g);
		end = clock();
		
		totalTimeAlgo2 += ((double)(end-start) / CLOCKS_PER_SEC);
		
		
		start = clock();
		/* run Modularity Maximization of the division of g */
		counter = maximizeModularity(A,bHat_g,s);
		end = clock();
		

		totalTimeAlgo4 += ((double)(end-start) / CLOCKS_PER_SEC);

		if (PRINT){

			printf("Total Time max for step %f\n", ((double)(end-start) / CLOCKS_PER_SEC));
			printf("A group of size %d took %d steps \n",size,counter);
		}

		/* Divide g into g1, g2 according to S */
		divise_g1_g2(g, &g1, &g2, s,size);

		/* free vector s allocated in algo2 */
		free(s);

		/* if either g1 or g2 is of size 0 */
		if (*(g1) == 0 || *(g2) == 0){
			 insertNode_g(O,g);
			 free(g1);
			 free(g2);
		 }
		 /* Add to O: any group (g1 and/or g2) of size 1 */
		 /* Add to P: any group (g1 and/or g2) larger than 1 */
		 else if (*(g1) == 1 && *(g2) == 1){
			 insertNode_g(O,g1);
			 insertNode_g(O,g2);
			 free(g);
		 }
		 else if (*(g1) == 1){
			insertNode_g(O,g1);
			insertNode_g(P,g2);
			free(g);
		 }
		else if (*(g2) == 1){

			insertNode_g(O,g2);
			insertNode_g(P,g1);
			free(g);
		}
		else {
			insertNode_g(P,g1);
			insertNode_g(P,g2);
			free(g);
		}

		free_bHat(bHat_g);
	}
	/* free link list P since while loop is done and p is empty,  */
	free(P);

	if (PRINT) {
		printf("Execution of algo2 took %f seconds\n", totalTimeAlgo2);
		printf("Execution of algo4 took %f seconds\n", totalTimeAlgo4); 
	}

	return O;
}

void initialize_P_and_O(int size,int *g, link_list **P , link_list **O){

	int 		i;

	link_list 	*temp1;
	link_list 	*temp2;

	temp1 = (link_list*)malloc(sizeof(link_list));
	if(temp1 == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	temp1 -> head = NULL;
	temp1 -> tail = NULL;

	temp2 = (link_list*)malloc(sizeof(link_list));
	if(temp2 == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	temp2 -> head = NULL;
	temp2 -> tail = NULL;

	g = (int*) malloc((size+1) * sizeof(int));
	if(g == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	*(g) = size;

	for(i = 1 ; i < size+1; i++)
			*(g+i) = i-1;

	insertNode_g(temp1,g);

	*P = temp1;
	*O = temp2;
}

void divise_g1_g2(int *g, int **g1, int **g2, double *s, int size){

	int 	i = 0;
	int 	size_g1 = 0;
	int 	size_g2 = 0;
	int 	*temp1,*temp2;

	size_g1 = countPostive(s,size);
	size_g2 = size - size_g1;

	/* allocate two vectors for groups g1 and g2 of size |g_i| + 1,
	 * first element representing the size of the group */
	temp1 = (int*)malloc((size_g1+1)*sizeof(int));
	if(temp1 == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}
	temp2 = (int*)malloc((size_g2+1)*sizeof(int));
	if(temp2 == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	*(temp1) = size_g1;
	*(temp2) = size_g2;

	*g1 = temp1;
	*g2 = temp2;

	g++;

	for (i = 0 ; i < size ; i++){
		if (*(s+i) == 1){
			temp1++;
			*(temp1) = *(g+i);
		}
		else{
			temp2++;
			*(temp2) = *(g+i);
		}
	}


}

int countPostive(double *s,int size){

	int i;
	int counter = 0;

	for (i = 0 ; i < size ; i++){
		if (*(s+i) == 1){
			counter++;
		}
	}
	return counter;
}

/* insert a new node to the link list */
void insertNode_g(link_list *list, int *g){

	/* create a new node */
	node *newNode = (node*)malloc(sizeof(node));
	if(newNode == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	/* insert data */
    newNode -> g = g;
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

void deleteHead(link_list *P){

	node *temp;

	if (P -> head == P -> tail){
		free(P -> head);
		P -> head = NULL;
		P -> tail = NULL;
	}
	else {
		temp = P -> head;
		P ->  head = P -> head -> next;
		free(temp);

	}
}

void freeLink_list(link_list *list){

	node *cur;
	node *temp;

	cur = list -> head;

	while (cur != NULL){
		temp = cur;
		free(temp -> g);
		cur = cur -> next;
		free(temp);
	}
	free(list);
}
