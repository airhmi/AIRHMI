/**
 * 13_ReactionMultiplayer  --  2 oyunculu reaksiyon yarisi
 *
 *   Start basinca 1..4 sn rastgele bekleme, sonra GO! goruunur.
 *   Iki oyuncudan hangisi onceaki butonuna basarsa kazanir; tepki
 *   suresi (ms) yazilir, kazanan skoruna 1 eklenir, gunun en iyisi
 *   AirVariable'da panel-side kalici sayilir.
 *
 *   Erken basan oyuncu hata yapar -> rakibe puan, "FOUL!" mesaji.
 */

#include <Airhmi.h>

AirButton bStart  = AirButton("bStart");
AirButton bP1     = AirButton("bP1");
AirButton bP2     = AirButton("bP2");
AirButton bGo     = AirButton("bGo");
AirLabel  lInfo   = AirLabel("lInfo");
AirLabel  lScore  = AirLabel("lScore");
AirLabel  lLastMs = AirLabel("lLastMs");
AirLabel  lBest   = AirLabel("lBest");

AirVariable bestVar = AirVariable("best");

const uint32_t COLOR_GO_OFF = 0xFF202020UL;
const uint32_t COLOR_GO_ON  = 0xFF00C000UL;
const uint32_t COLOR_BAD    = 0xFFC00000UL;

enum State { IDLE, WAITING, GO };
State    st = IDLE;
uint32_t goAtMs = 0;
uint32_t stateUntil = 0;
uint16_t scoreP1 = 0, scoreP2 = 0;
uint16_t bestMs = 9999;

void renderScore()
{
    char tmp[24];
    snprintf(tmp, sizeof(tmp), "P1 %u : %u P2", scoreP1, scoreP2);
    lScore.setText(tmp);
}

void renderBest()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "Best: %u ms", bestMs);
    lBest.setText(tmp);
}

void updateBest(uint16_t ms)
{
    if (ms < bestMs) { bestMs = ms; bestVar.VarSeti(ms); renderBest(); }
}

void onStart(void *p)
{
    st = WAITING;
    bGo.Set_background_color(COLOR_GO_OFF);
    bGo.setText("Wait...");
    lInfo.setText("Wait for green...");
    lLastMs.setText("---");
    stateUntil = millis() + 1000UL + (uint32_t)random(0, 3001);
}

void award(uint8_t winner, uint16_t ms)
{
    if (winner == 1) scoreP1++; else scoreP2++;
    char tmp[24];
    snprintf(tmp, sizeof(tmp), "P%u wins: %u ms", winner, ms);
    lInfo.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u ms", ms);
    lLastMs.setText(tmp);
    bGo.Set_background_color(COLOR_GO_OFF);
    bGo.setText("--");
    renderScore();
    updateBest(ms);
    st = IDLE;
}

void onP1(void *p)
{
    if (st == WAITING)
    {
        bGo.Set_background_color(COLOR_BAD); bGo.setText("FOUL");
        lInfo.setText("P1 too soon -> P2 +1");
        scoreP2++; renderScore();
        st = IDLE;
        return;
    }
    if (st == GO)
        award(1, (uint16_t)(millis() - goAtMs));
}

void onP2(void *p)
{
    if (st == WAITING)
    {
        bGo.Set_background_color(COLOR_BAD); bGo.setText("FOUL");
        lInfo.setText("P2 too soon -> P1 +1");
        scoreP1++; renderScore();
        st = IDLE;
        return;
    }
    if (st == GO)
        award(2, (uint16_t)(millis() - goAtMs));
}

AirTouch *air_listen_list[] = { &bStart, &bP1, &bP2, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    randomSeed(analogRead(A0));
    bStart.attachPop(onStart, &bStart);
    bP1.attachPop(onP1, &bP1);
    bP2.attachPop(onP2, &bP2);
    bGo.Set_background_color(COLOR_GO_OFF);
    bGo.setText("--");
    lInfo.setText("Press Start");
    renderScore();
    bestMs = (uint16_t)bestVar.VarGeti();
    if (bestMs == 0 || bestMs > 5000) bestMs = 9999;
    renderBest();
}

void loop()
{
    airLoop(air_listen_list);

    if (st == WAITING && millis() >= stateUntil)
    {
        st = GO;
        goAtMs = millis();
        bGo.Set_background_color(COLOR_GO_ON);
        bGo.setText("GO!");
        lInfo.setText("HIT NOW!");
    }
}
