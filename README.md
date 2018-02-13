# COP4610 Operating Systems Project 1 - Implementing a Shell
#### By Jamine Guo, Evan Schwalb, and Bayan Kharazmi

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
      - Bayan Kharazmi
          - Background Processing (Part 8)

## project1_guo_schwalb_kharazmi.tar contents
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
                - This is a problem for any file with a period.
                - This is most likely related to the logic of how the function finds files
                  that start with a dot/period.
          2. The path resolution works for cmds such as ls (/usr/bin/ls) when it is by
             itself but not when there are more arguments. It does not look for path
             resolution of ls when it as more arguments, such as flags.
          3. In order to fully test other functions, it may be better to comment out line
             273 and typing in full path functions for example in pipeline:
             "/usr/bin/cmd arg | /usr/bin/cmd arg arg | /usr/bin/cmd arg ...".
          4. This is unable to get the path of all commands when there are more than one
             commands in a single input for functions like piping. It does not know what
             to do with pipes and will sometimes replace them.
                - Similar to most of the other bugs, this is most likely because it keeps
                  going out of scope.
                - We tried minimizing the uses of these and keeping track of where in scope
                  the function is going.
      - Pipes
          1. This is having problems only because of path resolution explained in 4. of
             Path Resolution bugs. This functions is fully functional and should be tested
             either using functions such as "echo" or by commenting out line 273 and running
             commands by typing out "/usr/bin/cmd arg".
      - I/O Redirection
          1. The output redirection creates a file but it does not create correctly due
             to the problem of path resolution bug 1.
                - For example: current working directory is /home/usr/COP4610, the output
                  to create output.txt will result in a path resolution of
                  /home/udr/COP4610outputtxt. The file does create but in the parent
                  directory with the sample above name.
      - Background Processing
          1.  This was not finished in time and was not implemented. It is commented out
              in order to not interfere with the rest of the file.
          2.  Does not work due to the improper implementation of the structs and the
              linked list when setting their values and having mismatching types.
          3.  There also might be issues with the way the fork is implemented for the
              background processes in coordination with the parent process.
          4.  Upon reflection, changes that could have been made to properly improve the
              background process could have included putting it in a separate function
              instead of having it mixed in with the normal execution.
          5.  Properly implementing waitpid for the background processes would have been
              another improvement. Another possible improvement to background processing
              could have been the addition of a zombie process checking function.
          6.  Warnings for usage with strcmp and setting array values in the background
              process sections involving typecasting and char arrays. There are errors in
              mismatched types when setting values.
      - Built-ins
          1.  Nested built-ins would be slower and produce "weird" outputs, such as using
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
      - The pipeline function supports unlimited pipes, however the path resolution may
        cause problems that will not be able to showcase this. This was mentioned in the
        pipes bugs section and explains a way to fully test this.
      - Background processing was not completed so this program was created without
        regards to background processing.
