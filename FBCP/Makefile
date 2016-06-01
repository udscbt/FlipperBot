INC_FILES=fbcp.command.h fbcp.common.h fbcp.h fbcp.string.h fbcp.types.h
OBJ_FILES=fbcp.command.o fbcp.common.o fbcp.o fbcp.string.o fbcp.types.o

default: test

test: lib
	g++ -o test main.cpp -I include -L lib -lfbcp

fbcp.string.o: fbcp.string.cpp
	g++ -c -o fbcp.string.o fbcp.string.cpp

fbcp.types.o: fbcp.types.cpp
	g++ -c -o fbcp.types.o fbcp.types.cpp

fbcp.o: fbcp.cpp fbcp.command.h
	g++ -c -o fbcp.o fbcp.cpp

fbcp.command.h fbcp.command.cpp: command_lists/fbcp.command.list
	./make_commands command_lists/fbcp.command.list

fbcp.command.o: fbcp.command.cpp fbcp.command.h
	g++ -c -o fbcp.command.o fbcp.command.cpp

command_lists/fbcp.command.list:
	cat command_lists/* > command_lists/fbcp.command.list

fbcp.common.o: fbcp.common.cpp fbcp.command.h
	g++ -c -o fbcp.common.o fbcp.common.cpp

lib/libfbcp.a: $(OBJ_FILES)
	ar r lib/libfbcp.a $?

$(addprefix include/,$(INC_FILES)): $(INC_FILES)
	cp $(@F) include/$(@F)

lib: $(addprefix include/,$(INC_FILES)) lib/libfbcp.a

clean:
	-rm command_lists/fbcp.command.list
	-rm *.o
	-rm fbcp.command.h
	-rm fbcp.command.cpp
	-rm test
	-rm include/*
	-rm lib/*
