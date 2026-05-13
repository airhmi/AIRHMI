/**
 * 09_BuzzerPiano  --  Buzzer ile basit oktav (do re mi fa sol la si)
 *
 *   AirHMI'nin buzzer'i sadece sure parametresi alir (frekans sabittir,
 *   panel kart datasheet'ine bakin). Bu yuzden gercek bir piyano degil,
 *   her buton farkli surede beep cikartiyor (kisa = ince, uzun = kalin
 *   gibi tini farki).
 *
 * Panel tarafi (BuzzerPiano.ahi):
 *   bDo / bRe / bMi / bFa / bSol / bLa / bSi  : EButton x 7
 */

#include <Airhmi.h>

AirBuzzer buz = AirBuzzer("buz");

AirButton bDo  = AirButton("bDo");
AirButton bRe  = AirButton("bRe");
AirButton bMi  = AirButton("bMi");
AirButton bFa  = AirButton("bFa");
AirButton bSol = AirButton("bSol");
AirButton bLa  = AirButton("bLa");
AirButton bSi  = AirButton("bSi");

void onDo (void *p){ buz.Set_Buzzer(60);  }
void onRe (void *p){ buz.Set_Buzzer(80);  }
void onMi (void *p){ buz.Set_Buzzer(110); }
void onFa (void *p){ buz.Set_Buzzer(150); }
void onSol(void *p){ buz.Set_Buzzer(200); }
void onLa (void *p){ buz.Set_Buzzer(280); }
void onSi (void *p){ buz.Set_Buzzer(380); }

AirTouch *air_listen_list[] = {
    &bDo, &bRe, &bMi, &bFa, &bSol, &bLa, &bSi,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bDo.attachPop (onDo,  &bDo);
    bRe.attachPop (onRe,  &bRe);
    bMi.attachPop (onMi,  &bMi);
    bFa.attachPop (onFa,  &bFa);
    bSol.attachPop(onSol, &bSol);
    bLa.attachPop (onLa,  &bLa);
    bSi.attachPop (onSi,  &bSi);
}

void loop()
{
    airLoop(air_listen_list);
}
