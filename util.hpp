
#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>

namespace ft {

/* Implementation std::conditional_t */
template< bool B, typename T, typename F >
struct conditional { typedef T type; };

template< typename T, typename F >
struct conditional< false, T, F > { typedef F type; };
/* Implementation std::conditional_t end */

/* Implementation remove_const */
template< typename T >
struct remove_const { typedef T type; };

template< typename T >
struct remove_const <const T> { typedef T type; };
/* Implementation remove_const end */

/* Implementation u_nullptr */
static class nullptr_t {
public:
	template< class T >
	operator T*() const { return (0); }
	
	template< class C, class T >
	operator T C::*() const { return (0); }

	template< class T >
	T operator->() const { return (0); }

private:
		void operator&() const;
} u_nullptr = {};
/* Implementation u_nullptr  end */

/* Implementation enable_if */
template< bool B, class T = void >
	struct enable_if {};
	
template<class T>
	struct enable_if<true, T>{ typedef T type; };
/* Implementation enable_if end */

/* Implementation lexicographical_compare */
template< class InputIt1, class InputIt2 >
bool lexicographical_compare( InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2 ) {
	for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
	{
		if (*first1 < *first2)
			return true;
		if (*first2 < *first1)
			return false;
	}
	return (first1 == last1) && (first2 != last2);
}
template< class InputIt1, class InputIt2, class Compare >
bool lexicographical_compare( InputIt1 first1, InputIt1 last1,
							 InputIt2 first2, InputIt2 last2, Compare comp ) {
	for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
	{
		if (comp(*first1, *first2))
			return true;
		if (comp(*first2, *first1))
			return false;
	}
	return (first1 == last1) && (first2 != last2);
}
/* Implementation lexicographical_compare end */

/* Implementation equal */
template< class InputIt1, class InputIt2 >
bool equal( InputIt1 first1, InputIt1 last1, InputIt2 first2 ) {
	for (; first1 != last1; ++first1, ++first2) {
		if (!(*first1 == *first2))
			return false;
	}
	return true;
}
template< class InputIt1, class InputIt2, class BinaryPredicate >
bool equal( InputIt1 first1,
			InputIt1 last1,
			InputIt2 first2,
			BinaryPredicate p ) {
	for (; first1 != last1; ++first1, ++first2) {
		if (!p(*first1, *first2))
			return false;
	}
	return true;
}
/* Implementation equal end */

/* Implementation is_integral */
template <typename T, T v>
struct	integral_constant {
	typedef T						value_type;
	typedef integral_constant<T, v>	type;
	static const T					value = v;
	operator T() {return v;}
};
typedef ft::integral_constant<bool, true>	true_type;
typedef ft::integral_constant<bool, false>	false_type;
template <typename T> struct	is_integral : ft::integral_constant<bool, false> {};
template <> struct	is_integral<bool> : ft::integral_constant<bool, true> {};
template <> struct	is_integral<char> : ft::integral_constant<char, true> {};
template <> struct	is_integral<signed char> : ft::integral_constant<signed char, true> {};
template <> struct	is_integral<unsigned char> : ft::integral_constant<unsigned char, true> {};
template <> struct	is_integral<short> : ft::integral_constant<short, true> {};
template <> struct	is_integral<unsigned short> : ft::integral_constant<unsigned short, true> {};
template <> struct	is_integral<int> : ft::integral_constant<int, true> {};
template <> struct	is_integral<unsigned int> : ft::integral_constant<unsigned int, true> {};
template <> struct	is_integral<long> : ft::integral_constant<long, true> {};
template <> struct	is_integral<unsigned long> : ft::integral_constant<unsigned long, true> {};
template <> struct	is_integral<long long> : ft::integral_constant<long long, true> {};
template <> struct	is_integral<unsigned long long> : ft::integral_constant<unsigned long long, true> {};
template <> struct	is_integral<wchar_t> : ft::integral_constant<wchar_t, true> {};
/* Implementation is_integral end */

/* Implementation is_const */
template< class T > 
struct is_const          : ft::false_type {};

template< class T >
struct is_const< const T > : ft::true_type {};
/* Implementation is_const end */

/* Implementation is_same */
template< class T, class U >
struct is_same { static const bool	value = false; };

template< class U >
struct is_same<U, U> { static const bool	value = true; };
/* Implementation is_same end */

/* Implementation std::pair */
template< class T1, class T2 >
struct pair {
	/* Member types */
	typedef T1	first_type;
	typedef T2	second_type;
	/* Member objects */
	first_type	first;
	second_type	second;
	/* Member functions */
				pair() : first(), second() {}
				pair( const T1& x, const T2& y ) : first(x), second(y) {}
	template< class U1, class U2 >
				pair( const pair<U1,U2>& p ) : first(p.first), second(p.second) {}
	template< class U1, class U2 >
				pair& operator=( const pair<U1, U2>& other )  {
			this->first = other.first;
			this->second = other.second;
		return (*this);
	}
}; /* struct pair end */
/* Non-member functions */
template< class T1, class T2 >
std::ostream&		operator<<(std::ostream& o, ft::pair<T1,T2>& pair) {
	o << pair.first << "-" << pair.second;
	return o;
}

template< class T1, class T2 >
pair< T1,T2 >	make_pair(T1 x, T2 y) {
	return (pair<T1,T2>(x, y));
}
template< class T1, class T2 >
bool	operator==( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs )
{ return ( lhs.first == rhs.first && lhs.second == rhs.second ); }

template< class T1, class T2 >
bool	operator!=( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs )
{ return ( !(rhs == lhs) ); }

template< class T1, class T2 >
bool	operator<( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs )
{ return ( (lhs.first < rhs.first) || ((lhs.first == rhs.first) && (lhs.second < rhs.second)) ); }

template< class T1, class T2 >
bool	operator<=( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs )
{ return (!(rhs < lhs)); }

template< class T1, class T2 >
bool	operator>( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs )
{ return ( rhs < lhs ); }

template< class T1, class T2 >
bool	operator>=( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs )
{ return ( !(lhs < rhs) ); }
/* Implementation std::pair end */

class bad_array_new_length : public std::exception
{
public:
	bad_array_new_length() {}
	const char	*what() const throw() {
		return "bad array new length";
	};
};

} /* namespace ft end */

#endif /* UTILS_HPP */
