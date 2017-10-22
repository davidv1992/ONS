#include <ONS/rational.h>
#include <ONS/orbit.h>
#include <ONS/nomset.h>
#include <iostream>

using namespace std;
using namespace ONS;

int main() {
	// Create the nominal set with all rational numbers.
	nomset<rational> A = nomset_rationals();

	// Create the orbit and nomset for B, and insert the orbit into the nomset.
	orbit<pair<rational,rational>> B_orbit(make_pair(rational(1), rational(2)));
	nomset<pair<rational,rational>> B; // The default set is empty.
	B.insert(B_orbit);

	// Calculate the product
	auto AtimesB = nomset_product(A,B);

	// And print elements of each orbit
	for (auto orbit : AtimesB)
		cout << orbit.getElement() << endl;
}
