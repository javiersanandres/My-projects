CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic -ansi
# LDLIBS = -lodbc

# recompile if this header changes
HEADERS = utils.h

MENU = menu
OBJ_MENU = $(MENU).o utils.o 

EXE = tester
OBJ_EXE = $(EXE).o utils.o checkUtils.o checkCreateTable.o checkReplaceExtensionByIdx.o \
checkCreateIndex.o checkPrint.o checkFindKey.o checkAddIndexEntry.o checkAddTableEntry.o


all : $(EXE)

%.o: %.c $(HEADERS)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXE): $(DEPS) $(OBJ_EXE)
	$(CC) -o $(EXE) $(OBJ_EXE) $(LDLIBS)

$(MENU): $(DEPS) $(OBJ_MENU)
	$(CC) -o $(MENU) $(OBJ_MENU) $(LDLIBS)


run: $(MENU)
	@./menu

test_val:
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./tester

menu_val:
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./menu

clean :
	rm -f *.o core $(EXE)

clear:
	rm -f -v $(MENU) $(EXE)

