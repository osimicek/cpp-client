# Soubor:  Makefile


CC= g++ -Wall 


ALL:  packetParser.o transporter.o packetAssembler.o remoteCache.o RMItem.o RMMap.o test1.o 		
	$(CC)  packetParser.o transporter.o packetAssembler.o remoteCache.o RMItem.o RMMap.o test1.o -o test1


run:
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

kill:
	killall -v java


clean:							# smazani nezadoucich souboru
	rm -f *~
	rm -f *.o
	rm -f test1

pack:
	tar cf xsimic02.tar *.cpp README Makefile *.pdf 	


