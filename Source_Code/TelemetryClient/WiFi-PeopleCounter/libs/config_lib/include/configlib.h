#ifndef CONFIGLIB_H
#define CONFIGLIB_H

#include "wifilib.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t configlib_read(config_reader_t **config_udata);
 
#ifdef __cplusplus
 }
#endif

#endif
