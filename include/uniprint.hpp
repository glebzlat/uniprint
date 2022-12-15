#ifndef UNIPRINT_HPP
#define UNIPRINT_HPP

#include "typepack.hpp"

#include <type_traits>
#include <ostream>

namespace uniprint {

	namespace param {

		struct param_base {};

		template <class T>
			struct print_param_base : param_base {
				print_param_base(T val) : value(val) {}
				~print_param_base() = default;

				inline T get_value() const { return value; }

				protected:
				T value;
			};

		struct print_separator : print_param_base <const char*> {
			print_separator(const char* string) : print_param_base(string) {}
		};

		struct print_endline : print_param_base <const char*> {
			print_endline(const char* string) : print_param_base(string) {}
		};

		struct print_flush : param_base {};

		struct print_file : print_param_base <std::ostream&> {
			print_file(std::ostream& os) : print_param_base(os) {}
		};

	}

	namespace impl {

		inline void get_sep_impl([[maybe_unused]] const char** target, 
				param::print_separator const& ps) {
			*target = ps.get_value();
		}

		template <typename T> void get_sep_impl(const char**, T&) {}

		template <typename... Args>
			inline const char* get_sep(const char* def, Args const&... args) {
				static_assert(
						tp::count<param::print_separator>(tp::type_pack<Args...>{}) <= 1,
						"Function expects only one parameter of this type");
				const char* result = def;
				(get_sep_impl(&result, args), ...);
				return result;
			}

		inline void get_endl_impl([[maybe_unused]] const char** target,
				param::print_endline const& pe) {
			*target = pe.get_value();
		}

		template <typename T> void get_endl_impl(const char**, T const&) {}

		template <typename... Args>
			inline const char* get_endl(const char* def, Args const&... args) {
				static_assert(
						tp::count<param::print_endline>(tp::type_pack<Args...>{}) <= 1,
						"Function expects only one parameter of this type");
				const char* result = def;
				(get_endl_impl(&result, args), ...);
				return result;
			}

		template <typename... Args>
			constexpr bool get_flush() {
				static_assert(
						tp::count<param::print_flush>(tp::type_pack<Args...>{}) <= 1,
						"Function expects only one parameter of this type");
				using argpack = tp::type_pack<Args...>;
				return tp::contains<param::print_flush>(argpack{});
			}

		inline void get_file_impl([[maybe_unused]] std::ostream** target, 
				param::print_file const& pf) {
			*target = &pf.get_value();
		}

		template <typename T> void get_file_impl(std::ostream**, T const&) {}

		template <typename... Args>
			inline std::ostream& get_file(std::ostream& def, Args const&... args) {
				static_assert(
						tp::count<param::print_file>(tp::type_pack<Args...>{}) <= 1,
						"Function expects only one parameter of this type");
				std::ostream* result = &def;
				(get_file_impl(&result, args), ...);
				return *result;
			}

		template <typename T>
			struct not_param {
				static constexpr bool value = 
					!std::is_base_of_v<param::param_base, T>;
			};

	}

	namespace options {

		using sep = param::print_separator;
		using end = param::print_endline;
		using flush = param::print_flush;
		using file = param::print_file;

	}

	template <std::ostream& Os>
		struct printer {
			private:

				static constexpr std::ostream& def_os = Os;

				static constexpr const char* def_sep = " ";
				static constexpr const char* def_endl = "\n";

				size_t last_index;
				size_t curr_index;

				template <bool DoFlush, typename T>
					inline void print_impl(T const& val, 
							[[maybe_unused]] const char* sep, std::ostream& os) {
						if constexpr (!std::is_base_of_v<param::param_base, T>) {
							os << val << (curr_index++ == last_index ? "" : sep);
							if constexpr (DoFlush) os << std::flush;
						}
					}

			public:

				template <typename... Args>
					inline void operator() (Args&&... args) {
						[[maybe_unused]]
							const char* sep = impl::get_sep(def_sep, args...);
						const char* end = impl::get_endl(def_endl, args...);
						std::ostream& os = impl::get_file(def_os, args...);
						constexpr const bool flush = impl::get_flush<Args...>();

						using argpack = tp::type_pack<Args...>;
						using tp_no_params = 
							decltype(tp::filter<impl::not_param>(argpack{}));

						last_index = tp_no_params::size - 1;
						curr_index = 0;

						(print_impl<flush>(args, sep, os), ..., 
						 print_impl<flush>("", end, os));
					}

		};

}

#endif
