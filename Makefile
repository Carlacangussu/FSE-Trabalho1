dist:
	@gcc andar1.c -o andar1 -lbcm2835 -lpthread && gcc andar2.c -o andar2 -lbcm2835 -lpthread
	@./andar1 && ./andar2 
central:
	@gcc central.c -o central -lpthread
	@./central