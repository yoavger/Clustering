/*
 * ReadAndWrite.h
 *
 * 	This module read the input file and write output file according to format
 * 	input - a graph represented as Adjacency list
 * 	output - a division of the graph to Modularity groups
 *
 *  Created on: Aug 26, 2020
 *      Author: yoavger
 */

#ifndef __READANDWRITE_H__
#define __READANDWRITE_H__

#include "DivideToModularityGroups.h"

/* Utility function - printing a given link list (output file)  */
void print_linklist(link_list *list);

/* read input file (binary file of a graph) and store in memory as sparse matrix using link-list */
void readInputFile(struct mat *A,FILE *fileIn,int n);

/* write output file a division of the input graph to Modularity groups */
void writeOutputFile(link_list *list,FILE *fileOut);


#endif /* READANDWRITE_H_ */
