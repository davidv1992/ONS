#ifndef ORBSETS_ORBIT_VARIANT
#define ORBSETS_ORBIT_VARIANT

#include <OrbitSets/variant.h>

namespace OrbitSets {
	
	namespace orbvariant_internal {
		template<typename... Ts>
		class __typename_store {};

		template<typename T1, typename T2>
		class __typename_store_append;
		template<typename T, typename... Ts>
		class __typename_store_append<T, __typename_store<Ts...>> {
		public:
			typedef __typename_store<T, Ts...> type;
		};

		template<typename T>
		class __typename_store_variant;
		template<typename... Ts>
		class __typename_store_variant<__typename_store<Ts...>> {
		public:
			typedef variant<Ts...> type;
		};

		template<typename... Ts>
		class __typename_orbitize;
		template<>
		class __typename_orbitize<> {
		public:
			typedef __typename_store<> type;
		};
		template<typename T, typename... Ts>
		class __typename_orbitize<T,Ts...> {
		public:
			typedef typename __typename_store_append<orbit<T>, typename __typename_orbitize<Ts...>::type>::type type;
		};

		template<typename orbitVariant, typename Vartype, typename... Ts2>
		class __variant_orbit_helper;
		template<typename orbitVariant, typename Vartype>
		class __variant_orbit_helper<orbitVariant, Vartype> {
		public:
			static orbitVariant createFromElement(int index, Vartype el) {
				(void) index;
				(void) el;
				assert(false);
				return orbitVariant();
			}
			static int supportSize(int index, orbitVariant o) {
				(void) index;
				(void) o;
				assert(false);
				return 0;
			}
			static Vartype getElementFromSeq(int index, orbitVariant o, std::vector<rational> seq) {
				(void) index;
				(void) o;
				(void) seq;
				assert(false);
				return Vartype();
			}
			static std::vector<rational> getSeqFromElement(int index, orbitVariant o, Vartype el) {
				(void) index;
				(void) o;
				(void) el;
				assert(false);
				return std::vector<rational>();
			}
			static Vartype getElement(int index, orbitVariant o) {
				(void) index;
				(void) o;
				assert(false);
				return Vartype();
			}
			static bool isElement(int index, orbitVariant o, Vartype el) {
				(void) index;
				(void) o;
				(void) el;
				assert(false);
				return false;
			}
		};
		template<typename orbitVariant, typename Vartype, typename T1, typename... Ts2>
		class __variant_orbit_helper<orbitVariant, Vartype, T1, Ts2...> {
		public:
			static orbitVariant createFromElement(int index, Vartype el) {
				if (index == 0) {
					return orbit<T1>(el.template get<T1>());
				} else {
					return __variant_orbit_helper<orbitVariant, Vartype, Ts2...>::createFromElement(index-1, el);
				}
			}
			static int supportSize(int index, orbitVariant o) {
				if (index == 0) {
					return o.template get<orbit<T1>>().supportSize();
				} else {
					return __variant_orbit_helper<orbitVariant, Vartype, Ts2...>::supportSize(index-1, o);
				}
			}
			static Vartype getElementFromSeq(int index, orbitVariant o, std::vector<rational> seq) {
				if (index == 0) {
					return o.template get<orbit<T1>>().getElementFromSeq(seq);
				} else {
					return __variant_orbit_helper<orbitVariant, Vartype, Ts2...>::getElementFromSeq(index-1, o, seq);
				}
			}
			static std::vector<rational> getSeqFromElement(int index, orbitVariant o, Vartype el) {
				if (index == 0) {
					return o.template get<orbit<T1>>().getSeqFromElement(el.template get<T1>());
				} else {
					return __variant_orbit_helper<orbitVariant, Vartype, Ts2...>::getSeqFromElement(index-1, o, el);
				}
			}
			static Vartype getElement(int index, orbitVariant o) {
				if (index == 0) {
					return o.template get<orbit<T1>>().getElement();
				} else {
					return __variant_orbit_helper<orbitVariant, Vartype, Ts2...>::getElement(index-1, o);
				}
			}
			static bool isElement(int index, orbitVariant o, Vartype el) {
				if (index == 0) {
					return o.template get<orbit<T1>>().isElement(el.template get<T1>());
				} else {
					return __variant_orbit_helper<orbitVariant, Vartype, Ts2...>::isElement(index-1, o, el);
				}
			}
		};
	}

