#include <string>
#include <remoteCache.h>
#include <marshaller.h>
#include <RMMap.h>
#include <murmur/MurmurHash3.h>
#include <murmur/MurmurHash2.h>



//#include "./include/protobuf/examples/addressbook.pb.h"



void print_map(std::map<std::string,std::string>  *data){
	std::map<std::string,std::string>::iterator pos;
	std::cout << "Bulk:" <<std::endl;
    for (pos = (*data).begin(); pos != (*data).end(); ++pos) {
        std::cout << "key: \"" << pos->first << "\" "<< "value: " << pos->second << std::endl;
    }
}

// void print_rmmap(RMMap  *data){
// 	RMMap::iterator pos;
// 	std::cout << "RMMap:" <<std::endl;
//     for (pos = (*data).begin(); pos != (*data).end(); ++pos) {
//         std::cout << "key: \"" << pos->first << "\" "<< "value: " << pos->second << std::endl;
//     }
// }

int test1(){
	RemoteCache x = RemoteCache();
    x.clear();
	std::string resp;

	int ret = 0;
	
    std::string s1 = std::string("Hell");
    std::string s2 = std::string("aa");
	x.putIfAbsent(&s1,&s2);
	//std::cerr << x.get(std::string("Hell")) <<"|"<<std::endl;
	x.get(&resp, &s1);

	if( resp != s2){
		std::cerr << "Error #1 in test1" <<resp<<std::endl;
		ret = 1;
	}
	resp.clear();

    std::string s3 = std::string("bb");
	x.put(&s1,&s3);
	x.get(&resp, &s1);
	if(resp != s3){
		std::cerr << "Error #2 in test1" <<std::endl;
		ret = 1;
	}
	 resp.clear();

    std::string s4 = std::string("cc");
	x.replace(&s1,&s4);
	x.get(&resp, &s1);
	if(resp!= s4){
		std::cerr << "Error #3 in test1" <<std::endl;
		ret = 1;
	}
	resp.clear();
    x.clear();
    x.close();
	return ret;

}


int test2(){
	RemoteCache x = RemoteCache();
    x.clear();
	std::string resp;
	int ret = 0;
	int stat;
	long long ver = 0;
    x.put(&std::string("Hell"), &std::string("cc"));
	x.getWithVersion(&resp, &std::string("Hell"),&ver);
	if(resp != std::string("cc")){
		std::cerr << "Error #1 in test2 "<< resp<<std::endl;
		ret = 1;
	}
  std::cerr << "ver "<<ver<<std::endl;
	resp.clear();
	if((stat = x.replaceWithVersion("Hell","dd",ver++))){
		std::cerr << "Error #2 in test2   stat:"<<stat <<std::endl;
		ret = 1;
	}
	x.get(&resp, &std::string("Hell"));
	if(resp != std::string("dd")){
		std::cerr << "Error #3 in test2" <<std::endl;
		ret = 1;
	}
	resp.clear();


	for(int i=0;i<20;i++){
		x.put(&std::string("Hell"),&std::string("ee"));
	}
	if((stat = x.replaceWithVersion(&std::string("Hell"),&std::string("dd"),ver+20))){
		std::cerr << "Error #4 in test2   stat:"<<stat <<"  !for multiservers OK, klice se neinkrementuji"<<std::endl;
		ret = 1;
	}

	if(stat = x.removeWithVersion(&std::string("Hell"),ver+21)){
		std::cerr << "Error #5 in test2   stat:"<<stat <<"  !for multiservers OK, klice se neinkrementuji"<<std::endl;
		ret = 1;
	}
	//x.clear();
	std::cout <<"ping: "<<x.ping()<<"ms"<<std::endl;
    x.clear();
    x.close();
	return ret;
}

