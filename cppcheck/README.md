# Cppcheck

Cppcheck je statički analizator koda za C i C++.  
Omogućava identifikaciju logičkih grešaka, rizičnih konstrukcija, curenja memorije i potencijalnih problema u kodu bez njegovog izvršavanja.

## Pokretanje

Pokreni skriptu komandom:

```bash
chmod +x run_cppcheck.sh
./run_cppcheck.sh
```

## Rezultat

Rezultati se nalaze u direktorijumu `cppcheck-report`:

- Tekstualni izveštaj: `cppcheck-output.txt` sadrži listu svih detektovanih problema i njihov opis.

- HTML izveštaji: `index.html` i `stats.html` omogućavaju interaktivni pregled, filtriranje i statistički prikaz grešaka.

Ovi izveštaji omogućavaju brz pregled kritičnih delova koda i identifikaciju mesta koja zahtevaju pažnju.