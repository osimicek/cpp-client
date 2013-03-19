#include "varItem.h"



std::ostream & operator << (std::ostream& _stream, VarItem & varItem)
{
    varItem.print(_stream);
    return _stream;
}