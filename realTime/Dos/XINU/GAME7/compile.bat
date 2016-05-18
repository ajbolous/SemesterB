tcc -I.. -c initiali.c
tcc -I.. -c clkint.c
tlib xinu.lib -+ initiali.obj
tlib xinu.lib -+ clkint.obj
tcc -I.. game7b.c xinu.lib
game7b.exe