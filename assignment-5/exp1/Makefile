exp1:
	@lex file.l
	@yacc -d file.y
	@gcc lex.yy.c y.tab.c -o file -ll
	@./file

# input
# a=b*c+1/3-5*f;


three:
	@lex 3address.l
	@yacc -d 3address.y
	@gcc lex.yy.c y.tab.c -o 3address -ll
	@./3address