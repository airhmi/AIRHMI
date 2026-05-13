/**
 * 09_PomodoroTimer  --  Pomodoro tekniği: 25 dk calisma + 5 dk mola
 *
 *   Klasik Pomodoro dongusu: 25 dk WORK -> 5 dk BREAK -> 25 dk WORK ...
 *   Her durum bittiginde buzzer cinlar. Gunluk tamamlanan pomodoro
 *   sayisi panel'deki AirVariable'da tutulur (kalici), Reset Day ile
 *   sifirlanir.
 *
 *   Live tick yok; UI sadece dakika bazinda guncellenir (UART trafigi
 *   minimum, Stop/Reset event'leri guvenli).
 *
 * Panel tarafi (09_PomodoroTimer.ahi):
 *   lPhase     : ELabelBox -- "WORK" / "BREAK" / "IDLE"
 *   lTime      : ELabelBox -- "MM:SS"
 *   lCount     : ELabelBox -- "Today: N"
 *   pbar       : EveProgressBar (faz icindeki ilerleme)
 *   bStart / bStop / bResetDay
 *   buz        : AirBuzzer
 *   count      : EVariable (panel-side, kalici sayim)
 */

#include <Airhmi.h>

AirLabel  lPhase = AirLabel("lPhase");
AirLabel  lTime  = AirLabel("lTime");
AirLabel  lCount = AirLabel("lCount");
AirProgressBar pbar = AirProgressBar("pbar");

AirButton bStart    = AirButton("bStart");
AirButton bStop     = AirButton("bStop");
AirButton bResetDay = AirButton("bResetDay");

AirBuzzer    buz   = AirBuzzer("buz");
AirVariable  count = AirVariable("count");

enum Phase { IDLE, WORK, BREAK };
Phase phase = IDLE;
const unsigned long WORK_MS  = 25UL * 60UL * 1000UL;
const unsigned long BREAK_MS =  5UL * 60UL * 1000UL;
unsigned long phaseStart = 0;
unsigned long lastUiUpdate = 0;
char lastShown[8] = "";
int  todayCount = 0;

void renderPhase()
{
    if      (phase == WORK)  { lPhase.setText("WORK");  }
    else if (phase == BREAK) { lPhase.setText("BREAK"); }
    else                     { lPhase.setText("IDLE");  }
}

void renderTime(unsigned long elapsedMs, unsigned long phaseTotalMs)
{
    unsigned long remainMs = (elapsedMs >= phaseTotalMs) ? 0 : phaseTotalMs - elapsedMs;
    unsigned long s = remainMs / 1000UL;
    unsigned long mm = s / 60UL;
    unsigned long ss = s % 60UL;
    char tmp[8];
    snprintf(tmp, sizeof(tmp), "%02lu:%02lu", mm, ss);
    if (strcmp(tmp, lastShown) != 0)
    {
        strcpy(lastShown, tmp);
        lTime.setText(tmp);
    }

    /* progress: 0..100 */
    uint32_t pct = (uint32_t)((elapsedMs * 100UL) / phaseTotalMs);
    if (pct > 100) pct = 100;
    pbar.Set_Value(pct);
}

void renderCount()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "Today: %d", todayCount);
    lCount.setText(tmp);
}

void onStart(void *p)
{
    if (phase != IDLE) return;
    phase = WORK;
    phaseStart = millis();
    lastShown[0] = 0;
    renderPhase();
    pbar.Set_Value(0);
}

void onStop(void *p)
{
    phase = IDLE;
    renderPhase();
    pbar.Set_Value(0);
    lTime.setText("00:00");
    lastShown[0] = 0;
}

void onResetDay(void *p)
{
    todayCount = 0;
    count.VarSeti(0);
    renderCount();
}

AirTouch *air_listen_list[] = { &bStart, &bStop, &bResetDay, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    bStart.attachPop(onStart, &bStart);
    bStop.attachPop(onStop, &bStop);
    bResetDay.attachPop(onResetDay, &bResetDay);
    renderPhase();
    lTime.setText("25:00");
    renderCount();
    pbar.Set_Value(0);
}

void loop()
{
    airLoop(air_listen_list);

    if (phase == IDLE) return;

    unsigned long now = millis();
    unsigned long elapsed = now - phaseStart;
    unsigned long total = (phase == WORK) ? WORK_MS : BREAK_MS;

    /* faz bitti mi */
    if (elapsed >= total)
    {
        if (phase == WORK)
        {
            todayCount++;
            count.VarSeti(todayCount);
            renderCount();
            phase = BREAK;
        }
        else
        {
            phase = WORK;
        }
        phaseStart = millis();
        lastShown[0] = 0;
        renderPhase();
        buz.Set_Buzzer(800);
        return;
    }

    /* UI 1 sn'de bir */
    if (now - lastUiUpdate >= 1000UL)
    {
        lastUiUpdate = now;
        renderTime(elapsed, total);
    }
}
