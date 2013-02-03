#ifndef RANGE_H
#define RANGE_H

#include "Random.hpp"

template<class T> class Range {
public:
	Range(){}
	Range(T min, T max) : mMin(min), mMax(max) {}

	~Range(){}

	T GetRandom(){
		return Random(mMin, mMax);
	}
	
	T GetRange(){
		return mMax - mMin;
	}

	void AddValue(T value){
		if(value < mMin) mMin = value;
		if(value > mMax) mMax = value;
	}

	T mMin;
	T mMax;
};

#endif