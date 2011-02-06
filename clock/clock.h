
// global variable used to store clock value
// resolution is 0.1ms, 0.0001s
// uint64_t 
uint64_t unix_clock = 0;

// start the clock at 0
void unixclock_init(){
	
	OCR2 = 125;

	// enable interrupt for timer compare
	setbit(TIMSK, BIT(OCIE2));

	// set ctc mode and scalar of 64
	setbit(TCCR2, BIT(CS22) | BIT(WGM21));
}

// | BIT(CS21) | BIT(CS20)| 

// get the clock value, in seconds
//
uint32_t unixclock_gettime(){
	uint64_t result;	
	result = unix_clock / 1000;
	return result;

}

// set the clock
void unixclock_settime(uint32_t time){
	unix_clock = time;
	unix_clock *= 1000;	
}

// increment clock on timer compare interrupt
ISR(TIMER2_COMP_vect){
	unix_clock += 8;		
}
