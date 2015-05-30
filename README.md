## rshell

*rshell* is a C++ application that simulates a terminal's shell.
It executes commands, creates, overrides, and deletes files, and even runs programs, as if you are still using your terminal!

### Building and Running

**Note:** If you are using Ubuntu, you may need to install the GNU C++ compiler package in order to generate the executable.
There are two known ways to do this:

- Type in your terminal: `sudo get-apt install g++`

- Find the package under your OS version from this [link](http://packages.ubuntu.com/search?keywords=g%2B%2B).
Then download it and follow the instructions on installation.

Running this software requires **only 5 steps** using your terminal:

```
git clone https://github.com/aesgu001/rshell.git
cd rshell
git checkout hw3
make
bin/rshell
```

### Added Features

1. **Input/Output Redirection:** This shell can now read from an existing file with the `<` operator,
and write to a new or existing file by either truncating `>` or appending `>>`.
**Note, however, that it can only do 1 input and/or 1 output redirection,
and only the rightmost redirection arguments will be passed in per command.**

2. **Piping:** Piping is now implemented into this shell,
allowing information from a previous command to be passed in to the next one.
It can pipe two or more commands.

3. **Change Directory:** The new `cd` command feature allows the user to work under more than one existing directory.
The shell's prompt also displays the current working directory.

4. **Interrupt Signal:** The interrupt signal or `^C` now returns the user to the shell's prompt,
cancelling commands without the shell exiting itself!

5. **Stop Signal:** The stop signal or `^Z` now stops a child process,
and stores it in a stack, where it shall either be resumed or terminated by the user.
The `fg` and `bg` commands are also implemented to resume *the most recently stopped child process* either in the foreground or in the background.

### Bugs/Issues/Limitations

1. Entering `bin/rshell` in the shell simulator runs a duplication of the application.
Returning to the bash shell would require you to enter `exit` exactly the number of times you enter the previous command.

2. The `echo` command will include quotation marks when printing quoted arguments.

3. The `git commit -m <message>` command will not be executed successfully.

4. Appended redirection operators will not perform redirection.

5. Redirection operators passed in first per command will be recognized as executables,
which will not be executed successfully.

6. Any output redirection will be ignored should any input redirection fail in one command.

7. Piping commands without operands, such as `ls -r |`, will be treated as a syntax error.

8. Any child process will automatically be terminated by the `^C` signal,
stopped or running.
Interrupting running child processes in the background will display the shell
prompt in a disorderly manner.

9. The built-in stack data structure can store more than one stopped child process,
but only the top child process may continue by entering `fg` or `bg`,
since the only way to terminate it is by sending the `^C` signal.

10. Killing a stopped child process using `kill -9` will print the shell prompt
in a disorderly manner.
"Resuming" a killed child process will exit the shell.

11. Resuming child processes in the background will not be stopped if they are trying to read from the shell's standard input.

### ls Bugs/Issues/Limitations

1. Without the `-l` flag, `ls` only separates the files by spaces.

2. Texts of hidden files do not have a grey background.

3. Running `bin/ls -R -a` on heavy directories, like `.git`, will most likely fail due to a syscall error.

4. Number flags do not return an error.

5. `-l` flag may format unequal spacing due to the number of hard links for each file.
