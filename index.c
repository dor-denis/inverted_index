#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "index.h"

bool append_to_trie(int i, node *parent, char *word, char *filename, unsigned int line, unsigned int pos_of_word);

bool add_to_index(char* filename, node *index) {
    FILE *fp = fopen(filename, "r");
    
    if(fp == NULL) {
        return false;
    }
    
    int pos = 0;
    unsigned int line = 1, pos_in_line = 0, pos_of_word = 0;
    char *word;
    while(!feof(fp)) {
        for (unsigned int c = fgetc(fp); c != EOF; c = fgetc(fp))
        {
            pos_in_line++;
           
            if(pos == 0) {
                pos_of_word = pos_in_line;
                word = calloc(2, sizeof(char)); // allocate a string with one char and \0
                word[1] = '\0';
            }
            if (isalnum(c))
            {
                word[pos] = c;
                word = realloc(word, (++pos + 1));
                word[pos] = '\0';
            }
            else if (pos > 0)
            {
                append_to_trie(0, index, word, filename, line, pos_of_word);
                pos = 0;
                free(word);
            }
            else
            {
                free(word);
            }
            
            if(c == '\n') {
                pos = 0;
                line++;
                pos_in_line = 0;
            }
        }
    }
    
    fclose(fp);
    return true;
}

bool append_to_trie(int i, node *parent, char *word, char *filename, unsigned int line, unsigned int pos_of_word) {
    int pos = word[i];
    
    if(parent->children[pos] == NULL) {
        node *child = calloc(1, sizeof(node));
        parent->children[pos] = child;
    }
    
    node *child = parent->children[pos];
    
    if(child == NULL) {
        return false;
    }
    
    if(word[i+1] == '\0') {
        child->is_word = true;
        entry *entry = malloc(sizeof(entry) + strlen(filename) + 1);
        entry->position = pos_of_word;
        entry->line = line;
        entry->filename = calloc(strlen(filename) + 1, sizeof(char)); // allocate memory including terminating null byte
        memcpy(entry->filename, filename, strlen(filename) + 1); // copy the value of filename incl term byte
        entry->next = child->entry_list;
        child->entry_list = entry;
        return true;
    }
    
    if(!append_to_trie(i+1, child, word, filename, line, pos_of_word)) {
        return false;
    }
    
    return true;
}

void clean(node *node) {
    for(int i = 0; i < ALPHABET; i++) {
        if(node->children[i] != NULL) {
            clean(node->children[i]);    
        }
    }
    
    entry *next;
    for(entry *entry = node->entry_list; entry != NULL; entry = next)
    {
        next = entry->next;
        free(entry->filename);
        free(entry);
    }
    
    free(node);
    return;
}