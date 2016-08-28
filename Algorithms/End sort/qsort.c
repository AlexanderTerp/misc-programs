// Sorts a list of numbers using C's inbuilt qsort function. Used to compare
// my own algorithms.


#include <stdio.h>
#include <stdlib.h>

#define LEN 32

int cmpfunc (const void * a, const void * b);
void check_correctness(int array[], int n);

int main(void) {
	int array[] = {43, 54, 41, 85, -87, -16, 17, -84, -16, 92, 57, 61, 6, 4, 50, 26, 56, -75, 21, 83, -19, -45, -83, -22, -4, -88, 2, -95, -79, 63, 9, -26};

	check_correctness(array, LEN);
	qsort(array, LEN, sizeof(int), cmpfunc);
	check_correctness(array, LEN);

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