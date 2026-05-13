# 04 — Cafe POS (4 sayfa) ⭐

Donanımsız çoklu-ekran kafe POS demo. 4 sayfa: Menu / Cart / Pay / History.

## Sayfalar

| Screen | İçerik |
|---|---|
| 1 Menu | 6 ürün butonu (Espresso/Latte/Mocha/Tea/Cookie/Cake) + Items / Total |
| 2 Cart | Sepet özeti büyük metin + Clear Cart |
| 3 Pay  | CASH / CARD / Cancel + son işlem mesajı |
| 4 History | Günlük satış sayısı + günlük gelir + Reset Day |

## Ürün Tablosu

| # | Ürün | Fiyat (TL) |
|---|---|---|
| 1 | Espresso | 30 |
| 2 | Latte    | 50 |
| 3 | Mocha    | 55 |
| 4 | Tea      | 25 |
| 5 | Cookie   | 20 |
| 6 | Cake     | 45 |

## Mantık

- Ürün butonuna tıkla → `cartTotal += price`, `cartItems++`
- CASH/CARD → `dailyCount++`, `dailySum += cartTotal`, sepet sıfırlanır,
  ödeme mesajı yazılır
- Cancel → mesaj "Cancelled", sepet aynen kalır
- Clear Cart → sepet sıfırlanır
- Reset Day → `dailyCount/dailySum` sıfırlanır

![1](1.png)
