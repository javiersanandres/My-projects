########################################################################################################
#
# $@ es el item que aparece a la izquierda de ':'
# $< es el primer item en la lista de dependencias
# $^ son todos los archivos que se encuentran a la derecha de ':' (dependencias)
#
#-------------------------------------------------------------------------------------------------------
# Compilacion durante desarrollo:
# 	%gcc -ansi -pedantic -Wall -o nombre_prog prog.c
# 
# Compilacion para test/depuracion:
#	%gcc -ansi -pedantic -Wall -g -o prog prog.c
# 	valgrind --leak-check=full --track-origins=yes ./prog arg1 ...
# 	ddd ./prog
#	
#	Use --track-origins=yes to see where uninitialised values come from
#
# Compilacion final:
#	%gcc -ansi -pedantic -Wall -O3 -o prog prog.c
#
# -O3 Eficiencia, pero da unos warnings que a veces se pueden omitir
# -g Allows the compiler to collect debugging information
# -D Defines preprocessor Macros	#-DNDEBUG turns of asserts mandated by the C standards
#-------------------------------------------------------------------------------------------------------
#
##########################################################################################################

CC=gcc 
CFLAGS = -std=gnu17 -pedantic -g -Wall #-DDEBUG
#CFLAGS = -Wall -g -DNDEBUG #-O3 				

IFLAGS=-I. ##La opción -I se usa para indicar donde se encuentran los ficheros cabeceras (.h), puedes poner más de una -I
LDFLAGS=-L. ##La opción -L indica el directorio donde se encuentra las biblioteca adicionales, puedes poner más de una -L

LIBS=  -lm -pthread -lrt ### -lm enlaza la biblioteca matematica, -pthread enlaza la biblioteca de hilos

Executives =  miner monitor


MINER_ARGS1 = 30 5 
MINER_ARGS2 = 10 16 
MINER_ARGS3 = 20 10 

OBJECTS = funciones.o sysinfo.o circularbuffer.o block_wallet.o pow.o handlers.o

.Phony: run run_miner1 run_miner2 run_miner3 run_monitor clear clean rm_sem rm_queue


## EXECUTIVES CREATION ##
#----------------------------------------------------------------------------------------------------------------------------

all: $(Executives)


miner: miner.o $(OBJECTS)
	@echo ---------CREATING miner------------------
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo -----------------------------------------
	@echo

monitor: monitor.o $(OBJECTS)
	@echo ---------CREATING monitor----------------
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo -----------------------------------------
	@echo


#----------------------------------------------------------------------------------------------------------------------------


## OBJECTS CREATION ##
#----------------------------------------------------------------------------------------------------------------------------

miner.o: miner.c funciones.h sysinfo.h block_wallet.h pow.h handlers.h types.h
	$(CC) $(CFLAGS) -c $< $(IFLAGS)

monitor.o: monitor.c funciones.h circularbuffer.h block_wallet.h pow.h handlers.h types.h
	$(CC) $(CFLAGS) -c $< $(IFLAGS)



funciones.o: funciones.c funciones.h sysinfo.h circularbuffer.h block_wallet.h pow.h handlers.h types.h
	$(CC) $(CFLAGS) -c $< $(IFLAGS)

sysinfo.o: sysinfo.c sysinfo.h funciones.h circularbuffer.h block_wallet.h pow.h handlers.h types.h
	$(CC) $(CFLAGS) -c $< $(IFLAGS)

circularbuffer.o: circularbuffer.c circularbuffer.h block_wallet.h types.h
	$(CC) $(CFLAGS) -c $< $(IFLAGS)

block_wallet.o:	block_wallet.c block_wallet.h types.h
	$(CC) $(CFLAGS) -c $< $(IFLAGS)

pow.o: pow.c pow.h types.h
	$(CC) $(CFLAGS) -c $< $(IFLAGS)
	
handlers.o: handlers.c handlers.h
	$(CC) $(CFLAGS) -c $< $(IFLAGS)


#---------------------------------------------------------------------------------------------------------------------------


### RUN COMMANDS ###

## RUN ejercicio 1 ##
#------------------------------------------------------------------------------------------------------------------------------

run: run_miner1 run_miner2 run_miner3 run_monitor

run_miner1:
	./miner $(MINER_ARGS)

run_miner2:
	./miner $(MINER_ARGS2)

run_miner3:
	./miner $(MINER_ARGS1)

run_monitor:
	./monitor

run_miners:
	./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 &
	./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 &
	./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 &
	./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 &
	./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 &
	./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 & ./miner 15 5 &

	
#------------------------------------------------------------------------------------------------------------------------------

clean:
	rm -v $(Executives) *.o *.dat 

clear:
	rm -v *.o


rm_sem: 
	rm -v /dev/shm/sem* 

rm_queue: 
	rm -v /dev/mqueue/*
