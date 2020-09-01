/*
 * ModularityMaximization.c
 *
 *  Created on: 20 Aug 2020
 *      Author: roeeesquira
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ModularityMaximization.h"


double computeScore(struct mat *A,struct bHat *bHat_g, double *d, int index);

int maximizeModularity(struct mat *A, struct bHat *bHat_g, double *s){

	int  	*indices;
	int 	size,i,j,l;
	double 	d_q;
	double 	*improve,*score;
	char 	*moved,*zeros;

	/* mesurment var */
	clock_t		start, end;
	double		totalScore = 0;
	int 		counterWLoop = 0;


	size = bHat_g -> size;
	
	moved = (char*)malloc(size*sizeof(char));
	if(moved == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	zeros = (char*)calloc(size,sizeof(char));
	if(zeros == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	improve = (double*) malloc(size*sizeof(double));
	if(improve == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	score = (double*) malloc(size*sizeof(double));
	if(score == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	indices = (int*) malloc(size*sizeof(int));
	if(indices == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}


	do {
		counterWLoop++;
		/*--------------------------*/
		if (counterWLoop > 100 ){
			printf("ERROR INF LOOP \n");
			exit(EXIT_FAILURE);
		}
		
		zerofyVec(moved,size);

		for (i = 0; i < size; i++){
		
			for (j = 0; j < size; j++){
				/*if this vertex has not been moved yet*/
				if(!*(moved+j)){ 
					
					s[j] = -s[j];	
					start = clock();
					score[j] = computeScore(A,bHat_g,s,j);	
					end = clock();	
					totalScore += ((double)(end-start) / CLOCKS_PER_SEC);			
					s[j] = -s[j];
				}
			}

			j = getMaxInd(score,size,moved); /*find maximum score vertex move out of those unmoved*/
			
			s[j] = -s[j];
			moved[j]++;		/*signaling that vertex of index j has been moved*/
			indices[i] = j;
			
			if (i == 0) {
				improve[i] = score[j];
			}
			else {
				improve[i] = improve[i-1] + score[j];
			}
		}
		
		l = getMaxInd(improve,size,zeros); /*find maximum score arrangement*/
		
		for (i = size-1; i >= l+1 ;i--) {
			j = indices[i];
			s[j] = -s[j];
		}
		/*this is the case that maximum score is achieved after returning to original arrangement*/
		if (l == size-1) {
			d_q = 0;
		}
		else{	
			d_q = improve[l];
		}
		
	} while(d_q>0);

	free(moved);
	free(improve);
	free(score);
	free(indices);
	free(zeros);
	
	if(PRINT)
		printf("time for score %f \n",totalScore);

	return counterWLoop;

}
void zerofyVec(char *vec,int size){

	int i;

	for (i=0; i<size; i++)
		*(vec++) = 0;
	
}

/*input - array of integers, size of array, array of chars 'excludeInd'
 * pre: there is at least one index that excludeInd[index]==0
 * return - index of maximum value integer in array, s.t excludeInd[index]==0
 * */
int getMaxInd(double *arr,int size, char *excludeInd){

	int 	i,j;
	double 	val,tmp;

	j=0;

	while (*(excludeInd+j)){
		j++;
	}

	val = *(arr+j);
	for (i = j+1; i < size; i++){
		
		if (!(*(excludeInd+i))){
			
			tmp = *(arr+i);
			if (tmp > val){
				val = tmp;
				j = i;
			}
		}
	}
		return j;
}

double computeScore(struct mat *A,struct bHat *bHat_g, double *d, int i){

	int 	j,m,size,ki,kj;
	int 	*vk,*g;
	double 	elem2;
	double 	score = 0;

	node 		*node = NULL;
	link_list 	*list;

	m = A -> m;
	vk = A -> vectorK;
	size = bHat_g -> size;
	g = bHat_g -> g;

	ki = *(vk + *(g+i));
	list = *(((link_list**)(A->private))+ (*(g+i)));

	for (j = 0 ; j < size; ++j) {
		
		kj =  *(vk + *(g+j));
		elem2 = ((((double)ki)*kj)/m);

		if (node == NULL){
				node = list -> head;
			}
		while(node != NULL && node -> index != *(g+j)){
			if (node -> index > *(g+j))
				break;
			node = node -> next;
		}
		if (node != NULL && node -> index == *(g+j)){
			score += (1 - (elem2)) * (*(d+j));
		}
		else{
			score += (0 - (elem2)) * (*(d+j));
		}
	}
	return ((4 * (*(d+i))) * score + 4 *((((double)ki)*ki)/m));
}

