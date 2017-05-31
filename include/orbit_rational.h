#ifndef ORBSET_ORBIT_RATIONAL
#define ORBSET_ORBIT_RATIONAL

#include <cassert>

template<typename T>
class nomset;

template<>
class orbit<rational> {
private:
	orbit() {}
public:
	int supportSize() {
		return 1;
	}
	rational getElementFromSeq(std::vector<rational> seq) {
		assert(seq.size() >= 1);
		return seq[0];
	}
	std::vector<rational> getSeqFromElement(rational a) {
		return std::vector<rational>(1,a);
	}
	orbit(rational a) {
		(void) a;
	}
	rational getElement() {
		return 1;
	}
	bool isElement(rational a) {
		(void) a;
		return true;
	}
	
	friend nomset<rational> nomset_rationals();
};

bool operator==(orbit<rational> a, orbit<rational> b) {
	(void) a;
	(void) b;
	return true;
}
bool operator!=(orbit<rational> a, orbit<rational> b) {
	(void) a;
	(void) b;
	return false;
}
bool operator<(orbit<rational> a, orbit<rational> b) {
	(void) a;
	(void) b;
	return false;
}
bool operator>(orbit<rational> a, orbit<rational> b) {
	(void) a;
	(void) b;
	return false;
}

#endif
