#include <OrbitSets/rational.h>
#include <OrbitSets/variant.h>
#include <OrbitSets/singleton.h>
#include <OrbitSets/abstract.h>

using namespace std;

int main() {
	// Rationals
	rational a = 6, b = 3;
	if (a + b != rational(9) ||
		a - b != rational(3) ||
		a * b != rational(18) ||
		a / b != rational(2) ||
		a < b ||
		a == b ||
		!(a > b) ||
		!(a != b))
		return 1;
	
	// Singleton
	singleton s;
	(void)s;
	
	// Abstract
	abstract c({rational(2), rational(5)});
	
	// Variant
	variant<char, rational> d=rational(5);
	variant<char, rational> e='c';
	if (d.get<rational>() != rational(5)
		|| d.get<1>() != rational(5)
		|| d.index() != 1)
		return 1;
	if (e.get<char>() != 'c'
		|| e.get<0>() != 'c'
		|| e.index() != 0)
		return 1;
	try {
		d.get<char>();
		return 1;
	} catch (std::bad_cast x) {
		// Pass
	}
	
	return 0;
}
