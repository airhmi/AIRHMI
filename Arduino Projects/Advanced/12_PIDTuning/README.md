# 12 — PID Tuning Demo ⭐

Donanım YOK; Arduino sketch içinde **basit 1. derece (sıcaklık benzeri) bir
proses simülasyonu** çalışıyor. Slider'larla Kp/Ki/Kd ve setpoint
değerleri ayarlanır, gerçek zamanlı PID kontrolcüsü prosesi sürer.

## Proses Modeli

```
err   = setpoint - pv
intErr += err * dt
dErr  = (err - lastErr) / dt
u     = clamp(Kp*err + Ki*intErr + Kd*dErr, 0, 100)
pv    = pv + dt * (a * u - b * pv)
```

dt=0.1s, a=1.5 (heater gain), b=0.05 (cooling). Adım `loop()`'ta her 100 ms.

## Slider Ölçekleri

| Slider | Ham aralık | PID değeri |
|---|---|---|
| sKp | 0..100 | Kp = ham / 10 (0..10) |
| sKi | 0..200 | Ki = ham / 100 (0..2) |
| sKd | 0..100 | Kd = ham / 100 (0..1) |
| sSp | 0..200 | setpoint doğrudan |

## Component

| Nesne | Tur |
|---|---|
| `sKp`, `sKi`, `sKd`, `sSp` | EveSlider × 4 |
| `gPv`, `lPv` | EveGauge + ELabelBox process value |
| `lSp`, `lOut` | setpoint + PID çıkış (%) |
| `gph` | EGraph 2 line (sarı=SP, kırmızı=PV) |
| `bStep` | setpoint'i 150'e atla (step input testi) |
| `bReset` | pv = 0, integrator clear |

## Gözlem Önerisi

1. Kp=2.5, Ki=Kd=0 ile başla, P-only davranış
2. Yavaş yavaş Ki ekle, steady-state error sıfırlanır
3. Aşma artarsa Kd ekle (damping)
4. Step Apply ile setpoint değişimine yanıtı izle

![1](1.png)
