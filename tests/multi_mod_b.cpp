#include <iostream>
#include <utility>

#include <OrbitSets/nomset.h>
#include <OrbitSets/rational.h>

using namespace std;

nomset<pair<rational,rational>> f();

int main() {
	nomset<pair<rational,rational>> x = nomset_product(nomset_rationals(), nomset_rationals()), y = f();
	cout << (x == y) << endl;
	return 0;
}
