/**
 * 06_HomeSecurity  --  4 sayfali ev guvenlik paneli
 *
 *   Screen1 Status : ARMED / DISARMED + 4 zone gostergesi
 *   Screen2 Zones  : 4 zone enable/disable toggle
 *   Screen3 PIN    : 0-9 numpad + display + OK + Cancel (PIN = "1234")
 *   Screen4 Log    : son 5 olay listesi (etiket olarak)
 *
 *   PIN dogru girilince armed <-> disarmed toggle.
 */

#include <Airhmi.h>

/* Status */
AirLabel  lArmStatus = AirLabel("lArmStatus");
AirButton dZ1 = AirButton("dZ1");
AirButton dZ2 = AirButton("dZ2");
AirButton dZ3 = AirButton("dZ3");
AirButton dZ4 = AirButton("dZ4");

/* Zones */
AirButton bZ1 = AirButton("bZ1");
AirButton bZ2 = AirButton("bZ2");
AirButton bZ3 = AirButton("bZ3");
AirButton bZ4 = AirButton("bZ4");

/* PIN */
AirButton bN0 = AirButton("bN0"); AirButton bN1 = AirButton("bN1");
AirButton bN2 = AirButton("bN2"); AirButton bN3 = AirButton("bN3");
AirButton bN4 = AirButton("bN4"); AirButton bN5 = AirButton("bN5");
AirButton bN6 = AirButton("bN6"); AirButton bN7 = AirButton("bN7");
AirButton bN8 = AirButton("bN8"); AirButton bN9 = AirButton("bN9");
AirButton bOk = AirButton("bOk"); AirButton bCancel = AirButton("bCancel");
AirLabel  lPin    = AirLabel("lPin");
AirLabel  lPinMsg = AirLabel("lPinMsg");

/* Log */
AirLabel lLog0 = AirLabel("lLog0");
AirLabel lLog1 = AirLabel("lLog1");
AirLabel lLog2 = AirLabel("lLog2");
AirLabel lLog3 = AirLabel("lLog3");
AirLabel lLog4 = AirLabel("lLog4");

AirBuzzer buz = AirBuzzer("buz");

const char *PIN_CODE = "1234";
char  pinBuf[6] = "";
uint8_t pinLen = 0;
bool armed = false;
bool zoneEnabled[4] = { true, true, true, true };
const char *zoneLabel[4] = { "Door", "Window", "Garage", "Backyard" };

const uint32_t LED_OFF = 0xFF202020UL;
const uint32_t LED_ON  = 0xFF008000UL;

uint8_t logIndex = 0;
char logBuf[5][24];   /* 5 satir x 24 karakter ring buffer */

void renderArm()
{
    lArmStatus.setText(armed ? "ARMED" : "DISARMED");
}

void renderZone(uint8_t i)
{
    AirButton *b = (i==0)?&bZ1:(i==1)?&bZ2:(i==2)?&bZ3:&bZ4;
    AirButton *d = (i==0)?&dZ1:(i==1)?&dZ2:(i==2)?&dZ3:&dZ4;
    char tmp[20];
    snprintf(tmp, sizeof(tmp), "%s\n%s", zoneLabel[i], zoneEnabled[i]?"ON":"OFF");
    b->setText(tmp);
    b->Set_background_color(zoneEnabled[i] ? LED_ON : LED_OFF);
    d->Set_background_color(zoneEnabled[i] ? LED_ON : LED_OFF);
    d->setText(zoneLabel[i]);
}

void renderLog()
{
    AirLabel *labels[5] = { &lLog0, &lLog1, &lLog2, &lLog3, &lLog4 };
    for (uint8_t i = 0; i < 5; i++) labels[i]->setText(logBuf[i]);
}

void addLog(const char *msg)
{
    /* shift up */
    for (uint8_t i = 4; i > 0; i--)
        strcpy(logBuf[i], logBuf[i-1]);
    strncpy(logBuf[0], msg, 23);
    logBuf[0][23] = 0;
    renderLog();
}

void onZ1(void *p) { zoneEnabled[0] = !zoneEnabled[0]; renderZone(0); }
void onZ2(void *p) { zoneEnabled[1] = !zoneEnabled[1]; renderZone(1); }
void onZ3(void *p) { zoneEnabled[2] = !zoneEnabled[2]; renderZone(2); }
void onZ4(void *p) { zoneEnabled[3] = !zoneEnabled[3]; renderZone(3); }

void renderPin()
{
    char masked[8] = "";
    for (uint8_t i = 0; i < pinLen; i++) masked[i] = '*';
    masked[pinLen] = 0;
    lPin.setText(masked);
}

void appendDigit(char d)
{
    if (pinLen >= 4) return;
    pinBuf[pinLen++] = d;
    pinBuf[pinLen]   = 0;
    renderPin();
}

void on0(void *p){appendDigit('0');} void on1(void *p){appendDigit('1');}
void on2(void *p){appendDigit('2');} void on3(void *p){appendDigit('3');}
void on4(void *p){appendDigit('4');} void on5(void *p){appendDigit('5');}
void on6(void *p){appendDigit('6');} void on7(void *p){appendDigit('7');}
void on8(void *p){appendDigit('8');} void on9(void *p){appendDigit('9');}

void onCancel(void *p) { pinLen = 0; pinBuf[0] = 0; renderPin(); lPinMsg.setText(""); }

void onOk(void *p)
{
    if (strcmp(pinBuf, PIN_CODE) == 0)
    {
        armed = !armed;
        renderArm();
        lPinMsg.setText(armed ? "Armed OK" : "Disarmed OK");
        addLog(armed ? "ARMED" : "DISARMED");
        buz.Set_Buzzer(150);
    }
    else
    {
        lPinMsg.setText("Wrong PIN");
        addLog("PIN fail");
        buz.Set_Buzzer(500);
    }
    pinLen = 0; pinBuf[0] = 0;
    renderPin();
}

AirTouch *air_listen_list[] = {
    &bZ1, &bZ2, &bZ3, &bZ4,
    &bN0, &bN1, &bN2, &bN3, &bN4, &bN5, &bN6, &bN7, &bN8, &bN9,
    &bOk, &bCancel,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bZ1.attachPop(onZ1, &bZ1); bZ2.attachPop(onZ2, &bZ2);
    bZ3.attachPop(onZ3, &bZ3); bZ4.attachPop(onZ4, &bZ4);
    bN0.attachPop(on0,&bN0); bN1.attachPop(on1,&bN1);
    bN2.attachPop(on2,&bN2); bN3.attachPop(on3,&bN3);
    bN4.attachPop(on4,&bN4); bN5.attachPop(on5,&bN5);
    bN6.attachPop(on6,&bN6); bN7.attachPop(on7,&bN7);
    bN8.attachPop(on8,&bN8); bN9.attachPop(on9,&bN9);
    bOk.attachPop(onOk, &bOk);
    bCancel.attachPop(onCancel, &bCancel);

    renderArm();
    for (uint8_t i = 0; i < 4; i++) renderZone(i);
    for (uint8_t i = 0; i < 5; i++) logBuf[i][0] = 0;
    renderLog();
    renderPin();
    lPinMsg.setText("");
}

void loop()
{
    airLoop(air_listen_list);
}
