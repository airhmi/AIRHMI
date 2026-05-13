# Arduino Projects — AirHMI ile Mini HMI Projeleri

`Arduino Examples/` klasöründe her component için temel kullanım örnekleri
yer alır. Bu klasör ise birden çok component'ı birleştirerek **gerçek bir
senaryo** üreten projeleri toplar.

Üç seviye var:

- **Basic/** — tek bir konuya odaklanan, hızlı sonuç veren küçük projeler
  (sensör/donanım gerektirmez ya da en fazla potansiyometre/LED yeter)
- **Advanced/** — çoklu sensör + uzun-soluklu mantık + grafik/log barındıran
  kapsamlı projeler (tek ekran)
- **Expert/** — birden çok ekran (panel-side `ChangeScreenSet` ile geçiş),
  endüstriyel/IoT senaryoları, state machine + kalıcı veri (AirVariable)

Her proje klasörü:

```
NN_<Name>/
├── <Name>.ino     Arduino sketch
├── <Name>.ahi     AirHMI panel projesi
├── 1.png          Çalışan ekran görüntüsü
└── README.md      Proje dokümantasyonu
```

## 🟢 Basic

> Hepsi UNO için derlenir (tüm 10 sketch `arduino-cli compile` ile geçti,
> %16-29 flash kullanım). Panel'e yüklenip canlı test edildikçe ekran
> görüntüleri (`1.png`) eklenir.

- [x] **01_TrafficLight** — Trafik ışığı simülasyonu (auto/manuel mod)
      &nbsp;&nbsp;_AirButton × 4, AirLabel_
- [x] **02_LedDimmer** — PWM LED dimmer (slider ile %)
      &nbsp;&nbsp;_AirSlider, AirGpio (PWM), AirButton, AirLabel_
- [x] **03_Countdown** — Geri sayım sayacı + buzzer alarm
      &nbsp;&nbsp;_AirSlider, AirButton × 2, AirLabel × 2, AirBuzzer_
- [x] **04_Stopwatch** — Kronometre (mm:ss.cc)
      &nbsp;&nbsp;_AirLabel, AirButton × 3_
- [x] **05_ReflexGame** — Reaksiyon süresi oyunu
      &nbsp;&nbsp;_AirButton × 2, AirLabel × 2_
- [x] **06_Calculator** — Mini hesap makinesi (4-işlem)
      &nbsp;&nbsp;_AirButton × 17, AirLabel_
- [x] **07_PotVoltmeter** — Potansiyometre ile voltmetre göstergesi
      &nbsp;&nbsp;_AirGpio (analog_read), AirGauge, AirLabel_
- [x] **08_DigitalClock** — Dijital saat
      &nbsp;&nbsp;_AirRtc, AirLabel × 2, AirButton × 3_
- [x] **09_BuzzerPiano** — Buzzer piyano (do-re-mi-fa-sol-la-si)
      &nbsp;&nbsp;_AirButton × 7, AirBuzzer_
- [x] **10_ButtonLed** — 4 buton → 4 GPIO LED eşleşmesi
      &nbsp;&nbsp;_AirButton × 8 (4 toggle + 4 gösterge), AirGpio (digital_write)_

## 🔵 Advanced

> ⭐ = Donanım gerektirmez (sadece panel + Arduino).
> Diğerleri sensör / aktüatör ister (DHT22, DS18B20, MQ135, INA219,
> HC-SR04, servo, röle vb.).

**Veri görselleştirme**
- [x] **01_TempHumidStation** — Sıcaklık-Nem istasyonu (DHT22 + 24h grafik)
      &nbsp;&nbsp;_DHT22, AirGauge × 2, AirGraph, AirLabel × 2, AirRtc_
- [x] **02_AirQuality** — Hava kalitesi monitörü
      &nbsp;&nbsp;_MQ135, AirGauge, AirGraph, AirBuzzer, AirShape (alarm renkli)_
- [x] **03_VoltCurrentMonitor** — Voltaj/akım monitörü + alarm limit
      &nbsp;&nbsp;_INA219 (I2C), AirGauge × 2, AirSlider, AirBuzzer_

**Kontrol / Otomasyon**
- [x] **04_SmartThermostat** — Akıllı termostat (program edilebilir)
      &nbsp;&nbsp;_DS18B20, AirSlider, AirGpio (röle), AirRtc, AirBuzzer, AirLabel_
- [x] ⭐ **05_RGBMoodLight** — RGB ledli mood light (PWM 3 kanal)
      &nbsp;&nbsp;_AirSlider × 3, AirShape (preview), AirGpio (PWM × 3), AirButton (preset)_
- [x] **06_RelayScheduler** — 4-kanal röle + RTC zamanlayıcı
      &nbsp;&nbsp;_AirButton × 4, AirShape × 4 (gösterge), AirGpio, AirRtc, AirLabel_
- [x] **07_ServoPanel** — 4 servo kontrol + preset
      &nbsp;&nbsp;_4× SG90 servo, AirSlider × 4, AirButton (preset), AirGauge feedback_
- [x] **08_WaterTank** — Su tankı seviye + pompa kontrolü
      &nbsp;&nbsp;_HC-SR04, AirProgressBar, AirGraph, AirGpio (röle), AirBuzzer_

**Üretkenlik**
- [x] ⭐ **09_PomodoroTimer** — 25/5 dk Pomodoro + günlük sayaç
      &nbsp;&nbsp;_AirProgressBar, AirButton, AirLabel × 3, AirBuzzer, AirVariable, AirRtc_
- [x] ⭐ **10_MultiAlarmClock** — 5 slot programlanabilir alarm saati
      &nbsp;&nbsp;_AirRtc, AirVariable × 5, AirButton × 5, AirBuzzer, AirLabel grid_

**Eğlence / Demo**
- [x] ⭐ **11_MusicRecorder** — 8 nota kaydet/oynat
      &nbsp;&nbsp;_AirButton × 10 (8 nota + Play + Clear), AirBuzzer, AirVariable kayıt_
- [x] ⭐ **12_PIDTuning** — PID parametre ayarlama demo
      &nbsp;&nbsp;_AirSlider × 3 (Kp/Ki/Kd), AirGraph, AirGauge, AirButton (step input)_
- [x] ⭐ **13_ReactionMultiplayer** — 2-oyunculu reaksiyon yarışı
      &nbsp;&nbsp;_AirShape × 2, AirButton × 2, AirLabel skor, AirVariable best score_

İlerledikçe `[ ]` → `[x]` olarak işaretlenir.

## 🔴 Expert

> Çoklu ekranlı (3-5 sayfa) projeler. Sayfalar arası geçiş `AirPage`
> sınıfı veya panel-side `ChangeScreenSet` script ile yapılır.

- [x] **01_SmartHomeHub** — 5 sayfa: Dashboard / Aydınlatma / Klima / Güvenlik / Ayarlar
- [x] **02_IndustrialControl** — 5 sayfa: Dashboard / Reçete / Alarmlar / Trend / Ayarlar
- [x] **03_CNCPanel** — 4 sayfa: Jog / Otomatik / Ayarlar / Diagnostics
- [x] **04_CafePOS** ⭐ — 4 sayfa: Menü / Sepet / Ödeme / Geçmiş
- [x] **05_GreenhouseManager** — 4 sayfa: Durum / Kontrol / Program / Trend
- [x] **06_HomeSecurity** — 4 sayfa: Durum / Zone'lar / PIN Giriş / Olay Logu
- [x] **07_AquariumManager** — 4 sayfa: Durum / Besleme / Aydınlatma / Ayarlar
- [x] **08_PrinterPanel** — 4 sayfa: Status / Print / Sıcaklık / Ayarlar
- [x] **09_HVACMultiZone** — 4 sayfa: Dashboard / Zone1 / Zone2 / Program
- [x] **10_PomodoroTaskManager** ⭐ — 3 sayfa: Timer / Görev Listesi / İstatistik
- [x] **11_ElevatorController** ⭐ — 5 sayfa: Cabin / Floors / Diag / Maintenance / Log (state machine + cagri kuyrugu)
- [x] **12_EVChargingStation** ⭐ — 5 sayfa: Status / Charge / Pricing / Sessions / Diag (CCS-vari state machine + kWh integral)
- [x] **13_SCADAFactoryLine** ⭐ — 5 sayfa: Overview / Stations / OEE / Alarms / Recipes (4 istasyon + OEE hesabi + recete)
- [x] **14_SolarInverter** ⭐ — 5 sayfa: Live / Battery / Grid / History / Settings (mock MPPT + batarya integrali + grid balans)
- [x] **15_ShipBridge** ⭐ — 5 sayfa: Helm / Engine / Nav / Radar / Alarms (autopilot heading + waypoint nav + MOB)
- [x] **16_MedicalMonitor** ⭐ — 5 sayfa: Vitals / ECG / Alarms / Trend / Patient (vital signs + cok-esikli alarm)
- [x] **17_BrewingController** ⭐ — 5 sayfa: Mash / Boil / Ferment / Recipe / Log (cok-asamali sicaklik profili + recete)
- [x] **18_RailwaySignaling** ⭐ — 5 sayfa: Track / Trains / Signals / Interlock / Log (4-block ring + interlock kurallari)
- [x] **19_DroneGCS** ⭐ — 5 sayfa: Map / Telemetry / Mission / Camera / Settings (waypoint nav + battery failsafe + RTH)
- [x] **20_StockTradingTerminal** ⭐ — 5 sayfa: Watchlist / Chart / Portfolio / Order / News (**AirProgressBar** day-range + **AirShape**)
- [x] **21_MusicPlayer** ⭐ — 5 sayfa: NowPlaying / Playlist / Library / Equalizer / Settings (**AirProgressBar** track + 5 band EQ)
- [x] **22_GymTracker** ⭐ — 5 sayfa: Today / Exercise / Timer / Stats / Profile (**AirProgressBar** rest + **AirShape** badge)
- [x] **23_ATMSimulator** ⭐ — 5 sayfa: PIN / Account / Withdraw / Transfer / Receipt (numpad + **AirProgressBar** dispense + **AirShape** kart)
- [x] **24_FlightDeck** ⭐ — 5 sayfa: PFD / Nav / Engines / Autopilot / Comms (4 AirGauge + **AirProgressBar** fuel + **AirShape**)

## 🚀 Başlangıç

Her proje bağımsızdır; bir tanesini açıp Arduino IDE / `arduino-cli` ile
derleyip yükleyebilir, panel projesini de AirHMI Visual Screen Creator
ile panele yükleyebilirsiniz. Detaylar her projenin kendi `README.md`
dosyasında.

Genel kullanım kılavuzu için: [`../Arduino Examples/README.md`](../Arduino%20Examples/README.md)
