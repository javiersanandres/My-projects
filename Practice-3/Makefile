#-----------------------
# IMPORTANTE: The values of the parameters for exercise?_test have to be conveniently adjusted.
# It is assumed that the name of the source files is exercise1.c, exercise2.c,...,exercise.h
#-----------------------

CC = cc -ansi -pedantic 
#CFLAGS = -Wall
#CFLAGS = -Wall -g 
CFLAGS = -Wall  -g -DNDEBUG -O3 #-DNDEBUG turns of asserts mandated by the C standards
EXE = exercise1 exercise2

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
# -O3 Eficiencia, elimina a nivel compilador la recursion de cola, para no ocupar mas posiciones de pila 
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
# fit f(x) "exercise5.log" using 1:2 via a, b   Te ajusta la grafica a estos puntos
#########################################################################################################
all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o sorting.o search.o times.o permutations.o
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -o $@ $@.o sorting.o search.o times.o permutations.o

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

search.o : search.c search.h
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
	@./exercise1 -size 10 -key 257

exercise2_test:
	@echo Running exercise2
	@./exercise2 -num_min 1 -num_max 10000 -incr 20 -n_times 10000 -outputFile lin_auto_search_curiosity.log
