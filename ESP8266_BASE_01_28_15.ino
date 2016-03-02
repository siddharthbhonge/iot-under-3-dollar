#include <SoftwareSerial.h>

#define ESP8266_rxPin 4
#define ESP8266_txPin 5

//SSID + KEY
const char SSID_ESP[] = "YOUR SSID";
const char SSID_KEY[] = "YOUR PASSWORD";

// URLs
const char URL_youtube[] = "GET https://api.thingspeak.com/apps/thinghttp/send_request?api_key=OAYZRRSHYRYP4WZL HTTP/1.0\r\n\r\n";
const char URL_current_temp[] = "GET https://api.thingspeak.com/apps/thinghttp/send_request?api_key=3M27CDHR12NN52V1 HTTP/1.0\r\n\r\n";

//MODES
const char CWMODE = '1';//CWMODE 1=STATION, 2=APMODE, 3=BOTH
const char CIPMUX = '1';//CWMODE 0=Single Connection, 1=Multiple Connections

SoftwareSerial ESP8266(ESP8266_rxPin, ESP8266_txPin);// rx tx

//DEFINE ALL FUNCTIONS HERE
boolean setup_ESP();
boolean read_until_ESP(const char keyword1[], int key_size, int timeout_val, byte mode);
void timeout_start();
boolean timeout_check(int timeout_ms);
void serial_dump_ESP();
boolean connect_ESP();
void get_youtube();
void get_current_temp();
void get_hilo_temp();

//DEFINE ALL GLOBAL VAARIABLES HERE
unsigned long timeout_start_val;
char scratch_data_from_ESP[20];//first byte is the length of bytes
char payload[150];
byte payload_size=0, counter=0;
char ip_address[16];
char youtube_subs[10];
char youtube_views[13];
char current_temp[5];
char hi_temp[5];
char lo_temp[5];

//DEFINE KEYWORDS HERE
const char keyword_OK[] = "OK";
const char keyword_Ready[] = "Ready";
const char keyword_no_change[] = "no change";
const char keyword_blank[] = "#&";
const char keyword_ip[] = "192.";
const char keyword_rn[] = "\r\n";
const char keyword_quote[] = "\"";
const char keyword_carrot[] = ">";
const char keyword_sendok[] = "SEND OK";
const char keyword_linkdisc[] = "Unlink";


//keywords for youtube
const char keyword_html_start_b[] = "b>";
const char keyword_html_end_b[] = "</b";

//keywords for current temp
const char keyword_html_start_temp[] = "p\">";
const char keyword_html_end_temp[] = "<s";

void setup(){//        SETUP     START
  //Pin Modes for ESP TX/RX
  pinMode(ESP8266_rxPin, INPUT);
  pinMode(ESP8266_txPin, OUTPUT);
  
  ESP8266.begin(9600);//default baudrate for ESP
  ESP8266.listen();//not needed unless using other software serial instances
  Serial.begin(115200); //for status and debug
  delay(5000);//delay before kicking things off
  
  setup_ESP();//go setup the ESP 
}//                    SETUP     END

void loop(){//         LOOP     START

  get_youtube(); // get youtube views and subs
  delay(15000);//thingspeak needs ~12 secs before next connection
  get_current_temp();//current temperature
  delay(15000);

}//                    LOOP     END
