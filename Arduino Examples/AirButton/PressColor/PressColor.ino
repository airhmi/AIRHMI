/**
 * PressColor.ino  --  AirButton basili durum rengi yonetimi
 *
 * Buton basildiginda gosterilen renkleri kontrol eder. Normal renk ve
 * basili renk arasindaki farki goz onunde tutar:
 *
 *   - Color / ColorTo            : butonun normal (release) renkleri
 *   - Press_Color / Press_ColorTo: butonun basili (pressed) renkleri
 *
 *   SET:
 *     - Set_press_background_color(uint32_t)
 *     - Set_press_background_colorTo(uint32_t)
 *
 *   GET:
 *     - Get_press_background_color(uint32_t*)
 *     - Get_press_background_colorTo(uint32_t*)
 *
 * NOT: Press renkleri gormek icin bDemo butonuna parmagini basili tut.
 *      Burakildiginda normal renge doner.
 *
 * Panel tarafi (PressColor.ahi):
 *   bDemo                    : test butonu (basinca renk degisir)
 *   bRedPress / bGreenPress / bBluePress / bDarkPress
 *                            : Press_Color presets
 *   bGradientPress           : Press_Color + Press_ColorTo birlikte
 *   bRead                    : Press_Color + Press_ColorTo okur
 *   bReset                   : Default press renklerine doner
 *   lPressColor / lPressColorTo : Get sonuclari
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirButton bDemo          = AirButton("bDemo");
AirButton bRedPress      = AirButton("bRedPress");
AirButton bGreenPress    = AirButton("bGreenPress");
AirButton bBluePress     = AirButton("bBluePress");
AirButton bDarkPress     = AirButton("bDarkPress");
AirButton bGradientPress = AirButton("bGradientPress");
AirButton bRead          = AirButton("bRead");
AirButton bReset         = AirButton("bReset");

AirLabel  lPressColor    = AirLabel("lPressColor");
AirLabel  lPressColorTo  = AirLabel("lPressColorTo");

/* Renk sabitleri (0xAARRGGBB bit pattern) */
static const uint32_t COLOR_RED      = 0xFFFF0000UL;
static const uint32_t COLOR_DARKRED  = 0xFF800000UL;
static const uint32_t COLOR_GREEN    = 0xFF00FF00UL;
static const uint32_t COLOR_DARKGREEN= 0xFF008000UL;
static const uint32_t COLOR_BLUE     = 0xFF0000FFUL;
static const uint32_t COLOR_DARKBLUE = 0xFF000080UL;
static const uint32_t COLOR_BLACK    = 0xFF000000UL;
static const uint32_t COLOR_DARKGRAY = 0xFF404040UL;
static const uint32_t COLOR_GOLD     = 0xFFFFC000UL;
static const uint32_t COLOR_LIGHTGRAY= 0xFFC0C0C0UL;

/* ---------- callback'ler ---------- */
void onRedPress(void *ptr)
{
    bDemo.Set_press_background_color(COLOR_RED);
    bDemo.Set_press_background_colorTo(COLOR_DARKRED);
}

void onGreenPress(void *ptr)
{
    bDemo.Set_press_background_color(COLOR_GREEN);
    bDemo.Set_press_background_colorTo(COLOR_DARKGREEN);
}

void onBluePress(void *ptr)
{
    bDemo.Set_press_background_color(COLOR_BLUE);
    bDemo.Set_press_background_colorTo(COLOR_DARKBLUE);
}

void onDarkPress(void *ptr)
{
    bDemo.Set_press_background_color(COLOR_BLACK);
    bDemo.Set_press_background_colorTo(COLOR_DARKGRAY);
}

void onGradientPress(void *ptr)
{
    /* Yumusak gradient: altin -> koyu kirmizi */
    bDemo.Set_press_background_color(COLOR_GOLD);
    bDemo.Set_press_background_colorTo(COLOR_DARKRED);
}

void onRead(void *ptr)
{
    char tmp[16];
    uint32_t v = 0;

    bDemo.Get_press_background_color(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lPressColor.setText(tmp);

    bDemo.Get_press_background_colorTo(&v);
    snprintf(tmp, sizeof(tmp), "%ld", (long)(int32_t)v);
    lPressColorTo.setText(tmp);
}

void onReset(void *ptr)
{
    bDemo.Set_press_background_color(COLOR_LIGHTGRAY);
    bDemo.Set_press_background_colorTo(COLOR_DARKGRAY);
    lPressColor.setText("---");
    lPressColorTo.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bRedPress,
    &bGreenPress,
    &bBluePress,
    &bDarkPress,
    &bGradientPress,
    &bRead,
    &bReset,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bRedPress.attachPop(onRedPress,           &bRedPress);
    bGreenPress.attachPop(onGreenPress,       &bGreenPress);
    bBluePress.attachPop(onBluePress,         &bBluePress);
    bDarkPress.attachPop(onDarkPress,         &bDarkPress);
    bGradientPress.attachPop(onGradientPress, &bGradientPress);
    bRead.attachPop(onRead,                   &bRead);
    bReset.attachPop(onReset,                 &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
