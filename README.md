# Covjece, ne ljuti se (LUDO)
Implementacija klasicne drustvene igre za 1-4 igraca u C++ s Raylib grafickim suceljem.

## Ovisnosti
**Raylib - staticki linkan, ne treba instalacija**

Build alati:
```bash
# Linux
sudo apt install build-essential libgl1-mesa-dev libx11-dev

# Windows -  Visual Studio
```

### Windows - Visual Studio
1. Otvori projekt
2. Project Properties > C/C++ > Additional Include: `./include`
3. Linker > Additional Library: `./lib/windows`
4. Linker > Input: dodaj `raylib.lib; opengl32.lib; gdi32.lib; winmm.lib;`
5. Build i pokreni



## Kontrole u igri
#### Pocetak
- Klikni broj ljudskih igraca
- Odaberi boju za ljudske igrace

#### Igra:
- Klikni "BACI KOCKICU" ili pritisni SPACE/ENTER na tipkovnici
- Klikni na figuru za micanje
- Dobio/la si 6: odaberi novu figuru ili pomakni postojecu 

#### Racunalni igraci
Racunalo automatski igra svoje poteze prema pravilima.