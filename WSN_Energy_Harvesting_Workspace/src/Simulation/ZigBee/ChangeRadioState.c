/************************************************************************************
* Copyright (C) 2013                                                               *
* TETCOS, Bangalore. India                                                         *
*                                                                                  *
* Tetcos owns the intellectual property rights in the Product and its content.     *
* The copying, redistribution, reselling or publication of any or all of the       *
* Product or its content without express prior written consent of Tetcos is        *
* prohibited. Ownership and / or any other right relating to the software and all *
* intellectual property rights therein shall remain at all times with Tetcos.      *
*                                                                                  *
* Author:    Shashi Kant Suman                                                       *
*                                                                                  *
* ---------------------------------------------------------------------------------*/
# include "main.h"
# include "802_15_4.h"
#include "../BatteryModel/BatteryModel.h"

/** 
	The Purpose of this function is to change the transceiver radio states  
*/
bool fn_NetSim_Zigbee_ChangeRadioState(NETSIM_ID nDeviceId, PHY_TX_STATUS nOldState, PHY_TX_STATUS nNewState)
{
	ptrIEEE802_15_4_PHY_VAR phy = WSN_PHY(nDeviceId);
	ptrBATTERY battery = phy->battery;
	bool isChange = true;
	if (battery)
	{
		isChange = battery_set_mode(battery, nNewState, pstruEventDetails->dEventTime);
	}

	if(isChange)
	{
		phy->nOldState = nOldState; 
		phy->nRadioState = nNewState; 
	}
	else
	{
		phy->nRadioState = RX_OFF;
		WSN_MAC(nDeviceId)->nNodeStatus = OFF;
	}
	return isChange;
}

#define EH_FRACTION 0.1
// EH_FRACTION is the fraction of the received signal energy that can be
// captured and harvested by the sensor. 
int calculate_eh(NETSIM_ID dev1, NETSIM_ID dev2)
{ 
	double rx_pwr = GET_RX_POWER_mw(dev1, dev2, pstruEventDetails->dEventTime); 
	double eh_energy= EH_FRACTION * rx_pwr; 
	ptrBATTERY battery = WSN_PHY(dev2)->battery; 
	if(battery) 		
		battery_energy_harvesting(battery, eh_energy);
} 
