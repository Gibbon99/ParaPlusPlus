#ifndef PARA_PARARANDOM_H
#define PARA_PARARANDOM_H

#include <cstddef>
#include <random>
#include <chrono>

class paraRandom
{
public:

	paraRandom ()
	{
		rng.seed (std::chrono::high_resolution_clock::now ().time_since_epoch ().count ());
	}

	std::mt19937 rng;

	int get (int min, int max)
	{
		std::uniform_int_distribution<std::mt19937::result_type> dist (min, max);
		return dist (rng);
	}

private:
};

#endif //PARA_PARARANDOM_H
