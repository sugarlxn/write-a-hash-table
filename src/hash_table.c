#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash_table.h"
#include "prime.h"
#include <stdio.h>

#define HT_PRIME_1 151
#define HT_RPIME_2 163

#define LOAD_TH_MAX 70
#define LOAD_TH_MIN 10

/// 表示已经删除的表项
static ht_item HT_DELETE_ITEM = {NULL, NULL};
/// hash table 初始化大小
const int HT_INITIAL_BASE_SIZE = 10;

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

static ht_hash_table* ht_new_sized(const int base_size){
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    ht->basesize = base_size;
    ht->size = next_prime(ht->basesize);
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}
/// @brief 初始化一个hash table
/// @return 
ht_hash_table* ht_new(){
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
}

/// @brief 删除一个hash tabel item， 静态函数，只能在本文件中调用
/// @param i 
static void ht_del_item(ht_item* i){
    // printf("delete key:%s value:%s \n", i->key, i->value);
    free(i->key);
    free(i->value);
    free(i);
    
};

/// @brief 删除 hash table
/// @param ht 
void ht_del_hash_table(ht_hash_table* ht){
    for(int i = 0; i < ht->size; i++){
        ht_item* item = ht->items[i];
        if(item != NULL && item != &HT_DELETE_ITEM){
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

/// @brief 双重hash 法计算hash值, 使用双重hash方法处理hash碰撞问题
/// @param s 
/// @param num_buckets 
/// @param attempt 如果没有冲突 attempt = 0， 如果有冲突 attempt > 0
/// @return 
static int ht_get_hash(const char* s, const int num_buckets, const int attempt){
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_RPIME_2, num_buckets); 
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;  
}

static void ht_resize(ht_hash_table* ht, const int base_size){
    if(base_size < HT_INITIAL_BASE_SIZE){
        return; 
    }
    ht_hash_table* new_ht = ht_new_sized(base_size);
    for(int i=0; i < ht->size; i++){
        ht_item* item = ht->items[i];
        if(item != NULL && item != &HT_DELETE_ITEM){
            ht_insert(new_ht, item->key, item->value);
        }
    }

    ht->basesize = new_ht->basesize;
    ht->count = new_ht->count;

    // To delete new_ht, we give it ht's size and items 
    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);

}

static void ht_resize_up(ht_hash_table* ht){
    const int new_size = ht->basesize * 2;
    ht_resize(ht, new_size);
}

static void ht_resize_down(ht_hash_table* ht){
    const int new_size = ht->basesize / 2;
    ht_resize(ht, new_size);
}

// hash method 
/// @brief 插入一个item 到 hash table
void ht_insert(ht_hash_table* ht, const char* key, const char* value){
    const int load = ht->count *100 / ht->size;
    if(load > LOAD_TH_MAX){
        ht_resize_up(ht);
    }
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0); 
    ht_item* cur_item = ht->items[index];
    int i=1;
    while (cur_item != NULL && cur_item != &HT_DELETE_ITEM){
        if(cur_item != &HT_DELETE_ITEM){          //update 功能，如果hash值一致，并且key也一致，则更新该item，
            if(strcmp(cur_item->key, key) == 0){  //先删除先前的item 在插入当前item到该位置
                ht_del_item(cur_item);
                ht->items[index] = item;
                return;
            }
        }
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    }
    ht->items[index] = item; //插入 新的 hash item
    ht->count++;
};
/// @brief 搜索hash table 是否存在 某个key
/// the key exit return the item's value, or return NULL
char* ht_search(ht_hash_table* ht, const char* key){
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i=1;
    while (item != NULL){
        if(item != &HT_DELETE_ITEM){ // 跳过deleted item
            if (strcmp(item->key, key) == 0){
                return item->value;
            }
        }
        index = ht_get_hash(key, ht->size, i); //双重hash 方法处理Hash碰撞问题
        item = ht->items[index];
        i++;
    }
    return NULL;
};
/// @brief 开放地址法删除hash表中的一个条目比 search 和 insert 要复杂得多， 因为当前删除的item 可能是碰撞链的一部分，
/// 直接从表格中删除该item会破环该碰撞链(collision chain). 为了解决这个问题， 我们不直接删除item，而是简单地将其标记为删除
/// 我们通过用指向全局哨兵item的指针替换它来将项目标记为已删除，该指针表示存储桶包含已删除的项目。
/// @return 
void ht_delete(ht_hash_table* ht, const char* key){
    const int load = ht->count * 100 / ht->size;
    if(load < LOAD_TH_MIN){
        ht_resize_down(ht);
    }
    int index  = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i=1;
    while (item != NULL){
        if(item != &HT_DELETE_ITEM){
            ht_del_item(item);
            ht->items[index] = &HT_DELETE_ITEM;
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
};

