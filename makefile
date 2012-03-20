# Soubor:  Makefile


CC= g++ -Wall 


ALL:  transporter.o packetParser.o packetAssembler.o remoteCache.o test1.o				
	$(CC)  transporter.o packetParser.o  packetAssembler.o remoteCache.o test1.o -o test1


run:
	./test1

starts:
	./infinispan/bin/startServer.sh -r hotrod

clean:							# smazani nezadoucich souboru
	rm -f *~
	rm -f *.o
	rm -f test1

pack:
	tar cf xsimic02.tar *.cpp README Makefile *.pdf 	


