/**
 * 23_ATMSimulator  --  5 sayfali ATM simulasyonu
 *
 *   Screen1 PIN     : 0-9 numpad + display + ENTER + CLEAR (PIN = 1234)
 *   Screen2 Account : balance + son 4 islem listesi
 *   Screen3 Withdraw: 4 hizli tutar butonu (50/100/200/500) + AirProgressBar
 *                     dispense progress
 *   Screen4 Transfer: hedef hesap secimi (3 kayitli) + tutar slider + GONDER
 *   Screen5 Receipt : son islem ozeti + AirShape (kart slot dekoratif)
 *
 *   Yeni component: AirProgressBar (cash dispense).
 */

#include <Airhmi.h>

/* PIN */
AirButton bN0 = AirButton("bN0"); AirButton bN1 = AirButton("bN1");
AirButton bN2 = AirButton("bN2"); AirButton bN3 = AirButton("bN3");
AirButton bN4 = AirButton("bN4"); AirButton bN5 = AirButton("bN5");
AirButton bN6 = AirButton("bN6"); AirButton bN7 = AirButton("bN7");
AirButton bN8 = AirButton("bN8"); AirButton bN9 = AirButton("bN9");
AirButton bEnter = AirButton("bEnter");
AirButton bClear = AirButton("bClear");
AirLabel  lPin = AirLabel("lPin");
AirLabel  lPinMsg = AirLabel("lPinMsg");

/* Account */
AirLabel  lBalance = AirLabel("lBalance");
AirLabel  lH0 = AirLabel("lH0");
AirLabel  lH1 = AirLabel("lH1");
AirLabel  lH2 = AirLabel("lH2");
AirLabel  lH3 = AirLabel("lH3");

/* Withdraw */
AirButton b50 = AirButton("b50");
AirButton b100 = AirButton("b100");
AirButton b200 = AirButton("b200");
AirButton b500 = AirButton("b500");
AirProgressBar pbDisp = AirProgressBar("pbDisp");
AirLabel  lDispMsg = AirLabel("lDispMsg");

/* Transfer */
AirButton bAcct0 = AirButton("bAcct0");
AirButton bAcct1 = AirButton("bAcct1");
AirButton bAcct2 = AirButton("bAcct2");
AirSlider sAmount = AirSlider("sAmount");
AirLabel  lAmount = AirLabel("lAmount");
AirButton bSend = AirButton("bSend");

/* Receipt */
AirLabel  lRcpt = AirLabel("lRcpt");

AirBuzzer buz = AirBuzzer("buz");

const char *PIN_CODE = "1234";
char  pinBuf[6] = "";
uint8_t pinLen = 0;
bool unlocked = false;

uint32_t balance = 4250;
char history[4][20];

unsigned long dispStart = 0;
bool dispensing = false;
uint16_t dispAmount = 0;

uint8_t curAcct = 0;
const char *acctName[3] = { "Mom", "Spouse", "Boss" };
uint16_t txfAmount = 100;

char lastReceipt[40] = "No transaction";

const uint32_t COL_OFF = 0xFF202020UL;
const uint32_t COL_ON = 0xFF008000UL;

void renderPin()
{
    char masked[8] = "";
    for (uint8_t i = 0; i < pinLen; i++) masked[i] = '*';
    masked[pinLen] = 0;
    lPin.setText(masked);
}

void renderAccount()
{
    char tmp[20];
    snprintf(tmp, sizeof(tmp), "%lu $", balance);
    lBalance.setText(tmp);
    AirLabel *labels[4] = { &lH0, &lH1, &lH2, &lH3 };
    for (uint8_t i = 0; i < 4; i++) labels[i]->setText(history[i]);
}

void renderWithdraw()
{
    if (dispensing) {
        lDispMsg.setText("Dispensing...");
    } else {
        lDispMsg.setText("Select amount");
    }
}

void renderTransfer()
{
    AirButton *btns[3] = { &bAcct0, &bAcct1, &bAcct2 };
    for (uint8_t i = 0; i < 3; i++) {
        btns[i]->setText(acctName[i]);
        btns[i]->Set_background_color(i == curAcct ? COL_ON : COL_OFF);
    }
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u $", txfAmount);
    lAmount.setText(tmp);
}

void renderReceipt()
{
    lRcpt.setText(lastReceipt);
}

void addHistory(const char *msg)
{
    for (uint8_t i = 3; i > 0; i--) strcpy(history[i], history[i-1]);
    strncpy(history[0], msg, 19);
    history[0][19] = 0;
    renderAccount();
}

void appendDigit(char d)
{
    if (!unlocked) {
        if (pinLen >= 4) return;
        pinBuf[pinLen++] = d;
        pinBuf[pinLen] = 0;
        renderPin();
    }
}

