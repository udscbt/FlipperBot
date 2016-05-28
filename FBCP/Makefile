fbcp_test: main.cpp fbcp.command.h fbcp.o fbcp.common.o fbcp.types.o fbcp.command.o
	g++ -o fbcp_test main.cpp fbcp.o fbcp.common.o fbcp.types.o fbcp.command.o

fbcp.types.o: fbcp.types.cpp
	g++ -c -o fbcp.types.o fbcp.types.cpp

fbcp.o: fbcp.cpp fbcp.command.h
	g++ -c -o fbcp.o fbcp.cpp

fbcp.command.h: command_lists/fbcp.command.list
	./make_commands command_lists/fbcp.command.list

fbcp.command.o: fbcp.command.cpp fbcp.command.h
	g++ -c -o fbcp.command.o fbcp.command.cpp

command_lists/fbcp.command.list:
	cat command_lists/* > command_lists/fbcp.command.list

fbcp.common.o: fbcp.common.cpp fbcp.command.h
	g++ -c -o fbcp.common.o fbcp.common.cpp

clean:
	-rm command_lists/fbcp.command.list
	-rm *.o
	-rm fbcp.command.h
	-rm fbcp.command.cpp
	-rm fbcp_test
