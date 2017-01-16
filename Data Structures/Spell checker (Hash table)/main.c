#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define MAX_LEN 45

#define DEFAULT_DICT "dictionaries/large"

typedef struct Node {
    char word[MAX_LEN + 1];
    struct Node *next;
} Node;

int get_dict_len(char *dict_dir);
int hash(char *word, int hash_table_len);
void load(Node **hash_table, int hash_table_len, char *dict_dir);
void insert(Node* list, char* word);

int main(int argc, char *argv[]) {

    char *dictionary = (argc == 2) ? argv[1] : DEFAULT_DICT;
    int dict_len = get_dict_len(dictionary);
    int hash_table_len = ceil(sqrt(dict_len));

    // Node **hash_table = { 0 };

    Node *hash_table[hash_table_len];;
    // for (int i = 0; i < hash_table_len; i++) {
    //     hash_table[i] = calloc(1, sizeof *(hash_table[i]) );
    // }

    load(hash_table, hash_table_len, dictionary);
    //printf("%s\n", hash_table[100]->word);

    return 0;
}

int get_dict_len(char *dict_dir) {
    /* Given the string for the directory of a dictionary, returns the number 
    of words inside it. */

    FILE *file = fopen(dict_dir, "r");
    assert(file != NULL);

    char word[MAX_LEN + 1];
    int counter = 0;
    while (fscanf(file, "%s", word) != EOF) {
        counter++;
    }
    
    return counter;
}

int hash(char *word, int hash_table_len) {
    /* Takes a string and calculates a corresponding hash bucket. Is consistent
    when given the same input, as is the requirement for a hash function. */

    // Sums up the ASCII value of each character in the word.
    int sum = 0;
    int i = 0;
    while (word[i] != '\0') {
        sum += word[i++];
    }
    
    // Seed the random number generator with the sum.
    srand(sum);
        
    // Hash bucket gets calculated and returned.
    return ( rand() % hash_table_len );
}

void load(Node *hash_table[], int hash_table_len, char *dict_dir) {
    /* Given a pointer to a hash table and the string directory to a dictionary,
    hashes the dictionary and populates the hash table. */

    FILE *file = fopen(dict_dir, "r");
    assert(file != NULL);

    int hash_bucket;

    char word[MAX_LEN + 1];
    while (fscanf(file, "%s", word) != EOF) {
        hash_bucket = hash(word, hash_table_len);
        insert( &(hash_table[hash_bucket]), word );
    }
}

void insert(Node** list, char* word) {
    /* Given a linked list and a word, creates a new node for the word and adds
    the node to the front of the linked list. */
    
    Node *new_node = calloc(1, sizeof *(new_node));
    assert(new_node != NULL);

    strcpy(new_node->word, word);

    if ( *list == NULL ) {
        list = &new_node;
    } 
    else {
        new_node->next = list->next;
        list->next = new_node;
    }
}

// Node* init_node() {
//     Node *new_node = calloc(1, sizeof *(new_node));
//     new_node->word = NULL;
//     new_node->next = NULL;

//     return new_node;
// }