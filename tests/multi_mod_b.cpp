#include <iostream>
#include <utility>

#include <ONS/nomset.h>
#include <ONS/rational.h>

using namespace std;
using namespace ONS;

nomset<pair<rational,rational>> f();

int main() {
	nomset<pair<rational,rational>> x = nomset_product(nomset_rationals(), nomset_rationals()), y = f();
	if (x == y)
		return 0;
	else
		return 1;
}
