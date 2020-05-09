//------------------------------------------------------------------
// IntersectionTrafficLight sketch
// 交差点用信号機のスケッチ
// 
// 2020/1/26 aya
//------------------------------------------------------------------

#define SouthNorthBlueLed   3     // SothNroth信号機の青LEDの出力ポート設定
#define SouthNorthYellowLed 5     // SothNroth信号機の黄LEDの出力ポート設定
#define SouthNorthRedLed    6     // SothNroth信号機の赤LEDの出力ポート設定
#define EastWestBlueLed     9     // EastWest信号機の青LEDの出力ポート設定
#define EastWestYellowLed   10    // EastWest信号機の黄LEDの出力ポート設定
#define EastWestRedLed      11    // EastWest信号機の赤LEDの出力ポート設定

enum {
    BLE = 1,
    YEW,
    RED,
};

unsigned long gPreviousL1 = 0;    // 時間管理用変数

unsigned char gLightTime[]={200, 50, 10, 300, 50, 10};   // 点灯順序に対する点灯継続時間sec

//------------------------------------------------------------------
// Arduino固有の関数 setup() :初期設定
//------------------------------------------------------------------
void setup() {
  pinMode(SouthNorthBlueLed, OUTPUT);
  digitalWrite(SouthNorthBlueLed, LOW);
  pinMode(SouthNorthYellowLed , OUTPUT);
  digitalWrite(SouthNorthYellowLed , LOW);  
  pinMode(SouthNorthRedLed , OUTPUT);
  digitalWrite(SouthNorthRedLed , LOW);

  pinMode(EastWestBlueLed , OUTPUT);
  digitalWrite(EastWestBlueLed , LOW);
  pinMode(EastWestYellowLed , OUTPUT);
  digitalWrite(EastWestYellowLed , LOW);
  pinMode(EastWestRedLed , OUTPUT);
  digitalWrite(EastWestRedLed , LOW);

  gPreviousL1 = millis();
}

//---------------------------------------------------------------------
// Arduino main loop
//---------------------------------------------------------------------
void loop() {
  if ( (millis() - gPreviousL1) >= 100) { // 1000:1sec  100:100msec  10:10msec
    TrafficLightState();        //　100ms毎に信号機を処理する
    gPreviousL1 = millis();
  }
}

//---------------------------------------------------------------------
// 信号機点灯消灯処理
// 三項演算子という方法を使ってプログラムをスマート（わかりずらく）にしています
//---------------------------------------------------------------------
void BYR_LedCont(char SNLed , char EWLed){
  digitalWrite(SouthNorthBlueLed, SNLed & BLE ? HIGH:LOW);
  digitalWrite(SouthNorthYellowLed , SNLed & YEW ? HIGH:LOW);  
  digitalWrite(SouthNorthRedLed , SNLed & RED ? HIGH:LOW);

  digitalWrite(EastWestBlueLed , EWLed & BLE ? HIGH:LOW);
  digitalWrite(EastWestYellowLed , EWLed & YEW ? HIGH:LOW);
  digitalWrite(EastWestRedLed , EWLed & RED ? HIGH:LOW);
}

//---------------------------------------------------------------------
// 交差点信号機処理シーケンス
//---------------------------------------------------------------------
void TrafficLightState(){
  enum{
    ST_INIT = 0,        // 初期状態
    ST_SNblueEWred,     // 南北信号 青,東西信号 赤
    ST_SNYellowEWred,   // 南北信号 黄,東西信号 赤
    ST_SNredEWred,      // 南北信号 赤,東西信号 赤
    ST_EWblueSNred,     // 南北信号 赤,東西信号 青
    ST_EWyellowSNred,   // 南北信号 赤,東西信号 黄
    ST_EWredSNred,      // 南北信号 赤,東西信号 赤
  };
  static char state = ST_INIT;
  static char TimerCnt = 0;
  
  switch(state){
    case ST_INIT:
                  state = ST_SNblueEWred;
                  break;
    case ST_SNblueEWred:
                  BYR_LedCont(BLE,RED);          
                  TimerCnt = TimerCnt + 1;
                  if(TimerCnt >= gLightTime[0] ) {
                    state = ST_SNYellowEWred;
                    TimerCnt = 0;
                  }
                  break;
    case ST_SNYellowEWred:
                  BYR_LedCont(YEW,RED);          
                  TimerCnt = TimerCnt + 1;
                  if(TimerCnt >= gLightTime[1] ) {
                    state = ST_SNredEWred;
                    TimerCnt = 0;
                  }
                  break;
    case ST_SNredEWred:
                  BYR_LedCont(RED,RED);          
                  TimerCnt = TimerCnt + 1;
                  if(TimerCnt >= gLightTime[2] ) {
                    state = ST_EWblueSNred;
                    TimerCnt = 0;
                  }
                  break;                  

    case ST_EWblueSNred:
                  BYR_LedCont(RED,BLE);          
                  TimerCnt = TimerCnt + 1;
                  if(TimerCnt >= gLightTime[3] ) {
                    state = ST_EWyellowSNred;
                    TimerCnt = 0;
                  }
                  break;     
    case ST_EWyellowSNred:
                  BYR_LedCont(RED,YEW);          
                  TimerCnt = TimerCnt + 1;
                  if(TimerCnt >= gLightTime[4] ) {
                    state = ST_EWredSNred;
                    TimerCnt = 0;
                  }
                  break;     
    case ST_EWredSNred:
                  BYR_LedCont(RED,YEW);          
                  TimerCnt = TimerCnt + 1;
                  if(TimerCnt >= gLightTime[5] ) {
                    state = ST_SNblueEWred;
                    TimerCnt = 0;
                  }
                  break;   
    
    default:      break;
  }
}
