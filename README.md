# 🎮 Shadow Of The Cave 2

## 📖 Descrierea Proiectului

**Shadow of the Cave** este un joc de supraviețuere top-down 2D creat în C++ folosind biblioteca SFML. Jucătorul trebuie să supraviețuiască valurilor nesfârșite de inamici, folosind diverse arme și abilități achiziționate cu monede câștigate din luptă.

### 🎯 Conceptul Jocului

Ești un erou blocat într-o peșteră întunecată, înconjurat de hoardele de inamici. Trebuie să supraviețuiești cât mai mult timp posibil, să acumulezi monede din fiecare inamic ucis și să folosești sistemul de magazin pentru a cumpăra upgrade-uri și consumabile care te vor ajuta în luptă.

### ✨ Caracteristici Principale

#### 🗡️ **Sistem de Arme Diversificat**
- **Arme Melee** - Combat de apropiere cu damage mare
- **Arme Ranged** - Trage proiectile către inamici
- **Grenade** - Explozive aruncabile cu damage pe zonă
- **Magic Weapon** - Damage instant la toți inamicii pe ecran

#### 👾 **Tipuri de Inamici**
- **Melee Enemies** - Inamici rapizi care atacă de aproape
- **Ranged Enemies** - Inamici care trag proiectile de la distanță
- Fiecare tip are textură unică, comportament distinct și statistici diferite

#### 💰 **Sistem de Magazin și Inventar**
- Câștigi **monede** pentru fiecare inamic ucis
- Poți cumpăra:
  - **Grenade (50 monede)** - Explozive puternice
  - **Health Potions (25 monede)** - Regenerează 20 HP
  - **Damage Boost (40 monede)** - +10% damage permanent la toate armele (stackable)
  - **Magic Damage (75 monede)** - Consumabil care dă 50 damage tuturor inamicilor

#### 🎮 **Controale**

**Mișcare:**
- `W/A/S/D` sau `Arrow Keys` - Mișcare în 4 direcții

**Atacuri:**
- `Left Click` - Atac Ranged / Aruncă Grenadă (dacă ai în inventar)
- `Right Click` - Atac Melee

**Magazin (Cumpără):**
- `1` - Cumpără Grenadă (50 monede)
- `2` - Cumpără Health Potion (25 monede)
- `3` - Cumpără Damage Boost (40 monede)
- `4` - Cumpără Magic Damage (75 monede)

**Consumabile (Folosește):**
- `P` - Folosește Health Potion
- `L` - Folosește Magic Damage

#### 📊 **HUD (Heads-Up Display)**
- **Health Bar** - Afișează viața curentă a jucătorului
- **Timer** - Timpul de supraviețuire
- **Coins** - Monedele acumulate
- **Inventory** - Numărul de potiuni, grenade și magic damage
- **Shop Menu** - Meniu permanent cu prețuri și taste de cumpărare/utilizare
- **Damage Flash** - Ecranul devine roșu când primești damage

### 🏗️ Arhitectura Proiectului

#### 📂 Structura Fișierelor

```
Shadow-Of-The-Cave-2/
├── header/              # Fișiere header (.h)
│   ├── Game.h          # Clasa principală a jocului
│   ├── Player.h        # Jucătorul (Singleton)
│   ├── Entity.h        # Clasa de bază pentru entități
│   ├── Enemy.h         # Clasa de bază pentru inamici
│   ├── MeleeEnemy.h    # Inamic melee
│   ├── RangedEnemy.h   # Inamic ranged
│   ├── Weapon.h        # Clasa de bază pentru arme
│   ├── MeleeWeapon.h   # Armă melee
│   ├── RangedWeapon.h  # Armă ranged
│   ├── ThrowableWeapon.h # Grenade
│   ├── MagicWeapon.h   # Armă magică
│   ├── Projectile.h    # Proiectile jucătorului
│   ├── EnemyProjectile.h # Proiectile inamicilor
│   ├── Level.h         # Hartă și mediu
│   ├── HUD.h           # Interfață utilizator
│   ├── Inventory.h     # Sistem de inventar
│   ├── HealthPotion.h  # Potiune de viață
│   └── GameException.h # Ierarhie de excepții
├── source/             # Implementări (.cpp)
├── assets/             # Texturi și fonturi
├── CMakeLists.txt      # Configurare CMake
└── main.cpp            # Entry point
```

