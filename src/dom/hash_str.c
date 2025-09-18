/*
 * Notes
 * 
 */

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dom/hash_str.h"

/********************/
/*      defines     */
/********************/

// todo: make this reallocation automatic
#define INITIAL_SIZE 120
#define INITIAL_MULTIPLIER 6

/********************/
/* static variables */
/********************/

typedef struct hash_str_t
{
    struct hash_str_t*      next;

    unsigned char*    data;
    uint32_t          data_size;
    hash_str_t        hash;
} str_t;

static str_t*   table;
static uint32_t table_size  = 0;
static uint32_t next_slot   = 0;
static uint32_t total_nodes = 0;

/********************/
/* static functions */
/********************/

static str_t* get_free_node()
{
    assert(next_slot < total_nodes);

    return &table[next_slot++];
}

static str_t* find_node(hash_str_t hash)
{
    uint32_t idx    = hash % table_size;
    str_t* node     = &table[idx];

    while (node)
    {
        if (node->hash == hash) { return node; }
        node = node->next;
    }

    return NULL;
}


// http://www.cse.yorku.ca/~oz/hash.html
static hash_str_t hash_str(const unsigned char* str, const uint32_t str_size)
{
    uint32_t hash = 5381;
    uint32_t i = 0;

    while (i < str_size)
    {
        hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */
        i++;
    }

    return hash;
}


static void populate(str_t* node, hash_str_t hash, const unsigned char* str, const uint32_t str_size)
{
    node->hash = hash;
    node->data = malloc(str_size);
    node->data_size = str_size;

    memcpy(node->data, str, str_size);
}


static void append(hash_str_t hash, const unsigned char* str, const uint32_t str_size)
{
    str_t* node = find_node(hash);

    if (node) { return; }

    uint32_t idx = hash % table_size;

    if (table[idx].hash == 0)
    {
        populate(&table[idx], hash, str, str_size);
    }
    else
    {
        str_t* new = get_free_node();
        populate(new, hash, str, str_size);

        node = &table[idx];
        while (node->next) { node = node->next; }
    
        node->next = new;
    }
}


// static void grow()
// {
//     // todo: allocate new table
//     // allocate string space
//     // move everything from old to new table
// }

/********************/
/* public functions */
/********************/

void hash_str_pool_new()
{
    if (table) { return; }

    table       = malloc(sizeof(str_t) * INITIAL_SIZE * INITIAL_MULTIPLIER);

    printf("Allocating string pool: %lu\n", sizeof(str_t) * INITIAL_SIZE * INITIAL_MULTIPLIER);

    total_nodes = INITIAL_SIZE * INITIAL_MULTIPLIER;
    table_size  = INITIAL_SIZE;
    next_slot   = INITIAL_SIZE;

    append(0, "VL-EMPTY", 8);
}


hash_str_t hash_str_new(const unsigned char* str, const uint32_t str_size)
{
    assert(table);

    if (str_size == 0) { return 0; }

    hash_str_t hash = hash_str(str, str_size);
    append(hash, str, str_size);

    return hash;
}


hash_str_t hash_str_compute(const unsigned char* str, const uint32_t str_size)
{
    return hash_str(str, str_size);
}


const unsigned char* hash_str_get(hash_str_t hash)
{
    str_t* node = find_node(hash);
    assert(node);

    return node->data;
}


uint32_t hash_str_get_size(hash_str_t hash)
{
    str_t* node = find_node(hash);
    assert(node);

    return node->data_size;
}


void hash_str_print(hash_str_t hash)
{
    const unsigned char* str = hash_str_get(hash);
    const uint32_t str_size = hash_str_get_size(hash);

    printf("%u - %.*s\n", hash, str_size, str);
}


void hash_str_pool_stats()
{
    for (uint32_t i = 0; i < table_size; i++)
    {
        if (table[i].hash == 0) { continue; }

        printf("[%u] +", i);

        str_t* node = &table[i];

        while (node)
        {
            printf("+");
            node = node->next;
        }

        printf("\n");
    }
}


void hash_str_pool_free()
{
    if (!table) { return; }

    for (uint32_t i = 0; i < total_nodes; i++)
    {
        free(table[i].data);
    }

    free(table);
}