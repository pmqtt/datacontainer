#include "hash_stream.h"

#if defined(__GNUC__) || defined(__GNUG__)
	int hash_stream::PRIME_ARRAY[] = {
		1009, 1013, 4289, 4297, 6863, 6869, 7841, 7853, 7867, 7873,
		8009, 8117, 9013, 9029, 9967, 9973, 25801, 25819, 50767, 50773,
		109619, 109621, 175837, 175843, 296771, 296773, 610651, 610661,
		999961, 999979
};
#endif

hash_stream::hash_stream(): stream(hash_stream::PRIME_ARRAY[29]){
    current_index = 1;
}


hash_stream::~hash_stream(){
}

hash_stream::hash_stream(const hash_stream & rhs) : stream<int>() {
    current_index = rhs.current_index;
	stream = rhs.stream;

}
hash_stream &hash_stream::operator=(const hash_stream & rhs) {
	if (this != &rhs) {
        current_index = rhs.current_index;
		stream = rhs.stream;
	}
	return *this;
}
hash_stream::hash_stream(hash_stream && rhs) {
    current_index = rhs.current_index;
	stream = rhs.stream;
	rhs.current_index = -1;
}
hash_stream & hash_stream::operator=(hash_stream && rhs) {
	if (this != &rhs) {
        current_index = rhs.current_index;
		stream = rhs.stream;
		rhs.current_index = -1;
	}
	return *this;
}

int hash_stream::next(){
	if (current_index + 1 < 30) {
		current_index++;
		return hash_stream::PRIME_ARRAY[current_index];
	}
	return stream.next();
}


