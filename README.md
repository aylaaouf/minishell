# minishell
# Minishell - From Zero to Hero 🚀

Welcome to your **Minishell** journey! This README provides a full guide on what you need to learn and implement to successfully complete the **mandatory** part of the Minishell project.

---

## 📚 What You Will Learn

By building Minishell, you’ll learn:
- How shells work internally
- Parsing and executing commands
- Managing processes and file descriptors
- Handling pipes and redirections
- Managing environment variables
- Implementing built-in commands
- Signal handling

---

## 🧠 Step-by-Step Learning & Implementation Guide

---

### 1. 🧵 Shell & Terminal Basics
- What is a shell and how does it work?
- Shells vs terminals
- Execution flow of a command like `ls -l`

---

### 2. 🧠 C Programming Fundamentals
Make sure you're confident with:
- `malloc`, `free`, `realloc`
- String manipulation: `strdup`, `strtok`, `strchr`, `strncmp`
- File descriptors: `read`, `write`, `open`, `close`
- Understanding and using arrays of strings (`char **`)

---

### 3. 🔄 Process Management
Key system calls:
- `fork()` - create a new process
- `execve()` - replace process image
- `wait()` / `waitpid()` - wait for child process
- `exit()` - return exit code

💡 Each command will be run in a child process using these calls.

---

### 4. 🌍 Environment Variables
- Understand `char **envp` in `main`
- How to copy and modify environment variables
- Implement `env`, `export`, `unset`

You’ll maintain your own internal `env` list and update it as needed.

---

### 5. 🧰 Built-in Commands
Implement these:
- `echo` (with `-n`)
- `cd`
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

💡 Only `cd`, `exit`, and `export` must be run in the parent process.

---

### 6. 🧹 Parsing & Tokenizing Input
You’ll write a custom parser to:
- Split input by spaces while respecting:
  - Quotes `'like this'`, `"like this"`
  - Environment variables: `$HOME`, `$?`
- Detect syntax errors
- Handle escape sequences and empty inputs

Example:
```bash
echo "Hello $USER" | grep "o" > output.txt

