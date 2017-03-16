#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "heap.h"

#define START 'S'
#define END 'E'
#define OBSTACLE 'Z'
#define EMPTY '_'
#define NEW_LINE '\n'
#define PATH_CHAR '.'

// The number of neighbors surrounding a node usually (except edge and corner
// nodes).
#define NUM_SURR 8
#define INIT_HEAP_SIZE 16

#define PRECISION_MULT 10
#define SQRT_2 (int) (PRECISION_MULT * 1.4142)

#define RED "\x1B[1;31m"
#define GREEN "\x1B[1;32m"
#define CYAN "\x1B[1;36m"
#define NORMAL "\x1B[0m"

#define DEBUG 0

/* Three levels of animate:
    1 - Print out colored map once after exit is found.
    2 - Print out a colored map after each step.
    3 - Print out a colored map after each node that is analyzed. */
#define ANIMATE 1

// Check whether system OS is Windows or Unix.
#ifdef _WIN32
    #define OS_IS_WINDOWS 1
    #include <windows.h>
#else
    #define OS_IS_WINDOWS 0
    #include <unistd.h>
#endif

// Define global variables.
int NUM_ROWS;
int NUM_COLS;
Node*** grid;
Node* end_node;

typedef struct {
    Node** nodes;
    int curr_size;
    int num_open_nodes;
} Heap;

Node*** load_map(char *map_name, Heap* open_nodes);
Node** get_neighbors(Node*** grid, Node* node);
int analyze_step(Node*** grid, Node* node, Heap* open_nodes);
int analyze_node(Node* next_node, Node* curr_node);
int get_distance(Node* node1, Node* node2);
int cmp(Node* a, Node* b);
int num_min(int a, int b);
void print_grid(Node*** grid);
void print_node_list(Node** list, int len);
void add_open_node(Node* node, Heap* open_nodes);
void draw_path(Node*** grid, Node* end_node);
void clear_screen();

int main(int argc, char *argv[]) {

    // Ensure a map is provided as a command line argument.
    if (argc != 2) {
        printf("No argument given. Exiting...\n");
        return 0;
    }

    Heap open_nodes;

    open_nodes.curr_size = INIT_HEAP_SIZE;
    open_nodes.nodes = malloc(open_nodes.curr_size * sizeof 
        *(open_nodes.nodes) );
    open_nodes.num_open_nodes = 0;

    grid = load_map(argv[1], &open_nodes);
    //print_grid(grid);

    // Node** neighbors = get_neighbors(grid, grid[3][8]);
    // print_node_list(neighbors, grid[3][8]->num_neighbors);

    // analyze_step(grid, start_node);

    if (ANIMATE) {
        clear_screen();
    }

    // A STAR
    int found = 0;
    while (!found) {
        found = analyze_step(grid, open_nodes.nodes[0], &open_nodes);
    }

    draw_path(grid, end_node);
    printf("Found!\n");

    return 0;
}

Node*** load_map(char *map_name, Heap* open_nodes) {

    // Load in the map file.
    FILE *fp = fopen(map_name, "r");

    // Read the map's dimensions.
    fscanf(fp, "%dx%d\n\n", &NUM_ROWS, &NUM_COLS);

    // Allocate memory for the grid.
    Node*** grid = malloc( NUM_ROWS * sizeof *(grid) );
    for (int i = 0; i < NUM_ROWS; i++) {
        grid[i] = malloc( NUM_COLS * sizeof *(grid[i]));
    }

    // Read in map character by character and save it into the grid.
    char curr_char;
    int row = 0, col = 0;
    while ( (curr_char = getc(fp)) != EOF ) {

        if (curr_char == NEW_LINE) {
            row++;
            col = 0;
            continue;
        }

        // Create and allocate memory for a new node.
        Node* new_node = malloc(sizeof *(new_node));
        new_node->type = curr_char;
        new_node->row  = row;
        new_node->col  = col;

        // Set the status of the node to open or not (implied closed).
        if (new_node->type == EMPTY) {
            new_node->is_open = 1;
            new_node->analyzed_once = 0;
            new_node->heap_index = -1;
            new_node->prev_node = malloc(sizeof *(new_node->prev_node));
        } else if (new_node->type == END) {
            new_node->is_open = 1;
            end_node = new_node;
            new_node->analyzed_once = 0;
            new_node->heap_index = -1;
            new_node->prev_node = malloc(sizeof *(new_node->prev_node));
        } else if (new_node->type == START) {
            new_node->is_open = 1;
            new_node->analyzed_once = 0;
            new_node->heap_index = -1;
            new_node->g_cost = 0;
            add_open_node(new_node, open_nodes);
        } else if (new_node->type == OBSTACLE) {
            new_node->is_open = 0;
        }

        grid[row][col] = new_node;
        if (DEBUG >= 3) printf("Saved %c\n", grid[row][col]->type);

        col++;  
    }

    if (DEBUG >= 2) printf("load_map function finished.\n");
    return grid;
}

Node** get_neighbors(Node*** grid, Node* node) {
    /* Returns an array of pointers to neighboring open nodes of the 
    given node. */

    int counter = 0;
    Node** neighbors = malloc(NUM_SURR * sizeof *(neighbors));

    for (int row = node->row - 1; row <= node->row + 1; row++) {
        for (int col = node->col - 1; col <= node->col + 1; col++) {

            if ( (row == node->row && col == node->col) ||
                row < 0 || row >= NUM_ROWS ||
                col < 0 || col >= NUM_COLS ||
                !grid[row][col]->is_open) {
                continue;
            }

            neighbors[counter++] = grid[row][col];
        }
    }

    node->num_neighbors = counter;
    if (DEBUG == 2) printf("num_neighbors: %d\n", node->num_neighbors);
    return neighbors;
}

