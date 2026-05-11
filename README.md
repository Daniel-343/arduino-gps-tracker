# arduino-gps-tracker

## A Projekt Leírása
A projekt egy Proof of Concept, amelynek célja egy valós idejű GPS nyomkövető hardveres és szoftveres integrációjának bemutatása. A rendszer begyűjti a helyadatokat, majd azokat mobilhálózaton keresztül továbbítja egy Traccar szerverre.

## Hardver Komponensek
Az eszköz az alábbi elemekből épül fel:\
**Központi mikrokontroller:** Arduino MKR NB 1500. Ez az alaplap felel a program futtatásáért és a mobilhálózati kommunikációért a beépített u-blox SARA modul illetve egy iot sim segítségével.\
**GPS modul:** Arduino MKR GPS Shield.\
**Adatkapcsolat:** A mikrokontroller és a GPS Shield közötti kommunikáció egy I2C kábelen keresztül történik.\
**Tápellátás:** Egy 3.7V-os Li-Po akkumulátor csatlakozik közvetlenül a MKR alaplaphoz. Erre a u-blox SARA mobilhálózati modul működése során fellépő hirtelen feszültségtüskék stabilizálása miatt van feltétlenül szükség, megelőzve az újraindulásokat.\
**Antenna:** Egy külső antenna csatlakozik a központi egységhez a megfelelő minőségű mobilhálózati jel biztosítása érdekében.

<img width="4080" height="3072" alt="IMG_20260511_162020572" src="https://github.com/user-attachments/assets/f6b33571-65a8-495e-ad80-c602bf2b83a3" />


## Szoftveres Megvalósítás
A mikrokontroller vezérlőszoftvere C++ (Arduino) nyelven íródott, és az alábbi folyamatokat hajtja végre:  
**Függőségek:** A kód az Arduino_MKRGPS.h és az MKRNB.h könyvtárakat használja a szenzor és a hálózat kezelésére.  
**Inicializálás:** Indításkor a rendszer I2C buszon keresztül felébreszti a GPS modult. Ezt követően megkísérel felcsatlakozni a mobilhálózatra az iot.secure APN (hozzáférési pont) használatával. A folyamat addig várakozik, amíg a GPRS kapcsolat sikeresen fel nem épül.  
**Fő ciklus (Loop):** A rendszer 10 másodpercenként (10000 ms) ellenőrzi a pozíciót. Ez a frissítési gyakoriság elegendő ahhoz, hogy a modemet folyamatosan ébren tartsa.  
**Adatfeldolgozás:** Amikor a GPS sikeresen befogja a műholdakat, a program kiolvassa a szélességi (latitude) és hosszúsági (longitude) fokokat , majd diagnosztikai célból a csatlakozott műholdak számával együtt kiírja a soros monitorra.  

<img width="395" height="192" alt="Screenshot 2026-05-11 164442" src="https://github.com/user-attachments/assets/ed996188-457d-4f67-b889-be74c714e244" />

## Szerver és Adatátvitel
Az adatok fogadását és térképes megjelenítését egy Traccar demo szerver végzi.  
**Szerver és Port:** demo3.traccar.org szerver, 5055.  
**Eszköz azonosító:** A nyomkövető a 69-es Device ID-vel azonosítja magát a rendszerben.  
**Hálózati protokoll:** Az adatok továbbítása HTTP GET kéréssel történik. A generált URL tartalmazza az azonosítót (id), valamint a 6 tizedesjegy pontosságú szélességi (lat) és hosszúsági (lon) koordinátákat.  
**Visszajelzés:** Az adatküldést követően a kapcsolat lezárul, a panel beépített LED-je rövid villogással jelzi a sikeres ciklust.  

<img width="725" height="658" alt="Screenshot 2026-05-11 165804" src="https://github.com/user-attachments/assets/ae033be4-1da4-4f77-a0ca-8b20277360de" />


## Összegzés
A fenti felépítéssel a projekt sikeresen demonstrálja egy IoT alapú nyomkövető működését. A hardver képes a GPS adatok stabil kiolvasására, a tápellátás az akkumulátor segítségével kiegyensúlyozott, a hálózati réteg pedig megbízhatóan kézbesíti a paramétereket a felhőalapú platform számára.
