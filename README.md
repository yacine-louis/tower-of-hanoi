# Tower Of Hanoi

program that measures how the Tower of Hanoi algorithm performs on your PC, using both the recursive and iterative versions.
you can run the program with different numbers of disks and it will report the execution time.

## Features

- **Command-line interface**
  - Simple menu with three options:
  - **Single Test**, **Range Test**, or **Exit**.
- **Single Test**
  - choose between the recursive or iterative Tower of Hanoi algorithm.
  - Runs the selected method once and displays the execution time.
- **Range Test**
  - Specify a start and end number of disks.
  - Runs benchmarks for the whole range.
  - Automatically opens a Raylib window showing:
    - A table with execution times for each number of disks.
    - A graph plotting number of disks vs. execution time.

## Installation

you can download a working version of the program in releases page <a href="https://github.com/yacine-louis/tower-of-hanoi/releases/tag/hanoi">Here</a>

## Build Project

1. Download **w64devkit** from the official releases page:  
   https://github.com/skeeto/w64devkit/releases  
   (Choose the file named **w64devkit-x64-<version>.7z.exe**)

2. Extract the contents directly into your **C:\\** drive.  
   This will create the folder: `C:\w64devkit`

3. Remove any old **MinGW** paths from your system **Environment Variables** (PATH).  
   Look for entries like:  
   `C:\MinGW\bin`  
   `C:\msys64\mingw64\bin`  
   and delete them to avoid conflicts.

4. Add the following directory to your **Environment Variables** (PATH):  
   `C:\w64devkit\bin`

5. run the folowing commands

```
  # clone repo
  git clone https://github.com/yacine-louis/tower-of-hanoi

  cd tower-of-hanoi

  # compile application
  gcc src/*.c -o main.exe -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm

  # run executable
  ./main.exe
```
