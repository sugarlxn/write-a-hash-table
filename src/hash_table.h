#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct{
    int basesize; //初始大小
    int size;
    int count;
    ht_item** items; // Array of pointers to ht_items
} ht_hash_table;

// Public functions
ht_hash_table* ht_new();
void ht_del_hash_table(ht_hash_table* ht);

// hash method 
void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* ht, const char* key);

#endif // HASH_TABLE_H