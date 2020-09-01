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

#include "divideToTwo.h"

double* algo2(struct mat *A,struct bHat *bHat_g);

double multTwoVectors (double* vec1 ,double* vec2, int n);

double compute_sTXbHat_gXs(struct mat *A,struct bHat *bHat_g, double *s);

void computeS (double *vec, double *s, int n);

void computeSTrivaial(double *s, int n);

double compute_eigenValue (double *vec, mat *mat, bHat *bHat_g);

void divByNorma(double* vec, int n);

int diffSmallOfEps(double* vec1,double* vec2, int n);

double* poweriteration(struct mat *A ,struct bHat *bHat_g);

void printVectorDouble(double *v,int size);


double* algo2(struct mat *A, struct bHat *bHat_g){

	double 		*eigenvector,*s;
	double		eigenvalue;

	/* mesurment var */
	clock_t		start, end;

	start = clock();
	eigenvector = poweriteration(A,bHat_g);
	end = clock();
	
	if (PRINT)
		printf("Execution took for PI %f seconds\n", ((double)(end-start) / CLOCKS_PER_SEC));			

	eigenvalue = compute_eigenValue(eigenvector,A, bHat_g);

	s = (double*)malloc((bHat_g -> size) * sizeof(double));
	
	if(s == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	/* if β1 ≤ 0 return null- e.g. The group g is indivisible */
	if (eigenvalue <= 0){
		computeSTrivaial(s,bHat_g -> size);
		free(eigenvector);
		return s;
	}
	else{
		/* Compute s = {s1,...,sn} where si ∈ {+1,−1}, according to eigenvector */
		 computeS(eigenvector,s,bHat_g -> size);

		/* if sT*B􏰅[g]*s ≤ 0 return s trivaial- e.g. The group g is indivisible */
		if (compute_sTXbHat_gXs(A,bHat_g,s) <= 0){
			computeSTrivaial(s,bHat_g -> size);
			free(eigenvector);
			return s;
		}
	}
	/* free eigenvector alloceted at func poweriteration */
	free(eigenvector);

	/* return s reprseining a division of g */
	return s;
}

double multTwoVectors (double* vec1 ,double* vec2, int n){

	int 	i;
	double 	result = 0;

	for (i = 0; i < n; ++i) 
		result += (*(vec1++)) * (*(vec2++));
	

	return result;
}

double compute_eigenValue (double *vec, mat *A, bHat *bHat_g){

	double numerator = 0;
	double denominator = 0;
	double *resultVector;

	resultVector = (double*) malloc (bHat_g -> size * sizeof(double));
	if(resultVector == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	multMatrixByVector(A, vec,resultVector,bHat_g,0);

	numerator = multTwoVectors(vec,resultVector,bHat_g -> size);
	denominator = multTwoVectors(vec,vec,bHat_g -> size);

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

	multMatrixByVector(A,s,resultVector,bHat_g,0);
	
	value = multTwoVectors(s,resultVector,bHat_g -> size);

	free(resultVector);

	return value;
}

void computeS (double *vec, double *s, int n){

	int i;

	for (i = 0; i < n; ++i) {
			if (IS_POSITIVE(*(vec++))){
				*(s++) = 1;
			}
			else
				*(s++) = -1;
		}
}

void computeSTrivaial(double *s, int n){

	int i;

	for (i = 0; i < n; ++i) {
		*(s) = 1;
		s++;
	}
}

void divByNorma(double* vec, int n){

	double 	mag;
	int 	i;

	mag = sqrt(multTwoVectors(vec,vec,n));

	/* divide vector by it's norma*/
	for (i = 0; i < n; ++i){
			*vec = (*vec) / mag;
			vec++;
	}
}

int diffSmallOfEps(double* vec1,double* vec2, int n){

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

double* poweriteration(struct mat *A ,struct bHat *bHat_g){

	int i,n;
	int bool;
	int counter = 0;
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
			divByNorma(b0,n);
	}


	bool = 0;

	while(!bool){

		counter++;

		multMatrixByVector(A,b0,eigenvector,bHat_g,1);

		divByNorma(eigenvector,n);

		bool = diffSmallOfEps(eigenvector,b0,n);
		
		if (PRINT){
			if (counter > 1000){
				printf("----------TO MANY IN PI ---------- \n");
				printVectorDouble(b0,n);
				printVectorDouble(eigenvector,n);
			}
		}
	
		memcpy(b0,eigenvector, n * sizeof(double));
	}
	free(b0);
	return eigenvector;
}

void printVectorDouble(double *v,int size){
	int i;
	printf("Printing Vector - ");
	for (i = 0 ; i < size ; i++){
		printf("%f ",(*(v+i)));
	}
	printf("\n");
}
