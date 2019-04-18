COMPILER = g++
FLAGS = -std=c++11 -Wall -Wextra


router: router.o routing_table_entry.o socket_tools.o routing_table.o reception_tools.o
	$(COMPILER) -o router router.o routing_table_entry.o socket_tools.o routing_table.o reception_tools.o $(FLAGS)

routing_table_entry.o: routing_table_entry.cpp routing_table_entry.h
	$(COMPILER) -o routing_table_entry.o -c routing_table_entry.cpp $(FLAGS)

socket_tools.o: socket_tools.cpp socket_tools.h
	$(COMPILER) -o socket_tools.o -c socket_tools.cpp $(FLAGS)

routing_table.o: routing_table.cpp routing_table.h
	$(COMPILER) -o routing_table.o -c routing_table.cpp $(FLAGS)

reception_tools.o: reception_tools.cpp reception_tools.h
	$(COMPILER) -o reception_tools.o -c reception_tools.cpp $(FLAGS)

router.o: router.cpp router.h
	$(COMPILER) -o router.o -c router.cpp $(FLAGS)

clean:
	rm *.o

distclean:
	rm *.o router