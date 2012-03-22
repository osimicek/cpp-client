#include <iostream>
class Transporter;


class PacketParser{
	public:
    Transporter *transporter;
  
    PacketParser(Transporter*);
};