/**
 * 21_MusicPlayer  --  5 sayfali muzik calar
 *
 *   Screen1 NowPlaying: track adi, sanatci, AirProgressBar (track ilerleme)
 *   Screen2 Playlist  : 6 track listesi (panel butonu, secili olan yesil)
 *   Screen3 Library   : 4 album (Prev/Next ile dolas)
 *   Screen4 Equalizer : 5 band slider (Bass / LowMid / Mid / HighMid / Treble)
 *   Screen5 Settings  : volume slider, shuffle/repeat toggle butonlari
 *
 *   Yeni component: AirProgressBar (track progress).
 */

#include <Airhmi.h>

/* NowPlaying */
AirLabel  lTrack = AirLabel("lTrack");
AirLabel  lArtist = AirLabel("lArtist");
AirLabel  lTime = AirLabel("lTime");
AirProgressBar pbTrack = AirProgressBar("pbTrack");
AirButton bPlay = AirButton("bPlay");
AirButton bPrev = AirButton("bPrev");
AirButton bNext = AirButton("bNext");

/* Playlist */
AirButton bT0 = AirButton("bT0");
AirButton bT1 = AirButton("bT1");
AirButton bT2 = AirButton("bT2");
AirButton bT3 = AirButton("bT3");
AirButton bT4 = AirButton("bT4");
AirButton bT5 = AirButton("bT5");

/* Library */
AirLabel  lAlbum = AirLabel("lAlbum");
AirLabel  lAlbumArtist = AirLabel("lAlbumArtist");
AirButton bAlbPrev = AirButton("bAlbPrev");
AirButton bAlbNext = AirButton("bAlbNext");

/* Equalizer */
AirSlider sEq0 = AirSlider("sEq0");
AirSlider sEq1 = AirSlider("sEq1");
AirSlider sEq2 = AirSlider("sEq2");
AirSlider sEq3 = AirSlider("sEq3");
AirSlider sEq4 = AirSlider("sEq4");
AirLabel  lEqInfo = AirLabel("lEqInfo");

/* Settings */
AirSlider sVol = AirSlider("sVol");
AirLabel  lVol = AirLabel("lVol");
AirButton bShuffle = AirButton("bShuffle");
AirButton bRepeat = AirButton("bRepeat");

AirBuzzer buz = AirBuzzer("buz");

const uint8_t TRACK_COUNT = 6;
const char *trackName[TRACK_COUNT] = {
    "Bohemian Rhapsody", "Stairway to Heaven", "Hotel California",
    "Imagine", "Smells Like Teen Spirit", "Sweet Child O Mine"
};
const char *artistName[TRACK_COUNT] = {
    "Queen", "Led Zeppelin", "Eagles", "John Lennon", "Nirvana", "Guns N Roses"
};
const uint16_t trackLen[TRACK_COUNT] = { 354, 482, 391, 183, 301, 356 };

const uint8_t ALBUM_COUNT = 4;
const char *albumName[ALBUM_COUNT] = { "Greatest Hits", "Live Concert", "Studio 80s", "Acoustic" };
const char *albumArtist[ALBUM_COUNT] = { "Various", "Queen", "Various", "Eagles" };

uint8_t curTrack = 0;
uint8_t curAlbum = 0;
bool playing = false;
uint16_t trackPos = 0;
uint8_t volume = 70;
bool shuffle = false;
bool repeat = false;
uint8_t eqVal[5] = { 5, 5, 5, 5, 5 };

unsigned long lastTick = 0;

const uint32_t COL_OFF = 0xFF202020UL;
const uint32_t COL_ON = 0xFF008000UL;
const uint32_t COL_BLUE = 0xFF0000A0UL;

void renderNowPlaying()
{
    lTrack.setText(trackName[curTrack]);
    lArtist.setText(artistName[curTrack]);
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u:%02u / %u:%02u",
        trackPos/60, trackPos%60,
        trackLen[curTrack]/60, trackLen[curTrack]%60);
    lTime.setText(tmp);
    uint8_t pct = (uint8_t)((uint32_t)trackPos * 100 / trackLen[curTrack]);
    pbTrack.Set_Value(pct);
    bPlay.setText(playing ? "PAUSE" : "PLAY");
    bPlay.Set_background_color(playing ? COL_ON : COL_BLUE);
}

void renderPlaylist()
{
    AirButton *btns[TRACK_COUNT] = { &bT0, &bT1, &bT2, &bT3, &bT4, &bT5 };
    for (uint8_t i = 0; i < TRACK_COUNT; i++) {
        btns[i]->setText(trackName[i]);
        btns[i]->Set_background_color(i == curTrack ? COL_ON : COL_OFF);
    }
}

void renderLibrary()
{
    lAlbum.setText(albumName[curAlbum]);
    lAlbumArtist.setText(albumArtist[curAlbum]);
}

void renderEqInfo()
{
    char tmp[24];
    snprintf(tmp, sizeof(tmp), "B%u LM%u M%u HM%u T%u",
        eqVal[0], eqVal[1], eqVal[2], eqVal[3], eqVal[4]);
    lEqInfo.setText(tmp);
}

