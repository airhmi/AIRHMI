/**
 * 08_PrinterPanel  --  4 sayfali 3D yazici kontrol paneli
 *
 *   Screen1 Status   : nozzle/bed sicakligi, dosya adi, ilerleme
 *   Screen2 Print    : Start / Pause / Stop / Resume
 *   Screen3 Heaters  : nozzle/bed setpoint slider
 *   Screen4 Settings : filament tipi/uzunlugu, machine info
 */

#include <Airhmi.h>

/* Status */
AirLabel lNozzle = AirLabel("lNozzle");
AirLabel lBed    = AirLabel("lBed");
AirLabel lFile   = AirLabel("lFile");
AirProgressBar pbar = AirProgressBar("pbar");
AirLabel lProgress = AirLabel("lProgress");

/* Print */
AirButton bStart  = AirButton("bStart");
AirButton bPause  = AirButton("bPause");
AirButton bStop   = AirButton("bStop");
AirButton bResume = AirButton("bResume");
AirLabel  lState  = AirLabel("lState");

/* Heaters */
AirSlider sNozzle = AirSlider("sNozzle");
AirSlider sBed    = AirSlider("sBed");
AirLabel  lNzSp   = AirLabel("lNzSp");
AirLabel  lBdSp   = AirLabel("lBdSp");
AirButton bPreheat = AirButton("bPreheat");
AirButton bCooldown = AirButton("bCooldown");

/* Settings */
AirButton bMaterialPLA = AirButton("bMaterialPLA");
AirButton bMaterialPETG = AirButton("bMaterialPETG");
AirButton bMaterialABS = AirButton("bMaterialABS");
AirLabel  lMaterial = AirLabel("lMaterial");

uint16_t nzSp = 200, bdSp = 60;
float    nzCur = 25.0f, bdCur = 25.0f;
uint8_t  progress = 0;
bool     printing = false, paused = false;
const char *fileName = "demo.gcode";
const char *material = "PLA";

unsigned long lastTickMs = 0;
unsigned long lastTempMs = 0;

void renderTemps()
{
    char tmp[20];
    snprintf(tmp, sizeof(tmp), "%d / %u C", (int)nzCur, nzSp);
    lNozzle.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%d / %u C", (int)bdCur, bdSp);
    lBed.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u C", nzSp);  lNzSp.setText(tmp);
    snprintf(tmp, sizeof(tmp), "%u C", bdSp);  lBdSp.setText(tmp);
}

void renderProgress()
{
    pbar.Set_Value(progress);
    char tmp[12];
    snprintf(tmp, sizeof(tmp), "%u %%", progress);
    lProgress.setText(tmp);
}

void renderState()
{
    if      (printing && !paused) lState.setText("PRINTING");
    else if (printing && paused)  lState.setText("PAUSED");
    else                          lState.setText("IDLE");
}

void onStart(void *p)  { printing = true; paused = false; lastTickMs = millis(); renderState(); }
void onPause(void *p)  { paused = true; renderState(); }
void onResume(void *p) { paused = false; lastTickMs = millis(); renderState(); }
void onStop(void *p)   { printing = false; paused = false; progress = 0; renderProgress(); renderState(); }

void onNzSlider(void *p)
{
    uint32_t v = 0; sNozzle.Get_Value(&v);
    if (v > 280) v = 280;
    nzSp = (uint16_t)v;
    renderTemps();
}
void onBdSlider(void *p)
{
    uint32_t v = 0; sBed.Get_Value(&v);
    if (v > 110) v = 110;
    bdSp = (uint16_t)v;
    renderTemps();
}

void onPreheat(void *p)  { nzSp = 200; bdSp = 60;  sNozzle.Set_Value(nzSp); sBed.Set_Value(bdSp); renderTemps(); }
void onCooldown(void *p) { nzSp = 25;  bdSp = 25;  sNozzle.Set_Value(nzSp); sBed.Set_Value(bdSp); renderTemps(); }

void selectMaterial(const char *m, uint16_t nz, uint16_t bd)
{
    material = m;
    nzSp = nz; bdSp = bd;
    sNozzle.Set_Value(nz); sBed.Set_Value(bd);
    lMaterial.setText(m);
    renderTemps();
}

void onPLA (void *p) { selectMaterial("PLA",  200,  60); }
void onPETG(void *p) { selectMaterial("PETG", 230,  80); }
void onABS (void *p) { selectMaterial("ABS",  240, 100); }

AirTouch *air_listen_list[] = {
    &bStart, &bPause, &bStop, &bResume,
    &sNozzle, &sBed, &bPreheat, &bCooldown,
    &bMaterialPLA, &bMaterialPETG, &bMaterialABS,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bStart.attachPop(onStart, &bStart);
    bPause.attachPop(onPause, &bPause);
    bStop.attachPop(onStop, &bStop);
    bResume.attachPop(onResume, &bResume);
    sNozzle.attachPop(onNzSlider, &sNozzle);
    sBed.attachPop(onBdSlider, &sBed);
    bPreheat.attachPop(onPreheat, &bPreheat);
    bCooldown.attachPop(onCooldown, &bCooldown);
    bMaterialPLA.attachPop(onPLA, &bMaterialPLA);
    bMaterialPETG.attachPop(onPETG, &bMaterialPETG);
    bMaterialABS.attachPop(onABS, &bMaterialABS);

    sNozzle.Set_Value(nzSp); sBed.Set_Value(bdSp);
    lFile.setText(fileName);
    lMaterial.setText(material);
    renderTemps(); renderProgress(); renderState();
}

void loop()
{
    airLoop(air_listen_list);

    /* Print progress */
    if (printing && !paused && (millis() - lastTickMs >= 2000UL))
    {
        lastTickMs = millis();
        if (progress < 100) { progress++; renderProgress(); }
        else { printing = false; renderState(); }
    }

    /* Heater simülasyonu: setpoint'e dogru yumusak ilerle */
    if (millis() - lastTempMs >= 1000UL)
    {
        lastTempMs = millis();
        if (nzCur < nzSp) nzCur += 3.0f; if (nzCur > nzSp) nzCur = nzSp;
        if (bdCur < bdSp) bdCur += 1.5f; if (bdCur > bdSp) bdCur = bdSp;
        renderTemps();
    }
}
