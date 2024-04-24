exp2:
	@lex ip.l
	@yacc -d ip.y
	@gcc lex.yy.c y.tab.c -o ip -ll
	@./ip

inpo:
	@lex intopo.l
	@yacc -d intopo.y
	@gcc lex.yy.c y.tab.c -o intopo -ll
	@./intopo