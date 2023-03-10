
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <stack>
#include <map>
#include "vector.hpp"
#include "stack.hpp"
#include "map.hpp"

#define DBG			false			// Debag mode
#define NOENDL		false			// No end of line

std::string	enter_field(std::string prompt) {
	std::string	field;
	while (true) {
		std::cout << prompt;
		std::getline(std::cin, field, '\n');
		if (std::cin.eof() == true) exit(0);
		else if (std::cin.fail() == true || std::cin.bad() == true) exit(-1);
		if (field.length() != 0) break ; }
	return (field);
}

template<typename U>
void	print(U& var, std::string style = DEFAULT, bool endl = true) {
	if (!DBG) std::cout << style;
	std::cout << std::boolalpha << var;
	if (!DBG) std::cout << DEFAULT;
	if (endl) std::cout << std::endl;
}

void	print_time(time_t& start, time_t& finish, std::string prompt, bool endl = true) {
	std::cout << prompt << " time = ";
	if (!DBG) std::cout << YELLOW;
	std::cout << (finish - start) * 1000000 / CLOCKS_PER_SEC << " usec ";
	if (!DBG) std::cout << DEFAULT;
	if (endl) std::cout << std::endl;
}

void	make_word(std::string& s, char& ch) {
	s.push_back( std::tolower(static_cast<unsigned char>(ch)) );
}

template<typename V>
void	make_fileBuffer(std::ifstream& fin, V& v) {
	typename V::value_type	u;
	while ( fin.get(u) )
		v.push_back(u);
}

template<typename V>
void	print_container(V& v, int count) {
	typename V::iterator it = v.begin();
	typename V::iterator End = v.end();
	for (int i = 0; it != End && i < count; ++it, ++i )
		std::cout << *it << std::endl;
	std::cout << std::endl;
}

template<typename Container, typename Src>
void	fill_vector(Container& c, Src& src) {
	typename Src::size_type size = src.size();
	typename Src::size_type i = 0;
	c.push_back( "" );
	while ( i < size ) {
		while ( i < size && std::isalpha(src[i]) ) {
			make_word(c.back(), src[i]);
			++i; }
		while ( i < size && !std::isalpha(src[i]) )
			++i;
		if ( c.back() != "" )
			c.push_back( "" );
	}
	if ( c.back() == "" )
		c.pop_back();
}

template<typename Container, typename Src>
void	fill_stack(Container& c, Src& src) {
	typename Src::size_type size = src.size();
	typename Src::size_type i = 0;
	c.push("");
	while ( i < size ) {
		while ( i < size && std::isalpha(src[i]) ) {
			make_word(c.top(), src[i]);
			++i; }
		while ( i < size && !std::isalpha(src[i]) )
			++i;
		if ( c.top() != "" )
			c.push("");
	}
	if ( c.top() == "" )
		c.pop();
}

template<typename Container, typename Iterator_bool, typename Src>
void	fill_map(Container& c, Iterator_bool ib, Src& src) {
	typename Src::size_type size = src.size();
	typename Src::size_type i = 0;
	std::string str;
	while ( i < size ) {
		while ( i < size && std::isalpha(src[i]) ) {
			make_word(str, src[i]);
			++i; }
		while ( i < size && !std::isalpha(src[i]) )
			++i;
		if ( str != "" ) {
			ib = c.insert(typename Container::value_type(str, 1));
			if ( ib.second == false ) ib.first->second += 1;
			str.clear();
		}
	}
}

/* template<typename Container, typename Src>
void	fill_set(Container& c, Src& src) {
	typename Src::size_type size = src.size();
	typename Src::size_type i = 0;
	std::string str;
	while ( i < size ) {
		while ( i < size && std::isalpha(src[i]) ) {
			make_word(str, src[i]);
			++i; }
		while ( i < size && !std::isalpha(src[i]) )
			++i;
		if ( str != "" ) {
			c.insert(str);
			str.clear(); }
	}
} */

