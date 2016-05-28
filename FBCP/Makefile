fbcp_test: main.cpp fbcp.o fbcp.common.o fbcp.types.o fbcp.command.o
	g++ -o fbcp_test main.cpp fbcp.o fbcp.common.o fbcp.types.o fbcp.command.o

fbcp.types.o: fbcp.types.cpp fbcp.types.h
	g++ -c -o fbcp.types.o fbcp.types.cpp

fbcp.o: fbcp.cpp fbcp.h fbcp.command.h fbcp.types.h fbcp.types.o
	g++ -c -o fbcp.o fbcp.cpp

fbcp.command.h: command_lists/fbcp.command.list
	./make_commands command_lists/fbcp.command.list

fbcp.command.o: fbcp.command.h fbcp.command.cpp
	g++ -c -o fbcp.command.o fbcp.command.cpp

command_lists/fbcp.command.list:
	cat command_lists/* > command_lists/fbcp.command.list

fbcp.common.o: fbcp.common.cpp fbcp.o
	g++ -c -o fbcp.common.o fbcp.common.cpp

clean:
	-rm command_lists/fbcp.command.list
	-rm *.o
	-rm fbcp.command.h
