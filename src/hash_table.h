// hash_table.h
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

// our key-value pairs: items
typedef struct
{
    char *key;   // string
    char *value; // string
} ht_item;

typedef struct
{
    int size;
    int count;
    ht_item **items; // array of pointers to items
} ht_hash_table;

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
static ht_item *ht_new_item(const char *k, const char *v);

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
ht_hash_table *ht_new();

/*
Deletes an entire hash table by systematically freeing all necessary
items first and then freeing the reference to the items array,
and finally by freeing the table reference passed in.

Input:
    ht_hash_table *ht: hash table to delete

Output:
    None
*/
void ht_del_hash_table(ht_hash_table *ht);

/*
Deletes the specified item pointed to by the argument.
This is static as it is used internally by ht_hash_table.

Input:
    ht_item *i: item to delete

Output:
    None
*/
static void ht_del_item(ht_item *i);

#endif /* HASH_TABLE_H */