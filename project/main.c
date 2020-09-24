/*
 * 	COMMUNITY STRUCTURE IN NETWORKS - FINAL PROJECT
 *
 * 	This following project is implementation of a algorithm
 * 	for detecting community structures (or clusters) in a network
 *
 * 	Input -  binary file of a graph represented as Adjacency list
 *
 * 	Output -  binary file of a division of the graph to modularity groups
 *
 *  This is the main function of the project
 *
 *
 *  Created on: Aug 11, 2020
 *      Author: yoavger
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "read_and_write.h"

int main(int argc, char* argv[]){

	int 		n;
	int 		counter;

	mat 		*A;
	link_list	*O;
	FILE 		*fileIn;
	FILE		*fileOut;
	clock_t		start, end;

	/* start measuring time */
	start = clock();

	n = argc;

	srand(time(NULL));

	/* open input file to read */
	fileIn = fopen(argv[1], "r");
	if(fileIn == NULL){
		printf("ERROR - no input file\n");
		exit(EXIT_FAILURE);
	}

	/* read size of graph |V| */
	counter = fread(&n, sizeof(int), 1, fileIn);
	if(counter != 1 ){
		printf("ERROR\n");
		exit(EXIT_FAILURE);
	}

	/* allocate a sparse matrix of size n using link list*/
	A = mat_allocate_list(n);

	/* read input file and store as a sparse matrix using link-list */
	read_input_file(A,fileIn,n);

	fclose(fileIn);

	/* Division by zero Error */
	if(A -> m == 0){
		printf("ERROR - division by zero\n");
		exit(EXIT_FAILURE);
	}
	/*
	if (PRINT)
		printf("Number of edges is %d \n",((A -> m)/2));
	*/

	/* Algorithm 3 Divide the network into modularity groups  */
	O = algo3(A);

	/* free A matrix */
	A -> free(A);

	/* printing to cheek outputfile 
	print_linklist(O);
	*/

	/* open output file to write */
	fileOut = fopen(argv[2], "w");
	if(fileOut == NULL){
		printf("ERROR - no output file\n");
		exit(EXIT_FAILURE);
	}

	/* write output file all modularity groups */
	write_output_file(O,fileOut);

	fclose(fileOut);

	/* free link - list O allocated in algo3  */
	free_link_list(O);

	/* end measuring time */
	end = clock();
	printf("Execution took %f seconds\n", ((double)(end-start) / CLOCKS_PER_SEC));

	return 0;
}
