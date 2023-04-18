#define PIN_D7 7
#define PIN_D3 3
bool switch_state=false;

void setup() {
    pinMode(PIN_D7,INPUT_PULLUP);
    pinMode(PIN_D3,OUTPUT);
    digitalWrite(PIN_D3, LOW);
}

void loop() {
    // use tone(PIN_D11, 38000); for transmitting at 38kHz  
    switch_state = digitalRead(PIN_D7); 
    if(switch_state == true){ // modulation 100 Hz - bulid signal with T=10[msec] 
        tone(PIN_D3, 38000); 
        delay(1);
        noTone(PIN_D3);
        delay(9);
    }
    else{ // modulation 200 Hz - bulid signal with T=5[msec]
        tone(PIN_D3, 38000); 
        delay(1);
        noTone(PIN_D3);
        delay(4);   //4,2
    }
}
/*   if(switch_state == true){ // modulation 100 Hz - bulid signal with T=10[msec] 
        tone(PIN_D11, 38000); 
        delayMicroseconds(1000);
        noTone(PIN_D11);
        delayMicroseconds(9000);
    }
    else{ // modulation 200 Hz - bulid signal with T=5[msec]
        tone(PIN_D11, 38000); 
        delayMicroseconds(1000);
        noTone(PIN_D11);
        delayMicroseconds(4000);   //4,2
    }*/
