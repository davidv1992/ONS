#ifndef ORBSETS_RATIONAL
#define ORBSETS_RATIONAL

#include <cstdlib>
#include <iostream>

namespace OrbitSets {
	class rational;
}

std::ostream &operator<<(std::ostream &o, OrbitSets::rational a);

namespace OrbitSets {

	// Class definition
	class rational {
	private:
		long long numerator;
		long long denominator;
	
		void normalize() {
			if (denominator < 0) {
				denominator = -denominator;
				numerator = -numerator;
			}
			long long a = abs(numerator);
			long long b = denominator;
			while (b != 0) {
				long long t = a%b;
				a = b;
				b = t;
			}
			numerator /= a;
			denominator /= a;
		}
		rational(int num, int denom) {
			numerator = num;
			denominator = denom;
			normalize();
		}
	public:
		rational() {
			numerator = 0;
			denominator = 1;
		}
		rational(int a) {
			numerator = a;
			denominator = 1;
		}

		friend bool operator==(rational a, rational b);
		friend bool operator!=(rational a, rational b);

		friend bool operator<(rational a, rational b);
		friend bool operator>(rational a, rational b);

		friend rational operator+(rational a, rational b);
		friend rational operator-(rational a, rational b);
		friend rational operator*(rational a, rational b);
		friend rational operator/(rational a, rational b);

		friend std::ostream &::operator<<(std::ostream &o, rational a);
	};

	// Equality operations
	inline bool operator==(rational a, rational b) {
		return a.numerator == b.numerator && a.denominator == b.denominator;
	}
	inline bool operator!=(rational a, rational b) {
		return a.numerator != b.numerator || a.denominator != b.denominator;
	}

	// Ordering operations
	inline bool operator<(rational a, rational b) {
		return a.numerator * b.denominator < b.numerator * a.denominator;
	}
	inline bool operator>(rational a, rational b) {
		return a.numerator * b.denominator > b.numerator * a.denominator;
	}

	// Arithmetic
	inline rational operator+(rational a, rational b) {
		return rational(a.numerator*b.denominator+b.numerator*a.denominator, a.denominator*b.denominator);
	}
	inline rational operator-(rational a, rational b) {
		return rational(a.numerator*b.denominator-b.numerator*a.denominator, a.denominator*b.denominator);
	}
	inline rational operator*(rational a, rational b) {
		return rational(a.numerator*b.numerator, a.denominator*b.denominator);
	}
	inline rational operator/(rational a, rational b) {
		return rational(a.numerator*b.denominator, a.denominator*b.numerator);
	}
}

// Output
inline std::ostream &operator<<(std::ostream &o, OrbitSets::rational a) {
	return o << a.numerator << "/" << a.denominator;
}

#endif
