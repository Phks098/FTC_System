#include <AsyncEventSource.h>
#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include <AsyncWebSynchronization.h>
#include <ESPAsyncWebSrv.h>
#include <SPIFFSEditor.h>
#include <StringArray.h>
#include <WebAuthentication.h>
#include <WebHandlerImpl.h>
#include <WebResponseImpl.h>

#include <AsyncPrinter.h>
#include <DebugPrintMacros.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncTCPbuffer.h>
#include <SyncClient.h>
#include <async_config.h>
#include <tcp_axtls.h>

#include <ESP8266WiFi.h>

const char* ssid = "dlink-47A4";  // 여기에 WiFi SSID를 입력하세요.
const char* password = "adant62765";  // 여기에 WiFi 비밀번호를 입력하세요.

const int serverPort = 67; // 서버 포트 번호

AsyncWebServer server(serverPort);


//인덕션 1 관련 변수
const int induction1 = D4;
static bool induction1_state = true;// 인덕션1 ON/OFF 상태 on = true, off = false
static bool induction1_fireTool_state = true;// 인덕션1 조리기구 유무 상태
static int id1Timer = -1;


// 인덕션 2 관련 변수
const int induction2 = D7;
static bool induction2_state = true;// 인덕션1 ON/OFF 상태 on = true, off = false
static bool induction2_fireTool_state = true;// 인덕션1 조리기구 유무 상태
static int id2Timer = -1;



//인덕션1 관련 Request
//인덕션1 ON/OFF 변경
void Induction1(){
  server.on("/induction1", HTTP_GET, [](AsyncWebServerRequest *request){

    Serial.println("induction1 요청");
    induction1_state = !induction1_state;
    request->send(200, "text/plain", String(induction1_state));


  });
}

//인덕션1 ON/OFF 상태값 전달
void Induction1_state_turn(){
  server.on("/get1_state", HTTP_GET, [](AsyncWebServerRequest *request){
    
    Serial.println("인덕션1 상태 값 요청");
    request->send(200, "text/plain", String(induction1_state));

  });
}

//인덕션_1 타이머 셋팅
void Induction1_setTimer(){
  server.on("/ID1_SetTimer",HTTP_GET,[](AsyncWebServerRequest *request){
    induction1_state = 0;
    
    //timer의 매개 변수 값을 가지고 인덕션 1의 타이머 기능을 진행시킨다.
    if (request->hasParam("timer")) {
    AsyncWebParameter* param = request->getParam("timer");
    String induction1_timer = param->value();
    id1Timer = induction1_timer.toInt();
    Serial.print("인덕션 1 예약 시간: ");Serial.println(id1Timer);
  }
    request->send(200,"text/plain",String("요청 수용되었습니다."));
  });
}


//인덕션 1 카운터 다운 값 요청 처리
void ID1_CounteValue_Request(){
  server.on("/getID1_counter",HTTP_GET,[](AsyncWebServerRequest *request){

    Serial.print("인덕션_1 남은 시간 요청");Serial.println(id1Timer);
    request -> send(200,"text/plain", String(id1Timer));
  });
  
}

void ID1_StopCount(){
  //ID1_Stop
  server.on("/ID1_Stop",HTTP_GET,[](AsyncWebServerRequest *request){
    Serial.print("인덕션_1 예약을 취소했습니다.");
    induction1_state = 1;

    //led1 OFF 코드  
    id1Timer = -1;

    request -> send(200,"text/plain", "ID1_stop");
  });
}

//인덕션 1 타이머 카운트 다운 동작
void CountTimer_ID1(){
  if(id1Timer > -1 && id1Timer !=0){
    id1Timer--;
    delay(1000);
  } 
  if(id1Timer == 0){
    id1Timer = -1;
    Serial.print("induction1_state ="); Serial.println(induction1_state);
    induction1_state = 1;
  }

}



//인덕션 1 조리도구 유무상태를 알려주는 함수
// 초음파 센서를 통해 조리도구 유무를 확인한다.
void Check_ID1FireETool(){
  //만약 초음파 1의 거리가 100 미만이면 true(있음) 아니면 false(없음)
}


//인덕션2 관련 Request 
//인덕션2 ON/OFF 변경
void Induction2(){
  server.on("/induction2", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("induction2 요청");
    induction2_state = !induction2_state;
    request->send(200, "text/plain", String(induction2_state));
  });
}

//인덕션2 ON/OFF 상태값 전달
void Induction2_state_turn(){
  server.on("/get2_state", HTTP_GET, [](AsyncWebServerRequest *request){
    
  Serial.println("인덕션2 상태 값 요청");
  request->send(200, "text/plain", String(induction2_state));

  });
}


//인덕션_2 타이머 셋팅
void Induction2_setTimer(){
  server.on("/ID2_SetTimer",HTTP_GET,[](AsyncWebServerRequest *request){
    induction2_state = 0;
    
    //timer의 매개 변수 값을 가지고 인덕션 1의 타이머 기능을 진행시킨다.
    if (request->hasParam("timer")) {
    AsyncWebParameter* param = request->getParam("timer");
    String induction2_timer = param->value();
    id2Timer = induction2_timer.toInt();
    Serial.print("인덕션 2 예약 시간: ");Serial.println(id2Timer);
  }
    request->send(200,"text/plain",String("요청 수용되었습니다."));
  });
}


//인덕션 2 카운터 다운 값 요청 처리
void ID2_CounteValue_Request(){
  server.on("/getID2_counter",HTTP_GET,[](AsyncWebServerRequest *request){

    Serial.print("인덕션_2 남은 시간 요청");Serial.println(id2Timer);
    request -> send(200,"text/plain", String(id2Timer));
  });
  
}

void ID2_StopCount(){
  //ID1_Stop
  server.on("/ID2_Stop",HTTP_GET,[](AsyncWebServerRequest *request){
    Serial.print("인덕션_2 예약을 취소했습니다.");
    induction2_state = 1;

    //led1 OFF 코드  
    id2Timer = -1;

    request -> send(200,"text/plain", "ID2_stop");
  });
}


//인덕션_2 타이머 카운트 다운 동작
void CountTimer_ID2(){
  if(id2Timer > -1 && id2Timer !=0){
    id2Timer--;
    delay(1000);
  } 
  if(id2Timer == 0){
    id2Timer = -1;
    Serial.print("induction2_state ="); Serial.println(induction2_state);
    induction2_state = 1;
  }

}

//인덕션_2 조리도구 유무상태를 알려주는 함수

//인덕션_2 초음파 센서를 통해 조리도구 유무를 확인한다.


void Induction1_ONOFF(){
  if(induction1_state){
       digitalWrite(induction1, HIGH); // LED를 켜기
    }else{
       digitalWrite(induction1, LOW); // LED를 켜기
  }
}

void Induction2_ONOFF(){
  if(induction2_state){
       digitalWrite(induction2, HIGH); // LED를 켜기
    }else{
       digitalWrite(induction2, LOW); // LED를 켜기
  }
}

void setup() {
  Serial.begin(115200);
  // 모듈 핀 셋팅
  pinMode(induction1, OUTPUT);
  pinMode(induction2, OUTPUT);

  //와파이 초기화
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");


  Induction1();
  Induction1_state_turn();
  Induction1_setTimer();
  ID1_CounteValue_Request();
  ID1_StopCount();

  Induction2();
  Induction2_state_turn();
  Induction2_setTimer();
  ID2_CounteValue_Request();
  ID2_StopCount();


  server.begin();
  Serial.println("Server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  Induction1_ONOFF();
  Induction2_ONOFF();
  CountTimer_ID1();
  CountTimer_ID2();
  
  
}
