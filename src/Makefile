SRCFILES = node.cpp hash.cpp helper.cpp foreground.cpp background.cpp communication.cpp  main.cpp help.cpp startup.cpp
HEADERS = node.h hash.h helper.h foreground.h background.h communication.h help.h startup.h
CPPFLAGS = -std=c++0x -Wall -Wextra -lpthread

main: $(HEADERS) $(SRCFILES)
	g++ $(SRCFILES) -o ../main $(CPPFLAGS)

clean: 
	rm ../main
