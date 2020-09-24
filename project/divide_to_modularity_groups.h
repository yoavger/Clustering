/*
 * 	divide_to_modularity_groups.h
 *
 * 	ALGORITHM 3 DIVIDE A NETWORK INTO MODULARITY GROUPS
 *
 * 	This module implements the core routine of the program Algorithm 3 
 * 	using Algorithm 2 to repeatedly divide the graph into groups
 *
 *  Created on: Aug 11, 2020
 *      Author: yoavger
 */

#ifndef __DIVIDETOMODULARITYGROUPS_H__
#define __DIVIDETOMODULARITYGROUPS_H__

#include  "modularity_maximization.h"

/* implementation of Algorithm 3 as describe on the document */
link_list* algo3 (struct mat *A);

/*
 * Utility Function
 * initialize two groups representing P ,O as link-list holding sub groups of the graph.
 * and place the trivial division of the graph into P : P ← {{1, . . . , n}}
 */
void initialize_P_and_O(int size, int *g, link_list **P , link_list **O);

/*
 * Utility Function
 * initialize two groups representing P ,O as link-list holding sub groups of the graph.
 * and place the trivial division of the graph into P : P ← {{1, . . . , n}}
 */
void divise_g1_g2(int *g ,int **g1, int **g2, double *s, int size);

/*
 * Utility Function
 * counting the number of indices of vector s of value 1
 *
 */
int count_postive(double *s,int size);

/*
 * Utility Function
 * insert a new node to a given link-list, the nodes holds a pointer to a vector representing a sub group g
 *
 */
void insert_node_g(link_list *list, int *g);

/*
 * Utility Function
 * delete head of a given link-list
 *
 */
void delete_head(link_list *list);

/*
 * Utility Function
 * free all resources of a given link-list
 *
 */
void free_link_list(link_list *list);

#endif /* DIVIDETOMODULARITYGROUPS_H_ */
