#ifndef ORBSETS_ORBIT_PAIR
#define ORBSETS_ORBIT_PAIR

#include <utility>
#include <string>
#include <cassert>

template<typename A, typename B>
bool operator==(orbit<std::pair<A,B>> a, orbit<std::pair<A,B>>b) {
	return a.productMap == b.productMap && a.Aorbit == b.Aorbit && a.Borbit == b.Borbit;
}
template<typename A, typename B>
bool operator!=(orbit<std::pair<A,B>> a, orbit<std::pair<A,B>>b) {
	return a.productMap != b.productMap || a.Aorbit != b.Aorbit || a.Borbit != b.Borbit;
}

template<typename A, typename B>
bool operator<(orbit<std::pair<A,B>> a, orbit<std::pair<A,B>>b) {
	if (a.Aorbit != b.Aorbit)
		return a.Aorbit < b.Aorbit;
	if (a.Borbit != b.Borbit)
		return a.Borbit < b.Borbit;
	return a.productMap < b.productMap;
}
template<typename A, typename B>
bool operator>(orbit<std::pair<A,B>> a, orbit<std::pair<A,B>>b) {
	if (a.Aorbit != b.Aorbit)
		return a.Aorbit > b.Aorbit;
	if (a.Borbit != b.Borbit)
		return a.Borbit > b.Borbit;
	return a.productmap > b.productMap;
}

template<typename T>
class nomset;

template<typename A, typename B>
class orbit<std::pair<A,B>> {
private:
	std::string productMap;
	orbit<A> Aorbit;
	orbit<B> Borbit;
	orbit(orbit<A> aorb, orbit<B> borb, std::string p) : productMap(p), Aorbit(aorb), Borbit(borb) {}
public:
	int supportSize() {
		return productMap.size();
	}
	std::pair<A,B> getElementFromSeq(std::vector<rational> seq) {
		assert(seq.size() >= productMap.size());
		
		std::vector<rational> Aseq, Bseq;
		for (size_t i=0; i<productMap.size(); i++) {
			if (productMap[i] == 'A' || productMap[i] == 'C') {
				Aseq.push_back(seq[i]);
			}
			if (productMap[i] == 'B' || productMap[i] == 'C') {
				Bseq.push_back(seq[i]);
			}
		}
		
		return std::pair<A,B>(Aorbit.getElementFromSeq(Aseq), Borbit.getElementFromSeq(Bseq));
	}
	std::vector<rational> getSeqFromElement(std::pair<A,B> el) {
		assert(isElement(el));
		
		std::vector<rational> Aseq = Aorbit.getSeqFromElement(el.first);
		std::vector<rational> Bseq = Borbit.getSeqFromElement(el.second);
		
		std::vector<rational> seq;
		size_t i_A = 0, i_B = 0;
		for (size_t i=0; i<productMap.size(); i++) {
			if (productMap[i] == 'A') {
				seq.push_back(Aseq[i_A]);
			} else {
				seq.push_back(Bseq[i_B]);
			}
			
			if (productMap[i] == 'A' || productMap[i] == 'C') {
				i_A++;
			}
			if (productMap[i] == 'B' || productMap[i] == 'C') {
				i_B++;
			}
		}
		
		return seq;
	}
	orbit(){}
	orbit(std::pair<A,B> el) : productMap(""), Aorbit(el.first), Borbit(el.second) {
		std::vector<rational> Aseq = Aorbit.getSeqFromElement(el.first);
		std::vector<rational> Bseq = Borbit.getSeqFromElement(el.second);
		size_t A_i = 0;
		size_t B_i = 0;
		
		while (A_i < Aseq.size() && B_i < Bseq.size()) {
			if (Aseq[A_i] == Bseq[B_i]) {
				productMap += 'C';
				A_i++;
				B_i++;
			} else if (Aseq[A_i] < Bseq[B_i]) {
				productMap += 'A';
				A_i++;
			} else {
				productMap += 'B';
				B_i++;
			}
		}
		while (A_i < Aseq.size()) {
			productMap += 'A';
			A_i++;
		}
		while (B_i < Bseq.size()) {
			productMap += 'B';
			B_i++;
		}
	}
	std::pair<A,B> getElement() {
		std::vector<rational> seq;
		for (size_t i=0; i<productMap.size(); i++) {
			seq.push_back(i+1);
		}
		return getElementFromSeq(seq);
	}
	bool isElement(std::pair<A,B> el) {
		if (!Aorbit.isElement(el.first) || !Borbit.isElement(el.second)) {
			return false;
		}
		
		std::vector<rational> Aseq = Aorbit.getSeqFromElement(el.first);
		std::vector<rational> Bseq = Borbit.getSeqFromElement(el.second);
		
		size_t i_A = 0, i_B = 0;
		for (size_t i=0; i<productMap.size(); i++) {
			if (productMap[i] == 'C' && Aseq[i_A] != Bseq[i_B]) {
				return false;
			}
			
			if (productMap[i] == 'A' || productMap[i] == 'C') {
				if (productMap[i] != 'C' && i_B != Bseq.size() && !(Aseq[i_A] < Bseq[i_B]))
					return false;
				i_A++;
			}
			if (productMap[i] == 'B' || productMap[i] == 'C') {
				if (productMap[i] != 'C' && i_A != Aseq.size() && !(Aseq[i_A] > Bseq[i_B]))
					return false;
				i_B++;
			}
		}
		
		return true;
	}
	
	friend bool operator== <>(orbit<std::pair<A,B>> a, orbit<std::pair<A,B>>b);
	friend bool operator!= <>(orbit<std::pair<A,B>> a, orbit<std::pair<A,B>>b);
	friend bool operator<  <>(orbit<std::pair<A,B>> a, orbit<std::pair<A,B>>b);
	friend bool operator>  <>(orbit<std::pair<A,B>> a, orbit<std::pair<A,B>>b);
	template<typename C, typename D> friend nomset<std::pair<C,D>> nomset_product(nomset<C> a, nomset<D> b);
};

#endif
