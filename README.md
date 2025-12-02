# Epoll Server

## Сборка проекта

```bash
make
```
## Запуск
### Запуск сервера:

```bash
./epoll_server_cpp
```

### Запуск примера 1000 подключений:

```bash
./tcp_client_cpp
```
## Проверка UDP/TCP подключения

```bash
echo "Hello UDP" | nc -u 127.0.0.1 8080
echo "Hello TCP" | nc  127.0.0.1 8080
echo "/time" | nc  127.0.0.1 8080
echo "/stats" | nc  127.0.0.1 8080
echo "/shutdown" | nc  127.0.0.1 8080
```
