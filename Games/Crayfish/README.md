# Crayfish Game
A version of the Crayfish maths game written in C. 

Quick rules:
- Pots are 5 dollars.
- Boats are 100 dollars.
- Roll a die: 1, 2, and 3 are good weather, 4 and 5 are bad weather, and 6 is the same weather as the day before (if day 1, then 6 is bad weather).
- Each round, allocate a number of pots to put outside of bay and a number to put inside the bay.
- On good weather: each pot inside the bay give 2 dollars and each pot outside give 6 dollars.
- On bad weather: pots inside the bay give 4 dollars each, pots outside give 0 and are lost.
- Each boat can hold 10 pots.

Known bugs:
- Inputting numbers between periods where you're asked to may become automatically input when you ARE prompted for input (due to buffering).

Small note: This is one of my first projects in C and pointers certainly gave me a bit of a headache. This may not be the prettiest code but I certainly learnt a lot!
