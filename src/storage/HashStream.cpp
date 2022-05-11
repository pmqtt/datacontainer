#include "HashStream.h"

#if defined(__GNUC__) || defined(__GNUG__)
	int HashStream::PRIME_ARRAY[] = {
		1009, 1013, 4289, 4297, 6863, 6869, 7841, 7853, 7867, 7873,
		8009, 8117, 9013, 9029, 9967, 9973, 25801, 25819, 50767, 50773,
		109619, 109621, 175837, 175843, 296771, 296773, 610651, 610661,
		999961, 999979
};
#endif

HashStream::HashStream():stream(HashStream::PRIME_ARRAY[29]){
	currentIndex = 1;
}


HashStream::~HashStream(){
}

HashStream::HashStream(const HashStream & rhs) {
	currentIndex = rhs.currentIndex;
	stream = rhs.stream;

}
HashStream &HashStream::operator=(const HashStream & rhs) {
	if (this != &rhs) {
		currentIndex = rhs.currentIndex;
		stream = rhs.stream;
	}
	return *this;
}
HashStream::HashStream(HashStream && rhs) {
	currentIndex = rhs.currentIndex;
	stream = rhs.stream;
	rhs.currentIndex = -1;
}
HashStream & HashStream::operator=(HashStream && rhs) {
	if (this != &rhs) {
		currentIndex = rhs.currentIndex;
		stream = rhs.stream;
		rhs.currentIndex = -1;
	}
	return *this;
}

int HashStream::next(){
	if (currentIndex + 1 < 30) {
		currentIndex++;
		return HashStream::PRIME_ARRAY[currentIndex];
	}
	return stream.next();
}


