## Сборка и запуск проекта (POSIX / Linux / WSL)

Реализация простого чата (Client-Server) уровня Junior без использования сторонних сетевых библиотек, основанная на принципах Posix.

### 1. Компиляция Сервера
```bash
wsl g++ Server/main.cpp Server/Server.cpp Server/UserSession.cpp -o server_app -pthread
```

### 2. Компиляция Клиента
```bash
wsl g++ Client/main.cpp -o client_app
```

### 3. Запуск экосистемы

1. В первом терминале запустите серверную часть:
```bash
./server_app
```

2. Во втором терминале запустите клиент:
```bash
./client_app
```
