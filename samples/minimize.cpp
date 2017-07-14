#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <utility>
#include <sys/time.h>

#include <OrbitSets/nomset.h>
#include <OrbitSets/eqimap.h>

using namespace std;
using namespace OrbitSets;

long long get_time_usec() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

template<typename Q, typename A>
class automaton {
public:
	nomset<A> alphabet;
	nomset<Q> states;
	eqimap<pair<Q,A>,Q> delta;
	nomset<Q> finalStates;
	Q initialState;
	
	bool accepts(std::vector<A> word) {
		Q currentState = initialState;
		for (auto a : word) {
			currentState = delta({currentState, a});
		}
		return finalStates.contains(currentState);
	}
};

template<typename Q, typename A>
nomset<pair<Q,Q>> automaton_equiv(automaton<Q,A> aut) {
	nomset<Q> StatesSubFinal = nomset_filter(aut.states, [&](Q el) { return !aut.finalStates.contains(el); });
	
	nomset<pair<Q,Q>> result = nomset_union(nomset_product(aut.finalStates, aut.finalStates), nomset_product(StatesSubFinal, StatesSubFinal));
	
	while (true) {
		nomset<pair<Q,Q>> next;
		
		for (auto o : result.orbits) {
			nomset<pair<Q,Q>> curOrbit;
			curOrbit.orbits.insert(o);
			
			nomset<pair<pair<Q,Q>, A>> testInput = nomset_product(curOrbit, aut.alphabet);
			nomset<pair<Q,Q>> testOut = nomset_map<pair<pair<Q,Q>,A>, pair<Q,Q>>(testInput, [&](pair<pair<Q,Q>,A> el) {
				auto q1 = aut.delta(pair<Q,A>(el.first.first, el.second));
				auto q2 = aut.delta(pair<Q,A>(el.first.second, el.second));
				auto res = pair<Q,Q>(q1, q2);
				return res;
			});
			bool testResult = result.contains(testOut);
			if (testResult) {
				next.orbits.insert(next.orbits.end(), o);
			}
		}

		if (next == result)
			break;
		result = next;
	}
	
	return result;
}

template<typename Q, typename A>
automaton<pair<string, abstract>,A> automaton_minimize_a(automaton<Q,A> aut) {
	auto equiv = automaton_equiv(aut);
	automaton<pair<string, abstract>, A> res;
	res.alphabet = aut.alphabet;
	
	eqimap<Q, pair<string, abstract>> partition;
	int curLabel = 1;
	for (auto o : aut.states.orbits) {
		nomset<Q> curOrbit;
		curOrbit.orbits.insert(o);
		bool handled = false;
		for (auto s : aut.states.orbits) {
			if (!(s < o)) break;
			nomset<Q> refOrbit;
			refOrbit.orbits.insert(s);
			nomset<pair<Q,Q>> refProduct = nomset_product(refOrbit, curOrbit);
			for (auto p : refProduct.orbits) {
				if (equiv.contains(p)) {
					pair<Q,Q> el = p.getElement();
					std::vector<rational> seqRef, seqCur;
					seqRef = s.getSeqFromElement(el.first);
					seqCur = o.getSeqFromElement(el.second);
					std::vector<bool> refMask = partition.mapData[s].second;
					std::vector<bool> curMask(o.supportSize(), false);
					
					size_t i_A =0, i_B = 0;
					while (i_A < seqRef.size() && i_B < seqCur.size()) {
						if (seqRef[i_A] == seqCur[i_B]) {
							if (refMask[i_A])
								curMask[i_B] = true;
							i_A++;
							i_B++;
						} else if (seqRef[i_A] < seqRef[i_B]) {
							i_A++;
						} else {
							i_B++;
						}
					}
					partition.mapData[o] = {partition.mapData[s].first, curMask};
					
					handled = true;
					break;
				}
			}
			if (handled) break;
		}
		
		if (handled) continue;
		
		nomset<pair<Q,Q>> curProduct = nomset_product(curOrbit, curOrbit);
		std::vector<bool> mask(o.supportSize(), true);
		for (auto p : curProduct.orbits) {
			if (equiv.contains(p)) {
				pair<Q,Q> el = p.getElement();
				std::vector<rational> seqA, seqB;
				seqA = o.getSeqFromElement(el.first);
				seqB = o.getSeqFromElement(el.second);
				
				for (size_t i=0; i<seqA.size(); i++) {
					if (seqA[i] != seqB[i])
						mask[i] = false;
				}
			}
		}
		
		int tgtSupSize = 0;
		for (auto b : mask) {
			if (b) tgtSupSize++;
		}
		
		ostringstream labelStream;
		labelStream << "O" << curLabel;
		curLabel++;
		orbit<pair<string, abstract>> newOrbit = orbit<pair<string, abstract>>(labelStream.str(), orbit<abstract>(tgtSupSize));
		res.states.orbits.insert(newOrbit);
		partition.mapData[o] = {newOrbit, mask};
	}
	
	res.initialState = partition(aut.initialState);
	res.finalStates = nomset_map<Q, pair<string,abstract>,eqimap<Q,pair<string,abstract>>>(aut.finalStates, partition);
	
	auto deltaSpace = nomset_product(aut.states, aut.alphabet);
	for (auto o : deltaSpace.orbits) {
		auto el = o.getElement();
		pair<pair<string,abstract>,A> nel;
		nel.second = el.second;
		nel.first = partition(el.first);
		orbit<pair<pair<string,abstract>,A>> to(nel);
		if (res.delta.mapData.count(to) != 0) continue;
		
		pair<string,abstract> out = partition(aut.delta(el));
		res.delta.add(nel, out);
	}
	
	return res;
}

