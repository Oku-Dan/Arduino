#define RATE_CW 20000 //Hz (Max : 333kHz)
#define RATE_PULSE 1

const unsigned long INTERVAL_CW = 1000000 / RATE_CW;
const unsigned long INTERVAL_PULSE = 1000000 / RATE_PULSE;

/*
	CAUTION!! Pins must have been pulled down.
	D3~D6 CW
	D7 PULSE
*/

void setup(){
	DDRD = DDRD | 0b11111000; // enable D2~D6
	PORTD = PORTD & 0b00000111; // set D2~D6 LOW

	delay(1.02);
	unsigned long last_CW_time = 0;
	unsigned long last_PULSE_time = 0;
	unsigned long interval_PULSE = INTERVAL_PULSE;
	unsigned char reg = 0b01000000;

	while(1){
		register unsigned long time = micros();

		if (time - last_CW_time > INTERVAL_CW){
			last_CW_time = time;
			PORTD = PORTD & 0b10000111;
			PORTD = (PORTD & 0b10000111) | (reg & 0b01111000);
			reg = (~reg & 0b01111000) | (reg & 0b10000111);
		}

		if(time - last_PULSE_time > interval_PULSE){
			last_PULSE_time = time;
			PORTD = (PORTD & 0b01111111) | (reg & 0b10000000);
			reg = (~reg & 0b10000000) | (reg & 0b01111111);
			
			if((PORTD & 0b10000000) > 0)
				interval_PULSE = INTERVAL_PULSE / 4;
			else
				interval_PULSE = INTERVAL_PULSE;
		}
	}
}