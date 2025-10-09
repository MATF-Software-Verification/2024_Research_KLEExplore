# Valgrind – Massif

Massif je alat za profilisanje memorije u C/C++ aplikacijama.  
Njegova primarna funkcija je praćenje upotrebe heap memorije tokom izvršavanja programa.  
Omogućava identifikaciju mesta u kodu gde dolazi do prekomerne alokacije ili potencijalnih problema sa memorijom.

## Pokretanje

Pokreni skriptu:

```bash
chmod +x run_massif.sh
./run_massif.sh
```

## Rezultat

Rezultati alata nalaze se u fajlovima koji su oblika `massif_out/massif.out.<PID>`.
Mogu se pregledati koristeći ```ms_print```:

```bash
ms_print massif_out/massif.out.<PID>
```
ili grafički pomoću ```Massif Visualizer```:

```bash
massif-visualizer massif_out/massif.out.<PID>
```