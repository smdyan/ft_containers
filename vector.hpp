
#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <iostream>
# include <string>
# include <sstream>
# include "iterators.hpp"
# include "util.hpp"

namespace ft {

template< class T, class Allocator = std::allocator<T> >
class vector {
public:
	typedef T											value_type;
	typedef Allocator									allocator_type;
	typedef typename Allocator::size_type				size_type;
	typedef typename Allocator::difference_type			difference_type;
	typedef value_type&									reference;
	typedef const value_type&							const_reference;
	typedef typename Allocator::pointer					pointer;
	typedef typename Allocator::const_pointer			const_pointer;
	typedef ft::vector_iterator<value_type,false>		iterator;
	typedef ft::vector_iterator<const value_type,true>	const_iterator;
	typedef ft::reverse_iterator<iterator>				reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;

protected:
	size_type		cap;
	size_type		sz;
	allocator_type	alloc;
	value_type*		arr;

public:
	vector() : cap(0), sz(0), alloc(), arr(NULL) {}
	explicit vector( const Allocator& allocator ) : cap(0), sz(0), alloc(allocator), arr(NULL) {}
	explicit vector( size_type count, const value_type& value=T(), const Allocator& allocator=Allocator())
		: cap(0), sz(0), alloc(allocator) {
	if( count > 0 )
		insert( begin(), count, value ); }

	template< class InputIt >
	vector( InputIt first, InputIt last, const Allocator& allocator = Allocator(),
			typename ft::enable_if< !ft::is_integral<InputIt>::value, InputIt >::type* = 0 )
		: cap(0), sz(0), alloc( allocator ) {
		insert(begin(), first, last);
	}

	vector( const vector& other ) 
	: cap(0), sz(0), alloc(other.alloc), arr(NULL)
	{ *this = other; }

	~vector() {
		killArr(arr, sz, cap);
		sz = 0;
		cap = 0;
	}

	vector&	operator=( const vector& other ) {
		size_type	i = 0;
		if (cap < other.sz) {
			value_type*	newArr = NULL;
			if (other.cap != 0)
				newArr = alloc.allocate(other.sz);
			try {
				for (; i < other.sz; ++i)
					alloc.construct(newArr + i, other.arr[i]);
			} catch (...) {
				for (size_type j = 0; j < i; ++j)
					alloc.destroy(newArr + j);
				alloc.deallocate(newArr, other.sz);
				throw ;
			}
			killArr(arr, sz, cap);
			arr = newArr; newArr = NULL;
			sz = other.sz;
			cap = other.sz;
		} else {
			value_type*	clone = arrCopying(cap, 0, 0);
			clear();
			try {
				for (; i < other.sz; ++i)
					alloc.construct(arr + i, other.arr[i]);
			} catch (...) {
				for (size_type j = 0; j < i; ++j)
					alloc.destroy(arr + j);
				killArr(arr, sz, cap);
				arr = clone; clone = NULL;
				throw ;
			}
			killArr(clone, sz, cap);
			sz = other.sz;
		}
		return (*this);
	}

	void	assign( size_type count, const T& value ) {
		clear();
		insert( NULL, count, value );
	}
	
	template< class InputIt >
	void	assign( InputIt first, InputIt last ) {
		clear();
		insert( NULL, first, last );
	}

	allocator_type	get_allocator() const { return( alloc ); }
	
	/* Element access */
	reference	at( size_type pos ) {
		if (pos >= sz) {
			std::ostringstream sPos; sPos << pos;
			std::ostringstream sSize; sSize << size();
			throw std::out_of_range("vector::_M_range_check: __n (which is "  +
									sPos.str() +") >= this->size() (which is " +
									sSize.str() + ")");
		}
		return arr[pos];
	}

