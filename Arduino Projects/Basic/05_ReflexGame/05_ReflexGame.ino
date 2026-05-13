/**
 * 05_ReflexGame  --  Reaksiyon suresi olcumu
 *
 *   Start'a basinca rastgele 1..4 sn bekleme yapilir, sonra bGo butonu
 *   yesile doner ("HIT!" yazar). bGo'ya basildigi anda ms cinsinden tepki
 *   suresi olculur ve lTime'a yazilir. Erken basanlar "Too soon!" goruur.
 *
 * Panel tarafi (ReflexGame.ahi):
 *   bStart  : oyunu baslatir
 *   bGo     : tepki butonu (gri -> yesil)
 *   lTime   : sonuc "X ms"
 *   lInfo   : durum / talimat
 */

#include <Airhmi.h>

const uint32_t COLOR_GO_OFF = 0xFF404040UL;
const uint32_t COLOR_GO_ON  = 0xFF00C000UL;
const uint32_t COLOR_BAD    = 0xFFC00000UL;

AirButton bStart = AirButton("bStart");
AirButton bGo    = AirButton("bGo");
AirLabel  lTime  = AirLabel("lTime");
AirLabel  lInfo  = AirLabel("lInfo");

enum State { IDLE, WAITING, GO };
State state = IDLE;
unsigned long stateUntil = 0;
unsigned long goAtMs     = 0;

void onStart(void *p)
{
    state = WAITING;
    bGo.Set_background_color(COLOR_GO_OFF);
    bGo.setText("Wait...");
    lTime.setText("---");
    lInfo.setText("Concentrate...");
    /* 1000..4000 ms rastgele bekleme */
    unsigned long delayMs = 1000UL + (unsigned long)random(0, 3001);
    stateUntil = millis() + delayMs;
}

void onGo(void *p)
{
    if (state == WAITING)
    {
        /* erken bastik */
        bGo.Set_background_color(COLOR_BAD);
        bGo.setText("FAIL");
        lInfo.setText("Too soon!");
        lTime.setText("---");
        state = IDLE;
        return;
    }
    if (state == GO)
    {
        unsigned long ms = millis() - goAtMs;
        char tmp[16];
        snprintf(tmp, sizeof(tmp), "%lu ms", ms);
        lTime.setText(tmp);
        lInfo.setText("Press Start for again");
        bGo.Set_background_color(COLOR_GO_OFF);
        bGo.setText("--");
        state = IDLE;
    }
}

AirTouch *air_listen_list[] = { &bStart, &bGo, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    randomSeed(analogRead(A0));
    bStart.attachPop(onStart, &bStart);
    bGo.attachPop(onGo,       &bGo);
    bGo.Set_background_color(COLOR_GO_OFF);
    bGo.setText("--");
    lTime.setText("---");
    lInfo.setText("Press Start");
}

void loop()
{
    airLoop(air_listen_list);

    if (state == WAITING && millis() >= stateUntil)
    {
        state = GO;
        goAtMs = millis();
        bGo.Set_background_color(COLOR_GO_ON);
        bGo.setText("HIT!");
        lInfo.setText("NOW!");
    }
}
