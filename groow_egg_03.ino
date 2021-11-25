// Arduino таймер CTC прерывание
// avr-libc library includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Adafruit_NeoPixel.h>
#define PIN 8 // номер порта к которому подключен модуль
#define count_led 1 // количество светодиодов
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(count_led, PIN, NEO_GRB + NEO_KHZ800);

int button;
int touch = 0;
int LastTouch;

int button2;
int touch2 = 0;
int LastTouch2;

int calc_0;
int calc;
int WaterLevel;

int in;
long out;

void turnOn(){

    delay(500);
    digitalWrite(4, HIGH);
    delay(500);
    pixels.setPixelColor(0, pixels.Color(255,255,255));
    pixels.show();
    delay(500);
    digitalWrite(7, HIGH);
    delay(500);
    for(int i = 0; i <= 255; i++){
     analogWrite(5, i);
     Serial.println(i);
     //delay(5);
    }
    for(int i = 0; i <= 255; i++){
     analogWrite(6, i);
     Serial.println(i);
     //delay(5);
    }
    //digitalWrite(7, LOW);
    //delay(50);
      for(int i = 255; i >= 0; i--){
      analogWrite(5, i);
      analogWrite(6, i);
      Serial.println(i);
      //delay(1);
      }  
}

void turnOff(){

    
  if(button2 == 1){
      for(int i = 0; i <= 255; i++){
      analogWrite(6, i);
      Serial.println(i);
      delay(1);
      }
      for(int i = 255; i >= 0; i--){
      analogWrite(6, i);
      Serial.println(i);
      delay(1);
      }
      button2 = 0;
    }
  if(button == 0){
      for(int i = 0; i <= 255; i++){
      analogWrite(5, i);
      Serial.println(i);
      //delay(1);
      }
      for(int i = 255; i >= 0; i--){
      analogWrite(5, i);
      Serial.println(i);
      //delay(1);
      }
    } else {
      for(int i = 255; i >= 0; i--){
      analogWrite(5, i);
      Serial.println(i);
      //delay(1);
      }
      button = 0;
    }
    delay(500);
    digitalWrite(7, LOW);
    delay(500);
    calc = 2;
    Serial.println(calc);
    delay(500);
    digitalWrite(4, LOW);
    delay(500);
    digitalWrite(4, HIGH);
    digitalWrite(7, HIGH);
    calc = calc_0;
  
}

void setup(){    

    pixels.begin();
    pixels.show();
    
    attachInterrupt(0, myInterrupt, RISING);
    attachInterrupt(1, myInterrupt1, RISING);
    Serial.begin(9600);              // Серийный порт, скорость 9600hz
    pinMode(9, INPUT_PULLUP);        // 9 пин снимает сигнал с датчика уровня воды
    pinMode(8, OUTPUT);              // 8 пин сигнализирует о уровне воды
    pinMode(7, OUTPUT);              // 7 пин отвечает за вкл. и выкл. подсветки Vv
    pinMode(6, OUTPUT);              // 6 пин отвечает за вкл. и выкл. верхней подсветки устройства
    pinMode(5, OUTPUT);              // 5 пин отвечает за вкл. и выкл. нижней подсветки устройства
    pinMode(4, OUTPUT);              // 4 пин отвечает за вкл. и выкл. подсветки O
    pinMode(3, INPUT);               // 3 пин кнопка включения подсветки Vv
    pinMode(2, INPUT);               // 2 пин кнопка включения подсветки O 
    

    turnOn();

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
  
    if(button2 == 1){                        // если button = 1, то..                 
      Serial.println("button2 = 1 наченаем засвечивание растения");
      for(int i = 0; i <= 255; i++){
       analogWrite(6, i);
       Serial.println(i);
       delay(10);
      }
      
      for(long i = 0; i <= 64800; i++){       // цикл в 18 часов.. 1000 значений = 1 секунда!
        digitalWrite(6, HIGH);                        // 6 пин в 5V..
        if(button2 == 0) break;                       // если button = 0, то остановка цикла!
        delay(1000);                                  // задержка в 1000 микросикунд..
        Serial.println(1);
                       
      }

      if(button2 == 1){                               
        for(int i = 255; i >= 0; i--){
          analogWrite(6, i);
          Serial.println(i);
          delay(10);
        }
      
        for(long i = 0; i <= 21600;  i++){     // цикл в 18 часов.. 1000 значений = 1 секунда!
          digitalWrite(6, LOW);                       // 6 пин в 0V..
          if(button2 == 0) break;                     // если button = 0, то остановка цикла!
          delay(1000);                                // задержка в 1000 микросикунд..
          Serial.println(2); 
        }
      }
      
      if(button2 == 0){
        button2 = 0;           // если button2 = 0, то button2 = 0!
        for(int i = 255; i >= 0; i--){
          analogWrite(6, i);
          Serial.println(i);
          delay(10);
        }
      }
      if(button2 == 1)turnOff();
    }

    // основная программа
}

void downLight(){

   if(button == 1){
     for(int i = 0; i <= 255; i++){
       analogWrite(5, i);
       delay(1000);
     }
     Serial.println("downLight HIGH");
   }
   if(button == 0){
    for(int i = 255; i >= 0; i--){
      analogWrite(5, i);
      delay(1000);
    }
    Serial.println("downLight LOW"); 
   }
}

void myInterrupt(){
  
  touch = digitalRead(2);
  Serial.println("считали пин 2");
  if(touch != LastTouch){
    Serial.println("touch не равно LastTouch");
    if(touch == HIGH){
      Serial.println("touch равно HIGH на 2-oм пине, button++");
      button++;
    }
    if(button > 1) button = 0;
      Serial.println(button);
  }
  LastTouch != touch;
  Serial.println(LastTouch);
    
    if(button == 0 || button == 1){
      downLight();
    }
}

void myInterrupt1(){

  touch2 = digitalRead(3);
  Serial.println("считали пин 3");
    if(touch2 != LastTouch2){
      Serial.println("touch2 не равно LastTouch2");
      if(touch2 == HIGH){
        Serial.println("touch2 равно HIGH на 3-м пине, button2++");
        button2++;
      }
      if(button2 > 1){
        Serial.println("button2 = 0");
        button2 = 0;
      }
      Serial.println(button2);
    }
  LastTouch2 != touch2;
  Serial.println(LastTouch2);
}

ISR(TIMER1_COMPA_vect){
  
   if(WaterLevel == 0){
     pixels.setPixelColor(0, pixels.Color(255,50,0));
     pixels.show();
   } 
   if(WaterLevel == 1){
     pixels.setPixelColor(0, pixels.Color(255,255,255));
     pixels.show();
   }
   if(WaterLevel == 2){
     pixels.setPixelColor(0, pixels.Color(0,0,0));
     pixels.show();
   }
 
   calc_0 = digitalRead(9);
   if(calc == 2){ 
     WaterLevel = 2;
   }else{
    calc = calc_0;
   } 
   if(calc == 1){ 
     WaterLevel = 0;
   }
   if(calc == 0){ 
     WaterLevel = 1;
   }
     
}