int test3(){
    std::cout <<"ok"<<std::endl;
	RemoteCache x = RemoteCache();
    x.clear();
	int ret = 0;
	int stat;
	long long ver;
	std::cout <<"ok"<<std::endl;
	x.clear();
    std::cout <<"ok"<<std::endl;
	x.put(&std::string("Hell"),&std::string("ff"));
	x.put(&std::string("Hell2"),&std::string("f2"));
	x.put(&std::string("Hell3"),&std::string("f3"));
    std::cout <<"ok"<<std::endl;
	std::map<std::string,std::string>  bulk;

	x.getBulk(&bulk);
	print_map(&bulk);

	std::map<std::string,std::string>  data;
	data["Hell4"] = "f4";
	data["Hell5"] = "f5";
	data["Hell6"] = "f6";
	x.putAll(&data);
	bulk.clear();
	x.getBulk(&bulk);
	print_map(&bulk);

    x.clear();
	x.close();
	return ret;
}

void smallTest(){
	RemoteCache x = RemoteCache();
	x.put(&std::string("Hell"),&std::string("hi"));

/*	std::string resp;
	x.get(&resp, &std::string("Hell"));
	std::cout << resp << std::endl;

	x.clear();
	resp.clear();
	x.putIfAbsent(&std::string("Hell"),&std::string("hi2"));
	x.get(&resp, &std::string("Hell"));
	std::cout << resp << std::endl;

	resp.clear();
	x.replace(&std::string("Hell"),&std::string("hi3"));
	x.get(&resp, &std::string("Hell"));
	std::cout << resp << std::endl;


*/
}

// void loopTest(){
// 	RMMap x;
// 	std::string k ="a";
// 	x[k] = "b";
// 	print_rmmap(&x);

	
// 	RemoteCache c = RemoteCache();
// 	while(1){
		
// 		c.put(&std::string("a"),&std::string("hi"));
// 		std::string ret;
// 		c.get(&ret,&std::string("a"));
// 		std::cout<< ret<<std::flush<<std::endl;
// 		c.print_servers();
// 		//c.close();
// 		sleep(2);
// 	}

// }

void putAllTest(){
	RemoteCache x = RemoteCache();
	std::map<std::string,std::string>  data;
	data["Test1"] = "Sample text #1";
	data["Test2"] = "Sample text #2";
	data["Test3"] = "Sample text #3";
	data["Test4"] = "Sample text #1";
	data["Test5"] = "Sample text #2";
	data["Test6"] = "Sample text #3";
	data["Test7"] = "Sample text #1";
	data["Test8"] = "Sample text #2";
	data["Test9"] = "Sample text #3";
	data["Test10"] = "Sample text #1";
	data["Test11"] = "Sample text #2";
	data["Test12"] = "Sample text #3";
	data["Test13"] = "Sample text #1";
	data["Test14"] = "Sample text #2";
	data["Test15"] = "Sample text #3";
	data["Test12"] = "Sample text #1";
	data["Test16"] = "Sample text #2";
	data["Test18"] = "Sample text #3";
	for(int i='a';i<'z';i++){
		for(int j='a';j<'z';j++){

			data[std::string("t")+(char)i+(char)j] = "bla";
		}
	}

	struct timeval begin;

    gettimeofday(&begin, NULL);
    long long start_time,end_time, first_time, second_time;
        
    start_time = begin.tv_sec;
 

	x.putAll(&data);

	gettimeofday(&begin, NULL);
	end_time = begin.tv_sec;
    first_time = end_time - start_time;

    gettimeofday(&begin, NULL);
    start_time = begin.tv_sec;

	//x.putAllAsync(&data);

	gettimeofday(&begin, NULL);
	end_time = begin.tv_sec;
    second_time = end_time - start_time;

	data.clear();
	x.getBulk(&data);
	print_map(&data);
	std::cout<<"sync put all: "<<first_time<<"s \nasync put all: "<<second_time<<"s"<<std::endl;

}

// int MurmurHash2(void * payload) {
//       int M = 0x5bd1e995;
//        int R = 24;
//       int H = -1;
//       int h = H;
//       int len = payload.length;
//       int offset = 0;
//       while (len >= 4) {
//          int k = payload[offset];
//          k |= payload[offset + 1] << 8;
//          k |= payload[offset + 2] << 16;
//          k |= payload[offset + 3] << 24;

//          k *= M;
//          k ^= k >>> R;
//          k *= M;
//          h *= M;
//          h ^= k;

//          len -= 4;
//          offset += 4;
//       }

