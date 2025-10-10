# Analiza projekta Alat za analitiku

## Autor
- **Ime i prezime:** Jelisaveta Gavrilović  
- **Broj indeksa:** 1028/2024  

---

## Informacije o projektu

Predmet ove analize verifikacije softvera je projekat **[Alat za analitiku](https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2023-2024/alat-za-analitiku)**.  

Analiza je sprovedena nad granom `main` i commit-om  
[2f803aed](https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2023-2024/alat-za-analitiku/-/commit/2f803aed95026681789b7905e8ddceb2b4e815e9).

Cilj analize je bio da se, korišćenjem alata za verifikaciju softvera:

- identifikuju potencijalni problemi u kodu (curenje memorije, neefikasnosti),
- pruže smernice za optimizaciju performansi,
- unapredi stil i čitljivost koda,
- obezbedi standardizacija u skladu sa dobrim praksama razvoja softvera.

---

## Opis analiziranog projekta

**Alat za analitiku** je softverski sistem koji omogućava generisanje izveštaja, statistika i vizualizacija nad podacima.  
Podaci se učitavaju iz datoteka, a po potrebi se vrši konverzija tipova.  
Korisniku su dostupne sledeće funkcionalnosti:

- prikaz različitih grafika (raspodele, bar plot-ovi, analize zavisnosti podataka),  
- štampanje i čuvanje izveštaja.  

Projekat se može posmatrati kao pojednostavljena verzija alata poput PowerBI-ja, namenjena lokalnoj analizi i vizualizaciji podataka.

Projekat je razvijen u programskom jeziku **C++** uz korišćenje **Qt6** frameworka.  

---

## Korišćeni alati

Za sprovođenje analize korišćeni su sledeći alati:

- **Valgrind – Memcheck** → detekcija curenja memorije,  
- **Valgrind – Massif** → praćenje potrošnje memorije,  
- **Clang-Format** → automatsko formatiranje koda,  
- **Cppcheck** → statička analiza koda.

Svaki alat ima svoj direktorijum sa rezultatima analize i pratećim skriptama za pokretanje.

Organizacija:

    ├── clang-format/    # Rezultati i skripta za formatiranje

    ├── cppcheck/        # Rezultati i skripta za statičku analizu

    └── valgrind/

        ├──  massif/     # Rezultati i skripta za Massif
   
        └── memcheck/    # Rezultati i skripta za Memcheck  
        
  
Detaljna uputstva za pokretanje skripti nalaze se unutar odgovarajućih direktorijuma.

- ### Instalacija alata (Linux)

Na Linux sistemima, potrebni alati se mogu instalirati komandom:

```bash
sudo apt update && sudo apt install -y valgrind clang-format cppcheck
```

---

## Zaključak

Primena različitih alata za verifikaciju i analizu koda omogućila je sveobuhvatan uvid u kvalitet i pouzdanost projekta **Alat za analitiku**.  
Korišćenjem dinamičkih alata poput **Valgrind-a** detektovani su problemi u radu sa memorijom, uključujući curenje i neinicijalizovane pokazivače.  
Alat **Clang-Format** obezbedio je konzistentnost u stilu i formatiranju koda, čime je poboljšana čitljivost i održivost projekta.  
Na kraju, statička analiza korišćenjem **Cppcheck-a** omogućila je prepoznavanje logičkih grešaka, potencijalno rizičnih konstrukcija i problema u strukturi koda, čak i pre faze kompajliranja.

Kombinovanjem ovih pristupa ostvarena je dublja verifikacija softverskog sistema — od dinamičkog ponašanja i efikasnosti memorije, preko standardizacije stila, do statičke provere logike koda.  
Ovaj proces doprinosi većem kvalitetu i stabilnosti aplikacije, kao i stvaranju osnove za dalje unapređenje procesa razvoja i testiranja softvera kroz automatizaciju i kontinuiranu integraciju.

Detaljniji rezultati, analize i tumačenja nalaze se u dokumentu **`ProjectAnalysisReport.pdf`**.


