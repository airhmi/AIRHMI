# 10 — Button → GPIO LED

4 toggle butonu ile 4 GPIO cikisi ve onlarin gorsel "LED" karsiligi.

## Calisma Akisi

- 4 push-pull benzeri toggle button (`bL0..bL3`) — basinca `ledState[i]`
  ters cevirilir
- `apply(i)` cagirilir:
  - `dLi` (gosterge button) `Set_background_color` ile yesil/siyah arasi
  - `gpio.digital_write(i, 0|1)` ile fiziksel GPIO cikisi de degistirilir
- Acilista 4 LED de "off" (siyah)

## Kullanilan Component'lar

| Nesne | Tur | Islev |
|---|---|---|
| `bL0..bL3` | EButton | dokunmatik toggle |
| `dL0..dL3` | EButton (Active=False) | LED gorseli |

## Donanim

GPIO_0..GPIO_3 cikislarina LED + sinirlayici direnc baglayin. Buton'a
basildiginda hem panel'deki gorsel LED yanar/soner hem de fiziksel pin
HIGH/LOW olur.

![1](1.png)
