#include <utility>

#include <ONS/orbit.h>
#include <ONS/eqimap.h>
#include <ONS/rational.h>

using namespace std;
using namespace ONS;

int main() {
	eqimap<vector<rational>,vector<rational>> test;
	test.add(vector<rational>({rational(2), rational(1)}),vector<rational>({rational(2)}));
	
	for (auto kv : test) {
		cout << kv.first.getElement() << " " << kv.second.first.getElement() << " ";
		for (auto b : kv.second.second)
			cout << b;
		cout << endl;
	}
	return 0;
}
