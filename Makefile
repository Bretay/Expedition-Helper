CXX = /usr/bin/g++
NAME = poi
MAIN = ${NAME}
CPP_FILES = expedition_helper.cpp string_to.cpp
H_FILES = expedition_helper.h string_to.h
CXXFLAGS = -Wall -Wextra -pedantic -g

${MAIN}: ${MAIN}.cpp ${H_FILES} ${CPP_FILES}
	${CXX} ${CXXFLAGS} ${MAIN}.cpp ${CPP_FILES} -o ${MAIN}
	@echo 'Use ./${MAIN} to run'
	
style: helpful_files/cpplint.py ${FILE}
	python $< --filter=-runtime/references,-legal/copyright,-readability/streams,-runtime/explicit,-build/header_guard,-build/include ${MAIN}.cpp ${CPP_FILES} ${H_FILES}

clean:
	rm -rf ./${MAIN}
	
	