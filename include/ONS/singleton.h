#ifndef ONS_SINGLETON
#define ONS_SINGLETON

#include <iostream>

namespace ONS {

	class singleton {
	};

	inline std::ostream &operator<<(std::ostream &o, ONS::singleton s) {
		(void)s;// Singleton is dataless by definition so using it is meaningless
		return o << '*';
	}

}

#endif
