#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

int TEST_CNT = 100;
int MAX_LEN = 100;

void send_string(char* str) {
	size_t len = strlen(str);
	INPUT* inputs = calloc(len+1, sizeof(INPUT));
	if (inputs==NULL) return;

	for (int i=0; i<len; ++i) {
		inputs[i].type = INPUT_KEYBOARD;
		inputs[i].ki.dwFlags = KEYEVENTF_UNICODE;
		inputs[i].ki.wScan = str[i];
	}

	inputs[len].type = INPUT_KEYBOARD;
	inputs[len].ki.dwFlags = KEYEVENTF_UNICODE;
	inputs[len].ki.wScan = VK_RETURN;

	SendInput(len+1, inputs, sizeof(INPUT));
	free(inputs);
}

void test_task_1(double* (*func)()) {

	HANDLE process;
	PROCESS_MEMORY_COUNTERS pmc;
	LARGE_INTEGER start_time, end_time, frequency;

	process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());

	puts("Началось тестирование задания #1 из работы #16");
	puts("В случае ошибки проверьте требования к функции из README");
	puts("--------------------------------------------------------\n");
	Sleep(1500);

	double total_time = 0;
	int total_memory = 0;
	int errors = 0;
	bool free_check = false;

	srand(time(0));

	for (int i=0; i<TEST_CNT; ++i) {

		int test_len = (rand()%MAX_LEN+1) * (rand()%3-1);
		char input_str[4];
		_itoa(test_len, input_str, 10);

		printf("- Тест #%d | Длина массива: %d\n", i+1, test_len);

		QueryPerformanceFrequency(&frequency);
		GetProcessMemoryInfo(process, &pmc, sizeof(pmc));
		size_t start_memory = pmc.PeakWorkingSetSize;
		QueryPerformanceCounter(&start_time);

		send_string(input_str);
		double* ans = func();

		if (ans==NULL) {
			if (test_len>0) {
				errors++;
				puts("- Ошибка: был возращен NULL, ожидался указатель");
			}
		}
		else {
			if (test_len<=0) {
				errors++;
				puts("- Ошибка: не был возвращен NULL");
			}
			double* full = calloc(test_len, sizeof(double));
			int cnt = 0;
			for (int j=0; j<test_len; ++j) {
				if (ans[j]==0) cnt++;
			}
			if (cnt==test_len) free_check=true;
		}

		QueryPerformanceCounter(&end_time);
		GetProcessMemoryInfo(process, &pmc, sizeof(pmc));
		size_t end_memory = pmc.PeakWorkingSetSize;

		puts("\n--------------------------------------------------------\n");

		total_memory += end_memory - start_memory;
		total_time += (double) (end_time.QuadPart-start_time.QuadPart) / frequency.QuadPart;

	}

	Sleep(1000);
	printf("- Среднее количество потраченной памяти: %d байт\n", total_memory/TEST_CNT);
	printf("- Среднее время выполнения функции: %.3f сек\n", total_time/TEST_CNT);
	printf("- Ошибок при выполении тестов: %d\n", errors);
	printf("- Выделенная память %sбыла освобождена\n", free_check==true ? "" : "НЕ ");

}

