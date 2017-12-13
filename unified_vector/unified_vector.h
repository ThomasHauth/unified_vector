

/*
template <class... Ts> struct tuple {};

template <class T, class... Ts>
struct tuple<T, Ts...> : tuple<Ts...> {
  tuple(T t, Ts... ts) : tuple<Ts...>(ts...), tail(t) {}

  T tail;
};
*/

#include <vector>

template <class ... Ts > class unified_vector{};

template <class T>
class unified_vector<T> { 
public:
    void push_back( T const& item ) {
        m_items.emplace_back( item );
    }

private:
    std::vector<T> m_items;
};

template <class T, class... Ts>
class unified_vector<T, Ts...> : public unified_vector<Ts...> { 
public:

    // using is required when overloading a method in a derived class
    // otherwise, the compiler will stop looking for methods once one
    // (even with wrong signature) with the correct mothed name is found
    using unified_vector<Ts...>::push_back;

    void push_back( T const& item ) {
        m_items.emplace_back( item );
    }

private:
    std::vector<T> m_items;
};