int	main(int ac, char* av[]) {
	std::string			filename;
	time_t				startTime;
	time_t				finishTime;
	std::vector<char>	fileBuffer;
	size_t				x = 0;
	size_t				i = 0;

	if (ac == 1)
		filename = enter_field("Enter the name of an existing text file: ");
	else
		filename = std::string(av[1]);
	std::ifstream fin(filename.c_str());
	if (!fin) {
		std::cout << "File not found of file error.\n";
		return 1; }
	make_fileBuffer(fin, fileBuffer);
	fin.close();
	
	print("");
	
	ft::vector<std::string> ft_vector;
	startTime = clock();
	fill_vector(ft_vector, fileBuffer);
	finishTime = clock();
	print_time(startTime, finishTime, " ft::vector push_back()",NOENDL);
	x = ft_vector.size();
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT,NOENDL);
	x = ft_vector.capacity();
	print(" capacity = ",DEFAULT,NOENDL); print(x,DEFAULT);

	std::vector<std::string> std_vector;
	startTime = clock();
	fill_vector(std_vector, fileBuffer);
	finishTime = clock();
	print_time(startTime, finishTime, "std::vector push_back()",NOENDL);
	x = std_vector.size();
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT,NOENDL);
	x = std_vector.capacity();
	print(" capacity = ",DEFAULT,NOENDL); print(x,DEFAULT); print("");

	ft::stack<std::string, ft::vector<std::string> > ft_stack;
	startTime = clock();
	fill_stack(ft_stack, fileBuffer);
	finishTime = clock();
	print_time(startTime, finishTime, " ft::stack push()",NOENDL);
	x = ft_stack.size();
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT);

	std::stack<std::string, std::vector<std::string> > std_stack;
	startTime = clock();
	fill_stack(std_stack, fileBuffer);
	finishTime = clock();
	print_time(startTime, finishTime, "std::stack push()",NOENDL);
	x = std_stack.size();
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT); print("");

	ft::map<std::string,size_t> ft_map;
	startTime = clock();
	ft::map<std::string,size_t>::iterator_bool ft_ib;
	fill_map(ft_map, ft_ib, fileBuffer);
	finishTime = clock();
	print_time(startTime, finishTime, " ft::map insert()",NOENDL);
	x = ft_map.size();
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT);

	std::map<std::string,size_t> std_map;
	startTime = clock();
	std::pair<std::map<std::string,size_t>::iterator,bool> std_ib;
	fill_map(std_map, std_ib, fileBuffer);
	finishTime = clock();
	print_time(startTime, finishTime, "std::map insert()",NOENDL);
	x = std_map.size();
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT); print("");

/*	ft::set<std::string> ft_set;
	startTime = clock();
	fill_set(ft_set, fileBuffer);
	finishTime = clock();
	print_time(startTime, finishTime, " ft::set insert()",NOENDL);
	x = ft_set.size();
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT);

	std::set<std::string> std_set;
	startTime = clock();
	fill_set(std_set, fileBuffer);
	finishTime = clock();
	print_time(startTime, finishTime, "std::set insert()",NOENDL);
	x = ft_set.size();
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT); print("");
*/	
	std::string word = enter_field("Enter the word for search: ");
	print("");
	
	i = 0; x = ft_vector.size();
	startTime = clock();
	while (i < x && word != ft_vector[i]) ++i;
	finishTime = clock();
	print_time(startTime, finishTime, " ft::vector finding",NOENDL);
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT,NOENDL);
	x = ft_vector.capacity();
	print(" capacity = ",DEFAULT,NOENDL); print(x,DEFAULT);

	i = 0; x = std_vector.size();
	startTime = clock();
	while (i < x && word != std_vector[i]) ++i;
	finishTime = clock();
	print_time(startTime, finishTime, "std::vector finding",NOENDL);
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT,NOENDL);
	x = ft_vector.capacity();
	print(" capacity = ",DEFAULT,NOENDL); print(x,DEFAULT); print("");

	i = 0; x = ft_stack.size();
	startTime = clock();
	while (i < x && word != ft_stack.top()) {
		ft_stack.pop(); ++i; }
	finishTime = clock();
	print_time(startTime, finishTime, " ft::stack finding",NOENDL);
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT);

	i = 0; x = std_stack.size();
	startTime = clock();
	while (i < x && word != std_stack.top()) {
		std_stack.pop(); ++i; }
	finishTime = clock();
	print_time(startTime, finishTime, "std::stack finding",NOENDL);
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT); print("");

	x = ft_map.size();
	startTime = clock();
	ft_map.find(word);
	finishTime = clock();
	print_time(startTime, finishTime, " ft::map find()",NOENDL);
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT);

	x = std_map.size();
	startTime = clock();
	std_map.find(word);
	finishTime = clock();
	print_time(startTime, finishTime, "std::map find()",NOENDL);
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT); print("");

/*	x = ft_set.size();
	startTime = clock();
	ft_set.find(word);
	finishTime = clock();
	print_time(startTime, finishTime, " ft::set find()",NOENDL);
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT);

	x = std_set.size();
	startTime = clock();
	std_set.find(word);
	finishTime = clock();
	print_time(startTime, finishTime, "std::set find()",NOENDL);
	print("size = ",DEFAULT,NOENDL); print(x,DEFAULT); print("");
*/

	if (ac == 3 && std::string(av[2]) == "-t")
		ft_map.printRecursively();
	return 0;
}
