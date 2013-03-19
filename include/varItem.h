#ifndef VARITEM_H_INCLUDED
#define VARITEM_H_INCLUDED
#include <string.h>
// #include <stdio.h>

#include "constants.h"
#include <marshaller.h>




class VarItem{
    public:

        Marshaller* marshaller;
        std::string marshalled;


        VarItem(){init();}

        VarItem(const std::string value){
            init();
            marshaller->dump(&marshalled, &value);
        }

        VarItem(const std::string *value){
            init();
            marshaller->dump(&marshalled, value);
        }

        VarItem(const char *value){
            init();
            marshaller->dump(&marshalled, value);
        }

        VarItem(const double value){
            init();
            marshaller->dump(&marshalled, value);
        }

        VarItem(const int value){
            init();
            marshaller->dump(&marshalled, value);
        }

        void init(){
            
            if(DEFAULT_MARSHALLER != NULL){
                std::cout<<std::dec<< "WORKED"<<DEFAULT_MARSHALLER<< std::flush;
                marshaller = DEFAULT_MARSHALLER;
            }else{
  
                marshaller = new MarshallerJBoss();
            }
        }

        // operator std::string*() { 
        //     std::string *strVal = new std::string();
        //     marshaller->load(strVal, &marshalled);

        //     return strVal;
        // }
        virtual operator std::string() {
            std::string strVal; 
            marshaller->load(&strVal, &marshalled);
            return strVal; 
        }

        virtual operator char *() { 
            std::string strVal; 
            marshaller->load(&strVal, &marshalled);

            char *charVal=new char[strVal.size()+1];
            charVal[strVal.size()] = 0;
            memcpy(charVal, strVal.c_str(), strVal.size());
            return charVal; 
        }

        virtual operator int() { 
            int intVal;
            marshaller->load(&intVal, &marshalled);
            std::cout<<std::dec<< "load "<< intVal  << " " << marshalled.length() <<"  "<<this<< std::endl;
            return intVal; 
        }
        virtual operator double() { 
            double doubleVal;
            marshaller->load(&doubleVal, &marshalled);
            return doubleVal; 
        }



        virtual VarItem &operator=(const double value){
            marshaller->dump(&marshalled, value);
            return *this;
        }

        virtual VarItem &operator=(const int value){
            marshaller->dump(&marshalled, value);
            std::cout<<std::dec<< "value "<< value  << " " << marshalled.length() <<"  "<<this<< std::endl;
            return *this;
        }

        virtual VarItem &operator=(const std::string value){
            marshaller->dump(&marshalled, &value);
            return *this;
        }

        virtual VarItem &operator=(const std::string *value){
            marshaller->dump(&marshalled, value);
            return *this;
        }

        // VarItem &operator=(std::string *value){
        //     marshaller->dump(&marshalled, value);
        //     return *this;
        // }

        virtual VarItem &operator=(const char *value){
            marshaller->dump(&marshalled, value);
            return *this;
        }

        virtual void set_value(std::string *value){
            marshalled = *value;
        }

        virtual std::ostream & print(std::ostream& _stream){
            marshaller->print(_stream, &marshalled);
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

// class VarItemJBoss: public VarItem{
// public:
//     MarshallerJBoss marshaller;
//     VarItemJBoss(){}

//     VarItemJBoss(const std::string value){
//         marshaller->dump(&marshalled, &value);
//     }

//     VarItemJBoss(const std::string *value){
//         marshaller->dump(&marshalled, value);
//     }

//     VarItemJBoss(const char *value){
//         marshaller->dump(&marshalled, value);
//     }

//     VarItemJBoss(const double value){
//         marshaller->dump(&marshalled, value);
//     }

//     VarItemJBoss(const int value){
//         marshaller->dump(&marshalled, value);
//     }
// };


#endif