CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I.
LDFLAGS = 
LIBS = -lstdc++

TARGET = epoll_server_cpp
SOURCES = srcServer/main.cpp \
          srcServer/EpollServer.cpp \
          srcServer/TcpListener.cpp \
          srcServer/UdpSocket.cpp \
          srcServer/TcpClient.cpp \
          srcServer/utils.cpp
BUILD_DIR_SERVER = build
OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR_SERVER)/%.o)

.PHONY: all clean

all: $(BUILD_DIR_SERVER) $(TARGET)

$(BUILD_DIR_SERVER):
	@mkdir -p $(BUILD_DIR_SERVER)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

$(BUILD_DIR_SERVER)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR_SERVER) $(TARGET)