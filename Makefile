CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I.
LDFLAGS = 
LIBS = -lstdc++

TARGET = epoll_server_cpp
SOURCES = main.cpp EpollServer.cpp TcpListener.cpp UdpSocket.cpp TcpClient.cpp utils.cpp 
BUILD_DIR = build
OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)