template<typename Q, typename A> automaton<pair<int,abstract>,A> automaton_minimize_b(automaton<Q,A> aut) {
	nomset<pair<int,abstract>> part_domain;
	eqimap<Q,pair<int,abstract>> partition;
	
	part_domain.orbits.insert(orbit<pair<int,abstract>>(1, orbit<abstract>(0)));
	part_domain.orbits.insert(orbit<pair<int,abstract>>(2, orbit<abstract>(0)));
	partition = eqimap<Q,pair<int,abstract>>(aut.states, [&](Q el) {
		if (aut.finalStates.contains(el)) {
			return pair<int, abstract>(1, abstract());
		} else {
			return pair<int, abstract>(2, abstract());
		}
	});
	
	bool cont = true;
	while (cont) {
		nomset<pair<int,abstract>> next_domain;
		eqimap<Q,pair<int,abstract>> next_partition;
		int next_dom_count = 1;
		
		cont = false;
		for (auto o : aut.states.orbits) {
			bool done = false;
			nomset<Q> curOrbit;
			curOrbit.orbits.insert(o);
			
			for (auto s : aut.states.orbits) {
				if (!(s < o)) break;
				nomset<Q> refOrbit;
				refOrbit.orbits.insert(s);
				nomset<pair<Q,Q>> refProduct = nomset_product(curOrbit, refOrbit);
				for (auto p : refProduct.orbits) {
					pair<Q,Q> el = p.getElement();
					pair<int, abstract> e1 = partition(el.first);
					pair<int, abstract> e2 = partition(el.second);
					if (e1 != e2) continue;
					
					bool isOk = true;
					nomset<pair<Q,Q>> pOrbit;
					pOrbit.orbits.insert(p);
					nomset<pair<pair<Q,Q>,A>> alphProduct = nomset_product(pOrbit, aut.alphabet);
					for (auto ao : alphProduct.orbits) {
						pair<pair<Q,Q>,A> ael = ao.getElement();
						pair<int, abstract> e1 = partition(aut.delta(pair<Q,A>(ael.first.first, ael.second)));
						pair<int, abstract> e2 = partition(aut.delta(pair<Q,A>(ael.first.second, ael.second)));
						if (e1 != e2) {
							isOk = false;
							cont = true;
							break;
						}
					}
					
					if (!isOk) continue;
					auto md = next_partition.mapData.find(s);
					std::vector<bool> &refMask = md->second.second;
					std::vector<bool> curMask(o.supportSize(), false);
					std::vector<rational> seqRef, seqCur;
					seqCur = o.getSeqFromElement(el.first);
					seqRef = s.getSeqFromElement(el.second);
					size_t i_A = 0, i_B = 0;
					while (i_A < seqCur.size() && i_B < seqRef.size()) {
						if (seqCur[i_A] == seqRef[i_B]) {
							curMask[i_A] = refMask[i_B];
							i_A++;
							i_B++;
						} else if (seqCur[i_A] < seqRef[i_B]) {
							i_A++;
						} else {
							i_B++;
						}
					}
					next_partition.mapData[o] = {md->second.first, curMask};
					done = true;
				}
				
				if (done) break;
			}
			
			if (done) continue;
			
			nomset<pair<Q,Q>> curProduct = nomset_product(curOrbit, curOrbit);
			std::vector<bool> mask(o.supportSize(), true);
			for (auto p : curProduct.orbits) {
				auto el = p.getElement();
				if (partition(el.first) != partition(el.second)) continue;
				
				bool isOk = true;
				nomset<pair<Q,Q>> pOrbit;
				pOrbit.orbits.insert(p);
				nomset<pair<pair<Q,Q>,A>> alphProduct = nomset_product(pOrbit, aut.alphabet);
				for (auto ao : alphProduct.orbits) {
					auto ael = ao.getElement();
					if (partition(aut.delta({ael.first.first, ael.second})) != partition(aut.delta({ael.first.second,ael.second}))) {
						isOk = false;
						cont = true;
						break;
					}
				}
				if (!isOk) continue;
				
				auto Aseq = o.getSeqFromElement(el.first);
				auto Bseq = o.getSeqFromElement(el.second);
				
				for (size_t i = 0; i<Aseq.size(); i++) {
					if (Aseq[i] != Bseq[i])
						mask[i] = false;
				}
			}
			
			unsigned tgtSuppSize = 0;
			for (auto b : mask) {
				if (b) tgtSuppSize++;
			}
			
			orbit<pair<int, abstract>> newOrbit(next_dom_count, orbit<abstract>(tgtSuppSize));
			next_dom_count++;
			next_domain.orbits.insert(newOrbit);
			next_partition.mapData[o] = {newOrbit, mask};
		}
		
		part_domain = next_domain;
		partition = next_partition;
	}
	
	automaton<pair<int,abstract>,A> res;
	res.states = part_domain;
	res.initialState = partition(aut.initialState);
	res.finalStates = nomset_map<Q, pair<int,abstract>,eqimap<Q,pair<int,abstract>>>(aut.finalStates, partition);
	
	auto deltaSpace = nomset_product(aut.states, aut.alphabet);
	for (auto o : deltaSpace.orbits) {
		auto el = o.getElement();
		pair<pair<int,abstract>,A> nel;
		nel.second = el.second;
		nel.first = partition(el.first);
		orbit<pair<pair<int,abstract>,A>> to(nel);
		if (res.delta.mapData.count(to) != 0) continue;
		
		pair<int,abstract> out = partition(aut.delta(el));
		res.delta.add(nel, out);
	}
	
	return res;
}

