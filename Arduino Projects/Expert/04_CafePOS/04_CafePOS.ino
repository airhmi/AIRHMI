/**
 * 04_CafePOS  --  4 sayfali kafe POS sistemi
 *
 *   Screen1 Menu     : 6 urun butonu, basinca sepete ekle, total guncelle
 *   Screen2 Cart     : sepet ozetı (item count + total) + clear
 *   Screen3 Payment  : Cash / Card / Cancel + son islem mesaji
 *   Screen4 History  : gunluk satis sayisi + gunluk gelir
 */

#include <Airhmi.h>

/* Menu */
AirButton bP1 = AirButton("bP1");
AirButton bP2 = AirButton("bP2");
AirButton bP3 = AirButton("bP3");
AirButton bP4 = AirButton("bP4");
AirButton bP5 = AirButton("bP5");
AirButton bP6 = AirButton("bP6");
AirLabel  lCartTotal = AirLabel("lCartTotal");
AirLabel  lCartItems = AirLabel("lCartItems");

/* Cart */
AirLabel  lCartTotal2 = AirLabel("lCartTotal2");
AirLabel  lCartItems2 = AirLabel("lCartItems2");
AirButton bClear = AirButton("bClear");

/* Payment */
AirButton bCash   = AirButton("bCash");
AirButton bCard   = AirButton("bCard");
AirButton bCancel = AirButton("bCancel");
AirLabel  lPayMsg = AirLabel("lPayMsg");

/* History */
AirLabel lDailyCount = AirLabel("lDailyCount");
AirLabel lDailySum   = AirLabel("lDailySum");
AirButton bResetDay  = AirButton("bResetDay");

const char *PROD_NAME[6]  = { "Espresso", "Latte", "Mocha", "Tea", "Cookie", "Cake" };
const uint16_t PROD_PRICE[6] = {     30,      50,      55,    25,     20,      45 };

uint16_t cartTotal = 0;
uint8_t  cartItems = 0;
uint16_t dailyCount = 0;
uint32_t dailySum   = 0;

void renderCart()
{
    char tmp[20];
    snprintf(tmp, sizeof(tmp), "Total: %u TL", cartTotal);
    lCartTotal.setText(tmp);   lCartTotal2.setText(tmp);
    snprintf(tmp, sizeof(tmp), "Items: %u", cartItems);
    lCartItems.setText(tmp);   lCartItems2.setText(tmp);
}

void renderHistory()
{
    char tmp[24];
    snprintf(tmp, sizeof(tmp), "Sales: %u", dailyCount);
    lDailyCount.setText(tmp);
    snprintf(tmp, sizeof(tmp), "Total: %lu TL", (unsigned long)dailySum);
    lDailySum.setText(tmp);
}

void addProduct(uint8_t i)
{
    if (i >= 6) return;
    cartTotal += PROD_PRICE[i];
    cartItems++;
    renderCart();
}

void onP1(void *p) { addProduct(0); }
void onP2(void *p) { addProduct(1); }
void onP3(void *p) { addProduct(2); }
void onP4(void *p) { addProduct(3); }
void onP5(void *p) { addProduct(4); }
void onP6(void *p) { addProduct(5); }

void onClear(void *p)
{
    cartTotal = 0; cartItems = 0;
    renderCart();
    lPayMsg.setText("");
}

void completeSale(const char *method)
{
    if (cartItems == 0) { lPayMsg.setText("Cart empty"); return; }
    dailyCount++;
    dailySum += cartTotal;
    char tmp[40];
    snprintf(tmp, sizeof(tmp), "%s: %u TL OK", method, cartTotal);
    lPayMsg.setText(tmp);
    cartTotal = 0; cartItems = 0;
    renderCart();
    renderHistory();
}

void onCash(void *p)   { completeSale("Cash"); }
void onCard(void *p)   { completeSale("Card"); }
void onCancel(void *p) { lPayMsg.setText("Cancelled"); }

void onResetDay(void *p) { dailyCount = 0; dailySum = 0; renderHistory(); }

AirTouch *air_listen_list[] = {
    &bP1, &bP2, &bP3, &bP4, &bP5, &bP6,
    &bClear, &bCash, &bCard, &bCancel, &bResetDay,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bP1.attachPop(onP1, &bP1); bP2.attachPop(onP2, &bP2);
    bP3.attachPop(onP3, &bP3); bP4.attachPop(onP4, &bP4);
    bP5.attachPop(onP5, &bP5); bP6.attachPop(onP6, &bP6);
    bClear.attachPop(onClear, &bClear);
    bCash.attachPop(onCash, &bCash);
    bCard.attachPop(onCard, &bCard);
    bCancel.attachPop(onCancel, &bCancel);
    bResetDay.attachPop(onResetDay, &bResetDay);

    /* Buton metinlerini fiyatlarla guncelle */
    char tmp[24];
    AirButton *btns[6] = { &bP1, &bP2, &bP3, &bP4, &bP5, &bP6 };
    for (uint8_t i = 0; i < 6; i++)
    {
        snprintf(tmp, sizeof(tmp), "%s\n%u TL", PROD_NAME[i], PROD_PRICE[i]);
        btns[i]->setText(tmp);
    }
    renderCart();
    renderHistory();
    lPayMsg.setText("");
}

void loop()
{
    airLoop(air_listen_list);
}
