#include <iostream>
#include <utility>

#include <OrbitSets/nomset.h>
#include <OrbitSets/rational.h>

using namespace std;

nomset<pair<rational,rational>> f();

int main() {
	nomset<pair<rational,rational>> x = nomset_product(nomset_rationals(), nomset_rationals()), y = f();
	if (x == y)
		return 0;
	else
		return 1;
}
