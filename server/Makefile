TARGET = server
SRCS = server.cpp
SRCS2= ../socket/PracticalSocket.C
OBJS = $(SRCS:.cpp=.o)
OBJS2 = PracticalSocket.o
CC = g++
LIBS = -lpcap -ansi -pedantic
VPATH=../socket

all: $(TARGET)

$(TARGET): $(OBJS) $(OBJS2)
	$(CC) -o $@ $(OBJS) $(OBJS2) $(LIBS)

.cpp.o:
	$(CC) -c $<
.C.o:
	$(CC) -c $<

clean:
	rm -f $(TARGET) $(OBJS) $(OBJS2)
