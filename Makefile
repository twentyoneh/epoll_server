CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I.
LDFLAGS = 
LIBS = -lstdc++

TARGET = epoll_server_cpp
SOURCES = main.cpp EpollServer.cpp TcpListener.cpp UdpSocket.cpp TcpClient.cpp utils.cpp 
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)