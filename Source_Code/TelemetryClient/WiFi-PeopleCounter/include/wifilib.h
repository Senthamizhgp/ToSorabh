#ifndef WIFILIB_H
#define WIFILIB_H

#include <time.h>
#include <stdint.h>
#include <pcap.h>

/**
 * A structure to represent WiFi People Counter user data record
 */
typedef struct wifi_user_record_data
{
	int32_t channel;           /**< WiFi channel */
	int32_t signal;	           /**< Signal Strength RSSI */
	uint8_t ap_mac[20];        /**< MAC address of the NUC */
	uint8_t mac[20];           /**< MAC address of the WiFi packet */
	time_t t_start;            /**< Start time a particular MAC address first seen */
	time_t t_end;              /**< Time a particular MAC address last seen */
	unsigned long long cycle;  /**< Scanning cycle */
} wifi_user_record_data_t;

typedef int32_t (*add_hashmap_callback_func)(void *);

/**
 * A structure to represent in memory data stucture hashmap
 */
typedef struct hashmap_user_data
{
	FILE* h_fptr;                          /**< Hashmap file */
	add_hashmap_callback_func add_hashmap; /**< Call back function to add record to hashmap */
} hashmap_user_data_t;

/**
 * A structure to represent configuration for WiFi people counter
 */
typedef struct config_reader
{
	uint32_t interval; /**< WiFi scanning interval */
	char device[30];   /**< WiFi scanning device */
	int32_t signal;    /**< WiFi RSSI filter */
} config_reader_t;

/**
 * A structure to represent instance WiFi People Counter
 */
typedef struct wifi_ppl_counter
{
	config_reader_t         *config_udata;  /**< Configuration Structure */
	hashmap_user_data_t     *hashmap_udata; /**< In Memory Datastructure */
	wifi_user_record_data_t *wifi_udata;    /**< WiFi User record Datastructure  */
} wifi_ppl_counter_t;

#endif
