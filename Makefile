CPPFLAGS = -std=c++0x -Wall -Wextra -lpthread

main: main.cpp node.h node.cpp hash.h hash.cpp helper.h helper.cpp foreground.h foreground.cpp background.h background.cpp
	g++ node.cpp hash.cpp helper.cpp foreground.cpp background.cpp main.cpp -o main $(CPPFLAGS)

clean: 
	rm main
