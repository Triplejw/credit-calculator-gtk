# Credit Calculator (GTK-based GUI)

A simple desktop GUI application built using GTK in C to calculate total academic credits based on selected courses. Designed for ECE department students at Karunya University.

## 🧰 Features
- Dropdown lists for 8 semesters
- Select from predefined course list loaded from `data` file
- Calculates and validates if minimum credit requirement (24) is met
- Displays confirmation or warning via GTK dialog box

## 🛠️ Built With
- C
- GTK 3+

## 📁 Files
- `Credit_Calc.c` – Main C program with GTK GUI logic
- `data` – Course list (code, name, credits)
- `README.md` – This file

## 🧪 How to Run
```bash
gcc Credit_Calc.c -o credit_calc `pkg-config --cflags --libs gtk+-3.0`
./credit_calc
