/*
 * divideToTwo.c
 *
 *	DOCUMENTATION OF EACH FUNCTION IN H FILE "divideToTwo.h"
 *
 *  Created on: Aug 11, 2020
 *      Author: yoavger
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "divide_to_two.h"


int cheek_if_zigzag(double* vec1,double* vec2, int n);


double* algo2(struct mat *A, struct bHat *bHat_g){

	double 		*eigenvector,*s;
	double		eigenvalue;

	eigenvector = power_iteration(A,bHat_g);
			
	eigenvalue = compute_eigenValue(eigenvector,A, bHat_g);

	s = (double*)malloc((bHat_g -> size) * sizeof(double));
	
	if(s == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	/* if β1 ≤ 0 return null- e.g. The group g is indivisible */
	if (eigenvalue <= 0){
		compute_s_trivaial(s,bHat_g -> size);
		free(eigenvector);
		return s;
	}
	else{
		/* Compute s = {s1,...,sn} where si ∈ {+1,−1}, according to eigenvector */
		 compute_s(eigenvector,s,bHat_g -> size);

		/* if sT*B􏰅[g]*s ≤ 0 return s trivaial- e.g. The group g is indivisible */
		if (compute_sTXbHat_gXs(A,bHat_g,s) <= 0){
			compute_s_trivaial(s,bHat_g -> size);
			free(eigenvector);
			return s;
		}
	}
	/* free eigenvector alloceted at func power_iteration */
	free(eigenvector);

	/* return s reprseining a division of g */
	return s;
}

double mult_two_vectors (double* vec1 ,double* vec2, int n){

	int 	i;
	double 	result = 0;

	for (i = 0; i < n; ++i) 
		result += (*(vec1++)) * (*(vec2++));
	

	return result;
}

double compute_eigenValue(double *vec, mat *A, bHat *bHat_g){

	double numerator = 0;
	double denominator = 0;
	double *resultVector;

	resultVector = (double*) malloc (bHat_g -> size * sizeof(double));
	if(resultVector == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	mult_matrix_by_vector(A, vec,resultVector,bHat_g,0,1);

	numerator = mult_two_vectors(vec,resultVector,bHat_g -> size);
	denominator = mult_two_vectors(vec,vec,bHat_g -> size);

	free(resultVector);

	return (numerator/denominator);

}

double compute_sTXbHat_gXs(struct mat *A,struct bHat *bHat_g, double *s){

	double value;
	double *resultVector;

	resultVector = (double*) malloc ((bHat_g -> size) * sizeof(double));
	if(resultVector == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	mult_matrix_by_vector(A,s,resultVector,bHat_g,0,1);
	
	value = mult_two_vectors(s,resultVector,bHat_g -> size);

	free(resultVector);

	return value;
}

void compute_s (double *vec, double *s, int n){

	int i;

	for (i = 0; i < n; ++i) {
			if (IS_POSITIVE(*(vec++))){
				*(s++) = 1;
			}
			else
				*(s++) = -1;
		}
}

void compute_s_trivaial(double *s, int n){

	int i;

	for (i = 0; i < n; ++i) {
		*(s) = 1;
		s++;
	}
}

void div_by_norma(double* vec, int n){

	double 	mag;
	int 	i;

	mag = sqrt(mult_two_vectors(vec,vec,n));

	/* divide vector by it's norma*/
	for (i = 0; i < n; ++i){
			*vec = (*vec) / mag;
			vec++;
	}
}

int diff_small_of_eps(double* vec1,double* vec2, int n){

	double		diff;
	int 		i;

	/*for each pair of indices checks if the difference is greater than epsilon*/
	for (i = 0; i < n; ++i){
		diff = fabs(*(vec1) - *(vec2));
		if (diff > EPS){
			/* if the difference of the two vectors is greater then EPS return false*/
			return 0;
		}
		vec1++;
		vec2++;
	}

	/* if the difference of the two vectors is less then EPS return true*/
	return 1;
}

int cheek_if_zigzag(double* vec1,double* vec2, int n){

	int i;

	for (i = 0; i < n; ++i){
		if ((*(vec1)) != (- *(vec2)))
			return 0;

		vec1++;
		vec2++;
	}
	
	return 1;
}

double* power_iteration(struct mat *A ,struct bHat *bHat_g){

	int i,n;
	int bool;
	int counter = 0;
	int couner_zig = 0;
	double 	*eigenvector,*b0;

	n = bHat_g -> size;

	eigenvector = (double*)malloc(n * sizeof(double));
	if(eigenvector == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	b0 = (double*)malloc(n * sizeof(double));
	if(b0 == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}
	if (TEST){
		/* generate b0 vector all 1 for testing */
		for (i = 0 ; i < n ; i++)
			*(b0 + i) = 1;
	}
	else{
		
		/* generate random b0 vector */
		for (i = 0 ; i < n ; i++)
			*(b0 + i) = rand();
			div_by_norma(b0,n);
	}


	bool = 0;

	while(!bool){

		counter++;

		mult_matrix_by_vector(A,b0,eigenvector,bHat_g,1,1);

		div_by_norma(eigenvector,n);

		bool = diff_small_of_eps(eigenvector,b0,n);

		couner_zig += cheek_if_zigzag(eigenvector ,b0 , n);

		if (couner_zig >= 3){
			printf("ERROR - INF LOOP\n");
			exit(EXIT_FAILURE);
		}

		
		memcpy(b0,eigenvector, n * sizeof(double));


		if(counter > ((0.5)*n*n + 10000*n + 300000)){
			printf("ERROR - INF LOOP\n");
			exit(EXIT_FAILURE);
		}
	}

	free(b0);
	return eigenvector;
}

