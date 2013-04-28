#include "marshaller.h"
Marshaller* DEFAULT_MARSHALLER;

Marshaller::Marshaller(){}
int Marshaller::dump(const int src, std::string* dest){
    std::stringstream ss;
    ss << src;
    *dest = ss.str();
    return NO_ERROR_STATUS;
};
int Marshaller::dump(const double src, std::string* dest){
    std::stringstream ss;
    ss << src;
    *dest = ss.str();
    return NO_ERROR_STATUS;
};
int Marshaller::dump(const char *src, std::string* dest){
    *dest = src;
    return NO_ERROR_STATUS;
};
int Marshaller::dump(const std::string *src, std::string* dest){
    *dest = *src;
    return NO_ERROR_STATUS;
};
int Marshaller::dump(const VarItem *src, std::string* dest){
    *dest = src->marshalled;
    return NO_ERROR_STATUS;
};

int Marshaller::load(const std::string* src, int* dest){};
int Marshaller::load(const std::string* src, double* dest){};
int Marshaller::load(const std::string* src, char* dest){ // otestovat
    dest =(char *)src->c_str();
    return NO_ERROR_STATUS;
};
int Marshaller::load(const std::string* src, std::string* dest){
    *dest = *src;
    return NO_ERROR_STATUS;
};
int Marshaller::load(const std::string* src, VarItem* dest){
    dest->marshalled = *src;
    return NO_ERROR_STATUS;
};

int Marshaller::get_type(const std::string* src){};
std::ostream & Marshaller::print(std::ostream& _stream, const std::string* src){
    return _stream;
}


MarshallerJBoss::MarshallerJBoss(){}
int MarshallerJBoss::dump(const int src, std::string* dest){
    dest->clear();
    short type = INT_TYPE;
    *dest += ((char *) &type)[1];
    *dest += ((char *) &type)[0];
    for(int i=3; i>=0; i--){
        *dest += ((char *)&src)[i];
    }
    return NO_ERROR_STATUS;
};
int MarshallerJBoss::dump(const double src, std::string* dest){
    dest->clear();
    short type = DOUBLE_TYPE;
    *dest += ((char *) &type)[1];
    *dest += ((char *) &type)[0];
    for(int i=7; i>=0; i--){
        *dest += ((char *)&src)[i];
    }
    return NO_ERROR_STATUS;
};
int MarshallerJBoss::dump(const char *src, std::string* dest){
    std::string src2(src);
    return MarshallerJBoss::dump(&src2, dest);
};

int MarshallerJBoss::dump(const std::string *src, std::string* dest){
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
        return ERROR_TO_BIG_STRING_STATUS;
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

int MarshallerJBoss::dump(const VarItem *src, std::string* dest){
    *dest = src->marshalled;
    return NO_ERROR_STATUS;
}

int MarshallerJBoss::load(const std::string* src, int* dest){
    *dest = 0;
    int result = 0;
    short type = 0x00ff & (short)(*src)[0];
    type = type << 8;
    type |= 0x00ff & (short)(*src)[1];

    if(type != INT_TYPE or src->length() != 6){
        return ERROR_BAD_TYPE_STATUS;
    }

    for(int i=2; i<6;i++){
        result = result << 8;
        result =    (result | (0x00ff & ((short)((*src)[i]))));
    }

    *dest = result;
    return NO_ERROR_STATUS;
};
int MarshallerJBoss::load(const std::string* src, double* dest){
    *dest = 0;
    long long result = 0;

    short type = 0x00ff & (short)(*src)[0];
    type = type << 8;
    type |= 0x00ff & (short)(*src)[1];

    if(type != DOUBLE_TYPE or src->length() != 10){
        return ERROR_BAD_TYPE_STATUS;
    }

    for(int i=2; i<10;i++){
        result = result << 8;
        result =    (result | (0x00ff & ((short)((*src)[i]))));
    }

    *dest = *((double *)&result);
    return NO_ERROR_STATUS;
};
int MarshallerJBoss::load(const std::string* src, char* dest){};
int MarshallerJBoss::load(const std::string* src, std::string* dest){
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
        return ERROR_BAD_TYPE_STATUS;
    }
    for(int i=0; i<len_of_len;i++){
        len = len << 8;
        len |= 0x00ff & (short)(*src)[possition++];
    }

    dest->append(*src,possition,len);
    return NO_ERROR_STATUS;
};

int MarshallerJBoss::load(const std::string* src, VarItem* dest){
    dest->marshalled = *src;
    return NO_ERROR_STATUS;
}

int MarshallerJBoss::get_type(const std::string* src){
    if(src->length() < 2){
        return 0;
    }

    int possition = 0;
    short type = 0x00ff & (short)(*src)[possition++];
    type = type << 8;
    type |= 0x00ff & (short)(*src)[possition++];

    if(type == INT_TYPE){
        return INT_TYPE;
    }

    if(type == DOUBLE_TYPE){
        return DOUBLE_TYPE;
    }

    if(type == STRING_TYPE_LEN_0 || type == STRING_TYPE_LEN_1 || type == STRING_TYPE_LEN_2){
        return STRING_TYPE;
    }

    return 0;

};

std::ostream & MarshallerJBoss::print(std::ostream& _stream, const std::string* src){
    int type = get_type(src);
    if(type == INT_TYPE){
        int intVal;
        load(src, &intVal);
        _stream << intVal;
    }

    if(type == DOUBLE_TYPE){
        double doubleVal;
        load(src, &doubleVal);
        _stream << doubleVal;
    }

    if(type == STRING_TYPE){
        std::string stringVal;
        load(src, &stringVal);
        _stream << stringVal;
    }
    return _stream;
}
