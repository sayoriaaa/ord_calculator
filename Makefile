ord:src/o.l src/o.y src/o.h
	bison -d src/o.y 
	flex -o o.lex.c src/o.l
	gcc -o $@ o.tab.c o.lex.c src/o.c