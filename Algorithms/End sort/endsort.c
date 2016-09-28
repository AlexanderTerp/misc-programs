// Author:          Alexander M. Terp
// Creation date:   2016-08-25
// Purpose:         Demonstrates what is dubbed the "end sort" sorting 
//                  algorithm, a divide and conquer approach of O(nlogn).

#include <stdio.h>
#include <stdlib.h>

#define NORMAL "\x1B[0m"
#define LIGHT_RED "\x1B[1;31m"

#define ITEMS 23       // Number of items to be sorted
#define MAX_ITEM_LEN 3      // Length of the longest numbers (for printing purposes)
#define MAX_CYCLES 17 // The max numbers of cycles of the algorithm that are applied.

void recurs_endsort(int array[], int n, int start, int big_left);
void single_endsort(int array[], int numel, int start);
void swap(int *pointer1, int *pointer2);
void print_array(int array[], int n, int swap1, int swap2);
int get_correctness(int array[], int n);

int main(void) {
    int* array = malloc(sizeof(int) * ITEMS);
    int i, num_correct = 0, cycles = 0;
    for (i = 0; i < ITEMS; i++) {
        //printf("%d\n", i);
        scanf("%d", &array[i]);
    }
    //print_array(array, ITEMS, -1, -1);

    get_correctness(array, ITEMS);
    while (num_correct < ITEMS-1 && cycles < MAX_CYCLES) {
        //printf("NEW CYCLE!\n");
        single_endsort(array, ITEMS, 0);
        recurs_endsort(array, ITEMS, 0, cycles % 2);
        num_correct = get_correctness(array, ITEMS);
        cycles++;
    }
    //get_correctness(array, ITEMS);
    //print_array(array, ITEMS, -1, -1);

    return 0;
}

void recurs_endsort(int array[], int numel, int start, int big_left) {
    // Enacts a cycle of what is dubbed the "endsort" sorting algorithm.
    // Acts recursively.
    //printf("New call | numel: %2d | start: %2d | big_left: %d\n", numel, start, big_left);
    
    if (numel == 1 || numel == 0) {
        return;
    }

    int half = (numel + big_left) / 2;
    int odd_numel = numel % 2;

    //printf("F1 | ITEMS: %d | START: %d\n", half, start);
    single_endsort(array, half, start);
    //printf("F2 | ITEMS: %d | START: %d\n", numel - half, start + half);
    single_endsort(array, numel - half, start + half);
    if (odd_numel) {
        //printf("F3 | ITEMS: %d | START: %d\n", numel - half, start);
        single_endsort(array, numel - half, start);
        //printf("F4 | ITEMS: %d | START: %d\n", half, numel - half);
        single_endsort(array, half, start + numel - half);
    }

    //wait(1);
    recurs_endsort(array, numel - half, start, big_left);
    recurs_endsort(array, half, start + numel - half, big_left);
}

void single_endsort(int array[], int numel, int start) {
    int i, j;
    for (i = start, j = start + numel-1; i < (start + numel/2); i++, j--) {
        //printf("Comparing %d and %d\n", array[i], array[j]);
        if (array[i] > array[j]) {
            //printf("Swap %3d and %3d | \t\t", array[i], array[j]);
            //print_array(array, ITEMS, i, j);
            swap(&array[i], &array[j]);
        }
    }
}

void swap(int *pointer1, int *pointer2) {
    // Swap the values of two integer pointers.
    //printf("s");
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

int get_correctness(int array[], int n) {
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
    printf("\nCycle %2d | Amount correct: %3d | %% Correct: %lf\n", 
           cycles++, amount_correct, percentage_correct);
    return amount_correct;
}