#include <Arduino.h>// директива, позволяющая подключать в проект дополнительные файлы с кодом
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>//директива для дисплея

LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей

#define heat_water_temp_pin A0 //Директива, дающая команду препроцессору заменить указанное название
                               // на указанное значение. Чаще всего таким образом объявляют константы.
                               //Такой способ хранения констант не использует оперативную память микроконтроллера.
#define heat_water_pin 12 //греем воду
#define water_in_pin 13 //забираем  воду

#define start_button_pin 7 //кнопка старта
#define time_button_pin 2 //кнопка времени
#define temp_button_pin 3 //кнопка температуры
#define speed_button_pin 4 //кнопка скорости отжима

#define beep_pin 11// пищалка
#define Baraban 5 // барабан
#define Nasos 6 // насос слива


float heat_water_temp;// температура с датчика 
uint8_t set_temp = 30;//температура по дефолту
uint8_t t_wash = 10;//время стирки
uint8_t t_spinning;//время отжима
uint16_t speed = 300;//скорость отжима


byte Start_flag =0;//флаг старта работы

void start_button(void)// старт программы
{
  if(!digitalRead(start_button_pin))//читает состояние пина
  {
    delay(80);//“Приостанавливает” выполнение кода на time миллисекунд.
    tone(beep_pin, 1000, 1000);//tone(pin, frequency, duration) Генерирует звук частоты frequency (Герц)
                              // на пине pin. Опционально – делать это в течение времени duration, миллисекунды
    //noTone(beep_pin);
    Start_flag = 1;
    if(Start_flag)
    {
      lcd.setCursor(1, 0);
      lcd.print("START washing ");
    }
  }
}

void set_param()// установка значений стирки
{
  if(!digitalRead(time_button_pin))//читаем состояние пина -кнопка время
  {
    delay(150);
    tone(beep_pin, 1000, 50); 
    t_wash = t_wash + 10;
    if(t_wash >= 100 )
    {
      t_wash = 10;
    }
    lcd.setCursor(1, 1);
    lcd.print(t_wash);
  }
  
  if(!digitalRead(temp_button_pin))//читаем состояние пина - кнопка температура
  {
    delay(150);
    tone(beep_pin, 1000, 50);
    set_temp = set_temp + 30;
    if(set_temp > 90 )
    {
      set_temp = 30;
    }
    lcd.setCursor(5, 1);
    lcd.print(set_temp);
  }
    
  if(!digitalRead(speed_button_pin))//читаем состояние пига - кнопка скорость отжатия
  {
    delay(150);
    tone(beep_pin, 1000, 50);
    speed = speed + 100;
    if(speed > 900)
    {
      speed = 300;
    }
  }
  lcd.setCursor(10, 1);
  lcd.print(speed);
}

void water_in(void)// забор воды
{
  tone(beep_pin, 300, 200);
  lcd.setCursor(1, 0);
  lcd.print("Water_in     ");
  digitalWrite(water_in_pin,HIGH);//подаем на пин сигнал 1 - на пине 5 Вольт
  delay(8000);
  digitalWrite(water_in_pin,LOW);//подаем на пин сигнал 0 - на пине 0 Вольт
 
}

void heat_water(int set_temp)//подогрев воды
{ 
  tone(beep_pin, 300, 200);
  lcd.setCursor(1, 0);
  lcd.print("Water_heat   ");
  for(int i=0;i<10;i++)
  {
    digitalWrite(heat_water_pin,HIGH);
    delay(500);
    digitalWrite(heat_water_pin,LOW);
    delay(500);
  }
}

void wash (int t_wash)// стирка
{
  tone(beep_pin, 300, 200);
  lcd.setCursor(1, 0);
  lcd.print("####Wash######  ");
  for(int i=0;i<10;i++)
  {
    digitalWrite(Baraban,HIGH);//крутим барабан
    delay(t_wash*10);
    digitalWrite(Baraban,LOW);
    delay(t_wash*30);
  }
}

void rinsing (void)// полоскание
{
  tone(beep_pin, 300, 200);
  lcd.setCursor(1, 0);
  lcd.print("####Rising######  ");
  for(int i=0;i<10;i++)
  {
    digitalWrite(Baraban,HIGH);//крутим барабан
    delay(500);
    digitalWrite(Baraban,LOW);//крутим барабан
    delay(500);
  }
}

void spinning (int speed)// отжим
{
  tone(beep_pin, 300, 200);
  lcd.setCursor(1, 0);
  lcd.print("####Spining######");
  digitalWrite(Baraban,HIGH);//крутим барабан
  delay(speed);
  digitalWrite(Baraban,LOW);
} 

void water_out(void)// слив воды
{
  tone(beep_pin, 300, 200);
  lcd.setCursor(1, 0);
  lcd.print("###Water_out######");
  digitalWrite(Nasos,HIGH);//крутим барабан
  delay(1000);
  digitalWrite(Nasos,LOW);
}

void end_prog(void)// конец программы стирки
{
  lcd.setCursor(1, 0);
  lcd.print("*****END*******");
 
 for(int i=0;i<10;i++)// beep 10 times
 {
   tone(beep_pin, 500, 200);
   delay(500);
 }
 Start_flag = 0;
}

void Get_temp(void)//температура с датчика
{
  heat_water_temp = analogRead(A0);//Читает и возвращает оцифрованное напряжение с пина pin
}

//Функция, содержимое которой выполняется один раз при запуске микроконтроллера.
void setup()
{
  pinMode(start_button_pin,INPUT_PULLUP);//Устанавливает режим работы пина pin на режим mode: подтяжка к питанию - обработка кнопки
  pinMode(time_button_pin,INPUT_PULLUP);
  pinMode(temp_button_pin,INPUT_PULLUP);
  pinMode(speed_button_pin,INPUT_PULLUP);

  pinMode(Nasos,OUTPUT);//Устанавливает режим работы пина pin на режим mode: выход
  pinMode(Baraban,OUTPUT);

  pinMode(heat_water_pin,OUTPUT);
  pinMode(water_in_pin,OUTPUT);
  pinMode(beep_pin,OUTPUT);

  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея
  Serial.begin(9600);//Открыли порт для связи (монитор порта)
  lcd.setCursor(1, 0);
  lcd.print("Ready to START");
  lcd.setCursor(1, 1);
  lcd.print(t_wash);
  lcd.setCursor(5, 1);
  lcd.print(set_temp);
  lcd.setCursor(10, 1);
  lcd.print(speed);
}

//Функция, содержимое которой выполняется “по кругу” на протяжении всего времени работы МК
void loop() {
    start_button(); //начало работы программы - старт
 if(!Start_flag )
 {
   set_param();//set time//set temp//set speed  - установка значений стирки
 }
 if(Start_flag)
  {      
    water_in();//забор воды
    heat_water(set_temp);//подогрев воды
    wash (t_wash);//стирка
    rinsing ();//полоскание
    spinning (speed);//отжим
    water_out();//слив воды
    end_prog();//конец стирки
  }
}