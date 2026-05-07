/**
 * Position.ino  --  AirButton konum ve boyut yonetimi
 *
 * Buton'un Left, Top, Width, Height ozelliklerini yonetir:
 *
 *   SET:
 *     - Set_left(uint32_t)   : X koordinati (0..AIR_LCD_WIDTH)
 *     - Set_top(uint32_t)    : Y koordinati (0..AIR_LCD_HEIGHT)
 *     - Set_width(uint32_t)  : Genislik
 *     - Set_height(uint32_t) : Yukseklik
 *
 *   GET:
 *     - Get_left(uint32_t*)
 *     - Get_top(uint32_t*)
 *     - Get_width(uint32_t*)
 *     - Get_height(uint32_t*)
 *
 * Demo: bDemo butonunu paneldeki yon butonlariyla hareket ettir,
 *       boyut butonlariyla buyut/kuculusturur. Read mevcut konumu gosterir.
 *
 * Panel tarafi (Position.ahi):
 *   bDemo                : hareketli/yeniden boyutlanabilir buton
 *   bUp/bDown/bLeft/bRight: konum -50/+50 piksel kaydirir
 *   bSmaller/bLarger     : boyut +-20 piksel
 *   bRead                : Left + Top + Width + Height okur
 *   bReset               : Default konum/boyutuna doner
 *   lLeft / lTop / lWidth / lHeight : Get sonuclari
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirButton bDemo    = AirButton("bDemo");
AirButton bUp      = AirButton("bUp");
AirButton bDown    = AirButton("bDown");
AirButton bLeft    = AirButton("bLeft");
AirButton bRight   = AirButton("bRight");
AirButton bSmaller = AirButton("bSmaller");
AirButton bLarger  = AirButton("bLarger");
AirButton bRead    = AirButton("bRead");
AirButton bReset   = AirButton("bReset");

AirLabel  lLeft    = AirLabel("lLeft");
AirLabel  lTop     = AirLabel("lTop");
AirLabel  lWidth   = AirLabel("lWidth");
AirLabel  lHeight  = AirLabel("lHeight");

/* Default konum ve boyut (Position.ahi ile uyumlu) */
static const uint32_t DEFAULT_LEFT   = 320;
static const uint32_t DEFAULT_TOP    = 100;
static const uint32_t DEFAULT_WIDTH  = 160;
static const uint32_t DEFAULT_HEIGHT = 70;

/* Yerel state — panel sinirlari icinde tutmak icin */
static uint32_t demoLeft   = DEFAULT_LEFT;
static uint32_t demoTop    = DEFAULT_TOP;
static uint32_t demoWidth  = DEFAULT_WIDTH;
static uint32_t demoHeight = DEFAULT_HEIGHT;

/* Panel limitleri (800x480) */
static const uint32_t LCD_WIDTH  = 800;
static const uint32_t LCD_HEIGHT = 480;
static const uint32_t MIN_SIZE   = 60;
static const uint32_t MAX_SIZE   = 320;
static const uint32_t MOVE_STEP  = 50;
static const uint32_t SIZE_STEP  = 20;

/* ---------- callback'ler ---------- */
void onUp(void *ptr)
{
    if (demoTop >= MOVE_STEP)
        demoTop -= MOVE_STEP;
    else
        demoTop = 0;
    bDemo.Set_top(demoTop);
}

void onDown(void *ptr)
{
    if (demoTop + demoHeight + MOVE_STEP <= LCD_HEIGHT)
        demoTop += MOVE_STEP;
    bDemo.Set_top(demoTop);
}

void onLeftMove(void *ptr)
{
    if (demoLeft >= MOVE_STEP)
        demoLeft -= MOVE_STEP;
    else
        demoLeft = 0;
    bDemo.Set_left(demoLeft);
}

void onRight(void *ptr)
{
    if (demoLeft + demoWidth + MOVE_STEP <= LCD_WIDTH)
        demoLeft += MOVE_STEP;
    bDemo.Set_left(demoLeft);
}

void onSmaller(void *ptr)
{
    if (demoWidth > MIN_SIZE + SIZE_STEP)
        demoWidth -= SIZE_STEP;
    if (demoHeight > MIN_SIZE + SIZE_STEP)
        demoHeight -= SIZE_STEP;
    bDemo.Set_width(demoWidth);
    bDemo.Set_height(demoHeight);
}

void onLarger(void *ptr)
{
    if (demoWidth + SIZE_STEP <= MAX_SIZE && demoLeft + demoWidth + SIZE_STEP <= LCD_WIDTH)
        demoWidth += SIZE_STEP;
    if (demoHeight + SIZE_STEP <= MAX_SIZE && demoTop + demoHeight + SIZE_STEP <= LCD_HEIGHT)
        demoHeight += SIZE_STEP;
    bDemo.Set_width(demoWidth);
    bDemo.Set_height(demoHeight);
}

void onRead(void *ptr)
{
    char tmp[16];
    uint32_t v = 0;

    bDemo.Get_left(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lLeft.setText(tmp);

    bDemo.Get_top(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lTop.setText(tmp);

    bDemo.Get_width(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lWidth.setText(tmp);

    bDemo.Get_height(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v);
    lHeight.setText(tmp);
}

void onReset(void *ptr)
{
    demoLeft   = DEFAULT_LEFT;
    demoTop    = DEFAULT_TOP;
    demoWidth  = DEFAULT_WIDTH;
    demoHeight = DEFAULT_HEIGHT;
    bDemo.Set_left(demoLeft);
    bDemo.Set_top(demoTop);
    bDemo.Set_width(demoWidth);
    bDemo.Set_height(demoHeight);
    lLeft.setText("---");
    lTop.setText("---");
    lWidth.setText("---");
    lHeight.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bUp, &bDown, &bLeft, &bRight,
    &bSmaller, &bLarger,
    &bRead, &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bUp.attachPop(onUp,           &bUp);
    bDown.attachPop(onDown,       &bDown);
    bLeft.attachPop(onLeftMove,   &bLeft);
    bRight.attachPop(onRight,     &bRight);
    bSmaller.attachPop(onSmaller, &bSmaller);
    bLarger.attachPop(onLarger,   &bLarger);
    bRead.attachPop(onRead,       &bRead);
    bReset.attachPop(onReset,     &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
