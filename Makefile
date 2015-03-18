main: main.cpp node.h hash.h hash.cpp helper.h helper.cpp
	g++ helper.cpp hash.cpp main.cpp -o main

clean: 
	rm main
