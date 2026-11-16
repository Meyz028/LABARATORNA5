# Лабораторна робота 5 — hello1 / hello2

## Структура
```
Makefile
hello1.c      # експортує print_hello(), список + ktime_t, друк часу при rmmod
hello2.c      # параметр count, викликає print_hello()
inc/hello1.h  # спільний заголовок (ccflags-y += -I$(src)/inc)
```

## Збірка
```
make KDIR=$HOME/repos/linux-stable   # крос-збірка ARM (BeagleBone), як у лабі 4
modinfo hello1.ko
modinfo hello2.ko
```

## Рівні повідомлень (hello2, параметр count)
| count  | Результат                          |
|--------|------------------------------------|
| 1..4   | pr_info (привітання) — ОК          |
| 0      | pr_warn, модуль завантажується     |
| 5..10  | pr_warn, привітання, завантажується|
| >10    | pr_err, `-EINVAL`, не завантажується |

## Запуск (п. IV)
```
sudo insmod hello1.ko
sudo insmod hello2.ko count=0   ; sudo rmmod hello2     # warn
sudo insmod hello2.ko count=3   ; sudo rmmod hello2     # info
sudo insmod hello2.ko count=7   ; sudo rmmod hello2     # warn + info
sudo insmod hello2.ko count=11                          # err (insmod: Invalid parameters)
cat /sys/module/hello2/parameters/count                 # значення параметра
sudo rmmod hello1                                       # друк часу кожного виклику
sudo dmesg | grep -E "hello|Hello"
sudo dmesg --level=err,warn
```

## П. V — hello2.ko без hello1.ko
```
sudo rmmod hello2 hello1   # якщо завантажені
sudo insmod hello2.ko
# insmod: ERROR: could not insert module hello2.ko: Unknown symbol in module
sudo dmesg | tail
# hello2: Unknown symbol print_hello (err -2)
```
**Пояснення:** `hello2` використовує функцію `print_hello()`, яку експортує
(`EXPORT_SYMBOL`) тільки `hello1`. Поки `hello1` не завантажений, символу немає
в таблиці символів ядра, лінкер модулів не може розв'язати посилання, тому
завантаження `hello2` завершується помилкою (`Unknown symbol`, errno `ENOENT`).
`insmod` не підтягує залежності (на відміну від `modprobe`); після
`depmod` + `modprobe hello2` hello1 завантажився б автоматично.
Також `rmmod hello1` неможливий, поки завантажений `hello2` (лічильник посилань).
