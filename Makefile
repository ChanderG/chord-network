CPPFLAGS = -Wall -Wextra

main: main.cpp node.h node.cpp hash.h hash.cpp helper.h helper.cpp
	g++ node.cpp hash.cpp helper.cpp main.cpp -o main $(CPPFLAGS)

clean: 
	rm main
