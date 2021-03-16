#ifndef PARA_PARALIFT_H
#define PARA_PARALIFT_H

#include <map>

class paraLift
{
public:

	void createTunnel(int tunnelIndex, int liftIndex);

	int getLiftIndex(int tunnelIndex);

	int getTunnelIndex(int liftIndex);

	std::map<int,int> tunnels;
private:

};

#endif //PARA_PARALIFT_H
