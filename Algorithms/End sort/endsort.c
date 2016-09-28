// Author:          Alexander M. Terp
// Creation date:   2016-08-25
// Purpose:         Demonstrates what is dubbed the "end sort" sorting 
//                  algorithm, a divide and conquer approach of O(nlogn).

#include <stdio.h>
#include <stdlib.h>

#define NORMAL "\x1B[0m"
#define LIGHT_RED "\x1B[1;31m"

#define ITEMS 3210371       // Number of items to be sorted
#define MAX_ITEM_LEN 3      // Length of the longest numbers (for printing purposes)
#define CYCLES 24 // The number of cycles of the algorithm that are applied.

void endsort(int array[], int n, int start, int start_large_left);
void swap(int *pointer1, int *pointer2);
void print_array(int array[], int n, int swap1, int swap2);
void check_correctness(int array[], int n);

int main(void) {
    int* array = malloc(sizeof(int) * ITEMS);
    int i;
    for (i = 0; i < ITEMS; i++) {
        //printf("%d\n", i);
        scanf("%d", &array[i]);
    }

    check_correctness(array, ITEMS);
    for (i = 0; i < CYCLES; i++) {
        endsort(array, ITEMS, 0, i % 2);
        check_correctness(array, ITEMS);
    }
    //check_correctness(array, ITEMS);
    //print_array(array, ITEMS, -1, -1);

    return 0;
}

void endsort(int array[], int n, int start, int start_large_left) {
    // Enacts a cycle of what is dubbed the "endsort" sorting algorithm.
    // Acts recursively.
    //printf("NEW CALL | n: %2d | start: %2d | start_large_left: %d\n", n, start, start_large_left);
    int half = (n + start_large_left) / 2;

    if (n == 1) {
        return;
    } else {
        int i, j;
        for (j = 0; j < 1 + (n % 2); j++) {
            for (i = 0; i < (n + j + 1) / 2; i++) {
                if (array[start + i] > array[start + n + j - i - 1]) {
                    //printf("Swap %3d and %3d | \t\t", array[start + i], array[start + n + j - i - 1]);
                    //print_array(array, ITEMS, start + i, start + n + j - i - 1);
                    swap(&array[start + i], &array[start + n + j - i - 1]);
                }
            }
        }
    }

    endsort(array, half, start, start_large_left);
    endsort(array, (n + !start_large_left) / 2, start + half, start_large_left);
}

void swap(int *pointer1, int *pointer2) {
    // Swap the values of two integer pointers.
    int temp = *pointer1;
    *pointer1 = *pointer2;
    *pointer2 = temp;
}

void print_array(int array[], int n, int swap1, int swap2) {
    // Prints an array of numbers in a straight line. If given two valid indexes
    // (swap1 and swap2), will print them red when encountered.
    int i;
    for (i = 0; i < n; i++) {
        if (i == swap1 || i == swap2) {
            printf(LIGHT_RED "%3d ", array[i]);
            printf(NORMAL);
        } else {
            printf("%3d ", array[i]);
        }
    }
    printf("\n");
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
    printf("Cycle %2d | Amount correct: %3d | %% Correct: %lf\n", cycles++, amount_correct, percentage_correct);
}