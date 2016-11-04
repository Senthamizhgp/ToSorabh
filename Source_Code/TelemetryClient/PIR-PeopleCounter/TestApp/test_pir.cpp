#include <stdio.h>
#include <string.h>

#include "SPInterface.h"
#include "pir.h"
#include "ConfigurationReader.h"

int main(int argc, char **argv)
{
	int i = 0;
	int dev_id1, dev_id2 = -1; 
	char ch;
	SPDeviceList dev_list;

	/* open h/w with suset pass */
	if (SP_SUCCESS != spSystemOpen())
	{
		printf("Failure occured on spSystemOpen\n");

		return -1;
	}

	/* get device details from sunset pass */
	if (SP_SUCCESS != spGetDevicesDetail(&dev_list))
	{
		printf("Failure occured on spGetDevicesDetail\n");
		spSystemClose();

		return -1;
	}

	/* print available devices */
	printf("\nPrinting device details\n\n");
	for (i = 0; i < dev_list.nDeviceListSize; i++)
	{
		printf("m_nDeviceID:\t\t %d\n", dev_list.structDeviceList[i].m_nDeviceID);
		printf("m_nID:\t\t\t %d\n", dev_list.structDeviceList[i].m_nID);
		printf("m_eDeviceType:\t\t %d\n", dev_list.structDeviceList[i].m_eDeviceType);
		printf("m_eDeviceSubType:\t %d\n", dev_list.structDeviceList[i].m_eDeviceSubType);
		printf("m_bDeviceIsPowerdOn:\t %d\n", dev_list.structDeviceList[i].m_bDeviceIsPowerdOn);
		printf("m_sName:\t\t %s\n", dev_list.structDeviceList[i].m_sName);
		printf("m_sDeviceDescription:\t %s\n\n", dev_list.structDeviceList[i].m_sDeviceDescription);

		if (strcmp(dev_list.structDeviceList[i].m_sDeviceDescription, "GPI SENSOR I") == 0)
			dev_id1 = dev_list.structDeviceList[i].m_nID;

		if (strcmp(dev_list.structDeviceList[i].m_sDeviceDescription, "GPI SENSOR II") == 0)
			dev_id2 = dev_list.structDeviceList[i].m_nID;
	}


	if (dev_id1 == -1 || dev_id2 == -1)
	{
		printf("Invalid device ID[dev_id1 -%d, dev_id2 - %d]\n", dev_id1, dev_id2);
		spSystemClose();

		return -1;
	}

	ConfigurationReader *configReader;
	configReader = ConfigurationReader::getInstance();
	init_PIR(dev_id1, dev_id2);
	
	printf("Running PIR module. Press 'q' or 'Q' to exit\n");

	while (1)
	{
		ch = getchar();

		if (ch == 'q' || ch == 'Q')
		{
			deinit_PIR();
			spSystemClose();
			
			break;
		}
	}

	return 0;
}
