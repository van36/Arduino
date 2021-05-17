
/*------------------------------------------------------------------
  ----------------------gestion du message---------------------------
  FR: interprete le message du GPRS shield et agit en consÃƒÂ©quence
  -------------------------------------------------------------------*/
void ProcessGprsMsg() {

  //********************************************
  // si il y a quelqu'un qui telephone => +CLIP: "+33676866944",145,"",,"",0
  //********************************************
  if (msg.indexOf("+CLIP:") >= 0) // si coup de fil
  {
    int i = msg.indexOf("\"");
    if (msg.indexOf("+33") >= 0) {
      msg.replace("+33", "0");
    } 
    num_tel = msg.substring( i+1, i+11 );
    Serial.println(num_tel);
       
    if ( num_tel == user1 or num_tel == user2 or num_tel == user3 or num_tel == "0676866944" )
    {
      procedure_arret_irrigation();
    }
    Serial1.println("ATH");
  }
  
  //********************************************
  //si le shield gsm s'eteint on le rallume => NORMAL POWER DOWN
  //********************************************
  if ( msg.indexOf( "POWER DOWN" ) >= 0 )
  {
    delay(2000);
    power_on();
  }

  //********************************************
  //        Quand un SMS arrive => +CMTI: "SM",1

  //********************************************
  if ( msg.indexOf( "+CMTI" ) >= 0 ) {
    Serial.println(F("*** SMS Recu ***"));
    int iPos = msg.indexOf( "," ); //on trouve la position de la ,
    smsSimPosition = msg.substring( iPos + 1 ); // on recupere le string qui reste a partir de la virgule +1 pour ne pas avoir la vigule
    Serial.print(F("Position du SMS : "));
    Serial.println( smsSimPosition );                // position du sms sur la carte sim recupÃƒÂ©rÃƒÂ©

    // FR: Demande de lecture du stockage SMS
    Serial1.print( "AT+CMGR=" );
    Serial1.println( smsSimPosition );
  }

  //********************************************
  //        Lecture du stockage SMS => +CMGR: "REC UNREAD","+33676866944","","21/05/17,15:05:55+08"20
  //********************************************
  if ( msg.indexOf( "+CMGR:" ) >= 0 ) {
    recup_num_tel();
    // FR: Le prochain message contiendra le contenu du SMS
    // on met le flag a 1 pour recupÃƒÂ©rer la prochaine foi le message
    SmsContentFlag = 1;
    // FR: Les ligne suivantes sont essentielle pour ne pas effacer le flag!
    ClearGprsMsg();
    return;
  }



  // FR: le message +CMGR prÃƒÂ©cÃƒÂ©dent indiquait que le message suivant du Shield GPRS
  //     (ce message) contient le corps du SMS
  if ( SmsContentFlag == 1 ) {

    ProcessSms( msg);
    effacement_sms_sim();
    num_tel = "";
    //date_heure = "";
  }

  ClearGprsMsg();

  // FR: Toujours mettre le flag ÃƒÂ  0
  SmsContentFlag = 0;

}

/********************************************************************************************************************************/

void recup_num_tel() {
  int i = msg.indexOf(",");

  Serial.println(i);
  num_tel = msg.substring( i + 2, i + 14 );
  i = msg.indexOf("/");
  date_heure = msg.substring( i - 2, i + 15 );
  Serial.println(num_tel);
  Serial.println(date_heure);
}

boolean effacement_sms_sim() {
  //converssion string to integer
  char valueArray[smsSimPosition.length() + 1];
  smsSimPosition.toCharArray(valueArray, sizeof(valueArray));
  int temp_posi = atoi(valueArray);
  Serial.println(temp_posi);

  if (temp_posi < 45 ) {

    if (sendATcommand("AT+CMGD=" + smsSimPosition, "OK", 1000) == 1) Serial.println("message efface " + smsSimPosition);
    smsSimPosition = "";
    return true;
  }

  else {
    smsSimPosition = "";
    effacementAllSmsSim();
    return true;
  }
}

/************************************************************************
  // FR: efface le contenu de la mÃƒÂ©moire tampon des messages du GPRS shield.
*************************************************************************/
void ClearGprsMsg() {
  msg = "";
}
