// Author:			Alexander Terp
// Date created:	2016-08-20
// Purpose:			Plays the Crayfish maths game. Compatible with both 
//					Window and Unix systems.

// Quick rules:
// Pots are 5 dollars
// Boats are 100 dollars
// Roll a die: 1, 2, and 3 are good weather, 4 and 5 are bad weather, and 6 is 
//             the same weather as the day before (if day 1, then 6 is bad).
// Each round, allocate a number of pots outside of bay and a number inside bay.
// On good weather: pots inside the bay give 2 dollars and outside give 6 dollars
// 					per pot.
// On bad weather: pots inside the bay give 4 dollars, outside give 0 and are lost.
// Each boat can hold 10 pots.


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

// Check whether system OS is Windows or Unix.
#ifdef _WIN32
	#define OS_IS_WINDOWS 1
	#include <windows.h>
#else
	#define OS_IS_WINDOWS 0
	#include <unistd.h>
#endif

#define NORMAL "\x1B[0m"
#define DARK_RED "\x1B[0;31m"
#define LIGHT_RED "\x1B[1;31m"
#define LIGHT_GREEN "\x1B[1;32m"
#define LIGHT_CYAN "\x1B[1;36m"

#define NEW_LINE_CHAR 10

#define POT_PRICE 5
#define BOAT_PRICE 100
#define POTS_PER_BOAT 10
#define GOOD_DAY_RETURN_IN 2
#define GOOD_DAY_RETURN_OUT 6
#define BAD_DAY_RETURN_IN 4
#define NUM_STATS 6

void print_table(int history[][NUM_STATS], int day);
void clear_screen();
void update_info(int history[][NUM_STATS], int day);
void pot_allocation(int history[][NUM_STATS], int day);
void wait(int seconds);
void generate_weather(int history[][NUM_STATS], int day);
void post_weather_update(int history[][NUM_STATS], int day);
void purchase(int history[][NUM_STATS], int day);
void clear_scanf_buffer();
	
int main(void) {
	// Seed random number generator using system time.
	srand(time(NULL));
	// Disable output buffering.
	setbuf(stdout, NULL);
	int day = 1;
	int MAX_DAYS;
	clear_screen();
	// Retrieve from the user the number of days they'd like to play for
	while (1) {
		printf("Enter the number of days you'd like to play for (minimum 1): ");
		if (scanf("%d", &MAX_DAYS) != 1 || MAX_DAYS < 1) {
			printf("Please enter a digit greater than 0, try again.\n\n");
		} else {
			break;
		}
	}
	clear_scanf_buffer();
	clear_screen();
	// Prepare a two-dimensional matrix that holds a matrix for each day.
	// Each matrix contains the day, money, pots, boats, pots in and money earned.
	int history[MAX_DAYS][NUM_STATS];
	history[0][0] = day;	// Day #1
	history[0][1] = 0;		// Amount of money
	history[0][2] = 5;		// Number of pots
	history[0][3] = 1;		// Number of boats
	history[0][4] = -1;		// Pots in (-1 indicates blank)
	history[0][5] = -1;		// Money earned (-1 indicates blank)
	history[0][6] = -1;		// Weather (-1 indicates blank)
	while (day <= MAX_DAYS) {
		print_table(history, day);
		pot_allocation(history, day);
		clear_scanf_buffer();
		clear_screen();
		print_table(history, day);
		generate_weather(history, day);
		if (day != MAX_DAYS) {
			update_info(history, day);
		}
		post_weather_update(history, day);
		if (day != MAX_DAYS) {
			purchase(history, day);
			day++;
			clear_scanf_buffer();
			clear_screen();
		} else {
			break;
		}
		
	}
	printf(NORMAL "\n %d days are up! . ", MAX_DAYS);
	wait(1);
	printf(". ");
	wait(1);
	printf(". ");
	wait(1);
	clear_screen();
	print_table(history, day);

	int net_worth = 0;

	// Money in bank.
	net_worth += history[day - 1][1];

	// Value of boats.
	net_worth += BOAT_PRICE * history[day - 1][3];

	// Money earned on the last day.
	net_worth += history[day - 1][5];

	// Add the value of the remaining number of pots.
	if (history[day - 1][6] == 0) { // If it was a bad day
		net_worth += POT_PRICE * (history[day - 1][2] - history[day - 1][4]);
	} else { // If it was a good day
		net_worth += POT_PRICE * (history[day - 1][2]);
	}

	// Remove price of original starting boat and 5 pots.
	net_worth -= BOAT_PRICE + POT_PRICE * 5;

	printf(NORMAL "Congratulations, you've earned $");
	printf(LIGHT_GREEN "%d", net_worth);
	printf(NORMAL " in %d days!\n", MAX_DAYS);
	wait(1);
	printf("Extrapolated, that's $");
	printf(LIGHT_GREEN "%.2lf", net_worth * 365.0/MAX_DAYS);
	printf(NORMAL " a year!\n");
	wait(1);
	printf("Thanks for playing!\n");


	return 0;
}