int analyze_step(Node*** grid, Node* node, Heap* open_nodes) {
    // TODO IMPLEMENT HEAP

    if (DEBUG) printf("New step: (%d, %d)\n", node->row, node->col);
    
    node->is_open = 0;

    pop_root(open_nodes->nodes, (open_nodes->num_open_nodes)--, cmp);

    int found;
    Node** neighbors = get_neighbors(grid, node);

    for (int i = 0; i < node->num_neighbors; i++) {
        found = analyze_node(neighbors[i], node);

        if (found) {
            end_node = neighbors[i];
            return 1;
        }

        if (!neighbors[i]->analyzed_once) {
            add_open_node(neighbors[i], open_nodes);
            neighbors[i]->analyzed_once = 1;
        } else {
            int parent_index = get_parent_index(neighbors[i]->heap_index);
            check_node(open_nodes->nodes, open_nodes->num_open_nodes, parent_index, cmp);
        }
    }

    if (ANIMATE == 2) {
        clear_screen();
        print_grid(grid);
    }

    return 0;
}

int analyze_node(Node* next_node, Node* curr_node) {
    /* Given a node "next_node", analyzes and updates its g_cost, h_cost, and f_cost. */

    int g_cost = get_distance(next_node, curr_node) + curr_node->g_cost;

    if (next_node->analyzed_once) {
        // If the node has already been analyzed, then h_cost will not change.
        // Just check if g_cost is lower, and if so, update it and f_cost.
        if (g_cost < next_node->g_cost) {
            next_node->g_cost = g_cost;
            next_node->f_cost = next_node->g_cost + next_node->h_cost;
            next_node->prev_node = curr_node;
            if (DEBUG) {
                printf("New path defined: (%d, %d) -> (%d, %d)\n", next_node->prev_node->row, 
                    next_node->prev_node->col, next_node->row, next_node->col);
            }
        } else {
            return 0;
        }
    } else {
        next_node->g_cost = g_cost;
        next_node->h_cost = get_distance(next_node, end_node);
        next_node->f_cost = next_node->g_cost + next_node->h_cost;

        next_node->prev_node = curr_node;
        if (DEBUG) {
            printf("New path defined: (%d, %d) -> (%d, %d)*\n", next_node->prev_node->row, 
                next_node->prev_node->col, next_node->row, next_node->col);
        }
    }

    if (DEBUG) {
        printf("(%d, %d): g_cost %d | h_cost %d | f_cost %d\n", next_node->row, 
            next_node->col, next_node->g_cost, next_node->h_cost, next_node->f_cost);
    }

    if (ANIMATE == 3) {
        clear_screen();
        print_grid(grid);
    }

    return (next_node->type == END);
}

int get_distance(Node* node1, Node* node2) {

    int dx = abs(node1->col - node2->col);
    int dy = abs(node1->row - node2->row);

    return num_min(dx, dy) * SQRT_2 + abs(dx - dy) * PRECISION_MULT;
}

int num_min(int a, int b) {
    return (a < b) ? a : b;
}

int cmp(Node* a, Node* b) {
    if (DEBUG_H) printf("Minning %d and %d -> %d\n", a->f_cost, b->f_cost, (a->f_cost < b->f_cost));
    if (a->f_cost == b->f_cost) {
        return (a->h_cost < b->h_cost);
    } else {
        return (a->f_cost < b->f_cost);
    }
}

// int max(Node a, Node b) {
//     if (DEBUG_H) printf("Maxing %d and %d -> %d\n", a.f_cost, b.f_cost, (a.f_cost > b.f_cost));
//     return (a.f_cost > b.f_cost);
// }

void print_grid(Node*** grid) {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            if (ANIMATE) {
                if (grid[i][j]->is_open && grid[i][j]->analyzed_once)
                    printf(GREEN "%c", grid[i][j]->type);
                else if (!grid[i][j]->is_open && grid[i][j]->analyzed_once)
                    printf(RED "%c", grid[i][j]->type);
                else
                    printf(NORMAL "%c", grid[i][j]->type);
            } else {
                printf("%c", grid[i][j]->type);
            }
        }
        printf("\n");
    }
}

void print_node_list(Node** list, int len) {
    for (int i = 0; i < len; i++) {
        printf("%c", list[i]->type);
    }
    printf("\n");
}

void add_open_node(Node* node, Heap* open_nodes) {
    /* Adds a given node to the 'open' heap collection. */

    open_nodes->num_open_nodes++;
    if (open_nodes->num_open_nodes > open_nodes->curr_size) {
        open_nodes->curr_size *= 2;
        open_nodes->nodes = realloc(open_nodes->nodes, open_nodes->curr_size * 
            sizeof *(open_nodes->nodes) );
    }

    add_node(node, open_nodes->nodes, open_nodes->num_open_nodes, cmp);
}

void draw_path(Node*** grid, Node* end_node) {

    Node* curr_node = end_node->prev_node;
    while (curr_node->type != START) {
        grid[curr_node->row][curr_node->col]->type = PATH_CHAR;
        curr_node = curr_node->prev_node;
    }

    print_grid(grid);
}

void clear_screen() {
    // Clears the terminal. Compatible with both Windows and Unix systems.
    if (OS_IS_WINDOWS) {
        system("cls");
    } else {
        system("clear");
    }
}