OBJECT=Meeting.o User.o Date.o Storage.o AgendaService.o UDPUI.o
exe: $(OBJECT) 
	g++ -o res  UDPUI.o Meeting.o User.o Date.o Storage.o AgendaService.o -std=c++0x -g
Meeting.o: Meeting.h Meeting.cpp
	g++ -c Meeting.cpp -g
Date.o: Date.h Date.cpp
	g++ -c Date.cpp -g
User.o: User.h User.cpp
	g++ -c User.cpp -g
Storage.o: Storage.h Storage.cpp
	g++ -c Storage.cpp -std=c++0x -g
AgendaService.o: AgendaService.h AgendaService.cpp
	g++ -c AgendaService.cpp -std=c++0x -g
UDPUI.o: AgendaUI.h UDPUI.cpp
	g++ -c UDPUI.cpp -std=c++0x -g
clean:
	rm $(OBJECT) res
