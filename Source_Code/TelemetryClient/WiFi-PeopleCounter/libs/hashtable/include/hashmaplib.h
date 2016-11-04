#ifndef HASHMAPLIB_H
#define HASHMAPLIB_H

#include <stdint.h>
#include <errno.h>

#define UNUSED(expr) do { (void)(expr); } while (0)

#ifdef __cplusplus
extern "C" {
#endif

typedef void* void_ptr;
typedef void_ptr hmap_t;

int32_t hashmap_initialize(void);
void    hashmap_uninitialize(void);
int32_t hashmap_update_elem(uint8_t* mac, void* data, size_t len);
int32_t hashmap_remove_elem(uint8_t* mac, void* data, size_t len);
int32_t hashmap_length(void);
int32_t hashmap_print(void_ptr fptr);

#ifdef __cplusplus
}
#endif

#endif // HASHMAPLIB_H
