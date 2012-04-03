#include <string>
#include "remoteCache.h"
#include "RMMap.h"

void print_map(std::map<std::string,std::string>  *data){
	std::map<std::string,std::string>::iterator pos;
	std::cout << "Bulk:" <<std::endl;
    for (pos = (*data).begin(); pos != (*data).end(); ++pos) {
        std::cout << "key: \"" << pos->first << "\" "<< "value: " << pos->second << std::endl;
    }
}

int test1(){
	RemoteCache x = RemoteCache();
	std::string resp;

	int ret = 0;
	x.clear();
	x.putIfAbsent(&std::string("Hell"),&std::string("aa"));
	//std::cerr << x.get(std::string("Hell")) <<"|"<<std::endl;
	x.get(&resp, &std::string("Hell"));

	if( resp != std::string("aa")){
		std::cerr << "Error #1 in test1" <<std::endl;
		ret = 1;
	}
	resp.clear();

	x.put(&std::string("Hell"),&std::string("bb"));
	x.get(&resp, &std::string("Hell"));
	if(resp != std::string("bb")){
		std::cerr << "Error #2 in test1" <<std::endl;
		ret = 1;
	}
	resp.clear();

	x.replace(&std::string("Hell"),&std::string("cc"));
	x.get(&resp, &std::string("Hell"));
	if(resp!= std::string("cc")){
		std::cerr << "Error #3 in test1" <<std::endl;
		ret = 1;
	}
	resp.clear();

	return ret;

}


int test2(){
	RemoteCache x = RemoteCache();
	std::string resp;
	int ret = 0;
	int stat;
	long long ver;

	x.getWithVersion(&resp, &std::string("Hell"),&ver);
	if(resp != std::string("cc")){
		std::cerr << "Error #1 in test2 "<<std::endl;
		ret = 1;
	}
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


	for(int i=0;i<1000;i++){
		x.put(&std::string("Hell"),&std::string("ee"));
	}
	if((stat = x.replaceWithVersion("Hell","dd",ver+1000))){
		std::cerr << "Error #4 in test2   stat:"<<stat <<std::endl;
		ret = 1;
	}

	if(stat = x.removeWithVersion("Hell",ver+1001)){
		std::cerr << "Error #5 in test2   stat:"<<stat <<std::endl;
		ret = 1;
	}
	//x.clear();
	std::cout <<"ping: "<<x.ping()<<"ms"<<std::endl;
	return ret;
}

int test3(){
	RemoteCache x = RemoteCache();
	int ret = 0;
	int stat;
	long long ver;
	
	x.clear();
	x.put(&std::string("Hell"),&std::string("ff"));
	x.put(&std::string("Hell2"),&std::string("f2"));
	x.put(&std::string("Hell3"),&std::string("f3"));

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
	

	return ret;
}

void smallTest(){
	RemoteCache x = RemoteCache();
	x.put(&std::string("Hell"),&std::string("hi"));

	std::string resp;
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



}

int main(){

	RMMap x;
	std::string k ="a";
	x[k] = "b";
	RemoteCache c = RemoteCache();
	c.put(&std::string("b"),&std::string("hi"));

	std::cout<< x.size() <<std::flush<<std::endl;
	std::cout<< "x[k] "<<x[k] <<std::flush<<std::endl;
	std::cout<< "*** end of RMMap test"<<std::flush<<std::endl;


	if(!test1()){
		std::cout<< "Test 1: OK "<<std::flush<<std::endl;	
	}else{
		std::cout<< "Test 1: failed "<<std::flush<<std::endl;	
	}
	if(!test2()){
		std::cout<< "Test 2: OK "<<std::endl;	
	}else{
		std::cout<< "Test 2: failed "<<std::flush<<std::endl;	
	}
	if(!test3()){
		std::cout<< "Test 3: OK "<<std::endl;	
	}else{
		std::cout<< "Test 3: failed "<<std::flush<<std::endl;	
	}
	

	//smallTest();

	return 0;
}