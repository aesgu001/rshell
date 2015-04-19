## rshell

*rshell* is a C++ application that simulates a terminal's shell.
It executes commands and creates, overrides, deletes, and even run programs, as if you are still using your own terminal!

### Building and Running

Running this software requires only 5 easy steps using your terminal:

'''''
git clone https://github.com/aesgu001/rshell.git
cd rshell
git checkout hw0
make
bin/rshell
'''''

### Bugs/Limitations

1. Entering 'bin/rshell' in the shell simulator runs a duplication of the application. Returning to the bash shell would require you to enter 'exit' exactly the number of times you enter the previous command.
2. The 'echo' command does not have the ability to store text into a new or existing file.
3. Some git commands
