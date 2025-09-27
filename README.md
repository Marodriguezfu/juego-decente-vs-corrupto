# Decentes vs Corruptos (2017)

A small 2D game prototype built in C++ using **SFML**.  
The player launches the game executable and interacts to progress through a sequence of randomised board actions. This repository preserves the historical academic project from **2017**.

> Documents included: Use Cases and Installation Instructions (macOS + SFML).

---

## 🕹 Gameplay (from Use Cases)

- The game shows the board and instructions after launch.
- Pieces are randomly placed on white squares.
- A random draw selects which side starts: **1 = Decentes**, **2 = Corruptos**.
- A second random draw selects which piece moves and the movement to attempt.
- If a move is impossible, the game tries the next move clockwise; if none is possible it selects the next piece.
- **Controls**: `↑` (Up Arrow) to progress actions, `Esc` to exit.

---

## 📦 Project Structure
```
juego-decente-vs-corrupto-2017/
├── bin/ # (optional) compiled binaries / outputs
├── Fuentes/ # fonts
├── imag/ # images / sprites
├── Sonido/ # audio assets
├── main.cpp # C++ source (SFML)
├── INSTRUCCIONES DE INSTALACION.pdf
├── Casos de uso.pdf
├── README.md
└── .gitignore
```
## 🖥️ Installation Instructions (macOS)

> Requires basic knowledge of navigating directories in the macOS terminal.

1. Open **Terminal** and paste the following command to install **Homebrew**:
   ```bash
   /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

2. Enter your password when prompted.

3. In the same terminal, type:
```
brew install sfml
```
(without quotes). It may ask for your password again.

4. Copy the entire project folder to your Desktop (mandatory).

5. Navigate to the project directory:
```
cd
cd Desktop/game
```

6. Verify that main.cpp is in the current directory:

```
ls
```

7. Compile the project with:
```
g++ -std=c++11 main.cpp -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -o juego.out

```

8. Run the game:
```
./juego.out
```

