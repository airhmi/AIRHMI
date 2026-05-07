/**
 * Position.ino  --  AirLabel konum yonetimi
 *
 * Etiketin Left, Top, Width, Height ozelliklerini Arduino tarafindan
 * kontrol eder. AirButton/Position'in label benzeri.
 *
 *   SET:
 *     - Set_left(uint32_t)
 *     - Set_top(uint32_t)
 *     - Set_width(uint32_t)
 *     - Set_height(uint32_t)
 *
 *   GET:
 *     - Get_left/top/width/height(uint32_t*)
 *
 * Panel tarafi (Position.ahi):
 *   lDemo                : hareketli ana etiket
 *   bUp/bDown/bLeft/bRight: konum -50/+50
 *   bRead                : Left/Top/Width/Height okur
 *   bReset               : Default'a doner
 *   lLeft/lTop/lWidth/lHeight : Get sonuclari
 *
 * NOT: ELabel WinForms tarafinda auto-size eder; Set_width/Set_height
 *      data update yapar ama goruntu metne gore otomatik boyutlanir.
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirLabel  lDemo    = AirLabel("lDemo");
AirLabel  lLeft    = AirLabel("lLeft");
AirLabel  lTop     = AirLabel("lTop");
AirLabel  lWidth   = AirLabel("lWidth");
AirLabel  lHeight  = AirLabel("lHeight");

AirButton bUp      = AirButton("bUp");
AirButton bDown    = AirButton("bDown");
AirButton bLeft    = AirButton("bLeft");
AirButton bRight   = AirButton("bRight");
AirButton bRead    = AirButton("bRead");
AirButton bReset   = AirButton("bReset");

/* Default konum (Position.ahi ile uyumlu) */
static const uint32_t DEFAULT_LEFT = 280;
static const uint32_t DEFAULT_TOP  = 60;

/* Yerel konum state'i — sınır kontrolu icin */
static uint32_t demoLeft = DEFAULT_LEFT;
static uint32_t demoTop  = DEFAULT_TOP;

/* Panel limitleri */
static const uint32_t LCD_WIDTH  = 800;
static const uint32_t LCD_HEIGHT = 480;
static const uint32_t MOVE_STEP  = 50;

/* ---------- callback'ler ---------- */
void onUp(void *ptr)
{
    if (demoTop >= MOVE_STEP) demoTop -= MOVE_STEP;
    else demoTop = 0;
    lDemo.Set_top(demoTop);
}

void onDown(void *ptr)
{
    if (demoTop + 60 + MOVE_STEP <= LCD_HEIGHT) demoTop += MOVE_STEP;
    lDemo.Set_top(demoTop);
}

void onLeftMove(void *ptr)
{
    if (demoLeft >= MOVE_STEP) demoLeft -= MOVE_STEP;
    else demoLeft = 0;
    lDemo.Set_left(demoLeft);
}

void onRight(void *ptr)
{
    if (demoLeft + 200 + MOVE_STEP <= LCD_WIDTH) demoLeft += MOVE_STEP;
    lDemo.Set_left(demoLeft);
}

void onRead(void *ptr)
{
    char tmp[16];
    uint32_t v = 0;

    lDemo.Get_left(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lLeft.setText(tmp);

    lDemo.Get_top(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lTop.setText(tmp);

    lDemo.Get_width(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lWidth.setText(tmp);

    lDemo.Get_height(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lHeight.setText(tmp);
}

void onReset(void *ptr)
{
    demoLeft = DEFAULT_LEFT;
    demoTop  = DEFAULT_TOP;
    lDemo.Set_left(demoLeft);
    lDemo.Set_top(demoTop);
    lLeft.setText("---");
    lTop.setText("---");
    lWidth.setText("---");
    lHeight.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bUp, &bDown, &bLeft, &bRight,
    &bRead, &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bUp.attachPop(onUp,         &bUp);
    bDown.attachPop(onDown,     &bDown);
    bLeft.attachPop(onLeftMove, &bLeft);
    bRight.attachPop(onRight,   &bRight);
    bRead.attachPop(onRead,     &bRead);
    bReset.attachPop(onReset,   &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
