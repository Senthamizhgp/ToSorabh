#ifndef CHANNEL_SELECTOR_UT_H
#define CHANNEL_SELECTOR_UT_H

#include <stdint.h>
#include <sys/socket.h>
#include <linux/wireless.h>

#ifdef UT
#define SCOPE
#else
#define SCOPE static
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** Frequency flags. */
typedef enum {
        WIFI_FREQ_AUTO  = IW_FREQ_AUTO,  /**< Auto channel switching */
        WIFI_FREQ_FIXED = IW_FREQ_FIXED  /**< Fixed channel */
} wifi_freq_flag_t;

/** Supported operation modes. */
typedef enum {
        WIFI_MODE_AUTO          = IW_MODE_AUTO,         /**< Driver decides. */
        WIFI_MODE_ADHOC         = IW_MODE_ADHOC,        /**< Single cell network. */
        WIFI_MODE_MANAGED       = IW_MODE_INFRA,        /**< Multi cell network, roaming, ... */
        WIFI_MODE_MASTER        = IW_MODE_MASTER,       /**< Synchronisation master or access point. */
        WIFI_MODE_REPEAT        = IW_MODE_REPEAT,       /**< Wireless repeater, forwarder. */
        WIFI_MODE_SECOND        = IW_MODE_SECOND,       /**< Secondary master/repeater, backup. */
        WIFI_MODE_MONITOR       = IW_MODE_MONITOR       /**< Passive monitor, listen only. */
} wifi_mode_t;

SCOPE int32_t wifi_get_channel(const char *ifname, int32_t* channel);
SCOPE int32_t wifi_freq_to_chan(const char *ifname, double freq, int32_t *chan);
SCOPE int32_t wifi_get_freq(const char *ifname, double *freq, wifi_freq_flag_t *flag);
SCOPE int32_t wifi_get_mode(const char *ifname, wifi_mode_t *mode);

SCOPE int32_t wifi_set_channel(const char *ifname, uint32_t channel);
SCOPE int32_t wifi_get_dev_mac_addr(const char *ifname, uint8_t* mac_address);
SCOPE int32_t wifi_set_freq(const char *ifname, double freq, wifi_freq_flag_t flag);
SCOPE int32_t wifi_channel_to_freq(const char *ifname, uint32_t chan, double *freq);
SCOPE int32_t wifi_set_mode(const char *ifname, wifi_mode_t mode);
SCOPE int32_t wifi_set_state(const char *ifname, long flags);
SCOPE int32_t wifi_get_state(const char *ifname, long *flags);
#ifdef __cplusplus
}
#endif

#endif
