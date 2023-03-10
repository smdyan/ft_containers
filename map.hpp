
#ifndef MAP_HPP
# define MAP_HPP

# include <iostream>
# include "RBTree.hpp"
# include "iterators.hpp"
# include "util.hpp"

namespace ft {

template< class Key, class T, class Compare = std::less<Key>, 
			class Allocator=std::allocator<ft::pair<const Key, T> > >
class map {
	/* Member types */
	typedef typename ft::RBNode< ft::pair<const Key, T> >			node_type;
	typedef typename Allocator::template rebind<node_type>::other	node_allocator;
	typedef node_type*												node_pointer;
	typedef node_type&												node_reference;

public:
	typedef typename ft::remove_const<Key>::type					key_type;
	typedef T														mapped_type;
	typedef ft::pair<const key_type, mapped_type>					value_type;
	typedef typename Allocator::size_type							size_type;
	typedef typename Allocator::difference_type						difference_type;
	typedef Compare													key_compare;
	typedef Allocator												allocator_type;
	typedef value_type&												reference;
	typedef const value_type&										const_reference;
	typedef typename Allocator::pointer								pointer;
	typedef typename Allocator::const_pointer						const_pointer;
	typedef ft::rbtree_iterator<node_type,value_type,false>			iterator;
	typedef ft::rbtree_iterator<node_type,const value_type,true>	const_iterator;
	typedef ft::reverse_iterator<iterator>							reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>					const_reverse_iterator;
	typedef ft::pair<iterator,bool>									iterator_bool;

	/* Member classes */
	class value_compare {
		key_compare		comp;
	public:
				value_compare(const Compare& comp = Compare()) : comp(comp) {}
				~value_compare() {}
		bool	operator()(pointer x, pointer y) const {
			return ( comp(x->first, y->first) ); }
		bool	operator()(const value_type& x, const value_type& y) const {
			return ( comp(x.first, y.first) ); }
		bool	operator()(const value_type& x, pointer y) const {
			return ( comp(x.first, y->first) ); }
		bool	operator()(pointer x, const value_type& y) const {
			return ( comp(x->first, y.first) ); }
	}; /* Class value_compare end */
	
protected:
	/* Member types */
	typedef ft::RBTree<value_type,value_compare,node_allocator>		tree_type;
	/* Member objects */
	key_compare		k_comp;
	value_compare	compare;
	allocator_type	allocator;
	node_allocator	node_alloc;
	tree_type		tree;

public:
	/* Member functions */
	map():k_comp(), compare(k_comp), allocator(), node_alloc(), tree(compare, node_alloc){}
	explicit	map( const Compare& comp, const Allocator& alloc = Allocator() )
		: k_comp(comp), compare(k_comp), allocator(alloc), node_alloc(), tree(compare, node_alloc) {}
	
	template<class InputIt>
	map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator(),
			typename ft::enable_if< !ft::is_integral<InputIt>::value, InputIt >::type* = 0 )
			: k_comp(comp), compare(k_comp), allocator(alloc), node_alloc(), tree(compare, node_alloc) {
				for (; first != last; ++first)
				insert( value_type(first->first,first->second) ); }

	map( const map& other ) { *this = other; }
	~map() { clear(); }
	map&	operator=( const map& other ) {
		if ( other.size() >= tree.max_size() )
			throw ft::bad_array_new_length();
		tree_type	clone_tree = tree_type(compare, allocator);
		try {
			node_pointer node = other.tree.findMin();
			while (node != NULL) {
				clone_tree.insertNode(node->value);
				node = other.tree.findNextNode(node);
			}
		}
		catch (...) {
			clone_tree.clear();
			throw ;
		}
		tree.clear();
		tree.set_root( clone_tree.get_root() );
		clone_tree.set_root( NULL );
		tree.set_size( clone_tree.get_size() );
		clone_tree.set_size( 0 );
		return( *this );
	}
	allocator_type	get_allocator() const { return ( allocator ); }

	/* Element access */
	mapped_type&	at( const key_type& key ) {
		node_pointer node = findKey( key );
		if( node == NULL )
			throw std::out_of_range( "map::at:  key not found" );
		return( node->value.second );
	}
	const mapped_type&	at( const key_type& key ) const {
		node_pointer node = findKey( key );
		if( node == NULL )
			throw std::out_of_range( "ft::map::at:  key not found" );
		return( node->value.second );
	}
	mapped_type&	operator[]( const key_type& key ) {
		node_pointer node = findKey( key );
		if( node == NULL )
			node = tree.insertNode( value_type( key, mapped_type() ));
		return( node->value.second );
	}
	
	/* Iterators */
	
	iterator 				begin() { return iterator( tree.findMin() ); }
	const_iterator 			begin() const { return const_iterator( tree.findMin() ); }

	iterator				end() { return iterator( u_nullptr, tree.findMax() ); };
	const_iterator			end() const { return const_iterator( u_nullptr, tree.findMax() ); };

	reverse_iterator 		rbegin() { return reverse_iterator( end() ); }
	const_reverse_iterator 	rbegin() const { return const_reverse_iterator( end() ); }

	reverse_iterator		rend() { return reverse_iterator( begin() ); };
	const_reverse_iterator	rend() const { return const_reverse_iterator( begin() ); };

