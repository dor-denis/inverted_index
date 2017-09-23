#include <stdbool.h>

#define ALPHABET 255

// Linked list to store entries
typedef struct entry
{
    unsigned int position;
    unsigned int line;
    char *filename;
    struct entry *next;
} entry;

// Trie data structure to store the dictionary
typedef struct node
{
    bool is_word;
    struct entry *entry_list;
    struct node *children[ALPHABET];
} node;

bool add_to_index(char *filename, node *index);

void clean(node *index);