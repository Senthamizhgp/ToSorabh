
/******************************************************************************
*
* INTEL CONFIDENTIAL
* Copyright 2016 Intel Corporation All Rights Reserved.
*
* The source code contained or described herein and all documents related to the
* source code ("Material") are  owned by Intel Corporation or its suppliers or
* licensors. Title to the Material remains with Intel Corporation or its suppliers
* and licensors. The Material contains trade secrets and proprietary and
* confidential information of Intel or its suppliers and licensors. The Material
* is protected by worldwide  copyright and trade secret laws and treaty provisions.
* No part of the Material may be used, copied, reproduced, modified, published,
* uploaded, posted, transmitted, distributed, or disclosed in any way without
* Intel's prior express written permission.
*
* No license under any patent, copyright, trade secret or other intellectual
* property right is granted to or conferred upon you by disclosure or delivery of
* the Materials, either expressly, by implication,  inducement, estoppel or
* otherwise. Any license under such intellectual property rights must be express
* and approved by Intel in writing.
*
******************************************************************************/

/**
 *
 *	@file		wifi_ppl_counter.c
 *
 *	@brief		This file contains definition of all APIs which will
 *	                be exported by the WiFi People Counter module.
 *			Its the main entry point for the library and implements
 *			the alogrithm for passive WiFi Monitoring of the smart
 *			devices.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include "configlib.h"
#include "hashmaplib.h"
#include "pcap_user.h"
#include "pktparser.h"
#include "channel_selector.h"
#include "wifilib.h"
#include "dbconnector.h"
#include "wifi_ppl_counter.h"

#define MAX_STR 512

/* Static Declarations */

static int stop_counter = 0;

static wifi_ppl_counter_t*  g_wifi_ppl_counter_inst;

static int32_t init_library(wifi_ppl_counter_t *wifi_ppl_counter_inst);
static void free_library(wifi_ppl_counter_t *wifi_ppl_counter_inst);

static int32_t init_hashmap_module(hashmap_user_data_t **hashmap_udata);
static void free_hashmap_module(hashmap_user_data_t **hashmap_udata);

static int32_t init_configdata(config_reader_t **config_udata);
static void free_configdata(config_reader_t **config_udata);

static int32_t init_userdata(wifi_user_record_data_t **wifi_udata);
static void free_userdata(wifi_user_record_data_t **wifi_udata);

static uint8_t *trimwhitespace(uint8_t *str)
{
	uint8_t *end;

	// Trim leading space
	while(isspace(*str)) str++;

	if(*str == 0)  // All spaces?
		return str;

	// Trim trailing space
	end = str + strlen((char*)str) - 1;
	while(end > str && isspace(*end)) end--;

	// Write new null terminator
	*(end+1) = 0;

	return str;
}

/**
 * @brief Callback function to print the in memory data strcuture
 *
 * @param data pointer to the node in the in memory data strcuture
 *
 */
static void wifi_print_user_data(void* data)
{
	wifi_user_record_data_t *wifi_udata = (wifi_user_record_data_t *) data;
	FILE* h_fptr = g_wifi_ppl_counter_inst->hashmap_udata->h_fptr;
	fprintf(h_fptr, "key=%s |", trimwhitespace(wifi_udata->mac));
	fprintf(h_fptr, "cycle=%lld |\n", wifi_udata->cycle);
}

/**
 * @brief Remove the hashmap record and add data
 * the said key to the database
 *
 * @param key record to be removed from hashmap and added to the database
 *
 * @return 0 if success
 */
static int32_t push_hashmap_record_to_db(uint8_t* key)
{
	int32_t status = 0;
	wifi_user_record_data_t wifi_udata;

	memset(&wifi_udata, 0, sizeof(wifi_user_record_data_t));

	status =  hashmap_remove_elem(trimwhitespace(key), &wifi_udata, sizeof(wifi_user_record_data_t));
	if(status != 0)
	{
		printf("del_mac_from_hashtable failed status =%d\n", status);
		return status;
	}

	printf("push to db for %s called\n", key);

	status = add_record_database(&wifi_udata);
	if(status != 0)
	{
		printf("add_record_database failed status =%d\n", status);
		return status;
	}

	return 0;
}


