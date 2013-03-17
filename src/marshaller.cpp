#include "marshaller.h"

Marshaller::Marshaller(){}
int Marshaller::dump(std::string* dest, const int src){};
int Marshaller::dump(std::string* dest, const double src){};
int Marshaller::dump(std::string* dest, const char *src){};
int Marshaller::dump(std::string* dest, const std::string *src){};

int Marshaller::load(int* dest, std::string* src){};
int Marshaller::load(double* dest, std::string* src){};
int Marshaller::load(char* dest, std::string* src){};
int Marshaller::load(std::string* dest, std::string* src){};


MarshallerJBoss::MarshallerJBoss(){}
int MarshallerJBoss::dump(std::string* dest, const int src){
    dest->clear();
    short type = INT_TYPE;
    *dest += ((char *) &type)[1];
    *dest += ((char *) &type)[0];
    for(int i=3; i>=0; i--){
        *dest += ((char *)&src)[i];
    }
    return NO_ERROR_STATUS;
};
int MarshallerJBoss::dump(std::string* dest, const double src){
    dest->clear();
    short type = DOUBLE_TYPE;
    *dest += ((char *) &type)[1];
    *dest += ((char *) &type)[0];
    for(int i=7; i>=0; i--){
        *dest += ((char *)&src)[i];
    }
    return NO_ERROR_STATUS;
};
int MarshallerJBoss::dump(std::string* dest, const char *src){
    std::string src2(src);
    return MarshallerJBoss::dump(dest, &src2);
};

int MarshallerJBoss::dump(std::string* dest, const std::string *src){
    dest->clear();
    short type;
    int len = src->length();
    if(len == 0){
        type = STRING_TYPE_LEN_0;
    }else if(len > 0 and len < 256){
        type = STRING_TYPE_LEN_1;
    }else if(len >= 256 and len < 65536){
        type = STRING_TYPE_LEN_2;
    }else{
        return ERROR_TO_BIG_STRING;
    }

    dest->clear();
    *dest += ((char *) &type)[1];
    *dest += ((char *) &type)[0];

    if(type == STRING_TYPE_LEN_1){
        (*dest) += ((char *)(&(len)))[0];
    }else if(type == STRING_TYPE_LEN_2){
        (*dest) += ((char *)(&(len)))[1];
        (*dest) += ((char *)(&(len)))[0];
    }   

    dest->append(*src);
    return NO_ERROR_STATUS;
   
};

int MarshallerJBoss::load(int* dest, std::string* src){
    *dest = 0;
    int result = 0;

    short type = 0x00ff & (short)(*src)[0];
    type = type << 8;
    type |= 0x00ff & (short)(*src)[1];

    if(type != INT_TYPE or src->length() != 6){
        return ERROR_BAD_TYPE;
    }

    for(int i=2; i<6;i++){
        result = result << 8;
        result =    (result | (0x00ff & ((short)((*src)[i]))));
    }

    *dest = result;
    return NO_ERROR_STATUS;
};
int MarshallerJBoss::load(double* dest, std::string* src){
    *dest = 0;
    long long result = 0;

    short type = 0x00ff & (short)(*src)[0];
    type = type << 8;
    type |= 0x00ff & (short)(*src)[1];

    if(type != DOUBLE_TYPE or src->length() != 10){
        return ERROR_BAD_TYPE;
    }

    for(int i=2; i<10;i++){
        result = result << 8;
        result =    (result | (0x00ff & ((short)((*src)[i]))));
    }

    *dest = *((double *)&result);
    return NO_ERROR_STATUS;
};
int MarshallerJBoss::load(char* dest, std::string* src){};
int MarshallerJBoss::load(std::string* dest, std::string* src){
    for(int i=0; i<10;i++){
        std::cout<<std::hex<< (0x00ff & ((short)((char *) src->c_str())[i])) << " ";
    }
    dest->clear();
    int result = 0;
    int possition = 0;
    short type = 0x00ff & (short)(*src)[possition++];
    type = type << 8;
    type |= 0x00ff & (short)(*src)[possition++];

    int len_of_len = 0;
    int len = 0;

    if(type == STRING_TYPE_LEN_0){
        len_of_len = 0;
    }else if(type == STRING_TYPE_LEN_1){
        len_of_len = 1;
    }else if(type == STRING_TYPE_LEN_2){
        len_of_len = 2;
    }else{
        return ERROR_BAD_TYPE;
    }
    for(int i=0; i<len_of_len;i++){
        len = len << 8;
        len |= 0x00ff & (short)(*src)[possition++];
    }

    dest->append(*src,possition,len);
    return NO_ERROR_STATUS;
};