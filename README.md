*This project has been created as part of the 42 curriculum by fsitter, slambert*
# MiniShell42

## Description
MiniShell42 is a simplified Bash-like shell written in C.

Workflow:
`tokenization -> variable expansion -> word splitting -> commandization -> execution`

This project does not aim to be fully Bash-compatible. Behavior that is not explicitly listed below should be considered out of scope.

## Instructions
- Build: `make`
- Run: `./minishell` (do not run it with any arguments)

## Supported Behavior
- Interactive prompt with readline history.
- Quote handling:
	- `'single quotes'` prevent expansion.
	- `"double quotes"` allow `$VAR` and `$?` expansion.
- Environment variable expansion (`$VAR`) and last exit code expansion (`$?`).
- Word splitting using `IFS`:
	- If `IFS` is set, its characters are used.
	- If `IFS` is unset, fallback is `" \t\n"`.
	- If `IFS=""`, splitting is disabled.
	- Null arguments (for example "") are removed after word splitting if they were not quoted.
- Redirections: `<`, `>`, `>>`, `<<` (heredoc).
- Pipes (`|`) across commands.
- Builtins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`.
- Signal handling in interactive mode:
	- `Ctrl-C` shows a new prompt on a new line.
	- `Ctrl-D` exits the shell.
	- `Ctrl-\` is ignored.

## Unsupported / Out of Scope
- Semicolon command chaining (for example `cd ..; ls`).
- Backslash escape semantics beyond required subject scope.
- Command substitution.
- Arithmetic expansion.

## Exit Status Notes
- `0` on successful command execution.
- `1` on error
- `126` for permission/exec format style failures.
- `127` for command not found.
- `130` after `SIGINT` (`Ctrl-C`) interruption.

## Heredoc Notes
- Delimiter matching is exact (line must match the delimiter text exactly).
- Quoted heredoc delimiters disable variable expansion inside heredoc content.

## Quick Test Commands

Basic pipeline + redirection:
```bash
echo hello | wc -c
echo test > out
cat out
```

Heredoc basic:
```bash
cat << EOF
hello
EOF
```

Heredoc quoted delimiter (no expansion expected):
```bash
cat << "EOF"
$USER
EOF
```

IFS behavior checks:
```bash
export IFS=,
echo a,b,c
unset IFS
echo a b c
```

Redirection ordering regression check:
```bash
printf "echo segfault <'<<<'<<amazing\namazing\n" | ./minishell
```
Expected: heredoc input is consumed first, then `<<<: No such file or directory` is reported.

## Resources
- Manual pages: man 2 bash, man 2 pipe, man 2 dup2, man 2 execve, man 2 open, man 2 access, man 2 waitpid.
- GNU C Library / POSIX documentation for system calls and error handling.
- 42 documentation and intra subject PDF for the official pipex specification.
- AI was used for conceptual explanations and not for writing or generating any code.