tcc -I.. -c clkint.c
tlib xinu.lib -+ clkint.obj
tcc -I.. pacman.c xinu.lib