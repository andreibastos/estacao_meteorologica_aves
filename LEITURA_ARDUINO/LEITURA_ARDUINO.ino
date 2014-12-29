/* 
 [AQUISIÇÃO- EQUIPE AVES]
 DATA=17/10/2014
 horario = 16h30m
 responsável pelo código: andrei bastos
 
 Modelo Arduino = "Arduino Uno"
 
 BIBLIOTECAS REQUERIDAS = {  
 Para gravação no cartão de memória: 
 sdfat, disponivel em https://code.google.com/p/sdfatlib/downloads/list
 
 Para leitura do sensor BPM
 adafruit_BMP085 disponivel em https://github.com/adafruit/Adafruit-BMP085-Library
 
 Para leitura do sensor DHT
 dht disponivel em https://github.com/adafruit/DHT-sensor-library
 
 obs: baixa as lib em cada site, extraias e copie para pasta libraries em arduino no seu computador. 
 remova os traços " - " e substitua sempre por " _ " ou só remove, pq arduino não consegue ler uma pasta com esse simbolo, 
 }
 }
 
 COMPONENTES = {
 1 X SHIELD ETHERNET HANRUN HR911105A 13/52
 1 X SENSOR BAROMETRICO (BPM085);
 1 X SENSOR UMIDADE (DHT)
 
 }
 
 LIGAÇÕES={
 SENSOR BAROMETRICO = { 
 VCC of the BMP085 sensor to 3.3V (NOT 5.0V!);
 GND to Ground;
 SCL thats Analog 5 ; to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc 
 SDA thats Analog 4 ; to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc 
 EOC is not used, it signifies an end of conversion
 XCLR is a reset pin, also not used here  
 }
 
 SENSOR UMIDADE= {
 VCC - 5v
 GND - GROUND
 S   - pin 2
 
 IDEAL COLOCAR UM RESISTOR DE 10K ENTRE O PIN 2 E VCC para elimilar os ruidos
 
 }
 
 }
 */


//IMPORTANDO BIBLIOTECAS
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <SdFat.h>
#include "DHT.h"

//objeto do barometro
Adafruit_BMP085 bmp;


#define DHTPIN 3     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

// file system object
SdFat sd;

// SD chip select pin
const uint8_t chipSelect = 4;

//Inicialização para SD
SdFile aFile;               
SdFile bFile;
SdFile cFile; 
SdFile dFile; 
SdFile eFile; 
SdFile fFile; 
String dataString;
String separador = ",";

const uint8_t intervaloLeitura = 30; //segundos

unsigned int contador = 0;



//tempo para os dados do sensor barometro
unsigned long timeold = 0;
unsigned long timeold1 = 0;

//tempo para os dados do sensor dht
unsigned long timeold2 = 0;



//

unsigned  int contTemp = 0;
unsigned  int contPress = 0;
unsigned  int contDHT = 0;

unsigned long long timeP = millis(); //esse numero tem que ser muito grande, pois ele sempre pega o time. [64-bit]unsigned long long: 0 to 18446744073709551615

double temp1 = 0;
double pressure2 = 0;
double  altitude2 = 0;
double realAltitude2 = 0; 


double h2 = 0;
double t2 = 0;


//para saber se os sensores então ok!
boolean defeito = false ;


void openFiles(){

  aFile.open("a.csv", O_WRITE | O_CREAT | O_AT_END);
  bFile.open("b.csv", O_WRITE | O_CREAT | O_AT_END);
  cFile.open("c.csv", O_WRITE | O_CREAT | O_AT_END);
  dFile.open("d.csv", O_WRITE | O_CREAT | O_AT_END);
  eFile.open("e.csv", O_WRITE | O_CREAT | O_AT_END);
  fFile.open("f.csv", O_WRITE | O_CREAT | O_AT_END);
}


void closeFiles(){

  aFile.close();
  bFile.close();  
  cFile.close();
  dFile.close(); 
  eFile.close(); 
  fFile.close(); 

}


void fistPrint()
{
  String inicio =  "0" + separador + "0";

  String dataClose = "date" + separador + "close";
  aFile.println(dataClose);
  bFile.println(dataClose);
  cFile.println(dataClose);
  dFile.println(dataClose);
  eFile.println(dataClose);
  fFile.println(dataClose);

}



boolean testeSDcard()
{
  pinMode(chipSelect, OUTPUT);


  //verifica se o cartao esta inserido , se tiver defeito = true, se verificar todos os sensores e der true então pode funcionar, se não o led azul vai piscar, se algum der feito o vermelho piscará em frequencias diferentes
  if (!sd.begin(chipSelect)) {
    Serial.println("initialization SDcard..... fail!!");   
    return false;
  }
  else{
    Serial.println("initialization SDcard..... ok!!");    
    return true; 
  } 
}

