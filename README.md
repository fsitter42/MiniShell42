*This project has been created as part of the 42 curriculum by fsitter and slambert*
# MiniShell42

**Description**
This project simulates the bash shell in a simplified way. It does not behave like bash in all cases. Whatever is not explicitly listed as a feature is not supported and its output is considered undefined behaviour.

The workflow of minishell is tokenization -> variable expansion -> word splitting -> command execution

We implemented the following features:
- input history (in normal mode)
- quote handling
- environment variable expansion
- word splitting
During word splitting, the shell divides input based on the characters defined in the Internal Field Separator (IFS) variable. If IFS is unset, it defaults to splitting by spaces, tabs, and newlines (" \t\n"). Empty arguments (such as "") are removed during this process. This splitting mechanism applies consistently even when IFS contains non-whitespace characters.
- pipe handling
- file descriptor redirection
- ...

not supported:
- command expansion
- arithmetic expansion

**Instructions**
Run make to create an executable file called minishell.
you can't separate lines by semicolon (;) - e.g. "cd ..;ls".

Minishell can be used in 2 ways.
1. debug mode: usage with -d flag and ONE additional argument. This argument can hold one or multiple line(s) to be interpreted. If multiple lines should be passed, they have to be separated by a semicolon (;)

- Example usage 1: ./minishell -d "< infile cat -e | wc -l > outfile"
- Example usage 2: ./minishell -d "< infile cat -e | wc -l > outfile; echo $USER"

2. normal / interactive mode: This mode emulates the way a normal shell works. usage: run with ./minishell

**Resources**
- Manual pages: man 2 bash, man 2 pipe, man 2 dup2, man 2 execve, man 2 open, man 2 access, man 2 waitpid.

- GNU C Library / POSIX documentation for system calls and error handling.

- 42 documentation and intra subject PDF for the official pipex specification.

- AI was used for conceptual explanations and not for writing or generating any code.

TODO:
- semicolon split in interactive mode
- check ob atoi overflow (char * größer als long long) safe is