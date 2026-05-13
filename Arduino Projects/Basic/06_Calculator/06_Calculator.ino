/**
 * 06_Calculator  --  Mini hesap makinesi
 *
 *   Standart 4-islem (+ - * /) hesap makinesi. 4x5 buton grid'i:
 *
 *       C   /   *   -
 *       7   8   9   +
 *       4   5   6
 *       1   2   3   =
 *           0   .
 *
 *   Display ELabelBox uzerinde, ondalikli sayilar destekli.
 *
 * Panel tarafi (Calculator.ahi):
 *   bC, bDiv, bMul, bSub, bAdd, bEq, bDot, b0..b9 (toplam 17 buton)
 *   lDisp : ELabelBox -- ekran
 */

#include <Airhmi.h>

AirLabel  lDisp = AirLabel("lDisp");
AirButton b0 = AirButton("b0"); AirButton b1 = AirButton("b1");
AirButton b2 = AirButton("b2"); AirButton b3 = AirButton("b3");
AirButton b4 = AirButton("b4"); AirButton b5 = AirButton("b5");
AirButton b6 = AirButton("b6"); AirButton b7 = AirButton("b7");
AirButton b8 = AirButton("b8"); AirButton b9 = AirButton("b9");
AirButton bDot = AirButton("bDot");
AirButton bAdd = AirButton("bAdd"); AirButton bSub = AirButton("bSub");
AirButton bMul = AirButton("bMul"); AirButton bDiv = AirButton("bDiv");
AirButton bEq  = AirButton("bEq");  AirButton bC   = AirButton("bC");

/* ---------- state ---------- */
char  buf[16] = "0";
double acc = 0.0;
char  op = 0;        /* 0 yok, '+', '-', '*', '/' */
bool  startNew = true;

void show()
{
    lDisp.setText(buf);
}

void inputDigit(char d)
{
    if (startNew) { buf[0] = 0; startNew = false; }
    int len = strlen(buf);
    if (len < 12) { buf[len] = d; buf[len + 1] = 0; }
    if (strcmp(buf, "") == 0) strcpy(buf, "0");
    show();
}

void inputDot()
{
    if (startNew) { strcpy(buf, "0"); startNew = false; }
    if (strchr(buf, '.') == NULL)
    {
        int n = strlen(buf);
        buf[n] = '.';
        buf[n + 1] = 0;
    }
    show();
}

void compute()
{
    double v = atof(buf);
    if      (op == 0)   acc = v;
    else if (op == '+') acc += v;
    else if (op == '-') acc -= v;
    else if (op == '*') acc *= v;
    else if (op == '/') { if (v != 0.0) acc /= v; else { strcpy(buf, "Err"); show(); return; } }

    /* acc -> buf */
    dtostrf(acc, 0, 4, buf);
    /* trailing zero ve nokta temizligi */
    int n = strlen(buf);
    if (strchr(buf, '.'))
    {
        while (n > 0 && buf[n - 1] == '0') { buf[--n] = 0; }
        if (n > 0 && buf[n - 1] == '.') buf[--n] = 0;
    }
    show();
}

void inputOp(char c)
{
    if (!startNew) compute();
    op = c;
    startNew = true;
}

void inputEq()
{
    if (!startNew) compute();
    op = 0;
    startNew = true;
}

void inputClear()
{
    acc = 0.0; op = 0; startNew = true;
    strcpy(buf, "0");
    show();
}

/* ---------- callback'ler ---------- */
void on0(void *p){inputDigit('0');} void on1(void *p){inputDigit('1');}
void on2(void *p){inputDigit('2');} void on3(void *p){inputDigit('3');}
void on4(void *p){inputDigit('4');} void on5(void *p){inputDigit('5');}
void on6(void *p){inputDigit('6');} void on7(void *p){inputDigit('7');}
void on8(void *p){inputDigit('8');} void on9(void *p){inputDigit('9');}
void onDot(void *p){inputDot();}
void onAdd(void *p){inputOp('+');}  void onSub(void *p){inputOp('-');}
void onMul(void *p){inputOp('*');}  void onDiv(void *p){inputOp('/');}
void onEq(void *p){inputEq();}      void onC(void *p){inputClear();}

AirTouch *air_listen_list[] = {
    &b0,&b1,&b2,&b3,&b4,&b5,&b6,&b7,&b8,&b9,
    &bDot,&bAdd,&bSub,&bMul,&bDiv,&bEq,&bC,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    b0.attachPop(on0,&b0); b1.attachPop(on1,&b1);
    b2.attachPop(on2,&b2); b3.attachPop(on3,&b3);
    b4.attachPop(on4,&b4); b5.attachPop(on5,&b5);
    b6.attachPop(on6,&b6); b7.attachPop(on7,&b7);
    b8.attachPop(on8,&b8); b9.attachPop(on9,&b9);
    bDot.attachPop(onDot,&bDot);
    bAdd.attachPop(onAdd,&bAdd); bSub.attachPop(onSub,&bSub);
    bMul.attachPop(onMul,&bMul); bDiv.attachPop(onDiv,&bDiv);
    bEq.attachPop(onEq,&bEq);    bC.attachPop(onC,&bC);
    show();
}

void loop()
{
    airLoop(air_listen_list);
}
