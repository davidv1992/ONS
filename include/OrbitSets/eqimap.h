#ifndef ORBSETS_EQIMAP
#define ORBSETS_EQIMAP

#include <map>
#include <vector>
#include <stdexcept>
#include <OrbitSets/rational.h>
#include <OrbitSets/orbit.h>

template<typename Tdom, typename Trng>
class eqimap {
public:
	typedef typename std::map<orbit<Tdom>, std::pair<orbit<Trng>, std::vector<bool>>>::iterator iterator;

	std::map<orbit<Tdom>, std::pair<orbit<Trng>, std::vector<bool>>> mapData;
	eqimap() {}
	template<typename F>
	eqimap(nomset<Tdom> domain, F f) {
		for (auto o : domain.orbits) {
			Tdom inEl = o.getElement();
			std::vector<rational> inSeq = o.getSeqFromElement(inEl);
			Trng outEl = f(inEl);
			orbit<Trng> outo(outEl);
			std::vector<rational> outSeq = outo.getSeqFromElement(outEl);
			std::vector<bool> mask;
			size_t out_i = 0;
			for (size_t i=0; i<inSeq.size(); i++) {
				if (out_i < outSeq.size() && inSeq[i] > outSeq[out_i])
					throw std::range_error("Function does not give equivariant map");
				if (out_i < outSeq.size() && inSeq[i] == outSeq[out_i]) {
					mask.push_back(true);
					out_i++;
				} else {
					mask.push_back(false);
				}
			}
			
			mapData[o] = std::pair<orbit<Trng>,std::vector<bool>>(outo, mask);
		}
	}

	Trng operator()(Tdom in) const {
		orbit<Tdom> inOrbit(in);
		auto md = mapData.find(inOrbit);
		if (md == mapData.end())
			throw std::domain_error("Equivariant map input not in domain");
		
		std::vector<rational> inSeq = inOrbit.getSeqFromElement(in);
		std::vector<rational> outSeq;
		const std::vector<bool> &mapMask = md->second.second;
		
		for (size_t i=0; i<inSeq.size(); i++) {
			if (mapMask[i])
				outSeq.push_back(inSeq[i]);
		}
		
		return md->second.first.getElementFromSeq(outSeq);
	}
	
	void add(Tdom in, Trng out) {
		orbit<Tdom> ino(in);
		orbit<Trng> outo(out);
		std::vector<rational> inSeq = ino.getSeqFromElement(in);
		std::vector<rational> outSeq = outo.getSeqFromElement(out);
		std::vector<bool> mask;
		size_t out_i = 0;
		for (size_t i=0; i<inSeq.size(); i++) {
			if (out_i < outSeq.size() && inSeq[i] > outSeq[out_i])
				throw std::range_error("Input does not give equivariant map");
			if (out_i < outSeq.size() && inSeq[i] == outSeq[out_i]) {
				mask.push_back(true);
				out_i++;
			} else {
				mask.push_back(false);
			}
		}
		mapData[ino] = std::pair<orbit<Trng>,std::vector<bool>>(outo, mask);
	}
	
	std::pair<orbit<Trng>, std::vector<bool>> &operator[](orbit<Tdom> d) {
		return mapData[d];
	}
	
	iterator begin() {
		return mapData.begin();
	}
	
	iterator end() {
		return mapData.end();
	}
};

#endif
