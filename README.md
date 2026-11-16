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
make KDIR=$HOME/repos/linux-stable   # крос-збірка ARM (ядро 4.19.325, як у лабі 4)
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
insmod hello1.ko
insmod hello2.ko count=0   ; rmmod hello2     # warn
insmod hello2.ko count=3   ; rmmod hello2     # info
insmod hello2.ko count=7   ; rmmod hello2     # warn + info
insmod hello2.ko count=11                          # err (insmod: can't insert 'hello2.ko': invalid parameter)
cat /sys/module/hello2/parameters/count                 # значення параметра
rmmod hello1                                       # друк часу кожного виклику
dmesg | grep -E "hello|Hello"
dmesg --level=err,warn
```

## П. V — hello2.ko без hello1.ko
```
rmmod hello2 hello1   # якщо завантажені
insmod hello2.ko
# insmod: can't insert 'hello2.ko': unknown symbol in module or invalid parameter
dmesg | tail
# hello2: Unknown symbol print_hello (err -2)
```
**Пояснення:** `hello2` використовує функцію `print_hello()`, яку експортує
(`EXPORT_SYMBOL`) тільки `hello1`. Поки `hello1` не завантажений, символу немає
в таблиці символів ядра, лінкер модулів не може розв'язати посилання, тому
завантаження `hello2` завершується помилкою (`Unknown symbol`, errno `ENOENT`).
`insmod` не підтягує залежності (на відміну від `modprobe`); після
`depmod` + `modprobe hello2` hello1 завантажився б автоматично.
Також `rmmod hello1` неможливий, поки завантажений `hello2` (лічильник посилань).
