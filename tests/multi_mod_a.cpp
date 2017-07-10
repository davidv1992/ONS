#include <utility>

#include <OrbitSets/nomset.h>
#include <OrbitSets/rational.h>

using namespace std;
using namespace OrbitSets;

nomset<pair<rational,rational>> f() {
	return nomset_product(nomset_rationals(), nomset_rationals());
}
