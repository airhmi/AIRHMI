# 05 — Greenhouse Manager (4 sayfa)

DHT22 + toprak nem (A0) + LDR (A1) okuyup; fan/heat/water/light rölelerini
panel GPIO 0..3 üzerinden manuel veya auto kontrol eder. 4 sayfa: Status /
Control / Program / Trend.

## Donanım

| | |
|---|---|
| DHT22 DATA | D2 |
| Soil moisture analog | A0 |
| LDR (light) analog | A1 |
| Fan röle | Panel GPIO_0 |
| Heat röle | Panel GPIO_1 |
| Water röle | Panel GPIO_2 |
| Light röle | Panel GPIO_3 |

Kütüphane: **DHT sensor library** (Adafruit).

## Auto Mode

- Sıcaklık > 30°C → fan ON
- Sıcaklık < 18°C → heat ON
- Soil moisture < %30 → water ON

Manuel toggle herhangi bir röleyi tıkladığında otomatik mod kapanır.

![1](1.png)