#### 🔧 Concepte OOP Implementate

##### **1. Moștenire și Polimorfism**
```
Entity (clasă de bază abstractă)
├── Player (Singleton)
└── Enemy (clasă de bază)
    ├── MeleeEnemy
    └── RangedEnemy

Weapon (clasă de bază abstractă)
├── MeleeWeapon
├── RangedWeapon
├── ThrowableWeapon
└── MagicWeapon
```

##### **2. Design Patterns**
- **Singleton Pattern** - Clasa `Player` (o singură instanță)
- **Factory Pattern** - Crearea inamicilor (Melee/Ranged) în funcție de tip
- **Strategy Pattern** - Comportamente diferite pentru tipuri de arme
- **Observer Pattern** - HUD-ul observă starea jucătorului și inventarului

##### **3. Funcții Virtuale**
- `Entity::update()` - Actualizare entități (pure virtual)
- `Entity::render()` - Randare entități (pure virtual)
- `Entity::takeDamage()` - Primire damage (virtual)
- `Weapon::clone()` - Virtual copy constructor (pure virtual)
- `Weapon::applyUpgrade()` - Aplicare upgrade-uri (pure virtual)

##### **4. Ierarhie de Excepții**
```
std::exception
└── GameException (bază custom)
    ├── GameConfigException
    ├── AssetLoadException
    └── InvalidStatException
```

##### **5. Smart Pointers**
- `std::unique_ptr<Enemy>` - Gestionarea inamicilor
- `std::unique_ptr<Weapon>` - Gestionarea armelor
- `std::unique_ptr<ThrowableWeapon>` - Grenade active

##### **6. STL Usage**
- `std::vector<>` - Liste de entități, proiectile
- `std::string` - Nume arme, texte
- `std::unique_ptr<>` - Smart pointers
- `std::make_unique<>` - Creare smart pointers

##### **7. Rule of Three/Five**
- Clasa `Weapon` implementează:
  - Copy constructor
  - Copy assignment operator
  - Destructor (virtual)

##### **8. Composition**
- `Game` conține: `Player`, `Level`, `HUD`, `Inventory`, vectori de `Enemy`
- `Player` conține: `RangedWeapon`, `MeleeWeapon`, `ThrowableWeapon`, `MagicWeapon`
- `Enemy` conține: `std::unique_ptr<Weapon>`

##### **9. Encapsulare**
- Atribute `private`/`protected`
- Metode `const` unde este cazul
- Getteri/setteri pentru acces controlat

### 🎨 Mecanici de Gameplay

#### **Spawn System**
- Inamicii apar la intervale regulate în afara ecranului
- 50% șansă pentru Melee, 50% pentru Ranged
- Nu se spawneaza prea aproape de jucător (min 600px)

#### **Combat System**
- **Melee Combat** - Atac rapid de apropiere
- **Ranged Combat** - Proiectile cu viteză și damage configurabile
- **Grenade** - Explozii cu radius de damage
- **Magic Damage** - AOE (Area of Effect) instant

#### **Upgrade System**
- **Damage Boost** - Permanent, stackable, +10% la fiecare cumpărare
- Afectează toate armele (Melee, Ranged)

#### **Health System**
- Jucătorul începe cu 100 HP
- Poate fi vindecat cu potiuni (+20 HP)
- Nu poate depăși HP-ul maxim
- Visual feedback (ecran roșu) când primește damage

#### **Coin Economy**
- 10 coins pe kill
- Inamicii devin mai puternici în timp

### 🔨 Tehnologii Utilizate

- **Limbaj:** C++17
- **Biblioteca Grafică:** SFML 3.0 (Simple and Fast Multimedia Library)
- **Build System:** CMake
- **CI/CD:** GitHub Actions
- **Compiler:** GCC/Clang/MSVC

### 📋 Cerințe Tema POO

