#ifndef ORBSETS_SINGLETON
#define ORBSETS_SINGLETON

#include <iostream>

class singleton {
};

inline std::ostream &operator<<(std::ostream &o, singleton s) {
	(void)s;// Singleton is dataless by definition so using it is meaningless
	return o << '*';
}

#endif
