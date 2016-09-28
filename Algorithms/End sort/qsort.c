// Sorts a list of numbers using C's inbuilt qsort function. Used to compare
// my own algorithms.


#include <stdio.h>
#include <stdlib.h>

#define ITEMS 524288

int cmpfunc (const void * a, const void * b);
void check_correctness(int array[], int n);

int main(void) {
	int* array = malloc(sizeof(int) * ITEMS);
    int i;
    for (i = 0; i < ITEMS; i++) {
        //printf("%d\n", i);
        scanf("%d", &array[i]);
    }
	check_correctness(array, ITEMS);
	qsort(array, ITEMS, sizeof(int), cmpfunc);
	check_correctness(array, ITEMS);

	return 0;
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void check_correctness(int array[], int n) {
	// Given an array, checks the number of numbers that are followed by
	// a number greater than itslef (i.e. sorted in groups of 2).
	// Then prints the results (also keeps track of endsort cycles).
	static int cycles = 0;
	int amount_correct = 0;
	int i;
	for (i = 0; i < n - 1; i++) {
		if (array[i] <= array[i + 1]) {
			amount_correct++;
		}
	}
	double percentage_correct =  (double) amount_correct / (n - 1);
	printf("Cycle %d | Amount correct: %d | %% Correct: %lf\n", cycles++, amount_correct, percentage_correct);
}