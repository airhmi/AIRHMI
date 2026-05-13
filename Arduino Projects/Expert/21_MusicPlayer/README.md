# 21 - Music Player

5 sayfali muzik calar. **AirProgressBar** track ilerleme + **AirShape** album cover, 5 band EQ.

## Sayfalar
1. **Now Playing** - Track adi, sanatci, sure, AirProgressBar (track%), PREV/PLAY/NEXT
2. **Playlist** - 6 track butonu (secili olan yesil)
3. **Library** - 4 album dolas, AirShape (album cover yerine), Prev/Next
4. **Equalizer** - 5 band slider (Bass/LowMid/Mid/HighMid/Treble) renkli, 0..10
5. **Settings** - Volume slider (0-100), SHUFFLE/REPEAT toggle butonlari

## Yeni componentler
- **AirProgressBar (pbTrack)**: anlik track pozisyonu / sure
- **AirShape (shCover)**: dekoratif album cover yer tutucusu

## Donanim
| Pin | Modul       | Aciklama                      |
|-----|-------------|-------------------------------|
| TX/RX | Panel UART | 115200 baud                   |
| -   | (mock)     | Donanimsiz, simulator yeterli |

## Mantik
- Playing iken her saniye trackPos++
- Track bitince: repeat=on -> basa al, shuffle=on -> rastgele, normal -> sirayla
- Track gecisi panel uzeri buzz

## Build
```
arduino-cli compile --fqbn arduino:avr:uno 21_MusicPlayer
```
