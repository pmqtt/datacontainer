#include "prime_stream.h"


prime_stream::prime_stream(int start ) {
	this->current_value = start;
}

prime_stream::prime_stream(const prime_stream & rhs) : stream<int>() {
	this->current_value = rhs.current_value;
}

prime_stream & prime_stream::operator=(const prime_stream & rhs) {
	if (this != &rhs) {
		this->current_value = rhs.current_value;
	}
	return *this;
}

prime_stream::prime_stream(prime_stream && rhs) {
	this->current_value = rhs.current_value;
	rhs.current_value = 2;
}

prime_stream & prime_stream::operator=(prime_stream && rhs) {
	if (this != &rhs) {
		this->current_value = rhs.current_value;
		rhs.current_value = 2;
	}
	return *this;
}

int prime_stream::next() {
	if (current_value == 2) {
        current_value = 3;
		return current_value;
	}
	bool isPrime = false;
	while (!isPrime) {
        current_value += 2;
		isPrime = true;
		for (int i = 2; i < (int)sqrt(current_value) + 1; ++i) {
			if (current_value % i == 0) {
				isPrime = false;
				break;
			}
		}
	}
	return current_value;
	
}


