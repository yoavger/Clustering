/*
 * modularity_maximization.c
 *
 *  Created on: 20 Aug 2020
 *      Author: roeeesquira
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "modularity_maximization.h"


double findBIGI(struct mat *A,struct bHat *bHat_g,int i,int k);

int maximize_modularity(struct mat *A, struct bHat *bHat_g, double *s){

	int  	*indices;
	int 	size,i,j,l,k;
	double 	d_q,temp;
	double 	*improve,*score;
	char 	*moved,*zeros;
	int 	counterWLoop = 0;

	int 		jTag;
	double 		*x;
	int 		m;
	int 		*vk;
	double 		b = 0;
	int 		*g;

	size = bHat_g -> size;
	m = A -> m;
	vk = A -> vectorK;
	g = bHat_g -> g;

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

	x = (double*) malloc(size*sizeof(double));
	if(x == NULL){
		printf("ERROR - memory error\n");
		exit(EXIT_FAILURE);
	}

	
	do {
		zero_fy_vec(moved,size);
		mult_matrix_by_vector(A,s,x,bHat_g,0,0);
		
		for (i = 0; i < size; i++){
			b = *(vk+(*(g+i)));
			*(score+i) = -2 * ((*(s+i) * (*(x+i))) + ((b*b)/m));
		}

		/*find maximum score vertex move out of those unmoved*/
		jTag = argmax(score,size,moved); 
		*(s+jTag) *= (-1);

		*(indices) = jTag;
		*(improve) = *(score+jTag);
		k = jTag;

		for (j = 1; j < size; j++){
			for (i = 0; i < size; ++i){
				if ( i == k ){
					*(score+i) *= (-1);
				}
				else {
					temp = findBIGI(A,bHat_g,i,k);
					*(score+i) += (-4) * ( *(s+i) * (*(s+k)) * temp);
				}
			}
			jTag = argmax(score,size,moved); 
			*(s+jTag) *= (-1);
			*(indices + j) = k;
			*(improve + j) = *(improve + (j-1)) + *(score + jTag);
	
			k = jTag;
		}

		/* find maximum score arrangement */
		l = argmax(improve,size,zeros);

		for (i = size-1; i >= l+1 ;i--){
			
			j = indices[i];
			s[j] *= (-1);
		}
		/*this is the case that maximum score is achieved after returning to original arrangement*/
		if (l == size-1){ 
			d_q = 0;
		}
		else {
			d_q = improve[l];
		}

	} while(d_q>0);

	free(x);
	free(moved);
	free(improve);
	free(score);
	free(indices);
	free(zeros);
	
	return counterWLoop;
}

double findBIGI(struct mat *A,struct bHat *bHat_g,int i,int k){

	int 	m,ki,kk;
	int 	*vk,*g;
	double 	elem2;

	node 		*node = NULL;
	link_list 	*list;

	m = A -> m;
	vk = A -> vectorK;
	g = bHat_g -> g;

	ki = *(vk + *(g+i));
	kk = *(vk + *(g+k));

	elem2 =((((double)ki)*kk)/m);

	list = *(((link_list**)(A->private))+ (*(g+i)));
	node = list -> head;

	while(node != NULL && node -> index != *(g+k)){
		if (node -> index > *(g+k))
			break;
		node = node -> next;
	}
	if (node != NULL && node -> index == *(g+k)){
		return (1 - (elem2));
	}
	else{
		return (0 - (elem2));
	}
	
}

void zero_fy_vec(char *vec,int size){

	int i;

	for (i=0; i<size; i++)
		*(vec++) = 0;
	
}

/*input - array of integers, size of array, array of chars 'excludeInd'
 * pre: there is at least one index that excludeInd[index]==0
 * return - index of maximum value integer in array, s.t excludeInd[index]==0
 * */
int argmax(double *arr,int size, char *excludeInd){

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



