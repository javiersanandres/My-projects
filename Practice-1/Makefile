#-----------------------
# IMPORTANTE: The values of the parameters for exercise?_test have to be conveniently adjusted.
# It is assumed that the name of the source files is exercise1.c, exercise2.c,...,exercise.h
#-----------------------

CC = cc -ansi -pedantic 
#CFLAGS = -Wall
#CFLAGS = -Wall -g 
CFLAGS = -Wall  -g -DNDEBUG #-O3 #-DNDEBUG turns of asserts mandated by the C standards
EXE = exercise1 exercise1gnuplot exercise2 exercise3 exercise4 exercise5

########################################################################################################
#
# $@ es el item que aparece a la izquierda de ':'
# $< es el primer item en la lista de dependencias
# $^ son todos los archivos que se encuentran a la derecha de ':' (dependencias)
#
#-------------------------------------------------------------------------------------------------------
# Compilacion durante desarrollo:
# 	%gcc -ansi -pedantic -Wall -o prog prog.c
# 
# Compilacion para test/depuracion:
#	%gcc -ansi -pedantic -Wall -g -o prog prog.c
# 	valgrind --leak-check=full ./prog arg1 ...
# 	ddd ./prog
#
# Compilacion final:
#	%gcc -ansi -pedantic -Wall -03 -o prog prog.c
#
# -o3 Eficiencia 
# -g Allows the compiler to collect debugging information
#-------------------------------------------------------------------------------------------------------
#
# |sort -n  Sorts the elements from lowest to highest
# |uniq If 2 equal elements are together (one after another), it just keeps one
# |uniq -c same as uniq, but also counts how many were together
# |sort |uniq -c will therefore only keep one elent of each type, and they will be in order
#
# plot "num.data" using 1:2, 0.001*x*x    Te escoge la columna 1 como datos y 2 como las frecuencia, y tmb te dibuja la otra funcion 
# f(x)= a*x*x+b
# fit g(x) "exercise5.log" using 1:2 via a, b   Te ajusta la grafica a estos puntos
#########################################################################################################

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o sorting.o times.o permutations.o
	@echo "#---------------------------"
	@echo "# Generating $@  "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -o $@ $@.o sorting.o times.o permutations.o

permutations.o : permutations.c permutations.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $<

sorting.o : sorting.c sorting.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $<

 times.o : times.c times.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $<
	
exercise1_test:
	@echo Running exercise1
	@./exercise1 -limInf 1 -limSup 100 -numN 10000

exercise1gnuplot_test:
	@echo Running exercise1gnuplot
	@./exercise1gnuplot -limInf 1 -limSup 100 -numN 1000000 -outputFile exercise1gnuplot.log

exercise2_test:
	@echo Running exercise2
	@./exercise2 -size 10 -numP 5

exercise3_test:
	@echo Running exercise3
	@./exercise3 -size 5 -numP 20

exercise4_test:
	@echo Running exercise4
	@./exercise4 -size 20

exercise5_test:
	@echo Running exercise5
	@./exercise5 -num_min 1000 -num_max 20000 -incr 200 -numP 1 -outputFile exercise5.log

