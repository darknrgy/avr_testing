
// initialize analog input
void analog_input_init(){

	//return 1;

	// set ADC clock presclar
	setbit (ADCSRA, BIT(ADPS1) | BIT(ADPS0));

	// set reference voltage
	setbit (ADMUX, BIT(REFS0));

	// enable continuous update mode
	setbit(ADCSRA, BIT(ADFR));

	// use 8-bit mode
	setbit(ADMUX, BIT(ADLAR)); 

	// enable adc
	setbit(ADCSRA, BIT(ADEN)); 

	// kick off the adc, continuous mode
	setbit(ADCSRA, BIT(ADSC));
}


