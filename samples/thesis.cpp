#include <ONS/rational.h>
#include <ONS/orbit.h>
#include <ONS/nomset.h>
#include <iostream>

using namespace std;
using namespace ONS;

int main() {
	// Create the universe of pairs
	nomset<pair<rational,rational> > U = nomset_product(nomset_rationals(), nomset_rationals());
	
	// Filter out those pairs where a<= b, and those where a>= b
	nomset<pair<rational,rational> > A = nomset_filter(U, [](pair<rational,rational> p){return p.first <= p.second;});
	nomset<pair<rational,rational> > B = nomset_filter(U, [](pair<rational,rational> p){return p.first >= p.second;});
	
	// And calculate the intersection
	nomset<pair<rational,rational> > I = nomset_intersect(A,B);
	
	for (auto orbit : I)
		cout << orbit.getElement() << endl;
}
