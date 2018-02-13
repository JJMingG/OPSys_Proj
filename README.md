# COP4610 Operating Systems Project 1 - Implementing a Shell
#### By Jamine Guo, Evan Schwalb, and Bob King

## Division of labor
      - Jamine Guo
          - I/O Redirection (Part 6)
          - Pipes (Part 7)
          - Built-ins (Part 9)
          - Documentation
          - Testing
          - Execution (Part 5)
      - Evan Schwalb
          - main() and prompt (Part3)
          - Parsing (Part 1)
          - Environmental Variables (Part 2)
          - Path Resolution (Part 4)
          - Makefile
      - Bob King
          - Background Processing (Part 8)

## project1_guo_schwalb_king.tar contents
      - README
      - Project1_COP4016.c
          - We decided to write all of our code in one file so this contains the
            main() as well as all other functions required for this project.
      - Makefile

## How to compile executable using Makefile
      - To build:
          - make
      - To clean:
          - make clean
      - To run test:
          - ./proj1

## Known bugs and unfinished portions of the project
      - Parsing
          1. The parsing has a bug where it will allocate another space in the
             2d array that will point to an empty c string if there is a space
             at the end of a line of input (ie. "cd " will be a size of 2
             instead of a size of 1), which may cause errors such as in the cd
             function which looks at args[1] if it has a size greater than 1.
             When testing, please refrain from adding a space at the end of the
             input (multiple spaces in the middle of input is fine).
      - Path Resolution
          1. This does not work with directories/files without anything in front
             it. For example, cmd directory will only work with directory, but
             cmd ./directory will work with /home/majors/usr/directory.
          2. The path resolution works for cmds such as ls (/usr/bin/ls) when it is by
             itself but not when there are more arguments. It does not look for path
             resolution of ls when it as more arguments, such as flags.
      - I/O Redirection
          1. The output redirection creates a file but it does not create correctly due
             to the problem of path resolution bug 1.
                - For example: current working directory is /home/usr/COP4610, the output
                  to create output.txt will result in a path resolution of
                  /home/udr/COP4610outputtxt. The file does create but in the parent
                  directory with the sample above name.
      - Background Processing
          -
      - Built-ins
          - Nested built-ins would be slower and produce "weird" outputs, such as using
            the io function with another built in will output two tables. However,
            nested built-ins were listed as an exception from the project prompt.
      - Warnings
          1. Implicit declaration of cuserid
          2. Implicit declaration of setenv
              - Looked up the warning, which mentioned that we need to define
                DEFAULT_SOURCE, BSD_SOURCE, or POSIX_C_SOURCE, but they did not
                get rid of the warning. We see this warning on linprog but not from
                another machine environment.

## Special considerations, etc
      - We used a dynamically allocated 2d array to store and array of pointers
        pointing to dynamically allocated c strings.
      - The parsing is limited to 11 cmds/args/flags/etc due to predefined array
        arguments.
      - This shell compensates for more than 3 pipelines.
      - io function assumes that pid will be no greater than 10 digits
      - For background processing, the & is assumed to have a space before or
        after it, meaning that it is tokenized by itself.
