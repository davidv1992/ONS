#ifndef ONS_ORBIT_TRIVIAL
#define ONS_ORBIT_TRIVIAL

#include <vector>
#include <string>
#include <cassert>

#define ORBIT_SETS_TRIVIAL_ORBIT_INSTANCE(T)                                          \
	template <>                                                                       \
	struct orbit<T> {                                                                 \
		orbit() : data() {}                                                           \
		orbit(T b) : data(b) {}                                                       \
		bool isElement(T b) const { return data == b; }                               \
		T getElement() const { return data; }                                         \
		int supportSize() const { return 0; }                                         \
		std::vector<rational> getSeqFromElement(T b) const {                          \
			(void)b;                                                                  \
			assert(isElement(b));                                                     \
			return {};                                                                \
		}                                                                             \
		T getElementFromSeq(std::vector<rational> seq) const {                        \
			(void) seq;                                                               \
			assert(seq.size() == 0);                                                  \
			return data;                                                              \
		}                                                                             \
		T data;                                                                       \
	};                                                                                \
	inline bool operator==(orbit<T> lh, orbit<T> rh) { return lh.data == rh.data; }   \
	inline bool operator!=(orbit<T> lh, orbit<T> rh) { return lh.data != rh.data; }   \
	inline bool operator< (orbit<T> lh, orbit<T> rh) { return lh.data <  rh.data; }   \
	inline bool operator> (orbit<T> lh, orbit<T> rh) { return lh.data >  rh.data; }   \
	inline bool operator<=(orbit<T> lh, orbit<T> rh) { return lh.data <= rh.data; }   \
	inline bool operator>=(orbit<T> lh, orbit<T> rh) { return lh.data >= rh.data; }

namespace ONS {
	ORBIT_SETS_TRIVIAL_ORBIT_INSTANCE(bool)
	ORBIT_SETS_TRIVIAL_ORBIT_INSTANCE(signed char)
	ORBIT_SETS_TRIVIAL_ORBIT_INSTANCE(short)
	ORBIT_SETS_TRIVIAL_ORBIT_INSTANCE(int)
	ORBIT_SETS_TRIVIAL_ORBIT_INSTANCE(long long)
	ORBIT_SETS_TRIVIAL_ORBIT_INSTANCE(unsigned char)
	ORBIT_SETS_TRIVIAL_ORBIT_INSTANCE(unsigned short)
	ORBIT_SETS_TRIVIAL_ORBIT_INSTANCE(unsigned int)
	ORBIT_SETS_TRIVIAL_ORBIT_INSTANCE(unsigned long long)
	ORBIT_SETS_TRIVIAL_ORBIT_INSTANCE(std::string)
}

#undef ORBIT_SETS_TRIVIAL_ORBIT_INSTANCE

#endif
