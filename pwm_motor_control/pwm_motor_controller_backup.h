
void pwmmc_init(){
	
	DDRD = 0x00;
	PORTD = 0xFF;

	// set pin 5 for output
	setbit(DDRB, 0xFF);
	
	// set direction mask
	setbit(PORTB, BIT(DDB0));
	setbit(PORTB, BIT(DDB3));
	
	clearbit(PORTB, BIT(DDB4));
	clearbit(PORTB, BIT(DDB5));
	

    // set timer mode to phase correct 8 bit pwm
    //setbit(TCCR1A, BIT(WGM10));
    
    // set timer compare A to pin 5
    // setbit(TCCR1A, BIT(COM1A1));
	TCCR1A = 0xA1;
	

    // set an initial pulse width of 30,000
    OCR1A = 91; 
    OCR1B = 90; 

	TIMSK = 0x10;

    // set scalar and kick off timer
    setbit(TCCR1B, 0x01);  
}
