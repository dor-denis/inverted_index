#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "index.h"
#include "search.h"

#define SEARCH_PREFIX_LENGTH 8
#define INDEX_PREFIX_LENGTH 7

bool starts_with(const char *pre, const char *str);

int main(int argc, char *argv[])
{
    char *command = calloc(1, sizeof(char));
    node *index = calloc(1, sizeof(node));
    
    if(command == NULL) {
        return 1;
    }
    
    while (1)
    {
        printf("search: search term\nindex: /full/path/to/file\n> ");

        int count_command = 0;
        while (1)
        {
            char new_c = getchar();
            if (new_c != '\n')
            {
                command[count_command++] = new_c;
                command = realloc(command, count_command+1);
            } else 
            {
                command = realloc(command, count_command+1);
                command[count_command] = '\0';
                break;
            }
        }
        
        if (starts_with("index: ", command)) {
            char *filename = malloc(count_command - INDEX_PREFIX_LENGTH + 1);
            if(filename == NULL) {
                return 1;
            }
            memcpy(filename, &command[INDEX_PREFIX_LENGTH], count_command - INDEX_PREFIX_LENGTH + 1);
            
            if(!add_to_index(filename, index)) {
                printf("Could not index\n");
            }
            
            free(filename);
            continue;
        }
        
        if (starts_with("search: ", command)) {
            char *term = malloc(count_command - SEARCH_PREFIX_LENGTH + 1);
            if(term == NULL) {
                return 1;
            }
            memcpy(term, &command[SEARCH_PREFIX_LENGTH], count_command - SEARCH_PREFIX_LENGTH + 1);
            entry *found = find_in_index(term, index);
            printf("Search results for: %s\n", term);
            while(found != NULL) {
                printf("%s: %i:%i\n", found->filename, found->line, found->position);
                found = found->next;
            }
            free(term);
        }
        
        if (strcmp(command, "exit") == 0)
        {
            break;
            
        }
        
        printf("---------------\n");
    }
    
    clean(index);

    free(command);
    return 0;
}

bool starts_with(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre), lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}