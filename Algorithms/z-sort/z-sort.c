// Author:			Alexander M. Terp
// Creation date:	2016-08-15
// Purpose:			Using a method dubbed "Z-Sort", sorts a given array of
//					numbers. This particular program prints out every step.
//					Due to printing each step, this program is not 
//					representative of the speed of the algorithm behind Z-Sort.


#include <stdio.h>

// Define unix text colors
#define DARK_RED "\x1B[1;31m"
#define LIGHT_GREEN "\x1B[1;32m"
#define NORMAL "\x1B[0m"

void swap(int *p1, int *p2);
void print_array(int *array, int len, int index);
int * zsort(int array[], int len);

int main(void) {
	//int array[] = {2, 10, 13, 11, 12, 14, 4, 9, 3, 1, 7, 6, 8, 5};
	int array[] = {3,   16,  15,  11,  14,  0,   6,   7,   9,   12,  10,  5,   13,  4,   1,   2};
	int len = sizeof(array) / sizeof(int);

	zsort(array, len);

	print_array(array, len, -2);

	return 0;
}

void swap(int *p1, int *p2) {
	// Given two pointers, swaps their values.
	int temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

void print_array(int array[], int len, int index) {
	// Given an array, prints out each element in a line spaced out.
	printf("\n");
	
	int i;
	for (i = 0; i < len; i++) {
		if (i == index || i == index + 1) {
			if (array[index] > array[index + 1]) {
				printf(LIGHT_GREEN "%d ", array[i]);
			} else {
				printf(DARK_RED "%d ", array[i]);
			}
		} else {
			printf(NORMAL "%d ", array[i]);
		}
	}
}

int * zsort(int array[], int len) {
	// Returns a sorted number array using a method called 'z-sort'.
	int steps = (len - 1) * ((len - 1) + 1) / 2;
	int switches = 0;
	int total_steps = 0;
	int direction = 1; // 1 = Forward, -1 = backward
	int step = 1;
	int index = 0;

	while (step <= steps) {
		int *num1 = &array[index];
		int *num2 = &array[index + 1];

		print_array(array, len, index);
		if (*num1 > *num2) {
			swap(num1, num2);
		}


		if (step - total_steps == (len - 1 - switches)) {
			direction *= (-1);
			switches++;
			total_steps = step;
		} else {
		}
		index += direction;
		step++;
	}
	printf(" Steps: %d\n", steps);

	return array;
}

