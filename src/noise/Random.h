/*
 * Random.h
 *
 *  Created on: 11 Aug 2019
 *      Author: christiaan
 */

#include <stdint.h>
#include <cwchar>

#ifndef NOISE_RANDOM_H_
#define NOISE_RANDOM_H_

namespace Noise {

class Random {
public:
	Random();
	Random(int64_t seed);
	~Random();
	unsigned char nextByte();
	void nextByteArray(void* array, std::size_t leng);
	int nextInt();
	unsigned int nextInt(unsigned int bound);
	int64_t nextInt64();
	double nextDouble();
	double nextDoubleInclusive();
	bool nextBool();
private:
	int64_t seed;
	uint32_t i;
};

} /* namespace _4DCom */

#endif /* NOISE_RANDOM_H_ */
