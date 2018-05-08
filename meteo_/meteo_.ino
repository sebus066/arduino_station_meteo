// station meteo version 2.2
// inclus l'utilisation de capteur dht 22
// Connect  BMP sensor en 3.3V (pas 5.0V!)
// Connect GND sur Ground


// initialisation des librairies
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define DHTPIN            2  // capteur dht22 sur pin 2

// definir le type de capteur que vous utilisé:

//#define DHTTYPE           DHT11     // DHT 11
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

//DHT_Unified dht(DHTPIN, DHTTYPE);
DHT dht(DHTPIN, DHTTYPE);

uint32_t delayMS;



LiquidCrystal_I2C lcd(0x3F, 20, 4); // adresse du lcd

Adafruit_BMP085 bmp;

int cor = 8400;                    // variable de correction altimetrique MOI JE DOIS RAJOUTER 8400 POUR RAMENER MA PRESSION AU NIVEAU DE LA MER car je suis à 1360 m d'altitude

void setup() {

  lcd.init();                      // initialize le lcd
  lcd.clear();                     // effece la LCD
  lcd.backlight();                 // Allume le retro éclairage du lcd

  //affichage du message de bien venue
  lcd.setCursor(7, 0);
  lcd.print("Bonjour");
  lcd.setCursor(4, 2);
  lcd.print("Station Meteo");
  lcd.setCursor(5, 3);
  lcd.print(" version 2.2");
  delay(2000);

  Serial.begin(9600);
  Serial.println("Station Météo version 2.2");
  
  if (!bmp.begin()) {

    Serial.println("BPM085 Sensor erreur");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BPM085 Sensor erreur");
    //delay(4000);

    while (1) {}

  }
  
dht.begin();
Serial.println("DHTxx Unified Sensor Example");
  
 

  

pinMode(6, OUTPUT);// LED ROUGE qui indique une tendance à la baisse

digitalWrite(7, LOW);

pinMode(7, OUTPUT); // LED VERTE qui indique une tendance à la hausse

digitalWrite(6, HIGH);

pinMode(10, OUTPUT); // BUZZER ALARME tempete

digitalWrite(10, LOW);

}

void loop() {

  
  

  float x = (bmp.readPressure()); // donne une pression brute sans correction d'altitude

  float pression = (( x + cor) / 100); // MOI JE DOIS RAJOUTER 6200 POUR RAMENER MA PRESSION AU NIVEAU DE LA MER car je suis à 550 m d'altitude


  lcd.clear();

  lcd.setCursor(1, 0);

  lcd.print("Mesure de reference");

  lcd.setCursor(1, 1);

  lcd.print( "Pression " );

  lcd.print( pression );

  lcd.print(" hP");

  lcd.setCursor(4, 2);

  lcd.print( "Temperature" );

  lcd.setCursor(4, 3);


  lcd.print(bmp.readTemperature());

  lcd.print(" degres");

  delay(4000);

  const float ref = (pression); // cette valeur ref me servira ensuite pour test d'alarme

  Serial.print("Messure de reference = ");

  Serial.print(ref);

  Serial.println(" Hectopascal");

  Serial.println("Début des 60 mesures");

  for (byte b = 0; b <= 29; b++) // boucle de 30 mesures pour ensuite test alarme à issue des 30 mesures    1 mesure par minute

  {

    delay(delayMS);

    float z = (bmp.readPressure());

    float mesure = (( z + cor ) / 100 );

    Serial.print(" Pression Atmospherique: ");

    Serial.print(mesure);

    Serial.print(" Hectopascal     ");

    Serial.print(" Température Interieure: ");

    Serial.print(bmp.readTemperature());

    Serial.println(" °C");
    
    Serial.print("Humidité: ");
    
    Serial.print(dht.readHumidity());
    
    Serial.print("%    ");

    Serial.print(" Temperature Exterieure: ");
    
    Serial.print(dht.readTemperature());

    Serial.println(" *C");
     

    lcd.clear();

    lcd.setCursor(1, 0);

    lcd.print("Pression ");

    lcd.setCursor(10, 0);

    lcd.print( mesure);

    lcd.print(" hP");

    lcd.setCursor(1, 1);

    lcd.print("Temp INT ");

    lcd.setCursor(11, 1);

    lcd.print(bmp.readTemperature());
    
    lcd.print(" ");
    
    lcd.print((char)223);
    
    lcd.print("C");

    lcd.setCursor(1, 2);

    lcd.print("Temp EXT ");
    
    lcd.setCursor(11, 2);

    lcd.print(dht.readTemperature());

    lcd.print(" ");
    
    lcd.print((char)223);
    
    lcd.print("C");

    lcd.setCursor(1, 3);

    lcd.print("Humidite  ");

    lcd.print(dht.readHumidity());
    
    lcd.print(" ");
    
    lcd.print((char)37);
    


    delay(4000); // pour avoir une mesure de pression par minute

  }

  float z = (bmp.readPressure());

  float mesure = (( z + cor ) / 100 ); // mesure de pression qui va etre comparee à celle de reference pour test alarme

  Serial.print("reference etait ");

  Serial.println(ref);

  Serial.print("mesure pour test alerte  ");

  Serial.println(mesure );


  if ( mesure > ref ) // pour piloter leds en fonction tendance pression à la hausse ou baisse

  {

    digitalWrite(7, LOW);

    digitalWrite(6, HIGH);

  }

  else

  {

    digitalWrite(7, HIGH);

    digitalWrite(6, LOW);

  }


  if (( mesure < 1010 ) && ( ref - mesure ) >= 2 ) // 2 tests pour activer alarme tempete  on peut changer les 2 valeurs de seuil d'alarme que j'ai fixe inférieur a 1010 en pression et a au moins plus 2 en variation en 30mn  *** si alarme sonne trop facilement mettre 3 ou 4

  {


    for (byte a = 0; a <= 20; a++) // boucle de sonnerie buzzer

    {

      digitalWrite(10, HIGH);

      digitalWrite(7, HIGH);

      delay(500);

      digitalWrite(10, LOW);

      digitalWrite(7, LOW);

      delay(500);

    }

    digitalWrite(6, HIGH);

  }

}
