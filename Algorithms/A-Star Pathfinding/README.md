# A-Star Pathfinding
This folder contains my implementation of the [A* search algorithm](https://en.wikipedia.org/wiki/A*_search_algorithm). Includes a seperate file for the heap structure employed to keep track of nodes with lowest f_costs.

Maps are drawn with open, traversable spaces being represented by the '\_' character, walls/non-traversable objects by the 'Z' character, and 'S' and 'E' for the start and end respectively. These maps are interpreted via the load_graph function which loads each character in the map into a matrix of pointers to nodes containing information about what type of node it is ('_', 'Z', 'S', 'E'), whether it's 'open', whether it's been analyzed before, as well as g_cost, h_cost, and f_cost where appropriate.

The program contains a macro value 'ANIMATE'. As described more detailed by the comments in the code, this value (0 - 3) determines whether maps are colored or even shown as the algorithm progressively finds the shortest path. I recommend giving it value "1" as progressively showing the map is particularly laggy due to the slow printf output, particularly when using Windows (better on Linux).

### Examples:
map1:

<img src="https://raw.githubusercontent.com/Terpal47/misc-programs/master/Algorithms/A-Star%20Pathfinding/Pictures/map1_solved.PNG" width="180">

map6:

<img src="https://raw.githubusercontent.com/Terpal47/misc-programs/master/Algorithms/A-Star%20Pathfinding/Pictures/map6_solved.PNG" width="200">

map7:

<img src="https://raw.githubusercontent.com/Terpal47/misc-programs/master/Algorithms/A-Star%20Pathfinding/Pictures/map7_solved.PNG" width="400">

map8:

<img src="https://raw.githubusercontent.com/Terpal47/misc-programs/master/Algorithms/A-Star%20Pathfinding/Pictures/map8_solved.PNG" width="400">

maze1:

<img src="https://raw.githubusercontent.com/Terpal47/misc-programs/master/Algorithms/A-Star%20Pathfinding/Pictures/maze1_solved.PNG" width="200">

### To use (e.g. on map3):
```> gcc -std=c99 -Wall -o main main.c```

```> main Maps/map3```
