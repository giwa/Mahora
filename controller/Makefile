TARGET = controller
SRCS = controller.cpp
OBJS = $(SRCS:.cpp=.o)
CC = g++
LIBS = -lkyototycoon

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

.cpp.o:
	$(CC) -c $<

clean:
	rm -f $(TARGET) $(OBJS)
