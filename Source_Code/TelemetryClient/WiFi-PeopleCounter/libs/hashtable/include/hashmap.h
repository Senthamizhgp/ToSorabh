#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define HMAP_E_INVAL    (-6)     /**< Map is invalid */
#define HMAP_E_KEYUSED  (-5)     /**< Key already existed */
#define HMAP_E_OUTMEM   (-4)     /**< Out of Memory */
#define HMAP_E_NOTFOUND (-3)     /**< No such element */
#define HMAP_E_OVERFLOW (-2)     /**< Hashmap is full */
#define HMAP_E_FAIL     (-1)     /**< Hashmap api fail */
#define HMAP_S_OK       (0)      /**< Success */

/**
* void_ptr is a pointer. This allows you to put arbitrary structures in the hashmap.
*/
typedef void* void_ptr;

/**
 * hmap_t is a pointer to an internally maintained data structure.
 * Clients of this package do not need to know how hashmaps are
 * represented. They see and manipulate only hmap_t's.
 */
typedef void_ptr hmap_t;

/**
 * hmap_callback_func is a pointer to a function that can take two void_ptr arguments
 * and return an integer. Returns status code..
 */
typedef int32_t (*hmap_callback_func)(void_ptr, void_ptr); /**< Hashmap calback */

/**
 * A structure prototype for map element type
 */
typedef struct _hmap_pair_t {
	uint8_t     *key; /**< Key of the entry in hashmap */
	void_ptr  data;   /**< Data of the entry in hashmap associated to particular key */
} hmap_pair_t;

hmap_t hashmap_create(void);
int32_t hashmap_iterate(hmap_t in, hmap_callback_func fnIterValue, void_ptr arg);
int32_t hashmap_put(hmap_t in, uint8_t* key, void_ptr elem);
int32_t hashmap_get(hmap_t in, const uint8_t* key, void_ptr *elem);
int32_t hashmap_remove(hmap_t in, uint8_t* key, void_ptr *outValue);
void hashmap_destroy(hmap_t in, hmap_callback_func fnFreeValue, void_ptr arg);
int32_t hashmap_size(hmap_t in);

#if defined(__cplusplus)
}
#endif

#endif /* _HASHMAP_H_INCLUDED */
