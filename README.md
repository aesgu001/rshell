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
git checkout hw2
make
bin/rshell
```

### Added Features

1. **Input/Output Redirection:** This shell can now read from an existing file with the `<` operator,
and write to a new or existing file either by truncating `>` or appending `>>`.

2. **Piping:**

### Bugs/Issues/Limitations

1. Entering `bin/rshell` in the shell simulator runs a duplication of the application.
Returning to the bash shell would require you to enter `exit` exactly the number of times you enter the previous command.

2. The `echo` command will include quotation marks when printing quoted arguments.

3. The `cd` command will not be executed successfully.

4. The `git commit -m <message>` command will not be executed successfully.

5. Appended redirection operators will not perform redirection.

6. Redirection operators passed in first per command will be recognized as executables,
which will not be executed successfully.

### ls Bugs/Issues/Limitations

1. Without the `-l` flag, `ls` only separates the files by spaces.

2. Texts of hidden files do not have a grey background.

3. Running `bin/ls -R -a` on heavy directories, like `.git`, will most likely fail due to a syscall error.

4. Number flags do not return an error.

5. `-l` flag may format unequal spacing due to the number of hard links for each file.