void renderSettings()
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u %%", volume);
    lVol.setText(tmp);
    bShuffle.setText(shuffle ? "SHUFFLE ON" : "SHUFFLE OFF");
    bShuffle.Set_background_color(shuffle ? COL_ON : COL_OFF);
    bRepeat.setText(repeat ? "REPEAT ON" : "REPEAT OFF");
    bRepeat.Set_background_color(repeat ? COL_ON : COL_OFF);
}

void selectTrack(uint8_t i)
{
    curTrack = i;
    trackPos = 0;
    renderNowPlaying();
    renderPlaylist();
}

void onPlay(void *p) { playing = !playing; renderNowPlaying(); buz.Set_Buzzer(40); }
void onPrev(void *p) { selectTrack(curTrack==0 ? TRACK_COUNT-1 : curTrack-1); }
void onNext(void *p) { selectTrack((curTrack+1) % TRACK_COUNT); }

void onT0(void *p) { selectTrack(0); }
void onT1(void *p) { selectTrack(1); }
void onT2(void *p) { selectTrack(2); }
void onT3(void *p) { selectTrack(3); }
void onT4(void *p) { selectTrack(4); }
void onT5(void *p) { selectTrack(5); }

void onAlbPrev(void *p) { if (curAlbum==0) curAlbum=ALBUM_COUNT-1; else curAlbum--; renderLibrary(); }
void onAlbNext(void *p) { curAlbum = (curAlbum+1) % ALBUM_COUNT; renderLibrary(); }

void onEq0(void *p) { uint32_t v=0; sEq0.Get_Value(&v); eqVal[0]=v; renderEqInfo(); }
void onEq1(void *p) { uint32_t v=0; sEq1.Get_Value(&v); eqVal[1]=v; renderEqInfo(); }
void onEq2(void *p) { uint32_t v=0; sEq2.Get_Value(&v); eqVal[2]=v; renderEqInfo(); }
void onEq3(void *p) { uint32_t v=0; sEq3.Get_Value(&v); eqVal[3]=v; renderEqInfo(); }
void onEq4(void *p) { uint32_t v=0; sEq4.Get_Value(&v); eqVal[4]=v; renderEqInfo(); }

void onVol(void *p)
{
    uint32_t v = 0; sVol.Get_Value(&v);
    if (v > 100) v = 100;
    volume = v;
    renderSettings();
}
void onShuffle(void *p) { shuffle = !shuffle; renderSettings(); }
void onRepeat(void *p) { repeat = !repeat; renderSettings(); }

AirTouch *air_listen_list[] = {
    &bPlay, &bPrev, &bNext,
    &bT0, &bT1, &bT2, &bT3, &bT4, &bT5,
    &bAlbPrev, &bAlbNext,
    &sEq0, &sEq1, &sEq2, &sEq3, &sEq4,
    &sVol, &bShuffle, &bRepeat,
    NULL
};

void setup()
{
    Serial.begin(115200);
    airInit();
    bPlay.attachPop(onPlay, &bPlay);
    bPrev.attachPop(onPrev, &bPrev);
    bNext.attachPop(onNext, &bNext);
    bT0.attachPop(onT0,&bT0); bT1.attachPop(onT1,&bT1);
    bT2.attachPop(onT2,&bT2); bT3.attachPop(onT3,&bT3);
    bT4.attachPop(onT4,&bT4); bT5.attachPop(onT5,&bT5);
    bAlbPrev.attachPop(onAlbPrev, &bAlbPrev);
    bAlbNext.attachPop(onAlbNext, &bAlbNext);
    sEq0.attachPop(onEq0,&sEq0); sEq1.attachPop(onEq1,&sEq1);
    sEq2.attachPop(onEq2,&sEq2); sEq3.attachPop(onEq3,&sEq3);
    sEq4.attachPop(onEq4,&sEq4);
    sVol.attachPop(onVol, &sVol);
    bShuffle.attachPop(onShuffle, &bShuffle);
    bRepeat.attachPop(onRepeat, &bRepeat);

    sVol.Set_Value(volume);
    for (uint8_t i = 0; i < 5; i++) {
        if (i==0) sEq0.Set_Value(eqVal[i]);
        else if (i==1) sEq1.Set_Value(eqVal[i]);
        else if (i==2) sEq2.Set_Value(eqVal[i]);
        else if (i==3) sEq3.Set_Value(eqVal[i]);
        else sEq4.Set_Value(eqVal[i]);
    }
    pbTrack.Set_Value(0);
    renderNowPlaying();
    renderPlaylist();
    renderLibrary();
    renderEqInfo();
    renderSettings();
}

void loop()
{
    airLoop(air_listen_list);

    if (millis() - lastTick < 1000UL) return;
    lastTick = millis();

    if (playing) {
        trackPos++;
        if (trackPos >= trackLen[curTrack]) {
            if (repeat) trackPos = 0;
            else {
                uint8_t next = shuffle ? random(TRACK_COUNT) : (curTrack + 1) % TRACK_COUNT;
                selectTrack(next);
                buz.Set_Buzzer(80);
            }
        }
        renderNowPlaying();
    }
}
