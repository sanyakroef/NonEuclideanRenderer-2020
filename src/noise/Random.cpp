/*
 * Random.cpp
 *
 *  Created on: 11 Aug 2019
 *      Author: christiaan
 */

#include "Random.h"
#include<ctime>
#include <math.h>

namespace Noise {

Random::Random() {
	seed = time(0);
	i=0;
	if (seed == 0)
		seed = 7629689182272459474;
}
Random::Random(int64_t seed) {
	this->seed = seed;
	i=0;
	if (seed == 0)
		this->seed = 7629689182272459474;
}

Random::~Random() {
	// TODO Auto-generated destructor stub
}

unsigned char Random::nextByte() {
	i++;
	double v = tan(i*seed)*seed;

	unsigned char out = 0;
	unsigned char* vData = (unsigned char*)(void*) &v;
	for (unsigned char i = 0;i<sizeof(v);i++)
		out ^= *(vData+i);

	return out;
}

void Random::nextByteArray(void* array, std::size_t leng) {
	unsigned char* ucharArray = (unsigned char*)array;
	for (std::size_t i = 0;i<leng;i++)
		ucharArray[i] = nextByte();
}

int Random::nextInt() {
	int out;
	nextByteArray(&out, sizeof(out));
	return out;
}

unsigned int Random::nextInt(unsigned int bound) {
	int lead = 31-__builtin_clz(bound);//assumes 32 bit
	unsigned int SHIFTINT = 1<<lead;
	unsigned int ANDINT = SHIFTINT-1;

	//Checks for bound is power of 2
	if (SHIFTINT==bound)
		return nextInt() & ANDINT;

	//Brute forces numbers until a "good" number is found
	while (true) {
		unsigned int out = nextInt() & ANDINT;
		if (out < bound)
			return out;
	}
}

int64_t Random::nextInt64() {
	int64_t out;
	nextByteArray(&out, sizeof(out));
	return out;
}

double Random::nextDouble() {
	return ((double)(((uint64_t)nextInt64())&0x7FFFFFFFFFFFFFFF))/((double)(0x8000000000000000));
}

double Random::nextDoubleInclusive() {
	return ((double)(uint64_t)nextInt64())/((double)0xFFFFFFFFFFFFFFFF);
}

bool Random::nextBool() {
	return nextByte()&1;
}

} /* namespace _4DCom */
