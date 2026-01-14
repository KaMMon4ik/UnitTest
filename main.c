#define _CRT_SECURE_NO_WARNINGS

#include "test.h"

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

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

int comparator(double* a, double* b) {
	if (*a<*b) return -1;
	if (*a>*b) return 1;
	return 0;
}

double* sort_array(double* ptr, int len) {

	if (ptr==NULL || len<=0) return NULL;
	double* new_ptr = malloc(len*sizeof(double));
	if (new_ptr==NULL ) return NULL;

	memcpy(new_ptr, ptr, len*sizeof(double));
	
	qsort(new_ptr, len, sizeof(double), comparator);
	return new_ptr;

}

double sum_elements(double* ptr, int len, int begin, int end) {

	if (ptr==NULL || len<=0 || begin<0 || begin>=len || end<0 || end>=len || begin>end) return 0;

	double sum = 0;
	for (int i=begin; i<=end; ++i) sum += ptr[i];

	return sum;

}

int find_element(double* ptr, int len, double val) {

	if (ptr==NULL || len<=0) return -1;

	int index = -1;
	for (int i=0; i<len; ++i) {
		if (ptr[i]==val) {
			index = i;
			break;
		}
	}

	return index;

}

double* task_1() {

	double* ptr;
	int len;

	puts("Введите длину массива: ");
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

void test(int type) {
	
	switch (type) {
		case 1: {
			test_task_1(task_1);
			break;
		}
		case 2: {
			test_insert_element(insert_element);
			break;
		}
		case 3: {
			test_delete_element(delete_element);
			break;
		}
		case 4: {
			test_sort_array(sort_array);
			break;
		}
		case 5: {
			test_sum_elements(sum_elements);
			break;
		}
		case 6: {
			test_find_element(find_element);
			break;
		}
		default: {puts("Неверная цифра. Выберите цифру от 1 до 6\n");}
	}

}

void main() {

	setlocale(LC_ALL, "RUS");

	while (1) {
		puts("Введите цифры интересующих вас тестирований БЕЗ ПРОБЕЛОВ:\n"
			"(1) - тестирует функцию 1 задания из практической работы №16\n"
			"(2) - тестирует функцию вставки элемента в массив на определенный индекс\n"
			"(3) - тестирует функцию удаления элемента под определенным индексом из массива\n"
			"(4) - тестирует функцию сортировки массива от меньшего к большему\n"
			"(5) - тестирует функцию суммы элементов от первого до второго индекса\n"
			"(6) - тестирует функцию поиска первого элемента определенного значения");
		int type;
		scanf("%d", &type);
		int digit;
		while (type!=0) {
			test(type%10);
			type/=10;
			printf("\n");
		}
	}

}