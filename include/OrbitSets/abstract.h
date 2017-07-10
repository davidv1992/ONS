#ifndef ORBSETS_ABSTRACT
#define ORBSETS_ABSTRACT

#include <set>
#include <vector>
#include <iostream>
#include <OrbitSets/rational.h>

namespace OrbitSets {

	class abstract {
	public:
		typedef std::set<rational>::iterator iterator;

		std::set<rational> data;
	
		abstract() {}
	
		abstract(std::vector<rational> d) {
			for (auto e : d) {
				data.insert(e);
			}
		}
	
		iterator begin() {
			return data.begin();
		}
		iterator end() {
			return data.end();
		}
	
		size_t size() {
			return data.size();
		}
	};

	inline bool operator==(abstract a, abstract b) {
		return a.data == b.data;
	}
	inline bool operator!=(abstract a, abstract b) {
		return a.data != b.data;
	}

}

inline std::ostream& operator<<(std::ostream& o, OrbitSets::abstract el) {
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
