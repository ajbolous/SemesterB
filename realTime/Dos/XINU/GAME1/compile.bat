tcc -I.. -c clkint.c
tlib xinu.lib -+ clkint.obj
tcc -I.. game1.c xinu.lib