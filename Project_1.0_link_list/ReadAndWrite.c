/*
 * ReadAndWrite.c
 *
 *	DOCUMENTATION OF EACH FUNCTION IN H FILE "ReadAndWrite.h"
 *
 *  Created on: Aug 26, 2020
 *      Author: yoavger
 */

#include <stdio.h>
#include <stdlib.h>

#include "ReadAndWrite.h"


void print_linklist(link_list *list){

	node 	*cur;
	int 	*g;
	int 	i;

	cur = list -> head;

	printf("Printing link list output -");
	printf("\n");

	while (cur != NULL){
		g = cur -> g;
		for (i = 0 ; i < (*(g))+1 ; i++){
			printf("%d ", *(g+i));
		}
		printf("\n");
		cur = cur -> next;
	}
}

void readInputFile(struct mat *A,FILE *fileIn,int n){

	int 	i,degree,counter;
	int 	*row;

	/* allocate a vector of size n-1 for reading rows */
	row = (int*) malloc ((n-1) * sizeof(int));
	if(row == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < n; i++) {

			/* read k_i degree of vertex i */
			counter = fread(&degree, sizeof(int), 1, fileIn);
			if(counter != 1){
				printf("ERROR - read input error\n");
				exit(EXIT_FAILURE);
			}

			/* add degree of k_i to M */
			A -> m += degree;

			/* add degree of k_i to vectorK at index i */
			*((A -> vectorK)+i) = degree;

			/* read neighbors of vertex i */
			counter = fread(row, sizeof(int), degree, fileIn);
			if(counter != degree){
				printf("ERROR - read input error\n");
				exit(EXIT_FAILURE);
			}
			/* add row i to the matrix A */
			A -> add_row(A,row,i,degree);
		}

		free(row);
}

void writeOutputFile(link_list *list,FILE *fileOut){

	node 	*cur;
	int 	*row;
	int 	size, n;
	int 	counter = 0;

	/* write output file */
	n = fwrite(&counter,sizeof(int),1,fileOut);
	if(n != 1){
		printf("ERROR - write output error\n");
		exit(EXIT_FAILURE);
	}

	cur = list -> head;

	/* write each group of the division, first number is the size of the group
	 * followed by all indices of vertices that take part of the group
	 */
	while (cur != NULL){
		counter++;
		row = cur -> g;
		size = *(row) +1;
		n = fwrite(row,sizeof(int),size,fileOut);
		if(n != size){
			printf("ERROR - write output error\n");
			exit(EXIT_FAILURE);
		}
		cur = cur -> next;
	}
	rewind(fileOut);

	/* write the number of group in the division as the first element in the output file */
	n = fwrite(&counter,sizeof(int),1,fileOut);
	if(n != 1){
		printf("ERROR - write output error\n");
		exit(EXIT_FAILURE);
	}

}


