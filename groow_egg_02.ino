// Arduino таймер CTC прерывание
// avr-libc library includes
#include <avr/io.h>
#include <avr/interrupt.h>

int button;
int touch;
int LastTouch;

int button2;
int touch2;
int LastTouch2;

int calc;
int WaterLevel;

void setup()
{
    attachInterrupt(0, myInterrupt, CHANGE);
    attachInterrupt(1, myInterrupt1, CHANGE);
    Serial.begin(9600);              // Серийный порт, скорость 19200hz
    pinMode(9, OUTPUT);              // 9 пин отвечает за вкл. и выкл. нижней подсветки устройства
    pinMode(8, OUTPUT);              // 8 пин красный светодиод
    pinMode(6, OUTPUT);              // 6 пин сигнализирует о уровне воды
    pinMode(7, INPUT);               // 7 пин снимает сигнал с датчика уровня воды
    pinMode(2, INPUT);               // 2 пин кнопка включения устройства и его режимов 
    pinMode(3, INPUT);

        // инициализация Timer1
    cli();  // отключить глобальные прерывания
    TCCR1A = 0;   // установить регистры в 0
    TCCR1B = 0;

    OCR1A = 15624; // установка регистра совпадения

    TCCR1B |= (1 << WGM12);  // включить CTC режим 
    TCCR1B |= (1 << CS10); // Установить биты на коэффициент деления 1024
    TCCR1B |= (0 << CS11);
    TCCR1B |= (0 << CS12);

    TIMSK1 |= (1 << OCIE1A);  // включить прерывание по совпадению таймера 
    sei(); // включить глобальные прерывания
}


void loop(){
  
    if(button == 1){                        // если button = 1, то..                 
      for(long i = 0; i <= 10000; i++){       // цикл в 10 минут.. 100 значений = 1 секунда!
        digitalWrite(8, HIGH);             // 12 пин в 5V..
        if(button == 0) break;              // если button = 2, то остановка цикла!
        delay(10);                          // задержка в 10 микросикунд..
        Serial.println();
                       
      }
      for(long i = 0; i <= 100; i++){
        digitalWrite(8, LOW);              // 12 пин в 0V..
        if(button == 0) break;
        delay(10);
        Serial.println();
        
      }
      if(button == 0) button = 0;           // если button = 1, то button = 1!
    }

    // основная программа
}


void myInterrupt(){
  
    touch = digitalRead(2);
    Serial.println("считали пин 2");
    if(touch != LastTouch){
      if(touch == HIGH){
        button++;
      }
      if(button > 1) button = 0;
      
    }
    LastTouch = touch;
    Serial.println(LastTouch);

}


void myInterrupt1(){

  touch2 = digitalRead(3);
    if(touch2 != LastTouch2){
      if(touch2 == HIGH) button2++;
      if(button2 > 1) button2 = 0;
    }
    LastTouch2 = touch2;
    
    if(button2 == 0) digitalWrite(9, LOW);
    if(button2 == 1) digitalWrite(9, HIGH);
    else button2 = 0;

    Serial.println("LastTouch2 = " + LastTouch2);
    Serial.println("touch2 = " + touch2);
    Serial.println("button2 = " + button2);
    
}


ISR(TIMER1_COMPA_vect){
  
  
   if(WaterLevel == 0){
     digitalWrite(6, HIGH);
   } else {
     digitalWrite(6, LOW);
   }
 
    
    
    calc = digitalRead(7);
    if(calc == 1){ 
      WaterLevel = 1;
     }else { 
      WaterLevel = 0;
     }
    
    
    //Serial.println("beatch");
    
}
