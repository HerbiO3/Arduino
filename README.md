# Herbio - Ardiono design sheet

Scheme of wiring
![Scheme of wiring](model.png)

# Projekt

# Json aktualizácie

## CLI (Command Line Interface)

CLI (Command Line Interface) je v projekte Herbio použité na zjednodušenie a efektívnejšiu kontrolu nad zavlažovacím systémom. Umožňuje priamy prístup k špecifickým entitám v systéme a ich nastaveniam. Nižšie sú popísané jednotlivé príkazy a ich použitie.

Každý príkaz vráti číslo vykonaného príkazu

```c++
CMD_failed 0
CMD_get_all 1
CMD_get_time 2
CMD_get_dump 3
CMD_get_id 4
CMD_set_time 5
CMD_set_open 6
CMD_set_close 6
CMD_set_invalid 7
CMD_set_empty 8
CMD_set_num 9
CMD_set_dump 10
CMD_set_from_init 11
CMD_set_from_eeprom 12
CMD_updateJson 13
CMD_exit 99
```

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

# Model

## Entity

> Rodičovská trieda pre entity v systéme. Každá entita obsahuje:

* `id`		- unikátny identifikátor (byte),
* `name`		- názov,
* `toJson()`	- metódu na serializovanie,
* `update()`	- metodu na deserializovanie (aktualizacia premennlivých hodnôt),
* `dump()`	- metodu na uloženie svojho stavu do buffru
* `load()`	- metodu na načítanie svojho stavu z buffru
* `size()`	- vráti veľkosť triedy

### Senzory a Aktívne entity

* `MoistureSensor`		- Zisťuje vlhkosť z prostredia
* `Pump`				- Pumpa, zabezpečuje prítok pre ventily;
* `Tank`				- Abstrkcia nad senzorom merania hladiny vody.
  * Ak sú zadané `dist_full` a `dist_empty` tak vracia %, inak vracia absolutne cm
* ~~`TemperatureSensor`~~	- nezapojené
* `Valve`			- ventil, obsahuje `open()`, `close()` otvorí/zatvorí ventil a pumpu.
  * Pozn. Pumpa sa zavrie až keď všetky pumpy sú vypnuté

## Section (entity)

Hlavná ovládacia entita, ktorá rozhoduje kedy sa bude polievať.

#### módy:

`mode` - podľa akého módu sa sekcia správa

* auto		- zalievanie sa spustí ak vlhkosť klesne pod `min_humidity`

  * `min_humidity` : minimálna vlhkosť
* timed	- zalievanie sa spustí podľa RTC;

  * `water_start` - čas kedy sa má začať polievať,
  * `water_next`   - čas ďalšieho zalievanie
  * `water_time`  - dĺžka zalievanie
* manual	- zalievanie na požiadanie, ak príde update, ktorí zapne `water_now` = `true`

  * `water_now`	- spúšťacia premenná na manuálne zalievanie

Pomocné premenné
* `water_until`   - čas dokedy sa má polievať (`water_start`+`water_time`)
* `water_watered` - signalizuje či v jednej perióde už bolo zalievanie spustené


#### Vzťahy
* Valve* valve	      		- Sekcia má priradenú práve jeden ventil
* MoistSensor* moisture 	- Sekcia má priradený práve jeden senzor vlhkosti