void clear_screen() {
	// Clears the terminal. Compatible with both Windows and Unix systems.
	if (OS_IS_WINDOWS) {
	    system("cls");
	} else {
		system("clear");
	}
}

void print_table(int history[][NUM_STATS], int day) {
	// Prints out all the information in a table of the days up until 'day'.
	printf("Day | Money | Pots | Boats | Pots in | Pots out | Money earned | Weather\n");

	int i;
	for (i = 1; i <= day; i++) {
		int money = history[i - 1][1];
		int pots = history[i - 1][2];
		int boats = history[i - 1][3];
		int pots_in = history[i - 1][4];
		int money_earned = history[i - 1][5];
		int weather = history[i - 1][6];
		printf(NORMAL "%3d | %5d | %4d | %5d | ", i, money, pots, boats);
		if (pots_in == -1) {
			printf("%7c | ", '-');
			printf("%8c | ", '-');
		} else {
			printf("%7d | ", pots_in);
			printf("%8d | ", pots - pots_in);
		}

		if (money_earned == -1) {
			printf("%12c", '-');
		} else {
			printf("%12d", money_earned);
		}

		if (weather == -1) {
			printf("%7c\n", '-');
		} else if (weather == 0) {
			printf(LIGHT_RED "%7s\n", "Bad");
		} else {
			printf(LIGHT_CYAN "%7s\n", "Good");
		}
	}
	printf("\n");
}

void update_info(int history[][NUM_STATS], int day) {
	// Prepares the information for the next day.
	history[day][1] = history[day - 1][1];
	history[day][2] = history[day - 1][2];
	history[day][3] = history[day - 1][3];
	history[day][4] = -1;
	history[day][5] = -1;
	history[day][6] = -1;
}

void pot_allocation(int history[][NUM_STATS], int day) {
	int pots = history[day - 1][2];
	int *pots_in;
	pots_in = &history[day - 1][4];
	while (1) {
		printf("Enter the number of pots to put inside the bay (max %d): ", pots);
		if (scanf("%d", pots_in) != 1 || *pots_in < 0 || *pots_in > pots) {
			printf("Please enter a digit between 0 and %d, try again.\n\n", pots);
			// Clear buffer in case of letters having been entered.
			while(getchar() != '\n');
		} else {
			break;
		}
	}
}

void wait(int seconds) {
	// Causes a delay as indicated by the 'seconds' parameter.
	// Compatible with both Windows and Unix systems.
	if (OS_IS_WINDOWS) {
		Sleep(1000 * seconds);
	} else {
		sleep(seconds);
	}
}

