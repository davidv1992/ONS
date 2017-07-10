#ifndef ORBSETS_ORBIT_PAIR
#define ORBSETS_ORBIT_PAIR

#include <utility>
#include <string>
#include <cassert>
#include <iostream>

namespace OrbitSets {

	template<typename A, typename B>
	std::ostream &operator<<(std::ostream &o, std::pair<A,B> p) {
		return o << "(" << p.first << "," << p.second << ")";
	}
	
	namespace orbpair_internal {
		inline unsigned prodCount(unsigned A, unsigned B) {
			static std::vector<std::vector<unsigned>>prodTable(1,std::vector<unsigned>(1,1));
			if (!(prodTable.size() > A && prodTable[0].size() > B)) {
				A++;
				B++;
		
				if (prodTable[0].size() < B) {
					for (unsigned i=0; i<prodTable.size(); i++) {
						for (unsigned j=prodTable[i].size(); j<B; j++) {
							unsigned res = 0;
							if (i > 0) res += prodTable[i-1][j];
							if (j > 0) res += prodTable[i][j-1];
							if (i > 0 && j > 0) res += prodTable[i-1][j-1];
							prodTable[i].push_back(res);
						}
					}
				}

				for (unsigned i=prodTable.size(); i<A; i++) {
					prodTable.push_back(std::vector<unsigned>(B));
					for (unsigned j=0; j<B; j++) {
						unsigned res = 0;
						if (i > 0) res += prodTable[i-1][j];
						if (j > 0) res += prodTable[i][j-1];
						if (i > 0 && j > 0) res += prodTable[i-1][j-1];
						prodTable[i][j] = res;
					}
				}
		
				A--;
				B--;
			}
			return prodTable[A][B];
		}
	}

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
		return a.productMap > b.productMap;
	}

	template<typename T>
	class nomset;

	template<typename A, typename B>
	class orbit<std::pair<A,B>> {
	private:
		unsigned productMap;
		unsigned size;
		orbit<A> Aorbit;
		orbit<B> Borbit;
		orbit(orbit<A> aorb, orbit<B> borb, unsigned p) : productMap(p), Aorbit(aorb), Borbit(borb) {
			unsigned nA = Aorbit.supportSize();
			unsigned nB = Borbit.supportSize();
			unsigned idx = productMap;
			size = 0;
			while (nA || nB) {
				size++;
				if (nA != 0 && idx < orbpair_internal::prodCount(nA-1, nB)) {
					nA--;
				} else {
					if (nA != 0)
						idx -= orbpair_internal::prodCount(nA-1, nB);
					nB--;
					if (nA != 0 && idx < orbpair_internal::prodCount(nA-1, nB)) {
						nA--;
					} else if (nA != 0) {
						idx -= orbpair_internal::prodCount(nA-1, nB);
					}
				}
			}
		}
	public:
		int supportSize() const {
			return size;
		}
		std::pair<A,B> getElementFromSeq(std::vector<rational> seq) const {
			assert(seq.size() == size);
		
			std::vector<rational> Aseq, Bseq;
			unsigned nA = Aorbit.supportSize();
			unsigned nB = Borbit.supportSize();
			unsigned idx = productMap;
			for (unsigned i = 0; i<size; i++) {
				if (nA != 0 && idx < orbpair_internal::prodCount(nA-1, nB)) {
					Aseq.push_back(seq[i]);
					nA--;
				} else {
					if (nA != 0)
						idx -= orbpair_internal::prodCount(nA-1, nB);
					Bseq.push_back(seq[i]);
					nB--;
					if (nA != 0 && idx < orbpair_internal::prodCount(nA-1, nB)) {
						Aseq.push_back(seq[i]);
						nA--;
					} else if (nA != 0) {
						idx -= orbpair_internal::prodCount(nA-1, nB);
					}
				}
			}
		
			return std::pair<A,B>(Aorbit.getElementFromSeq(Aseq), Borbit.getElementFromSeq(Bseq));
		}
		std::vector<rational> getSeqFromElement(std::pair<A,B> el) const {
			assert(isElement(el));
		
			std::vector<rational> Aseq = Aorbit.getSeqFromElement(el.first);
			std::vector<rational> Bseq = Borbit.getSeqFromElement(el.second);
		
			std::vector<rational> seq;
			seq.reserve(size);
			unsigned nA = Aorbit.supportSize(), nB = Borbit.supportSize();
			size_t i_A = 0, i_B = 0;
			unsigned idx = productMap;
			for (unsigned i = 0; i < size; i++) {
				if (nA != 0 && idx < orbpair_internal::prodCount(nA-1, nB)) {
					seq.push_back(Aseq[i_A]);
					nA--;
					i_A++;
				} else {
					if (nA != 0)
						idx -= orbpair_internal::prodCount(nA-1, nB);
					seq.push_back(Bseq[i_B]);
					nB--;
					i_B++;
					if (nA != 0 && idx < orbpair_internal::prodCount(nA-1, nB)) {
						nA--;
						i_A++;
					} else if (nA != 0) {
						idx -= orbpair_internal::prodCount(nA-1, nB);
					}
				}
			}
		
			return seq;
		}
		orbit(){}
		orbit(std::pair<A,B> el) : Aorbit(el.first), Borbit(el.second) {
			std::vector<rational> Aseq = Aorbit.getSeqFromElement(el.first);
			std::vector<rational> Bseq = Borbit.getSeqFromElement(el.second);
			size_t A_i = 0;
			size_t B_i = 0;
			unsigned nA = Aorbit.supportSize(), nB = Borbit.supportSize();
		
			size = 0;
			productMap = 0;
			while (nA != 0 && nB != 0) {
				size++;
				if (Aseq[A_i] < Bseq[B_i]) {
					nA--;
					A_i++;
				} else {
					productMap += orbpair_internal::prodCount(nA-1, nB);
					if (Aseq[A_i] == Bseq[B_i]) {
						nB--;
						B_i++;
						nA--;
						A_i++;
					} else {
						productMap += orbpair_internal::prodCount(nA-1, nB-1);
						nB--;
						B_i++;
					}
				}
			}
			size += nA;
			size += nB;
		}
		std::pair<A,B> getElement() const {
			std::vector<rational> seq;
			for (unsigned i=0; i<size; i++) {
				seq.push_back(i+1);
			}
			return getElementFromSeq(seq);
		}
		bool isElement(std::pair<A,B> el) const {
			if (!Aorbit.isElement(el.first) || !Borbit.isElement(el.second)) {
				return false;
			}
		
			std::vector<rational> Aseq = Aorbit.getSeqFromElement(el.first);
			std::vector<rational> Bseq = Borbit.getSeqFromElement(el.second);
		
			size_t i_A = 0, i_B = 0;
			unsigned nA = Aorbit.supportSize(), nB = Borbit.supportSize();
			unsigned idx = productMap;
			for (unsigned i = 0; i< size && nA != 0 && nB != 0; i++) {
				if (idx < orbpair_internal::prodCount(nA-1, nB)) {
					if (!(Aseq[i_A] < Bseq[i_B])) return false;
					nA--;
					i_A++;
				} else {
					idx -= orbpair_internal::prodCount(nA-1, nB);
					if (idx < orbpair_internal::prodCount(nA-1, nB-1)) {
						if (Aseq[i_A] != Bseq[i_B]) return false;
						nA--;
						i_A++;
						nB--;
						i_B++;
					} else {
						if (!(Aseq[i_A] > Bseq[i_B])) return false;
						idx -= orbpair_internal::prodCount(nA-1, nB-1);
						nB--;
						i_B++;
					}
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
	
}

#endif