//       switch (len) {
//          case 3:
//             h ^= payload[offset + 2] << 16;
//          case 2:
//             h ^= payload[offset + 1] << 8;
//          case 1:
//             h ^= payload[offset];
//             h *= M;
//       }

//       h ^= h >>> 13;
//       h *= M;
//       h ^= h >>> 15;

//       return h;
//    }

int xx(){
  std::cout << "test je OK" << std::endl;
}
#define TISK() xx()

template <typename F>
void doOperation(F f)
{
  int temp=0;

  std::cout << "Result is " << f << std::endl;
}


#include <iostream>
#include <map>
#include <string>
using namespace std;

union Values {
    char asChar;
    unsigned char asUChar;
    short asShort;
    unsigned short asUShort;
    int asInt;
    unsigned int asUInt;
    long asLong;
    unsigned long asULong;  
    float asFloat;
    double asDouble;
    char* asStr;
    std::string* asStdStr;
    Values() { asULong = 0; }
    Values(char in) { asUChar = in; }
    Values(unsigned char in) { asChar = in; }
    Values(short in) { asShort = in; }
    Values(unsigned short in) { asUShort = in; }
    Values(int in) { asInt = in; }
    Values(unsigned int in) { asUInt = in; }
    Values(long in) { asLong = in; }
    Values(unsigned long in) { asULong = in; }
    Values(float in) { asFloat = in; }
    Values(double in) { asDouble = in; }
    Values(char* in) { asStr = in; }
    Values(std::string in) { asStdStr = &in; }
    
    
    operator char() { return asChar; }
    operator unsigned char() { return asUChar; }
    operator short() { return asShort; }
    operator unsigned short() { return asUShort; }
    operator int() { return asInt; }
    operator unsigned int() { return asUInt; }
    operator long() { return asLong; }
    operator unsigned long() { return asULong; }
    operator float() { return asFloat; }
    operator double() { return asDouble; }
    operator std::string*() { return asStdStr; }
    operator char*() { return asStr; }
    
};


namespace std
{
 template<> struct less<VarItem>
        {
           bool operator() (const VarItem& lhs, const VarItem& rhs)
           {
               return 1;
           }
        };

}



