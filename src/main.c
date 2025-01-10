#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

int main(int argc, char* argv[]){

    printf("Good day today !\n");
    ht_hash_table* ht = ht_new();
    ht_insert(ht, "key1", "value1");
    ht_insert(ht, "key2", "value2");
    ht_insert(ht, "key3", "value3");
    ht_insert(ht, "key4", "value4");
    ht_insert(ht, "name", "lxn");

    char* value = ht_search(ht, "key1");
    printf("key1: %s\n", value);
    value = ht_search(ht, "key2");
    printf("key2: %s\n", value);

    ht_delete(ht, "key1");
    value = ht_search(ht, "key1");
    if(value == NULL){
        printf("key1 is deleted\n");
    }

    ht_insert(ht, "key1", "value1");
    value = ht_search(ht, "key1");
    printf("key1: %s\n", value);

    ht_insert(ht, "key1", "new_value1");
    value = ht_search(ht, "key1");
    printf("key1: %s\n", value);
    
    ht_del_hash_table(ht);

    return 0;
}