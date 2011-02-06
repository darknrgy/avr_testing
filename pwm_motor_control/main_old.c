#define F_CPU 1000000UL  // 1 MHz
#define __OPTIMIZE__
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../macros/macros.h"
#include "../pwm_motor_control/pwm_motor_controller.h"

int rand_range(a, b){
	int t = rand();
	t = t % (b-a+1);
	t += a;
	return t;
}

int main( void ){
	int r	;
	sei();
	pwmmc_init();
	for(;;){

		r = rand_range(1,11);

		switch (r){

			case 1:	pwmmc_cmd(PWMMC_ROTATE_LEFT);
					break;
			case 2:	pwmmc_cmd(PWMMC_ROTATE_RIGHT);
					break;
			case 3:	pwmmc_cmd(PWMMC_FWD_SLOW);
					break;
			case 4:	pwmmc_cmd(PWMMC_FWD_FAST);
					break;
			case 5:	pwmmc_cmd(PWMMC_FWD_LEFT);
					break;
			case 6:	pwmmc_cmd(PWMMC_FWD_RIGHT);
					break;
			case 7:	pwmmc_cmd(PWMMC_REV_SLOW);
					break;
			case 8:	pwmmc_cmd(PWMMC_REV_FAST);
					break;
			case 9:	pwmmc_cmd(PWMMC_REV_LEFT);
					break;
			case 10:pwmmc_cmd(PWMMC_REV_RIGHT);
					break;
			case 11:pwmmc_cmd(PWMMC_STOP);
					break;
		}
		_delay_ms(1500);
	}
}


