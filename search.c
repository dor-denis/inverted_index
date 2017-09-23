#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "search.h"

struct entry * find_in_index(const char *term, node *index) 
{
    if(index == NULL) {
        return NULL;
    }
    
    int i = 0;
   
    node *node = index;
    while(term[i] != '\0' && node->children[term[i]] != NULL) {
        node = node->children[term[i]];
        i++;
    }
    
    if(node->is_word && i == strlen(term)) {
        return node->entry_list;
    }
    
    return NULL; 
}