#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <assert.h>
#include "hashmap.h"
#include "hashmaplib.h"

#define HMAP_NULL_CHECK(val)  if (val == NULL) return -EINVAL; /**< Check for NULL @hideinitializer */

/**
 * A global hashmap pointer
 * All the operations are performed over this map
 * More details about this value.
 */
static hmap_t g_map;

/**
 * A structure to represent hashmap element data structure
 */
typedef struct userelem_t {
	uint8_t   key[20]; /**< The Key attribute for hashmap   */
	void      *value;  /**< the Value attribute for hashmap */
} userelem;

/**
 * Internal functions
 */
static int32_t iter_elem(void *elem, void *arg)
{
	userelem *el = (userelem *) elem;
	void (*printdata)(void*) = arg;
	printdata(el->value);
	return 0;
}

static int32_t free_elem(void* elem, void *arg)
{
	userelem *el = (userelem *) elem;
	UNUSED(arg);
	if(el)
	{
		if(el->value)
		{
			free(el->value);
			el->value = NULL;
		}
		free(el);
		el = NULL;
	}
	return 0;
}

static int32_t free_data(void* data, void *arg)
{
	UNUSED(arg);
	free(data);
	return 0;
}

/**
 * Library Functions
 */

/**
* @brief Create and initialize a hashmap
*
* @return HMAP_S_OK for success
*/
int32_t hashmap_initialize(void)
{
	g_map = hashmap_create();
	if(g_map != NULL)
		return HMAP_S_OK;
	else
		return HMAP_E_OUTMEM;
}

/**
* @brief Uninitialize and destroy the hashmap
*/
void hashmap_uninitialize(void)
{
	hashmap_destroy(g_map, free_data, 0);
	g_map = NULL;
	return;
}

/**
* @brief Remove a hashmap entry for the given key
*
* @param key  Key for which the entry needs to be removed
* @param data Buffer to hold the removed entry data
* @param len  Size of the buffer that holds the data
*
* @return HMAP_S_OK for success
* @return HMAP_E_NOTFOUND for Key not found
*/
int32_t hashmap_remove_elem(uint8_t* key, void* data, size_t len)
{
	userelem *dat = NULL;
	int32_t ret = 0;

	HMAP_NULL_CHECK(key);
	HMAP_NULL_CHECK(data);
	if(len <= 0)
		return -EINVAL;

	ret = hashmap_remove(g_map, key, (void_ptr) &dat);
	if(ret == HMAP_S_OK)
	{
		memcpy(data, dat->value, len);
		free_elem(dat, 0);
	}
	return ret;
}

/**
* @brief Update a key-value pair into the hashmap
*
* @param key  Key for which the entry needs to be added
* @param data Buffer that holds the entry data
* @param len  Size of the buffer that holds the data
*
* @return HMAP_S_OK for success
* @return HMAP_E_OUTMEM for allocation issue
*/
int32_t hashmap_update_elem(uint8_t* key, void* data, size_t len)
{
	userelem *dat = NULL;
	int32_t ret = 0;

	HMAP_NULL_CHECK(key);
	HMAP_NULL_CHECK(data);
	if(len <= 0)
		return -EINVAL;

	dat = (userelem *)calloc(sizeof(userelem), sizeof(char));
	if(!dat)
		return HMAP_E_OUTMEM;
	snprintf((char*) dat->key, 20, "%s", key);
	dat->value = calloc(len, sizeof(char));
	if(!(dat->value))
	{
		free(dat);
		dat = NULL;
		return HMAP_E_OUTMEM;
	}
	memcpy(dat->value, data, len);
	ret = hashmap_put(g_map, dat->key, dat);
	//printf("ret=%d\n", ret);
	//assert(ret == HMAP_S_OK);
	return ret;
}

/**
* @brief Get the number of entries present in the hashmap
*
* @return Number of entries
*/
int32_t hashmap_length()
{
	return hashmap_size(g_map);
}

/**
* @brief Print the hashmap in the hashmapfile
*
* @param fptr pointer to the hashmapfile
*
* @return HMAP_S_OK for success
* @return HMAP_E_NOTFOUND for no entries in hashmap
*/
int32_t hashmap_print(void_ptr fptr)
{
	return hashmap_iterate(g_map, iter_elem, fptr);
}
