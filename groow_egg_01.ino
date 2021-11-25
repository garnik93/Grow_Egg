// Arduino таймер CTC прерывание
// avr-libc library includes
#include <avr/io.h>
#include <avr/interrupt.h>
#define LEDPIN 13

int button;
int touch;
int button2;
int touch2;
int LastTouch = 1;
int LastTouch2 = 0;
int calc;
int WaterLevel;
int T = 0;
long led_up;
int led_down;
int times = 0;

void setup()
{
    digitalWrite(4, HIGH);
    attachInterrupt(0, myInterrupt, LOW);
    Serial.begin(9600);             // Серийный порт, скорость 19200hz
    pinMode(12, OUTPUT);             // 12 пин синий светодиод
    pinMode(10, OUTPUT);             // 10 пин желтый светодиод
    pinMode(8, OUTPUT);              // 8 пин красный светодиод
    pinMode(6, OUTPUT);              // 6 пин сигнализирует о нормальном уровне воды
    pinMode(5, OUTPUT);              // 5 пин сигнализирует о низком уровне воды
    pinMode(4, OUTPUT);
    pinMode(A1, INPUT);
    pinMode(A3, INPUT);              // A3 пин снимает сигнал с датчика уровня воды
    pinMode(2, INPUT_PULLUP);        // 2 пин кнопка включения устройства и его режимов 
    pinMode(3, INPUT_PULLUP);
    pinMode(LEDPIN, OUTPUT);

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


void loop()
{

    
    
    if(button == 1){                        // если button = 1, то..                 
      for(long i = 0; i <= 10000; i++){       // цикл в 10 минут.. 100 значений = 1 секунда!
        digitalWrite(12, HIGH);             // 12 пин в 5V..
        if(button == 2) break;              // если button = 2, то остановка цикла!
        delay(10);                          // задержка в 10 микросикунд..
        Serial.println();                 // вывод переменной i в серийный порт..
                       
      }
      for(long i = 0; i <= 100; i++){
        digitalWrite(12, LOW);              // 12 пин в 0V..
        if(button == 2) break;
        delay(10);
        Serial.println();
        
      }
      if(button == 1) button = 1;           // если button = 1, то button = 1!
    }

    if(button == 2){
      
      for(long i = 0; i <= 500; i++){
        digitalWrite(10, HIGH);
        delay(10);
        Serial.println();
        if(button == 3) break;
      }
      digitalWrite(10, LOW);
      if(button == 2) button = 0;  
    }

    if(button == 3){
      digitalWrite(8, HIGH);
      for(long i = 0; i <= 200; i++){
        delay(10);
        Serial.println();
        if(button == 0) break;
      }
      digitalWrite(8, LOW);
      if(button == 3) button = 0;  
    }
    // основная программа
}




void myInterrupt(){
      

    if(touch == LOW){
      T++;
      Serial.println(T);
    } else T = 0;
    if(T > 200){
      digitalWrite(4, LOW);
      T = 0;
    }
    
    touch = digitalRead(2);
    if(touch != LastTouch){
      if(touch == HIGH){
        button++;
      }
      if(button > 3) button = 0;
      
    }
    LastTouch = touch;

    touch2 = digitalRead(3);
    if(touch2 != LastTouch2){
      if(touch2 == HIGH) button2++;
    }
    LastTouch2 = touch2;
}

ISR(TIMER1_COMPA_vect)
{
  led_up++;
  led_down++;
  
  if(button != 0){
      if(WaterLevel == 0){
        digitalWrite(6, LOW);
        if(led_up >= 10){
          times++;
          led_up = 0;
          analogWrite(5, times);
          Serial.println(times);
        }
        if(times == 255) times = 0;
      } else {
        digitalWrite(6, HIGH);
        digitalWrite(5, LOW);
      }
    } else { 
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
    }

    if(button == 0) times = 0;
    
    calc = analogRead(A1);
    if(calc <= 150) WaterLevel = 0;
    else WaterLevel = 1;
    
    //Serial.println("beatch");
    
}