void test_insert_element(double* (*func)(double* ptr, int len, int index, double val)) {
	
	HANDLE process;
	PROCESS_MEMORY_COUNTERS pmc;
	LARGE_INTEGER start_time, end_time, frequency;

	process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());

	puts("Началось тестирование вставки элемента в массив");
	puts("В случае ошибки проверьте требования к функции из README");
	puts("--------------------------------------------------------\n");
	Sleep(1500);

	double total_time = 0;
	int total_memory = 0;
	int errors = 0;

	srand(time(0));

	for (int i=0; i<TEST_CNT; ++i) {

		int test_len = (rand()%MAX_LEN+1) * (rand()%3-1);
		int test_index = (rand()%max(test_len, 1)) - (rand()%max(test_len, 1)) + (rand()%max(test_len, 1));
		double test_val = (double) (rand()%(rand()+1)) / (rand()%(rand()+1)+1) - (rand()%(rand()+1)) + (rand()%(rand()+1));

		double* ptr = calloc(test_len, sizeof(double));
		if (rand()%10==0) ptr = NULL;

		printf("- Тест #%d | Длина массива: %d | Индекс вставки: %d | Значение для вставки: %lf\n", i+1, test_len, test_index, test_val);

		QueryPerformanceFrequency(&frequency);
		GetProcessMemoryInfo(process, &pmc, sizeof(pmc));
		size_t start_memory = pmc.PeakWorkingSetSize;
		QueryPerformanceCounter(&start_time);

		double* ans = func(ptr, test_len, test_index, test_val);

		if (ans==NULL) {
			if (test_len>0 && test_index>=0 && test_index<=test_len && ptr!=NULL) {
				errors++;
				puts("- Ошибка: был возращен NULL, ожидался указатель");
			}
		}
		else {
			if (test_len<=0 || test_index<0 || test_index>test_len || ptr==NULL) {
				errors++;
				puts("- Ошибка: не был возвращен NULL");
			}
			else {
				if (ans[test_index]!=test_val) {
					errors++;
					puts("- Ошибка: элемент не был вставлен на данный индекс");
				}
				if (_msize(ans)/sizeof(double)!=test_len+1) {
					errors++;
					puts("- Ошибка: длина массива не увеличилась");
				}
			}
		}

		QueryPerformanceCounter(&end_time);
		GetProcessMemoryInfo(process, &pmc, sizeof(pmc));
		size_t end_memory = pmc.PeakWorkingSetSize;

		puts("\n--------------------------------------------------------\n");

		free(ptr);
		free(ans);

		total_memory += end_memory - start_memory;
		total_time += (double) (end_time.QuadPart-start_time.QuadPart) / frequency.QuadPart;

	}

	Sleep(1000);
	printf("- Среднее количество потраченной памяти: %d байт\n", total_memory / TEST_CNT);
	printf("- Среднее время выполнения функции: %.3f сек\n", total_time / TEST_CNT);
	printf("- Ошибок при выполении тестов: %d\n", errors);

}

void test_delete_element(double* (*func)(double* ptr, int len, int index)) {

	HANDLE process;
	PROCESS_MEMORY_COUNTERS pmc;
	LARGE_INTEGER start_time, end_time, frequency;

	process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());

	puts("Началось тестирование удаления элемента из массива");
	puts("В случае ошибки проверьте требования к функции из README");
	puts("--------------------------------------------------------\n");
	Sleep(1500);

	double total_time = 0;
	int total_memory = 0;
	int errors = 0;

	srand(time(0));

	for (int i=0; i<TEST_CNT; ++i) {

		int test_len = (rand()%MAX_LEN+1) * (rand()%3-1);
		int test_index = (rand()%max(test_len, 1)) - (rand()%max(test_len, 1)) + (rand()%max(test_len, 1));

		double* ptr = malloc(test_len*sizeof(double));
		if (rand()%10==0) ptr = NULL;
		if (test_len>0 && ptr!=NULL) {
			for (int j=0; j<test_len; ++j) ptr[j] = (double) (rand()%(rand()+1)) / (rand()%(rand()+1)+1) - (rand()%(rand()+1)) + (rand()%(rand()+1));
		}

		printf("- Тест #%d | Длина массива: %d | Индекс удаления: %d\n", i+1, test_len, test_index);

		QueryPerformanceFrequency(&frequency);
		GetProcessMemoryInfo(process, &pmc, sizeof(pmc));
		size_t start_memory = pmc.PeakWorkingSetSize;
		QueryPerformanceCounter(&start_time);

		double* ans = func(ptr, test_len, test_index);

		if (ans==NULL) {
			if (test_len>0 && test_index>=0 && test_index<test_len && ptr!=NULL) {
				errors++;
				puts("- Ошибка: был возращен NULL, ожидался указатель");
			}
		}
		else {
			if (test_len<=0 || test_index<0 || test_index>=test_len || ptr==NULL) {
				errors++;
				puts("- Ошибка: не был возвращен NULL");
			}
			else {
				if (ans[test_index]==ptr[test_index]) {
					errors++;
					puts("- Ошибка: элемент не был удален из массива");
				}
				if (_msize(ans)/sizeof(double)!=test_len-1) {
					errors++;
					puts("- Ошибка: длина массива не увеличилась");
				}
			}
		}

		QueryPerformanceCounter(&end_time);
		GetProcessMemoryInfo(process, &pmc, sizeof(pmc));
		size_t end_memory = pmc.PeakWorkingSetSize;

		puts("\n--------------------------------------------------------\n");

		free(ptr);
		free(ans);

		total_memory += end_memory - start_memory;
		total_time += (double) (end_time.QuadPart-start_time.QuadPart) / frequency.QuadPart;

	}

	Sleep(1000);
	printf("- Среднее количество потраченной памяти: %d байт\n", total_memory / TEST_CNT);
	printf("- Среднее время выполнения функции: %.3f сек\n", total_time / TEST_CNT);
	printf("- Ошибок при выполении тестов: %d\n", errors);

}

