import os
import sys

if __name__ == "__main__":
	commands = ""
	
	dir = sys.argv[1]
	exe = sys.argv[2]
	
	for file in os.listdir(dir):
		if exe not in file and file.endswith(".c"):
			commands= commands + "tcc -I.. -c {}\n".format(file)
			commands= commands + "tlib xinu.lib -+ {}\n".format(file.replace(".c",".obj"))
			
	commands = commands + "tcc -I.. {} xinu.lib".format(exe)

		
	outfile = open(dir + "/compile.bat",'w')
	outfile.write(commands)
