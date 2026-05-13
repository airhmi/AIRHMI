/**
 * 20_StockTradingTerminal  --  5 sayfali borsa terminali
 *
 *   Screen1 Watchlist : 5 hisse (sembol/fiyat) + button rengiyle yon, day-range
 *                       AirProgressBar (low..high arasinda mevcut konum)
 *   Screen2 Chart     : secili hisse icin AirGraph (60-bar trend)
 *   Screen3 Portfolio : 3 pozisyon (qty, P/L) + total equity
 *   Screen4 Order     : sembol secimi, miktar slider, BUY/SELL
 *   Screen5 News      : son 5 haber baslik
 *
 *   Yeni component: AirProgressBar (day range gostergesi).
 */

#include <Airhmi.h>

/* Watchlist - butonlar renk degisimi icin (Set_background_color) */
AirButton b0 = AirButton("b0");
AirButton b1 = AirButton("b1");
AirButton b2 = AirButton("b2");
AirButton b3 = AirButton("b3");
AirButton b4 = AirButton("b4");
AirProgressBar pbDay = AirProgressBar("pbDay");
AirLabel  lDayRange = AirLabel("lDayRange");

/* Chart */
AirLabel  lChSym = AirLabel("lChSym");
AirGraph  gChart = AirGraph("gChart");

/* Portfolio */
AirLabel  lP0 = AirLabel("lP0");
AirLabel  lP1 = AirLabel("lP1");
AirLabel  lP2 = AirLabel("lP2");
AirLabel  lTotal = AirLabel("lTotal");

/* Order */
AirLabel  lOrdSym = AirLabel("lOrdSym");
AirButton bPrev = AirButton("bPrev");
AirButton bNext = AirButton("bNext");
AirSlider sQty = AirSlider("sQty");
AirLabel  lQty = AirLabel("lQty");
AirButton bBuy = AirButton("bBuy");
AirButton bSell = AirButton("bSell");
AirLabel  lOrdMsg = AirLabel("lOrdMsg");

/* News */
AirLabel lN0 = AirLabel("lN0");
AirLabel lN1 = AirLabel("lN1");
AirLabel lN2 = AirLabel("lN2");
AirLabel lN3 = AirLabel("lN3");
AirLabel lN4 = AirLabel("lN4");

AirBuzzer buz = AirBuzzer("buz");

const char *sym[5] = { "AAPL", "MSFT", "TSLA", "AMZN", "NVDA" };
float price[5] = { 175.0f, 410.0f, 240.0f, 180.0f, 920.0f };
float dayLow[5] = { 174.0f, 408.0f, 238.0f, 178.0f, 915.0f };
float dayHigh[5] = { 178.0f, 415.0f, 245.0f, 184.0f, 928.0f };

uint8_t pos[3] = { 0, 1, 2 };
uint16_t qty[3] = { 10, 5, 8 };
float costBasis[3] = { 170.0f, 405.0f, 235.0f };

uint8_t selected = 0;
uint16_t ordQty = 1;

unsigned long lastTick = 0;

const uint32_t COL_GREEN = 0xFF008000UL;
const uint32_t COL_RED = 0xFFFF0000UL;
const uint32_t COL_GRAY = 0xFF404040UL;

char newsBuf[5][24];

void renderWatchlist()
{
    AirButton *btns[5] = { &b0, &b1, &b2, &b3, &b4 };
    char tmp[24];
    for (uint8_t i = 0; i < 5; i++) {
        char p[8]; dtostrf(price[i], 0, 2, p);
        snprintf(tmp, sizeof(tmp), "%s\n%s", sym[i], p);
        btns[i]->setText(tmp);
        uint32_t c = COL_GRAY;
        float mid = (dayLow[i] + dayHigh[i]) * 0.5f;
        if (price[i] > mid + 0.5f) c = COL_GREEN;
        else if (price[i] < mid - 0.5f) c = COL_RED;
        btns[i]->Set_background_color(c);
    }
    float lo = dayLow[selected], hi = dayHigh[selected], cur = price[selected];
    uint8_t pct = 0;
    if (hi > lo) pct = (uint8_t)((cur - lo) / (hi - lo) * 100.0f);
    if (pct > 100) pct = 100;
    pbDay.Set_Value(pct);
    char loS[8], hiS[8], curS[8];
    dtostrf(lo, 0, 2, loS); dtostrf(hi, 0, 2, hiS); dtostrf(cur, 0, 2, curS);
    snprintf(tmp, sizeof(tmp), "%s L%s C%s H%s", sym[selected], loS, curS, hiS);
    lDayRange.setText(tmp);
}

void renderChart()
{
    lChSym.setText(sym[selected]);
}

