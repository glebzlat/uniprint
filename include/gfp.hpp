#ifndef GET_FROM_PACK_HPP
#define GET_FROM_PACK_HPP

#include <type_traits>
#include <utility>

namespace gfp {

struct none_type {};

namespace details {

/*
 * Recursively iterates through Types and finds the Desired type in them.
 * If the Desired type found, the typedef `type` will be equal to Desired,
 * otherwise to `none_type`.
 *
 * if Desired is in Types:
 *   type = Desired (1)
 * else:
 *   type = none_type (2)
 */
template <typename Desired, typename AlwaysVoid, typename... Types>
struct find_type {};

// (1)
template <typename Desired, typename Head, typename... Tail>
struct find_type<
    Desired,
    typename std::enable_if<
        std::is_same<Desired, typename std::decay<Head>::type>::value,
        void>::type,
    Head, Tail...> {
  using type = Head;
};

// (2)
template <typename Desired> struct find_type<Desired, void> {
  using type = none_type;
};

// inductive case
template <typename Desired, typename Head, typename... Tail>
struct find_type<
    Desired,
    typename std::enable_if<
        !std::is_same<Desired, typename std::decay<Head>::type>::value,
        void>::type,
    Head, Tail...> {
  using type = typename find_type<Desired, void, Tail...>::type;
};

} // namespace details

template <class T> class get_from_pack {
private:
  template <typename... Types>
  using find_type_t = typename details::find_type<T, void, Types...>::type;

  /*
   * Recursively iterates through the pack of types and returns the Desired
   * type, if found, `none_type` otherwise. Acts similar to `find_type`,
   * but it is a function.
   */
  template <typename Head, typename... Rest>
  static auto extract(Head &&head, Rest &&...) -> typename std::enable_if<
      std::is_same<T, typename std::decay<Head>::type>::value, Head>::type {
    return head;
  }

  static none_type extract() { return {}; }

  /*
   * Inductive case: Head != T, so recursively call `extract` with the Rest.
   */
  template <typename Head, typename... Rest>
  static auto extract(Head &&, Rest &&...rest) -> typename std::enable_if<
      !std::is_same<T, typename std::decay<Head>::type>::value,
      find_type_t<Head, Rest...>>::type {
    return extract(std::forward<Rest>(rest)...);
  }

public:
  template <typename... Types>
  auto operator()(Types &&...args) -> find_type_t<Types...> {
    /*
     * `find_type` is needed due to C++11's restriction: it does not allow
     * auto type deduction. So `find_type` is used to create `find_type_t`,
     * which is used to tell the compiler the return type.
     *
     * BTW, `get_from_pack` performs two iterations through the same sequence,
     * but it seems that there is no way to avoid this in C++11.
     */
    return extract(std::forward<Types>(args)...);
  }
};

/*
 * Recursively iterates through the pack of function types Funcs searching
 * for the functor type, which argument type is the same as T. If found, calls
 * this functor. Has no "default" specialization for the case when functor not
 * found, so there will be a compilation error if type T is not accepted
 * by any of Funcs.
 *
 * Base specialization.
 * In case when type T matches F's argument type:
 * F(Targ), Targ == T,
 * call the functor and return its result.
 */
template <typename T, typename F, typename... Funcs>
auto call_match(T &&value, F &&f, Funcs &&...)
    -> decltype(f(std::forward<T>(value))) {
  return f(std::forward<T>(value));
}

/*
 * Recursive specialization.
 * When type T does not match current F's argument type:
 * F(Targ), Targ != T,
 * recursively call `call_match`.
 */
template <typename T, typename F, typename... Funcs>
auto call_match(T &&value, F &&, Funcs &&...funcs)
    -> decltype(call_match(std::forward<T>(value),
                           std::forward<Funcs>(funcs)...)) {
  return call_match(std::forward<T>(value), std::forward<Funcs>(funcs)...);
}

} // namespace gfp

#endif
