/*
 * modularity_maximization.h
 *
 *  Created on: 20 Aug 2020
 *      Author: roeeesquira
 */

#ifndef MODULARITYMAXIMIZATION_H_
#define MODULARITYMAXIMIZATION_H_

#include "divide_to_two.h"


int maximize_modularity(struct mat *A, struct bHat *bHat_g, double *s);

void zero_fy_vec(char *vec,int size);

int argmax(double *arr,int size, char *excludeInd);


#endif /* MODULARITYMAXIMIZATION_H_ */
