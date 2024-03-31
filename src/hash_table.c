// hash_table.c
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

/*
This function allocates a chunk of memory the size of an ht_item, and saves
a copy of the string `k` and `v` in the new chunk of memory.
It is static because it is only ever used internally by the hash table.

Input:
    const char* k: a string as the key
    const char* v: a string as the associated value to key

Output:
    ht_item: pointer to the location of the newly created ht_item
*/
static ht_item *ht_new_item(const char *k, const char *v)
{
    // we make a pointer the size of an item
    ht_item *i = malloc(sizeof(ht_item));
    i->key = strdup(k); // because it
    i->value = strdup(v);
    return i;
}

/*
Initialises a new hash table.
`size` defines how many items we can store in this hash table.
We initialise the `items` empty array with calloc to be filled with NULL bytes.
Where a NULL entry indicates that the bucket (that location) is empty.

Input:
    None

Output:
    ht_hash_table: pointer to the location of the newly created hash_table.
*/
ht_hash_table *ht_new()
{
    ht_hash_table *ht = malloc(sizeof(ht_hash_table));

    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item *));
}

/*
Deletes the specified item pointed to by the argument.
This is static as it is used internally by ht_hash_table.

Input:
    ht_item *i: item to delete

Output:
    None
*/
static void ht_del_item(ht_item *i)
{
    free(i->key);
    free(i->value);
    free(i);
}

/*
Deletes an entire hash table by systematically freeing all necessary
items first and then freeing the reference to the items array,
and finally by freeing the table reference passed in.

Input:
    ht_hash_table *ht: hash table to delete

Output:
    None
*/
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