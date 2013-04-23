# Soubor:  Makefile


CC= g++ -Wall -dynamic_cast  -lpthread -I.
hotrod_lib = libHotRod.a
main = main

CPPFLAGS += -g #-Wall -W -ansi
CPPFLAGS += -I./include
objects = include/UnitTest++/AssertException.o \
	include/UnitTest++/Test.o \
	include/UnitTest++/Checks.o \
	include/UnitTest++/TestRunner.o \
	include/UnitTest++/TestResults.o \
	include/UnitTest++/TestReporter.o \
	include/UnitTest++/TestReporterStdout.o \
	include/UnitTest++/ReportAssert.o \
	include/UnitTest++/TestList.o \
	include/UnitTest++/TimeConstraint.o \
	include/UnitTest++/TestDetails.o \
	include/UnitTest++/MemoryOutStream.o \
	include/UnitTest++/DeferredTestReporter.o \
	include/UnitTest++/DeferredTestResult.o \
	include/UnitTest++/XmlTestReporter.o \
	include/UnitTest++/CurrentTest.o \
	include/UnitTest++/Posix/SignalTranslator.o \
	include/UnitTest++/Posix/TimeHelpers.o


test_objects = tests/main.o \
	tests/put_get_test.o \
	tests/replace_test.o \
	tests/put_if_absent_test.o \
	tests/remove_test.o \
	tests/rmmap_test.o \
	tests/ping_stats_contains_key_test.o\
	tests/put_all_test.o\
	tests/get_with_meta_test.o\
	tests/get_bulk_key_set_test.o\

	

hotrod_cli_lib_obj := 	include/murmur/MurmurHash2.o \
						include/murmur/MurmurHash3.o \
						src/varItem.o\
						src/RMMap.o\
						src/codec10.o\
						src/codec11.o\
						src/codec12.o\
						src/transport.o\
						src/consistentHash10.o\
						src/consistentHash11.o\
						src/consistentHash12.o\
						src/transportFactory.o\
						src/operations.o\
						src/marshaller.o\
						src/remoteCache.o\

# $(hotrod_lib): $(hotrod_cli_lib_obj) 
# 	@echo Creating $(hotrod_lib) library...
# 	@ar cr $(hotrod_lib) $(hotrod_cli_lib_obj)

# $(main):  $(hotrod_lib) main.o
# 	@echo Linking $(main)...
# 	$(CC) $(hotrod_lib) main.o -o $(main) 
# 	@echo Running unit tests...
# 	@./$(main)

ALL: $(hotrod_cli_lib_obj)  $(main).o
	$(CC) $(hotrod_cli_lib_obj) $(main).o -o $(main) 

run:
	./main

go: ALL
	./main

basic_example: $(hotrod_cli_lib_obj) examples/basic_example.o
	$(CC) $(hotrod_cli_lib_obj) examples/basic_example.o -o examples/basic_example
	./examples/basic_example

bulk_example: $(hotrod_cli_lib_obj) examples/bulk_example.o
	$(CC) $(hotrod_cli_lib_obj) examples/bulk_example.o -o examples/bulk_example
	./examples/bulk_example

rmmap_example: $(hotrod_cli_lib_obj) examples/rmmap_example.o
	$(CC) $(hotrod_cli_lib_obj) examples/rmmap_example.o -o examples/rmmap_example
	./examples/rmmap_example

advanced_example: $(hotrod_cli_lib_obj) examples/advanced_example.o
	$(CC) $(hotrod_cli_lib_obj) examples/advanced_example.o -o examples/advanced_example
	./examples/advanced_example


rmmap_advanced_example: $(hotrod_cli_lib_obj) examples/rmmap_advanced_example.o
	$(CC) $(hotrod_cli_lib_obj) examples/rmmap_advanced_example.o -o examples/rmmap_advanced_example
	./examples/rmmap_advanced_example
# test:    $(hotrod_cli_lib_obj) test/test2.o test/test1.o
# 	#make -C ./include/nosjob/		
# 	$(CC) $(libnosjob)  $(hotrod_cli_lib_obj) test/test1.o test/test1.o -o test/test1