#### ✅ Tema 1 - Bază POO
- ✅ **4+ clase** cu compunere: `Game`, `Player`, `Weapon`, `Level`, `HUD`, `Inventory`
- ✅ **Constructori cu parametri** pentru toate clasele
- ✅ **Rule of Three** pentru `Weapon`: copy constructor, copy assignment, destructor
- ✅ **operator<<** pentru toate clasele cu compunere de apeluri
- ✅ **const** și **private** extensiv folosite
- ✅ **Funcții membre complexe**: 
  - `Game::checkCollisions()` - Detectare coliziuni
  - `Player::updateMovement()` - Sistem de mișcare cu normalizare vectorială
  - `Enemy::updateMovementEnemy()` - AI pentru urmărirea jucătorului
  - `Weapon::update()` - Rotație armă către mouse
- ✅ **Scenariu complet** în `main.cpp` cu try-catch pentru excepții
- ✅ **CI/CD** cu GitHub Actions
- ✅ Tag: `v0.1`

#### ✅ Tema 2 - Moșteniri și Polimorfism
- ✅ **Separare .h/.cpp** pentru toate clasele
- ✅ **Ierarhii de moștenire:**
  - `Entity` (bază) → `Player`, `Enemy` → `MeleeEnemy`, `RangedEnemy`
  - `Weapon` (bază) → `MeleeWeapon`, `RangedWeapon`, `ThrowableWeapon`, `MagicWeapon`
- ✅ **Funcții virtuale pure:**
  - `Entity::update()`, `Entity::render()`
  - `Weapon::clone()`, `Weapon::applyUpgrade()`
- ✅ **Funcții virtuale specifice temei:**
  - `Entity::takeDamage()` - Sistem de damage polimorfic
  - `Weapon::applyUpgrade()` - Upgrade-uri diferite pentru fiecare armă
- ✅ **Virtual constructor (clone)** pentru toate armele
- ✅ **Copy-and-swap** în `Weapon`
- ✅ **dynamic_cast** pentru `ThrowableWeapon` în `Game::handleGrenadeThrow()`
- ✅ **Smart pointers:** `std::unique_ptr<Enemy>`, `std::unique_ptr<Weapon>`
- ✅ **Ierarhie excepții:**
  - `GameException` → `GameConfigException`, `AssetLoadException`, `InvalidStatException`
- ✅ **Funcții/atribute static:** `Player::getInstance()` (Singleton)
- ✅ **STL:** `std::vector`, `std::unique_ptr`, `std::string`, `std::make_unique`
- ✅ **Clasă derivată nouă:** `RangedEnemy` adăugată după `MeleeEnemy`
- ✅ Tag: `v0.2`

#### ✅ Tema 3 - Design Patterns și Template-uri

##### **4 Design Patterns Implementate (2 cerute)**

1. **Singleton Pattern** - `Player::getInstance()`
   - Locație: `Player.h:13`, `Player.cpp:102-105`
   - Utilizare: Acces global la jucător din inamici (`Enemy.cpp:~90, ~118`)
   - Implementare: Meyer's Singleton cu static local variable

2. **Factory Pattern** - `Game::spawnOneEnemy()`
   - Locație: `Game.cpp:447-482`
   - Crează dinamic `MeleeEnemy` sau `RangedEnemy` pe bază de tip
   - Polimorfism prin `std::unique_ptr<Enemy>`
   - Randomizare 50% Melee, 50% Ranged

3. **Strategy Pattern** - Ierarhia `Weapon`
   - Locație: `Weapon.h` (interfață), `MeleeWeapon.h`, `RangedWeapon.h`, `ThrowableWeapon.h`, `MagicWeapon.h`
   - Player conține 4 strategii diferite (membrii: `mRangedWeapon`, `mMeleeWeapon`, `mThrowableWeapon`, `mMagicWeapon`)
   - Enemy folosește `std::unique_ptr<Weapon>` polimorfic

4. **Observer Pattern** - `HUD` observă starea jocului
   - Locație: `HUD.cpp:58-105`, `Game.cpp:191`
   - HUD observă: `Player` health, `Inventory` state, coins
   - Reactiv la schimbări: damage flash, inventory updates

##### **Clasă Template - Pool<T> (2 instanțieri)**

**Fișier:** `header/Pool.h` (87 linii)

```cpp
template<typename T>
class Pool {
    T acquire();                    // Obține obiect din pool (reuse)
    void release(T&& obj);          // Returnează obiect la pool
    size_t getAvailableCount() const;
    size_t getActiveCount() const;
};
```

