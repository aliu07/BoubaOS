CC=gcc
CFLAGS=-I./include/core -I./include/memory -I./include/scheduling

# Create build directory if it doesn't exist
$(shell mkdir -p build)

# Object files will go in build directory
OBJECTS=build/shell.o \
        build/interpreter.o \
        build/shellmemory.o \
        build/pcb.o \
        build/ready_queue.o \
        build/scheduler.o

mysh: $(OBJECTS)
	$(CC) $(CFLAGS) -o build/mysh $(OBJECTS)

build/shell.o: src/core/shell.c
	$(CC) $(CFLAGS) -c src/core/shell.c -o build/shell.o

build/interpreter.o: src/core/interpreter.c
	$(CC) $(CFLAGS) -c src/core/interpreter.c -o build/interpreter.o

build/shellmemory.o: src/memory/shellmemory.c
	$(CC) $(CFLAGS) -c src/memory/shellmemory.c -o build/shellmemory.o

build/pcb.o: src/scheduling/pcb.c
	$(CC) $(CFLAGS) -c src/scheduling/pcb.c -o build/pcb.o

build/ready_queue.o: src/scheduling/ready_queue.c
	$(CC) $(CFLAGS) -c src/scheduling/ready_queue.c -o build/ready_queue.o

build/scheduler.o: src/scheduling/scheduler.c
	$(CC) $(CFLAGS) -c src/scheduling/scheduler.c -o build/scheduler.o

clean:
	rm -rf build/
