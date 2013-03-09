#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED
   const short DEBUG = 0;

   const int MAX_EXECUTE_REPEAT = 3;

   const char REQUEST_MAGIC = 0xA0;
   const char RESPONSE_MAGIC = 0xA1;

   const char VERSION_10 = 10;
   const char VERSION_11 = 11;
   const char VERSION_12 = 12;

   //requests
   const char PUT_REQUEST = 0x01;
   const char GET_REQUEST = 0x03;
   const char PUT_IF_ABSENT_REQUEST = 0x05;
   const char REPLACE_REQUEST = 0x07;
   const char REPLACE_IF_UNMODIFIED_REQUEST = 0x09;
   const char REMOVE_REQUEST = 0x0B;
   const char REMOVE_IF_UNMODIFIED_REQUEST = 0x0D;
   const char CONTAINS_KEY_REQUEST = 0x0F;
   const char GET_WITH_VERSION = 0x11;
   const char CLEAR_REQUEST = 0x13;
   const char STATS_REQUEST = 0x15;
   const char PING_REQUEST = 0x17;
   const char BULK_GET_REQUEST = 0x19;
   const char GET_WITH_METADATA = 0x1B;
   const char BULK_GET_KEYS_REQUEST = 0x1D;


   //responses
   const char PUT_RESPONSE = 0x02;
   const char GET_RESPONSE = 0x04;
   const char PUT_IF_ABSENT_RESPONSE = 0x06;
   const char REPLACE_RESPONSE = 0x08;
   const char REPLACE_IF_UNMODIFIED_RESPONSE = 0x0A;
   const char REMOVE_RESPONSE = 0x0C;
   const char REMOVE_IF_UNMODIFIED_RESPONSE = 0x0E;
   const char CONTAINS_KEY_RESPONSE = 0x10;
   const char GET_WITH_VERSION_RESPONSE = 0x12;
   const char CLEAR_RESPONSE = 0x14;
   const char STATS_RESPONSE = 0x16;
   const char PING_RESPONSE = 0x18;
   const char BULK_GET_RESPONSE = 0x1A;
   const char GET_WITH_METADATA_RESPONSE = 0x1C;
   const char BULK_GET_KEYS_RESPONSE = 0x1E;
   const char ERROR_RESPONSE = 0x50;

   //response status
   const int NO_ERROR_STATUS = 0x00;
   const int INVALID_MAGIC_OR_MESSAGE_ID_STATUS = 0x81;
   const int REQUEST_PARSING_ERROR_STATUS = 0x84;
   const char NOT_PUT_REMOVED_REPLACED_STATUS = 0x01;
   const int UNKNOWN_COMMAND_STATUS = 0x82;
   const int SERVER_ERROR_STATUS = 0x85;
   const int KEY_DOES_NOT_EXIST_STATUS = 0x02;
   const int UNKNOWN_VERSION_STATUS = 0x83;
   const int COMMAND_TIMEOUT_STATUS = 0x86;


   const char CLIENT_INTELLIGENCE_BASIC = 0x01;
   const char CLIENT_INTELLIGENCE_TOPOLOGY_AWARE = 0x02;
   const char CLIENT_INTELLIGENCE_HASH_DISTRIBUTION_AWARE = 0x03;

   const char INFINITE_LIFESPAN = 0x01;
   const char INFINITE_MAXIDLE = 0x02;

   // server side errors
   const int INVALID_SERVER_MAGIC = 0x50; //server send bad MAGIC

   // client side errors
   const int FAILED_TO_SEND = 0x60; // failed when sending data to server
   const int FAILED_TO_CHOOSE_TRANSPORT = 0x61; // could not choose transport
#endif