void generate_weather(int history[][NUM_STATS], int day) {
	// Roll the die and generate a good or bad weather.
	// Randomize # between 1 and 6.
	int random = (rand() % 6) + 1;
	int *good_weather;
	good_weather = &history[day - 1][6];
	switch (random) {
		case 1:
		case 2:
		case 3:
			*good_weather = 1;
			break;
		case 4:
		case 5:
			*good_weather = 0;
			break;
		case 6:
			// Keep the same.
			if (day == 1) {
				*good_weather = 0;
			} else {
				*good_weather = history[day - 2][6];
			}
			break;
	}
	printf("Weather was ");
	wait(1);
	printf(". ");
	wait(1);
	printf(". ");
	wait(1);
	printf(". ");
	if (*good_weather) {
		printf(LIGHT_CYAN "GOOD!");
	} else {
		printf(LIGHT_RED "BAD!");
	}
	printf(NORMAL " (%d)\n", random);
}

void post_weather_update(int history[][NUM_STATS], int day) {
	// Based on what the weather was, updates money earned and
	// if the weather was bad, reduces the number of owned pots by the amount
	// that was outside the bay.
	int pots_in = history[day - 1][4];
	int pots_out = history[day - 1][2] - pots_in;
	int good_weather = history[day - 1][6];
	int money_earned;
	if (good_weather) {
		money_earned = pots_in * GOOD_DAY_RETURN_IN + pots_out * GOOD_DAY_RETURN_OUT;
		printf(NORMAL "You earned $");
		printf(LIGHT_GREEN "%d\n", money_earned);
	} else {
		money_earned = pots_in * BAD_DAY_RETURN_IN;
		history[day][2] -= pots_out;
		printf(NORMAL "You earned $");
		printf(LIGHT_GREEN "%d", money_earned);
		printf(NORMAL ", however you lost ");
		printf(LIGHT_RED "%d", pots_out);
		printf(NORMAL " pots.\n");
	}
	history[day - 1][5] = money_earned;
	history[day][1] += money_earned;
}

void purchase(int history[][NUM_STATS], int day) {
	int *money, *pots, *boats;
	money = &history[day][1];
	pots = &history[day][2];
	boats = &history[day][3];

	printf(NORMAL "\nYou have $");
	printf(LIGHT_GREEN "%d\n", *money);

	int max_boats = *money / BOAT_PRICE;
	int boats_to_buy;
	if (max_boats > 0) {
		while (1) {
			printf(NORMAL "Enter the number of boats you want to buy (max %d): ", max_boats);
			if (scanf("%d", &boats_to_buy) != 1 || boats_to_buy < 0 || boats_to_buy > max_boats) {
				printf("Please enter a digit greater or equal to 0, try again.\n\n");
				// Clear buffer in case of letters having been entered.
				while(getchar() != '\n');
			} else {
				break;
			}
		}
		*money -= boats_to_buy * BOAT_PRICE;
		history[day][3] += boats_to_buy;
	} else {
		printf(NORMAL "You don't have enough money to buy any boats . ");
		wait(1);
		printf(". ");
		wait(1);
		printf(". \n");
	}

	int pots_to_buy;
	int max_pots = *money / POT_PRICE;
	if (max_pots + *pots > *boats * POTS_PER_BOAT) {
		max_pots = *boats * POTS_PER_BOAT - *pots;
	}
	if (max_pots > 0) {
		while (1) {
			printf("Enter the number of pots you'd like to buy (max %d): ", max_pots);
			if (scanf("%d", &pots_to_buy) != 1 || pots_to_buy < 0 || pots_to_buy > max_pots) {
				printf("Please enter a digit greater or equal to 0, try again.\n\n");
				// Clear buffer in case of letters having been entered.
				while(getchar() != '\n');
			} else {
				break;
			}
		}
		history[day][1] -= POT_PRICE * pots_to_buy;
		history[day][2] += pots_to_buy;
	} else {
		printf(NORMAL "You don't have enough boats/money to buy any pots . ");
		wait(1);
		printf(". ");
		wait(1);
		printf(". ");
	}
}

void clear_scanf_buffer() {
	// Clear buffer in case of letters or otherwise unwanted characters having 
	// been entered.
	int ch = getc(stdin);
	ungetc(ch, stdin);
	if (ch != NEW_LINE_CHAR) { // 10 is ASCII for newline
	    while (getchar() != '\n');
	}
}
