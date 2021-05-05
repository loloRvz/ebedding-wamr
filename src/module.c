


int sum(int a, int b){
	return a+b;
}


__attribute__((import_name("callback"))) void callback(void);

void callcallback(){
	callback();
}






