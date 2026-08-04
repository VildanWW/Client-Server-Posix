## Сборка и запуск проекта (POSIX / Linux / WSL, Qt)

Реализация простого чата (Client-Server) уровня Junior без использования сторонних сетевых библиотек, основанная на принципах Posix.

### 1. Компиляция Сервера
```bash
wsl g++ Server/main.cpp Server/Server.cpp Server/UserSession.cpp -o server_app
```

### 3. Запуск экосистемы

1. В первом терминале запустите серверную часть:
```bash
./server_app
```
