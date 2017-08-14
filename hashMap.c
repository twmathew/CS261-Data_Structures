/*
 * CS 261 Data Structures
 * Assignment 6
 * Name: Thomas Mathew
 * Date: Due 5/29/16
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
    return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(sizeof(HashLink*) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map)
{
    // FIXME: implement

	int count = (map->size);
	//loop through the map
	for (int i = 0; i < count; i++)
	{
		HashLink* link = map->table[i];
		//Go through all links in the current bucket
		while (link != NULL)
		{
			HashLink* temp = link;
			hashLinkDelete(link);
			HashLink* link = temp->next;
		}
	}
}



/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity)
{
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
{
    hashMapCleanUp(map);
    free(map);
}

/**
 * Returns a pointer to the value of the link with the given key. Returns NULL
 * if no link with that key is in the table.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key)
{
    // FIXME: implement

	assert(map != 0);
	assert(key != 0);

	int count = (map->size);
	//loop through the map
	for (int i = 0; i < count; i++)
	{
		HashLink* link = map->table[i];
		//Go through all links in the current bucket
		while (link != NULL)
		{
//should this be if (link->key == HASH_FUNCTION(key)) if (link->key == key)
			if (link->key == key)
			{
				int intResult = (link->value);
				int *intPtr = &intResult;
			return intPtr;
			}
			link = link->next;
		}
	}
	//If match not found, return null
	return NULL;
}



/**
 * Resizes the hash table to have a number of buckets equal to the given
 * capacity. After allocating the new table, all of the links need to be
 * rehashed into it because the capacity has changed.
 * 
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 * 
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
	// FIXME: implement

	//1. Create a new map tempMap with the new capacity.
	//2. Add all links from map into tempMap using the put function.
	//3. Swap the table pointers and capacities of map and tempMap.
	//4. Free tempMap using the delete function.

	printf("\n--- resize called ---\n");
	
	//allocate new hash table
	HashMap* newMap = hashMapNew(capacity);
	//rehash all links into new table
	for (int i = 0; i < (map->size); i++)
	{
		HashLink* link = map->table[i];

		while (link != NULL)
		{
			HashLink* temp = link;
			hashMapPut(newMap, (link->key), (link->value));
			link = temp->next;
		}

	}

	printf("\n--- new Map made ---\n");

	//Delete original
	hashMapDelete(map);
	//Recreate with double capacity
	map = hashMapNew(newMap->capacity);
	//put newMap values into map
	for (int k = 0; k < (newMap->size); k++)
	{
		HashLink* alink = newMap->table[k];
		while (alink != NULL)
		{
			HashLink* atemp = alink;
			hashMapPut(map, (alink->key), (alink->value));
			alink = atemp->next;
		}
	}


	//delete and free old table
	hashMapDelete(newMap);

}



/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
    // FIXME: implement
//	printf("\n--- start of hash put---\n");

	int hashed = HASH_FUNCTION(key);

	int hashIdx = (int)(labs(hashed) % (map->capacity));


	//in case index is negative
	if (hashIdx < 0)
	{
		hashIdx += (map->capacity);
	}

	struct HashLink* newLink = (struct HashLink *) malloc(sizeof(struct HashLink));
	assert(newLink);
	newLink->value = value;
	newLink->next = map->table[hashIdx];
	map->table[hashIdx] = newLink; // add to bucket
	map->size++;

	if (((map->size) / (map->capacity)) > MAX_TABLE_LOAD)
	{
	printf("\n--- max load exceeded ---\n");
	int capInt = (2 * (map->capacity));
		resizeTable(map, capInt);
	}

}


/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(HashMap* map, const char* key)
{
    // FIXME: implement
	assert(map->size != 0);

	//Hash the key
	int hashed = HASH_FUNCTION(key);
	int hashIdx = hashed % (map->capacity);
	HashLink *cur;
	HashLink *last;

	if (hashMapContainsKey(map, key) == 1)
	{
		cur = map->table[hashIdx];
		last = map->table[hashIdx];
		//Go through the bucket and look for a match to remove
		while (cur != 0)
		{
			if (cur->key == key)
			{
				if (cur == map->table[hashIdx]) //special case 1st
				{
					map->table[hashIdx] = cur->next;
				}
				else
				{
					last->next = cur->next;
				}
				free(cur);
				return;
				//cur = 0; //make sure jump out of loop.
			}
			else
			{
				last = cur;
				cur = cur->next;
			}
		}
	}
}



/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(HashMap* map, const char* key)
{
    // FIXME: implement


	assert(map != 0);
	assert(key != 0);

	int count = (map->size);
	//loop through the map
	for (int i = 0; i < count; i++)
	{
		HashLink* link = map->table[i];
		//Go through all links in the current bucket
		while (link != NULL)
		{
			//should this be if (link->key == key)
			if (link->key == key)
			{
				return 1;
			}
			link = link->next;
		}
	}
	//If not found, return 0
    return 0;
}



/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
    // FIXME: implement

    return (map->size);

}


/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
    // FIXME: implement

	return (map->capacity);
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
    // FIXME: implement
	int emptyCount;
	int counter;

	int count = (map->size);
	//loop through the map
	for (int i = 0; i < count; i++)
	{
		HashLink* link = map->table[i];
		counter = 0;
		//Go through all links in the current bucket
		while (link != NULL)
		{
			counter += 1;
			link = link->next;
		}
		if (counter == 0)
		{
			emptyCount += 1;
		}
	}
	return emptyCount;

}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map)
{
    // FIXME: implement

	assert(map->size != 0);
	assert(map->capacity != 0);

	float linkCount = hashMapSize(map);
	float bucketCount = hashMapCapacity(map);
	float result = (linkCount / bucketCount);
	return result;

}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map)
{
    for (int i = 0; i < map->capacity; i++)
    {
        HashLink* link = map->table[i];
        if (link != NULL)
        {
            printf("\nBucket %i ->", i);
            while (link != NULL)
            {
                printf(" (%s, %d) ->", link->key, link->value);
                link = link->next;
            }
        }
    }
    printf("\n");
}