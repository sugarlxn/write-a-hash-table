#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"

int main(int argc, char* argv[]){

    printf("Good day today !\n");
    ht_hash_table* ht = ht_new();
    ht_del_hash_table(ht);

    return 0;
}