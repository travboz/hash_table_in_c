// hash_table.c
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"

#define HT_PRIME_1 129
#define HT_PRIME_2 131
#define HT_INITIAL_BASE_SIZE 50

// value representing a deleted item
static ht_item HT_DELETED_ITEM = {NULL, NULL};

static ht_item *ht_new_item(const char *k, const char *v)
{
    // we make a pointer the size of an item
    ht_item *i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

ht_hash_table *ht_new()
{
    ht_hash_table *ht = malloc(sizeof(ht_hash_table));

    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item *));

    return ht;
}

void ht_del_hash_table(ht_hash_table *ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        ht_item *item = ht->items[i];

        if (item != NULL)
        {
            ht_del_item(item);
        }
    }

    free(ht->items);
    free(ht);
}

static void ht_del_item(ht_item *i)
{
    free(i->key);
    free(i->value);
    free(i);
}

static int ht_hash(const char *s, const int large_prime, const int num_buckets)
{
    long hash = 0;
    const int len_s = strlen(s);

    for (int i = 0; i < len_s; i++)
    {
        hash += (long)pow(large_prime, len_s - (i + 1)) * s[i];
        hash = hash % num_buckets;
    }

    return (int)hash;
}

static int ht_get_hash(const char *s, const int num_buckets, const int attempt)
{
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);

    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void ht_insert(ht_hash_table *ht, const char *key, const char *value)
{
    const int load = ht->count * 100 / ht->size;
    if (load > 70)
    {
        ht_resize_up(ht);
    }

    ht_item *item = ht_new_item(key, value);         // make a new item from the arguments
    int index = ht_get_hash(item->key, ht->size, 0); // compute the hash with the new item's key and the current max size

    ht_item *cur_item = ht->items[index]; // retrieve the item with that index ^
    int i = 1;                            // initiate the hash collision to 1
    while (cur_item != NULL)              // if there's a hash collision, move until there is a free spot
    {
        if (cur_item != &HT_DELETED_ITEM)
        {
            if (strcmp(cur_item->key, key) == 0)
            {
                ht_del_item(cur_item);
                ht->items[index] = item;
                return;
            }
        }
        // get the calculate a new hash (using hash_b this time)
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index]; // get the current item
        i++;                         // increase hash count
    }
    ht->items[index] = item; // we've found a free spot, so store out new item there
    ht->count++;             // hash_table now holds another item so we increase the current count of items
}

char *ht_search(ht_hash_table *ht, const char *key)
{
    int index = ht_get_hash(key, ht->size, 0); // calculate the index (hash) of the item with the key we're looking for
    ht_item *item = ht->items[index];          // grab the item at the index

    // in case of hashes we iterate through
    int i = 1;
    while (item != NULL)
    {                                 // while there are still items ahead
        if (item != &HT_DELETED_ITEM) // and the current item hasn't been marked as deleted
        {
            if (strcmp(item->key, key) == 0)
            { // if our key matches the item's key
                return item->value;
            }
        }
        // move to the next item
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    return NULL; // if we don't find the item
}

void ht_delete(ht_hash_table *ht, const char *key)
{
    const int load = ht->count * 100 / ht->size;
    if (load < 10)
    {
        ht_resize_down(ht);
    }

    // get the item
    int index = ht_get_hash(key, ht->size, 0);
    ht_item *item = ht->items[index];

    // iterate through the buckets and find the item
    int i = 1;
    while (item != NULL)
    {
        // if the address of the item doesn't point to a HT_DELETED_ITEM
        if (item != &HT_DELETED_ITEM)
        {
            // if the current item's key is the same as the one we're looking for
            if (strcmp(item->key, key) == 0)
            {
                // delete it
                ht_del_item(item);
                // point that bucket to deleted
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        // move to the next item
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    // we've deleted an item so reduce the count
    ht->count--;
}

static ht_hash_table *ht_new_sized(const int base_size)
{
    ht_hash_table *ht = xmalloc(sizeof(ht_hash_table));
    ht->base_size = base_size;

    ht->size = next_prime(ht->base_size);

    ht->count = 0;
    ht->items = xcalloc((size_t)ht->size, sizeof(ht_item *));
    return ht;
}

ht_hash_table *ht_new()
{
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
}

static void ht_resize(ht_hash_table *ht, const int base_size)
{
    if (base_size < HT_INITIAL_BASE_SIZE)
    {
        return;
    }
    ht_hash_table *new_ht = ht_new_sized(base_size);
    for (int i = 0; i < ht->size; i++)
    {
        ht_item *item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM)
        {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    // To delete new_ht, we give it ht's size and items
    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item **tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}

static void ht_resize_up(ht_hash_table *ht)
{
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}

static void ht_resize_down(ht_hash_table *ht)
{
    const int new_size = ht->base_size / 2;
    ht_resize(ht, new_size);
}