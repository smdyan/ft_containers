
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <stack>
#include <map>
#include "vector.hpp"
#include "map.hpp"
#include "stack.hpp"

#define DBG			false		// Debag mode
#define NOENDL		false		// No end of line
#define OUT_STYLE	"\033[2m"	// Style for output
#define NORM_STYLE	"\033[0m"	// Normal style

template <class T1, class T2>
std::ostream&		operator<<(std::ostream& o, std::pair<T1,T2>& pair) {
	o 	<< pair.first << "-" << pair.second;
	return o;
}

template<typename Container>
void print_container(Container& c) {
	typename Container::iterator it = c.begin();
	typename Container::iterator End = c.end();
	if (!DBG) std::cout << OUT_STYLE;
	for (; it != End; ++it) std::cout << *it << " ";
	if (!DBG) std::cout << NORM_STYLE;
	std::cout << "size = " << c.size() << std::endl;
}

template<class T>
std::ostream&		operator<<(std::ostream& o, ft::vector<T>& v) {
	print_container(v);
	return o;
}

template<class T>
std::ostream&		operator<<(std::ostream& o, std::vector<T>& v) {
	print_container(v);
	return o;
}

template<class Key, class T>
std::ostream&		operator<<(std::ostream& o, typename ft::map<Key,T>& v) {
	print_container(v);
	return o;
}

template<class Key, class T>
std::ostream&		operator<<(std::ostream& o, typename std::map<Key,T>& v) {
	print_container(v);
	return o;
}

template<typename U>
void	print(U& var, bool endl = true) {
	std::cout << std::boolalpha << var;
	if (endl) std::cout << std::endl;
}

template<typename Container>
void	rand_fill(Container& c, int start, int end) {
	srand( static_cast<unsigned int>(clock()) );
	for (int i = start; i <= end; ++i) {
		c.insert(c.begin(), typename Container::value_type(start + rand() % (end - start + 1),"⎕") );
	}
}

int	main(void) {

	print("\n================ MAP TESTING ================");
	print("MAP IS RED-BLACK BINARY SELF-BALANCING SEARCH");
	print(" TREE KEY-VALUE PAIRS, SORTED BY UNIQUE KEYS\n");

	print("INITIALIZING CONSTRUCTOR ACCEPTS A RANGE OF");
	print("ITERATORS FROM ANOTHER CONTAINER\n");

	ft::vector< std::pair<int,std::string> > v;
	print("ft::vector< std::pair<int,std::string> > v;");
	rand_fill(v, 1, 24);
	print("rand_fill(v, 1, 24);");
	print("v: ",NOENDL); print(v,NOENDL);

	ft::map<int,std::string> m( v.begin(), v.end() );
	print("ft::map<int,std::string> m( v.begin(), v.end() );");
	print("m: ",NOENDL); print(m,NOENDL); m.printRecursively();

	print("\nTHE operator=() METHOD CREATES A NEW TREE\n");

	ft::map<int,std::string> m1 = m;
	print("ft::map<int,std::string> m1 = m;");
	print("m1: ",NOENDL); print(m1,NOENDL); m1.printRecursively();

	print("\nTHE at() METHOD CHECKS KEY AND THROWS EXCEPTION\n");

	print("try { m.at(0) = \"⏀\"; } catch (std::out_of_range const& e) {");
	print("    std::cerr << e.what() << std::endl; }");
	try { m.at(0) = "⏀"; }
	catch (std::out_of_range const& e) {
		std::cerr	<< (DBG ? "" : OUT_STYLE) << e.what()
					<< (DBG ? "" : NORM_STYLE"") << std::endl; }

	print("\nOPERATOR[]() METHOD CREATES A NEW ELEMENT");
	print("OR MODIFIES AN EXISTING\n");
	for (int i = 1; i <= 24 ; ++i) m1[i] = "⏀";
	print("for (int i = 1; i <= 24 ; ++i) m1[i] = \"⏀\";");
	print("m1: ",NOENDL); print(m1,NOENDL); m1.printRecursively();
	
	print("\nMAP ITERATORS ARE NEVER INVALIDATED:\n");
	
	ft::map<int,std::string>::iterator it1, it2, it3;
	print("ft::map<int,std::string>::iterator it1, it2, it3;");
	it1 = m.begin(); it2 = it1; ++it2; it3 = it2; ++it3;
	print("it1 = m.begin(); it2 = it1; ++it2; it3 = it2; ++it3;");
	print(OUT_STYLE,NOENDL); print("*it1 = ",NOENDL); print(*it1,NOENDL);
	print("  *it2 = ",NOENDL); print(*it2,NOENDL);
	print("  *it3 = ",NOENDL); print(*it3);
	m.erase(it2); print(NORM_STYLE,NOENDL); print("m.erase(it2)");
	print(OUT_STYLE,NOENDL); print("*it1 = ",NOENDL); print(*it1,NOENDL);
	print("  *it3 = ",NOENDL); print(*it3); print(NORM_STYLE);
}
