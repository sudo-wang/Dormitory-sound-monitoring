#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>    

int voice = A0;//声音传感器
int buzzer=13;//设置控制蜂鸣器的数字IO脚
int i = 0;
         
void setup(){
  pinMode(voice,INPUT);
  pinMode(buzzer,OUTPUT);//设置数字IO脚模式，OUTPUT为输出 
  Serial.begin(115200);
  Serial.println("system is ready!"); 
}

void loop() {
  
  tmElements_t tm;
  RTC.read(tm);
  /*                       // 输出时间
  print2digits(tm.Hour);
  Serial.write(':');
  print2digits(tm.Minute);
  Serial.write(':');
  print2digits(tm.Second);
  Serial.print(", Date (D/M/Y) = ");
  Serial.print(tm.Day);
  Serial.write('/');
  Serial.print(tm.Month);
  Serial.write('/');
  Serial.print(tmYearToCalendar(tm.Year));
  Serial.println();
  */
  // delay(1000);          测试串口时用
  if(tm.Hour>= 22 && tm.Hour <= 23 && tm.Minute >= 12 && tm.Minute <= 50 )   // 22点开始到23点开始工作(两个小时)
    {
         int tmp=analogRead(voice);
         //测试使用
         /*  
         Serial.print("voice is=");
         Serial.println(tmp);
         */
         // delay(500);                     // 测声音延时，可根据需求调整
         if(tmp>700) {                      //有声音
            Serial.println("HIGH!!!");
            int start_M = tm.Minute;
            print2digits(tm.Minute);
            while(analogRead(voice)>700){
              RTC.read(tm);
              Serial.print("s");              // 测试使用
              delay(100);
            }      
            int end_M = tm.Minute;
            print2digits(tm.Minute);
            if((end_M - start_M) >= 4){      // 最少讲 3 分钟开始计时
              //报警提示
              for(i=0;i<80;i++){              //输出一个频率的声音(警报)
                digitalWrite(buzzer,HIGH);   //发声音
                delay(1);//延时1ms 
                digitalWrite(buzzer,LOW);    //不发声音
                delay(1);//延时ms 
             } 
              // 开始录声音  ！！！！！！
            }   
         }
    }
}

void print2digits(int number) {                   // 时间格式转换函数
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}



