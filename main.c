#define _CRT_SECURE_NO_WARNINGS

#include "test.h"

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double* full_elements(double* ptr, int len) {

	if (ptr==NULL || len<=0) return NULL;
	double* new_ptr = malloc(len*sizeof(double));
	if (new_ptr==NULL) return NULL;

	for (int i=0; i<len; ++i) new_ptr[i] = pow(i+1, 2) + sin(5*(i+1));

	return new_ptr;

}

double* calc_elements(double *ptr, int len) {

	if (ptr==NULL || len<=0) return NULL;
	double* new_ptr = malloc(len*sizeof(double));
	if (new_ptr==NULL) return NULL;

	for (int i=0; i<len; ++i) new_ptr[i] = ptr[i] * 2;

	return new_ptr;

}

void put_elements(double* ptr, int len) {

	if (ptr==NULL || len<=0) return NULL;

	for (int i=0; i<len; ++i) printf("%lf ", ptr[i]);
	printf("\n");

}

double* insert_element(double* ptr, int len, int index, double val) {

	if (ptr==NULL || len<=0) return NULL;
    double* new_ptr = malloc((len+1)*sizeof(double));
	if (new_ptr==NULL) return NULL;

	if (index<0 || index>len) return NULL;

    for (int i=0; i<index; ++i) new_ptr[i] = ptr[i];
    new_ptr[index] = val;
    for (int i=index; i<len; ++i) new_ptr[i+1] = ptr[i];

    return new_ptr;
}

double* delete_element(double* ptr, int len, int index) {

	if (ptr==NULL || len<=0) return NULL;
	double* new_ptr = malloc((len-1)*sizeof(double));
	if (new_ptr==NULL) return NULL;

	if (index<0 || index>=len) return NULL;

	for (int i=0; i<index; ++i) new_ptr[i] = ptr[i];
	for (int i=index; i<(len-1); ++i) new_ptr[i] = ptr[i+1];

    return new_ptr;

}

double* task_1() {

	double* ptr;
	int len;

	puts("¬ведите длину массива: ");
	scanf("%d", &len);

	if (len<=0) return NULL;
	ptr = (double*) malloc(len*sizeof(double));
	if (ptr==NULL) return NULL;

	ptr = full_elements(ptr, len);
	put_elements(ptr, len);

	ptr = calc_elements(ptr, len);
	put_elements(ptr, len);

	free(ptr);

	return ptr;

}

int comp(const void* a, const void* b) {
	return *(double*)a-*(double*)b;
}

double* sort_array(double* ptr, int len) {

	qsort(ptr, len, sizeof(double), comp);

	return ptr;

}

void main() {

	setlocale(LC_ALL, "RUS");

	//test_task_1(task_1);
	
	//test_insert_element(insert_element);

	//test_delete_element(delete_element);

	test_sort_array(sort_array);

}