/**
 * @brief Filter out the entries which does not match the current cycle
 * and push them to the database
 * Basically it moves the un-updated records to the database
 *
 * @param fp hashmap file
 * @param cycle_str current cycle
 *
 */
static void filter_hashmap_records_per_cycle(FILE* fp, char *cycle_str)
{
	unsigned long long cycle;
	int32_t line_num = 1;
	int32_t find_result = 0;
	char temp[MAX_STR];
	uint8_t key[30] = {0};

	rewind(fp);
	while(fgets(temp, MAX_STR, fp) != NULL)
	{
		if(temp[0] == 'k')
		{
			if((strstr(temp, cycle_str)) == NULL)
			{
				sscanf(temp, "key=%s |cycle=%lld |", key, &cycle);
				//TODO
				//Not treating it serious to add record to DB
				push_hashmap_record_to_db(key);
				find_result++;
				memset(key, 0, sizeof(key));
			}
		}
		memset(temp, 0, sizeof(temp));
		line_num++;
	}
}

/**
 * @brief Add data to the in memory data structure
 * Remove if existing entry exists, update the time stamp based on old entry
 * and update the entry
 * If no entry exists add a new entry
 *
 * @param ptr data pointer for the record
 *
 * @return 0 if success
 */
static int32_t wifi_add_record_to_hashmap(void* ptr)
{
	wifi_user_record_data_t wifi_user_data_test;
	wifi_user_record_data_t *wifi_user_data_out = (wifi_user_record_data_t *) ptr;
	uint8_t *mac = NULL;
	int32_t status = 0;

	if(wifi_user_data_out != NULL)
	{
		mac = trimwhitespace(wifi_user_data_out->mac);
		memset(&wifi_user_data_test, 0, sizeof(wifi_user_record_data_t));

		status = hashmap_remove_elem(mac, &wifi_user_data_test, sizeof(wifi_user_record_data_t));

		if(status == 0)
		{
			wifi_user_data_out->t_start = wifi_user_data_test.t_start;
		}

		status =  hashmap_update_elem(mac, wifi_user_data_out, sizeof(wifi_user_record_data_t));

		if(status != 0)
		{
			printf("hashmap_update_elem failed status = %d\n", status);
		}
		//TODO
		//Ignore failure to add into hashmap
	}

	return status;
}

/**
 * @brief Extract the in memory data structure to a file
 */
static void wifi_extract_hashmap(void)
{
	hashmap_print(wifi_print_user_data);
}

/**
 * @brief Scan the in memory data structure hashmap inorder to filter out
 * records which were out-dated with respect to the current scanning cycle
 *
 * @param wifi_ppl_counter_inst pointer to global library instance
 */
static void scan_update_hashmap_file_per_cycle(wifi_ppl_counter_t *wifi_ppl_counter_inst)
{
	unsigned long long cycle;
	char cycle_str[MAX_STR];
	FILE* h_fptr = NULL;

	if(wifi_ppl_counter_inst)
	{
		memset(cycle_str, '\0', sizeof(cycle_str));

		cycle = wifi_ppl_counter_inst->wifi_udata->cycle;
		h_fptr = wifi_ppl_counter_inst->hashmap_udata->h_fptr;

		if(cycle != 0)
			snprintf(cycle_str, MAX_STR, "cycle=%lld ", cycle);
		else
			snprintf(cycle_str, MAX_STR, "cycle=");

		fprintf(h_fptr, "=====HashMap=====\n");
		wifi_extract_hashmap();
		filter_hashmap_records_per_cycle(h_fptr, cycle_str);
	}
}

/**
 * @brief Push all the in memory data structure records
 * to database.
 * It would be used when the people counter is stopped
 * and the collected in memory data needs to be saved to the
 * database
 *
 * @param wifi_ppl_counter_inst pointer to global library instance
 */
