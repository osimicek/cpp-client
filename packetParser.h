#include <iostream>
class Transporter;
//void Transporter::close(void);

class PacketParser{
	public:
    Transporter *transporter;
  
    PacketParser(Transporter*);
};