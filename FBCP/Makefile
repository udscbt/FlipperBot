fbcp_test: main.cpp fbcp.h fbcp.o
	g++ -o fbcp_test main.cpp fbcp.o

fbcp.o: fbcp.cpp fbcp.h fbcp.command.h
	g++ -c fbcp.cpp -o fbcp.o

fbcp.command.h: command_lists/fbcp.command.list
	./make_commands command_lists/fbcp.command.list

command_lists/fbcp.command.list:
	cat command_lists/* > command_lists/fbcp.command.list

clean:
	-rm command_lists/fbcp.command.list
	-rm *.o
	-rm fbcp.command.h
