# Soubor:  Makefile


CC= g++ -Wall -dynamic_cast -lpthread -I.


CPPFLAGS += -I.
CPPFLAGS += -I./include
# CPPFLAGS += -I./include/nosjob/include
# libnosjob := ./include/nosjob/src/nosjob.o \
# 						./include/nosjob/src/Array.o \
# 						./include/nosjob/src/Boolean.o \
# 						./include/nosjob/src/Double.o \
# 						./include/nosjob/src/Integer.o \
# 						./include/nosjob/src/Mutex.o \
# 						./include/nosjob/src/Object.o \
# 						./include/nosjob/src/Parser.o \
# 						./include/nosjob/src/Utf8String.o \
# 						./include/nosjob/src/Utf16String.o \
# 						./include/nosjob/src/parser/JSON_parser.o \
# 						./include/nosjob/src/whalloc_amalgamation.o
		

hotrod_cli_lib_obj := 	include/murmur/MurmurHash2.o \
						include/murmur/MurmurHash3.o \
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
						src/remoteCache2.o\


ALL:    $(hotrod_cli_lib_obj)    test1.o 
	#make -C ./include/nosjob/		
	$(CC) $(libnosjob)  $(hotrod_cli_lib_obj) test1.o -o test1


run:
	./test1

go: ALL
	./test1

starts:
	./infinispan/bin/startServer.sh -r hotrod

startservers:
	 ./infinispan/bin/startServer.sh -r hotrod -p 11222 -c ./infinispan/bin/distrib.xml &
	 sleep 6
	 ./infinispan/bin/startServer.sh -r hotrod -p 11223 -c ./infinispan/bin/distrib.xml &
	 sleep 3
	 ./infinispan/bin/startServer.sh -r hotrod -p 11224 -c ./infinispan/bin/distrib.xml &
	 sleep 3
	 ./infinispan/bin/startServer.sh -r hotrod -p 11225 -c ./infinispan/bin/distrib.xml &
	 sleep 3
	 ./infinispan/bin/startServer.sh -r hotrod -p 11226 -c ./infinispan/bin/distrib.xml &
	 sleep 3
	 ./infinispan/bin/startServer.sh -r hotrod -p 11227 -c ./infinispan/bin/distrib.xml &
	 sleep 3
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
	rm -f include/murmur/*.o
	rm -f test1

pack:
	tar cf xsimic02.tar *.cpp README Makefile *.pdf 	




