//Libraries for DHT
#include <DHT.h>

//Constants for DHT
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


//DHT Variables
int chk;
float hum;  //Stores humidity value
float temp=1000; //Stores temperature value
float previoustemp;
int led=13;

//Gas sensor Variables
int GasValue = 0;
int GasBoolCheck = 0;
int GasThreshold=750;
int buzzer=48;

//Flame
#define FLAME 14 // connect DO pin of sensor to this pin

//Motors
int pumpPositive=3;
int pumpGround=4;

int frontleft_f= 5;
int frontleft_b=6;
int frontright_f= 8;
int frontright_b=7;

int backleft_f= 9;
int backleft_b=10;
int backright_f= 12;
int backright_b=11;



int everything_okay=0;

void setup()
{
  pinMode(A0, INPUT); //for gas
  pinMode(FLAME, INPUT);//define FLAME input pin
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pumpPositive, OUTPUT);//define FLAME input pin
  pinMode(pumpGround, OUTPUT);//define FLAME input pin
  Serial.begin(9600);
  dht.begin();
}

void loop()
{

  
    //Temperature
    
    delay(2000);
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    previoustemp=temp;
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    if(temp>previoustemp)
    {
      Serial.println("Temperature is increasing.");
      digitalWrite(led, HIGH);
    }
    else if (temp<previoustemp)
    {
      Serial.println("Temperature is decreasing.");    
      digitalWrite(led, LOW);   
    }


    //Gas
    GasValue = analogRead(A0);
    Serial.println(GasValue);
    if((GasValue > GasThreshold) && (GasBoolCheck == 1))
    {
      Serial.println("Smoke Detected . . .");
      digitalWrite(buzzer, HIGH);
      GasBoolCheck = 0;
    }
    
    if((GasValue < GasThreshold) && (GasBoolCheck == 0))
    {
      Serial.println("All Clear . . .");
      digitalWrite(buzzer, LOW);
      GasBoolCheck = 1;
    }

    //Flame
    int fire = digitalRead(FLAME);// read FLAME sensor


    if( fire == HIGH)
    {
      Serial.println("No fire detected.");
      analogWrite(pumpPositive, 0);
      analogWrite(pumpGround, 0);     

      
    }else{
      Serial.println("Fire detected!"); 
      analogWrite(pumpPositive, 160);
      analogWrite(pumpGround, 0);
      delay(10000);
      analogWrite(pumpPositive, 0);
      analogWrite(pumpGround, 0);

    }

    if( temp<=previoustemp && GasValue <= GasThreshold && fire==HIGH)
    {
      
      c_rotate();
      delay(250);
      everything_okay++;
      brake();


 
    }



    if(everything_okay>=6)
    {

      go_forward();
      delay(600);
      everything_okay=0;
      
    }

    delay(500); //Delay 2 sec.
    
}

void go_forward()
{
  analogWrite(frontleft_f, 255);
  analogWrite(frontright_f, 255);
  analogWrite(backleft_f, 255);
  analogWrite(backright_f, 255);
  analogWrite(frontleft_b, 0);
  analogWrite(frontright_b, 0);
  analogWrite(backleft_b, 0);
  analogWrite(backright_b, 0);  
}

void c_rotate()
{

  analogWrite(frontleft_f, 255);
  analogWrite(frontleft_b, 0);
  analogWrite(backleft_f, 255);
  analogWrite(backleft_b, 0);
  analogWrite(frontright_f, 0);
  analogWrite(frontright_b, 155);
  analogWrite(backright_f, 0);
  analogWrite(backright_b, 155);  
  
}

void brake()
{

  analogWrite(frontleft_f, 0);
  analogWrite(frontleft_b, 0);
  analogWrite(backleft_f, 0);
  analogWrite(backleft_b, 0);
  analogWrite(frontright_f, 0);
  analogWrite(frontright_b, 0);
  analogWrite(backright_f, 0);
  analogWrite(backright_b, 0);  
  
}
