#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>  
#include <SoftwareSerial.h>
#define WIFI_TX       11
#define WIFI_RX       10  

SoftwareSerial wifi(WIFI_RX, WIFI_TX);   //RX, TX
String _comdata_wifi = "";     

int voice = A0;//声音传感器
char ledtmp;//led
int m1 = 3;//mp3
int m2 = 4;//mp3
int buzzer = 8;

int counter=0; //声音计数
int exit_counter=0;

void setup(){
  Serial.begin(9600);
  Serial.println("system is ready!"); 
  wifi.begin(115200);
  delay(500);
  wifi.println("AT+CWQAP\r\n");
  delay(500);
  wifi.println("AT+CWMODE=3\r\n");
  delay(500);
//  wifi.println("AT+RST\r\n");
//  delay(1000);
 // wifi.println("AT+CWSAP=\"AlphaThink\",\"123456789\",1,0\r\n");
// delay(500);
  wifi.println("AT+CIPMUX=0\r\n");
  delay(500);
  wifi.println("AT+CIPMODE=1\r\n");
  delay(500);

  wifi.println("AT+CWJAP=\"123456789\",\"123456789\"\r\n");
  delay(5000);
  wifi.println("AT+CIPSTART=\"TCP\",\"192.168.1.108\",9092\r\n");
  delay(500); 
//  wifi.println("AT+CIPSEND\r\n");
//  delay(1000);
//  wifi.println("AT+CIPSERVER=1\r\n");
//  delay(500); 

  pinMode(A1,OUTPUT);//led
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  
  pinMode(m1,OUTPUT);//mp3
  pinMode(m2,OUTPUT);
  
  pinMode(voice,INPUT);
  pinMode(buzzer,OUTPUT);
}

void loop() {         
//      while(Serial.available() > 0)
//     {
//      Serial.read();
//     }
//
//     while(wifi.available() > 0)
//     {
//      wifi.read();
//     }
    
//    wifi.println("AT\r\n");
//  delay(500);

   digitalWrite(3,HIGH);
   digitalWrite(4,HIGH);
   digitalWrite(5,HIGH);
   digitalWrite(6,HIGH);
   digitalWrite(7,HIGH);          
//  getWifiSerialData();
  
   tmElements_t tm;
   RTC.read(tm);

  // delay(1000);          测试串口时用
  
  if(tm.Hour>= 1 && tm.Hour <= 23 && tm.Minute >= 0 && tm.Minute <= 59 )   // 22点开始到23点开始工作(两个小时)
    {    
         digitalWrite(A2,HIGH);//绿灯亮
         digitalWrite(A1,LOW);
         digitalWrite(A3,LOW);
         //delay(1000);
         
         /*         提醒休息语句        */
    
         int tmp=analogRead(voice);
        
         //测试使用
         Serial.print("voice is=");
         Serial.println(tmp);        
         delay(500);                     // 测声音延时，可根据需求调整      
      
         if(tmp>700) {                      //有声音
            digitalWrite(A3,HIGH);//蓝灯亮
            digitalWrite(A1,LOW);
            digitalWrite(A2,LOW);
            delay(1000);   
            Serial.println("HIGH!!!");
            int start_M = tm.Minute;
            int start_S = tm.Second;
            print2digits(tm.Minute);
            Serial.write(':');
            print2digits(tm.Second);
            Serial.println(""); 
            while(analogRead(voice)>700){
              RTC.read(tm);
              Serial.print("s");              // 测试使用
             // delay(100);
            }                
            int end_M = tm.Minute;
            int end_S = tm.Second;
            print2digits(tm.Minute);
            Serial.write(':');
            print2digits(tm.Second);
            Serial.println("");   
                    
            if(((end_M - start_M)*60+(end_S-start_S)) >= 1){      // 最少讲 1秒 开始计时

               Serial.println("开始计数！");
               while(1)
               {
                  tmp=analogRead(voice);
                  if(tmp>700)
                  {
                    counter++;
                    Serial.println(counter);
                    }
                   else
                   {
                     exit_counter++;
                    }
                    tmp=0;
                   delay(1000);
                  if(counter >= 10 ) //达到这个标准就报警
                  {
                    //报警提示
                    // MP3  +  灯泡
                    digitalWrite(A1,HIGH);//红灯亮
                    digitalWrite(A2,LOW);
                    digitalWrite(A3,LOW);
                    delay(1000);
                    digitalWrite(m1,LOW);
                    digitalWrite(4,HIGH);
                    digitalWrite(5,HIGH);
                    digitalWrite(6,HIGH);
                    digitalWrite(7,HIGH);
                   for(int i=0;i<100;i++)
                  {
                    digitalWrite(buzzer,HIGH);//发声音
                    delay(1);//延时1ms
                    digitalWrite(buzzer,LOW);//不发声音
                    delay(1);//延时ms
                   }
             
                  /* 开始录声音  ！！！！！！*/

                    Serial.println(_comdata_wifi);
                    Serial.println("\r\n");
      
                  //   wifi.println("AT+CIPSEND=0,23\r\n");
                  //    delay(1000);
                  //wifi.println("AT+CIPSEND=0,5\r\n");
                  // wifi.println(tmp);

                   wifi.println("AT+CIPSEND\r\n");
                   delay(1000);
                   wifi.println("0#192.168.170.221#1-01#");
                   delay(500);   
                   wifi.print("+++");
                   delay(500);   

                               while(wifi.available() > 0)
                               {
                                wifi.read();
                               }

                   digitalWrite(buzzer,LOW);  
                   _comdata_wifi = String("");
                        break;
                    }
                  if(exit_counter >= 5)
                  {
                    break;
                    }
                }
               
               counter=0;   //清零操作
               exit_counter=0;      
            }   
         }
    }
//      while(Serial.available() > 0)
//     {
//      Serial.read();
//     }
//         while(wifi.available() > 0)
//     {
//      wifi.read();
//     }

}

void print2digits(int number) {                   // 时间格式转换函数
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

void getWifiSerialData()
{
  while (wifi.available() > 0)
  {
    _comdata_wifi += char(wifi.read());   
    delay(4);
  }
}

