
// speed
#define PWMMC_STOP 0x00
#define PWMMC_SLOW 175
#define PWMMC_MORESLOW 75
#define PWMMC_FAST 255



// direction masks
#define PWMMC_CHAN_L_FWD 0x08
#define PWMMC_CHAN_R_FWD 0x01
#define PWMMC_CHAN_L_REV 0x20
#define PWMMC_CHAN_R_REV 0x10

// channel masks
#define PWMMC_CHAN_L 0x28
#define PWMMC_CHAN_R 0x11

// commands
// the last 4 bits tell you which direction each motor is going
// use this to decide if you need to delay a reversal of a motor
#define PWMMC_ROTATE_LEFT   0x06
#define PWMMC_ROTATE_RIGHT  0x09
#define PWMMC_FWD_SLOW      0x0A
#define PWMMC_FWD_FAST      0x1A
#define PWMMC_FWD_LEFT      0x2A
#define PWMMC_FWD_RIGHT     0x3A
#define PWMMC_REV_SLOW      0x05
#define PWMMC_REV_FAST      0x15
#define PWMMC_REV_LEFT      0x25
#define PWMMC_REV_RIGHT     0x35


// initialize PWMMC
void pwmmc_init(){
	
	// set  PORTB to output mode
	setbit(DDRB, 0xFF);
    
    // set initial values
    OCR1A = 0x00; // L speed
    OCR1B = 0x00; // R speed
    setbit(PORTB, PWMMC_CHAN_L_FWD | PWMMC_CHAN_R_FWD); // direction
    
    // enable output compare match channel A intterupt
    setbit(TIMSK, BIT(OCIE1A));
    
    // initialize timer
    // 8 bit PWM non-inverted
	setbit(TCCR1A, BIT(COM1A1) | BIT(COM1B1) | BIT(WGM10));
	
	// TCCR1A = 0xA1;
	
    // set scalar and kick off timer
    setbit(TCCR1B, BIT(CS10));
	//setbit(TCCR1B, 0x02);  
}


// cheat and use the compare A interrupt to increment our watchdog_timer counter
ISR(TIMER1_COMPA_vect){
	watchdog_timer(1);
}

// watchdog_timer,
// call with increment = 0 to reset it
void watchdog_timer(int reset){
    
    static uint32_t counter;
    
    if (reset == 0){
        counter = 0;    
        return;
    }
    
    if (counter == -1) return;
    
    counter ++;
    
    if (counter > 128000){
        pwmmc_cmd(PWMMC_STOP);
        counter = -1;
		usart_psend(PSTR("LAAAAAAAAAG\n"));
    }
    return;
}

// calibrate the direction
int pwmmc_calibrate(int calibrate){
	static int calibration = 0;
	if (calibrate){
		calibration += calibrate;		
	}
	return calibration;
	
}


// assign a direction to a motor
void pwmmc_motor_direction(char channel, char direction){
 	if (PORTB & channel & direction) return;
    flipbit(PORTB, channel);
}


// assign a speed to a motor
void pwmmc_motor_speed(char channel, char speed){
    if (channel & PWMMC_CHAN_L){
        OCR1A = speed;        
    }else{
        OCR1B = speed? (speed - pwmmc_calibrate(0)):0;
    }
}

// brakes
void pwmmc_brake(char state){
	if (state){
		setbit(PORTB, PWMMC_CHAN_L | PWMMC_CHAN_R);
		pwmmc_motor_speed(PWMMC_CHAN_L, PWMMC_FAST);
		pwmmc_motor_speed(PWMMC_CHAN_R, PWMMC_FAST);
	}else{
		clearbit(PORTB, PWMMC_CHAN_L | PWMMC_CHAN_R);
		setbit(PORTB, PWMMC_CHAN_L_FWD | PWMMC_CHAN_R_FWD);
		pwmmc_motor_speed(PWMMC_CHAN_L, PWMMC_STOP);
		pwmmc_motor_speed(PWMMC_CHAN_R, PWMMC_STOP);
	}
}



