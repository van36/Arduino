/***************************************
     inputs irrigation
****************************************

#define input_in1 40     ampoule marche
#define input_in3 43     ampoule incident
****************************************/

void gestion_info_input()
{
  long temp_millis = 0;
  byte bl_input_in1 = 0;    // ampoule marche
  byte bl_input_in3 = 0;    // ampoule pression
  bl_input_in1 = digitalRead(input_in1);
  bl_input_in3 = digitalRead(input_in3);
  
  //---------------------------------------------
  //         AMPOULE MARCHE
  //----------------------------------------------
  if ((bl_input_in1_last != bl_input_in1) )
  {
    temp_millis = millis();
    do
    {
      if (bl_input_in1_last == bl_input_in1) return;
    } while (millis() < (temp_millis + 2000));
    if ((bl_input_in1 == 1))   // Si MARCHE a 0 car braché sur un contacte nc : l'ampoule marche est allumÃƒÂ©e et lampe incident eteind
    {
      Serial.println(message_marche_irrigation);
      envoie_message_multiple(message_marche_irrigation);
    }
    else if ((bl_input_in1==0) && (bl_input_in3==0))    // si lampoule marche est eteinte et arret non "sous pression"
    {
      Serial.println(message_arret_irrigation);
      envoie_message_multiple(message_arret_irrigation);
    }
    else if ((bl_input_in1 == 0) && (bl_input_in3 == 1))    // si lampoule marche est eteinte et arret "sous pression"
    {
      Serial.println(message_arret_irrigation);
      envoie_message_multiple(message_arret_irrigation+ " " + message_pression );
    }
    bl_input_in1_last = bl_input_in1;
  }
}
