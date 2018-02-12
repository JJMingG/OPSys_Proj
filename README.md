# COP4610 Operating Systems Project 1 - Implementing a Shell
#### By Jamine Guo, Evan Schwalb, and Bob King

## Division of labor
      - Jamine Guo
          - I/O Redirection (Part 6)
          - Pipes (Part 7)
          - Built-ins (Part 9)
          - Documentation
      - Evan Schwalb
          - Parsing (Part 1)
          - Environmental Variables (Part 2)
          - Prompt (Part 3)
          - Path Resolution (Part 4)
      - Bob King
          - Execution (Part 5)
          - Background Processing (Part 8)

## project1_guo_schwalb_king.tar contents
      - README
      - Project1_COP4016.c
          - We decided to write all of our code in one file so this contains the
            main() as well as all other functions required for this project.
      - Makefile

## How to compile executable using Makefile
      - To build:
          - TBD
      - To clean:
          - TBD
      - To run test:
          - TBD

## Known bugs and unfinished portions of the project
      - Parsing
          1. The parsing has a bug where it will allocate another space in the
             2d array that will point to an empty c string if there is a space
             at the end of a line of input (ie. "cd " will be a size of 2
             instead of a size of 1), which may cause errors such as in the cd
             function which looks at args[1] if it has a size greater than 1.
             When testing, please refrain from adding a space at the end of the
             input (multiple spaces in the middle of input is fine).
      - Environmental Variables
          -
      - Path Resolution
          -
      - Execution
          1. Not necessarily a bug, but when output redirection creates a file,
             you will need to chmod the file in order to check the contents of
             the file. This was not necessarily specified that it had to be
             changed so I (Jamine) did not do anything to change this. Otherwise,
             this function works correctly.
      - I/O Redirection
          -
      - Pipes
          -
      - Background Processing
          -
      - Built-ins
          -
      - Warnings
          1. Implicit declaration of cuserid
          2. Implicit declaration of setenv

## Special considerations, etc
      - We used a dynamically allocated 2d array to store and array of pointers
        pointing to dynamically allocated c strings.
      - The parsing is limited to 11 cmds/args/flags/etc due to predefined array
        arguments.
