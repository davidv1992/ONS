#include <ONS/rational.h>
#include <ONS/orbit.h>
#include <ONS/nomset.h>
#include <iostream>

using namespace std;
using namespace ONS;

int main() {
	nomset<rational> A = nomset_rationals();
	nomset<pair<rational, rational>> B({rational(1),rational(2)});

	auto AtimesB = nomset_product(A, B);   // compute the product
	for (auto orbit : AtimesB)
		cout << orbit.getElement() << endl;
}
