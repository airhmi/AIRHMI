# 20 - Stock Trading Terminal

5 sayfali borsa terminali. **AirProgressBar** ve **AirShape** componentlarinin ilk kullanildigi proje.

## Sayfalar
1. **Watchlist** - 5 hisse butonu (renk: yesil/kirmizi/gri trende gore) + day-range AirProgressBar + AirShape (dekoratif)
2. **Chart** - Secili hisse icin 60-bar trend AirGraph
3. **Portfolio** - 3 pozisyon (qty x sym, P/L) + total equity
4. **Order** - Sembol secimi (Prev/Next), miktar slider (1-100), BUY/SELL
5. **News** - Son 5 islem haberi

## Yeni componentler
- **AirProgressBar (pbDay)**: hissenin gunluk fiyat araligında (low..high) mevcut konum (0-100%)
- **AirShape (shDeco)**: dekoratif (Set_color/Set_visible olmadıgı için statik kullanılır)

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Mantik
- Her 1.5 sn rastgele fiyat hareketi (+/- 1.0)
- Day low/high otomatik guncellenir
- Fiyat dayMid'in uzerinde -> yesil, alti -> kirmizi
- BUY/SELL panel uzeri buzz + News'a kayit

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 20_StockTradingTerminal
```
