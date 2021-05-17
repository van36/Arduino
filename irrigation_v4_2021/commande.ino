void power_on() {

  uint8_t answer = false;

  // checks if the module is started
  while (answer == false) {
    answer = sendATcommand("AT", "OK", 2000);
    if (answer == false)
    {
      Serial.println ("redemarrage");
      pinMode(9, OUTPUT);
      digitalWrite(9, LOW);
      delay(1000);
      digitalWrite(9, HIGH);
      delay(2000);
      digitalWrite(9, LOW);
      delay(4000);
    }
  }
}

/******************************************************************************************************************************************/

boolean sendATcommand(String ATcommand, String expected_answer, unsigned int timeout) {
  Serial.print(F("ATcommande :"));
  Serial.println(ATcommand);

  uint8_t x = 0,  answer = 0;
  char reponse_voulu[20];
  expected_answer.toCharArray(reponse_voulu, 20);
  char response[100];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialice the string

  delay(100);

  while ( Serial1.available() > 0) Serial1.read();   // Clean the input buffer

  Serial1.println(ATcommand);    // Send the AT command

  x = 0;
  previous = millis();

  // this loop waits for the answer
  do {
    // if there are data in the UART input buffer, reads it and checks for the asnwer
    if (Serial1.available() != 0) {
      response[x] = Serial1.read();
      Serial.print( response[x]);
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(response, reponse_voulu) != NULL)
      {
        
        answer = 1;
      }
    }
    // Waits for the asnwer with time out
  } while ((answer == 0) && ((millis() - previous) < timeout));

  if (answer == 1){
    return true;
  } else return false;

}