tests: $(objects) $(hotrod_cli_lib_obj) $(test_objects)
	$(CC) $(objects) $(hotrod_cli_lib_obj) $(test_objects) -o tests/hotrod_test
	@echo "*******************"
	@echo "** Running tests **"
	@echo "*******************"
	./tests/hotrod_test

runtests:
	@echo "*******************"
	@echo "** Running tests **"
	@echo "*******************"
	./test/hotrod_test










startserver:
	./infinispan/bin/startServer.sh -r hotrod

startservers:
	 ./infinispan/bin/startServer.sh -r hotrod -p 11222 -c ./infinispan/bin/distrib.xml &
	 sleep 3
	 ./infinispan/bin/startServer.sh -r hotrod -p 11223 -c ./infinispan/bin/distrib.xml &
	 sleep 1
	 ./infinispan/bin/startServer.sh -r hotrod -p 11224 -c ./infinispan/bin/distrib.xml &
	 sleep 1
	 ./infinispan/bin/startServer.sh -r hotrod -p 11225 -c ./infinispan/bin/distrib.xml &
	 sleep 1
	 ./infinispan/bin/startServer.sh -r hotrod -p 11226 -c ./infinispan/bin/distrib.xml &
	 sleep 1
	 ./infinispan/bin/startServer.sh -r hotrod -p 11227 -c ./infinispan/bin/distrib.xml &
	 sleep 1
	 ./infinispan/bin/startServer.sh -r hotrod -p 11228 -c ./infinispan/bin/distrib.xml &

startservers10:
	 ./infinispan-4.1.0.FINAL/bin/startServer.sh -r hotrod -p 11222 -c ./infinispan-4.1.0.FINAL/bin/distrib.xml &
	 sleep 6
	 ./infinispan-4.1.0.FINAL/bin/startServer.sh -r hotrod -p 11223 -c ./infinispan-4.1.0.FINAL/bin/distrib.xml &
	 sleep 3
	 ./infinispan-4.1.0.FINAL/bin/startServer.sh -r hotrod -p 11224 -c ./infinispan-4.1.0.FINAL/bin/distrib.xml &
	 sleep 3
	 ./infinispan-4.1.0.FINAL/bin/startServer.sh -r hotrod -p 11225 -c ./infinispan-4.1.0.FINAL/bin/distrib.xml &
	 sleep 3
	 ./infinispan-4.1.0.FINAL/bin/startServer.sh -r hotrod -p 11226 -c ./infinispan-4.1.0.FINAL/bin/distrib.xml &
	 sleep 3
	 ./infinispan-4.1.0.FINAL/bin/startServer.sh -r hotrod -p 11227 -c ./infinispan-4.1.0.FINAL/bin/distrib.xml &
	 sleep 3
	 ./infinispan-4.1.0.FINAL/bin/startServer.sh -r hotrod -p 11228 -c ./infinispan-4.1.0.FINAL/bin/distrib.xml &

kill:
	killall -9 -v java


java:
	cd ./java\ client/client/hotrod-client; ./compile.sh
runjava:
	cd ./java\ client/client/hotrod-client; ./run.sh #editovat cpp-client/java client/client/hotrod-client/src/main/java/org/infinispan/client/hotrod-client

java10:
	cd ./java\ client/client/hotrod-client; ./compile.sh	


clean:							# smazani nezadoucich souboru
	rm -f *~
	rm -f *.o
	rm -f src/*.o
	rm -f test/*.o
	rm -f examples/*.o
	rm -f include/murmur/*.o
	rm -f $(objects) $(test_objects)
	rm -f $(hotrod_lib)
	rm -f main
	rm -f tests/hotrod_test
	rm -f examples/basic_example
	rm -f examples/bulk_example
	rm -f examples/rmmap_example
	rm -f examples/advanced_example
	rm -f examples/rmmap_advanced_example

pack:
	tar cf xsimic02.tar *.cpp README Makefile *.pdf 	




