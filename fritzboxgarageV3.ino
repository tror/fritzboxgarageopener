     /*
     Fritzbox Garagentor Öffner mit Arduino Ethernetschield 
    
     Schaltung:
     *Denken sie bei dieser Variante an den Keramik Kondensator 47uf zwischen gnd und reset oben auf dem Ethernetschield schield.
     *Der Relaispin ist hier auf die 7 geschaltet
     
     created 14 Sep 2012
     modified 22 Jan 2013
     by Johannes Osterberg
     */
      #include <SPI.h>
      #include <Ethernet.h>
      
      byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
      IPAddress fritzbox(192,168,178,1); //IP der Fritzbox
      IPAddress ip(192,168,178,55);// IP des Arduino
      
      int relay = 7; //Relaispin
      
      void(* resetFunc) (void) = 0; //Reset Funktion wird Initialisiert.
      
      String callmonitor; // Variable für die Werte des Callmonitors
      char c;
      
      EthernetClient client;
      
      void setup() {
        Serial.begin(9600);
        pinMode(relay, OUTPUT);
        digitalWrite(relay, LOW);
        delay(5000);
        Ethernet.begin(mac, ip);
        delay(5000);
        if (client.connect(fritzbox, 1012)) {
           Serial.println("verbunden");
        }
          else {
           Serial.println("nicht verbunden");
          delay(1000);
          resetFunc(); 
          }
      }
      
      int oeffner(String nummer){
             // Solang das Ethernetschield daten empfängt werden sie in den String callmonitor geschrieben
             while(client.available()) {
                  Serial.println("empfängt");
                  c = client.read();
                  //c wird zu String callmonitor zusammengefügt
                  callmonitor.concat(c);
                  // Sting callmonitor wird von Leerzeichen befreit
                  callmonitor.trim();
              }
              
            Serial.println(callmonitor);
            delay(1000);
      
           // Wenn der String callmonitor die zeichenfolge von nummer enthält = true.
           if (callmonitor.indexOf(nummer) >=0) {
             callmonitor = "";
             Serial.println("öffnet");
             digitalWrite(relay, HIGH);
             //3 Sekunden Schaltet das Relais auf High
             delay(3000);
             digitalWrite(relay, LOW);
             delay(5000);
             client.stop();
             resetFunc(); 
              }
           else{
              digitalWrite(relay, LOW);
               }      
      }
      
      
      void loop()
      { 

     
           //"01234567890;123456;" Die Nummern Kombination
           //oeffner("GenutzteNummer;AngerufeneNummer"); Nummer 1
           oeffner("01727321856;510010");
           oeffner("01727533275;510010");
        
       if (!client.connected()) {
             Serial.println("restart");
             delay(1000);
             client.stop();
             delay(3000);
             resetFunc();
        }
        if (millis() > 600000){
            client.stop();
            delay(3000);
            resetFunc();
        }
      }
      