void pwmmc_cmd(char cmd){
    
    static char prev = 0x00;
    
    // if motors switch direction, pause for a bit
    if ((prev != PWMMC_STOP) && 
		(( (cmd & 0x0C) && ( (cmd & 0x0C) != (prev & 0x0C) ) ) || 
        ( (cmd & 0x03) && ( (cmd & 0x03) != (prev & 0x03) ) ) )){        
		
		
		
		pwmmc_brake(1);
		_delay_ms(250);
		pwmmc_brake(0);
		
    }   
    
        
    switch (cmd){    
        case PWMMC_ROTATE_LEFT:
            pwmmc_motor_speed(PWMMC_CHAN_L, PWMMC_SLOW);
            pwmmc_motor_speed(PWMMC_CHAN_R, PWMMC_SLOW);
            pwmmc_motor_direction(PWMMC_CHAN_L, PWMMC_CHAN_L_REV);
            pwmmc_motor_direction(PWMMC_CHAN_R, PWMMC_CHAN_R_FWD);            
            break;
        case PWMMC_ROTATE_RIGHT:
            pwmmc_motor_speed(PWMMC_CHAN_L, PWMMC_SLOW);
            pwmmc_motor_speed(PWMMC_CHAN_R, PWMMC_SLOW);
            pwmmc_motor_direction(PWMMC_CHAN_L, PWMMC_CHAN_L_FWD);
            pwmmc_motor_direction(PWMMC_CHAN_R, PWMMC_CHAN_R_REV);
            break;
        case PWMMC_FWD_SLOW:
            pwmmc_motor_speed(PWMMC_CHAN_L, PWMMC_SLOW);
            pwmmc_motor_speed(PWMMC_CHAN_R, PWMMC_SLOW);
            pwmmc_motor_direction(PWMMC_CHAN_L, PWMMC_CHAN_L_FWD);
            pwmmc_motor_direction(PWMMC_CHAN_R, PWMMC_CHAN_R_FWD);
            break;
        case PWMMC_FWD_FAST:
            pwmmc_motor_speed(PWMMC_CHAN_L, PWMMC_FAST);
            pwmmc_motor_speed(PWMMC_CHAN_R, PWMMC_FAST);
            pwmmc_motor_direction(PWMMC_CHAN_L, PWMMC_CHAN_L_FWD);
            pwmmc_motor_direction(PWMMC_CHAN_R, PWMMC_CHAN_R_FWD);
            break;  
        case PWMMC_FWD_LEFT:
            pwmmc_motor_speed(PWMMC_CHAN_L, PWMMC_MORESLOW);
            pwmmc_motor_speed(PWMMC_CHAN_R, PWMMC_FAST);
            pwmmc_motor_direction(PWMMC_CHAN_L, PWMMC_CHAN_L_FWD);
            pwmmc_motor_direction(PWMMC_CHAN_R, PWMMC_CHAN_R_FWD);
            break;
        case PWMMC_FWD_RIGHT:
            pwmmc_motor_speed(PWMMC_CHAN_L, PWMMC_FAST);
            pwmmc_motor_speed(PWMMC_CHAN_R, PWMMC_MORESLOW);
            pwmmc_motor_direction(PWMMC_CHAN_L, PWMMC_CHAN_L_FWD);
            pwmmc_motor_direction(PWMMC_CHAN_R, PWMMC_CHAN_R_FWD);
            break;
        case PWMMC_REV_SLOW:
            pwmmc_motor_speed(PWMMC_CHAN_L, PWMMC_SLOW);
            pwmmc_motor_speed(PWMMC_CHAN_R, PWMMC_SLOW);
            pwmmc_motor_direction(PWMMC_CHAN_L, PWMMC_CHAN_L_REV);
            pwmmc_motor_direction(PWMMC_CHAN_R, PWMMC_CHAN_R_REV);
            break;
		case PWMMC_REV_FAST:
            pwmmc_motor_speed(PWMMC_CHAN_L, PWMMC_FAST);
            pwmmc_motor_speed(PWMMC_CHAN_R, PWMMC_FAST);
            pwmmc_motor_direction(PWMMC_CHAN_L, PWMMC_CHAN_L_REV);
            pwmmc_motor_direction(PWMMC_CHAN_R, PWMMC_CHAN_R_REV);       
            break; 
        case PWMMC_REV_LEFT:
            pwmmc_motor_speed(PWMMC_CHAN_L, PWMMC_SLOW);
            pwmmc_motor_speed(PWMMC_CHAN_R, PWMMC_FAST);
            pwmmc_motor_direction(PWMMC_CHAN_L, PWMMC_CHAN_L_REV);
            pwmmc_motor_direction(PWMMC_CHAN_R, PWMMC_CHAN_R_REV);
            break;
        case PWMMC_REV_RIGHT:
            pwmmc_motor_speed(PWMMC_CHAN_L, PWMMC_FAST);
            pwmmc_motor_speed(PWMMC_CHAN_R, PWMMC_SLOW);
            pwmmc_motor_direction(PWMMC_CHAN_L, PWMMC_CHAN_L_REV);
            pwmmc_motor_direction(PWMMC_CHAN_R, PWMMC_CHAN_R_REV);
            break;
        case PWMMC_STOP:
			pwmmc_motor_speed(PWMMC_CHAN_L, PWMMC_STOP);
			pwmmc_motor_speed(PWMMC_CHAN_R, PWMMC_STOP);


            break;
    }
	prev = cmd;
}










