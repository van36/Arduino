/*****************************************************************************
  ----------------------Analyse et gestion du message---------------------------
  ------------------------------------------------------------------------------
  FR: Execute une action sur base du contenu d'un SMS.
******************************************************************************/

void ProcessSms( String sms ) {

  String temp_param_num = "";
  String temp_sms = "";
  int position_temp = 0;
  Serial.println(F("processSms"));

  num_tel.replace("+33", "0");

  if (sms.indexOf(code_param) >= 0)
  {
    sms.replace(code_param, "");
    sms.replace(" ", "");
    Serial.println (sms.substring(0, 3));
    while (sms.indexOf(".") >= 0)                    // 6666 C1:0676866944.C2:0684135526.C3:0685742695.
    {

      position_temp = sms.indexOf(".");
      temp_sms = sms.substring(0, position_temp + 1);
      Serial.println (sms);
      Serial.println (temp_sms);

      if (temp_sms.indexOf("C1:") >= 0)
      {
        String str_temp = temp_sms.substring((position_temp - 10), position_temp);
        if (test_num(str_temp) == true)
        {
          user1 = str_temp;
          Serial.println ("changement user1 :" + user1);
        }

      }
      else if  (temp_sms.indexOf("C2:") >= 0)
      {
        String str_temp = temp_sms.substring((position_temp - 10), position_temp);
        if (test_num(str_temp) == true)
        {
          user2 = str_temp;
          Serial.println ("changement user2 :" + user2);
        }
      }
      else if  (temp_sms.indexOf("C3:") >= 0)
      {
        String str_temp = temp_sms.substring((position_temp - 10), position_temp);

        if (test_num(str_temp) == true)
        {
          user3 = str_temp;
          Serial.println ("changement user3 :" + user3);
        }
      }
       else if  (temp_sms.indexOf("C4:") >= 0)
      {
        String str_temp = temp_sms.substring((position_temp - 10), position_temp);

        if (test_num(str_temp) == true)
        {
          user4 = str_temp;
          Serial.println ("changement user4 :" + user4);
        }
      }
     
      sms.replace(temp_sms, "");
    }

    Serial.println (F("presentation des utilisateurs avant enregistrement "));
    Serial.println (user1);
    Serial.println (user2);
    Serial.println (user3);
    Serial.println (user4);
    enregistrement_user ();
    rapport ();
  }

  /*------------------------------------------------------------------
    si les bons utilisateurs :
        - enregistrement des utilisateurs
        - allumage & arret de irrigation
        - rapport
        - effacer les sms stockÃ© sur la carte sim
        - envoie de config comme le cgangement d'heure
    -------------------------------------------------------------------- */
  else if ( num_tel == user1 or num_tel == user2 or num_tel == user3 or num_tel == user3 )
  {
    Serial.println("Numero valide");

    if ( (sms.indexOf("Marche") >= 0) or (sms.indexOf("marche") >= 0) or (sms.indexOf("MARCHE") >= 0) or (sms.indexOf("O1On") >= 0) or (sms.indexOf("o1on") >= 0) or (sms.indexOf("O1ON") >= 0))
    {
      Serial.println( "Mise en marche" );
      procedure_mise_en_marche_irrigation();
      return;
    }
    else if ( (sms.indexOf("Arret") >= 0) or (sms.indexOf("arret") >= 0) or (sms.indexOf("Arrêt") >= 0) or (sms.indexOf("arrêt") >= 0) or (sms.indexOf("ARRET") >= 0 ) or (sms.indexOf("O1Off") >= 0) or (sms.indexOf("o1off") >= 0) or (sms.indexOf("O1OFF") >= 0))
    {
      procedure_arret_irrigation();
      return;
    }
    else if ( (sms.indexOf("rapport?") >= 0 ) or (sms.indexOf("Rapport?") >= 0 ) or (sms.indexOf("R?") >= 0 ) or (sms.indexOf("r?") >= 0 ))
    {
      Serial.println(F("creation et envoie du rapport"));
      rapport ();
    }
    else if ( (sms.indexOf("efface sms") >= 0 ) or (sms.indexOf("Efface sms") >= 0 ))
    {
      effacementAllSmsSim();
    }
    //code config :AT+CCLK="14/09/15,08:29:30+01
    else if ( (sms.indexOf("config :") >= 0 ) or (sms.indexOf("Config :") >= 0 ))
    {
      Serial.println(F("entre dans config"));
      sms.replace("config :", "");
      Serial.print(F("config a envoyer :"));
      Serial.println(sms);
      sendATcommand(sms, "OK", 1000);    // sets the SMS mode to text
      //Serial1.println(sms);
      rapport ();
    }
    else envoie_message("le code n est pas valide", num_tel);
  }
}


boolean test_num(String num)
{
  Serial.println("test_num");
  if (num.length() == 10)
  {
    Serial.println("num.length() == 10");
    if (num[0] == '0')
    {
      for (int i = 0; i <= 9 ; i++)
      {
        int a = int(num[i]);
        if (a<48 or a> 57 ) return false;
      }
      return true;
    }
    return false;
  }
  return false;
}

/*****************************
  -----   RAPPORT   ----------
*****************************/
boolean rapport ()
{
  String rapport = str_site_irrigation + " - Utilisateurs : " + user1 + "; " + user2 + "; " + user3 + "; " + user4;
  rapport = rapport + "   etat input marche : " + String(bl_input_in1_last);
  rapport = rapport + ", etat input incidents : " + String( bl_input_in3_last);
  Serial.print( "message du rapport : ");
  Serial.println( rapport);
  envoie_message(rapport, num_tel);
}
