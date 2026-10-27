# Drivers

Репозиторий для самообучения разработке драйверов Linux.

## Содержимое

- `01_hello_world/` — первый модуль ядра (Hello World). Выводит приветствие в dmesg при загрузке и прощание при выгрузке.

## Сборка и запуск

```bash
cd 01_hello_world
make
sudo insmod hello.ko
sudo dmesg | tail -3   # "Привет, ядро! Модуль загружен."
sudo rmmod hello
sudo dmesg | tail -3   # "Пока, ядро! Модуль выгружен."
make clean