void on0(void *p){appendDigit('0');} void on1(void *p){appendDigit('1');}
void on2(void *p){appendDigit('2');} void on3(void *p){appendDigit('3');}
void on4(void *p){appendDigit('4');} void on5(void *p){appendDigit('5');}
void on6(void *p){appendDigit('6');} void on7(void *p){appendDigit('7');}
void on8(void *p){appendDigit('8');} void on9(void *p){appendDigit('9');}

void onClear(void *p) { pinLen = 0; pinBuf[0] = 0; renderPin(); lPinMsg.setText(""); }

void onEnter(void *p)
{
    if (strcmp(pinBuf, PIN_CODE) == 0) {
        unlocked = true;
        lPinMsg.setText("OK - Welcome");
        buz.Set_Buzzer(80);
    } else {
        lPinMsg.setText("Wrong PIN");
        buz.Set_Buzzer(300);
    }
    pinLen = 0; pinBuf[0] = 0;
    renderPin();
}

void doWithdraw(uint16_t amt)
{
    if (!unlocked) { lDispMsg.setText("Locked"); return; }
    if (amt > balance) { lDispMsg.setText("Insufficient funds"); buz.Set_Buzzer(400); return; }
    dispensing = true;
    dispAmount = amt;
    dispStart = millis();
    pbDisp.Set_Value(0);
    char tmp[40];
    snprintf(tmp, sizeof(tmp), "WITHDRAW %u $", amt);
    strcpy(lastReceipt, tmp);
}

void on50(void *p)  { doWithdraw(50); }
void on100(void *p) { doWithdraw(100); }
void on200(void *p) { doWithdraw(200); }
void on500(void *p) { doWithdraw(500); }

void onAcct0(void *p) { curAcct = 0; renderTransfer(); }
void onAcct1(void *p) { curAcct = 1; renderTransfer(); }
void onAcct2(void *p) { curAcct = 2; renderTransfer(); }

void onAmount(void *p)
{
    uint32_t v = 0; sAmount.Get_Value(&v);
    if (v < 10) v = 10; if (v > 1000) v = 1000;
    txfAmount = v;
    renderTransfer();
}

void onSend(void *p)
{
    if (!unlocked) return;
    if (txfAmount > balance) { buz.Set_Buzzer(400); return; }
    balance -= txfAmount;
    char tmp[20];
    snprintf(tmp, sizeof(tmp), "TXF %s %u$", acctName[curAcct], txfAmount);
    addHistory(tmp);
    snprintf(lastReceipt, sizeof(lastReceipt), "TRANSFER %u $ to %s", txfAmount, acctName[curAcct]);
    buz.Set_Buzzer(80);
    renderReceipt();
}

AirTouch *air_listen_list[] = {
    &bN0,&bN1,&bN2,&bN3,&bN4,&bN5,&bN6,&bN7,&bN8,&bN9,
    &bEnter, &bClear,
    &b50, &b100, &b200, &b500,
    &bAcct0, &bAcct1, &bAcct2, &sAmount, &bSend,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bN0.attachPop(on0,&bN0); bN1.attachPop(on1,&bN1);
    bN2.attachPop(on2,&bN2); bN3.attachPop(on3,&bN3);
    bN4.attachPop(on4,&bN4); bN5.attachPop(on5,&bN5);
    bN6.attachPop(on6,&bN6); bN7.attachPop(on7,&bN7);
    bN8.attachPop(on8,&bN8); bN9.attachPop(on9,&bN9);
    bEnter.attachPop(onEnter, &bEnter);
    bClear.attachPop(onClear, &bClear);
    b50.attachPop(on50, &b50);
    b100.attachPop(on100, &b100);
    b200.attachPop(on200, &b200);
    b500.attachPop(on500, &b500);
    bAcct0.attachPop(onAcct0, &bAcct0);
    bAcct1.attachPop(onAcct1, &bAcct1);
    bAcct2.attachPop(onAcct2, &bAcct2);
    sAmount.attachPop(onAmount, &sAmount);
    bSend.attachPop(onSend, &bSend);

    sAmount.Set_Value(txfAmount);
    pbDisp.Set_Value(0);
    for (uint8_t i = 0; i < 4; i++) history[i][0] = 0;
    strcpy(history[0], "Init balance");
    renderPin(); renderAccount(); renderWithdraw(); renderTransfer(); renderReceipt();
    lPinMsg.setText("Enter PIN");
    lDispMsg.setText("Locked");
}

void loop()
{
    airLoop(air_listen_list);

    if (dispensing) {
        unsigned long el = millis() - dispStart;
        uint8_t pct = el >= 3000UL ? 100 : (uint8_t)(el * 100UL / 3000UL);
        pbDisp.Set_Value(pct);
        if (el >= 3000UL) {
            dispensing = false;
            balance -= dispAmount;
            char tmp[20];
            snprintf(tmp, sizeof(tmp), "WD %u$", dispAmount);
            addHistory(tmp);
            lDispMsg.setText("Take cash");
            buz.Set_Buzzer(150);
            renderReceipt();
        }
    }
}
