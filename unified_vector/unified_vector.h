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
#include <algorithm>
#include <cstddef>

template<class TForType>
class unified_vector_implement_for_type {
public:

	typedef std::function<void(TForType & item)> visiting_type;

	void push_back(TForType const& item) {
		m_items.emplace_back(item);
	}

protected:
	template<class TVisitor>
	void _internal_visit(TVisitor visitor) {
		for (auto item : m_items) {
			visitor(item);
		}
	}

	template<class TTransformResult, class TVisitor>
	TTransformResult _internal_transform(TVisitor visitor) {
		TTransformResult tr;

		for (auto item : m_items) {
			tr.push_back(visitor(item));
		}

		return tr;
	}

	std::size_t _internal_size() const {
		return m_items.size();
	}

	void clear() {
		m_items.clear();
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

	template<class TTransformResult, class TVisitor>
	TTransformResult transform(TVisitor visitor) {
		TTransformResult tr;
		auto tr_thisclass =
				unified_vector_implement_for_type<T>::_internal_transform(
						visitor);

		return tr_thisclass;
	}

	size_t size() const {
		return unified_vector_implement_for_type<T>::_internal_size();
	}

	void clear () {
		unified_vector_implement_for_type<T>::clear();
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

	template<class TTransformResult, class TVisitor>
	TTransformResult transform(TVisitor /*visitor*/) {
/*		auto tr_thisclass =
				unified_vector_implement_for_type<T>::_internal_transform<
				TTransformResult TTransformResult>(visitor);

		// recurse the visitor for all other types in the unified vector
		auto tr_subclasses = unified_vector<Ts...>::transform(visitor);

		tr_thisclass.reserve(tr_subclasses.size() + tr_thisclass.size());
		tr_thisclass.insert(tr_thisclass.end(), tr_subclasses.begin(),
				tr_subclasses.end());

		return tr_thisclass;*/
		return TTransformResult();
	}

	size_t size() const {
		return unified_vector_implement_for_type<T>::_internal_size() + unified_vector<Ts...>::size();
	}

	void clear() {
		unified_vector_implement_for_type<T>::clear();
		unified_vector<Ts...>::clear();
	}
	/*

	 */
};

