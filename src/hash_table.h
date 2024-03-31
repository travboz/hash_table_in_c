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

/*
Hash the string to insert using a generic string hashing function.
`large_prime` is chosen to be larger than the ASCII alphabet size
as that is how large our alphabet is (so, > 128).

The hashing function follows this pseduocode:
function hash(string, a, num_buckets):
    hash = 0
    string_len = length(string)
    for i = 0, 1, ..., string_len:
        hash += (a ** (string_len - (i+1))) * char_code(string[i])
    hash = hash % num_buckets
    return hash

1. Convert the string to a large integer.
2. Reduce the size of that integer to a fixed range by taking its remainder `mod num_buckets`
where `num_buckets` is how many buckets in our hash table.

Input:
    const char *s: string to hash
    const int large_prime: a prime number larger than the size of the alphabet used in the string
    const int num_buckets: the number of buckets in the hash table

Output:
    int: the newly hashed value for the string s.
*/
static int ht_hash(const char *s, const int large_prime, const int num_buckets);

// Preventing collisions by using open addressing and double hashing
/*
Double hashing to reduce collisions using the following structure:

    `index = (hash_a(string) + i * (hash_b(string) + 1)) % num_buckets`

where `i` represents the number of collisions that have occurred.
If zero collisions occur we can notice the following:

    `index = (hash_a(string) % num_buckets`

`i = 0` removes the use of hash_b.

*/
static int ht_get_hash(const char *s, const int num_buckets, const int attempt);

/*
Inserting into the hash table by calculating the hash for the new item,
checking if the bucket at that hash is empty (NULL) and, if not empty, then
double hashing to find a new bucket. If empty, just putting the item
into the bucket.

Input:
    ht_hash_table *ht: pointer to the hash table to insert into
    const char *key: key of new item
    const char *value: value of new item

Output:
    None
*/
void ht_insert(ht_hash_table *ht, const char *key, const char *value);

/*
Search for the hash in the hash table and if it's not there, linearly search
for it.

Input:
    ht_hash_table *ht: pointer to the hash table to search
    const char *key: key of item
    const char *value: value of item

Output:
    None
*/
char *ht_search(ht_hash_table *ht, const char *key);

/*
Instead of deleting the item from the table, and thereby breaking the linear
probing chain, we mark the key-value pair as deleted and modify the search
and insert functions with checks for deletion markers.

Input:
    ht_hash_table *ht: pointer to the hash table to search
    const char *key: key of item

Output:
    None
*/
void ht_delete(ht_hash_table *ht, const char *key);

#endif /* HASH_TABLE_H */