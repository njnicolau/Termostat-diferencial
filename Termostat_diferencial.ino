#include "DHT.h"
#include <math.h>
#define DHTPIN 7
#define DHTTYPE DHT11
#define RBOMBA 2
#define RCALORAUXILIAR 4
#define RRADIADOR 3
#define TMINCALORAUX 23
#define TMAXPLAQUES 19
#define TCONECTARRADIADOR 55

DHT dht(DHTPIN, DHTTYPE);
int t1;
double t2, t3;
void setup() {
pinMode(RBOMBA, OUTPUT);
Serial.begin(115200);
dht.begin();
}

double ThermisterNTC(int RawADC) {  
  double Temp;
  Temp = log(((10240000/RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;// Converierte de Kelvin a Celsius
  //Para convertir Celsius a Farenheith esriba en esta linea: Temp = (Temp * 9.0)/ 5.0 + 32.0; 
  return Temp;
}

void reles(){
  //Comprova si la temperatura de plaques es superior a la part de baix del deposit
  //Comprova si la temperatura de plaques supera la temperatura maxima de les plaques
  if (t1>t3||t1>TMAXPLAQUES){
    digitalWrite(RBOMBA, HIGH);
  }else{
    digitalWrite(RBOMBA, LOW);
  }
  
  // Conecta el radiador quan la temperatura de ACS supera TCONECTARRADIADOR
  if (TCONECTARRADIADOR<t2){
    digitalWrite(RRADIADOR, HIGH);
  }else{
    digitalWrite(RRADIADOR, LOW);
  }
  // Comprova si es necessaria la caldera
  if (t2<TMINCALORAUX){
      digitalWrite(RCALORAUXILIAR, HIGH);
    }else{
      digitalWrite(RCALORAUXILIAR, LOW);
  }
}

void loop() {
int val;//Crea una variable entera
val=analogRead(0);//Lee el valor del pin analogo 0 y lo mantiene como val
t2=ThermisterNTC(val);//Realiza la conversión del valor analogo a grados Celsius
val=analogRead(1);
t3=ThermisterNTC(val);
Serial.print("NTC SUPERIOR DEPOSIT: ");
Serial.println(t2);//Escribe la temperatura en el monitor serial
Serial.print("NTC INFERIOR DEPOSIT: ");
Serial.println(t3);//Escribe la temperatura en el monitor serial

t1= dht.readTemperature();//Lee la temperatura             
Serial.print("DHT11 PLAQUES: ");                  
Serial.print(t1);//Escribe la temperatura
Serial.println(" C'");
reles();
delay (2500);
Serial.println ();
}