	/* Capacity */
	bool			empty( void ) const { return ( begin() == end() ); }
	size_type 		size( void ) const { return tree.get_size() ; }
	size_type 		max_size( void ) const { return ( tree.max_size() ); }

	/* Modifiers */
	void 			clear( void ) { tree.clear(); }
	iterator_bool	insert( const value_type& value ) {
		node_pointer	node = findKey( value.first );
		bool			is_inserted = (node == NULL ? true : false);
		if( is_inserted == true )
			node = tree.insertNode( value );
		return (iterator_bool( iterator(node, tree.findMax()), is_inserted ));
	}
	iterator 		insert( iterator pos, const value_type& value ) {
		(void)pos;
		return ( insert(value).first );
	}
	template< class InputIt >
	void	insert( InputIt first, InputIt last ) {
		for( ; first != last; ++first )
			insert( value_type(first->first,first->second) );
	}
	iterator	erase( iterator pos ) {
		if ( pos != end() ) {
			erase(pos++->first);
		}
		return( pos );
	}
	iterator	erase( iterator first, iterator last ) {
		while( first != last )
			erase( first++ );
		return ( first );
	}
	size_type	erase( const key_type& key ) {
		size_type s = size();
		node_pointer node = findKey(key);
		if (node != NULL)
			tree.deleteNode( node );
		return( s - size() );
	}
	void	swap( map& other ) {
		node_pointer	tmp = tree.get_root();
		tree.set_root(other.tree.get_root());
		other.tree.set_root(tmp);
		size_type	s = tree.get_size();
		tree.set_size(other.tree.get_size());
		other.tree.set_size(s);
	}
	
	/* Lookup */
	size_type	count( const key_type& key ) const { return !(findKey( key ) == NULL); }
	iterator 	find( const key_type& key ) {
		node_pointer node = findKey( key );
		if (node == NULL)
			return( end() );
		return( iterator( node, tree.findMax() ));
	}
	const_iterator 	find( const key_type& key ) const {
		node_pointer node = findKey( key );
		if (node == NULL)
			return( end() );
		return( const_iterator(node, tree.findMax()) );
	}

	ft::pair<iterator,iterator> equal_range( const key_type& key ) {
		return ft::pair<iterator,iterator>( lower_bound(key),upper_bound(key) ); }
	
	ft::pair<const_iterator,const_iterator> equal_range( const key_type& key ) const {
			return ft::pair<const_iterator,const_iterator>( lower_bound(key),upper_bound(key) );}

	iterator	lower_bound( const key_type& key) {
		node_pointer node = tree.findMin();
		while( node != NULL && k_comp( node->value.first, key ))
			node = tree.findNextNode(node);
		if( node == NULL )
			return( end() );
		return( iterator(node, tree.findMax()) );
	}

	const_iterator	lower_bound( const key_type& key ) const {
		node_pointer node = tree.findMin();
		while( node != NULL && k_comp( node->value.first, key ))
			node = tree.findNextNode( node );
		if( node == NULL )
			return( end() );
		return( const_iterator(node, tree.findMax()) );
	}

	iterator	upper_bound( const key_type& key ) {
		node_pointer node = tree.findMin();
		while( node != NULL && !k_comp( key, node->value.first ))
			node = tree.findNextNode(node);
		if( node == NULL )
			return( end() );
		return ( iterator(node, tree.findMax()) );
	}

	const_iterator	upper_bound( const key_type& key ) const {
		node_pointer node = tree.findMin();
		while (node != NULL && !k_comp(key, node->value.first))
			node = tree.findNextNode(node);
		if( node == NULL )
			return( end() );
		return( const_iterator(node, tree.findMax()) );
	}
	
	/* Observers */
	key_compare 	key_comp() const { return (k_comp); }
	value_compare 	value_comp() const { return (compare); };

//protected:
	/* Several internal functions */
	void			printIteratively(void) { tree.printTreeIteratively(false); }
	void			printRecursively(void) { tree.printTreeRecursively(); }
	node_pointer	findKey( const key_type& key ) const {
		value_type value( key, mapped_type() );
		return tree.findNode(value);
	}
}; /* class map end */

/* Map non-member functions */
template< class Key, class T, class Compare, class Alloc >
bool	operator==(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
		if (lhs.size() == rhs.size())
			return ( equal(lhs.begin(), lhs.end(), rhs.begin()) );
		return( false );
	}

template< class Key, class T, class Compare, class Alloc >
bool operator!=(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
	return( !(lhs == rhs) ); }

template< class Key, class T, class Compare, class Alloc >
bool operator<(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
	return( lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) ); }

template< class Key, class T, class Compare, class Alloc >
bool operator<=(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
	return( !(rhs < lhs) ); }

template< class Key, class T, class Compare, class Alloc >
bool operator>(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
	return( rhs < lhs ); }

template< class Key, class T, class Compare, class Alloc >
bool operator>=(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
	return( !(lhs < rhs) ); }

template< class Key, class T, class Compare, class Alloc >
void swap(map<Key,T,Compare,Alloc>& lhs, map<Key,T,Compare,Alloc>& rhs ) {
	lhs.swap(rhs); }

} /* namespace ft end */

#endif /* MAP_HPP end */
