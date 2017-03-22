#define DEBUG_H 0

#define DATATYPE Node
#define PROPERTY .f_cost

#define MAKE_PROP(var) (var PROPERTY)

typedef struct node {
    char type;
    int row;
    int col;

    int g_cost; // Distance to start node.
    int h_cost; // Distance to end node.
    int f_cost; // Sum of g_cost and h_cost.

    int is_open;
    int analyzed_once;
    int num_neighbors;
    int heap_index;
    struct node* prev_node;
} Node;

// Function declarations --------------------------------------------------------------------------

void heapify(Node** array, int len, int (*cmp)(Node* a, Node* b));
Node* pop_root(Node** arr, int len, int (*cmp)(Node* a, Node* b));
void add_node ( Node* node, Node** array, int len, int (*cmp)(Node* a, Node* b) );
int get_parent_index(int child_index);
int get_first_child_index(int parent_index);
void check_node( Node** array, int len, int index, int (*cmp)(Node* a, Node* b) );
void swap_nodes(Node** num1, Node** num2);
// int min(Node* a, Node* b);
// int max(Node* a, Node* b);

// Function declarations end ----------------------------------------------------------------------

void heapify(Node** array, int len, int (*cmp)(Node* a, Node* b)) {
    /* Turns any given array of integers into a heap structure following the rules given by the
    given cmp function. */

    // Get the index of the first parent node.
    int first_parent_index = get_parent_index(len - 1);

    // Check that each parent node is in the right position relative to its children.
    // This guarentees a heap structure.
    for (int i = first_parent_index; i >= 0; i--) {
        check_node(array, len, i, cmp);
    }
}

Node* pop_root(Node** arr, int len, int (*cmp)(Node* a, Node* b)) {
    /* Removes the root of the binary tree and returns it. Reorganizes the array back into heap
    form afterwards. */

    // Save the root value and swap it with the last leaf. Then free the memory of the last leaf.
    Node* root = arr[0];
    if (DEBUG_H) printf("Pop ");
    swap_nodes(&arr[0], &arr[--len]);

    // Reorganize the array back into a correct heap structure and return the former root.
    check_node(arr, len, 0, cmp);
    return root;
}

void add_node ( Node* node, Node** array, int len, int (*cmp)(Node* a, Node* b) ) {
    /* Adds a given node to the heap and restructures the heap into proper heap structure following
    the rules described by the given cmp function. */

    if (DEBUG_H) printf("Add node %d\n", node->f_cost);

    // Allocate memory for new node and insert it at the end.
    array[len-1] = node;
    
    // Reorganize the array until it is a proper heap structure following the rules described by
    // the given cmp function.
    int parent_index, index = len-1;
    while (1) {
        // Repeatedly compare the leaf with its parent. If out of position, swap them and check the
        // node's new parent (unless the node is now the root).

        parent_index = get_parent_index(index);
        if (index == 0) {
            // The node is at the root. No more parents to check. Must be in correct position.
            node->heap_index = index;
            break;
        }

        if (cmp(array[index], array[parent_index])) {
            // Node must be out of position relative to its parent.
            swap_nodes(&array[index], &array[parent_index]);
            index = parent_index;
        } else {
            // Node must be in the correct position relative to its parents. Array must be in
            // proper heap structure now.
            node->heap_index = index;
            break;
        }

        
    }
}

int get_parent_index(int child_index) {
    // Given the index of an array, returns the index of its parent. If it does
    // not have a parent (i.e. its the root), it will return its own index. This
    // is dealt with uniquely in the functions that call this one.

    return (child_index - 1)  / 2;
}

int get_first_child_index(int parent_index) {
    // Given the index of an array, returns the index of its parent. If it does
    // not have a parent (i.e. its the root), it will return its own index. This
    // is dealt with uniquely in the functions that call this one.

    if (DEBUG_H) printf("Parent_i %d -> Child_i %d\n", parent_index, 2 * parent_index + 1);
    return (2 * parent_index + 1);
}

void check_node( Node** array, int len, int index, int (*cmp)(Node* a, Node* b) ) {
    /* Given the index of an array, checks whether its in the correct heap 
    position relative to its children according to the rules of the given cmp 
    function. If not, switches with one of the children and recursively calls 
    this function on the node at the new position. */

    // Determine the number of children that the nodes has and save their 
    // indices in an array.
    int children = 0;
    int children_index_arr[2];
    children += ( children_index_arr[0] = get_first_child_index(index) ) <= (len - 1);
    children += ( children_index_arr[1] = get_first_child_index(index) + 1 ) <= (len - 1);
    if (DEBUG_H) printf("Checknode! Index: %d | Number: %d | Children: %d\n", index, array[index]->f_cost, children);

    // Determine which child to swap with to fulfill the heap structure and swap
    // if necessary. TODO: COMMENT
    if (children == 1) {
        if (cmp(array[children_index_arr[0]], array[index])) {
            swap_nodes(&array[index], &array[children_index_arr[0]]);
            check_node(array, len, children_index_arr[0], cmp);
        } else if (DEBUG_H) {
            printf("Correct order, no swap.\n");
        }
    } else if (children == 2) {
        int important_child_index;
        int cmp_result = cmp(array[children_index_arr[0]], array[children_index_arr[1]]);
        if (cmp_result == 1) {
            important_child_index = children_index_arr[0];
        } else {
            important_child_index = children_index_arr[1];
        }

        if (cmp(array[important_child_index], array[index])) {        
            swap_nodes(&array[index], &array[important_child_index]);
            check_node(array, len, important_child_index, cmp);
        }
    } else if (DEBUG_H) {
        printf("Exit check_node, no children.\n");
    }
}

void swap_nodes(Node** num1, Node** num2) {
    if (DEBUG_H) printf("Swap!\n");
    Node* temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}