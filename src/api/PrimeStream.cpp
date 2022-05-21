#include "PrimeStream.h"


PrimeStream::PrimeStream(int start ) {
	this->currentValue = start;
}

PrimeStream::PrimeStream(const PrimeStream & rhs) {
	this->currentValue = rhs.currentValue;
}

PrimeStream & PrimeStream::operator=(const PrimeStream & rhs) {
	if (this != &rhs) {
		this->currentValue = rhs.currentValue;
	}
	return *this;
}

PrimeStream::PrimeStream(PrimeStream && rhs) {
	this->currentValue = rhs.currentValue;
	rhs.currentValue = 2;
}

PrimeStream & PrimeStream::operator=(PrimeStream && rhs) {
	if (this != &rhs) {
		this->currentValue = rhs.currentValue;
		rhs.currentValue = 2;
	}
	return *this;
}

int PrimeStream::next() {
	if (currentValue == 2) {
		currentValue = 3;
		return currentValue;
	}
	bool isPrime = false;
	while (!isPrime) {
		currentValue += 2;
		isPrime = true;
		for (int i = 2; i < (int)sqrt(currentValue) + 1; ++i) {
			if (currentValue % i == 0) {
				isPrime = false;
				break;
			}
		}
	}
	return currentValue;
	
}


