#ifndef ORBSETS_ORBIT_ILABEL
#define ORBSETS_ORBIT_ILABEL

#include <utility>
#include <string>
#include <cassert>

namespace OrbitSets {

	template<typename A>
	bool operator==(orbit<std::pair<int,A>> a, orbit<std::pair<int,A>> b) {
		return a.label == b.label && a.Aorbit == b.Aorbit;
	}
	template<typename A>
	bool operator!=(orbit<std::pair<int,A>> a, orbit<std::pair<int,A>> b) {
		return a.label != b.label || a.Aorbit != b.Aorbit;
	}

	template<typename A>
	bool operator<(orbit<std::pair<int,A>> a, orbit<std::pair<int,A>> b) {
		if (a.label != b.label)
			return a.label < b.label;
		return a.Aorbit < b.Aorbit;
	}
	template<typename A>
	bool operator>(orbit<std::pair<int,A>> a, orbit<std::pair<int,A>> b) {
		if (a.label != b.label)
			return a.label > b.label;
		return a.Aorbit > b.Aorbit;
	}

	template<typename A>
	class orbit<std::pair<int,A>> {
	private:
		int label;
		orbit<A> Aorbit;
	public:
		int supportSize() const {
			return Aorbit.supportSize();
		}
		std::pair<int,A> getElementFromSeq(std::vector<rational> seq) const {
			return std::pair<int,A>(label, Aorbit.getElementFromSeq(seq));
		}
		std::vector<rational> getSeqFromElement(std::pair<int,A> el) const {
			return Aorbit.getSeqFromElement(el.second);
		}
		orbit(){}
		orbit(std::pair<int, A> el) : label(el.first), Aorbit(el.second) {}
		orbit(int s, orbit<A> o) : label(s), Aorbit(o) {}
		std::pair<int,A> getElement() const {
			return std::pair<int,A>(label, Aorbit.getElement());
		}
		bool isElement(std::pair<int,A> el) const {
			if (el.first != label)
				return false;
			return Aorbit.isElement(el.second);
		}
	
		friend bool operator==<>(orbit<std::pair<int,A>> a, orbit<std::pair<int,A>> b);
		friend bool operator!=<>(orbit<std::pair<int,A>> a, orbit<std::pair<int,A>> b);
		friend bool operator< <>(orbit<std::pair<int,A>> a, orbit<std::pair<int,A>> b);
		friend bool operator> <>(orbit<std::pair<int,A>> a, orbit<std::pair<int,A>> b);
	};

}

#endif