void test_sort_array(double* (*func)(double* ptr, int len)) {

	HANDLE process;
	PROCESS_MEMORY_COUNTERS pmc;
	LARGE_INTEGER start_time, end_time, frequency;

	process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());

	puts("Началось тестирование сортировки массива");
	puts("В случае ошибки проверьте требования к функции из README");
	puts("--------------------------------------------------------\n");
	Sleep(1500);

	double total_time = 0;
	int total_memory = 0;
	int errors = 0;

	srand(time(0));

	for (int i=0; i<TEST_CNT; ++i) {

		int test_len = (rand()%MAX_LEN+1) * (rand()%3-1);

		double* ptr = malloc(test_len*sizeof(double));
		if (rand()%10==0) ptr = NULL;
		if (test_len>0 && ptr!=NULL) {
			for (int j=0; j<test_len; ++j) ptr[j] = (double) (rand()%(rand()+1)) / (rand()%(rand()+1)+1) - (rand()%(rand()+1)) + (rand()%(rand()+1));
		}

		printf("- Тест #%d | Длина массива: %d\n", i+1, test_len);

		QueryPerformanceFrequency(&frequency);
		GetProcessMemoryInfo(process, &pmc, sizeof(pmc));
		size_t start_memory = pmc.PeakWorkingSetSize;
		QueryPerformanceCounter(&start_time);

		double* ans = func(ptr, test_len);

		if (ans==NULL) {
			if (test_len>0 && ptr!=NULL) {
				errors++;
				puts("- Ошибка: был возращен NULL, ожидался указатель");
			}
		}
		else {
			if (test_len<=0 || ptr==NULL) {
				errors++;
				puts("- Ошибка: не был возвращен NULL");
			}
			else {
				bool sort_check = true;
				for (int j=0; j<test_len-1; ++j) {
					if (ans[j]>ans[j+1]) {
						sort_check = false;
						break;
					}
				}
				if (!sort_check) {
					errors++;
					puts("- Ошибка: массив не был отсортирован от меньшего к большему");
				}
				if (ptr==ans) {
					errors++;
					puts("- Ошибка: возвращенный указатель равняется переданному");
				}
			}
		}

		QueryPerformanceCounter(&end_time);
		GetProcessMemoryInfo(process, &pmc, sizeof(pmc));
		size_t end_memory = pmc.PeakWorkingSetSize;

		puts("\n--------------------------------------------------------\n");

		free(ptr);
		free(ans);

		total_memory += end_memory - start_memory;
		total_time += (double) (end_time.QuadPart-start_time.QuadPart) / frequency.QuadPart;

	}

	Sleep(1000);
	printf("- Среднее количество потраченной памяти: %d байт\n", total_memory / TEST_CNT);
	printf("- Среднее время выполнения функции: %.3f сек\n", total_time / TEST_CNT);
	printf("- Ошибок при выполении тестов: %d\n", errors);

}

