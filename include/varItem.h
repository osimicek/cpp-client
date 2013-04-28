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
            marshaller->dump(&value, &marshalled);
        }

        VarItem(const std::string *value){
            init();
            marshaller->dump(value, &marshalled);
        }

        VarItem(const char *value){
            init(); 
            marshaller->dump(value, &marshalled);
        }

        VarItem(const double value){
            init();
            marshaller->dump(value, &marshalled);
        }

        VarItem(const int value){
            init();
            marshaller->dump(value, &marshalled);
        }

        void init(){
            
            if(DEFAULT_MARSHALLER != NULL){
                // std::cout<<std::dec<< "WORKED"<<DEFAULT_MARSHALLER<< std::flush;
                marshaller = DEFAULT_MARSHALLER;
            }else{
  
                marshaller = new MarshallerJBoss();
            }
        }

        int get_type(){
            return marshaller->get_type(&marshalled);
        }

        void clear(){
            marshalled  = "";
        }

        // operator std::string*() { 
        //     std::string *strVal = new std::string();
        //     marshaller->load(strVal, &marshalled);

        //     return strVal;
        // }
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

        virtual void set_value(std::string *value){
            marshalled = *value;
        }

        virtual std::ostream & print(std::ostream& _stream) const{
            marshaller->print(_stream, &marshalled);
            return _stream;
        }

        bool operator==(const VarItem &other) const { 
            return marshalled == other.marshalled;
         }

         bool operator!=(const VarItem &other) const { 
            return marshalled != other.marshalled;
         }

         bool operator==(const std::string  &other) const { 
            std::string strVal; 
            marshaller->load(&marshalled, &strVal);
            return  strVal == other;
         }

         bool operator!=(const std::string  &other) const { 
            std::string strVal; 
            marshaller->load(&marshalled, &strVal);
            return  strVal != other;
         }

         bool operator==(const char*  other) const { 
            return  *this == std::string(other);
         }

         bool operator!=(const char*  other) const { 
            return  *this != std::string(other);
         }

         bool operator==(const int &other) const { 
            int intVal;
            marshaller->load(&marshalled, &intVal);
            return intVal == other;
         }

         bool operator!=(const int &other) const { 
            int intVal;
            marshaller->load(&marshalled, &intVal);
            return intVal == other;
         }

         bool operator==(const double &other) const { 
            double doubleVal;
            marshaller->load(&marshalled, &doubleVal);
            return doubleVal == other;
         }

         bool operator!=(const double &other) const { 
            double doubleVal;
            marshaller->load(&marshalled, &doubleVal);
            return doubleVal == other;
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