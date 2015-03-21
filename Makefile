SRCFILES = node.cpp hash.cpp helper.cpp foreground.cpp background.cpp main.cpp
HEADERS = node.h hash.h helper.h foreground.h background.h
CPPFLAGS = -std=c++0x -Wall -Wextra -lpthread

main: $(HEADERS) $(SRCFILES)
	g++ $(SRCFILES) -o main $(CPPFLAGS)

clean: 
	rm main
