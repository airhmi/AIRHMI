/**
 * Rotation.ino  --  AirPicture rotasyon yonetimi
 *
 *   Set/Get_rotationAngle       : derece (0..359)
 *   Set/Get_rotationCenterLeft  : pivot x noktasi
 *   Set/Get_rotationCenterTop   : pivot y noktasi
 *
 * Panel tarafi (Rotation.ahi):
 *   pDemo                       : test edilen ana resim
 *   bA0/bA45/bA90/bA180/bA270   : Set_rotationAngle(...)
 *   bCL0 / bCL50                : Set_rotationCenterLeft (sol/orta)
 *   bCT0 / bCT50                : Set_rotationCenterTop (ust/orta)
 *   bRead                       : 3 Get -> lAng / lCL / lCT
 *   bReset                      : default'a don
 */

#include <Airhmi.h>

/* ---------- HMI nesneleri ---------- */
AirPicture pDemo  = AirPicture("pDemo");
AirLabel   lAng   = AirLabel("lAng");
AirLabel   lCL    = AirLabel("lCL");
AirLabel   lCT    = AirLabel("lCT");

AirButton bA0    = AirButton("bA0");
AirButton bA45   = AirButton("bA45");
AirButton bA90   = AirButton("bA90");
AirButton bA180  = AirButton("bA180");
AirButton bA270  = AirButton("bA270");

AirButton bCL0   = AirButton("bCL0");
AirButton bCL50  = AirButton("bCL50");
AirButton bCT0   = AirButton("bCT0");
AirButton bCT50  = AirButton("bCT50");

AirButton bRead  = AirButton("bRead");
AirButton bReset = AirButton("bReset");

/* ---------- callback'ler ---------- */
void onA0(void *ptr)   { pDemo.Set_rotationAngle(0); }
void onA45(void *ptr)  { pDemo.Set_rotationAngle(45); }
void onA90(void *ptr)  { pDemo.Set_rotationAngle(90); }
void onA180(void *ptr) { pDemo.Set_rotationAngle(180); }
void onA270(void *ptr) { pDemo.Set_rotationAngle(270); }

void onCL0(void *ptr)  { pDemo.Set_rotationCenterLeft(0); }
void onCL50(void *ptr) { pDemo.Set_rotationCenterLeft(50); }
void onCT0(void *ptr)  { pDemo.Set_rotationCenterTop(0); }
void onCT50(void *ptr) { pDemo.Set_rotationCenterTop(50); }

void onRead(void *ptr)
{
    char tmp[12];
    uint32_t v = 0;

    pDemo.Get_rotationAngle(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v); lAng.setText(tmp);

    pDemo.Get_rotationCenterLeft(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v); lCL.setText(tmp);

    pDemo.Get_rotationCenterTop(&v);
    snprintf(tmp, sizeof(tmp), "%lu", (unsigned long)v); lCT.setText(tmp);
}

void onReset(void *ptr)
{
    pDemo.Set_rotationAngle(0);
    pDemo.Set_rotationCenterLeft(0);
    pDemo.Set_rotationCenterTop(0);
    lAng.setText("---");
    lCL.setText("---");
    lCT.setText("---");
}

/* ---------- HMI dinleyici listesi ---------- */
AirTouch *air_listen_list[] = {
    &bA0, &bA45, &bA90, &bA180, &bA270,
    &bCL0, &bCL50, &bCT0, &bCT50,
    &bRead, &bReset,
    &pDemo,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();

    bA0.attachPop(onA0,       &bA0);
    bA45.attachPop(onA45,     &bA45);
    bA90.attachPop(onA90,     &bA90);
    bA180.attachPop(onA180,   &bA180);
    bA270.attachPop(onA270,   &bA270);
    bCL0.attachPop(onCL0,     &bCL0);
    bCL50.attachPop(onCL50,   &bCL50);
    bCT0.attachPop(onCT0,     &bCT0);
    bCT50.attachPop(onCT50,   &bCT50);
    bRead.attachPop(onRead,   &bRead);
    bReset.attachPop(onReset, &bReset);
}

void loop()
{
    airLoop(air_listen_list);
}
