/*
 * bHat.c
 *
 *	DOCUMENTATION OF THE IN H FILE "bHat.h"
 *
 *  Created on: Aug 12, 2020
 *      Author: yoavger
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bHat.h"

bHat* intizeleize_bHat(struct mat *A ,int *g, int size);

void free_bHat(struct bHat *bHat_g);

double compute_noram1(double *norma1_v, int size);

void compute_f_g_i(struct mat *A, struct bHat *bHat_g);

void multMatrixByVector(const struct mat *A, const double *v, double *result,struct bHat *bHat_g, int bool);

/* intizeleize_bHat given a sub group g of the graph G */
bHat* intizeleize_bHat (struct mat* A, int *g, int size){

	bHat 		*bHat_g;
	double 		*f_g_i;

	bHat_g = (bHat*)malloc(sizeof(bHat));
	if(bHat_g == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}
	f_g_i = (double*)malloc(size*sizeof(double));
	if(f_g_i == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	bHat_g -> size = size;
	bHat_g -> g = g;
	bHat_g -> f_g_i = f_g_i;
	bHat_g -> norma1 = 0;

	compute_f_g_i(A , bHat_g);

	return bHat_g;
}
/* free all resources used bHat */
void free_bHat(struct bHat *bHat_g){

	free(bHat_g -> f_g_i);
	free(bHat_g);
}
/* compute noram1 - max of all sum of column */
double compute_noram1(double *norma1_v, int size){

	int 	i;
	double 	max = 0;

	for (i = 0; i < size; ++i) {
		if (*(norma1_v) > max){
			max = *(norma1_v);
		}
		norma1_v++;
	}
	return max;
}

/* compute f_g_i  - vector containing for each row in matrix B[g] its sum */
void compute_f_g_i(struct mat *A, struct bHat *bHat_g){

	int 		i,j,ki,kj,index_ki,index_kj,m,size;
	int 		*vk,*g;
	double 		sumOfRow,elem2,x;
	double 		*norma1_v,*f_g_i;
	node 		*node = NULL;
	link_list 	*list;

	vk = A -> vectorK;
	m = A -> m;
	g = bHat_g -> g;
	size = bHat_g -> size;
	f_g_i = bHat_g -> f_g_i;

	norma1_v = (double*)calloc(size,sizeof(double));
	if(norma1_v == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0 ; i < size; i++){

		/* sum of row i of matrix b[g] */
		sumOfRow = 0;

		/* k_i degree of vertex i */
		index_ki = *(g+i);
		ki = *(vk + index_ki);

		list = *(((link_list**)(A->private)) + index_ki);
		node = NULL;

		for (j = 0 ; j < size; j++){

			/* k_j egree of vertex j*/
			index_kj = *(g+j);
			kj = *(vk + index_kj);

			/* elem 2 = expected number of edges between vertices i and j */
			elem2 = ((((double)ki)*kj)/m);


			if (i == j){
				/*f_g_i += b[g]_ij = 0 - elem2 */
				sumOfRow += -elem2;
				continue;
			}

			/* a_ij = 0, elem2 = 0 */
			else if (kj == 0 || ki == 0){
					continue;
			}
			else{
				
				if (node == NULL){
					node = list -> head;
				}

				while(node != NULL && node -> index != index_kj){
					if (node -> index > index_kj)
						break;
					node = node -> next;
				}

				if (node != NULL && node -> index == index_kj){

					/* f_g_i += b[g]_ij = 1 - elem2 */
					sumOfRow += (1 - elem2);

					/* sum of column j in absolute value */
					*(norma1_v+j) += (1 - elem2);
				}
				else {
					/* f_g_i += b[g]_ij = 0 - elem2 */
					sumOfRow += -elem2;
					/* sum of column j in absolute value */
					*(norma1_v+j) += elem2;
				}
			}
		}
		/* f_g_i = sum of row of b[g]ij */
		*(f_g_i + i) = sumOfRow;
		*(norma1_v+i) += fabs((0-((((double)ki)*ki)/m)) - sumOfRow);
	}

	/* compute norma1 of bHat_g */
	x = compute_noram1(norma1_v, size);
	bHat_g -> norma1 = x;
	free(norma1_v);
}


void multMatrixByVector(const struct mat *A, const double *v, double *result, bHat *bHat_g, int bool){

	int 		i,j,m,size;
	int			*vk,*g;
	int			ki,kj;
	double 		norma1,elem2, dotPro;
	double 		*f_g_i;
	node 		*node = NULL;
	link_list 	*list;


	g = bHat_g -> g;
	size = bHat_g -> size;
	f_g_i = bHat_g -> f_g_i;

	if (bool == 1 ){
		norma1 = bHat_g -> norma1;
	}
	else {
		norma1 = 0;
	}

	m = A -> m;
	vk = A -> vectorK;

	/* calculate the matrix-by-vector product */
	for (i = 0 ; i < size; i++){

		/* dot proudct of bHat_g_i * V = result_i  */
		dotPro = 0;

		ki = *(vk + *(g+i));

		list = *(((link_list**)(A->private))+ (*(g+i)));

		node = NULL;
		
		for (j = 0; j < size; j++) {

			kj = *(vk + *(g+j));

			elem2 = ((((double)ki)*kj)/m);

			if (i == j){
				dotPro += (0 - (elem2) - *(f_g_i+i) + (norma1)) * (*(v + j));
				continue;
			}
			else if (kj == 0 || ki == 0){
				continue;
			}

			else{
				
				if (node == NULL){
					node = list -> head;
				}

				while(node != NULL && node -> index != *(g+j)){
					if (node -> index > *(g+j))
						break;
					node = node -> next;

				}

				if (node != NULL && node -> index == *(g+j)){
					dotPro += (1 - (elem2))* (*(v + j));

				}
				else{
					dotPro += (-(elem2))* (*(v + j));
				}
			}
		}
		*(result++) = dotPro;
	}

}
