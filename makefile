# Soubor:  Makefile
# Autor: Ondrej Simicek

CC= g++ -Wall -dynamic_cast  -pthread -I.

CPPFLAGS += -std=c++0x -g 
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
						src/codec.o\
						src/codec10.o\
						src/codec11.o\
						src/codec12.o\
						src/transport.o\
						src/consistentHash.o\
						src/consistentHash10.o\
						src/consistentHash11.o\
						src/consistentHash12.o\
						src/transportFactory.o\
						src/operations.o\
						src/marshaller.o\
						src/remoteCache.o\

.PHONY: tests clean doc

ALL: $(hotrod_cli_lib_obj)  main.o
	$(CC) $(hotrod_cli_lib_obj) main.o -o main 

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

not_thread_safe_example:  examples/not_thread_safe_example.o $(hotrod_cli_lib_obj)
	$(CC) examples/not_thread_safe_example.o $(hotrod_cli_lib_obj) -o examples/not_thread_safe_example
	./examples/not_thread_safe_example

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

doc:
	doxygen ./doc/conf.doxygen 







startserver:
	./server/infinispan/bin/startServer.sh -r hotrod

startservers:
	 ./server/infinispan/bin/startServer.sh -r hotrod -p 11222 -c ./server/distrib.xml &
	 sleep 3
	 ./server/infinispan/bin/startServer.sh -r hotrod -p 11223 -c ./server/distrib.xml &
	 sleep 1
	 ./server/infinispan/bin/startServer.sh -r hotrod -p 11224 -c ./server/distrib.xml &
	 sleep 1
	 ./server/infinispan/bin/startServer.sh -r hotrod -p 11225 -c ./server/distrib.xml &
	 sleep 1
	 ./server/infinispan/bin/startServer.sh -r hotrod -p 11226 -c ./server/distrib.xml &
	 sleep 1
	 ./server/infinispan/bin/startServer.sh -r hotrod -p 11227 -c ./server/distrib.xml &
	 sleep 1
	 ./server/infinispan/bin/startServer.sh -r hotrod -p 11228 -c ./server/distrib.xml &

kill:
	killall -9 -v java


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
	rm -f examples/not_thread_safe_example
	rm -f -r doc/html
	rm -f -r doc/latex

