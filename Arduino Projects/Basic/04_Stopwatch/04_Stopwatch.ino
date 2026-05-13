/**
 * 04_Stopwatch  --  Kronometre (mm:ss live tick)
 *
 *   Arduino loop'unda millis() ile elapsed hesaplanir, her saniyede bir
 *   lTime "MM:SS" olarak guncellenir.
 *
 *   Library tarafindaki "pending touch event" buffer (AirHardware.cpp)
 *   recvRet sirasinda gelen 0x65 event'lerini yakalar; bizde de her
 *   setText cagrisindan SONRA `airLoop(air_listen_list)` cagirarak pending
 *   event'leri hemen dispatch ediyoruz. Bu sayede live tick sirasinda
 *   Stop/Reset olaylari kaybolmaz.
 *
 * Panel tarafi (04_Stopwatch.ahi):
 *   bStart / bStop / bReset
 *   lTime  : ELabelBox -- "MM:SS"
 */

#include <Airhmi.h>

AirButton bStart = AirButton("bStart");
AirButton bStop  = AirButton("bStop");
AirButton bReset = AirButton("bReset");
AirLabel  lTime  = AirLabel("lTime");

extern AirTouch *air_listen_list[];   /* asagida tanimli */

bool running = false;
unsigned long startMs   = 0;
unsigned long elapsedMs = 0;
unsigned long lastUiMs  = 0;
char lastShown[12] = "";

void writeTime(const char *tmp)
{
    lTime.setText(tmp);
    airLoop(air_listen_list);   /* pending event'leri hemen dispatch et */
}

void renderTime(unsigned long ms)
{
    unsigned long total_cs = ms / 10UL;            /* 1/100 sn */
    unsigned long mm = total_cs / 6000UL;
    unsigned long ss = (total_cs / 100UL) % 60UL;
    unsigned long cc =  total_cs % 100UL;
    char tmp[12];
    snprintf(tmp, sizeof(tmp), "%02lu:%02lu.%02lu", mm, ss, cc);
    if (strcmp(tmp, lastShown) != 0)
    {
        strcpy(lastShown, tmp);
        writeTime(tmp);
    }
}

void onStart(void *p)
{
    if (running) return;
    startMs = millis() - elapsedMs;
    running = true;
}

void onStop(void *p)
{
    if (!running) return;
    elapsedMs = millis() - startMs;
    running   = false;
    renderTime(elapsedMs);
}

void onReset(void *p)
{
    running   = false;
    elapsedMs = 0;
    lastShown[0] = 0;
    renderTime(0);
}

AirTouch *air_listen_list[] = { &bStart, &bStop, &bReset, NULL };

void setup()
{
    Serial.begin(115200);
    airInit();
    bStart.attachPop(onStart, &bStart);
    bStop.attachPop(onStop,   &bStop);
    bReset.attachPop(onReset, &bReset);
    renderTime(0);
}

void loop()
{
    airLoop(air_listen_list);

    if (running)
    {
        unsigned long now = millis();
        if (now - lastUiMs >= 200UL)
        {
            lastUiMs = now;
            renderTime(now - startMs);
        }
    }
}
