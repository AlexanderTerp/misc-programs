# End sort
This is a sorting algorithm I've dubbed 'End sort' due to the nature of how it sorts numbers. It uses a recursive divide and conquer method and is of O(nlogn). Each iteration, it compares opposite numbers at the ends of the array and swaps them if necessary. It does this until it reaches the middle. Originally, at this point it'd simply recursively call itself on the bottom and top half to repeat the process, although this isn't always capable of fully sorting all arrays.

This folder also contains 'qsort.c' which is just a small generic program that sorts numbers using C's inbuilt qsort function. This is to compare it with my own algorithms.

Likewise, it also contains 'rng.py', which again is just a small generic program that generates random numbers in a text file to test the algorithm on.

The algorithm is not complete and does not work for larger size arrays.

To do:
- Solve the issue where it may get stuck on large arrays e.g. size = 6287587
- Expand on comments and documentation.