void test_aut_small() {
	using Q = variant<singleton, rational, pair<rational, rational>>;
	automaton <Q, rational> aut;
	
	nomset<Q> S1 = nomset_singleton();
	nomset<Q> S2 = nomset_rationals();
	nomset<Q> S3 = nomset_product(nomset_rationals(), nomset_rationals());
	
	aut.alphabet = nomset_rationals();
	aut.states = nomset_union(S1, nomset_union(S2, S3));
	aut.delta = eqimap<pair<Q,rational>,Q>(nomset_product(aut.states, aut.alphabet), [](pair<Q,rational> in) {
		Q orig_state = in.first;
		if (orig_state.index() == 0) {
			return Q(in.second);
		} else if (orig_state.index() == 1) {
			return Q(pair<rational, rational>(orig_state.get<rational>(), in.second));
		} else {
			return Q(pair<rational, rational>(orig_state.get<pair<rational,rational>>().second, in.second));
		}
	});
	aut.finalStates = nomset_filter(aut.states, [](Q state){
		if (state.index() != 2) {
			return false;
		} else {
			pair<rational, rational> s = state.get<pair<rational, rational>>();
			return s.first > s.second;
		}
	});
	aut.initialState = singleton();
	
	long long t_start = get_time_usec();
	auto min_a = automaton_minimize_a(aut);
	long long t_mid = get_time_usec();
	auto min_b = automaton_minimize_b(aut);
	long long t_end = get_time_usec();
	
	cout << "Small testcase: " << endl;
	cout << "Variant a: " << t_mid - t_start << " usec" << endl;
	cout << "Variant b: " << t_end - t_mid << " usec" << endl;
	cout << "Automata sizes: ";
	cout << aut.states.size() << " " << min_a.states.size() << " " << min_b.states.size() << endl;
	
	vector<rational> test1 = {1,2,3,4,5};
	vector<rational> test2 = {1,3,2,5,4};
	cout << "Comparing acceptance: " << endl;
	cout << "Testcase 1: (";
	bool first = true;
	for (auto v : test1) {
		if (first) first = false;
		else cout << ", ";
		cout << v;
	}
	cout << ")" << endl;
	
	cout << "Original: " << aut.accepts(test1) << endl;
	cout << "Variant a: " << min_a.accepts(test1) << endl;
	cout << "Variant b: " << min_b.accepts(test1) << endl;
	
	cout << "Testcase 2: (";
	first = true;
	for (auto v : test1) {
		if (first) first = false;
		else cout << ", ";
		cout << v;
	}
	cout << ")" << endl;
	
	cout << "Original: " << aut.accepts(test2) << endl;
	cout << "Variant a: " << min_a.accepts(test2) << endl;
	cout << "Variant b: " << min_b.accepts(test2) << endl;
}

