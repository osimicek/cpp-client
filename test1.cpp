#include <string>
#include "remoteCache.h"

void print_map(std::map<std::string,std::string>  *data){
	std::map<std::string,std::string>::iterator pos;
	std::cout << "Bulk:" <<std::endl;
    for (pos = (*data).begin(); pos != (*data).end(); ++pos) {
        std::cout << "key: \"" << pos->first << "\" "<< "value: " << pos->second << std::endl;
    }
}

int test1(){
	RemoteCache *x = new RemoteCache();
	int ret = 0;
	x->putIfAbsent(&std::string("Hell"),&std::string("aa"));
	//std::cerr << x->get(std::string("Hell")) <<"|"<<std::endl;
	if(*(x->get(&std::string("Hell"))) != std::string("aa")){
		std::cerr << "Error #1 in test1" <<std::endl;
		ret = 1;
	}
	x->put(&std::string("Hell"),&std::string("bb"));
	if(*(x->get(&std::string("Hell"))) != std::string("bb")){
		std::cerr << "Error #2 in test1" <<std::endl;
		ret = 1;
	}
	x->replace(&std::string("Hell"),&std::string("cc"));	
	if(*(x->get(&std::string("Hell"))) != std::string("cc")){
		std::cerr << "Error #3 in test1" <<std::endl;
		ret = 1;
	}

	return ret;

}


int test2(){
	RemoteCache *x = new RemoteCache();
	int ret = 0;
	int stat;
	long long ver;
	if(*(x->getWithVersion(&std::string("Hell"),&ver)) != std::string("cc")){
		std::cerr << "Error #1 in test2 "<<std::endl;
		ret = 1;
	}
	if((stat = x->replaceWithVersion("Hell","dd",ver++))){
		std::cerr << "Error #2 in test2   stat:"<<stat <<std::endl;
		ret = 1;
	}
	if(*(x->get(&std::string("Hell"))) != std::string("dd")){
		std::cerr << "Error #3 in test2" <<std::endl;
		ret = 1;
	}


	for(int i=0;i<1000;i++){
		x->put(&std::string("Hell"),&std::string("ee"));
	}

	if((stat = x->replaceWithVersion("Hell","dd",ver+1000))){
		std::cerr << "Error #4 in test2   stat:"<<stat <<std::endl;
		ret = 1;
	}

	if(stat = x->removeWithVersion("Hell",ver+1001)){
		std::cerr << "Error #5 in test2   stat:"<<stat <<std::endl;
		ret = 1;
	}
	//x->clear();
	std::cout <<"ping: "<<x->ping()<<"ms"<<std::endl;
	return ret;
}

int test3(){
	RemoteCache *x = new RemoteCache();
	int ret = 0;
	int stat;
	long long ver;
	std::map<std::string,std::string>  data;
	x->clear();
	x->put(&std::string("Hell"),&std::string("ff"));
	x->put(&std::string("Hell2"),&std::string("f2"));
	x->put(&std::string("Hell3"),&std::string("f3"));
	print_map(x->getBulk());
	std::cout << "ok"<<std::endl;

	data["Hell4"] = "f4";
	data["Hell5"] = "f5";
	data["Hell6"] = "f6";
	x->putAll(&data);
	print_map(x->getBulk());
	

	return ret;
}
int main(){

	
	/*
	RemoteCache *x = new RemoteCache();
	//x->print_info();
	x->ping();
	std::string value;
	for(int i = 0;i<10; i++){
		value +="žž ";

	}
	x->put("Hell",value.c_str());
	std::cout<< "r "<< x->containsKey("Hell")<<std::endl;
	
	std::cout<< "r "<< x->remove("Hell")<<std::endl;
	std::cout<< "r "<< x->remove("Hell")<<std::endl;
	std::cout<< "r "<< x->containsKey("Hell")<<std::endl;
	std::cout<< "r "<< x->containsKey("Hell")<<std::endl;
	x->clear();*/
	if(!test1()){
		std::cout<< "Test 1: OK "<<std::flush<<std::endl;	
	}
	if(!test2()){
		std::cout<< "Test 2: OK "<<std::endl;	
	}
	if(!test3()){
		std::cout<< "Test 3: OK "<<std::endl;	
	}
	/*response  x;
	std::string s = "aa";
	x.data = &s;
	s +="xx";
	*(x.data) += "ff";
	std::cout<< *x.data<<std::endl;	

	std::map<std::string,std::string> m;
	m["b"] = "g";
	std::cout<< m[std::string("b")]<<std::endl;	
	x.bulk = &m;
	std::cout<< (*x.bulk)[std::string("b")]<<std::endl;*/
	//std::cout<< *x.data<<std::endl;
	/*RemoteCache *x = new RemoteCache();
	int ret = 0;
	x->putIfAbsent(std::string("Hell"),std::string("aa"));
	std::cerr << x->get(std::string("Hell"))<<std::flush<<std::endl;
	if(x->get(std::string("Hell")) != std::string("aa")){
		std::cerr << "Error #1 in test1" <<std::endl;
		ret = 1;
	}*/
	return 0;
}