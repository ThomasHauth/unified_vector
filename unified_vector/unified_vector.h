#pragma once

/*
 template <class... Ts> struct tuple {};

 template <class T, class... Ts>
 struct tuple<T, Ts...> : tuple<Ts...> {
 tuple(T t, Ts... ts) : tuple<Ts...>(ts...), tail(t) {}

 T tail;
 };
 */

#include <vector>
#include <functional>

template<class TForType>
class unified_vector_implement_for_type {
public:

	typedef std::function<void(TForType & item)> visiting_type;

	void push_back(TForType const& item) {
		m_items.emplace_back(item);
	}

	template<class TVisitor>
	void _internal_visit(TVisitor visitor) {
		for (auto item : m_items) {
			visitor(item);
		}
	}

private:
	std::vector<TForType> m_items;
};

template<class ... Ts> class unified_vector {
};

template<class T>
class unified_vector<T> : public unified_vector_implement_for_type<T> {
public:
	using unified_vector_implement_for_type<T>::push_back;

	template<class TVisitor>
	void visit(TVisitor visitor) {
		unified_vector_implement_for_type<T>::_internal_visit(visitor);

		// not recursive call here, obviously
	}
};

template<class T, class ... Ts>
class unified_vector<T, Ts...> : public unified_vector<Ts...>,
		unified_vector_implement_for_type<T> {
public:

	// using is required when overloading a method in a derived class
	// otherwise, the compiler will stop looking for methods once one
	// (even with wrong signature) with the correct mothed name is found
	using unified_vector<Ts...>::push_back;
	using unified_vector_implement_for_type<T>::push_back;

	template<class TVisitor>
	void visit(TVisitor visitor) {
		unified_vector_implement_for_type<T>::_internal_visit(visitor);

		// recurse the visitor for all other types in the unified vector
		unified_vector<Ts...>::visit(visitor);
	}
	/*

	 */
};

