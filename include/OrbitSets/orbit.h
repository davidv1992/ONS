#ifndef ORBSETS_ORBIT
#define ORBSETS_ORBIT

namespace OrbitSets {
	template<typename T>
	class orbit;
}

// Interface required
#include <vector>

// Alphabet
#include <OrbitSets/rational.h>

// Orbit types
#include <OrbitSets/impl/orbit_singleton.h>
#include <OrbitSets/impl/orbit_rational.h>
#include <OrbitSets/impl/orbit_abstract.h>
#include <OrbitSets/impl/orbit_label.h>
#include <OrbitSets/impl/orbit_ilabel.h>
#include <OrbitSets/impl/orbit_pair.h>
#include <OrbitSets/impl/orbit_variant.h>

#endif
