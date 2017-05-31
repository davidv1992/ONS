#ifndef ORBSETS_ABSTRACT
#define ORBSETS_ABSTRACT

#include <set>
#include <iostream>
#include <rational.h>

class abstract {
public:
	std::set<rational> data;
	
	abstract() {}
	
	abstract(std::vector<rational> d) {
		for (auto e : d) {
			data.insert(e);
		}
	}
};

std::ostream& operator<<(std::ostream& o, abstract el) {
	o << "{";
	bool isFirst = true;
	for (auto r : el.data) {
		if (isFirst)
			isFirst =false;
		else
			o << ", ";
		o << r;
	}
	return o << "}";
}

#endif
