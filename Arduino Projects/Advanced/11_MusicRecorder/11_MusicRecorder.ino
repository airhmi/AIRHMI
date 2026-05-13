/**
 * 11_MusicRecorder  --  8 nota kaydet/oynat
 *
 *   8 piyano tusu (DO..DO+1) basildikca AirBuzzer ile cinlar VE
 *   kaydediliyor moddaysa nota dizisine eklenir. Play butonuna basinca
 *   kayitli nota dizisi sirayla calinir. Clear ile temizlenir.
 *   Kayit gostergesi (lStatus) "REC" / "PLAY" / "IDLE" goruntuler.
 *
 *   Kayit kapasitesi: 32 nota (her biri uint8_t indeks 0..7).
 *
 * Panel tarafi (11_MusicRecorder.ahi):
 *   bDo bRe bMi bFa bSol bLa bSi bDo2  (8 tus)
 *   bRec bPlay bClear  (kontrol)
 *   lStatus            (durum)
 *   lCount             (kayitli nota sayisi)
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
AirButton bDo2 = AirButton("bDo2");

AirButton bRec   = AirButton("bRec");
AirButton bPlay  = AirButton("bPlay");
AirButton bClear = AirButton("bClear");

AirLabel  lStatus = AirLabel("lStatus");
AirLabel  lCount  = AirLabel("lCount");

const uint16_t NOTE_DUR[8] = { 60, 80, 110, 150, 200, 280, 380, 500 };
const uint8_t  MAX_NOTES   = 32;
uint8_t  notes[MAX_NOTES];
uint8_t  noteCount = 0;
bool     recording = false;

void renderCount()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u/%u", noteCount, (unsigned)MAX_NOTES);
    lCount.setText(tmp);
}

void playNote(uint8_t idx)
{
    if (idx >= 8) return;
    buz.Set_Buzzer(NOTE_DUR[idx]);
    if (recording && noteCount < MAX_NOTES)
    {
        notes[noteCount++] = idx;
        renderCount();
    }
}

void onDo (void *p) { playNote(0); }
void onRe (void *p) { playNote(1); }
void onMi (void *p) { playNote(2); }
void onFa (void *p) { playNote(3); }
void onSol(void *p) { playNote(4); }
void onLa (void *p) { playNote(5); }
void onSi (void *p) { playNote(6); }
void onDo2(void *p) { playNote(7); }

void onRec(void *p)
{
    recording = !recording;
    lStatus.setText(recording ? "REC" : "IDLE");
}

void onPlay(void *p)
{
    if (recording) { recording = false; lStatus.setText("IDLE"); }
    if (noteCount == 0) return;
    lStatus.setText("PLAY");
    for (uint8_t i = 0; i < noteCount; i++)
    {
        buz.Set_Buzzer(NOTE_DUR[notes[i]]);
        delay(NOTE_DUR[notes[i]] + 80);
    }
    lStatus.setText("IDLE");
}

void onClear(void *p)
{
    noteCount = 0;
    recording = false;
    renderCount();
    lStatus.setText("IDLE");
}

AirTouch *air_listen_list[] = {
    &bDo, &bRe, &bMi, &bFa, &bSol, &bLa, &bSi, &bDo2,
    &bRec, &bPlay, &bClear,
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
    bDo2.attachPop(onDo2, &bDo2);
    bRec.attachPop  (onRec,   &bRec);
    bPlay.attachPop (onPlay,  &bPlay);
    bClear.attachPop(onClear, &bClear);
    lStatus.setText("IDLE");
    renderCount();
}

void loop()
{
    airLoop(air_listen_list);
}