static void push_all_hashmap_data_to_db(wifi_ppl_counter_t * wifi_ppl_counter_inst)
{
	wifi_ppl_counter_inst->wifi_udata->cycle = 0; /* set cycle to 0 to push all hashmap records */
	scan_update_hashmap_file_per_cycle(wifi_ppl_counter_inst);
}

/**
 * @brief Initialize the configuration data library object that holds
 * configuration from the config file and get the configuration
 * data from the configuration file.
 *
 * @param config_udata configlib module buffer that needs to be allocated
 *
 * @return 0 for success
 */
int32_t init_configdata(config_reader_t **config_udata)
{
	int32_t status = 0;
	*config_udata = (config_reader_t *) calloc (1, sizeof(config_reader_t));
	if(!(*config_udata))
	{
		printf("init_configdata: Allocation issue\n");
		return -ENOMEM;
	}

	status = configlib_read(config_udata);
	if(status != 0)
	{
		printf("configlib_read failed\n");
		free_configdata(config_udata);
		return status;
	}
	printf("Interval=%d\n", (*config_udata)->interval);
	printf("Device=%s\n", (*config_udata)->device);
	printf("RSSI filter=%d\n", (*config_udata)->signal);

	return 0;
}

/**
 * @brief free configuration data library object
 *
 * @param config_udata Allocated buffer for config lib module
 *
 */
void free_configdata(config_reader_t **config_udata)
{
	if(*config_udata)
	{
		free(*config_udata);
		*config_udata = NULL;
	}
}

/**
 * @brief Initialize the in memory data stucture module (hashmap)
 * It allocated the hashmap buffer and initializes it.
 * It also sets the call back function that adds data to the
 * hashmap in memory data stucture
 *
 * @param hashmap_udata  buffer that needs to be allocated
 *
 * @return 0 for success
 */
int32_t init_hashmap_module(hashmap_user_data_t **hashmap_udata)
{
	int32_t status = 0;

	do
	{
		*hashmap_udata = (hashmap_user_data_t *) calloc (1, sizeof(hashmap_user_data_t));
		if(!(*hashmap_udata))
		{
			printf("init_hashmap_module: Allocation issue\n");
			status = -ENOMEM;
			break;
		}

		if(((*hashmap_udata)->h_fptr = fopen("/tmp/hashfile", "w+")) == NULL)
		{
			status = -EIO;
			break;
		}

		if((status = hashmap_initialize()) != 0)
		{
			printf("hashmap_initialize failed \n");
			break;
		}

		(*hashmap_udata)->add_hashmap = wifi_add_record_to_hashmap;

	} while(0);

	if(status != 0)
		free_hashmap_module(hashmap_udata);

	return status;
}

/**
 * @brief Free the in memory data stucture module (hashmap)
 *
 * @param hashmap_udata buffer that needs to be deallocated
 *
 */
void free_hashmap_module(hashmap_user_data_t **hashmap_udata)
{
	if(*hashmap_udata)
	{
		if((*hashmap_udata)->add_hashmap)
			(*hashmap_udata)->add_hashmap = NULL;
		hashmap_uninitialize();
		if((*hashmap_udata)->h_fptr)
			fclose((*hashmap_udata)->h_fptr);
		free((*hashmap_udata));
		*hashmap_udata = NULL;
	}
}

/**
* @brief Initialize the WiFi People Counter user data record
* which is used finally pushed to the database
*
* @param wifi_udata buffer that needs to be allocated
*
* @return 0 for success
*/
int32_t init_userdata(wifi_user_record_data_t **wifi_udata)
{
	*wifi_udata = (wifi_user_record_data_t *)calloc(1, sizeof(wifi_user_record_data_t));
	if(!(*wifi_udata))
	{
		printf("init_userdata: Allocation issue\n");
		return -ENOMEM;
	}
	return 0;
}

