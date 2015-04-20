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
git checkout hw0
make
bin/rshell
```

### Bugs/Issues/Limitations

1. Entering `bin/rshell` in the shell simulator runs a duplication of the application.
Returning to the bash shell would require you to enter `exit` exactly the number of times you enter the previous command.

2. The `echo` command will include quotation marks when printing quoted arguments.
It cannot send text to a new or existing file using the `>` character, either.

3. Connectors entered first will not return a syntax error.

4. The `cd` command will not be executed successfully.

5. The `git commit -m <message>` command will not be executed successfully.
