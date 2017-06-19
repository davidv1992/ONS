#ifndef ORBSETS_ORBIT_ABSTRACT
#define ORBSETS_ORBIT_ABSTRACT

#include <abstract.h>

template<typename T>
class nomset;

template<>
class orbit<abstract> {
private:
	size_t support;
public:
	orbit() {}
	int supportSize() const {
		return support;
	}
	abstract getElementFromSeq(std::vector<rational> seq) const {
		return abstract(seq);
	}
	std::vector<rational> getSeqFromElement(abstract el) const {
		std::vector<rational> seq;
		for (auto r : el.data) {
			seq.push_back(r);
		}
		return seq;
	}
	orbit(abstract el) {
		support = el.data.size();
	}
	orbit(size_t i) {
		support = i;
	}
	abstract getElement() const {
		abstract el;
		for (size_t i=1; i<=support; i++) {
			el.data.insert(i);
		}
		return el;
	}
	bool isElement(abstract el) const {
		return el.data.size() == support;
	}
	
	friend bool operator==(orbit<abstract> a, orbit<abstract> b);
	friend bool operator!=(orbit<abstract> a, orbit<abstract> b);
	friend bool operator< (orbit<abstract> a, orbit<abstract> b);
	friend bool operator> (orbit<abstract> a, orbit<abstract> b);
};

bool operator==(orbit<abstract> a, orbit<abstract> b) {
	return a.support == b.support;
}
bool operator!=(orbit<abstract> a, orbit<abstract> b) {
	return a.support != b.support;
}
bool operator< (orbit<abstract> a, orbit<abstract> b) {
	return a.support < b.support;
}
bool operator> (orbit<abstract> a, orbit<abstract> b) {
	return a.support > b.support;
}

#endif
