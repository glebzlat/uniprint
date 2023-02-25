#ifndef GET_FROM_PACK_HPP
#define GET_FROM_PACK_HPP

#include <type_traits>
#include <utility>

namespace gfp {

  struct none_type {};

  namespace details {

    template <typename Desired, typename AlwaysVoid, typename... Types>
    struct find_type {};

    template <typename Desired, typename Head, typename... Tail>
    struct find_type<
        Desired,
        typename std::enable_if<
            std::is_same<Desired, typename std::decay<Head>::type>::value,
            void>::type,
        Head, Tail...> {
      using type = Head;
    };

    template <typename Desired>
    struct find_type<Desired, void> {
      using type = none_type;
    };

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

  template <class T>
  class get_from_pack {
  private:
    template <typename... Types>
    using find_type_t = typename details::find_type<T, void, Types...>::type;

    template <typename Head, typename... Rest>
    auto extract(Head&& head, Rest&&...) -> typename std::enable_if<
        std::is_same<T, typename std::decay<Head>::type>::value, Head>::type {
      return head;
    }

    none_type extract() {
      return {};
    }

    template <typename Head, typename... Rest>
    auto extract(Head&&, Rest&&... rest) -> typename std::enable_if<
        !std::is_same<T, typename std::decay<Head>::type>::value,
        find_type_t<Head, Rest...>>::type {
      return extract(std::forward<Rest>(rest)...);
    }

  public:
    template <typename... Types>
    auto operator()(Types&&... args) -> find_type_t<Types...> {
      return extract(std::forward<Types>(args)...);
    }
  };

  template <typename T, typename F, typename... Funcs>
  auto call_match(T&& value, F&& f, Funcs&&...)
      -> decltype(f(std::forward<T>(value))) {
    return f(std::forward<T>(value));
  }

  template <typename T, typename F, typename... Funcs>
  auto call_match(T&& value, F&&, Funcs&&... funcs)
      -> decltype(call_match(std::forward<T>(value),
                             std::forward<Funcs>(funcs)...)) {
    return call_match(std::forward<T>(value), std::forward<Funcs>(funcs)...);
  }

} // namespace gfp

#endif