int main(){

	// char key[7]; //xx
 //    int seed = 9001;
 //    key[0] = 3;
 //      key[1] = 62;
 //      key[2] = 4; //pocet
 //      key[3] = 120;
 //      key[4] = 120;
 //      key[5] = 120;
 //      key[6] = 120;
   

 //    std::string resp;
	// RemoteCache x = RemoteCache();
 //    std::string name(key);
 //    x.put(&name, &name);
 //    std::cout<< "????"<<resp<<std::flush<<std::endl;   
 //    //x.clear();
 //    x.put(&name, &name);
 //    x.close();

 //    uint tes;
 //    int cis;
 //    cis = MurmurHash3_x64_32( (const char *) key, 5,9001 );
 //    std::cout<<std::dec<< "&&&"<<cis<<std::flush<<std::endl;  
 //    std::cout<<std::dec<< "&&&"<<cis%2147483647<<std::flush<<std::endl;   
 //    std::cout<<std::dec<< "&&&"<<cis%INT_MAX<<std::flush<<std::endl;   


 //    int murmur = MurmurHash3_x64_32(key,7,9001);



 //      std::cout<<std::dec<< "MUR "<<(murmur & INT_MAX) <<std::flush<<std::endl;  


  char key[7]; //xx
    int seed = 9001;
    key[0] = 120;
      key[1] = 120;
      key[2] = 120; //pocet
      key[3] = 120;
      key[4] = 120;
      key[5] = 120;
      key[6] = 120;
   

    std::string resp;
    RemoteCacheConfig remote_cache_config;
    remote_cache_config.cache_name = "";
    remote_cache_config.version = 12;
    RemoteCache x = RemoteCache(&remote_cache_config);
    std::string name(key);


    std::string k("key");
    std::string v("value");
    // k = key;
    x.get(&v, &k);
    v = "value";


    x.put(&k, &v, 1000, 1000);
    v.clear();
    x.get(&v, &k);


    std::cout << "RESP " << v << std::endl;


  //   x.ping();
    // x.close();

    // uint tes;
    // int cis;
    // cis = MurmurHash3_x64_32( (const char *) key, 7,9001 );
    // std::cout<<std::dec<< "&&&"<<cis<<std::flush<<std::endl;  
    // std::cout<<std::dec<< "&&&"<<cis%2147483647<<std::flush<<std::endl;   
    // std::cout<<std::dec<< "&&&"<<cis%INT_MAX<<std::flush<<std::endl;   


    // int murmur = MurmurHash3_x64_32(key,7,9001);
    // std::cout<<std::dec<< "MUR "<<(murmur & INT_MAX) <<std::flush<<std::endl;  

    // cis = MurmurHash3_x64_32( (const char *) key, 7,9001 );
    // std::cout<<std::dec<< "&&&"<<cis<<std::flush<<std::endl;  
    // std::cout<<std::dec<< "&&&"<<cis%2147483647<<std::flush<<std::endl;   
    // std::cout<<std::dec<< "&&&"<<cis%INT_MAX<<std::flush<<std::endl;   


    // int murmur2 = MurmurHash3_x64_32(key,7,9001);
    // std::cout<<std::dec<< "MUR2 "<<(murmur2 & INT_MAX) <<std::flush<<std::endl; 


    // std::cout<<std::dec<< test1() <<std::flush<<std::endl; 
    // std::cout<<std::dec<< test2() <<std::flush<<std::endl; 
    // std::cout<<std::dec<< test3() <<std::flush<<std::endl; 

    // std::map<std::string,std::string>  bulk;

    // x.stats(&bulk);
    // print_map(&bulk);
    // Metadata meta;
    // x.getWithMetadata(&v, &meta, &k);
    // std::cout<<std::dec<<meta.created<<std::endl;
    // // x.get(&v,&"sfs");
    double ff = 5.5323;
    for(int i=0; i<8;i++){
        std::cout<<std::hex<< (0x00ff & ((short)((char *) &ff)[i])) << " ";
    }
    std::cout << std::endl;
    int dd = 9876543;
    for(int i=0; i<4;i++){
        std::cout<<std::hex<< (0x00ff & ((short)((char *) &dd)[i])) << " ";
    }
    std::cout << std::endl;
    k = key;
    x.get(&v, &k);

    MarshallerJBoss mj;
    int in;
    std::string re;
    // mj.load(&re, &v);



    // std::cout << in<<std::endl;
    for(int i=0; i<10;i++){
        std::cout<<std::hex<< (0x00ff & ((short)((char *) v.c_str())[i])) << " ";
    }

    std::cout.precision(15);
  std::cout <<std::dec<< "double " <<re<<"   "<< re.length()<<std::endl;
    std::string vv;
    vv.clear();
    int cislo = 9876543;
    re = "nam to slape muhehehe jojojo ono to jede to je uplne super :Plkj";

   std::cout <<std::dec<< "delka " <<"   "<< re.length()<<std::endl;
    // mj.dump(&vv, &re);
    for(int i=0; i<10;i++){
        std::cout<<std::hex<< (0x00ff & ((short)((char *) vv.c_str())[i])) << " ";
    }
    x.put(&k, &vv, 1000, 1000);

    int ci = 60660;
    char *xx = "xxxxx";

    x.put(xx,ci);
    std::cout<<ci<<std::endl;


    map<string, VarItem> mymap;
    mymap["Item1"] = 2;
    mymap["Item2"] = re;
    Values val = "asdf";
    // re = val.asStdStr;
    mymap["Item3"] = "It was the best of times, it was the worst of times";
    ci = mymap["Item1"];
    // cout << mymap["Item1"].asInt <<" " << mymap["Item2"].asStdStr<<re << endl;
    // cout << mymap["Item3"].asStr <<"  " << ci<<endl;

    VarItem  vi;
    vi = 666;
    ci = vi;
    vi = 66.66;
    // vi = "wow";
    cout <<std::dec<< "  " << ci<< (double)vi<<endl;
    std::vector<VarItem>  keys;
    x.keySet(&keys);
    for(int t=0;t<keys.size();++t){
            std::cout<<(std::string)keys.at(t)<<std::endl;
    }
	return 0;
}



    