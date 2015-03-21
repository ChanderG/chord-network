CPPFLAGS = -std=c++0x -Wall -Wextra -lpthread

main: main.cpp node.h node.cpp hash.h hash.cpp helper.h helper.cpp
	g++ node.cpp hash.cpp helper.cpp main.cpp -o main $(CPPFLAGS)

clean: 
	rm main
