
NAMET		=	test_time
NAMEV		=	test_vector
NAMES		=	test_stack
NAMEM		=	test_map
NAME42		=	test_intra

SRCT		=	time_test.cpp
SRCV		=	vector_test.cpp
SRCS		=	stack_test.cpp
SRCM		=	map_test.cpp
SRC42		=	main.cpp

OBJT 		= 	${SRCT:.cpp=.o}
OBJV 		= 	${SRCV:.cpp=.o}
OBJS 		= 	${SRCS:.cpp=.o}
OBJM 		= 	${SRCM:.cpp=.o}
OBJ42 		= 	${SRC42:.cpp=.o}

HEADERS		=	map.hpp \
				stack.hpp \
				vector.hpp \
				iterators.hpp \
				RBTree.hpp \
				util.hpp

FLAGS		=	-Wall -Wextra -Werror -std=c++98

CPP			=	c++

%.o:			%.cpp ${HEADERS}
				${CPP} -c ${FLAGS} -I. $< -o ${<:.cpp=.o}

${NAMET}:		${OBJT}
				${CPP} ${FLAGS} ${OBJT} -o ${NAMET}

${NAMEV}:		${OBJV}
				${CPP} ${FLAGS} ${OBJV} -o ${NAMEV}

${NAMES}:		${OBJS}
				${CPP} ${FLAGS} ${OBJS} -o ${NAMES}

${NAMEM}:		${OBJM}
				${CPP} ${FLAGS} ${OBJM} -o ${NAMEM}

${NAME42}:		${OBJ42}
				${CPP} ${FLAGS} ${OBJ42} -o ${NAME42}

all:			vector stack map time intra
time:			${NAMET}
vector:			${NAMEV}
stack:			${NAMES}
map:			${NAMEM}
intra:			${NAME42}

clean:
				rm -f ${OBJT} ${OBJV} ${OBJS} ${OBJM} ${OBJ42}

fclean:			clean
				rm -f ${NAMET} ${NAMEV} ${NAMES} ${NAMEM} ${NAME42}

re:				fclean all

.PHONY:			all clean fclean re time vector stack map intra
