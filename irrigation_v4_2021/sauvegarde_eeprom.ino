
//---------------------------------
//    LECTURE / ECRITURE 1 float
//---------------------------------


void EEwriteFloat(int addr, float f) {
    unsigned char *buf = (unsigned char*)(&f);
    for ( int i = 0 ; i < sizeof(f) ; i++ ) {
        EEPROM.write(addr+i, buf[i]);
    }
}
 
float EEreadFloat(int addr) {
    float f;
    unsigned char *buf = (unsigned char*)(&f);
    for ( int i = 0 ; i < sizeof(f) ; i++ ) {
         buf[i] = EEPROM.read(addr+i);
    }
    return f;
}

//---------------------------------
//    LECTURE / ECRITURE 1 valeur
//---------------------------------

int lecture_eeprom(int num_case )
{
  int valeur = EEPROM.read(num_case);
  return valeur;
}



boolean enregistrement_eeprom(int num_case, int valeur)
{
  EEPROM.write(num_case, valeur);
  delay(10);
}



//------------------------------------------------
//    LECTURE / ECRITURE pour les utilisateurs
//------------------------------------------------

String lecture_user_eeprom(int debut, int taille )
{
  char char_temp2[taille + 1];  //variable de recuperation de l'eeprom

  for (int i = debut; i < (debut + taille) ; i++)
  {
    char_temp2[i - debut] = EEPROM.read(i);
    delay(100);
  }
  //Serial.println ("char_temp2 = " + String(char_temp2));
  // String temp_str = char_temp2 ;
  
  return String(char_temp2);
}

boolean enregistrement_user ()
{
  Serial.println(F("entrer dans enregistrement eeprom"));
  char char_temp[60];
 
  String str_recup;
  str_recup =   user1 +  user2 + user3 + user4 ;
  str_recup.toCharArray(char_temp, str_recup.length() + 1); //string en char

  Serial.println(F("str_recup dans enregistrement eeprom :"));
  Serial.println(str_recup);
  Serial.println(char_temp);

  Serial.println("str_recup.length() :" + String(str_recup.length()));



  for (int i = adresse_eeprom_user; i < adresse_eeprom_user + str_recup.length() ; i++)
  {
    EEPROM.write(i, char_temp[i - adresse_eeprom_user]);
    delay(10);

  }

  str_recup = "";
  return true;

}
