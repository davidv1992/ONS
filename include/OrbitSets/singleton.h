#ifndef ORBSETS_SINGLETON
#define ORBSETS_SINGLETON

#include <iostream>

namespace OrbitSets {

	class singleton {
	};

	inline std::ostream &operator<<(std::ostream &o, OrbitSets::singleton s) {
		(void)s;// Singleton is dataless by definition so using it is meaningless
		return o << '*';
	}

}

#endif
