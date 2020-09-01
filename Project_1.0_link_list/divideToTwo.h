/*
 * divideToTwo.h
 *
 *	ALGORITHM 2 DIVIDE A GROUP INTO TWO
 *
 *	This module implements ALGORITHM 2 as describe in the document
 *
 *  Created on: Aug 11, 2020
 *      Author: yoavger
 */

#ifndef __DIVIDETOTWO_H__
#define __DIVIDETOTWO_H__

#include "bHat.h"

#define IS_POSITIVE(X) ((X) > 0.00001)
#define EPS 0.0001

/*
 * input - A matrix and b_Hat_g
 * return - vector S representing division of g
 *
*/
double* algo2(struct  mat *A,struct bHat *bHat_g);

/*
 * 	input - two vectors and size of vectors n (row vectors or colunm vector)
 *  return - dot product of the two vectors (scaler)
 */
double multTwoVectors (double* vec1 ,double* vec2,int n);

/*
 * input - eigenvector, pointer to vector S and size of vector n
 * return
 *
 */
double compute_sTXbHat_gXs(struct mat *A,struct bHat *bHat_g, double *s);

/*
 * input - eigenvector, pointer to vector S and size of vector n
 * return - create vector S representing a division of group of vertices in to two groups
 *
 */
void computeS (double *vec, double *s, int n);
/*
 * input -  pointer to vector S and size of vector n
 * return -  vector s all indices equal 1
 */
void computeSTrivaial(double *s, int n);


/*
 * input - eigenvectors, matrix , bHat[g] - matrix
 * return - eigenvalue
 *
 */
double compute_eigenValue (double *vec,struct mat *mat, struct bHat *bHat_g);

/*
 * input - vector and size of vector n
 * normalization of a given vector (e.g. vector divided by its norm)
 *
 */
void divByNorma(double* vec, int n);

/*
 * input - two vectors and size of vector n
 * return - cheek if the difference of two vectors is less then EPS
 *
 */
int diffSmallOfEps(double* vec1,double* vec2, int n);


/*
 * Performs power iteration
 * input - matrix A (sparse) and bHat_g sub group of A
 * return - leading eigenvector
 *
*/
double* poweriteration(struct mat *A ,struct bHat *bHat_g);

void printVectorDouble(double *v,int size);



#endif /* DIVIDETOTWO_H_ */
