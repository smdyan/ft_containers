
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "vector.hpp"
#include "stack.hpp"

#define DBG			false		// Debag mode
#define NOENDL		false		// No end of line
#define OUT_STYLE	"\033[2m"	// Style for output
#define NORM_STYLE	"\033[0m"	// Normal style


template<typename U>
void	print(U& var, bool endl = true) {
	std::cout << std::boolalpha << var;
	if (endl) std::cout << std::endl;
}

int	main(void) {
	{
		bool b;
		print("\n==== STACK TESTING ====\n");
		
		ft::stack< char, ft::vector<char> >	s;
		print("ft::stack< char, ft::vector<char> >	s;");
		for ( char i = 'a'; i <= 'z'; ++i ) s.push(i);
		print("for ( char i = 'a'; i <= 'z'; ++i ) s.push(i);");
		size_t size = s.size();
		print("s.size() = ",NOENDL); print(size); print("");
		
		ft::stack< char, ft::vector<char> >	s1;
		print("ft::stack< char, ft::vector<char> >	s1;");
		s1 = s;
		print("s1 = s;");
		size_t size1 = s1.size();
		print("s1.size() = ",NOENDL); print(size1); print("");
		
		print("for ( size_t i = 0; i < size1; ++i ) s1.pop();");
		for ( size_t i = 0; i < size1; ++i ) { print(s1.top(),NOENDL); s1.pop(); }
		print(""); size1 = s1.size();
		print("s1.size() = ",NOENDL); print(size1); print(""); print("");
		
		print("s == s1 | ",NOENDL); print(b = s == s1);
		print("s != s1 | ",NOENDL); print(b = s != s1);
		print("s  < s1 | ",NOENDL); print(b = s < s1);
		print("s  > s1 | ",NOENDL); print(b = s > s1);
		print("s <= s1 | ",NOENDL); print(b = s <= s1);
		print("s >= s1 | ",NOENDL); print(b = s >= s1); print("");
	}
	{
		bool b;
		print("==== namespace std, STL lib\n");
		
		std::stack< char, std::vector<char> >	s;
		print("std::stack< char, std::vector<char> >	s;");
		for ( char i = 'a'; i <= 'z'; ++i ) s.push(i);
		print("for ( char i = 'a'; i <= 'z'; ++i ) s.push(i);");
		size_t size = s.size();
		print("s.size() = ",NOENDL); print(size); print("");
		
		std::stack< char, std::vector<char> >	s1;
		print("std::stack< char, std::vector<char> >	s1;");
		s1 = s;
		print("s1 = s;");
		size_t size1 = s1.size();
		print("s1.size() = ",NOENDL); print(size1); print("");
		
		print("for ( size_t i = 0; i < size1; ++i ) s1.pop();");
		for ( size_t i = 0; i < size1; ++i ) { print(s1.top(),NOENDL); s1.pop(); }
		print(""); size1 = s1.size();
		print("s1.size() = ",NOENDL); print(size1); print(""); print("");
		
		print("s == s1 | ",NOENDL); print(b = s == s1);
		print("s != s1 | ",NOENDL); print(b = s != s1);
		print("s  < s1 | ",NOENDL); print(b = s < s1);
		print("s  > s1 | ",NOENDL); print(b = s > s1);
		print("s <= s1 | ",NOENDL); print(b = s <= s1);
		print("s >= s1 | ",NOENDL); print(b = s >= s1); print("");

	}
}
