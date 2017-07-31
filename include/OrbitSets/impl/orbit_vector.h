#ifndef ORBSETS_ORBIT_VECTOR
#define ORBSETS_ORBIT_VECTOR

#include <vector>

namespace std {
	template<typename A>
	std::ostream &operator<<(std::ostream &o, std::vector<A> v) {
		bool first = true;
		o << "[";
		for (auto e : v) {
			if (first) first = false;
			else o << ", ";
			o << e;
		}
		o << "]";
		return o;
	}
}

namespace OrbitSets {
	template<typename T>
	bool operator==(orbit<std::vector<T>> a, orbit<std::vector<T>> b) {
		if (a.inner == b.inner) return true;
		if (a.inner == NULL || b.inner == NULL) return false;
		return *(a.inner) == *(b.inner);
	}
	template<typename T>
	bool operator!=(orbit<std::vector<T>> a, orbit<std::vector<T>> b) {
		if (a.inner == b.inner) return false;
		if (a.inner == NULL || b.inner == NULL) return true;
		return *(a.inner) != *(b.inner);
	}
	template<typename T>
	bool operator<(orbit<std::vector<T>> a, orbit<std::vector<T>> b) {
		if (a.inner == NULL && b.inner != NULL)
			return true;
		if (a.inner == NULL && b.inner == NULL)
			return false;
		if (b.inner == NULL)
			return false;
		return *(a.inner) < *(b.inner);
	}
	template<typename T>
	bool operator>(orbit<std::vector<T>> a, orbit<std::vector<T>> b) {
		if (b.inner == NULL && a.inner != NULL)
			return true;
		if (b.inner == NULL && a.inner == NULL)
			return false;
		if (a.inner == NULL)
			return false;
		return *(a.inner) > *(b.inner);
	}
	template<typename T>
	bool operator<=(orbit<std::vector<T>> a, orbit<std::vector<T>> b) {
		if (a.inner == NULL && b.inner != NULL)
			return true;
		if (a.inner == NULL && b.inner == NULL)
			return true;
		if (b.inner == NULL)
			return false;
		return *(a.inner) <= *(b.inner);
	}
	template<typename T>
	bool operator>=(orbit<std::vector<T>> a, orbit<std::vector<T>> b) {
		if (b.inner == NULL && a.inner != NULL)
			return true;
		if (b.inner == NULL && a.inner == NULL)
			return true;
		if (a.inner == NULL)
			return false;
		return *(a.inner) >= *(b.inner);
	}
	
	template<typename T>
	class orbit<std::vector<T>> {
	private:
		orbit<std::pair<std::vector<T>, T>> *inner;
	public:
		orbit() {
			inner = NULL;
		}
		orbit(const orbit<std::vector<T>> &orig) {
			if (orig.inner == NULL) {
				inner = NULL;
			} else {
				inner = new orbit<std::pair<std::vector<T>,T>>(*orig.inner);
			}
		}
		~orbit() {
			if (inner != NULL) delete inner;
		}
		int supportSize() const {
			if (inner == NULL) return 0;
			return inner->supportSize();
		}
		std::vector<T> getElementFromSeq(std::vector<rational> seq) const {
			if (inner == NULL) return std::vector<T>(0);
			
			std::pair<std::vector<T>, T> inEl = inner->getElementFromSeq(seq);
			inEl.first.push_back(inEl.second);
			return inEl.first;
		}
		std::vector<rational> getSeqFromElement(std::vector<T> el) const {
			assert(isElement(el));
			
			if (inner == NULL) return std::vector<rational>(0);
			std::pair<std::vector<T>, T> inEl = {el, el.back()};
			inEl.first.pop_back();
			return inner->getSeqFromElement(inEl);
		}
		orbit(std::vector<T> el) {
			if (el.size() == 0) {
				inner = NULL;
			} else {
				std::pair<std::vector<T>, T> inEl = {el, el.back()};
				inEl.first.pop_back();
				inner = new orbit<std::pair<std::vector<T>, T>>(inEl);
			}
		}
		std::vector<T> getElement() const {
			if (inner == NULL) return std::vector<T>(0);
			
			auto inEl = inner->getElement();
			inEl.first.push_back(inEl.second);
			return inEl.first;
		}
		bool isElement(std::vector<T> el) const {
			if (inner == NULL) return el.size() == 0;
			
			std::pair<std::vector<T>,T> inEl = {el, el.back()};
			inEl.first.pop_back();
			return inner->isElement(inEl);
		}
		
		friend bool operator==<>(orbit<std::vector<T>> a, orbit<std::vector<T>> b);
		friend bool operator!=<>(orbit<std::vector<T>> a, orbit<std::vector<T>> b);
		friend bool operator< <>(orbit<std::vector<T>> a, orbit<std::vector<T>> b);
		friend bool operator> <>(orbit<std::vector<T>> a, orbit<std::vector<T>> b);
		friend bool operator<=<>(orbit<std::vector<T>> a, orbit<std::vector<T>> b);
		friend bool operator>=<>(orbit<std::vector<T>> a, orbit<std::vector<T>> b);
	};
}

#endif

