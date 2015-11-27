String response = "";
String request = "";

//#define COMMAND_START_TCP_CONNECTION "AT+CIPSTART=\"TCP\",\"10.0.1.5\",80"
//#define COMMAND_START_TCP_CONNECTION "AT+CIPSTART=\"TCP\",\"10.0.1.234\",8080"
#define COMMAND_START_TCP_CONNECTION "AT+CIPSTART=\"TCP\",\"192.168.43.1\",8080"
#define COMMAND_SEND_MESSAGE_SIZE "AT+CIPSENDEX="
#define AMP "&"
#define CRLF "\r\n"
#define SCORE_1 "score_1="
#define SCORE_2 "score_2="
#define POST_PREFIX "POST /index.php HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: "
#define TIMEOUT 20000
#define KEEP_ALIVE "Connection: Keep-Alive"

unsigned long timeout = 0;
int testCount = 2;

bool isSent = false;
bool isError = false;
void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200); // your esp's baud rate might be different
  Serial.write("---\n");
}
 
void loop()
{
  if(Serial1.available()) // check if the esp is sending a message 
  {
    while(Serial1.available())
    {
      // The esp has data so display its output to the serial window 
      response = Serial1.readString(); // read the next character.
      Serial.println(response);
    }  
  }

//      if(response.substring(response.length()-9, response.length()-2) == "SEND OK") {
//        Serial.println("need to close connection");
//      }


      
      if(!isSent || (millis() - timeout) > TIMEOUT){
//        Serial.println(response.lastIndexOf("CLOSED"));
//      if(response.lastIndexOf("CLOSED") == -1){
//      if(!isSent) {
//        createPostRequest();
//      } else {
//        sendNextRequest();
//      }
//          if(response.lastIndexOf("ALREADY CONNECT") != -1) {
//            sendNextRequest();
////              Serial.println("already connected");
//          } //else {
//            createPostRequest();
//          }
          createPostRequest();
        
        isSent = true;
        Serial.print("timeout before = ");
        Serial.println(timeout);
        timeout = millis();
        Serial.print("timeout after = ");
        Serial.println(timeout);
      }
      
      if(response.substring(response.length()-2, response.length()-1) == ">") {
//        Serial.println("!!!");
          sendPostRequest();
          response = "";
      }

//      if(response.lastIndexOf("ERROR") != -1) {
////          sendPostRequest();
//        isError = false;
//          Serial.println(response.lastIndexOf("ERROR"));
////          Serial1.println("AT+RST");
//          Serial1.flush();
//          
//          response = "";
//      } else {
//        isError = true;
//      }
      
  
  if(Serial.available())
  {
    // the following delay is required because otherwise the arduino will read the first letter of the command but not the rest
    // In other words without the delay if you use AT+RST, for example, the Arduino will read the letter A send it, then read the rest and send it
    // but we want to send everything at the same time.
    delay(100);
    
    String command="";
    while(Serial.available()) // read the command character by character
    {
        // read one character
      command+=(char)Serial.read();
    }
    Serial.println(command);
    Serial.println(command.length());
    Serial1.println(command); // send the read character to the esp8266
  }
}

void createPostRequest() {
  Serial1.flush();
  Serial1.println(COMMAND_START_TCP_CONNECTION);

  //log
//        Serial.println(COMMAND_START_TCP_CONNECTION);

   delay(300);
   Serial1.print(COMMAND_SEND_MESSAGE_SIZE);
   request = getPostRequestString();
   Serial1.println(request.length());
}

void sendNextRequest() {
  delay(300);
   Serial1.print(COMMAND_SEND_MESSAGE_SIZE);
   request = getPostRequestString();
   Serial1.println(request.length());
}

void sendPostRequest() {
  Serial1.println(request);
  Serial1.flush();
}

void testPostRequest() {
  Serial.println(getPostRequestString());
}

String getPostRequestString(){
  String body = SCORE_1;
  body += String(testCount);
  body += AMP;
  body += SCORE_2;
  body += String(testCount++);

  String postRequest = POST_PREFIX;
  postRequest += String(body.length());
//  postRequest += CRLF;
//  postRequest += KEEP_ALIVE;
  postRequest += CRLF;
  postRequest += CRLF;
  postRequest += body;
  return postRequest;
}