**Instanțieri în cod:**
- **Instanțiere 1:** `Pool<Projectile>` (Game.h:79)
  - Locație: `header/Game.h:79`, `source/Game.cpp:22`
  - Pre-alocă 20 proiectile jucătorului
  - Reduce memory fragmentation în joc

- **Instanțiere 2:** `Pool<EnemyProjectile>` (Game.h:80)
  - Locație: `header/Game.h:80`, `source/Game.cpp:23`
  - Pre-alocă 30 proiectile inamici
  - Optimizează performance la mulți inamici

**Concepte demonstrate:**
- Generic programming cu templates
- RAII (Resource Acquisition Is Initialization)
- Memory pooling pattern
- Factory functions cu std::function

##### **Funcții Template - MathHelper (11+ instanțieri)**

**Fișier:** `header/MathHelper.h` (121 linii)

**6 Funcții Template disponibile:**
```cpp
namespace MathHelper {
    template<typename T> T clamp(T value, T min, T max);
    template<typename T> T distance(const sf::Vector2<T>&, const sf::Vector2<T>&);
    template<typename T> sf::Vector2<T> normalize(const sf::Vector2<T>&);
    template<typename T> T lerp(T a, T b, float t);
    template<typename T> const T& max(const T&, const T&);
    template<typename T> const T& min(const T&, const T&);
}
```

**11+ Instanțieri în cod:**

| # | Tip | Fișier | Linie | Utilizare |
|---|-----|--------|-------|-----------|
| 1 | `clamp<float>` | Player.cpp | 119 | Damage health clamping |
| 2 | `clamp<float>` | Player.cpp | 129 | Heal health clamping |
| 3 | `distance<float>` | Enemy.cpp | 221 | AI distance calculation |
| 4 | `normalize<float>` | Enemy.cpp | 230 | Melee movement normalization |
| 5 | `normalize<float>` | Enemy.cpp | 241 | Ranged movement normalization |
| 6 | `clamp<int>` | Inventory.cpp | 12 | Add health potions |
| 7 | `clamp<int>` | Inventory.cpp | 18 | Remove health potions |
| 8 | `clamp<int>` | Inventory.cpp | 24 | Add grenades |
| 9 | `clamp<int>` | Inventory.cpp | 30 | Remove grenades |
| 10 | `clamp<int>` | Inventory.cpp | 36 | Add damage potions |
| 11 | `clamp<int>` | Inventory.cpp | 42 | Remove damage potions |

**Concepte demonstrate:**
- Function templates cu type deduction
- Static assertions pentru type checking (`std::is_arithmetic<T>`)
- Template specialization (sf::Vector2<T>)
- Multiple instantiations cu diferiți tipuri
- Type traits și SFINAE

##### **Integrare în Gameplay**

1. **Pool<T> Benefits:**
   - Pre-alocă projectiles = mai puțin lag
   - Reuse objects = menos memory allocations
   - Console output verifies initialization

2. **MathHelper Benefits:**
   - Health nu iese din bounds (0-100)
   - Inventory items clamped la 0-99
   - Vector math consistent și type-safe


### 📊 Statistici Proiect

- **Linii de cod:** ~3000+ linii C++
- **Clase:** 20+ clase
- **Fișiere header:** 15+
- **Fișiere sursă:** 15+
- **Design patterns:** 4+
- **Ierarhii de moștenire:** 2 majore + 1 excepții
- **Procent C++:** ~85%

## 🚀 Instrucțiuni de Compilare și Rulare

### Cerințe de Sistem

**Minime:**
- Compilator C++17 (GCC 9+, Clang 10+, MSVC 2019+)
- CMake 3.16+
- SFML 3.0 (descărcat automat de CMake)
- Sistem de operare: Linux, Windows, macOS

### Compilare

Proiectul este configurat cu CMake și se compilează folosind următorii pași:

#### **1. Configurare (pasul inițial)**
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# sau folosind scriptul helper
./scripts/cmake.sh configure
```

Pentru **Windows cu GCC** (folosind Git Bash):
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# sau
./scripts/cmake.sh configure -g Ninja
```

#### **2. Compilare**
```bash
cmake --build build --config Debug --parallel 6
# sau
./scripts/cmake.sh build
```

Opțiunea `--parallel 6` compilează 6 fișiere în paralel (ajustați după numărul de core-uri).

