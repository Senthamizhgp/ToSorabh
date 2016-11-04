#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __linux__
#include <time.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* timer functions */
#ifdef _WIN32

#elif __linux__
int start_periodic_timer(timer_t *id, unsigned int time_msec, void(*callback)());
#endif

#ifdef __cplusplus
}
#endif


#endif // !_COMMON_H_
