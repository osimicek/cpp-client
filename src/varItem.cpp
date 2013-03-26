#include "varItem.h"



std::ostream & operator << (std::ostream& _stream, const VarItem & varItem) 
{
    varItem.print(_stream);
    return _stream;
}