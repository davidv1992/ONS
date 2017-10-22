#include <utility>

#include <ONS/nomset.h>
#include <ONS/rational.h>

using namespace std;
using namespace ONS;

nomset<pair<rational,rational>> f() {
	return nomset_product(nomset_rationals(), nomset_rationals());
}