void test_sum_elements(double (*func)(double* ptr, int len, int begin, int end)) {

	HANDLE process;
	PROCESS_MEMORY_COUNTERS pmc;
	LARGE_INTEGER start_time, end_time, frequency;

	process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());

	puts("Началось тестирование суммы чисел диапазона массива");
	puts("В случае ошибки проверьте требования к функции из README");
	puts("--------------------------------------------------------\n");
	Sleep(1500);

	double total_time = 0;
	int total_memory = 0;
	int errors = 0;

	srand(time(0));

	int test_len = MAX_LEN - (rand()%(MAX_LEN/10+1));

	double* ptr = malloc(test_len*sizeof(double));
	if (rand()%10==0) ptr = NULL;
	if (test_len>0 && ptr!=NULL) {
		for (int i=0; i<test_len; ++i) ptr[i] = (double) (rand()/(rand()+1.0)) - (rand()/(rand()+1.0));
	}

	// - Нахождение суммы через префиксую сумму -

	double* sum = calloc(test_len+1, sizeof(double));
	if (sum==NULL) return NULL;

	if (test_len>0 && ptr!=NULL) {
		for (int i=0; i<test_len; ++i) sum[i+1] = sum[i] + ptr[i];
	}

	// ------------------------------------------

	for (int i=0; i<TEST_CNT; ++i) {

		int test_begin = (rand()%max(test_len, 1)) - (rand()%max(test_len, 1)) + (rand()%max(test_len, 1));
		int test_end = (rand()%max(test_len, 1)) - (rand()%max(test_len, 1)) + (rand()%max(test_len, 1));

		printf("- Тест #%d | Длина массива: %d | Начальный индекс: %d | Конечный индекс: %d\n", i+1, test_len, test_begin, test_end);

		QueryPerformanceFrequency(&frequency);
		GetProcessMemoryInfo(process, &pmc, sizeof(pmc));
		size_t start_memory = pmc.PeakWorkingSetSize;
		QueryPerformanceCounter(&start_time);

		double ans = func(ptr, test_len, test_begin, test_end);

		if (ans==0) {
			if (test_len>0 && test_begin>=0 && test_begin<test_len && test_end>=0 && test_end<test_len && test_begin<=test_end && ptr!=NULL) {
				errors++;
				puts("- Ошибка: был возращен 0, ожидалось число");
			}
		}
		else {
			if (test_len<=0 || test_begin<0 || test_begin>=test_len || test_end<0 || test_end>=test_len || test_begin>test_end || ptr==NULL) {
				errors++;
				puts("- Ошибка: не был возвращен 0");
			}
			else if (fabs(ans-(sum[test_end+1]-sum[test_begin]))>0.1) {
				errors++;
				printf("- Ошибка: неверный результат | Результат функции: %lf | Правильный результат: %lf\n", ans, sum[test_end+1]-sum[test_begin]);
			}
		}

		QueryPerformanceCounter(&end_time);
		GetProcessMemoryInfo(process, &pmc, sizeof(pmc));
		size_t end_memory = pmc.PeakWorkingSetSize;

		puts("\n--------------------------------------------------------\n");

		total_memory += end_memory - start_memory;
		total_time += (double)(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;

	}

	free(ptr);
	free(sum);

	Sleep(1000);
	printf("- Среднее количество потраченной памяти: %d байт\n", total_memory / TEST_CNT);
	printf("- Среднее время выполнения функции: %.3f сек\n", total_time / TEST_CNT);
	printf("- Ошибок при выполении тестов: %d\n", errors);

}

void test_find_element(int (*func)(double* ptr, int len, double val)) {

	HANDLE process;
	PROCESS_MEMORY_COUNTERS pmc;
	LARGE_INTEGER start_time, end_time, frequency;

	process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());

	puts("Началось тестирование поиска элемента в массиве");
	puts("В случае ошибки проверьте требования к функции из README");
	puts("--------------------------------------------------------\n");
	Sleep(1500);

	double total_time = 0;
	int total_memory = 0;
	int errors = 0;

	srand(time(0));

	for (int i=0; i<TEST_CNT; ++i) {

		int test_len = (rand()%MAX_LEN+1) * (rand()%3-1);
		int test_index = -1;

		double* ptr = malloc(test_len*sizeof(double));
		if (rand()%10==0) ptr = NULL;
		if (test_len>0 && ptr!=NULL) {
			for (int j=0; j<test_len; ++j) ptr[j] = (double) (rand()%(rand()+1)) / (rand()%(rand()+1)+1) - (rand()%(rand()+1)) + (rand()%(rand()+1));
			if (rand()%2==0) test_index = rand() % test_len;
		}

		printf("- Тест #%d | Длина массива: %d | Элемент: %lf\n", i+1, test_len, test_index==-1 ? 0 : ptr[test_index]);

		QueryPerformanceFrequency(&frequency);
		GetProcessMemoryInfo(process, &pmc, sizeof(pmc));
		size_t start_memory = pmc.PeakWorkingSetSize;
		QueryPerformanceCounter(&start_time);

		int ans = func(ptr, test_len, test_index==-1 ? 0 : ptr[test_index]);

		if (ans==-1) {
			if (test_len>0 && test_index>=0 && ptr!=NULL) {
				errors++;
				puts("- Ошибка: был возращен NULL, ожидался указатель");
			}
		}
		else {
			if (test_len<=0 || test_index==-1 || ptr==NULL) {
				errors++;
				puts("- Ошибка: не был возвращен NULL");
			}
			else if (ptr[ans]!=ptr[test_index]) {
				errors++;
				puts("- Ошибка: возращен неверный индекс");
			}
		}

		QueryPerformanceCounter(&end_time);
		GetProcessMemoryInfo(process, &pmc, sizeof(pmc));
		size_t end_memory = pmc.PeakWorkingSetSize;

		puts("\n--------------------------------------------------------\n");

		free(ptr);

		total_memory += end_memory - start_memory;
		total_time += (double) (end_time.QuadPart-start_time.QuadPart) / frequency.QuadPart;

	}

	Sleep(1000);
	printf("- Среднее количество потраченной памяти: %d байт\n", total_memory / TEST_CNT);
	printf("- Среднее время выполнения функции: %.3f сек\n", total_time / TEST_CNT);
	printf("- Ошибок при выполении тестов: %d\n", errors);

}