	const_reference	at( size_type pos ) const {
		if (pos >= sz) {
			std::ostringstream sPos; sPos << pos;
			std::ostringstream sSize; sSize << size();
			throw std::out_of_range("vector::_M_range_check: __n (which is "  +
									sPos.str() +") >= this->size() (which is " +
									sSize.str() + ")");
		}
		return arr[pos];
	}
	reference 			operator[]( size_type pos ) { return (arr[pos]); }
	const_reference 	operator[]( size_type pos ) const { return (arr[pos]); }
	reference 			front() { return (arr[0]); }
	const_reference 	front() const { return (arr[0]); }
	reference 			back() { return (arr[sz - 1]); }
	const_reference 	back() const { return (arr[sz - 1]); }
	value_type* 		data() { return (arr); }
	const value_type* 	data() const { return (arr); }
	
	/* Iterators */
	iterator 				begin() { return iterator(arr); }
	const_iterator 			begin() const { return const_iterator(arr); }
	
	iterator				end() { return iterator(arr + sz); };
	const_iterator			end() const { return const_iterator(arr + sz); };
	
	reverse_iterator 		rbegin() { return reverse_iterator( end() ); }
	const_reverse_iterator 	rbegin() const { return const_reverse_iterator( end() ); }
	
	reverse_iterator		rend() { return reverse_iterator( begin() ); };
	const_reverse_iterator	rend() const { return const_reverse_iterator( begin() ); };
	
	/* Capacity */
	bool		empty() const { return( begin() == end() ); }
	size_type 	size() const { return( sz ); };
	size_type 	max_size() const { return (alloc.max_size()); }
	void		reserve( size_type capacity ) {
		if ( capacity <= cap && cap != 0 ) return ;
		size_type	i = 0;
		value_type*	newarr = alloc.allocate( capacity );
		try {
			for( ; i < sz; ++i )
				alloc.construct( newarr + i, arr[i] );
		} catch (...) {
			for( size_type j = 0; j < i; ++j )
				alloc.destroy( newarr + j );
			alloc.deallocate( newarr, capacity );
			throw ;
		}
		killArr( arr, sz, cap );
		arr = newarr;
		cap = capacity;
	}
	size_type	capacity() const { return( cap ); }
	
	/* Modifiers */
	void	clear() {
		while( sz )
			alloc.destroy( arr + --sz );
	}

	iterator	insert( iterator pos, const value_type& value ) {
		return( insert( pos, 1, value ));
	}

	iterator	insert( iterator pos, size_type count, const value_type& value ) {
		if (count == 0) return (pos);
		size_type	posNum = ( cap == 0 || pos == NULL ? 0 : posNumber(pos) );
		size_type	distance = count;
		size_type	newSize = sz + distance;
		size_type	newCapacity = ( cap * 2 < newSize || pos == NULL ? newSize : cap * 2 );
		value_type*	newArr = NULL;
		value_type*	clone = NULL;

		if (cap < newSize) {
			newArr = arrCopying( newCapacity, posNum, distance );
			size_type i = 0;
			try {
				for (; i < count; ++i)
					alloc.construct( newArr + posNum + i, value );
			} catch (...) {
				killArr( newArr, i, newCapacity );
				newArr = NULL;
				throw;
			}
			killArr( arr, sz, cap );
			arr = newArr;
			newArr = NULL;
			sz = newSize;
			cap = newCapacity;
		} 
		else {
			clone = arrCopying( cap, 0, 0 );
			try {
				shiftArr( posNum, distance );
				for( size_type i = 0; i < count; ++i )
					alloc.construct( arr + posNum + i, value );
			} catch (...) {
				killArr( arr, sz, cap );
				arr = clone; clone = NULL;
				throw;
			}
			killArr( clone, sz, cap );
			clone = NULL;
			sz = newSize;
		}
		return( iterator( arr + posNum ));
	}

