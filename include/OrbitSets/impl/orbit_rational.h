#ifndef ORBSET_ORBIT_RATIONAL
#define ORBSET_ORBIT_RATIONAL

#include <cassert>

template<typename T>
class nomset;

template<>
class orbit<rational> {
public:
	orbit() {}
	int supportSize() const {
		return 1;
	}
	rational getElementFromSeq(std::vector<rational> seq) const {
		assert(seq.size() >= 1);
		return seq[0];
	}
	std::vector<rational> getSeqFromElement(rational a) const {
		return std::vector<rational>(1,a);
	}
	orbit(rational a) {
		(void) a;
	}
	rational getElement() const {
		return 1;
	}
	bool isElement(rational a) const {
		(void) a;
		return true;
	}
	
	friend nomset<rational> nomset_rationals();
};

inline bool operator==(orbit<rational> a, orbit<rational> b) {
	(void) a;
	(void) b;
	return true;
}
inline bool operator!=(orbit<rational> a, orbit<rational> b) {
	(void) a;
	(void) b;
	return false;
}
inline bool operator<(orbit<rational> a, orbit<rational> b) {
	(void) a;
	(void) b;
	return false;
}
inline bool operator>(orbit<rational> a, orbit<rational> b) {
	(void) a;
	(void) b;
	return false;
}

#endif
