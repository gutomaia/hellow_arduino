#ifdef MOCK

#else
  #include <SPI.h>
  #include <Ethernet.h>
#endif

#include <Stdio.h>
#include "md5.c"

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(10,0,0,68);

char username[] = "hellowgmn@hotmail.com";
//password is used on tweener authentication!

EthernetClient client;
EthernetClient switchboard;

char input[256];
char output[256];

char income[] = "<<<";
char outcome[] = ">>>";

int trid = 1;

void execute_cmd(char cmd[]){
  Serial.print(outcome);
  Serial.print(cmd);
  client.print(cmd);
  trid++;
}

void ver(){
  sprintf(output, "VER %d MSNP8 CVR0\r\n", trid);
  execute_cmd(output);
}

void connect(){
  trid = 1;
  Serial.println("connecting...");
  if (client.connect("messenger.hotmail.com", 1863)) {
    Serial.println("connected");
    ver();
  } else {
    Serial.println("connection failed");
  }
}

void setup()
{
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  delay(1000);
  connect();
}



int get_address(String host, byte ip_port[]){
  char buf[4];
  int cutter = host.indexOf(':');
  String ip = host.substring(0,cutter);
  String port = host.substring(cutter+1);
  cutter = host.indexOf('.');
  host.substring(0,cutter).toCharArray(buf,10);

  ip_port[0] = atoi(buf);

  host = host.substring(cutter+1);
  cutter = host.indexOf('.');
  host.substring(0,cutter).toCharArray(buf,10);
  ip_port[1] = atoi(buf);

  host = host.substring(cutter+1);
  cutter = host.indexOf('.');
  host.substring(0,cutter).toCharArray(buf,10);
  ip_port[2] = atoi(buf);

  host = host.substring(cutter+1);
  cutter = host.indexOf('.');
  host.substring(0,cutter).toCharArray(buf,10);
  ip_port[3] = atoi(buf);
  
  port.toCharArray(buf,10);
  int iport = atoi(buf);
  
  Serial.println(ip_port[0]);
  Serial.println(ip_port[1]);
  Serial.println(ip_port[2]);
  Serial.println(ip_port[3]);
  Serial.println(iport);
  return iport;
}

void redirect(String host){
  byte _ip[5];
  int port = get_address(host, _ip);
  client.stop();
  IPAddress redirect(_ip[0],_ip[1],_ip[2],_ip[3]);
  if (client.connect(redirect, port)) {
    Serial.println("redirected");
    ver();
  } else {
    Serial.println("redirect failed");
  }
}

void cvr(){
  sprintf(output, "CVR %d 0x0409 win 4.10 i386 MSNMSGR 6.0.0602 MSMSGS %s\r\n", trid, username);
  execute_cmd(output);
}

void usr(){
  sprintf(output, "USR %d TWN I %s\r\n", trid, username);
  execute_cmd(output);
}

void syn(){
  sprintf(output,"SYN 1 0\r\n");
  execute_cmd(output);
}

void chg(){
  sprintf(output,"CHG %d NLN 0\r\n", trid);
  execute_cmd(output);
}

String token(){
  String stmnt = input;
  String t;
  int cutter = stmnt.indexOf(' ');
  if (cutter != -1){
    t = stmnt.substring(0, cutter);
    stmnt.substring(cutter+1).toCharArray(input, 256);
  } else {
    t = stmnt;
  }
  return t;
}

void rng(){
  delay(100);
  token();
  String session_id = token();
  Serial.println(session_id);
  String host = token();
  Serial.println(host);
  byte _ip[5];
  int port = get_address(host, _ip);
  delay(30);
  Serial.println(_ip[0]);
  Serial.println(_ip[1]);
  Serial.println(_ip[2]);
  Serial.println(_ip[3]);
  Serial.println(port);
  token();
  String auth = token();
  Serial.println(auth);
  String user = token();
  Serial.println(user);
  /*
  IPAddress chat_ip(ip_port[0],ip_port[1],ip_port[2],ip_port[3]);
  if (switchboard.connect(chat_ip, ip_port[4])) {
    Serial.print("switchboard with ");
    Serial.println(user);
    sprintf(output, "ANS 1 %s %s %s\r\n", username, auth, session_id);
    switchboard.print(output);
    switchboard.print("MSG 3 N 127\r\n");
    switchboard.print("MIME-Version: 1.0\r\n");
    switchboard.print("Content-Type: text/plain; charset=UTF-8\r\n");
    switchboard.print("X-MMS-IM-Format: FN=MS%20Sans%20Serif; EF=; CO=0; CS=0; PF=0\r\n");
    switchboard.print("\r\n");
    switchboard.print("Hi.");
  } else {
    Serial.println("redirect failed");
  }
  */
  
   //DO ANS
  //payload
}