/**
* @brief Free the WiFi People Counter user data record buffer
*
* @param wifi_udata buffer that needs to be freed
*
*/
void free_userdata(wifi_user_record_data_t **wifi_udata)
{
	if(*wifi_udata)
	{
		free((*wifi_udata));
		*wifi_udata = NULL;
	}
}

/**
 * @brief Initialize the library data objects
 *
 * @param wifi_ppl_counter_inst pointer to global library instance
 *
 * @return 0 for success
 */
int32_t init_library(wifi_ppl_counter_t *wifi_ppl_counter_inst)
{
	int32_t status;

	if(!wifi_ppl_counter_inst)
		return -EINVAL;
	do
	{
		memset(wifi_ppl_counter_inst, 0, sizeof(wifi_ppl_counter_t));

		status = init_configdata(&wifi_ppl_counter_inst->config_udata);
		if(status != 0)
			break;
		status = init_hashmap_module(&wifi_ppl_counter_inst->hashmap_udata);
		if(status != 0)
			break;

		status = init_userdata(&wifi_ppl_counter_inst->wifi_udata);
		if(status != 0)
			break;

		status = init_pkt_capture_module(wifi_ppl_counter_inst->config_udata->device, wifi_ppl_counter_inst->config_udata->interval);
		if(status != 0)
			break;

		status = init_pkt_parser_module(wifi_ppl_counter_inst);
		if(status != 0)
			break;

		status = init_database_connector();
		if(status != 0)
			break;
	} while(0);

	return status;
}

/**
 * @brief Free the library data objects
 *
 * @param wifi_ppl_counter_inst pointer to global library instance
 */
void free_library(wifi_ppl_counter_t *wifi_ppl_counter_inst)
{
	free_database_connector();
	free_pkt_parser_module();
	free_pkt_capture_module();
	free_userdata(&wifi_ppl_counter_inst->wifi_udata);
	free_hashmap_module(&wifi_ppl_counter_inst->hashmap_udata);
	free_configdata(&wifi_ppl_counter_inst->config_udata);
}

/**
 * @brief Start the WiFi signal based people counter
 * It initializes the people counter library data objects
 * It gets the NUC WiFi MAC address
 * It put the WiFi adapter into monitor mode
 * Runs a loop to run packet capturing over different channels
 * across multiple cycles
 *
 * @return 0 for success
 */
int32_t start_peoplecounter(void)
{
	wifi_ppl_counter_t wifi_ppl_counter_inst;
	int32_t status;
	unsigned long long g_cycle = 1;

	g_wifi_ppl_counter_inst = &wifi_ppl_counter_inst;

	status = init_library(&wifi_ppl_counter_inst);
	if(status != 0)
		goto freelib;

	status = wifi_rfkill_unblock();
	if(status != 0)
		goto freelib;

	status = wifi_get_nuc_mac_addr(wifi_ppl_counter_inst.config_udata->device, wifi_ppl_counter_inst.wifi_udata->ap_mac);
	if(status != 0)
		goto freelib;

	status = wifi_set_monitor_mode(wifi_ppl_counter_inst.config_udata->device);
	if(status != 0)
		goto freelib;

	while(stop_counter != 1)
	{
		wifi_ppl_counter_inst.wifi_udata->cycle = g_cycle;

		while(channel_switcher(wifi_ppl_counter_inst.config_udata->device) != 1)
		{
			status =  wifi_pcap_setup_function();
			if(status != 0)
			{
				//printf("wifi_pcap_setup_function failed");
				break;
			}
			wifi_start_pcap_capture_function();
		}
		if(g_cycle > 1)
		{
			scan_update_hashmap_file_per_cycle(&wifi_ppl_counter_inst);
			system("> /tmp/hashfile");
		}
		g_cycle++;
	}
	push_all_hashmap_data_to_db(&wifi_ppl_counter_inst);
freelib:
	free_library(&wifi_ppl_counter_inst);
	return status;
}

/**
 * @brief Stop the WiFi signal based people counter
 *
 */
void stop_peoplecounter(void)
{
	printf("Stopping people counter\n");
	stop_counter = 1;
}
