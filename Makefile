CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I.
LDFLAGS = 
LIBS = -lstdc++

# Сервер
SERVER_TARGET = epoll_server_cpp
SERVER_SOURCES = srcServer/main.cpp \
                 srcServer/EpollServer.cpp \
                 srcServer/TcpListener.cpp \
                 srcServer/UdpSocket.cpp \
                 srcServer/TcpClient.cpp \
                 srcServer/utils.cpp
SERVER_BUILD_DIR = build_server
SERVER_OBJECTS = $(SERVER_SOURCES:%.cpp=$(SERVER_BUILD_DIR)/%.o)

# Клиент
CLIENT_TARGET = tcp_client_cpp
CLIENT_SOURCES = srcClient/TcpClient.cpp
CLIENT_BUILD_DIR = build_client
CLIENT_OBJECTS = $(CLIENT_SOURCES:%.cpp=$(CLIENT_BUILD_DIR)/%.o)

.PHONY: all server client clean clean-all

# Основные цели
all: server client

# Сборка сервера
server: $(SERVER_BUILD_DIR) $(SERVER_TARGET)

# Сборка клиента
client: $(CLIENT_BUILD_DIR) $(CLIENT_TARGET)

# Создание директорий сборки
$(SERVER_BUILD_DIR):
	@mkdir -p $(SERVER_BUILD_DIR)

$(CLIENT_BUILD_DIR):
	@mkdir -p $(CLIENT_BUILD_DIR)

# Сервер
$(SERVER_TARGET): $(SERVER_OBJECTS)
	$(CXX) $(LDFLAGS) $(SERVER_OBJECTS) -o $@ $(LIBS)

# Клиент
$(CLIENT_TARGET): $(CLIENT_OBJECTS)
	$(CXX) $(LDFLAGS) $(CLIENT_OBJECTS) -o $@ $(LIBS)

# Правила компиляции
$(SERVER_BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(CLIENT_BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean-server:
	rm -rf $(SERVER_BUILD_DIR) $(SERVER_TARGET)

clean-client:
	rm -rf $(CLIENT_BUILD_DIR) $(CLIENT_TARGET)

clean: clean-server clean-client

# Пересборка
rebuild: clean all

rebuild-server: clean-server server

rebuild-client: clean-client client