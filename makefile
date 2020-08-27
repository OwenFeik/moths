program: *.c *.h
	gcc main.c util.c -o program -l GL -l glut -l m -I /usr/include/freetype2