	template< class InputIt >
	iterator	insert( iterator pos, InputIt first, InputIt last,
						typename ft::enable_if< !ft::is_integral< InputIt >::value, InputIt >::type* = 0 ) {
		size_type	posNum = ( cap == 0 || pos == NULL ? 0 : posNumber(pos) );
		size_type	distance = (size_type)ft::distance(first, last);
		size_type	newSize = sz + distance;
		size_type	newCapacity = ( cap * 2 < newSize || pos == NULL ? newSize : cap * 2 );
		value_type*	newArr = NULL;
		value_type*	clone = NULL;

		if (cap < newSize) {
			newArr = arrCopying( newCapacity, posNum, distance );
			size_type i = 0;
			try {
				for (; i < distance; ++i, ++first)
					alloc.construct( newArr + posNum + i, *first );
			} catch (...) {
				killArr( newArr, i, newCapacity );
				newArr = NULL;
				throw;
			}
			killArr( arr, sz, cap );
			arr = newArr;
			newArr = NULL;
			sz = newSize;
			cap = newCapacity;
		} else {
			clone = arrCopying( cap, 0, 0 );
			try {
				shiftArr( posNum, distance );
				for( size_type i = 0; i < distance; ++i, ++first )
					alloc.construct( arr + posNum + i, *first );
			} catch (...) {
				killArr( arr, sz, cap );
				arr = clone;
				clone = NULL;
				throw;
			}
			killArr( clone, sz, cap );
			clone = NULL;
			sz = newSize;
		}
		return( iterator( arr + posNum ));
	}
	
	iterator	erase( iterator pos ) {
		size_type	posNum = posNumber( pos );
		if (posNum == sz) {
			std::ostringstream sPos; sPos << posNum;
			std::ostringstream sSize; sSize << size();
			throw std::out_of_range("vector::_M_range_check: out of range");
		}
		return( erase(posNum, 1) );
	}

	iterator erase( iterator first, iterator last ) {
		size_type	firstNum = posNumber( first );
		size_type	lastNum = posNumber( last );
		if( lastNum < firstNum )
			throw std::out_of_range("vector::_M_range_check: wrong range\n");
		return( erase( firstNum, lastNum - firstNum ));
	}

	void	push_back( const value_type& value ) {
		if (cap > 0) {
			if (sz == cap)
				reserve( cap * 2 );
			alloc.construct(arr + sz, value);
			++sz;
		} else {
			arr = alloc.allocate( 1 );
			++cap;
			try {
				alloc.construct( arr, value );
				++sz;
			} catch (...) {
				alloc.deallocate( arr, 1 );
				--cap;
				throw;
			}
		}
	}

	void	pop_back() {
		if( sz > 0 ) {
			alloc.destroy( arr + sz - 1 );
			--sz;
		}
	}

	void	resize( size_type count, value_type value = value_type() ) {
		if( sz == count )
			return ;
		if( sz > count ) {
			for( size_type i = count; i < sz; ++i )
				alloc.destroy( arr + i );
			sz = count;
		} else {
			if( cap < count ) {
				size_type	i = 0;
				size_type	newCapacity = ( cap * 2 >= count ? cap * 2 : count);
				value_type*	newArr = alloc.allocate( newCapacity );
				try {
					for( ; i < sz; ++i )
						alloc.construct( newArr + i, arr[i] );
					for( ; i < count; ++i )
						alloc.construct( newArr + i, value );
				} catch (...) {
					for( size_type j = 0; j < i; ++j )
						alloc.destroy( newArr + j );
					alloc.deallocate( newArr, newCapacity );
					throw ;
				}
				killArr( arr, sz, cap );
				arr = newArr;
				newArr = NULL;
				cap = newCapacity;
				sz = count;
			} else {
				size_type	i = sz;
				value_type*	clone = arrCopying(cap, 0, 0);
				try {
					for( ; i < count; ++i )
						alloc.construct( arr + i, value );
				} catch (...) {
					for( size_type j = 0; j < i; ++j )
						alloc.destroy( arr + j );
					alloc.deallocate( arr, cap );
					arr = clone;
					clone = NULL;
					throw ;
				}
				killArr( clone, sz, cap );
				clone = NULL;
				sz = count;
			}
		}
	}

