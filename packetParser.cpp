#include "packetParser.h"

PacketParser::PacketParser(Transporter *t):transporter(t)
{
        //std::cout << "PacketParser zije " <<"\n"<<transporter->host<<":"<<transporter->port<<std::endl;
        //std::cout <<t->port<<std::endl;
        //transporter->close();
   }