void test_aut_large() {
	using Q = variant<pair<string, singleton>, rational, pair<rational, rational>, pair<pair<rational, rational>,rational>>;
	automaton <Q, rational> aut;
	
	orbit<Q> O1(pair<string, singleton>("I", singleton()));
	orbit<Q> O2(pair<string, singleton>("E", singleton()));
	nomset<Q> S1 = nomset_rationals();
	nomset<Q> S2 = nomset_product(nomset_rationals(), nomset_rationals());
	nomset<Q> S3 = nomset_product(nomset_product(nomset_rationals(), nomset_rationals()), nomset_rationals());
	
	aut.alphabet = nomset_rationals();
	aut.states = nomset_union(nomset_union(S1, S2), S3);
	aut.states.orbits.insert(O1);
	aut.states.orbits.insert(O2);
	aut.finalStates = nomset_filter(aut.states, [](Q state) {
		if (state.index() != 3) {
			return false;
		} else {
			auto el = state.get<3>();
			return el.first.first == el.first.second || el.first.first == el.second || el.first.second == el.second;
		}
	});
	aut.initialState = O1.getElement();
	aut.delta = eqimap<pair<Q,rational>,Q>(nomset_product(aut.states, aut.alphabet), [](pair<Q,rational> in) {
		Q orig_state = in.first;
		if (orig_state.index() == 0) {
			if (orig_state.get<0>().first == "I") {
				return Q(in.second);
			} else {
				return orig_state;
			}
		} else if (orig_state.index() == 1) {
			return Q(pair<rational,rational>(orig_state.get<1>(), in.second));
		} else if (orig_state.index() == 2) {
			return Q(pair<pair<rational,rational>,rational>(orig_state.get<2>(), in.second));
		} else {
			return Q(pair<string, singleton>("E", singleton()));
		}
	});
	
	long long t_start = get_time_usec();
	auto min_a = automaton_minimize_a(aut);
	long long t_mid = get_time_usec();
	auto min_b = automaton_minimize_b(aut);
	long long t_end = get_time_usec();
	
	cout << "Large testcase: " << endl;
	cout << "Variant a: " << t_mid - t_start << " usec" << endl;
	cout << "Variant b: " << t_end - t_mid << " usec" << endl;
	cout << "Automata sizes: ";
	cout << aut.states.size() << " " << min_a.states.size() << " " << min_b.states.size() << endl;
}

int main() {
	test_aut_small();
	cout << endl;
	test_aut_large();
	return 0;
}
