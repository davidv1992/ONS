#ifndef ORBSETS_VARIANT
#define ORBSETS_VARIANT

#include <type_traits>
#include <typeinfo>
#include <utility>
#include <cassert>
#include <iostream>

namespace OrbitSets {
	
	namespace variant_internal {
		template<typename... Ts>
		class __temparg_count;
		template<>
		class __temparg_count<> {
		public:
			static const unsigned v = 0;
		};
		template<typename T, typename... Ts>
		class __temparg_count<T,Ts...> {
		public:
			static const unsigned v = __temparg_count<Ts...>::v+1;
		};

		template<unsigned n, typename... Ts>
		class __temparg_extract;
		template<typename T, typename... Ts>
		class __temparg_extract<0, T, Ts...> {
		public:
			typedef T type;
		};
		template<unsigned n, typename T, typename... Ts>
		class __temparg_extract<n, T, Ts...> {
		public:
			typedef typename __temparg_extract<n-1, Ts...>::type type;
		};

		template<typename T, typename... Ts>
		class __temparg_index;
		template<typename T, typename... Ts>
		class __temparg_index<T, T, Ts...> {
		public:
			static const unsigned v = 0;
		};
		template<typename T1, typename T2, typename... Ts>
		class __temparg_index<T1, T2, Ts...> {
		public:
			static const unsigned v = __temparg_index<T1, Ts...>::v+1;
		};

		template<typename... Ts>
		class __type_helper;
		template<>
		class __type_helper<> {
		public:
			static void d(unsigned n, void *data) {
				(void) n;
				(void) data;
				assert(false);
			}
			static void c(unsigned n, void *dst, const void *src) {
				(void) n;
				(void) dst;
				(void) src;
				assert(false);
			}
			static void m(unsigned n, void *dst, void *src) {
				(void) n;
				(void) dst;
				(void) src;
				assert(false);
			}
			static bool eq(unsigned n, void *a, void *b) {
				(void) n;
				(void) a;
				(void) b;
				assert(false);
				return false;
			}
			static bool lt(unsigned n, void *a, void *b) {
				(void) n;
				(void) a;
				(void) b;
				assert(false);
				return false;
			}
			static std::ostream& output(unsigned n, std::ostream& o, void *data) {
				(void) n;
				(void) data;
				assert(false);
				return o;
			}
		};
		template<typename T, typename... Ts>
		class __type_helper<T, Ts...> {
		public:
			static void d(unsigned n, void *data) {
				if (n == 0) {
					reinterpret_cast<T*>(data)->~T();
				} else {
					__type_helper<Ts...>::d(n-1, data);
				}
			}
			static void c(unsigned n, void *dst, const void *src) {
				if (n == 0) {
					new (dst) T(*reinterpret_cast<const T*>(src));
				} else {
					__type_helper<Ts...>::c(n-1, dst, src);
				}
			}
			static void m(unsigned n, void *dst, void *src) {
				if (n == 0) {
					new (dst) T(std::move(*reinterpret_cast<T*>(src)));
				} else {
					__type_helper<Ts...>::m(n-1, dst, src);
				}
			}
			static bool eq(unsigned n, void *a, void *b) {
				if (n == 0) {
					return *reinterpret_cast<T*>(a) == *reinterpret_cast<T*>(b);
				} else {
					return __type_helper<Ts...>::eq(n-1, a, b);
				}
			}
			static bool lt(unsigned n, void *a, void *b) {
				if (n == 0) {
					return *reinterpret_cast<T*>(a) < *reinterpret_cast<T*>(b);
				} else {
					return __type_helper<Ts...>::lt(n-1, a, b);
				}
			}
			static std::ostream& output(unsigned n, std::ostream& o, void *data) {
				if (n == 0) {
					return o << *reinterpret_cast<T*>(data);
				} else {
					return __type_helper<Ts...>::output(n-1, o, data);
				}
			}
		};

		template<typename VT1, typename VT2, typename... Ts>
		class __extract_helper;
		template<typename VT1, typename VT2>
		class __extract_helper<VT1, VT2> {
		public:
			static VT1 extract(unsigned index, VT2 el) {
				(void) index;
				(void) el;
				assert(false);
				return VT1();
			}
		};
		template<typename VT1, typename VT2, typename T, typename... Ts>
		class __extract_helper<VT1, VT2, T, Ts...> {
		public:
			static VT1 extract(unsigned index, VT2 el) {
				if (index == 0) {
					return el.template get<T>;
				} else {
					return __extract_helper<VT1, VT2, Ts...>::extract(index-1, el);
				}
			}
		};
	}

	template<typename... Ts>
	class variant;

	template<typename... Ts>
	bool operator==(variant<Ts...> a, variant<Ts...> b) {
		if (a.current_index != b.current_index) {
			return false;
		} else {
			return variant_internal::__type_helper<Ts...>::eq(a.current_index, &(a.data), &(b.data));
		}
	}
	template<typename... Ts>
	bool operator!=(variant<Ts...> a, variant<Ts...> b) {
		return !(a == b);
	}
	template<typename... Ts>
	bool operator<(variant<Ts...> a, variant<Ts...> b) {
		if (a.current_index < b.current_index)
			return true;
		if (a.current_index > b.current_index)	
			return false;
		return variant_internal::__type_helper<Ts...>::lt(a.current_index, &(a.data), &(b.data));
	}
	template<typename... Ts>
	bool operator>(variant<Ts...> a, variant<Ts...> b) {
		return b < a;
	}

