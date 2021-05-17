//****************************************************************
//       FONCTION ENVOIE MESSAGE
//****************************************************************
void envoie_message(String message, String num_tel)
{
  Serial.println(F("envoie_message"));
  Serial.println(message);
  String msgTemp;// = demande_heure ();
  delay(1000);

 msgTemp = msgTemp + " " + message;
  Serial.println("message avant envoi : " + msgTemp);
  if (num_tel != "") envoie_message_final(msgTemp, num_tel);
}

void envoie_message_multiple(String message)
{
  String msgTemp;// = demande_heure ();
  delay(1000);
  msgTemp = msgTemp + " " + message;
  if (user1 != "") envoie_message_final(msgTemp, user1);
  if (user2 != "") envoie_message_final(msgTemp, user2);
  if (user3 != "") envoie_message_final(msgTemp, user3);
  if (user4 != "") envoie_message_final(msgTemp, user4);

}

/**************************************************************************************************/
boolean envoie_message_final(String message, String num_tel)
{

  int notSend = 0;

  if  (num_tel != "0000000000")
  {
    while ( notSend < 5)
    {
      Serial1.print("AT+CMGS=\"");
      Serial1.print(num_tel);
      Serial1.print("\"\r");
      delay(500);
      Serial1.print(message);     // Le texte du message
      delay(500);
      Serial1.write(26);  // Equivalent du Ctrl+Z (fin de texte du SMS)
      
      bool answer = sendATcommand("", "OK", 20000);
      if (answer == true)
      {
        Serial.println(F("message envoye"));
        return true;
      }
      else
      {
        Serial.print(F("error "));
      }
      notSend++;
    }
    power_on();
  }
}


/**************************************
  RECUPERATION DE LA DATE ET L HEURE
        Commande : "AT+CCLK?"
***************************************/
String demande_heure ()
{
  char SerialInByteBis;
  long chrono = millis();
  String  msg_recu = "";
  String msg3 = "";
  boolean trame_valide = false;
  Serial.println("demande d'heure");

  //demande de l'heure au GSM : reponse : AT+CCLK="14/09/15,08:29:30+01"
  Serial1.println("AT+CCLK?");

  // delay(200);

  while ( millis() < (chrono +  2000)) {

    if (Serial1.available())
    {
      //flag_power == 0;

      SerialInByteBis = (unsigned char)Serial1.read();
      Serial.print( SerialInByteBis );

      if ( SerialInByteBis == 13 ) // = retour a la ligne
      {
        if (msg_recu.indexOf("+CCLK:") >= 0)
        {
          msg3 = msg_recu;
          //reponse : AT+CCLK="14/09/15,08:29:30+01"
          Serial.println("");
          Serial.println( "demande heure avant traitement : " + msg3);
          int debut_date_heure = msg3.indexOf("+CCLK:");

          msg3 = msg3.substring(debut_date_heure + 8, debut_date_heure + 25); //14/09/15,08:29:30
          Serial.println (msg3);    //14/09/15,08:29:30
          if (msg3.charAt(14) == ':')
          {
            trame_valide = true;
            Serial.println ("Trame date valide : " + msg3);
          }
          else Serial.println (F("Trame date non valide "));
          //return msg3;
          msg_recu = "";
        }
        if (msg_recu.indexOf("OK") >= 0 && trame_valide == true)
        {
          Serial.println ("");
          String msg_temp = msg3.substring(0, 2);
          msg3[0] = msg3[6];
          msg3[1] = msg3[7];

          msg3[6] = msg_temp[0];
          msg3[7] = msg_temp[1];
          return msg3;
        }
        msg_recu = "";
      }
      if ( SerialInByteBis == 10 )
      {
        // FR: Ignorer les Line Feed
      }
      else
      {
        msg_recu += String(SerialInByteBis);
      }
    }
  }
  if (msg3 != "" && trame_valide == true) return msg3;
}

boolean effacementAllSmsSim() {
  sendATcommand("AT+CMGD=1,4" , "OK", 1000);
  Serial.println("All message efface " );
  return true;
}

bool configSmsModeText() {
  Serial.println(F("Configuration SMS mode texte"));
  sendATcommand("AT+CMGF=1", "OK", 1000);    // sets the SMS mode to text
}
