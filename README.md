# Drivers

Репозиторий для самообучения разработке драйверов Linux.

## Содержимое

- `01_hello_world/` — первый модуль ядра (Hello World). Выводит приветствие в dmesg.

## Сборка и запуск

```bash
cd 01_hello_world
make
sudo insmod hello.ko
sudo dmesg | tail -3
sudo rmmod hello
sudo dmesg | tail -3
make clean