	template<typename... Ts>
	std::ostream& operator<<(std::ostream& o, variant<Ts...> a) {
		return variant_internal::__type_helper<Ts...>::output(a.index(), o, &(a.data));
	}

	template<typename TF, typename... Ts>
	class variant<TF, Ts...> {
	private:
		typename std::aligned_union<0, TF, Ts...>::type data;
		unsigned current_index;
	public:
		~variant() {
			if (current_index != variant_internal::__temparg_count<TF, Ts...>::v) {
				variant_internal::__type_helper<TF, Ts...>::d(current_index, &data);
			}
		}

		variant() {
			current_index = variant_internal::__temparg_count<TF, Ts...>::v;
		}
	
		variant(const variant<TF, Ts...>& orig) {
			current_index = variant_internal::__temparg_count<TF, Ts...>::v;
			if (orig.current_index != current_index) {
				variant_internal::__type_helper<TF, Ts...>::c(orig.current_index, &data, &(orig.data));
				current_index = orig.current_index;
			}
		}
	
		variant(variant<TF, Ts...>&& orig) {
			current_index = variant_internal::__temparg_count<TF, Ts...>::v;
			if (orig.current_index != current_index) {
				variant_internal::__type_helper<TF, Ts...>::m(orig.current_index, &data, &(orig.data));
				current_index = orig.current_index;
				orig.current_index = variant_internal::__temparg_count<TF, Ts...>::v;
			}
		}
	
		template<typename... Ts2>
		variant(variant<Ts2...> orig) {
			current_index = variant_internal::__temparg_count<TF, Ts...>::v;
			variant_internal::__type_helper<Ts2...>::c(orig.index(), orig.data)
			*this = variant_internal::__extract_helper<variant<TF, Ts...>, variant<Ts2...>, Ts2...>::extract(orig.index(), orig);
		}
	
		variant<TF, Ts...>& operator=(const variant<TF, Ts...>& orig) {
			if (current_index != variant_internal::__temparg_count<TF, Ts...>::v) {
				variant_internal::__type_helper<TF, Ts...>::d(current_index, &data);
			}
			current_index = variant_internal::__temparg_count<TF, Ts...>::v;
			if (orig.current_index != current_index) {
				variant_internal::__type_helper<TF, Ts...>::c(orig.current_index, &data, &(orig.data));
				current_index = orig.current_index;
			}
		
			return *this;
		}
	
		variant<TF, Ts...>& operator=(variant<TF, Ts...>&& orig) {
			if (current_index != variant_internal::__temparg_count<TF, Ts...>::v) {
				variant_internal::__type_helper<TF, Ts...>::d(current_index, &data);
			}
			current_index = variant_internal::__temparg_count<TF, Ts...>::v;
			if (orig.current_index != current_index) {
				variant_internal::__type_helper<TF, Ts...>::m(orig.current_index, &data, &(orig.data));
				current_index = orig.current_index;
				orig.current_index = variant_internal::__temparg_count<TF, Ts...>::v;
			}
		
			return *this;
		}
	
		template<typename T>
		variant(const T& value) {
			current_index = variant_internal::__temparg_count<TF, Ts...>::v;
			new (&data) T(value);
			current_index = variant_internal::__temparg_index<T, TF, Ts...>::v;
		}
	
		template<typename T>
		variant(typename std::enable_if<!std::is_reference<T>::value, T>::type &&value) {
			current_index = variant_internal::__temparg_count<TF, Ts...>::v;
			new (&data) T(value);
			current_index = variant_internal::__temparg_index<T, TF, Ts...>::v;
		}
	
		unsigned index() const {
			return current_index;
		}
	
		static const unsigned npos = variant_internal::__temparg_count<TF, Ts...>::v;
	
		bool valid() const {
			return current_index != npos;
		}
	
		template<unsigned n>
		typename variant_internal::__temparg_extract<n, TF, Ts...>::type& get() {
			return get<typename variant_internal::__temparg_extract<n, TF, Ts...>::type>();
		}
	
		template<typename T>
		T& get() {
			if (current_index != variant_internal::__temparg_index<T, TF, Ts...>::v)
				throw std::bad_cast();
			return *reinterpret_cast<T*>(&data);
		}
	
		template<unsigned n>
		void set(typename variant_internal::__temparg_extract<n, TF, Ts...>::type&& value) {
			set<typename variant_internal::__temparg_extract<n, TF, Ts...>::type>(std::move(value));
		}
	
		template<typename T>
		void set(T&& value) {
			if (current_index != variant_internal::__temparg_count<TF, Ts...>::v) {
				variant_internal::__type_helper<TF, Ts...>::d(current_index, &data);
			}
			current_index = variant_internal::__temparg_count<TF, Ts...>::v;
			new (&data) T(value);
			current_index = variant_internal::__temparg_index<T, TF, Ts...>::v;
		}
	
		friend bool operator== <>(variant<TF, Ts...> a, variant<TF, Ts...> b);
		friend bool operator!= <>(variant<TF, Ts...> a, variant<TF, Ts...> b);
		friend bool operator<  <>(variant<TF, Ts...> a, variant<TF, Ts...> b);
		friend bool operator>  <>(variant<TF, Ts...> a, variant<TF, Ts...> b);
		friend std::ostream& operator<< <>(std::ostream& o, variant<TF, Ts...> a);
	};

}

#endif
