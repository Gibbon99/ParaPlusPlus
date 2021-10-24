#include "game/shipDecks.h"
#include "system/util.h"
#include "classes/paraLift.h"

//-----------------------------------------------------------------------------------------------------------------------
//
// Create a tunnel entry for a deck
void paraLift::createTunnel(int tunnelIndex, int liftIndex)
//-----------------------------------------------------------------------------------------------------------------------
{
	tunnels.insert (std::pair<int, int> (tunnelIndex, liftIndex));
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the lift linked with a tunnel
int paraLift::getLiftIndex(int tunnelIndex)
//-----------------------------------------------------------------------------------------------------------------------
{
	return tunnels.at (tunnelIndex);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the tunnel linked with a lift
int paraLift::getTunnelIndex(int liftIndex)
//-----------------------------------------------------------------------------------------------------------------------
{
	if (tunnels.size () == 1)
	{
		try
		{
			return tunnels.begin ()->first;
		}
		catch (std::out_of_range &outOfRange)
		{
			sys_shutdownWithError (sys_getString ("Error. Attempting to access beyond a single tunnel size."));
		}
	}

	for (auto tunnelItr: tunnels)
	{
		if (tunnelItr.second == liftIndex)
		{
			return tunnelItr.first;
		}
	}

	sys_shutdownWithError ("Unable to find tunnel index.");
	return -1;
}