boolean testeBarometro()
{

  if (!bmp.begin()) {
    Serial.println("initialization Barometro..... fail!!");  
    return false;
  }
  else
  {
    Serial.println("initialization Barometro..... ok!!");    
    return true;
  } 
}

boolean testeDHT()
{
  double h = dht.readHumidity();
  // Read temperature as Celsius
  double t = dht.readTemperature();
 
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("initialization DHT..... fail!!");    
    return false;
  }
  else{
    Serial.println("initialization DHT..... ok!!");    
    return true;
  } 
}


void setup(){

  dht.begin();

  int condicoesLED = 0;
  Serial.begin(9600);

  if (testeSDcard())
  {
    openFiles();
    fistPrint(); 
  }
  else
  {
    condicoesLED = condicoesLED + 1 ;
  }


  if(!testeBarometro())
  {
    condicoesLED = condicoesLED + 3 ;
  }

  if (!testeDHT())
  {
    condicoesLED = condicoesLED + 5;
  }


Serial.print("periodo de amostras:");
Serial.println(intervaloLeitura);

}


void loop(){
  // Real Altitude em metros, Se sabido a pressão
// ao nível do mar, que varia de com o tempo/clima
// Definido aqui como 101600 Pascal são josé dos campos
long pressure = 101600;
  setFileData(pressure);    
}

void setFileData(int pressureReal){
  //essa função verifica se está na hora de pegar o valor do sensor e grava no cartão.

  unsigned long timeSegundos = millis()/1000;
  double temp = bmp.readTemperature();
  double pressure = bmp.readPressure();
  double altitude =bmp.readAltitude(); 
  double realAltitude = bmp.readAltitude(pressure);



  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  




  if((millis() - timeP) >= intervaloLeitura*1000)
  {
    
    
   
    Serial.print(temp1/contTemp);
    Serial.print(separador);
    
    Serial.print(pressure2/contPress);
    Serial.print(separador); 
    
    Serial.print(altitude2/contPress);
    Serial.print(separador); 
    
    Serial.print(realAltitude2/contPress);
    Serial.print(separador); 
    
    Serial.print(t2/contDHT);
    Serial.print(separador); 
    
    Serial.println(h2/contDHT);
    
    
    // Temperatura BMP085 em ºC 
    aFile.print(timeSegundos);
    aFile.print(separador);            
    aFile.println(temp1/contTemp);     

    // Pressão BMP085 em Pa
    bFile.print(timeSegundos);
    bFile.print(separador);
    bFile.println(pressure2/contPress);

    // Altitude em metros
    cFile.print(timeSegundos);
    cFile.print(separador);   
    cFile.println(altitude2/contPress);

    // Real Altitude em metros, Se sabido a pressão
    // ao nível do mar, que varia de com o tempo/clima
    // Definido aqui como 102300 Pascal
    dFile.print(timeSegundos);
    dFile.print(separador);
    dFile.println(realAltitude2/contPress);


    // Temperatura DHT em ºC 
    eFile.print(timeSegundos);
    eFile.print(separador);            
    eFile.println(t2/contDHT);    


    // Temperatura DHT em ºC 
    fFile.print(timeSegundos);
    fFile.print(separador);            
    fFile.println(h2/contDHT);    

    contPress = 0;
    contTemp = 0;
    contDHT = 0;

    temp1 = 0;
    pressure2 = 0;
    altitude2 = 0;
    realAltitude2 =0;
    t2 = 0;
    h2 = 0;

    closeFiles();    
    openFiles();   


//    Serial.print("FIM DA GRAVACAO: ");
//    Serial.println(contador);
//    Serial.print("tempo: ");
//    Serial.println(timeSegundos);
    contador++;

    timeP = millis();
  }

  if((millis()-timeold)>=100)  // Loop dos sensores a 10Hz
  {
    temp1  = temp1 + temp;
    contTemp++;  

    timeold = millis();
  }

  if((millis()-timeold1)>=50)  // Loop dos sensores a 20Hz
  {
    pressure2 = pressure2 + pressure;
    altitude2 = altitude2 + altitude;
    realAltitude2 = realAltitude2 + realAltitude; 
    contPress++;
    timeold1 = millis();
  }



  if((millis()-timeold2)>=2000)  // Loop dos sensores a 0.5Hz
  {
    h2 = h2 + h;
    t2 = t2 + t;
    contDHT++;
    timeold2 = millis();
  }


}







