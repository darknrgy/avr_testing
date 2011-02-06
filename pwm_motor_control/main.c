#define __OPTIMIZE__
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "../macros/macros.h"
#include "../better_serial/usart.h"
#include "../pwm_motor_control/pwm_motor_controller.h"

int rand_range(a, b){
	int t = rand();
	t = t % (b-a+1);
	t += a;
	return t;
}

int main( void ){
		
	pwmmc_init();
    usart_init();
    sei();
	for(;;){
        sleep_mode();
    }
}


void usart_rx_callback(char *s){

    // give the watchdog_timer a reset to let it know things are still working
    watchdog_timer(0);
    
    char cmd = 0xff;
    if (strcmp(s, "DIR_ROTATE_LEFT") == 0) cmd = PWMMC_ROTATE_LEFT;
    if (strcmp(s, "DIR_ROTATE_RIGHT") == 0) cmd = PWMMC_ROTATE_RIGHT;
    if (strcmp(s, "DIR_FWD_SLOW") == 0) cmd = PWMMC_FWD_SLOW;
    if (strcmp(s, "DIR_FWD_FAST") == 0) cmd = PWMMC_FWD_FAST;
    if (strcmp(s, "DIR_FWD_LEFT") == 0) cmd = PWMMC_FWD_LEFT;
    if (strcmp(s, "DIR_FWD_RIGHT") == 0) cmd = PWMMC_FWD_RIGHT;
    if (strcmp(s, "DIR_REV_SLOW") == 0) cmd = PWMMC_REV_SLOW;
    if (strcmp(s, "DIR_REV_FAST") == 0) cmd = PWMMC_REV_FAST;
    if (strcmp(s, "DIR_REV_LEFT") == 0) cmd = PWMMC_REV_LEFT;
    if (strcmp(s, "DIR_REV_RIGHT") == 0) cmd = PWMMC_REV_RIGHT;
    if (strcmp(s, "DIR_REV_STOP") == 0) cmd = PWMMC_STOP;
    if (strcmp(s, "CALIBRATE_PLUS") == 0){
		pwmmc_calibrate(1);		
	}
	if (strcmp(s, "CALIBRATE_MINUS") == 0){
		pwmmc_calibrate(-1);
		
	}
	if (cmd != 0xff){
    	pwmmc_cmd(cmd);
    }
    usart_psend(PSTR("RCV:"));
    usart_send(s);
    usart_psend(PSTR("\n"));
	return 1;
}


