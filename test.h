#ifndef TEST_H
#define TEST_H

void test_task_1(double* (*func)());
void test_insert_element(double* (*func)(double* ptr, int len, int index, double val));
void test_delete_element(double* (*func)(double* ptr, int len, int index));
void test_sort_array(double* (*func)(double* ptr, int len));
void test_sum_elements(double (*func)(double* ptr, int len, int begin, int end));
void test_find_element(int (*func)(double* ptr, int len, double val));

#endif