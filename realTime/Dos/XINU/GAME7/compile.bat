
tcc -c -I.. -DTURBOC CLKINT.C INITIALI.c >>a.txt

tlib xinu.lib -+ initiali.obj
tlib xinu.lib -+ clkint.obj
tcc -I.. game7b.c xinu.lib
game7b.exe