	template<typename... Ts>
	bool operator==(orbit<variant<Ts...>> a, orbit<variant<Ts...>> b) {
		return a.Vorbit == b.Vorbit;
	}
	template<typename... Ts>
	bool operator!=(orbit<variant<Ts...>> a, orbit<variant<Ts...>> b) {
		return a.Vorbit != b.Vorbit;
	}
	template<typename... Ts>
	bool operator<(orbit<variant<Ts...>> a, orbit<variant<Ts...>> b) {
		return a.Vorbit < b.Vorbit;
	}
	template<typename... Ts>
	bool operator>(orbit<variant<Ts...>> a, orbit<variant<Ts...>> b) {
		return a.Vorbit > b.Vorbit;
	}
	template<typename... Ts>
	bool operator<=(orbit<variant<Ts...>> a, orbit<variant<Ts...>> b) {
		return a.Vorbit <= b.Vorbit;
	}
	template<typename... Ts>
	bool operator>=(orbit<variant<Ts...>> a, orbit<variant<Ts...>> b) {
		return a.Vorbit >= b.Vorbit;
	}

	template<typename... Ts>
	class orbit<variant<Ts...>> {
	private:
		typedef typename orbvariant_internal::__typename_orbitize<Ts...>::type orbitTypestore;
		typedef typename orbvariant_internal::__typename_store_variant<orbitTypestore>::type orbitVariant;
	
		orbitVariant Vorbit;
	public:
		int supportSize() const {
			assert(Vorbit.valid());
			return orbvariant_internal::__variant_orbit_helper<orbitVariant, variant<Ts...>, Ts...>::supportSize(Vorbit.index(), Vorbit);
		}
		variant<Ts...> getElementFromSeq(std::vector<rational> seq) const {
			assert(Vorbit.valid());
			return orbvariant_internal::__variant_orbit_helper<orbitVariant, variant<Ts...>, Ts...>::getElementFromSeq(Vorbit.index(), Vorbit, seq);
		}
		std::vector<rational> getSeqFromElement(variant<Ts...> el) const {
			assert(isElement(el));
			return orbvariant_internal::__variant_orbit_helper<orbitVariant, variant<Ts...>, Ts...>::getSeqFromElement(Vorbit.index(), Vorbit, el);
		}
		orbit() {}
		orbit(variant<Ts...> el) {
			Vorbit = orbvariant_internal::__variant_orbit_helper<orbitVariant, variant<Ts...>, Ts...>::createFromElement(el.index(), el);
		}
		template<typename T>
		orbit(orbit<T> orig) {
			Vorbit = orig;
		}
	
		variant<Ts...> getElement() const {
			assert(Vorbit.valid());
			return orbvariant_internal::__variant_orbit_helper<orbitVariant, variant<Ts...>, Ts...>::getElement(Vorbit.index(), Vorbit);
		}
		bool isElement(variant<Ts...> el) const {
			if (el.index() != Vorbit.index())
				return false;
			return orbvariant_internal::__variant_orbit_helper<orbitVariant, variant<Ts...>, Ts...>::isElement(Vorbit.index(), Vorbit, el);
		}
	
		friend bool operator== <>(orbit<variant<Ts...>> a, orbit<variant<Ts...>> b);
		friend bool operator!= <>(orbit<variant<Ts...>> a, orbit<variant<Ts...>> b);
		friend bool operator<  <>(orbit<variant<Ts...>> a, orbit<variant<Ts...>> b);
		friend bool operator>  <>(orbit<variant<Ts...>> a, orbit<variant<Ts...>> b);
		friend bool operator<= <>(orbit<variant<Ts...>> a, orbit<variant<Ts...>> b);
		friend bool operator>= <>(orbit<variant<Ts...>> a, orbit<variant<Ts...>> b);
	};

}

#endif