void renderPortfolio()
{
    AirLabel *labels[3] = { &lP0, &lP1, &lP2 };
    char tmp[32];
    float total = 0;
    for (uint8_t i = 0; i < 3; i++) {
        float pl = (price[pos[i]] - costBasis[i]) * qty[i];
        total += price[pos[i]] * qty[i];
        char p[8]; dtostrf(pl, 0, 1, p);
        snprintf(tmp, sizeof(tmp), "%s %ux PL %s$", sym[pos[i]], qty[i], p);
        labels[i]->setText(tmp);
    }
    char t[12]; dtostrf(total, 0, 0, t);
    snprintf(tmp, sizeof(tmp), "Equity %s $", t);
    lTotal.setText(tmp);
}

void renderOrder()
{
    char tmp[16];
    char p[8]; dtostrf(price[selected], 0, 2, p);
    snprintf(tmp, sizeof(tmp), "%s @ %s", sym[selected], p);
    lOrdSym.setText(tmp);
    snprintf(tmp, sizeof(tmp), "Qty: %u", ordQty);
    lQty.setText(tmp);
}

void renderNews()
{
    AirLabel *labels[5] = { &lN0, &lN1, &lN2, &lN3, &lN4 };
    for (uint8_t i = 0; i < 5; i++) labels[i]->setText(newsBuf[i]);
}

void addNews(const char *msg)
{
    for (uint8_t i = 4; i > 0; i--) strcpy(newsBuf[i], newsBuf[i-1]);
    strncpy(newsBuf[0], msg, 23);
    newsBuf[0][23] = 0;
    renderNews();
}

void onB0(void *p) { selected = 0; renderChart(); renderWatchlist(); renderOrder(); }
void onB1(void *p) { selected = 1; renderChart(); renderWatchlist(); renderOrder(); }
void onB2(void *p) { selected = 2; renderChart(); renderWatchlist(); renderOrder(); }
void onB3(void *p) { selected = 3; renderChart(); renderWatchlist(); renderOrder(); }
void onB4(void *p) { selected = 4; renderChart(); renderWatchlist(); renderOrder(); }

void onPrev(void *p) { if (selected==0) selected=4; else selected--; renderOrder(); renderChart(); renderWatchlist(); }
void onNext(void *p) { selected = (selected+1) % 5; renderOrder(); renderChart(); renderWatchlist(); }
void onQty(void *p)
{
    uint32_t v = 0; sQty.Get_Value(&v);
    if (v < 1) v = 1; if (v > 100) v = 100;
    ordQty = v;
    renderOrder();
}
void onBuy(void *p)
{
    char tmp[24];
    snprintf(tmp, sizeof(tmp), "BUY %s %u", sym[selected], ordQty);
    addNews(tmp);
    buz.Set_Buzzer(80);
    lOrdMsg.setText("Order filled");
}
void onSell(void *p)
{
    char tmp[24];
    snprintf(tmp, sizeof(tmp), "SELL %s %u", sym[selected], ordQty);
    addNews(tmp);
    buz.Set_Buzzer(120);
    lOrdMsg.setText("Order filled");
}

AirTouch *air_listen_list[] = {
    &b0, &b1, &b2, &b3, &b4,
    &bPrev, &bNext, &sQty, &bBuy, &bSell,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    b0.attachPop(onB0, &b0); b1.attachPop(onB1, &b1);
    b2.attachPop(onB2, &b2); b3.attachPop(onB3, &b3);
    b4.attachPop(onB4, &b4);
    bPrev.attachPop(onPrev, &bPrev);
    bNext.attachPop(onNext, &bNext);
    sQty.attachPop(onQty, &sQty);
    bBuy.attachPop(onBuy, &bBuy);
    bSell.attachPop(onSell, &bSell);

    sQty.Set_Value(ordQty);
    pbDay.Set_Value(50);
    for (uint8_t i = 0; i < 5; i++) newsBuf[i][0] = 0;
    addNews("Market open");
    renderWatchlist(); renderChart(); renderPortfolio(); renderOrder();
    lOrdMsg.setText("");
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastTick < 1500UL) return;
    lastTick = millis();

    for (uint8_t i = 0; i < 5; i++) {
        float delta = (random(200) - 100) / 100.0f;
        price[i] += delta;
        if (price[i] < dayLow[i]) dayLow[i] = price[i];
        if (price[i] > dayHigh[i]) dayHigh[i] = price[i];
    }

    int16_t v = (int16_t)((price[selected] - 100.0f) / 5.0f);
    if (v < 0) v = 0; if (v > 200) v = 200;
    gChart.addValue((uint32_t)v, 0);

    renderWatchlist(); renderPortfolio();
}
