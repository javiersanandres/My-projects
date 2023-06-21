# Version 1.0
# Por lo general no se requiere modificar el fichero

export PGDATABASE:=classicmodels
export PGUSER :=alumnodb
export PGPASSWORD :=alumnodb
export PGCLIENTENCODING:=LATIN9
export PGHOST:=localhost

DBNAME =$(PGDATABASE)
PSQL = psql
CREATEDB = createdb
DROPDB = dropdb --if-exists
PG_DUMP = pg_dump
PG_RESTORE = pg_restore

all: dropdb createdb restore shell
createdb:
	@echo Creando BBDD
	@$(CREATEDB)
dropdb:
	@echo Eliminando BBDD
	@$(DROPDB) $(DBNAME)
	rm -f *.log
dump:
	@echo creando dumpfile
	@$(PG_DUMP) > $(DBNAME).sql
restore:
	@echo restore data base
	@cat $(DBNAME).sql | $(PSQL)  
	
psql: shell 
shell:
	@echo create psql shell
	@$(PSQL)  


removelogs: 
	rm -rf *.log



CC = cc 
CFLAGS = -Wall -g -ansi -pedantic 
LDLIBS = -lodbc

EXE = menu

compile : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

$(EXE) :  Menu.o MenuProducts.o MenuOrders.o MenuCustomers.o MYodbc.o odbc.o Consultas.o ProductsQueries.o OrdersQueries.o CustomersQueries.o

	@echo "#---------------------------"
	@echo "# Generating $@  "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC)  -o $@ $^ $(LDLIBS)

Menu.o : Menu.c 
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $<  


ProductsQueries.o : ProductsQueries.c ProductsQueries.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $< 

OrdersQueries.o : OrdersQueries.c OrdersQueries.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $< 

CustomersQueries.o : CustomersQueries.c CustomersQueries.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $< 

Consultas.o : Consultas.c Consultas.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $< 

MenuProducts.o : MenuProducts.c MenuProducts.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $< 

MenuOrders.o : MenuOrders.c MenuOrders.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $< 

MenuCustomers.o : MenuCustomers.c MenuCustomers.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $< 

MYodbc.o : MYodbc.c MYodbc.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $< 

odbc.o : odbc.c odbc.h
	@echo "#---------------------------"
	@echo "# Generating $@ "
	@echo "# Depepends on $^"
	@echo "# Has changed $<"
	$(CC) $(CFLAGS) -c $< 

menu_design: 
	@echo Running menu in design mode
	@./menu 1


tests: 
	@echo "Running all the tests>>>>>>>>> $(shell date) "
	@echo "\n\n\n-----------------RUNNING TESTS $(shell date)------------------\n\n\n" >> tests.log
	@make -s test_products 
	@make -s test_orders 
	@make -s test_customers
	

test_products:
	@echo "#---------------------------"
	@echo "Running products_stock"
	@echo "-------------------Running products_stock---------------------" >> tests.log
	@./products_stock.sh >> tests.log
	@echo "#---------------------------"
	@echo "Running products_find"
	@echo "-------------------Running products_find----------------------" >> tests.log
	@./products_find.sh >> tests.log
	

test_orders:
	@echo "#---------------------------"
	@echo "Running orders_open"
	@echo "--------------------Running orders_open------------------------" >> tests.log
	@./orders_open.sh >> tests.log
	@echo "#---------------------------"
	@echo "Running orders_range"
	@echo "--------------------Running orders_range-----------------------" >> tests.log
	@./orders_range.sh >> tests.log
	@echo "#---------------------------"
	@echo "Running orders_detail"
	@echo "--------------------Running orders_detail-----------------------" >> tests.log
	@./orders_detail.sh >> tests.log

test_customers:
	@echo "#---------------------------"
	@echo "Running customers_balance"
	@echo "--------------------Running customers_balance-------------------" >> tests.log
	@./customers_balance.sh >> tests.log
	@echo "#---------------------------"
	@echo "Running customers_find"
	@echo "--------------------Running customers_find----------------------" >> tests.log
	@./customers_find.sh >> tests.log
	@echo "#---------------------------"
	@echo "Running customers_list_products"
	@echo "--------------------Running customers_list_products-------------" >> tests.log
	@./customers_list_products.sh >> tests.log


