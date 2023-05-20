# Herbio - Ardiono design sheet

Scheme of wiring
![Scheme of wiring](model.png)

# Projekt



# Json aktualizácie



## CLI (Command Line Interface)

CLI (Command Line Interface) je v projekte Herbio použité na zjednodušenie a efektívnejšiu kontrolu nad zavlažovacím systémom. Umožňuje priamy prístup k špecifickým entitám v systéme a ich nastaveniam. Nižšie sú popísané jednotlivé príkazy a ich použitie.

### Príkazy

**Každý príkaz sa začína znakom `/`
`/exit` - Ukončuje príkazovú slučku pre ladenie. (Musí byť kompilácia s `#define DEBUG`)

### `/get`

> Používa sa na získanie informácií o špecifických entitách alebo celkových informácií o systéme.

**Options**

`<id>`    - Vypíše informácie o entite s daným ID.
`all`  - Vypíše informácie o všetkých entitách.
`time`  - Vypíše aktuálny čas.
`dump`  - Vypíše dump obsahu EEPROM.

*Examples*
`/get all`, `/get 21`, `/get time`, `/get dump ` ...

### `/set`

> Používa sa na nastavenie hodnôt entít v systéme.
>

**Options**

`time <TimeStampISO8601>` - Nastaví čas systému. Formát: `/set time YYYY-MM-DDThh:mm:ss`
`open <id>` - Otvorí ventil s daným ID. Formát: `/set open ID`
`close <id>` - Zavrie ventil s daným ID. Formát: `/set close ID`
`invalid` - Nastaví flag na neplatné v EEPROM pamäti.
`empty`    - Vyprázdni EEPROM pamäť.
`num <byte>`  - Nastaví hodnotu premennej setNum. Formát: `/set num VALUE`
`dump`  - Uloží súčasný stav entít do EEPROM.
Použitie
`/set time `, `/set open 11`, `/set close 11`, `/set invalid ` ...

