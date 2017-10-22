#ifndef ONS_ORBIT
#define ONS_ORBIT

namespace ONS {
	template<typename T>
	class orbit;
}

// Interface required
#include <vector>

// Alphabet
#include <ONS/rational.h>

// Orbit types
#include <ONS/impl/orbit_singleton.h>
#include <ONS/impl/orbit_rational.h>
#include <ONS/impl/orbit_abstract.h>
#include <ONS/impl/orbit_label.h>
#include <ONS/impl/orbit_ilabel.h>
#include <ONS/impl/orbit_pair.h>
#include <ONS/impl/orbit_variant.h>
#include <ONS/impl/orbit_vector.h>

#endif
