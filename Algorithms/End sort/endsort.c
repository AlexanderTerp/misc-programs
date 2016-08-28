// Author:          Alexander M. Terp
// Creation date:   2016-08-25
// Purpose:         Demonstrates what is dubbed the "end sort" sorting 
//                  algorithm, a divide and conquer approach of O(nlogn).

#include <stdio.h>

#define NORMAL "\x1B[0m"
#define LIGHT_RED "\x1B[1;31m"

#define LEN 23      // Number of items to be sorted
#define MAX_NUM_LEN 3   // Length of the longest numbers (for printing purposes)

void endsort(int array[], int n, int start, int LARGER_LEFT);
void swap(int *pointer1, int *pointer2);
void print_array(int array[], int n, int swap1, int swap2);
void check_correctness(int array[], int n);

int main(void) {
    int array[LEN];
    int i;
    for (i = 0; i < LEN; i++) {
        scanf("%d", &array[i]);
    }

    check_correctness(array, LEN);
    for (i = 0; i < 16; i++) {
        endsort(array, LEN, 0, i % 2);
        check_correctness(array, LEN);
    }
    //check_correctness(array, LEN);
    //print_array(array, LEN, -1, -1);

    return 0;
}

void endsort(int array[], int n, int start, int LARGER_LEFT) {
    // Enacts a cycle of what is dubbed the "endsort" sorting algorithm.
    // Acts recursively.
    //printf("NEW CALL | n: %d | start: %d\n", n, start);
    int half = (n + LARGER_LEFT) / 2;

    if (n == 1) {
        return;
    } else {
        int i;
        for (i = 0; i < (n + 1) / 2; i++) {
            if (array[start + i] > array[start + n - i - 1]) {
                //printf("Swap %3d and %3d | \t\t", array[start + i], array[start + n - i - 1]);
                //print_array(array, LEN, start + i, start + n - i - 1);
                swap(&array[start + i], &array[start + n - i - 1]);
            }
        }
    }

    endsort(array, half, start, (LARGER_LEFT + 1) % 2);
    endsort(array, (n + !LARGER_LEFT) / 2, start + half, (LARGER_LEFT + 1) % 2);
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