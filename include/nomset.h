#ifndef ORBSETS_NOMSET
#define ORBSETS_NOMSET

#include <set>
#include <utility>
#include <string>

#include <singleton.h>
#include <rational.h>
#include <orbit.h>

template<typename T>
class nomset {
public:
	std::set<orbit<T>> orbits;
	
	nomset(){}
	template<typename Q>
	nomset(nomset<Q> orig) {
		for (auto o : orig.orbits) {
			orbits.insert(orbit<Q>(o));
		}
	}
	
	bool contains(T el) {
		for (auto o: orbits) {
			if (o.isElement(el)) {
				return true;
			}
		}
		return false;
	}
	bool contains(orbit<T> orb) {
		return orbits.count(orb) != 0;
	}
	bool contains(nomset<T> subset) {
		auto setit = subset.orbits.begin();
		auto selfit = orbits.begin();
		while (setit != subset.orbits.end() && selfit != orbits.end()) {
			if (*selfit > *setit) {
				return false;
			} else if (*selfit < *setit) {
				selfit++;
			} else {
				selfit++;
				setit++;
			}
		}
		if (setit != subset.orbits.end())
			return false;
		return true;
	}
};

template<typename T>
bool operator==(nomset<T> a, nomset<T> b) {
	return a.orbits == b.orbits;
}

template<typename T>
bool operator!=(nomset<T> a, nomset<T> b) {
	return a.orbits != b.orbits;
}

template<typename T>
nomset<T> nomset_union(nomset<T> a, nomset<T> b) {
	nomset<T> c;
	auto ait = a.orbits.begin();
	auto bit = b.orbits.begin();
	while (ait != a.orbits.end() && bit != b.orbits.end()) {
		if (*ait == *bit) {
			c.orbits.insert(c.orbits.end(), *ait);
			ait++;
			bit++;
		} else if (*ait < *bit) {
			c.orbits.insert(c.orbits.end(), *ait);
			ait++;
		} else {
			c.orbits.insert(c.orbits.end(), *bit);
			bit++;
		}
	}
	while (ait != a.orbits.end()) {
		c.orbits.insert(c.orbits.end(), *ait);
		ait++;
	}
	while (bit != b.orbits.end()) {
		c.orbits.insert(c.orbits.end(), *bit);
		bit++;
	}
	return c;
}

template<typename T>
nomset<T> nomset_intersect(nomset<T> a, nomset<T> b) {
	nomset<T> c;
	auto ait = a.orbits.begin();
	auto bit = b.orbits.begin();
	while (ait != a.orbits.end() && bit != b.orbits.end()) {
		if (*ait == *bit) {
			c.orbits.insert(c.orbits.end(), *ait);
			ait++;
			bit++;
		} else if (*ait < *bit) {
			ait++;
		} else {
			bit++;
		}
	}
	return c;
}

void nomset_generate_product(std::string &cur, int nA, int nB, std::vector<std::string> &res) {
	if (nA == 0 && nB == 0) {
		res.push_back(cur);
	}
	
	if (nA != 0) {
		cur.push_back('A');
		nomset_generate_product(cur, nA-1, nB, res);
		cur.pop_back();
	}
	if (nB != 0) {
		cur.push_back('B');
		nomset_generate_product(cur, nA, nB-1, res);
		cur.pop_back();
	}
	if (nA != 0 && nB != 0) {
		cur.push_back('C');
		nomset_generate_product(cur, nA-1, nB-1, res);
		cur.pop_back();
	}
}

template<typename A, typename B>
nomset<std::pair<A,B>> nomset_product(nomset<A> a, nomset<B> b) {
	nomset<std::pair<A,B>> c;
	for (auto aorb : a.orbits) {
		for (auto borb : b.orbits) {
			std::vector<std::string> prods;
			std::string gen="";
			nomset_generate_product(gen, aorb.supportSize(), borb.supportSize(), prods);
			for (auto p : prods) {
				c.orbits.insert(c.orbits.end(), orbit<std::pair<A,B>>(aorb, borb, p));
			}
		}
	}
	return c;
}

nomset<singleton> nomset_singleton() {
	nomset<singleton> c;
	c.orbits.insert(c.orbits.end(), orbit<singleton>());
	return c;
}

nomset<rational> nomset_rationals() {
	nomset<rational> c;
	c.orbits.insert(c.orbits.end(), orbit<rational>());
	return c;
}

template<typename A, typename F>
nomset<A> nomset_filter(nomset<A> a, F f) {
	nomset<A> c;
	for (auto o : a.orbits) {
		if (f(o.getElement()))
			c.orbits.insert(c.orbits.end(), o);
	}
	return c;
}

template<typename A, typename B, typename F>
nomset<B> nomset_map(nomset<A> a, F f) {
	nomset<B> c;
	for (auto o : a.orbits) {
		c.orbits.insert(orbit<B>(f(o.getElement())));
	}
	return c;
}
#endif
