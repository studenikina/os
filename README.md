# os

ex1
host$ ./simplesh
> $
< ls
> <содержимое директории>
> $
//< cat /proc/cpuinfo | grep model | grep name | sed -re s/.*:\s(.*)/\1/ | uniq
< sysctl -n machdep.cpu.brand_string
> <модель процессора>
> $
< ^D
ex2
host$ ./simplesh
> $
< ls
> <содержимое директории>
> $
< cat /proc/cpuinfo | grep model | grep name | sed -re s/.*:\s(.*)/\1/ | uniq
> <модель процессора>
> $
< ^D
< и > символизируют ввод и вывод и показаны исключительно для наглядности.
ex3
host$ ./simplesh
> $
< cat | wc -l
< first line
< second line
< third line
< ^D
> 3
> $
< cat|wc -l
< first
< second
< third
< ^D
> 3
> $
< ^D
