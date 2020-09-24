/*
 * b_hat.h
 *
 *	STRUCTURE OF B[g]_hat
 *
 * 	This model is a implementation B[g]_hat containing all information of a sub group g of the graph
 *
 *  Created on: Aug 12, 2020
 *      Author: yoavger
 */

#ifndef __BHAT_H__
#define __BHAT_H__

#include "mat.h"

typedef struct bHat {

	/*  size of matrix bHat_g */
	int 		size;

	/*  norma1 e.g. sum of the largest column of B[g]_hat */
	double 	 	norma1;

	/* pointer to a list of all vertices of the group g */
	int 		*g;

	/* pointer to a list of sums for every row i of B[g] */
	double		*f_g_i;


} bHat;

/* intizeleize_bHat  */
bHat* intizeleize_bHat(struct mat *A ,int *g, int size);

/* Frees all resources used by bHat */
void free_bHat(struct bHat *bHat_g);

/* counts the number of vertices in g */
int countElemnts(int *g);

/* compute norma1 e.g. the sum of largest column in absolute value */
double compute_noram1(double *norma1_v, int size);

/* compute the sum of each row in bHat_g*/
void compute_f_g_i(struct mat *A ,struct bHat *bHat_g);

/* Multiplies matrix A by vector v, store result into (result is pre-allocated), using link-list
 * bool - indicating if the multiplication considers norma1 or not
 */
void mult_matrix_by_vector(const struct mat *A, const double *v, double *result,struct bHat *bHat_g, int bool,int bool_1);


#endif /* BHAT_H_ */
