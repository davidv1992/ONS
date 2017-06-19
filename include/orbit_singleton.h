#ifndef ORBSETS_ORBIT_SINGLETON
#define ORBSETS_ORBIT_SINGLETON

#include <singleton.h>

template<typename T>
class nomset;

template<>
class orbit<singleton> {
private:
	orbit() {}
public:
	int supportSize() const {
		return 0;
	}
	singleton getElementFromSeq(std::vector<rational> seq) const {
		(void)seq;
		return singleton();
	}
	std::vector<rational> getSeqFromElement(singleton s) const {
		(void) s;
		return std::vector<rational>(0);
	}
	orbit(singleton s) {
		(void) s;
	}
	singleton getElement() const {
		return singleton();
	}
	bool isElement(singleton s) const {
		(void) s;
		return true;
	}

	friend nomset<singleton> nomset_singleton();

};

bool operator==(orbit<singleton> a, orbit<singleton> b) {
	(void) a;
	(void) b;
	return true;
}
bool operator!=(orbit<singleton> a, orbit<singleton> b) {
	(void)a;
	(void)b;
	return false;
}
bool operator<(orbit<singleton> a, orbit<singleton> b) {
	(void) a;
	(void) b;
	return false;
}
bool operator>(orbit<singleton> a, orbit<singleton> b) {
	(void) a;
	(void) b;
	return false;
}

#endif