void chl(String challenge){
  //2513224641500947162124220509Q1P7W2E4J9R8connecting
  //unsigned char c[challenge.length()-2 + MSN_CODE.length()];
  Serial.print("-");
  Serial.print(challenge);
  Serial.print("-");
  unsigned char c[64];
  int i = 0;
  Serial.println();
  for (i=0; i < (challenge.length()-2); i++){
    c[i] = challenge.charAt(i);
    Serial.print(challenge.charAt(i));
  }
  String MSN_CODE = "Q1P7W2E4J9R8U3S5";
  for (int j=0; j< MSN_CODE.length(); j++, i++){
      c[i] = MSN_CODE.charAt(j);
      Serial.print(MSN_CODE.charAt(j));
  }
  Serial.println();
  Serial.println("done to toChar");
  MD5(c, challenge.length()-2 + MSN_CODE.length());
  Serial.println("done to md5");
  sprintf(output,"QRY %d msmsgs@msnmsgr.com 32\r\n", trid);
  execute_cmd(output);
  String query = "";
  int x = 0;
  char *tmp;
  Serial.println("about to start");
  while(x <= 15) {
    sprintf(tmp, "%.2X%", MD5Digest[x]);
    Serial.print(tmp);
    query.concat(tmp);
    Serial.print(tmp);
    //PrintHex8((unsigned char *)&MD5Digest[x], 1); //The Hash is stored in a static variable in MD5.c called MD5Digest, of type unsigned char[16]
    x++;
  }
  Serial.println();
  query.toLowerCase();
  Serial.println(query);


  /*Serial.println("droga droga droga");
  
  query.toLowerCase();

  Serial.println();
  Serial.println(query);
  client.print(query);
 // 		$this->send("QRY 10 msmsgs@msnmsgr.com 32\r\nd0c1178c689350104350d99f8c36ed9c");
 */
}


/*Taken from http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1279500031 */
/*
void PrintHex8(uint8_t *data, uint8_t length) // prints 8-bit data in hex with leading zeroes
{
        String aux;
	char *tmp;
	  for (int i=0; i<length; i++) {
	    sprintf(tmp, "%.2X",data[i]);
            aux = tmp;
            aux.toLowerCase();
	    Serial.print(aux);
            client.print(aux);
	  }
}
*/

void twenner(){
  Serial.println("twenner");
  IPAddress i(10,0,0,10);
  EthernetClient twn;
  if (twn.connect(i,80)){
    //String i = input;
    //Serial.println("ok");
    //Serial.println(i);
    //twn.print("PUT /~gutomaia/test.php HTTP/1.1\r\n");
    twn.print("GET /~gutomaia/test.php HTTP/1.1\r\n");
    twn.print("Host: guto.net\r\n");
    //twn.print("Content-Length: ");
    //twn.print(i.length());
    twn.print("\r\n");
    //twn.print("\r\n");
    //twn.print(i);
    unsigned long t = millis();
    while((!twn.available()) && ((millis() - t) < 15L * 1000L));
    if (twn.available() > 0){
      if (twn.find("\r\n\r\n")){
         sprintf(output, "USR %d TWN S ", trid);
         execute_cmd(output);
          while (twn.available() > 0){
            char c = twn.read();
            Serial.print(c);
            client.print(c);
          }
          Serial.print("\r\n");
          client.print("\r\n");          
        }
    }
    twn.stop();
  }
}

void authenticate(){
  Serial.println("authenticate");
  twenner();
}

void readline(){
  char c = client.read();
  input[0] = c;
  int i = 1;
  while (c != '\n'){
    c = client.read();
    if (i < 255){
      input[i] = c;
      i++;
    }
  }
  input[i]='\0';
}

String payload(){
}


void execute(){
  String stmnt = input;
  String cmd = stmnt.substring(0,3);
  if (cmd == "VER"){
    cvr();
  } else if (cmd == "CVR"){
    usr();
  } else if (cmd == "XFR"){
    token();
    token();
    token();
    String host = token();
    redirect(host);
  } else if (cmd == "USR"){
    if (input[6] == 'T' && input[7] == 'W' && input[8] == 'N'){
      authenticate();
    } else if (input[6] == 'O' && input[7] == 'K'){
      syn();
      chg();
    }
  } else if (cmd == "CHL"){
    //challenger
    token();
    token();
    String c = token();
    chl(c);
  } else if (cmd == "RNG"){
    //ringing
    //<<<RNG 2119415875 65.54.48.161:1863 CKI 11123870.121643 gutomneto@hotmail.com Gustavo
    rng();

  } else if (cmd == "OUT"){
    //disconnect
  } else if (cmd == "911"){
  }
}

void loop()
{
  int msn_state = 1;
  while (client.connected()){
    if (client.available() > 0){
      readline();
      Serial.print(income);
      Serial.print(input);
      execute();
    }
  }
  /*
  while(switchboard.connected()){
    if(switchboard.available()>){
    }
  }
  */
  if (!client.connected()){
    Serial.println("disconnecting");
    client.stop();
    delay(10000);
    connect();
  }
}
