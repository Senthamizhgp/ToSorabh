#include <stdio.h>
#include "configlib.h"
#include <string.h>
#include <errno.h>
#include "ConfigurationReader.h"

/**
* @brief Fetch the WiFi configuration parameters from configuration file
*
* @param config_udata buffer to hold the configuration parameters
*
* @return 0 for success
*/
int32_t configlib_read(config_reader_t **config_udata)
{
	ConfigurationReader *configReader;
	configReader = ConfigurationReader::getInstance();
	
	if(configReader)
	{
		(*config_udata)->interval = configReader->m_WiFiAcquisitionFreq;
		memcpy((*config_udata)->device, configReader->m_WiFiDeviceID.c_str(), configReader->m_WiFiDeviceID.size() + 1);
		(*config_udata)->signal = configReader->m_WiFiRSSI;
	}
	else
		return -EFAULT;

	return 0;
}
