
#ifndef RBTREE_HPP
# define RBTREE_HPP

# include <iostream>
# include "util.hpp"

#define REDC		"\033[31m"
#define YELLOW		"\033[1;33m"
#define BLUE		"\033[1;34m"
#define GREEN		"\033[1;32m"
#define GRAY		"\033[2m"
#define DEFAULT		"\033[0m"

# define RED	false
# define BLACK	true
# define LEFT	false
# define RIGHT	true
# define INDENT	"\033[2m:   \033[0m"

namespace ft {

template<typename T>
struct RBSize {
	typedef T		size_type;
	size_type		size, nodes, reds, blacks;
	bool			is_unsorted;
					RBSize()
					:
						size(0),
						nodes(0),
						reds(0),
						blacks(0),
						is_unsorted(false)
					{}
					~RBSize() {}
}; /* struct Size end */

template<typename T>
std::ostream& operator<<(std::ostream& o, ft::RBSize<T>& size) {
//	o 	<< GRAY <<	"Size: " << size.size << ". "
	o	<< GRAY << size.nodes << " nodes, "
		<< size.reds << " REDs, "
		<< size.blacks << " BLACKs. "
		<< (size.is_unsorted ? "Tree is unsorted" : "Tree is sorted. ")
		<< DEFAULT;
	return o;
}

template< typename T >
struct RBNode {
	
	/* Member types */
	typedef T		value_type;
	typedef RBNode	node_type;
	typedef RBNode*	node_pointer;
	typedef RBNode&	node_reference;

	/* Member objects */
	value_type		value;
	bool			color;
	node_pointer	parent, left, right;
	
