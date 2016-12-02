# Author:       Alexander M. Terp
# Date:         26 September, 2016
# Purpose:      Plays a game consisting of "trials". For each trial, a color
#               is shown and the user has to input whether they believe it was
#               the same color as the previous, or different. The challenge is
#               to do this as fast as possible without making mistakes.

from msvcrt import getch
from clint.textui import colored
from math import ceil
from random import uniform
from time import sleep, clock
from os import system, name
from sys import exit

# Define values received when ord(getchar()) is used, i.e. ASCII values.
RIGHT_KEY = 77
LEFT_KEY = 75
SPECIAL_CHAR = 224
EXIT_CHAR = 3

# Define values for the char dimensions in the terminal.
CHAR_HEIGHT = 12
CHAR_WIDTH = 7

# Amount the colored cube is indented.
CUBE_INDENT = 5
# Side lengths of the colored cube in characters.
CUBE_SIDE_LEN = 8

# Define chances for different colors. Chances are RELATIVE! i.e. if
#   colored.green : 0.2,
#   colored.red : 0.2,
#   colored.blue: 0.2
# then each color has a third chance of getting picked, whereas if it were
# 0.2, 0.1, 0.1, then green would have 50% chance, while the other two have 
# 25% chance each.
COLOR_CHANCES = {
                 colored.green : 0.2,
                 colored.red : 0.2,
                 colored.blue : 0.2,
}
SUM_CHANCE = sum(list(COLOR_CHANCES.values()))

# Number of colors that are tested each round.
NUM_TRIALS = 10

def clear_screen():
    """Clears the window, ensures DOS and Unix compatibility"""
    system("cls" if name=="nt" else "clear")
    
def draw_cube(color, char, indent, row_num):
    """Draws a colored cube of row_num side length. Calculates the 
    appropriate number of columns based on character height and width and 
    the row_num."""

    # Calculate the appropriate number of columns based on the dimensions
    # of characters in the terminal and the number of rows.
    col_num = ceil(CHAR_HEIGHT / CHAR_WIDTH) * row_num

    # Print the cube.
    for i in range(row_num):
        print(color(" " * indent + char * col_num))

def get_random_color():
    """Using uniform probability, gets a color under the probabilities defined
    in COLOR_CHANCES. Returns a 'colored' method e.g. colored.red."""
    
    num_colors = len(COLOR_CHANCES)
    rand_num = uniform(0, SUM_CHANCE)

    color_chance_items = list(COLOR_CHANCES.items())
    # List comprehension to get a list containing just the chances.
    chances = [chance[1] for chance in color_chance_items]

    # Find which color was generated.
    for color in range(num_colors):
        if rand_num < sum(chances[0:color + 1]):
            return color_chance_items[color][0]

# Initial screen-clear.
clear_screen()

# Prepare score value to hold the number of correct inputs.
score = 0

# Not correct or incorrect yet
prev_correct = -1

# Print the first color such that for the second color the player can start 
# playing. Show the color for three seconds before going to the first input.
print("\nFirst color:")
prev_color = get_random_color()
draw_cube(prev_color, "#", CUBE_INDENT, CUBE_SIDE_LEN)
for i in range(3):
    print(". ", end="")
    sleep(1)

clear_screen()

# Start the number of trials as defined by NUM_TRIALS as well as the timer.
# Each loop first tells whether the previous input was correct or incorrect, 
# then shows a colored cube and waits for an input.
time_start = clock()
trial = 0
while trial < NUM_TRIALS:
    # Print the result of the previous trial.
    if prev_correct == True:
        print(colored.green("CORRECT!"))
    elif prev_correct == False:
        print(colored.red("WRONG!"))
    else:
        print("")
        # prev_correct must be -1 i.e. first trial, 
        # so can't be correct or wrong yet.
        pass

    # Print the trial number.
    print("#{}".format(trial+1))

    # Randomly generate a color and draw a corresponding cube.
    color = get_random_color()
    draw_cube(color, "#", CUBE_INDENT, CUBE_SIDE_LEN)

    # Get user input.
    key_press = ord(getch())
    if key_press == SPECIAL_CHAR:
        # If key_press is the special character, get the next one for the
        # specific special character.
        key_press = ord(getch())
    elif key_press == EXIT_CHAR:
        # If KeyboardInterrupt has been entered.
        print("Keyboard Interrupt, exiting...")
        exit()
    
    # Left key = SAME as previous color           ####CONTROLS####
    # Right key = DIFFERENT from previous color   ####CONTROLS####
    if (key_press == LEFT_KEY):
        if (prev_color == color): # If CORRECT
            score += 1
            prev_correct = True
        else:
            prev_correct = False
    elif (key_press == RIGHT_KEY):
        if (prev_color != color): # If CORRECT
            score += 1
            prev_correct = True
        else:
            prev_correct = False

    # Prepare for the next round by saving the current color.
    prev_color = color
    trial += 1
    clear_screen()

time_taken = clock() - time_start
print("You got a score of {}/{} in {:.3f} seconds!".format(score, NUM_TRIALS, 
                                                           time_taken))