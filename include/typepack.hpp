#ifndef FSM_TYPELIST_HPP
#define FSM_TYPELIST_HPP

#include <type_traits>
#include <utility>

namespace tp {

  template <class T>
    struct just_type {
      using type = T;
    };

  template <class... Ts>
    struct type_pack {
      static constexpr std::size_t size = sizeof...(Ts);
    };

  using empty_pack = type_pack<>;

  template <typename TP>
    constexpr std::size_t size(TP) noexcept {
      return TP::size;
    }

  template <class... Ts>
    constexpr bool empty(type_pack<Ts...> tp) {
      return tp.size == 0;
    }

  template <class T, class... Ts>
    constexpr just_type<T> head(type_pack<T, Ts...>) {
      return {};
    }

  template <class T, class... Ts>
    constexpr type_pack<Ts...> tail(type_pack<T, Ts...>) {
      return {};
    }

  template <class T, class U>
    constexpr bool operator==(just_type<T>, just_type<U>) {
      return false;
    }

  template <class T>
    constexpr bool operator==(just_type<T>, just_type<T>) {
      return true;
    }

  template <class T, class U>
    constexpr bool operator!=(just_type<T>, just_type<U>) {
      return true;
    }

  template <class T>
    constexpr bool operator!=(just_type<T>, just_type<T>) {
      return false;
    }

  template <class... Ts, class... Us>
    constexpr bool operator==(type_pack<Ts...>, type_pack<Us...>) {
      return false;
    }

  template <class... Ts>
    constexpr bool operator==(type_pack<Ts...>, type_pack<Ts...>) {
      return true;
    }

  template <class... Ts, class... Us>
    constexpr bool operator!=(type_pack<Ts...>, type_pack<Us...>) {
      return true;
    }

  template <class... Ts>
    constexpr bool operator!=(type_pack<Ts...>, type_pack<Ts...>) {
      return false;
    }

  template <class... Ts, class U>
    constexpr auto operator+ (type_pack<Ts...>, just_type<U>) {
      return type_pack<Ts..., U> {};
    }

  template <class... Ts, class... Us>
    constexpr auto operator+ (type_pack<Ts...>, type_pack<Us...>) {
      return type_pack<Ts..., Us...> {};
    }

  template <class T, class... Ts>
    constexpr type_pack<T, Ts...> push_front(type_pack<Ts...>) {
      return {};
    }

  template <class... Ts, class T>
    constexpr type_pack<T, Ts...> push_front(type_pack<Ts...>, just_type<T>) {
      return {};
    }

  template <class T, class... Ts>
    constexpr type_pack<Ts...> pop_front(type_pack<T, Ts...>) {
      return {};
    }

  template <class T, class... Ts>
    constexpr type_pack<Ts..., T> push_back(type_pack<Ts...>) {
      return {};
    }

  template <class T, class... Ts>
    constexpr type_pack<Ts..., T> push_back(type_pack<Ts...>, just_type<T>) {
      return {};
    }

  template <class T, class... Ts>
    constexpr bool contains(type_pack<Ts...>, just_type<T>) {
      return (... || std::is_same<T, Ts>::value);
    }

  template <class T, class... Ts>
    constexpr bool contains(type_pack<Ts...> tp) {
      return contains(tp, just_type<T>{});
    }

  template <class T, class... Ts>
    constexpr std::size_t find(type_pack<Ts...> tp) {
      bool bs[] = { std::is_same<T, Ts>::value... };
      std::size_t s = size(tp);

      for (std::size_t i = 0; i != s; ++i)
        if (bs[i]) return i;
      return s;
    }

  template <template <class...> class F, class... Ts>
    struct part_caller {
      template <class... Us>
        using type = typename F<Ts..., Us...>::type;
    };

  template <template <class...> class F, class... Ts>
    constexpr std::size_t find_if(type_pack<Ts...> tp) {
      bool bs[] = { F<Ts>::value... };
      std::size_t s = size(tp);

      for (std::size_t i = 0; i != s; ++i)
        if (bs[i]) return i;
      return s;
    }

  template <template <class...> class F>
    struct value_fn {
      template <class... Ts>
        constexpr auto operator() (just_type<Ts>...) {
          return F<Ts...>::value;
        }
    };

  template <template <class...> class F>
    constexpr value_fn<F> value_fn_v;