	/* Member functions */
					RBNode()
					:
						value(),
						color(RED),
						parent(NULL),
						left(NULL),
						right(NULL)
					{}
					~RBNode() {};
					RBNode(
						const value_type& value,
						bool color = RED,
						node_pointer parent = NULL,
						node_pointer left = NULL,
						node_pointer right = NULL
						   )
					:
						value(value),
						color(color),
						parent(parent),
						left(left),
						right(right)
					{}
					RBNode (const RBNode& node)
					:
						value(node.value),
						color(node.color),
						parent(node.parent),
						left(node.left),
						right(node.right)
					{}
	node_reference	operator=(const RBNode& other) {
		this->value = other.value;
		this->color = other.color;
		this->parent = other.parent;
		this->left = other.left;
		this->right = other.right;
		return (*this);
	}

}; /* struct RBNode end */

template<typename T>
std::ostream&		operator<<(std::ostream& o, ft::RBNode<T>& node) {
	o 	<< (node.color == RED ? REDC : GRAY)
		<< node.value
		<< DEFAULT;
	return o;
}

template<
	class T,
	class Compare = std::less<T>,
	typename Allocator = std::allocator<RBNode<T> >
> class RBTree {
public:
	/* Member types */
	typedef T								value_type;
	typedef value_type*						value_pointer;
	typedef const value_type&				value_reference;
	typedef Compare							compare_type;
	typedef RBNode<value_type>				node_type;
	typedef node_type*						node_pointer;
	typedef node_type&						node_reference;
	typedef Allocator						allocator_type;
	typedef typename Allocator::size_type	size_type;
	typedef RBSize<size_type>				Size_type;
	
private:
	/* Member objects */
	compare_type							compare;
	allocator_type							allocator;
	node_pointer							root;
	Size_type								size;

public:
	/* Member functions */
					RBTree(const Compare& comp = Compare(),
						   const Allocator& alloc = Allocator() )
					:
						compare(comp),
						allocator(alloc),
						root(NULL)
					{}
					~RBTree() { clear(); }
	node_pointer	get_root() const { return (root); }
	void 			set_root(node_pointer r) { root = r; }
	size_type 		get_size() const { return (size.size); }
	void 			set_size(size_type s) { size.size = s; }
	size_type 		max_size() const { return allocator.max_size(); }
	allocator_type	get_allocator() const { return (allocator); }
	node_pointer	insertNode(const value_type& value) {
		if ( size.size >= allocator.max_size() )
			throw ft::bad_array_new_length();
		if ( root != NULL ) {
			bool child = RIGHT;
			node_pointer node = root, parent = NULL;
			while (node != NULL) {
				if ( !compare(value, node->value) && !compare(node->value, value) )
					return (NULL);
				parent = node;
				child = (compare(value, node->value) ? LEFT : RIGHT);
				node = (child == LEFT ? node->left : node->right);
			}
			node = allocator.allocate(1);
			try { allocator.construct(node, node_type(value, RED, parent));
			} catch (...) { allocator.deallocate(node, 1); throw ; }
			child == LEFT ? parent->left = node : parent->right = node;
			size.size++;
			insertCase1(node);
			return (node);
		} else {
			root = allocator.allocate(1);
			try { allocator.construct(root, node_type(value, BLACK));
			} catch (...) { allocator.deallocate(root, 1); root = NULL; throw ; }
			size.size++;
			return (root);
		}
	}
	void			deleteNode(node_pointer node) {
		node_pointer node_to_swap = NULL;
		if (node != NULL) {
			if (node->left != NULL) {
				node_to_swap = findMaxNode(node->left);
				swapNodes(node, node_to_swap);
				deleteOneChild(node);
			} else if (node->right != NULL) {
				node_to_swap = findMinNode(node->right);
				swapNodes(node, node_to_swap);
				deleteOneChild(node);
			} else
				deleteOneChild(node);
		}
	}
	void			deleteNode(const value_type& value) {
		deleteNode( findNode(value) );
	}
	void			clear(void) {
		while (size.size)
				deleteNode(findMin());
	}
	node_pointer	findNode(const value_type& value) const {
		node_pointer node = root;
		while (node != NULL) {
			if ( !compare(value, node->value) && !compare(node->value, value) )
				break ;
			else if ( compare(value, node->value) )
				node = node->left;
			else
				node = node->right;
		}
		return (node);
	}
	node_pointer	findMin() const { return findMinNode(root); }
	node_pointer	findMax() const { return findMaxNode(root); }
	node_pointer	findNextNode(node_pointer node) const {
		if (node != NULL) {
			if (node->right != NULL)
				return (findMinNode(node->right));
			while (node->parent != NULL) {
				if (node == node->parent->left)
					return (node->parent);
				node = node->parent;
			}
		}
		return (NULL);
	}
	node_pointer	findPreviousNode(node_pointer node) const {
		if (node != NULL) {
			if (node->left != NULL)
				return (findMaxNode(node->left));
			while (node->parent != NULL) {
				if (node == node->parent->right)
					return (node->parent);
				node = node->parent;
			}
		}
		return (NULL);
	}
private:
	void			deleteOneChild(node_pointer node) {
		/* Condition of the function: node has at most one non-zero child. */
		if (node->left == NULL && node->right == NULL) {
			if (node->parent == NULL)		/* Tree is empty after deletion */
				root = NULL;
			else {
				if (node == node->parent->left)
					node->parent->left = NULL;
				else
					node->parent->right = NULL;
				deleteCase1(node->parent);
			}
		}
		else {
			node_pointer child = ( node->right == NULL ?
								  node->left : node->right );
			replaceNode(node, child);
			if (node->color == BLACK) {
				if (child->color == RED)
					child->color = BLACK;
				else
					deleteCase1(child);
			}
		}
		allocator.destroy(node);
		allocator.deallocate(node, 1);
		size.size--;
	}
	void			replaceNode(node_pointer node, node_pointer child) {
		child->parent = node->parent;
		if (node == node->parent->left)
			node->parent->left = child;
		else
			node->parent->right = child;
	}
	void			swapNodes(node_pointer node, node_pointer node_to_swap) {
		bool			color = node->color;
		node_pointer	parent = node->parent;
		node_pointer	left = node->left;
		node_pointer	right = node->right;
		/* swap colors node and node_to_swap */
		node->color = node_to_swap->color;
		node_to_swap->color = color;
		/* swap parents outside node and node_to_swap */
		if (node->parent == NULL)
			root = node_to_swap;
		else if (node == node->parent->left)
			node->parent->left = node_to_swap;
		else
			node->parent->right = node_to_swap;
		if (node_to_swap == node_to_swap->parent->left)
			node_to_swap->parent->left = node;
		else
			node_to_swap->parent->right = node;
		/* swap childrens outside with childlessness check */
		if (node->left != NULL) node->left->parent = node_to_swap;
		if (node->right != NULL) node->right->parent = node_to_swap;
		if (node_to_swap->left != NULL) node_to_swap->left->parent = node;
		if (node_to_swap->right != NULL) node_to_swap->right->parent = node;
		/* swap parents and childrens inside
		** with a check if node_to_swap is a child of node */
		if (node == node_to_swap->parent) {
			node->parent = node_to_swap;
			node_to_swap->parent = parent;
			node->left = node_to_swap->left;
			node->right = node_to_swap->right;
			if (node_to_swap == left) {
				node_to_swap->left = node;
				node_to_swap->right = right;
			} else {
				node_to_swap->right = node;
				node_to_swap->left = left;
			}
		} else {
			node->parent = node_to_swap->parent;
			node_to_swap->parent = parent;
			node->left = node_to_swap->left;
			node_to_swap->left = left;
			node->right = node_to_swap->right;
			node_to_swap->right = right;
		}
	}
	node_pointer	grandparent(node_pointer node) {
		if (node != NULL && node->parent != NULL)
			return (node->parent->parent);
		return (NULL);
	}
	node_pointer	uncle(node_pointer node) {
		node_pointer grandpa = grandparent(node);
		if (grandpa == NULL) /* No grandparent means no uncle */
			return (NULL);
		if (node->parent == grandpa->left)
			return (grandpa->right);
		else
			return (grandpa->left);
	}
	node_pointer	sibling(node_pointer node) {
		if (node != NULL && node->parent != NULL) { /* No parent - no brother (root) */
			if (node == node->parent->left)
				return node->parent->right;
			else
				return node->parent->left;
		}
		return (NULL);
	}
	node_pointer	findMaxNode(node_pointer node) const {
		while (node != NULL && node->right != NULL)
			node = node->right;
		return (node);
	}
	node_pointer	findMinNode(node_pointer node) const {
		while (node != NULL && node->left != NULL)
			node = node->left;
		return (node);
	}
	void			rotateLeft(node_pointer node) {
		node_pointer	pivot = node->right;
		
		pivot->parent = node->parent; /* perhaps the pivot becomes the root of the tree */
		if (node->parent != NULL) {
			if (node->parent->left == node)
				node->parent->left = pivot;
			else
				node->parent->right = pivot;
		} else
			root = pivot;
		node->right = pivot->left;
		if (pivot->left != NULL)
			pivot->left->parent = node;
		node->parent = pivot;
		pivot->left = node;
	}
	void			rotateRight(node_pointer node) {
		node_pointer	pivot = node->left;
		
		pivot->parent = node->parent; /* perhaps the pivot becomes the root of the tree */
		if (node->parent != NULL) {
			if (node->parent->left == node)
				node->parent->left = pivot;
			else
				node->parent->right = pivot;
		} else
			root = pivot;
		node->left = pivot->right;
		if (pivot->right != NULL)
			pivot->right->parent = node;
		node->parent = pivot;
		pivot->right = node;
	}
	void			insertCase1(node_pointer node) {
		if (node->parent == NULL)
			node->color = BLACK;
		else
			insertCase2(node);
	}
	void			insertCase2(node_pointer node) {
		if (node->parent->color == BLACK)
			return;
		else
			insertCase3(node);
	}
	void			insertCase3(node_pointer node) {
		node_pointer	uncl = uncle(node);
		node_pointer	grandpa = grandparent(node);

		if ((uncl != NULL) && (uncl->color == RED)) {
			node->parent->color = BLACK;
			uncl->color = BLACK;
			grandpa->color = RED;
			insertCase1(grandpa);
		} else {
			insertCase4(node);
		}
	}
	void			insertCase4(node_pointer node) {
		node_pointer	grandpa = grandparent(node);

		if ((node == node->parent->right) && (node->parent == grandpa->left)) {
			rotateLeft(node->parent);
			node = node->left;
		} else if ((node == node->parent->left) && (node->parent == grandpa->right)) {
			rotateRight(node->parent);
			node = node->right;
		}
		insertCase5(node);
	}
	void			insertCase5(node_pointer node) {
		node_pointer	grandpa = grandparent(node);

		node->parent->color = BLACK;
		grandpa->color = RED;
		if ((node == node->parent->left) && (node->parent == grandpa->left)) {
			rotateRight(grandpa);
		} else {
			rotateLeft(grandpa);
		}
	}
	void			deleteCase1(node_pointer node) {
		if (node->parent != NULL)
			deleteCase2(node);
	}
	void			deleteCase2(node_pointer node) {
		node_pointer	siblng = sibling(node);

		if (siblng != NULL && siblng->color == RED) {
			node->parent->color = RED;
			siblng->color = BLACK;
			if (node == node->parent->left)
				rotateLeft(node->parent);
			else
				rotateRight(node->parent);
		}
		deleteCase3(node);
	}
	void			deleteCase3(node_pointer node) {
		node_pointer	siblng = sibling(node);

		if ( siblng != NULL &&
			(node->parent->color == BLACK) &&
			(siblng->color == BLACK) &&
			(siblng->left == NULL || siblng->left->color == BLACK) &&
			(siblng->right == NULL || siblng->right->color == BLACK)
			)
		{
			siblng->color = RED;
			deleteCase1(node->parent);
		} else
			deleteCase4(node);
	}
	void			deleteCase4(node_pointer node) {
		node_pointer	siblng = sibling(node);

		if ( siblng != NULL &&
			(node->parent->color == RED) &&
			(siblng->color == BLACK) &&
			(siblng->left == NULL || siblng->left->color == BLACK) &&
			(siblng->right == NULL || siblng->right->color == BLACK)
			)
		{
			siblng->color = RED;
			node->parent->color = BLACK;
		} else
			deleteCase5(node);
	}
	void			deleteCase5(node_pointer node) {
		node_pointer	siblng = sibling(node);

		if (siblng != NULL && siblng->color == BLACK) {
			if (
				(node == node->parent->left) &&
				(siblng->right == NULL || siblng->right->color == BLACK) &&
				(siblng->left != NULL && siblng->left->color == RED)
			   )
			{
				siblng->color = RED;
				siblng->left->color = BLACK;
				rotateRight(siblng);
			} else if (
					(node == node->parent->right) &&
					(siblng->left == NULL || siblng->left->color == BLACK) &&
					(siblng->right != NULL && siblng->right->color == RED)
				)
			{
				siblng->color = RED;
				siblng->right->color = BLACK;
				rotateLeft(siblng);
			}
		}
		deleteCase6(node);
	}
	void			deleteCase6(node_pointer node) {
		node_pointer	siblng = sibling(node);

		if (siblng != NULL)
		{
			siblng->color = node->parent->color;
			node->parent->color = BLACK;
			
			if (node == node->parent->left) {
				if (siblng->right != NULL)
					siblng->right->color = BLACK;
				rotateLeft(node->parent);
			} else {
				if (siblng->left != NULL)
					siblng->left->color = BLACK;
				rotateRight(node->parent);
			}
		}
	}
	void			treeBalanceCounter(node_pointer node) {
		if (node != NULL) {
			if (node->left != NULL && compare(node->value, node->left->value))
				size.is_unsorted = true;
			if (node->right != NULL && compare(node->right->value, node->value))
				size.is_unsorted = true;
			size.nodes++;
			if (node->color == RED)
				size.reds++;
			else
				size.blacks++;
//			if (node->right == NULL)
//				size.blacks++;
//			if (node->left == NULL)
//				size.blacks++;
		}
	}
	void			treeBalanceCounter_reset(void) {
		size.nodes = 0;
		size.reds = 0;
		size.blacks = 0;
		size.is_unsorted = false;
	}
	void			printRecursively(node_pointer node, int indents) {
		if (node != NULL)
		{
			printRecursively(node->right, indents + 1);
			for (int i = 0; i < indents; ++i) std::cout << INDENT;
			std::cout << *node << std::endl;
			treeBalanceCounter(node);
			printRecursively(node->left, indents + 1);
		}
	}
	void			printIteratively(node_pointer node) {
		if (node != NULL) {
			node_pointer save_node = node->parent;
			node->parent = NULL;
			node_pointer tmp = findMinNode(node);
			while (tmp != NULL) {
				std::cout << *tmp;
				treeBalanceCounter(tmp);
				tmp = findNextNode(tmp);
				std::cout << (tmp != NULL ? " " : "\n");
			}
//			std::cout << size << std::endl;
			node->parent = save_node;
		}
	}
public:
	void			printTreeRecursively(void) {
		treeBalanceCounter_reset();
		printRecursively(root, 0);
		std::cout << size << std::endl;
	}
	void			printTreeIteratively(bool branches) {
		if ( root != NULL && branches ) {
			treeBalanceCounter_reset();
			printIteratively(root->left);
			treeBalanceCounter_reset();
			printIteratively(root->right);
		} else if ( root != NULL ) {
			treeBalanceCounter_reset();
			printIteratively(root);
		}
	}
}; /* class RBTree end */

} /* namespace ft end */

#endif /* RBTREE_HPP end */
