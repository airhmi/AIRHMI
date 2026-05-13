# 23 - ATM Simulator

5 sayfali ATM simulasyonu. **AirProgressBar** dispense + **AirShape** kart slot/receipt.

## Sayfalar
1. **PIN** - 0-9 numpad + CLR + OK + AirShape (kart slot)
2. **Account** - Buyuk balance + son 4 islem listesi
3. **Withdraw** - 4 hizli tutar (50/100/200/500) + AirProgressBar dispense
4. **Transfer** - 3 kayitli hesap secimi + tutar slider + SEND
5. **Receipt** - Son islem ozeti + AirShape (kagit yer tutucu)

## Yeni componentler
- **AirProgressBar (pbDisp)**: cash dispenser progress (3 sn)
- **AirShape**: kart slot (Screen1) + receipt arka plan (Screen5)

## Kullanim
- PIN = "1234"
- Yanlis PIN -> buzz uzun
- Withdraw: amount > balance -> "Insufficient funds"
- Transfer: SEND -> History'e eklenir, balance dusulur

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 23_ATMSimulator
```
