// hash_table.h

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