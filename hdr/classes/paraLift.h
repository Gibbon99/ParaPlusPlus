#pragma once

#include <map>

class paraLift
{
public:

	void createTunnel(int tunnelIndex, int liftIndex);

	int getLiftIndex(int tunnelIndex);

	int getTunnelIndex(int liftIndex);

	std::map<int, int> tunnels;
private:

};