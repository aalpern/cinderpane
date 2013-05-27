#pragma once
#include <vector>

/// A macro to aid in defining new types which implement the
/// std::vector interface by delegating to a member container. This is
/// helpful because the standard library wasn't designed to be
/// extended via inheritance.
#define CINDERPANE_DELEGATE_VECTOR(vector_type, member)					\
	typedef vector_type                      container_type;			\
	typedef container_type::value_type       value_type;				\
	typedef container_type::size_type        size_type;					\
	typedef container_type::difference_type  difference_type;			\
	typedef container_type::allocator_type   allocator_type;			\
	typedef container_type::pointer          pointer;					\
	typedef container_type::const_pointer    const_pointer;				\
	typedef container_type::reference        reference;					\
	typedef container_type::const_reference  const_reference;			\
	typedef container_type::iterator         iterator;					\
	typedef container_type::const_iterator   const_iterator;			\
	typedef container_type::reverse_iterator reverse_iterator;			\
	typedef container_type::const_reverse_iterator const_reverse_iterator; \
																		\
	allocator_type get_allocator() const {								\
		return member.get_allocator(); }								\
																		\
	reference operator[](size_type i) { return member[i]; }				\
	const_reference operator[](size_type i) const { return member[i]; } \
	reference at(size_type i) { return member.at(i); }					\
	const_reference at(size_type i) const { return member.at(i); }		\
																		\
	reference back() { return member.back(); }							\
	const_reference back() const { return member.back(); }				\
	reference front() { return member.front(); }						\
	const_reference front() const { return member.front(); }			\
																		\
	iterator begin() { return member.begin(); }							\
	const_iterator begin() const { return member.begin(); }				\
	iterator end() { return member.end(); }								\
	const_iterator end() const { return member.end(); }					\
																		\
	reverse_iterator rbegin() { return member.rbegin(); }				\
	const_reverse_iterator rbegin() const { return member.rbegin(); }	\
	reverse_iterator rend() { return member.rend(); }					\
	const_reverse_iterator rend() const { return member.rend(); }		\
																		\
	void push_back(const_reference value) { member.push_back(value); }	\
	void pop_back() { member.pop_back(); }								\
																		\
	iterator erase(iterator where) { return member.erase(where); }		\
	iterator erase(iterator first, iterator last) {						\
		return member.erase(first, last); }								\
																		\
	void swap(vector_type &right) { member.swap(right); }				\
																		\
	void clear() { member.clear(); }									\
	bool empty() const { return member.empty(); }						\
																		\
	size_type capacity() const { return member.capacity(); }			\
	size_type size() const { return member.size(); }					\
	size_type max_size() const { return member.max_size(); }			\
	void resize(size_type size) { member.resize(size); }				\
	void resize(size_type size, value_type value) { member.resize(size, value); } \
	void reserve(size_type count) { member.reserve(count); }			\
	template<class InputIt>												\
	void insert(iterator where, InputIt first, InputIt last) {			\
		member.insert(where, first, last); }

/*
	iterator insert(iterator where, const_reference value) {			\
		return member.insert(where, value); }							\
	iterator insert(iterator where, size_type count,					\
					const_reference value) {							\
		return member.insert(where, count, value); }					\


*/