	void	swap( vector& other ) {
		value_type* arrTmp	= this->arr;
		size_type	szTmp	= this->sz;
		size_type	capTmp	= this->cap;
		this->arr	= other.arr;
		this->sz	= other.sz;
		this->cap	= other.cap;
		other.arr	= arrTmp;
		other.sz	= szTmp;
		other.cap	= capTmp;
	}
	
protected:
	
	/* Several internal functions */
	value_type*	arrCopying( size_type capacity, size_type position, size_type distance) {
		if( capacity == 0 )
			return( NULL );
		size_type	i = 0;
		value_type*	arrCopy = alloc.allocate( capacity );
		if( capacity >= sz + distance ) {
			try {
				for( ; i < position; ++i ) {
					alloc.construct( arrCopy + i, arr[i] );
				}
				i += distance;
				for( ; i < sz + distance; ++i ) {
					alloc.construct( arrCopy + i, arr[i - distance] );
				}
			} catch (...) {
				for(size_type j = 0; j < i; ++j) {
					alloc.destroy( arrCopy + j );
				}
				alloc.deallocate( arrCopy, capacity );
				arrCopy = NULL;
				throw;
			}
		}
		return( arrCopy );
	}
	void	shiftArr( size_type position, size_type distance ) {
		for( size_type i = sz; i > position; i-- ) {
			alloc.construct( arr + i + distance - 1, arr[i - 1] );
			alloc.destroy( arr + i - 1 );
		}
	}
	void	killArr( value_type* array, size_type size, size_type capacity) {
		if( capacity > 0 ) {
			for( size_type i = 0; i < size; ++i ) {
				alloc.destroy( array + i );
			}
			alloc.deallocate( array, capacity );
		}
	}
	size_type	posNumber( iterator pos ) {
		size_type	count = 0;
		iterator 	it = begin();
		iterator 	End = end();
		for( ; it != End; ++count, ++it ) {
			if( it == pos )
				return( count );
		}
		if( it == pos )
			return( count );
		throw std::out_of_range( "vector::_M_range_check: out of range" );
	}
	iterator	erase( size_type pos, size_type distance ) {
		value_type*	clone = arrCopying( cap, 0, 0 );
		try {
			for( size_type i = pos; i < sz - distance; ++i ) {
				alloc.destroy( arr + i );
				alloc.construct( arr + i, arr[i + distance] );
			}
		} catch (...) {
			killArr( arr, sz, cap );
			arr = clone;
			clone = NULL;
			throw;
		}

		killArr(clone, sz, cap);
		for( size_type i = sz - distance; i < sz + distance; ++i ) {
			alloc.destroy( arr + i );
			--sz;
		}
		return( iterator( arr + pos ));
	}

}; /* class vector end */

/* Non-member functions */

template< class T, class Alloc >
bool	operator==( const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs ) {
	return( lhs.size() == rhs.size() &&
			ft::equal( lhs.begin(), lhs.end(), rhs.begin() ) );
}

template< class T, class Alloc >
bool	operator!=( const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs ) {
	return( !(lhs == rhs) );
}

template< class T, class Alloc >
bool	operator<( const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs ) {
	return( ft::lexicographical_compare( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() ));
}

template< class T, class Alloc >
bool	operator<=(const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs ) {
	return( !(rhs < lhs) );
}

template< class T, class Alloc >
bool	operator>( const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs ) {
	return( rhs < lhs );
}

template< class T, class Alloc >
bool	operator>=( const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs ) {
	return( !(lhs < rhs) );
}
template< class T, class Alloc >
void	swap(ft::vector<T,Alloc>& lhs, ft::vector<T,Alloc>& rhs ) { lhs.swap(rhs); }

} /* namespace ft end */

#endif /* VECTOR_HPP end */