  template <template <class...> class F>
    struct type_fn {
      template <class... Ts>
        constexpr auto operator() (just_type<Ts>...) {
          return just_type<typename F<Ts...>::type> {};
        }
    };

  template <template <class...> class F>
    constexpr type_fn<F> type_fn_v;

  template <class F, class... Ts>
    constexpr std::size_t find_if(F f, type_pack<Ts...> tp) {
      bool bs[] = { f(just_type<Ts>{})... };
      std::size_t s = size(tp);

      for (std::size_t i = 0; i != s; ++i)
        if (bs[i]) return i;
      return s;
    }

  template <class F, class... Ts>
    constexpr bool any_of(F f, type_pack<Ts...>) {
      return (... || f(just_type<Ts> {}));
    }

  template <template <class> class F, class... Ts>
    constexpr bool any_of(type_pack<Ts...>) {
      return (... || F<Ts>::value);
    }

  template <class F, class... Ts>
    constexpr bool non_of(F f, type_pack<Ts...> tp) {
      return !any_of(f, tp);
    }

  template <template <class...> class F, class... Ts>
    constexpr auto transform(type_pack<Ts...>) {
      return type_pack<typename F<Ts>::type...> {};
    }

  template <class F, class... Ts>
    constexpr auto transform(F f, type_pack<Ts...>) {
      return (... + f(just_type<Ts> {}));
    }

  template <std::size_t I, class T>
    struct indexed_type {
      static constexpr std::size_t value = I;
      using type = T;
    };

  template <class Is, class... Ts>
    struct indexed_types;

  template <std::size_t... Is, class... Ts>
    struct indexed_types<std::index_sequence<Is...>, Ts...> {
      struct type : indexed_type<Is, Ts>... {};
    };

  template <class... Ts>
    using indexed_types_for =
    typename indexed_types<std::index_sequence_for<Ts...>, Ts...>::type;

  template <std::size_t I, class T>
    constexpr just_type<T> get_indexed_type(indexed_type<I, T>) {
      return {};
    }

  template <std::size_t I, class... Ts>
    constexpr auto get(type_pack<Ts...> tp) {
      static_assert(I < tp.size);
      return get_indexed_type<I>(indexed_types_for<Ts...> {});
    }

  template <std::size_t I, class... Ts>
    constexpr auto at(type_pack<Ts...> tp) {
      return get<I>(tp);
    }

  static_assert(get<1>(type_pack<int, char, double>{}) == just_type<char>{});

  template <class... Ts>
    constexpr auto reverse(type_pack<Ts...> tp) {
      return reverse_impl(std::index_sequence_for<Ts...> {}, tp);
    }

  template <class... Ts>
    constexpr type_pack<Ts...> generate_helper(Ts*...) {
      return {};
    }

  template <class T, std::size_t... Is>
    constexpr auto generate_impl(std::index_sequence<Is...>) {
      return generate_helper(((void) Is, (T*) 0)...);
    }

  template <std::size_t I, class T>
    constexpr auto generate() {
      return generate_impl<T>(std::make_index_sequence<I> {});
    }

  template <template <class...> class F, class T>
    constexpr auto filter_one() {
      if constexpr (F<T>::value)
        return type_pack<T> {};
      else 
        return empty_pack {};
    }

  template <template <class...> class F, class... Ts>
    constexpr auto filter(type_pack<Ts...>) {
      return (empty_pack {} + ... + filter_one<F, Ts>());
    }

  template <template <class...> class F, class... Ts>
    constexpr std::size_t count(type_pack<Ts...> tp) noexcept {
      bool bs[] = { F<Ts>::value... };
      std::size_t counter = 0;
      for (std::size_t i = 0; i < tp.size; ++i)
        if (bs[i]) counter++;
      return counter;
    }

  template <template <class...> class F>
    inline constexpr std::size_t count(empty_pack) noexcept {
      return 0;
    }

  template <class T, class... Ts>
    constexpr std::size_t count(type_pack<Ts...> tp) noexcept {
      bool bs[] = { std::is_same_v<T, Ts>... };
      std::size_t counter = 0;
      for (std::size_t i = 0; i < size(tp); ++i) 
        if (bs[i]) counter++;
      return counter;
    }

  template <class T>
    inline constexpr std::size_t count(type_pack<>) noexcept {
      return 0;
    }


}

#endif
