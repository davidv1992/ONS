#ifndef ONS_ORBIT_LABEL
#define ONS_ORBIT_LABEL

#include <utility>
#include <string>
#include <cassert>

namespace ONS {

	template<typename A>
	bool operator==(orbit<std::pair<std::string,A>> a, orbit<std::pair<std::string,A>> b) {
		return a.label == b.label && a.Aorbit == b.Aorbit;
	}
	template<typename A>
	bool operator!=(orbit<std::pair<std::string,A>> a, orbit<std::pair<std::string,A>> b) {
		return a.label != b.label || a.Aorbit != b.Aorbit;
	}

	template<typename A>
	bool operator<(orbit<std::pair<std::string,A>> a, orbit<std::pair<std::string,A>> b) {
		if (a.label != b.label)
			return a.label < b.label;
		return a.Aorbit < b.Aorbit;
	}
	template<typename A>
	bool operator>(orbit<std::pair<std::string,A>> a, orbit<std::pair<std::string,A>> b) {
		if (a.label != b.label)
			return a.label > b.label;
		return a.Aorbit > b.Aorbit;
	}
	template<typename A>
	bool operator<=(orbit<std::pair<std::string,A>> a, orbit<std::pair<std::string,A>> b) {
		if (a.label != b.label)
			return a.label <= b.label;
		return a.Aorbit <= b.Aorbit;
	}
	template<typename A>
	bool operator>=(orbit<std::pair<std::string,A>> a, orbit<std::pair<std::string,A>> b) {
		if (a.label != b.label)
			return a.label >= b.label;
		return a.Aorbit >= b.Aorbit;
	}
	

	template<typename A>
	class orbit<std::pair<std::string,A>> {
	private:
		std::string label;
		orbit<A> Aorbit;
	public:
		int supportSize() const {
			return Aorbit.supportSize();
		}
		std::pair<std::string,A> getElementFromSeq(std::vector<rational> seq) const {
			return std::pair<std::string,A>(label, Aorbit.getElementFromSeq(seq));
		}
		std::vector<rational> getSeqFromElement(std::pair<std::string,A> el) const {
			return Aorbit.getSeqFromElement(el.second);
		}
		orbit(){}
		orbit(std::pair<std::string, A> el) : label(el.first), Aorbit(el.second) {}
		orbit(std::string s, orbit<A> o) : label(s), Aorbit(o) {}
		std::pair<std::string,A> getElement() const {
			return std::pair<std::string,A>(label, Aorbit.getElement());
		}
		bool isElement(std::pair<std::string,A> el) const {
			if (el.first != label)
				return false;
			return Aorbit.isElement(el.second);
		}
	
		friend bool operator==<>(orbit<std::pair<std::string,A>> a, orbit<std::pair<std::string,A>> b);
		friend bool operator!=<>(orbit<std::pair<std::string,A>> a, orbit<std::pair<std::string,A>> b);
		friend bool operator< <>(orbit<std::pair<std::string,A>> a, orbit<std::pair<std::string,A>> b);
		friend bool operator> <>(orbit<std::pair<std::string,A>> a, orbit<std::pair<std::string,A>> b);
		friend bool operator<=<>(orbit<std::pair<std::string,A>> a, orbit<std::pair<std::string,A>> b);
		friend bool operator>=<>(orbit<std::pair<std::string,A>> a, orbit<std::pair<std::string,A>> b);
	};
}

#endif
