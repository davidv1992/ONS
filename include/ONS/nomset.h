#ifndef ONS_NOMSET
#define ONS_NOMSET

#include <set>
#include <utility>
#include <string>

#include <ONS/singleton.h>
#include <ONS/rational.h>
#include <ONS/orbit.h>

namespace ONS {

	template<typename T>
	class nomset {
	public:
		typedef typename std::set<orbit<T>>::iterator iterator;
		typedef typename std::set<orbit<T>>::const_iterator const_iterator;
	
		std::set<orbit<T>> orbits;
	
		nomset(){}
		template<typename Q>
		nomset(nomset<Q> orig) {
			for (auto o : orig.orbits) {
				orbits.insert(orbit<Q>(o));
			}
		}
		nomset(T el) {
			orbits.insert(orbit<T>(el));
		}
		
		bool contains(orbit<T> orb) const {
			return orbits.count(orb) != 0;
		}
		bool contains(T el) const {
			return contains(orbit<T>(el));
		}
		bool contains(nomset<T> subset) const {
			if (subset.orbits.size() > orbits.size())
				return false;
			if (subset.orbits.size() * 10 > orbits.size()) {
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
			} else {
				for (auto o : subset.orbits) {
					if (!contains(o)) return false;
				}
				return true;
			}
		}
	
		void insert(orbit<T> orb) {
			orbits.insert(orb);
		}
		void insert(T el) {
			orbits.insert(orbit<T>(el));
		}
	
		iterator begin() {
			return orbits.begin();
		}
	
		iterator end() {
			return orbits.end();
		}
		
		const_iterator cbegin() const {
			return orbits.cbegin();
		}
		
		const_iterator cend() const {
			return orbits.cend();
		}
	
		size_t size() const {
			return orbits.size();
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
	nomset<T> nomset_intersect(nomset<T> const &a, nomset<T> const &b) {
		nomset<T> c;
		auto ait = a.orbits.cbegin();
		auto bit = b.orbits.cbegin();
		while (ait != a.orbits.cend() && bit != b.orbits.cend()) {
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
	
	template<typename T>
	nomset<T> nomset_minus(nomset<T> const &a, nomset<T> const &b) {
		nomset<T> c;
		auto ait = a.orbits.cbegin();
		auto bit = b.orbits.cbegin();
		while (ait != a.orbits.cend() && bit != b.orbits.cend()) {
			if (*ait == *bit) {
				ait++;
				bit++;
			} else if (*ait < *bit) {
				c.orbits.insert(c.orbits.end(), *ait);
				ait++;
			} else {
				bit++;
			}
		}
		while (ait != a.orbits.cend()) {
			c.orbits.insert(c.orbits.end(), *ait);
			ait++;
		}
		return c;
	}
	
	template<typename T>
	nomset<T> nomset_symmetric_difference(nomset<T> const &a, nomset<T> const &b) {
		nomset<T> c;
		auto ait = a.orbits.cbegin();
		auto bit = b.orbits.cbegin();
		while (ait != a.orbits.cend() && bit != b.orbits.cend()) {
			if (*ait == *bit) {
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
		while (ait != a.orbits.cend()) {
			c.orbits.insert(c.orbits.end(), *ait);
			ait++;
		}
		while (bit != b.orbits.cend()) {
			c.orbits.insert(c.orbits.end(), *bit);
			bit++;
		}
		return c;
	}

	template<typename A, typename B>
	nomset<std::pair<A,B>> nomset_product(nomset<A> const &a, nomset<B> const &b) {
		nomset<std::pair<A,B>> c;
		for (const auto &aorb: a.orbits) {
			for (const auto &borb: b.orbits) {
				for (unsigned i = 0; i<orbpair_internal::prodCount(aorb.supportSize(), borb.supportSize()); i++) {
					c.orbits.insert(c.orbits.end(), orbit<std::pair<A,B>>(aorb,borb,i));
				}
			}
		}
		return c;
	}

	inline nomset<singleton> nomset_singleton() {
		nomset<singleton> c;
		c.orbits.insert(c.orbits.end(), orbit<singleton>());
		return c;
	}

	inline nomset<rational> nomset_rationals() {
		nomset<rational> c;
		c.orbits.insert(c.orbits.end(), orbit<rational>());
		return c;
	}

	template<typename A, typename F>
	nomset<A> nomset_filter(nomset<A> const &a, F f) {
		nomset<A> c;
		for (const auto &o : a.orbits) {
			if (f(o.getElement()))
				c.orbits.insert(c.orbits.end(), o);
		}
		return c;
	}

	template<typename A, typename B, typename F>
	nomset<B> nomset_map(nomset<A> const &a, F f) {
		nomset<B> c;
		for (const auto &o : a.orbits) {
			c.orbits.insert(orbit<B>(f(o.getElement())));
		}
		return c;
	}

}
#endif
