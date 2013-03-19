#ifndef VARITEM_H_INCLUDED
#define VARITEM_H_INCLUDED
#include <string.h>
// #include <stdio.h>

#include "constants.h"
#include <marshaller.h>



class VarItem{
    public:

        MarshallerJBoss marshaller;
        std::string marshalled;

        VarItem(){}

        VarItem(const std::string value){
            marshaller.dump(&marshalled, &value);
        }

        VarItem(const std::string *value){
            marshaller.dump(&marshalled, value);
        }

        VarItem(const char *value){
            marshaller.dump(&marshalled, value);
        }

        VarItem(const double value){
            marshaller.dump(&marshalled, value);
        }

        VarItem(const int value){
            marshaller.dump(&marshalled, value);
        }

        // operator std::string*() { 
        //     std::string *strVal = new std::string();
        //     marshaller.load(strVal, &marshalled);

        //     return strVal;
        // }
        operator std::string() {
            std::string strVal; 
            marshaller.load(&strVal, &marshalled);
            return strVal; 
        }

        operator char *() { 
            std::string strVal; 
            marshaller.load(&strVal, &marshalled);

            char *charVal=new char[strVal.size()+1];
            charVal[strVal.size()] = 0;
            memcpy(charVal, strVal.c_str(), strVal.size());
            return charVal; 
        }

        operator int() { 
            int intVal;
            marshaller.load(&intVal, &marshalled);
            std::cout<<std::dec<< "load "<< intVal  << " " << marshalled.length() <<"  "<<this<< std::endl;
            return intVal; 
        }
        operator double() { 
            double doubleVal;
            marshaller.load(&doubleVal, &marshalled);
            return doubleVal; 
        }



        VarItem &operator=(const double value){
            marshaller.dump(&marshalled, value);
            return *this;
        }

        VarItem &operator=(const int value){
            marshaller.dump(&marshalled, value);
            std::cout<<std::dec<< "value "<< value  << " " << marshalled.length() <<"  "<<this<< std::endl;
            return *this;
        }

        VarItem &operator=(const std::string value){
            marshaller.dump(&marshalled, &value);
            return *this;
        }

        VarItem &operator=(const std::string *value){
            marshaller.dump(&marshalled, value);
            return *this;
        }

        // VarItem &operator=(std::string *value){
        //     marshaller.dump(&marshalled, value);
        //     return *this;
        // }

        VarItem &operator=(const char *value){
            marshaller.dump(&marshalled, value);
            return *this;
        }

        void set_value(std::string *value){
            marshalled = *value;
        }

        std::ostream & print(std::ostream& _stream){
            marshaller.print(_stream, &marshalled);
            return _stream;
        }
        
};

std::ostream & operator << (std::ostream& _stream, VarItem & varItem);

namespace std
{
    template<> struct less<VarItem>{
           bool operator() (const VarItem& lhs, const VarItem& rhs)
           {
               return lhs.marshalled < rhs.marshalled;
           }
    };
}

#endif