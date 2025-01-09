#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

#define HT_PRIME_1 151
#define HT_RPIME_2 163

/// @brief 初始化一个hash table item，静态函数，只能在本文件中调用
/// @param k 
/// @param v 
/// @return 
static ht_item * ht_new_item(const char* k, const char* v){
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k); // 复制字符串k到新分配的内存中，返回指向新空间的指针
    i->value = strdup(v);
    return i;
};

/// @brief 初始化一个hash table
/// @return 
ht_hash_table* ht_new(){
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*)); //动态内存分配 分配了一个大小为size的数组，每个元素的大小为ht_item*
    return ht;
};

/// @brief 删除一个hash tabel item， 静态函数，只能在本文件中调用
/// @param i 
static void ht_del_item(ht_item* i){
    free(i->key);
    free(i->value);
    free(i);
};

/// @brief 删除 hash table
/// @param ht 
void ht_del_hash_table(ht_hash_table* ht){
    for(int i = 0; i < ht->size; i++){
        ht_item* item = ht->items[i];
        if(item != NULL){
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

/// @brief 计算字符 s 的hash值
/// @param s 
/// @param a 
/// @param m 
/// @return 字符串s的hash值
static int ht_hash(const char* s, const int a, const int m){
    long hash = 0;
    const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++){
        hash += (long)pow(a, len_s - (i+1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}

/// @brief 双重hash 法计算hash值
/// @param s 
/// @param num_buckets 
/// @param attempt 如果没有冲突 attempt = 0， 如果有冲突 attempt > 0
/// @return 
static int ht_get_hash(const char* s, const int num_buckets, const int attempt){
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_RPIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

// TODO hash method 
void ht_insert(ht_hash_table* ht, const char* key, const char* value){

};
char* ht_search(ht_hash_table* ht, const char* key){

};
void ht_delete(ht_hash_table* ht, const char* key){

};