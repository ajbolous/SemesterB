tcc -I.. -c clkint.c
tlib xinu.lib -+ clkint.obj
tcc -I.. game2.c xinu.lib