#### **3. Instalare (opțional)**
```bash
cmake --install build --config Debug --prefix install_dir
# sau
./scripts/cmake.sh install
```

### Rulare

După compilare, executabilul se găsește în:
- **Linux/Mac:** `build/oop`
- **Windows:** `build/oop.exe`

Rulați din folderul rădăcină al proiectului pentru ca texturile din `assets/` să fie găsite:
```bash
cd /path/to/Shadow-Of-The-Cave-2
./build/oop
```

### Structura după Build

```
Shadow-Of-The-Cave-2/
├── build/           # Fișiere generate (ignorat de git)
│   ├── oop          # Executabil
│   └── ...
├── install_dir/     # Instalare (opțional, ignorat de git)
├── assets/          # Texturi, fonturi (NECESAR pentru rulare)
│   ├── player.png
│   ├── enemy.png
│   ├── rifle.png
│   ├── grenade.png
│   └── arial.ttf
└── ...
```

⚠️ **Important:** Folderele `build/` și `install_dir/` sunt în `.gitignore` deoarece conțin fișiere generate.

### Debugging

Pentru debugging cu informații detaliate:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
```

### Testing Tools

Proiectul include scripturi pentru:
- **Valgrind:** `./scripts/run_valgrind.sh` - Memory leak detection
- **Cppcheck:** `./scripts/run_cppcheck.sh` - Static analysis
- **Sanitizers:** `./scripts/run_sanitizers.sh` - Address/Undefined behavior sanitizers


## 🐛 Known Issues & Future Improvements

### Current Limitations
- Spawn rate-ul inamicilor nu crește în timp
- Nu există sistem de wave-uri
- Lipsa efectelor de sunet

### Planned Features
- 🔄 Sistem de wave-uri cu dificultate crescândă
- 🎵 Efecte sonore și muzică de fundal
- ✨ Efecte vizuale (particule, animații)
- 🏆 Sistem de scoreboard și high scores
- 🗺️ Multiple niveluri/camere
- 💎 Power-ups temporare
- 🎨 Animații pentru personaje

## 🤝 Contribuții

Proiect realizat individual pentru cursul de **Programare Orientată pe Obiecte**.

**Autor:** Frumuzache Alexandru-Marian  
**Universitate:** Universitatea din Bucuresti
**An:** 2025-2026  

## 📖 Resurse și Referințe

### Inspirație
- **DaniDev** - Concept original de joc: [Video YouTube](https://www.youtube.com/watch?v=YMWnwBpUgoI&t=1s)

### Biblioteci și Framework-uri
- **SFML (Simple and Fast Multimedia Library)** - [https://www.sfml-dev.org/](https://www.sfml-dev.org/)
  - Documentație SFML 3.0: [https://www.sfml-dev.org/documentation/3.0.0/](https://www.sfml-dev.org/documentation/3.0.0/)
  
### Tutoriale și Documentație
- **Learn C++** - [https://www.learncpp.com/](https://www.learncpp.com/)
- **C++ Reference** - [https://en.cppreference.com/](https://en.cppreference.com/)
- **CMake Documentation** - [https://cmake.org/documentation/](https://cmake.org/documentation/)

### Design Patterns
- **Refactoring.Guru** - Design Patterns: [https://refactoring.guru/design-patterns](https://refactoring.guru/design-patterns)
  - Singleton Pattern
  - Factory Pattern
  - Strategy Pattern

### Asset-uri
- Texturi create manual sau găsite pe:
  - OpenGameArt.org
  - Itch.io (free assets)
  - Font: Arial (system font)


## 📄 Licență

Acest proiect este realizat în scop educațional pentru cursul de POO.

## 🎮 Cum să Joci

1. **Compilează** proiectul folosind instrucțiunile de mai sus
2. **Rulează** executabilul din folderul build
3. **Supraviețuiește** cât mai mult timp posibil
4. **Ucide inamici** pentru a câștiga monede
5. **Cumpără upgrade-uri** cu tastele 1-4
6. **Folosește consumabile** cu P și L
7. **Aruncă grenade** cu Left Click (dacă ai în inventar)

---

**Mulțumiri speciale** tuturor celor care au contribuit la dezvoltarea SFML și comunității de game development pentru resurse și tutoriale!

🎮 **Have fun playing Shadow of the Cave!** 🎮
