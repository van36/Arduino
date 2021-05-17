
/**************************************************************
****  changement d'utilisateur : "6666 C2:06********."

****
****  INPUT
****  -----
****
****  Gestion de la batterie :
****    - exctinction du module gsm pendant la durÃƒÂ©e de la coupure de courant
****
****  -------------------------------------------------   *****
****************************************************************/

//
//   *** Configurer le Moniteur Serie sur Carriage Return, 19200 bauds ***
//
//

#include <EEPROM.h>

/*#include <SoftwareSerial.h>
  SoftwareSerial Serial1(10, 11);*/
  
//**************************   CONFIG MESSAGES *********************************************************
String str_site_irrigation = "Domarville";
String message_demarrage_carte = "Demarrage GSM du puit de " + str_site_irrigation;
String message_arret_irrigation = "Arret irrigation ";    //de "+str_site_irrigation;
String message_marche_irrigation = "Mise en marche";      // de l irrigation de "+str_site_irrigation;
String message_pression = "en depression";                //message par rapport input2 ici depression


String msg = String("");
String num_tel = String("");
String smsSimPosition = String("");
String dateTimeSms = String("");
String date_heure = String("");
String code_param = "6311";
#define tempo_relay 1000;

int SmsContentFlag = 0;

/****************************************************************************************************
*******     numeros de telephone valide et code_pin   *************
******************************************************/
#define adresse_eeprom_user 10

String  user1 = "0000000000";
String  user2 = "0000000000";
String  user3 = "0000000000";
String  user4 = "0000000000";


//----------------------------------
// relais numeros irrigation
//----------------------------------
#define relais_output1 24  // relai pour simuler le bouton arret (bp tournant 2positions tournant) cablÃƒÂ© en serie
#define relais_output2 25  // relai pour simuler le bouton rearmement (bp poussoir) cablÃƒÂ© en parallele

//-------------------------
// inputs irrigation
//-------------------------

#define input_in1 40    // ampoule marche
#define input_in3 43    // ampoule incident

byte bl_input_in1_last = 1;
byte bl_input_in3_last = 1;

//------------------------------------------------------------------------------------------------------

void setup()
{
  Serial1.begin(19200);               // the GPRS baud rate
  Serial.begin(19200);                 // the GPRS baud rate


  Serial.println("debut initialisation version v3.3.1 10/07/2020");
  //********************
  // Initialize la PIN
  //*********************

  pinMode(relais_output1, OUTPUT);
  pinMode(relais_output2, OUTPUT);


  //lecture dans l'eeprom pour recupÃƒÂ©rer la dernier valeur du relais bp marche
  int valeur = lecture_eeprom(1); //num case 1 comme output1
  Serial.print(F("valeur : ") );
  Serial.println(valeur );

  digitalWrite(relais_output1, valeur);//ch4  // relais bp marche
  digitalWrite(relais_output2, 1);//ch3 // bp rearmement

  //-----------------------------------------------------------------------------------------
  // config : input
  //----------------

  pinMode (input_in1, INPUT); // Ensemble broche ÃƒÂ  l'entrÃƒÂ©e
  pinMode (input_in3, INPUT); // Ensemble broche ÃƒÂ  l'entrÃƒÂ©e

  //activation de la resistance interne
  //-----------------------------------
  digitalWrite (input_in1, HIGH); // Mettez une rÃƒÂ©sistance de tirage
  digitalWrite (input_in3, HIGH); // Mettez une rÃƒÂ©sistance de tirage


  bl_input_in1_last = digitalRead(input_in1);
  bl_input_in3_last = digitalRead(input_in3);

  Serial.println ("Lampe marche : " + String(bl_input_in1_last ));
  Serial.println ("Lampe incident : " + String(bl_input_in3_last) );

  //-----------------------
  // Config : UTILISATEURS
  //-----------------------
  delay(1000);

  //*******************************************************************************************************
  // si nouvel arduino enregistrer les users sur eeprom

  //enregistrement_user ();

  //*********************************************************************************************************

  delay(1000);
  user1 = lecture_user_eeprom(10, 10).substring(0, 10);
  user2 = lecture_user_eeprom(20, 10).substring(0, 10);
  user3 = lecture_user_eeprom(30, 10).substring(0, 10);
  user4 = lecture_user_eeprom(40, 10).substring(0, 10);

  Serial.println(F("recap des variable :"));
  Serial.println(user1);
  Serial.println(user2);
  Serial.println(user3);
  Serial.println(user4);
  //-----------------------------
  //  config et test Carte GSM
  //-----------------------------

  Serial.println(F("Demarrage..."));
  delay(1000);

  Serial.println(F("test connection reseau..."));
  //testReseau();
  power_on();
  effacementAllSmsSim();
  configSmsModeText();
  Serial.println(F("fin initialisation"));

  envoie_message_multiple(message_demarrage_carte);
}



//----------------------------------------------------------------------------------------------------------------------

void loop()
{
  char SerialInByte;


  if (Serial.available())
  {
    Serial1.print((unsigned char)Serial.read());
  }
  else  if (Serial1.available())
  {

    char SerialInByte;
    SerialInByte = (unsigned char)Serial1.read();

    Serial.print( SerialInByte );

    // FR: Si le message se termine par un <CR> alors traiter le message
    if ( SerialInByte == 13 ) {
      ProcessGprsMsg();
    }
    if ( SerialInByte == 10 ) { // FR: Ignorer les Line Feed

    }
    else {
      // FR: stocker le caractÃƒÂ¨re dans la mÃƒÂ©moire tampon rÃƒÂ©servÃƒÂ© au message
      msg += String(SerialInByte);
    }
  }
  gestion_info_input();
}
