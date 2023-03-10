
#ifndef ITERATOR_HPP
#define ITERATOR_HPP

# include <iostream>
# include "util.hpp"

namespace ft {

/* Class iterator */
template<
	class Category,
	class T,
	class Distance = std::ptrdiff_t,
	class Pointer = T*,
	class Reference = T&
> struct iterator {
	/* Member types */
	typedef T									value_type;
	typedef Category							iterator_category;
	typedef Distance							difference_type;
	typedef Pointer								pointer;
	typedef Reference							reference;
}; /* Class iterator end */

/* Implementation iterator_traits */

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template< class Iter >
struct iterator_traits {
	typedef typename Iter::difference_type		difference_type;
	typedef	typename Iter::value_type			value_type;
	typedef	typename Iter::pointer				pointer;
	typedef	typename Iter::reference			reference;
	typedef	typename Iter::iterator_category	iterator_category;
};

template< class T >
struct iterator_traits<T*> {
	typedef	typename std::ptrdiff_t				difference_type;
	typedef	T									value_type;
	typedef	T*									pointer;
	typedef	T&									reference;
	typedef ft::random_access_iterator_tag		iterator_category;
};

template< class T >
struct iterator_traits<const T*> {
	typedef	typename std::ptrdiff_t				difference_type;
	typedef	const T								value_type;
	typedef	const T*							pointer;
	typedef	const T&							reference;
	typedef ft::random_access_iterator_tag		iterator_category;
};
/* Implementation iterator_traits end */

/* Implementation reverse_iterator */
template<class Iter>
class reverse_iterator {
protected:
	/* Member objects */
	Iter current;
public:
	/* Member types */
	typedef Iter													iterator_type;
	typedef typename ft::iterator_traits<Iter>::iterator_category	iterator_category;
	typedef typename ft::iterator_traits<Iter>::value_type			value_type;
	typedef typename ft::iterator_traits<Iter>::difference_type		difference_type;
	typedef typename ft::iterator_traits<Iter>::pointer				pointer;
	typedef typename ft::iterator_traits<Iter>::reference			reference;
	/* Member functions */
						reverse_iterator() : current() {}
	explicit			reverse_iterator( iterator_type x ) : current(x) {}
	template< class U >	reverse_iterator(const reverse_iterator<U>& other ) : current(other.base()) {}
	template< class U >
	reverse_iterator&	operator=( const reverse_iterator<U>& other ) {
		current = other.base();
		return (*this);
	}
	reference			operator*() const {
		Iter tmp = current;
		return ( *(--tmp) );
	}
	pointer				operator->() const {
		Iter tmp = current;
		return ( &(*(--tmp)) );
	}
	reference			operator[]( difference_type n ) const {
		return(base()[-n-1]);
	}
	reverse_iterator& 	operator++(void) {
		--current;
		return (*this);
	}
	reverse_iterator& 	operator--(void) {
		++current;
		return (*this);
	}
	reverse_iterator 	operator++(int) {
		reverse_iterator	tmp = *this;
		++(*this);
		return (tmp);
	}
	reverse_iterator	operator--(int) {
		reverse_iterator	tmp = *this;
		--(*this);
		return (tmp);
	}
	reverse_iterator	operator+( difference_type n ) const {
		return (reverse_iterator(base()-n));
	}
	reverse_iterator	operator-( difference_type n ) const {
		return (reverse_iterator(base()+n));
	}
	reverse_iterator&	operator+=( difference_type n ) {
		current -= n;
		return (*this);
	}
	reverse_iterator&	operator-=( difference_type n ) {
		current += n;
		return (*this);
	}
	Iter				base() const { return current; }
};
/* Non-member functions for reverse_iterator */
template< class Iterator1, class Iterator2 >
bool	operator==(const ft::reverse_iterator<Iterator1>& lhs,
				   const ft::reverse_iterator<Iterator2>& rhs ) {
	return ( lhs.base() == rhs.base() );
}
template< class Iterator1, class Iterator2 >
bool	operator!=(const ft::reverse_iterator<Iterator1>& lhs,
				   const ft::reverse_iterator<Iterator2>& rhs ) {
		return ( lhs.base() != rhs.base() );
}
template< class Iterator1, class Iterator2 >
bool	operator<(const ft::reverse_iterator<Iterator1>& lhs,
				  const ft::reverse_iterator<Iterator2>& rhs ) {
	return ( lhs.base() > rhs.base() );
}
template< class Iterator1, class Iterator2 >
bool	operator<=(const ft::reverse_iterator<Iterator1>& lhs,
				   const ft::reverse_iterator<Iterator2>& rhs ) {
	return ( lhs.base() >= rhs.base() );
}
template< class Iterator1, class Iterator2 >
bool	operator>(const ft::reverse_iterator<Iterator1>& lhs,
				  const ft::reverse_iterator<Iterator2>& rhs ) {
	return ( lhs.base() < rhs.base() );
}
template< class Iterator1, class Iterator2 >
bool	operator>=(const ft::reverse_iterator<Iterator1>& lhs,
				   const ft::reverse_iterator<Iterator2>& rhs ) {
	return ( lhs.base() <= rhs.base() );
}
template< class Iter >
reverse_iterator<Iter>
		operator+(typename reverse_iterator<Iter>::difference_type n,
				  const reverse_iterator<Iter>& it ) {
		return ( reverse_iterator<Iter>(it.base() - n) );
	}
template< class Iter >
reverse_iterator<Iter>
		operator-(const reverse_iterator<Iter>& it,
				  typename reverse_iterator<Iter>::difference_type n) {
		return ( reverse_iterator<Iter>(it.base() + n) );
	}
template< class Iterator1, class Iterator2 >
typename reverse_iterator<Iterator1>::difference_type
		operator-(const reverse_iterator<Iterator1>& lhs,
				  const reverse_iterator<Iterator2>& rhs ) {
		return( rhs.base() - lhs.base() );
	}
/* Implementation reverse_iterator end */

/* Class rbtree_iterator */
template<
	typename node_type,
	typename value_type,
	bool isConst = false
> class rbtree_iterator : public ft::iterator<ft::bidirectional_iterator_tag, value_type> {
public:
	/* Member types */
	typedef node_type*													node_pointer;
	typedef ft::iterator<ft::bidirectional_iterator_tag, value_type>	bidirectional_iterator;
	typedef typename bidirectional_iterator::pointer					pointer;
	typedef typename bidirectional_iterator::reference					reference;
protected:
	typedef typename ft::remove_const<value_type>::type 				non_const_value_type;
	typedef rbtree_iterator<node_type, non_const_value_type>			non_const_iterator;

	/* Member objects */
	node_pointer			ptr;
	node_pointer			max;
public:
	/* Member functions */
							rbtree_iterator() : ptr(NULL), max(NULL) {}
							rbtree_iterator(node_pointer node, node_pointer maxNode = NULL) : ptr(node), max(maxNode) {
								if (max == NULL) max = findMax(ptr);
							}
							rbtree_iterator(const non_const_iterator& other) : ptr(other.get_ptr()), max(other.get_max()) {}
							~rbtree_iterator() {}
	node_pointer			get_ptr() const { return (this->ptr); }
	node_pointer			get_max() const { return (this->max); }
	node_pointer			findMax(node_pointer node) const {
		while (node && node->parent)
			node = node->parent;
		while (node && node->right)
			node = node->right;
		return (node);
	}
	rbtree_iterator			operator=(const non_const_iterator& other) {
		this->ptr = other.get_ptr();
		this->max = other.get_max();
		return ( *this );
	}
	reference				operator*() const {
								return (*ptr).value;
							}
	pointer					operator->() const {
								return &(ptr->value);
							}
	rbtree_iterator&		operator++(void) {
		if (ptr != NULL) {
			max = findMax(ptr);
			if (ptr->right != NULL) {
				ptr = ptr->right;
				while (ptr->left != NULL)
					ptr = ptr->left;
				return (*this);
			}
			while (ptr->parent != NULL) {
				if (ptr == ptr->parent->left) {
					ptr = ptr->parent;
					return (*this);
				}
				ptr = ptr->parent;
			}
			ptr = u_nullptr;
		}
		return (*this);
	}
	rbtree_iterator			operator++(int) {
		rbtree_iterator tmp = *this;
		++(*this);
		return ( tmp );
	}
	rbtree_iterator&		operator--(void) {
		if (ptr == u_nullptr) {
			ptr = max;
			return (*this);
		}
		if (ptr != NULL) {
			max = findMax(ptr);
			if (ptr->left != NULL) {
				ptr = ptr->left;
				while (ptr->right != NULL)
					ptr = ptr->right;
				return (*this);
			}
			while (ptr->parent != NULL) {
				if (ptr == ptr->parent->right) {
					ptr = ptr->parent;
					return (*this);
				}
				ptr = ptr->parent;
			}
			ptr = u_nullptr;
		}
		return (*this);
	}
	rbtree_iterator			operator--(int) {
		rbtree_iterator tmp = *this;
		--(*this);
		return ( tmp );
	}
	friend bool				operator==(const rbtree_iterator& lhs, const rbtree_iterator& rhs) {
		return( lhs.ptr == rhs.ptr );
	}
	friend bool				operator!=(const rbtree_iterator& lhs, const rbtree_iterator& rhs)  {
		return( !(lhs == rhs) );
	}
}; /* class rbtree_iterator end */

/* class vector_iterator */
template<
	typename value_type,
	bool isConst = false
> class vector_iterator : public ft::iterator<ft::random_access_iterator_tag,value_type> {
public:
	/* Member types */
	typedef ft::iterator<ft::random_access_iterator_tag,value_type>		random_access_iterator;
	typedef typename random_access_iterator::pointer					pointer;
	typedef typename random_access_iterator::reference					reference;
	typedef std::size_t													size_type;
	typedef std::ptrdiff_t												difference_type;
protected:
	typedef typename ft::remove_const<value_type>::type 				non_const_value_type;
	typedef vector_iterator<non_const_value_type>						non_const_iterator;

	/* Member objects */
	pointer					ptr;
public:
	/* Member functions */
							vector_iterator() : ptr(NULL) {}
							vector_iterator(pointer ptr) : ptr(ptr) {}
							vector_iterator(const non_const_iterator& other) : ptr(other.get_ptr()) {}
							~vector_iterator() {}
	pointer					get_ptr() const { return (this->ptr); }
	vector_iterator			operator=(const non_const_iterator& other ) {
		this->ptr = other.get_ptr();
		return ( *this );
	}
	reference				operator*() const { return (*ptr); }
	pointer					operator->() const { return (ptr); }
	reference				operator[](size_type n) const { return( *(*this + n) ); }
	vector_iterator&		operator++(void) { ++ptr; return (*this); }
	vector_iterator			operator++(int) {
		vector_iterator tmp = *this;
		++(*this);
		return ( tmp );
	}
	vector_iterator&		operator--(void) { --ptr; return (*this); }
	vector_iterator			operator--(int) {
		vector_iterator tmp = *this;
		--(*this);
		return (tmp);
	}
	vector_iterator&		operator+=(const difference_type n) {
		difference_type m = n;
		if (m >= 0)
			while (m--) ++*this;
		else
			while (m++) --*this;
		return (*this);
	}
	vector_iterator			operator+(const difference_type n) const {
		vector_iterator tmp = this->ptr;
		return(tmp += n);
	}
	vector_iterator&		operator-=(const difference_type n) { return (*this += -n); }
	vector_iterator			operator-(const difference_type n) const {
		vector_iterator tmp = *this;
		return(tmp -= n);
	}
	friend bool 			operator==(const vector_iterator& lhs, const vector_iterator& rhs) { return( lhs.get_ptr() == rhs.get_ptr() ); }
	friend bool 			operator!=(const vector_iterator& lhs, const vector_iterator& rhs) { return( !(lhs == rhs) ); }
	friend bool				operator<(const vector_iterator& lhs, const vector_iterator& rhs) { return (rhs.get_ptr() - lhs.get_ptr() > 0); }
	friend bool				operator>(const vector_iterator& lhs, const vector_iterator& rhs) { return ( rhs < lhs ); }
	friend bool				operator<=(const vector_iterator& lhs, const vector_iterator& rhs) { return ( !(lhs > rhs) ); }
	friend bool				operator>=(const vector_iterator& lhs, const vector_iterator& rhs) { return ( !(lhs < rhs) ); }
	
}; /* class vector_iterator end */

/* Non-member functions for vector_iterator */
template<typename value_type, bool isConst>
typename ft::vector_iterator<value_type,isConst>
		operator+(const typename ft::vector_iterator<value_type,isConst>::difference_type n,
				  const typename ft::vector_iterator<value_type,isConst>& rhs) {
	typename ft::vector_iterator<value_type,isConst> tmp(rhs);
	return(tmp += n);
}
template<typename value_type, bool isConst>
typename ft::vector_iterator<value_type>::difference_type
		operator-(const typename ft::vector_iterator<value_type,isConst>& lhs,
				  const typename ft::vector_iterator<value_type,isConst>& rhs) {
			typename ft::vector_iterator<value_type,isConst>::difference_type n = lhs.get_ptr() - rhs.get_ptr();
	return(n);
}
template<typename value_type, bool isConst>
typename ft::vector_iterator<value_type>::difference_type
		operator-(const typename ft::vector_iterator<value_type,isConst>& lhs,
				  const typename ft::vector_iterator<typename ft::remove_const<value_type>::type,!isConst>& rhs) {
			typename ft::vector_iterator<value_type,isConst>::difference_type n = lhs.get_ptr() - rhs.get_ptr();
	return(n);
}
template<typename value_type, bool isConst>
typename ft::vector_iterator<value_type>::difference_type
		operator-(const typename ft::vector_iterator<typename ft::remove_const<value_type>::type,!isConst>& lhs,
				  const typename ft::vector_iterator<value_type,isConst>& rhs) {
			typename ft::vector_iterator<value_type,isConst>::difference_type n = lhs.get_ptr() - rhs.get_ptr();
	return(n);
}

/* Implementation distance */
namespace detail {
template<class It>
typename ft::iterator_traits<It>::difference_type
		do_distance(It first, It last, ft::input_iterator_tag) {
	typename ft::iterator_traits<It>::difference_type result = 0;
	while (first != last) {
		++first;
		++result;
	}
	return result;
}
template<class It>
typename std::iterator_traits<It>::difference_type
		do_distance(It first, It last, std::input_iterator_tag) {
	typename std::iterator_traits<It>::difference_type result = 0;
	while (first != last) {
		++first;
		++result;
	}
	return result;
}
template<class It>
typename ft::iterator_traits<It>::difference_type
		do_distance(It first, It last, ft::forward_iterator_tag) {
	typename ft::iterator_traits<It>::difference_type result = 0;
	while (first != last) {
		++first;
		++result;
	}
	return result;
}
template<class It>
typename std::iterator_traits<It>::difference_type
		do_distance(It first, It last, std::forward_iterator_tag) {
	typename std::iterator_traits<It>::difference_type result = 0;
	while (first != last) {
		++first;
		++result;
	}
	return result;
}
template<class It>
typename ft::iterator_traits<It>::difference_type
		do_distance(It first, It last, ft::random_access_iterator_tag) {
	return last - first;
}
template<class It>
typename std::iterator_traits<It>::difference_type
		do_distance(It first, It last, std::random_access_iterator_tag) {
	return last - first;
}
} /* namespace detail end */
template<class It>
typename ft::iterator_traits<It>::difference_type
		distance(It first, It last) {
	return detail::do_distance(first, last,
						typename ft::iterator_traits<It>::iterator_category());
}
/* Implementation distance end */

} /* namespace ft end */

#endif /* ITERATOR_HPP */
