// #define F_CPU 1000000UL  // 1 MHz
#include <avr/io.h>
#include <stdlib.h>	
#include <util/delay.h>
#include <string.h>

int rand_range(int a, int b){
	int t = rand();
	t = t % (b-a+1);
	t += a;
	return t;
}


int main(){

	char bytes[4] = {0x8f,0xc2,0x75,0x3c};

	//float* test;
	//test = (float*) &bytes[0];

	float test;
	memcpy(&test, &bytes[0], 4);
	
	DDRB = 0xFF;
	int rand_num;

	for(;;){
		PORTB = 0x01;
		//rand_num  = rand_range(0,255);
		rand_num = 255	;
		_delay_ms(rand_num);
		PORTB = 0x02;
		//rand_num  = rand_range(0,255);
		rand_num = 255;
		_delay_ms(rand_num);
		
	}
}



/*
typedef struct TestStruct{
	char a;
	char b;
} TestStruct;

TestStruct getnew(){
	TestStruct t;
	t.a = 0;
	t.b = 0;
	return t;
}

void testFunc(){
	static TestStruct t[2] = {0,0};
}


TestStruct* getTest(char chan){
	static TestStruct a[2];
	return &a[chan];

}
*/
