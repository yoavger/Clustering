/*
 * ModularityMaximization.h
 *
 *  Created on: 20 Aug 2020
 *      Author: roeeesquira
 */

#ifndef MODULARITYMAXIMIZATION_H_
#define MODULARITYMAXIMIZATION_H_

#include "divideToTwo.h"


int maximizeModularity(struct mat *A, struct bHat *bHat_g, double *s);

void zerofyVec(char *vec,int size);

int getMaxInd(double *arr,int size, char *excludeInd);


#endif /* MODULARITYMAXIMIZATION_H_ */
