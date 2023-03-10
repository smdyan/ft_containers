
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include "vector.hpp"

#define DBG			false		// Debag mode
#define NOENDL		false		// No end of line
#define OUT_STYLE	"\033[2m"	// Style for output
#define NORM_STYLE	"\033[0m"	// Normal style
typedef  int		T;			// Container value
typedef  char		T1;			// Container value

template<typename Container>
void print_container(Container& c) {
	typename Container::iterator it = c.begin();
	typename Container::iterator End = c.end();
	if (!DBG) std::cout << OUT_STYLE;
	for (; it != End; ++it)
		std::cout << *it << " ";
	std::cout << "size = " << c.size();
	std::cout << " capacity = " << c.capacity() << std::endl;
	if (!DBG) std::cout << NORM_STYLE;
}

template< class T >
std::ostream&	operator<<(std::ostream& o, ft::vector<T>& v) {
	print_container(v);
	return o;
}

template< class T >
std::ostream&	operator<<(std::ostream& o, std::vector<T>& v) {
	print_container(v);
	return o;
}

template< typename U >
void	print(U& var, bool endl = true) {
	std::cout << std::boolalpha << var; //returns "true/false" if bool param passed
	if (endl) std::cout << std::endl;
}

int	main(void) {
	{
		print("\n==== VECTOR TESTING CONSTRUCTORS AND operator=() ====\n");

		ft::vector<T> v;
		print("ft::vector<T> v;");
		print("v: ", NOENDL); print(v);

		std::allocator<T1> alloc;
		print("std::allocator<T1> alloc;");
		ft::vector<T> v1(alloc);
		print("ft::vector<T> v1(alloc);");
		print("v1: ", NOENDL); print(v1);

		ft::vector<T> v2(9, 42);
		print("ft::vector<T> v2(9, 42);");
		print("v2: ", NOENDL); print(v2);

		ft::vector<T> v3(v2.begin(), v2.end());
		print("ft::vector<T> v3(v2.begin(), v2.end());");
		print("v3: ", NOENDL); print(v3);

		ft::vector<T> v4(v3);
		print("ft::vector<T> v4(v3);");
		print("v4: ", NOENDL); print(v4);

		v2 = v4;
		print("v2 = v4;");
		print("v2: ", NOENDL); print(v2, NOENDL);
		print("v4: ", NOENDL); print(v4);

		v1 = v2;
		print("v1 = v2;");
		print("v1: ", NOENDL); print(v1, NOENDL);
		print("v2: ", NOENDL); print(v2);
	}
	{
		print("==== namespace std, STL lib\n");

		std::vector<T> v;
		print("std::vector<T> v;");
		print("v: ", NOENDL); print(v);

		std::allocator<T1> alloc;
		print("std::allocator<T1> alloc;");
		std::vector<T> v1(alloc);
		print("std::vector<T> v1(alloc);");
		print("v1: ", NOENDL); print(v1);

		std::vector<T> v2(9, 21);
		print("std::vector<T> v2(9, 21);");
		print("v2: ", NOENDL); print(v2);

		std::vector<T> v3(v2.begin(), v2.end());
		print("std::vector<T> v3(v2.begin(), v2.end());");
		print("v3: ", NOENDL); print(v3);

		std::vector<T> v4(v3);
		print("std::vector<T> v4(v3);");
		print("v4: ", NOENDL); print(v4);

		v2 = v4;
		print("v2 = v4;");
		print("v2: ", NOENDL); print(v2, NOENDL);
		print("v4: ", NOENDL); print(v4);

		v1 = v2;
		print("v1 = v2;");
		print("v1: ", NOENDL); print(v1, NOENDL);
		print("v2: ", NOENDL); print(v2);
	}
	{
		print("==== VECTOR TESTING FUNCTIONS ====\n");

		ft::vector<T1> v;
		print("ft::vector<T1> v;");
		v.assign(9, '*');
		print("v.assign(9, '*');");
		print("v: ", NOENDL); print(v);

		ft::vector<T1> v1(9, '#');
		print("ft::vector<T1> v1(9, '#');");
		print("v1: ", NOENDL); print(v1, NOENDL);
		v1.assign(++v.begin(), --v.end());
		print("v1.assign(++v.begin(), --v.end());");
		print("v1: ", NOENDL); print(v1);

		ft::vector<T> v2;
		print("ft::vector<T> v2;");
		for (int i = 0; i <= 9; ++i) v2.push_back(i);
		print("for (int i = 0; i <= 9; ++i) v2.push_back(i);");
		print("v2: ", NOENDL); print(v2);

		v2.pop_back();;
		print("v2.pop_back();");
		print("v2: ", NOENDL); print(v2);

		v2[6] = 90009;
		print("v2[6] = 90009;");
		print("v2: ", NOENDL); print(v2);

		v2.at(6) = 10001;
		print("v2.at(6) = 10001;");
		print("v2: ", NOENDL); print(v2);

		print("try { v2.at(15) = 10001; } catch (std::out_of_range const& e) {");
		print("\tstd::cerr << e.what() << std::endl; }");
		try { v2.at(15) = 10001; } catch (std::out_of_range const& e) {
			std::cerr	<< (DBG ? "" : OUT_STYLE) << e.what()
						<< (DBG ? "\n" : NORM_STYLE"\n") << std::endl; }

		v2.reserve(100);
		print("v2.reserve(100);");
		print("v2: ", NOENDL); print(v2);

		print("try { v2.reserve(v2.max_size() + 1); } catch (std::exception& e) {");
		print("\tstd::cerr << e.what() << std::endl; }");
		try { v2.reserve((v2.max_size() + 1)); } catch (std::exception& e) {
			std::cerr	<< (DBG ? "" : OUT_STYLE) << e.what()
						<< (DBG ? "\n" : NORM_STYLE"\n") << std::endl; }

		v2.resize(3);
		print("v2.resize(3);");
		print("v2: ", NOENDL); print(v2);

		v2.resize(10);
		print("v2.resize(10);");
		print("v2: ", NOENDL); print(v2);

		v2.clear();
		print("v2.clear();");
		print("v2: ", NOENDL); print(v2);

	}
	{
		print("==== namespace std, STL lib\n");

		std::vector<T1> v;
		print("std::vector<T1> v;");
		v.assign(9, '*');
		print("v.assign(9, '*');");
		print("v: ", NOENDL); print(v);

		std::vector<T1> v1(9, '#');
		print("std::vector<T1> v1(9, '#');");
		print("v1: ", NOENDL); print(v1, NOENDL);
		v1.assign(++v.begin(), --v.end());
		print("v1.assign(++v.begin(), --v.end());");
		print("v1: ", NOENDL); print(v1);

		std::vector<T> v2;
		print("std::vector<T> v2;");
		for (int i = 0; i <= 9; ++i) v2.push_back(i);
		print("for (int i = 0; i <= 9; ++i) v2.push_back(i);");
		print("v2: ", NOENDL); print(v2);

		v2.pop_back();;
		print("v2.pop_back();");
		print("v2: ", NOENDL); print(v2);

		v2[6] = 90009;
		print("v2[6] = 90009;");
		print("v2: ", NOENDL); print(v2);

		v2.at(6) = 10001;
		print("v2.at(6) = 10001;");
		print("v2: ", NOENDL); print(v2);

		print("try { v2.at(15) = 10001; } catch (std::out_of_range const& e) {");
		print("\tstd::cerr << e.what() << std::endl; }");
		try { v2.at(15) = 10001; } catch (std::out_of_range const& e) {
			std::cerr	<< (DBG ? "" : OUT_STYLE) << e.what()
						<< (DBG ? "\n" : NORM_STYLE"\n") << std::endl; }

		v2.reserve(100);
		print("v2.reserve(100);");
		print("v2: ", NOENDL); print(v2);

		print("try { v2.reserve(v2.max_size() + 1); } catch (std::exception& e) {");
		print("\tstd::cerr << e.what() << std::endl; }");
		try { v2.reserve((v2.max_size() + 1)); } catch (std::exception& e) {
			std::cerr	<< (DBG ? "" : OUT_STYLE) << e.what()
						<< (DBG ? "\n" : NORM_STYLE"\n") << std::endl; }

		v2.resize(3);
		print("v2.resize(3);");
		print("v2: ", NOENDL); print(v2);

		v2.resize(10);
		print("v2.resize(10);");
		print("v2: ", NOENDL); print(v2);

		v2.clear();
		print("v2.clear();");
		print("v2: ", NOENDL); print(v2);
	}
	{
		print("\n==== VECTOR TESTING FUNCTIONS WHITH ITERATORS ====\n");

		ft::vector<T1> v(9, '*');
		print("ft::vector<T1> v(9, '*');");
		print("v: ", NOENDL); print(v, NOENDL);
		ft::vector<T1>::iterator it(v.begin());
		print("ft::iterator<T1>::iterator it(v.begin());");
		v.insert(it, '#');
		print("v.insert(it, '#');");
		print("v: ", NOENDL); print(v);

		ft::vector<T1> v1(4, '#');
		print("ft::vector<T1> v1(4, '#');");
		print("v1: ", NOENDL); print(v1, NOENDL);
		v.insert(++++++++++v.begin(), v1.begin(), v1.end());
		print("v.insert(++++++++++v.begin(), v1.begin(), v1.end());");
		print("v: ", NOENDL); print(v);

		v.erase(++++++v.begin());
		print("v.erase(++++++v.begin());");
		print("v: ", NOENDL); print(v);

		v.erase(++++++++v.begin(), ----------v.end());
		print("++++++++v.erase(v.begin(), ----------v.end());");
		print("v: ", NOENDL); print(v);

		print("v: ", NOENDL); print(v, NOENDL);
		print("v1: ", NOENDL); print(v1, NOENDL);
		ft::vector<T1>::iterator iT = v.begin();
		print("ft::vector<T1>::iterator iT = v.begin(); ");
		print("*iT = ",NOENDL); print(*iT);
		v.swap(v1);
		print("v.swap(v1);");
		print("*iT = ",NOENDL); print(*iT);
		print("v: ", NOENDL); print(v, NOENDL);
		print("v1: ", NOENDL); print(v1);

	}
	{
		print("==== namespace std, STL lib\n");

		std::vector<T1> v(9, '*');
		print("std::vector<T1> v(9, '*');");
		print("v: ", NOENDL); print(v, NOENDL);
		std::vector<T1>::iterator it(++++++v.begin());
		print("std::iterator<T1>::iterator it(++++++v.begin());");
		v.insert(it, '#');
		print("v.insert(it, '#');");
		print("v: ", NOENDL); print(v);

		std::vector<T1> v1(4, '#');
		print("std::vector<T1> v1(4, '#');");
		print("v1: ", NOENDL); print(v1, NOENDL);
		v.insert(++++++++++v.begin(), v1.begin(), v1.end());
		print("v.insert(++++++++++v.begin(), v1.begin(), v1.end());");
		print("v: ", NOENDL); print(v);

		v.erase(++++++v.begin());
		print("v.erase(++++++v.begin());");
		print("v: ", NOENDL); print(v);

		v.erase(++++++++v.begin(), ----------v.end());
		print("++++++++v.erase(v.begin(), ----------v.end());");
		print("v: ", NOENDL); print(v);

		print("v: ", NOENDL); print(v, NOENDL);
		print("v1: ", NOENDL); print(v1, NOENDL);
		std::vector<T1>::iterator iT = v.begin();
		print("std::vector<T1>::iterator iT = v.begin(); ");
		print("*iT = ",NOENDL); print(*iT);
		v.swap(v1);
		print("v.swap(v1);");
		print("*iT = ",NOENDL); print(*iT);
		print("v: ", NOENDL); print(v, NOENDL);
		print("v1: ", NOENDL); print(v1, NOENDL);
	}
	{
		print("\n==== VECTOR TESTING ITERATOR COMPARISONS ====\n");
		
		ft::vector<T> v;
		print("ft::vector<T> v;");
		for(int i = 0; i < 9; ++i)
			v.push_back(i);
		print("for(int i = 0; i < 9; ++i) v.push_back(i);");
		print("v: ",NOENDL); print(v);
		
		ft::vector<T>::iterator Begin = v.begin();
		print("ft::vector<T>::iterator Begin = v.begin();");
		ft::vector<T>::const_iterator cEnd = v.end();
		print("ft::vector<T>::const_iterator cEnd = v.end();");
		ft::vector<T>::reverse_iterator rBegin = v.rbegin();
		print("ft::vector<T>::reverse_iterator rBegin = v.rbegin();");
		ft::vector<T>::const_reverse_iterator crEnd = v.rend();
		print("ft::vector<T>::const_reverse_iterator crEnd = v.rend();");
		print("");
		
		bool b;
		print("Begin == cEnd | ",NOENDL);
		print(b = Begin == cEnd);;
		print("cEnd != Begin | ",NOENDL);
		print(b = cEnd != Begin);;
		print("");
		print("rBegin == crEnd | ",NOENDL);
		print(b = rBegin == crEnd);;
		print("crEnd != rBegin | ",NOENDL);
		print(b = crEnd != rBegin);;
		print("");
		
		print("Begin < cEnd | ",NOENDL);
		print(b = Begin < cEnd);;
		print("cEnd > Begin | ",NOENDL);
		print(b = cEnd > Begin);;
		print("");
		print("rBegin < crEnd | ",NOENDL);
		print(b = rBegin < crEnd);;
		print("crEnd > rBegin | ",NOENDL);
		print(b = crEnd > rBegin);;
		print("");

		print("Begin <= cEnd | ",NOENDL);
		print(b = Begin <= cEnd);;
		print("cEnd >= Begin | ",NOENDL);
		print(b = cEnd >= Begin);;
		print("");
		print("rBegin <= crEnd | ",NOENDL);
		print(b = rBegin <= crEnd);;
		print("crEnd >= rBegin | ",NOENDL);
		print(b = crEnd >= rBegin);;
		print("");
	}
	{
		print("==== namespace std, STL lib\n");

		std::vector<T> v;
		print("std::vector<T> v;");
		for(int i = 0; i < 9; ++i) v.push_back(i);
		print("for(int i = 0; i < 9; ++i) v.push_back(i);");
		print("v: ",NOENDL); print(v);
		
		std::vector<T>::iterator Begin = v.begin();
		print("std::vector<T>::iterator Begin = v.begin();");
		std::vector<T>::const_iterator cEnd = v.end();
		print("std::vector<T>::const_iterator cEnd = v.end();");
		std::vector<T>::reverse_iterator rBegin = v.rbegin();
		print("std::vector<T>::reverse_iterator rBegin = v.rbegin();");
		std::vector<T>::const_reverse_iterator crEnd = v.rend();
		print("std::vector<T>::const_reverse_iterator crEnd = v.rend();");
		print("");
		
		bool b;
		print("Begin == cEnd | ",NOENDL);
		print(b = Begin == cEnd);;
		print("cEnd != Begin | ",NOENDL);
		print(b = cEnd != Begin);;
		print("");
		print("rBegin == crEnd | ",NOENDL);
		print(b = rBegin == crEnd);;
		print("crEnd != rBegin | ",NOENDL);
		print(b = crEnd != rBegin);;
		print("");
		
		print("Begin < cEnd | ",NOENDL);
		print(b = Begin < cEnd);;
		print("cEnd > Begin | ",NOENDL);
		print(b = cEnd > Begin);;
		print("");
		print("rBegin < crEnd | ",NOENDL);
		print(b = rBegin < crEnd);;
		print("crEnd > rBegin | ",NOENDL);
		print(b = crEnd > rBegin);;
		print("");

		print("Begin <= cEnd | ",NOENDL);
		print(b = Begin <= cEnd);;
		print("cEnd >= Begin | ",NOENDL);
		print(b = cEnd >= Begin);;
		print("");
		print("rBegin <= crEnd | ",NOENDL);
		print(b = rBegin <= crEnd);;
		print("crEnd >= rBegin | ",NOENDL);
		print(b = crEnd >= rBegin);;
		print("");
	}
	return 0;
}

