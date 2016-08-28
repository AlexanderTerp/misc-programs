# End sort
This is a sorting algorithm I've dubbed 'End sort' due to the nature of how it sorts numbers. It uses a recursive divide and conquer method and is of O(nlogn). Each iteration, it compares opposite numbers at the ends of the array and swaps them if necessary. It does this until it reaches the middle. Originally, at this point it'd simply recursively call itself on the bottom and top half to repeat the process, although this isn't always capable of fully sorting all arrays. However I'm actively working on altering it to fix this (see the example under sheet 'Example 6 (23)' in the excel file).

This folder also contains 'rng.py', which is just a small generic program that generates random numbers in a text file to test the algorithm on.

TODO:
- Implement new sorting method.
- Expand on comments and documentation.