#ifndef VARITEM_H_INCLUDED
#define VARITEM_H_INCLUDED
#include <string.h>
// #include <stdio.h>

#include "constants.h"
#include <marshaller.h>




class VarItem{
        Marshaller* marshaller;
        bool alocated_marshaller;
    public:
        std::string marshalled;
        VarItem();
        VarItem(const std::string value);
        VarItem(const std::string *value);
        VarItem(const char *value);
        VarItem(const double value);
        VarItem(const int value);
        VarItem(const VarItem *item);
        VarItem(const VarItem& origin);
         ~VarItem();

        void init();
        int get_type();
        void clear();
        void set_value(std::string *value);
        std::ostream & print(std::ostream& _stream) const;

        bool operator==(const VarItem &other) const;
        bool operator!=(const VarItem &other) const;
        bool operator==(const std::string  &other) const;
        bool operator!=(const std::string  &other) const;
        bool operator==(const char*  other) const;
        bool operator!=(const char*  other) const;
        bool operator==(const int &other) const;
        bool operator!=(const int &other) const;
        bool operator==(const double &other) const;
        bool operator!=(const double &other) const;

        virtual operator std::string() {
            std::string strVal; 
            marshaller->load(&marshalled, &strVal);
            return strVal; 
        }

        virtual operator char *() { 
            std::string strVal; 
            marshaller->load(&marshalled, &strVal);

            char *charVal=new char[strVal.size()+1];
            charVal[strVal.size()] = 0;
            memcpy(charVal, strVal.c_str(), strVal.size());
            return charVal; 
        }

        virtual operator int() { 
            int intVal;
            marshaller->load(&marshalled, &intVal);
            return intVal; 
        }
        virtual operator double() { 
            double doubleVal;
            marshaller->load(&marshalled, &doubleVal);
            return doubleVal; 
        }



        virtual VarItem &operator=(const double value){
            marshaller->dump(value, &marshalled);
            return *this;
        }

        virtual VarItem &operator=(const int value){
            marshaller->dump(value, &marshalled);
            return *this;
        }

        virtual VarItem &operator=(const std::string value){
            marshaller->dump(&value, &marshalled);
            return *this;
        }

        virtual VarItem &operator=(const std::string *value){
            marshaller->dump(value, &marshalled);
            return *this;
        }


        virtual VarItem &operator=(const char *value){
            marshaller->dump(value, &marshalled);
            return *this;
        }
        
};

std::ostream & operator << (std::ostream& _stream, const VarItem & varItem) ;

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