# Завдання 1

## Умова:

Використайте popen(), щоб передати вивід команди rwho (команда UNIX) до more (команда UNIX) у програмі на C.

## [Програмний код](1.c)

## Опис:

З використанням rwho виникла проблема: програма не дає виводу, а команди типу who чи w працюють нормально
(видно [тут](#Проблема-з-rwho)). Також перевірка роботи чи наявності демона rwhod (відповідає за команду rwho)
показала, що він не запущений та не встановлений. Через pkg install він не встановлюється, тому його очевидне
використання не можливе в мене. rwho має виводити залогінених в систему користувачів (кілька рядків тексту,
подібно до who чи w), more - для переглядання тексту по рядкам (enter) та сторінкам (пробіл).
ps aux видає купу тексту, що є непоганою демонстрацією функціоналу more, тому rwho замінено на ps aux

таким чином написано програму, де за допомогою popen() (відкриває новий процес, створює pipe ("трубу"), яка
під'єднує процес з переданою командою) отримується вивід команди ps aux (виводить інформцію про всі активні
процеси) і записується у буфер buff. Далі цей вивід передається у pipe, який був створений з командою more,
що дозволяє переглядати цей вивід частково (скільки влазить у поточне віконце) та гортати його.

## Результат виконання:

### Проблема з rwho:

```
max@aspz:~/c/7 $ rwho
max@aspz:~/c/7 $ who
max              pts/0        May 13 10:15 (:0)
max              pts/1        May 13 10:15 (:0)
max              pts/2        May 13 10:15 (:0)
max              pts/3        May 13 10:24 (:0)
max@aspz:~/c/7 $ w
11:27AM  up  1:13, 4 users, load averages: 0.49, 0.53, 0.43
USER       TTY      FROM    LOGIN@  IDLE WHAT
max        pts/0    :0     10:15AM  1:12 -
max        pts/1    :0     10:15AM  1:12 /bin/sh
max        pts/2    :0     10:15AM     - w
max        pts/3    :0     10:24AM    23 /bin/sh
max@aspz:~/c/7 $ pgrep rwhod
max@aspz:~/c/7 $ sudo service rwhod onestart
rwhod does not exist in /etc/rc.d or the local startup
directories (/usr/local/etc/rc.d), or is not executable
max@aspz:~/c/7 $ sudo /etc/rc.d/rwhod onestart
sudo: /etc/rc.d/rwhod: command not found
max@aspz:~/c/7 $ sudo pkg update
Updating FreeBSD repository catalogue...
Fetching data.pkg: 100%   10 MiB 135.5kB/s    01:17
Processing entries: 100%
FreeBSD repository update completed. 35955 packages processed.
All repositories are up to date.
max@aspz:~/c/7 $ pkg search rwho
max@aspz:~/c/7 $ sudo pkg install rwho
Updating FreeBSD repository catalogue...
FreeBSD repository is up to date.
All repositories are up to date.
pkg: No packages available to install matching 'rwho' have been found in the repositories

```

### Виконання програми з ps aux замість rwho:

```
SER        PID  %CPU %MEM       VSZ    RSS TT  STAT STARTED      TIME COMMAND
root         11 400.0  0.0         0     64  -  RNL  10:14   291:17.96 [idle]
root        897   4.0  5.7    644360 476208  -  S    10:15     1:31.94 /usr/local/libexec/Xorg -nolisten tcp -background none -seat seat0 vt9 -auth /var/run/sddm/xauth_CdiLkK -noreset -displayfd 16
max         947   4.0  7.8   1199672 652556  -  S    10:15     2:14.48 /usr/local/bin/kwin_x11
max        1043   4.0  2.6    508980 218100  -  S    10:15     0:39.74 /usr/local/bin/kate -b /home/max/c/shittemp.txt
root          0   0.0  0.0         0    592  -  DLs  10:14     0:01.36 [kernel]
root          1   0.0  0.0     12724   1196  -  ILs  10:14     0:00.00 /sbin/init
root          2   0.0  0.0         0     64  -  WL   10:14     0:01.25 [clock]
root          3   0.0  0.0         0     80  -  DL   10:14     0:00.00 [crypto]
root          4   0.0  0.0         0     48  -  DL   10:14     0:01.64 [cam]
root          5   0.0  0.0         0     16  -  DL   10:14     0:00.00 [busdma]
root          6   0.0  0.0         0     16  -  DL   10:14     0:08.86 [rand_harvestq]
root          7   0.0  0.0         0     48  -  DL   10:14     0:03.29 [pagedaemon]
--More--(byte 1218)

```

# Завдання 2

## Умова:

Напишіть програму мовою C, яка імітує команду ls -l в UNIX — виводить список усіх файлів у поточному
каталозі та перелічує права доступу тощо. (Варіант вирішення, що просто виконує ls -l із вашої
програми, — не підходить.)

## [Програмний код](2.c)

## Опис:

Створено програму, яка імітує вивід команда ls -l шляхом отримання всіх даних що виводить ця команда
та виводом їх у правильному порядку. Саме у наступному порядку виводяться: права доступу з типом файлу
(рядок типу -rw-r--r--, збудований побуквенно у функції print_perms), к-ть хард лінків (з'єднання назви
з адресою пам'яті у системі), користувач, група, розмір, дата та час останнього редагування, та назва.
У результатах роботи також запущено команду ls -l для порівняння, що показує коректність роботи програми
та, за вийнятком дизайну та порядку виведення файлів, ідентичності двох виводів.

## Результат виконання:

```
max@aspz:~/c/7 $ gcc -Wall 2.c -o 2
max@aspz:~/c/7 $ ./2
-rwxr-xr-x   1 max      max          9000 May 13 11:24 1
-rwxr-xr-x   1 max      max         10568 May 15 23:07 2
-rw-r--r--   1 max      max          6698 May 13 11:48 someshit.txt
-rw-r--r--   1 max      max          1595 May 13 11:47 1.c
-rw-r--r--   1 max      max          3047 May 15 23:07 2.c
max@aspz:~/c/7 $ ls -l
total 40
-rwxr-xr-x  1 max max  9000 May 13 11:24 1
-rw-r--r--  1 max max  1595 May 13 11:47 1.c
-rwxr-xr-x  1 max max 10568 May 15 23:07 2
-rw-r--r--  1 max max  3047 May 15 23:07 2.c
-rw-r--r--  1 max max  6698 May 13 11:48 someshit.txt

```

# Завдання 3

## Умова:

Напишіть програму, яка друкує рядки з файлу, що містять слово, передане як аргумент програми (проста версія утиліти grep в UNIX).

## [Програмний код](3.c)

## Опис:

Створено файл для читання [yuh.txt](yuh.txt) для тестування та заповнено текстом
Написано програму для виведення тих рядків з текстових документів, які містять слово, передане до
програми при запуску. Окрім цього слова в аргументи також передається назва файлу, що потім перевіряється.
Знаходження реалізоване шляхом зчитування файлу по рядку у line, які потім передаються разом з шуканим
словом у ф-ю strstr, яка перевіряє, чи є це слово/патерн у рядку (прям як grep). Далі також запущено unix
утиліту grep для порівняння функціоналу, що показало ідентичність виводу моєї програми та grep.

## Результат виконання:

```
max@aspz:~/c/7 $ gcc -Wall 3.c -o 3
max@aspz:~/c/7 $ ./3
Need 2 args: word for search, file
max@aspz:~/c/7 $ ./3 crazy yuh.txt
Crazy? I was crazy once.
And rats make me crazy.
max@aspz:~/c/7 $ grep crazy yuh.txt
Crazy? I was crazy once.
And rats make me crazy.
```

# Завдання 4

## Умова:

Напишіть програму, яка виводить список файлів, заданих у вигляді аргументів, з зупинкою кожні 20 рядків, доки не буде натиснута клавіша (спрощена версія утиліти more в UNIX).

## [Програмний код](4.c)

## Опис:

Схоже до юніксовської команди more, але з простішим функціоналом створено програму, що дозволяє
посторінково (розмір сторінки задано як 5 для простішого приведення прикладів) листати файли.
Також можна переходити до наступних файлівяк після пролистування всіх сторінок, так і після
натискання n. Також можна завершити читати файл на q. Це реалізовано за допомогою порядкового
читання файлів та рахування рядків при їх виводі, а також виведення підказки, подібно до more.
Для тестування написані два файли: [huh.txt](huh.txt) [wuh.txt](wuh.txt), які заповнено текстом
на 13 та 11 рядків відповідно для можливості гортання.

## Результат виконання:

```
max@aspz:~/c/7 $ gcc -Wall 4.c -o 4
max@aspz:~/c/7 $ ./4 huh.txt wuh.txt
-----
huh.txt
-----
From the moment I understood the weakness of my flesh,
it disgusted me.
I craved the strength and certainty of steel.
I aspired to the purity of the blessed machine.
Your kind cling to your flesh
---Enter to continue, 'q' to exit file, 'n' to next file---
as if it will not decay and fail you.
One day the crude biomass you call a temple
will wither
and you will beg my kind to save you.
But I am already saved.
---Enter to continue, 'q' to exit file, 'n' to next file---n
...Going to next file...

---Enter to next file---

-----
wuh.txt
-----
The itsy bitsy spider
climbed up the waterspout.
Down came the rain
And washed
the spider out.
---Enter to continue, 'q' to exit file, 'n' to next file---q
...Closing wuh.txt...

max@aspz:~/c/7 $
```

# Завдання 5

## Умова:

Напишіть програму, яка перелічує всі файли в поточному каталозі та всі файли в підкаталогах.

## [Програмний код](5.c)

## Опис:

Написано програму, що виводить усі файли у поточній папці, а також файли у папках, що знаходяться
у ній.Для цього створюється структура dirent (тип для роботи з файловою системою) який зчитується
з поточної директорії (отриманої як тип DIR *) за допомогою readdir. Перевіряється d_type кожного
файлу, і якщо це папка (DT_DIR), то функція виводу файлів у папці (print_files) викликається для неї.
Для тестування створено папку [folder](folder) з кількома пустими файлами.

## Результат виконання:

```
max@aspz:~/c/7 $ gcc -Wall 5.c -o 5
max@aspz:~/c/7 $ ./5
Files in this catalog and daughter catalogs:
------------
./1
./2
./someshit.txt
./1.c
./2.c
./yuh.txt
./3.c
./3
./huh.txt
./4.c
./4
./5
./5.c
./folder
./folder/folder_file3.txt
./folder/folder_file2.txt
./folder/folder_file1.txt
./wuh.txt
max@aspz:~/c/7 $
```

# Завдання 6

## Умова:

Напишіть програму, яка перелічує лише підкаталоги у алфавітному порядку.

## [Програмний код](6.c)

## Опис:

Написана програма обходить всі підкаталоги та збирає їх у один масив dirs. Далі цей масив
сортується qsort та написана функція compare у алфавітному порядку. Це все виконує функція
print_dirs_sorted, яка також викликається рекурсивно при виведенні директорій для того,
щоб вивести можливі піддиректорії у них.

## Результат виконання:

```
max@aspz:~/c/7 $ ./6
Sub dirs sorted:
------------
./bolder
./bolder/colder
./bolder/dolder
./folder
./molder
max@aspz:~/c/7 $
```

# Завдання 7

## Умова:

Напишіть програму, яка показує користувачу всі його/її вихідні програми на C, а потім в інтерактивному режимі запитує, чи потрібно надати іншим дозвіл на читання (read permission); у разі ствердної відповіді — такий дозвіл повинен бути наданий.

## [Програмний код](7.c)

## Опис:

Програма шукає та виводить всі .с файли користувача у поточній та дочірніх директоріях. Також після
кожного виводиться інформація про те, чи є у інших користувачів доступ до нього. Це виконано
шляхом використання lstat для отриманя даних про доступ для файлу, та додавання атрибуту S_IROTH
(доступ для інших) до навого режиму (mode_t) файлу, який застосовується через chmod(). Для тестування
Заборонено доступ до 1.с вручну, далі змінено його програмою і перевірено правильність другим запуском.

## Результат виконання:

```
ax@aspz:~/c/7 $ ./7
Searching C files...
-------
Your C file at ./1.c
  Give access for others? (y/n): y
  Access for others givenYour C file at ./2.c
  Access already given
Your C file at ./3.c
  Access already given
Your C file at ./4.c
  Access already given
Your C file at ./5.c
  Access already given
Your C file at ./7.c
  Access already given
-------
max@aspz:~/c/7 $ ./7
Searching C files...
-------
Your C file at ./1.c
  Access already given
Your C file at ./2.c
  Access already given
Your C file at ./3.c
  Access already given
Your C file at ./4.c
  Access already given
Your C file at ./5.c
  Access already given
Your C file at ./7.c
  Access already given
-------
max@aspz:~/c/7 $
```

# Завдання 8

## Умова:

Напишіть програму, яка надає користувачу можливість видалити будь-який або всі файли у поточному робочому каталозі. Має з’являтися ім’я файлу з запитом, чи слід його видалити.

## [Програмний код](8.c)

## Опис:

Створено програму, яка перелічує всі файли у поточній директорії та запитує, чи бажаї користувач видалити
кожен з файлів окремо? Видається повідомлення типу (y/n) та, при введенні y, цей файл видаляється. Для
тестування створено test.txt, наявність якого перед запуском перевірено командою ls -l далі після запуску
та вибору видалення цього файлу, ls -l показує його відсутність.

## Результат виконання:

```
max@aspz:~/c/7 $ ls -l
total 176
-rwxr-xr-x  1 max max  9000 May 13 11:24 1
-rw-r--r--  1 max max  1595 May 13 11:47 1.c
-rwxr-xr-x  1 max max 10568 May 15 23:07 2
-rw-r--r--  1 max max  3047 May 15 23:07 2.c
-rwxr-xr-x  1 max max  9192 May 15 23:36 3
-rw-r--r--  1 max max   774 May 15 23:36 3.c
-rwxr-xr-x  1 max max 11224 May 16 01:46 4
-rw-r--r--  1 max max  2370 May 16 01:46 4.c
-rwxr-xr-x  1 max max  9480 May 16 10:26 5
-rw-r--r--  1 max max  1429 May 16 10:26 5.c
-rwxr-xr-x  1 max max 10072 May 16 11:19 6
-rw-r--r--  1 max max  1609 May 16 11:24 6.c
-rwxr-xr-x  1 max max 11872 May 16 11:00 7
-rw-r--r--  1 max max  2847 May 16 11:00 7.c
-rwxr-xr-x  1 max max  9488 May 16 12:21 8
-rw-r--r--  1 max max   985 May 16 12:21 8.c
drwxr-xr-x  4 max max   512 May 16 11:20 bolder
drwxr-xr-x  2 max max   512 May 16 10:13 folder
-rw-r--r--  1 max max   418 May 16 01:45 huh.txt
drwxr-xr-x  2 max max   512 May 16 11:21 molder
-rw-r--r--  1 max max 16532 May 16 11:26 someshit.txt
-rw-r--r--  1 max max     2 May 16 12:06 test.txt
-rw-r--r--  1 max max   192 May 16 01:46 wuh.txt
-rw-r--r--  1 max max   115 May 15 23:27 yuh.txt
max@aspz:~/c/7 $ gcc -Wall 8.c -o 8
max@aspz:~/c/7 $ ./8
Delete "1"? (y/n): n
Delete "2"? (y/n): n
Delete "someshit.txt"? (y/n): n
Delete "1.c"? (y/n): n
Delete "2.c"? (y/n): n
Delete "yuh.txt"? (y/n): n
Delete "3.c"? (y/n): n
Delete "3"? (y/n): n
Delete "huh.txt"? (y/n): n
Delete "4.c"? (y/n): n
Delete "4"? (y/n): n
Delete "5"? (y/n): n
Delete "5.c"? (y/n): n
Delete "wuh.txt"? (y/n): n
Delete "7"? (y/n): n
Delete "7.c"? (y/n): n
Delete "6"? (y/n): n
Delete "6.c"? (y/n): n
Delete "test.txt"? (y/n): y
File "test.txt" deleted
Delete "8.c"? (y/n): n
Delete "8"? (y/n): n
max@aspz:~/c/7 $ n
/bin/sh: n: not found
max@aspz:~/c/7 $ n
/bin/sh: n: not found
max@aspz:~/c/7 $ ls -l
total 172
-rwxr-xr-x  1 max max  9000 May 13 11:24 1
-rw-r--r--  1 max max  1595 May 13 11:47 1.c
-rwxr-xr-x  1 max max 10568 May 15 23:07 2
-rw-r--r--  1 max max  3047 May 15 23:07 2.c
-rwxr-xr-x  1 max max  9192 May 15 23:36 3
-rw-r--r--  1 max max   774 May 15 23:36 3.c
-rwxr-xr-x  1 max max 11224 May 16 01:46 4
-rw-r--r--  1 max max  2370 May 16 01:46 4.c
-rwxr-xr-x  1 max max  9480 May 16 10:26 5
-rw-r--r--  1 max max  1429 May 16 10:26 5.c
-rwxr-xr-x  1 max max 10072 May 16 11:19 6
-rw-r--r--  1 max max  1609 May 16 11:24 6.c
-rwxr-xr-x  1 max max 11872 May 16 11:00 7
-rw-r--r--  1 max max  2847 May 16 11:00 7.c
-rwxr-xr-x  1 max max  9488 May 16 12:22 8
-rw-r--r--  1 max max   985 May 16 12:21 8.c
drwxr-xr-x  4 max max   512 May 16 11:20 bolder
drwxr-xr-x  2 max max   512 May 16 10:13 folder
-rw-r--r--  1 max max   418 May 16 01:45 huh.txt
drwxr-xr-x  2 max max   512 May 16 11:21 molder
-rw-r--r--  1 max max 16532 May 16 11:26 someshit.txt
-rw-r--r--  1 max max   192 May 16 01:46 wuh.txt
-rw-r--r--  1 max max   115 May 15 23:27 yuh.txt
max@aspz:~/c/7 $
```

# Завдання 9

## Умова:

Напишіть програму на C, яка вимірює час виконання фрагмента коду в мілісекундах.

## [Програмний код](9.c)

## Опис:

Дуже проста програма, створена за допомогою функції clock(), яка повертає тип clock_t(). Ця команда
застосовується для отримання часу перед виконанням сегменту коду та після, які потім порівнюються
і різниця використовується для отримання результату у мс. Як сегмент коду для замірювання часу створено
ф-ю garbage яка просто дуже багато разів інкрементує число, тим самим займаючи час.

## Результат виконання:

```
max@aspz:~/c/7 $ gcc -Wall 9.c -o 9
max@aspz:~/c/7 $ ./9
Time taken: 109.38 ms
max@aspz:~/c/7 $
```

# Завдання 10

## Умова:

Напишіть програму мовою C для створення послідовності випадкових чисел з плаваючою комою у діапазонах:
 (a) від 0.0 до 1.0
 (b) від 0.0 до n, де n — будь-яке дійсне число з плаваючою точкою.
 Початкове значення генератора випадкових чисел має бути встановлене так, щоб гарантувати унікальну послідовність.
Примітка: використання прапорця -Wall під час компіляції є обов’язковим.

## [Програмний код](10.c)

## Опис:

У написаній програмі генеруються випадкові числа від 0 до 1 ф-єю rand_0_1, та від 1 до якогось
заданого користувацьким вводом числа ф-єю rand_0_n. Ділення рандомного числа з rand() на маскимальне
значення рандому гарантує отримання дробового числа від 0 до 1. Якщо його помножити на число n, то
можна отримати рандом від 0 * n = 0 до n (1 * n = n), що робить друга функція. Гарантування унікальної
послідовності рандому реалізовано простим задаванням сім'ячка srand() поточним значенням time().

## Результат виконання:

```
max@aspz:~/c/7 $ gcc -Wall 10.c -o 10
max@aspz:~/c/7 $ ./10
Random from 0 to 1:
0.4949
0.7517
0.5856
0.1710
0.0431
0.9618
0.3606

Enter n: 17.3

Random from 0 to 17.30:
0.9058
10.3533
7.5793
12.1506
2.1276
16.2031
4.1796

max@aspz:~/c/7 $
```

# Завдання 11 (варіант 18)

## Умова:

Створіть команду, яка дає змогу "перемотати назад" виконання shell-команд (але не через історію).

## [Програмний код](11.c)

## Опис:

Не використовуючи історію, створено програму для перемотки назад у виконанні команд, за допомогою .sh
лог-файлу, який зберігає у собі попередньо відтворені команди. Ці команди користувач вводить у імітацію
командної строки, у яку можна вводити як звичайні команди, які виконуються у exec() за допомогою system(),
але і replay для відтворення минулих команд та exit для виходу з цього режиму. Для перевірки роботи
введено таким чином ls -l та who, які виконалися та після replay відтворилися знову. Коректну роботу
програми видно у результатах виконання.

## Результат виконання:

```
max@aspz:~/c/7 $ gcc -Wall 11.c -o 11
max@aspz:~/c/7 $ ./11
---Enter 'exit' to exit, 'replay' to replay---
cmd> ls -l
total 224
-rwxr-xr-x  1 max max  9000 May 13 11:24 1
-rw-r--r--  1 max max  1595 May 13 11:47 1.c
-rwxr-xr-x  1 max max  9624 May 16 12:57 10
-rw-r--r--  1 max max   991 May 16 12:57 10.c
-rwxr-xr-x  1 max max 10344 May 16 13:24 11
-rw-r--r--  1 max max  2130 May 16 13:24 11.c
-rwxr-xr-x  1 max max 10568 May 15 23:07 2
-rw-r--r--  1 max max  3047 May 15 23:07 2.c
-rwxr-xr-x  1 max max  9192 May 15 23:36 3
-rw-r--r--  1 max max   774 May 15 23:36 3.c
-rwxr-xr-x  1 max max 11224 May 16 01:46 4
-rw-r--r--  1 max max  2370 May 16 01:46 4.c
-rwxr-xr-x  1 max max  9480 May 16 10:26 5
-rw-r--r--  1 max max  1429 May 16 10:26 5.c
-rwxr-xr-x  1 max max 10072 May 16 11:19 6
-rw-r--r--  1 max max  1609 May 16 11:24 6.c
-rwxr-xr-x  1 max max 11872 May 16 11:00 7
-rw-r--r--  1 max max  2847 May 16 11:00 7.c
-rwxr-xr-x  1 max max  9488 May 16 12:22 8
-rw-r--r--  1 max max   985 May 16 12:21 8.c
-rwxr-xr-x  1 max max  8576 May 16 12:38 9
-rw-r--r--  1 max max   642 May 16 12:38 9.c
drwxr-xr-x  4 max max   512 May 16 11:20 bolder
drwxr-xr-x  2 max max   512 May 16 10:13 folder
-rw-r--r--  1 max max   418 May 16 01:45 huh.txt
drwxr-xr-x  2 max max   512 May 16 11:21 molder
-rw-r--r--  1 max max 22771 May 16 13:02 someshit.txt
-rw-r--r--  1 max max   192 May 16 01:46 wuh.txt
-rw-r--r--  1 max max   115 May 15 23:27 yuh.txt
cmd> who
max              pts/0        May 16 09:49 (:0)
max              pts/1        May 16 09:50 (:0)
max              pts/2        May 16 09:50 (:0)
cmd> replay
-------Replaying-------
<<rewind_time<< ls -l
total 224
-rwxr-xr-x  1 max max  9000 May 13 11:24 1
-rw-r--r--  1 max max  1595 May 13 11:47 1.c
-rwxr-xr-x  1 max max  9624 May 16 12:57 10
-rw-r--r--  1 max max   991 May 16 12:57 10.c
-rwxr-xr-x  1 max max 10344 May 16 13:24 11
-rw-r--r--  1 max max  2130 May 16 13:24 11.c
-rwxr-xr-x  1 max max 10568 May 15 23:07 2
-rw-r--r--  1 max max  3047 May 15 23:07 2.c
-rwxr-xr-x  1 max max  9192 May 15 23:36 3
-rw-r--r--  1 max max   774 May 15 23:36 3.c
-rwxr-xr-x  1 max max 11224 May 16 01:46 4
-rw-r--r--  1 max max  2370 May 16 01:46 4.c
-rwxr-xr-x  1 max max  9480 May 16 10:26 5
-rw-r--r--  1 max max  1429 May 16 10:26 5.c
-rwxr-xr-x  1 max max 10072 May 16 11:19 6
-rw-r--r--  1 max max  1609 May 16 11:24 6.c
-rwxr-xr-x  1 max max 11872 May 16 11:00 7
-rw-r--r--  1 max max  2847 May 16 11:00 7.c
-rwxr-xr-x  1 max max  9488 May 16 12:22 8
-rw-r--r--  1 max max   985 May 16 12:21 8.c
-rwxr-xr-x  1 max max  8576 May 16 12:38 9
-rw-r--r--  1 max max   642 May 16 12:38 9.c
drwxr-xr-x  4 max max   512 May 16 11:20 bolder
drwxr-xr-x  2 max max   512 May 16 10:13 folder
-rw-r--r--  1 max max   418 May 16 01:45 huh.txt
drwxr-xr-x  2 max max   512 May 16 11:21 molder
-rw-r--r--  1 max max 22771 May 16 13:02 someshit.txt
-rw-r--r--  1 max max   192 May 16 01:46 wuh.txt
-rw-r--r--  1 max max   115 May 15 23:27 yuh.txt
<<rewind_time<< who
max              pts/0        May 16 09:49 (:0)
max              pts/1        May 16 09:50 (:0)
max              pts/2        May 16 09:50 (:0)
-------Ended replaying-------
cmd> exit
max@aspz:~/c/7 $
```
