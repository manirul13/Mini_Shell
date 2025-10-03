# 🐚 Mini_Shell

Mini_Shell is a lightweight implementation of a Unix-like shell written in C.  
It provides a minimal environment to execute commands, handle processes, and mimic basic shell behavior.
 

## ✨ Features

- Run external commands (e.g., `ls`, `pwd`, `cat`, etc.)  
- Built-in commands:  
  - `cd` → change directory  
  - `exit` → close the shell  
- Input and output redirection (`<`, `>`)  
- Pipe (`|`) support  
- Basic error handling  
- Simple, modular C implementation 


## 🛠️ Technologies Used

- **Language:** C
- **OS:** Linux / Ubuntu
- **Tools:** GCC, Makefile (optional)


## 🚀 Getting Started

### 1. Clone the repository
bash
git clone git@github.com:manirul13/Mini_Shell.git
cd Mini_Shell
### 2. Compile the project
bash
Copy code
gcc main.c minishell.c -o minishell
### 3. Run the shell
bash
Copy code
./minishell

Now you can type commands:

$ ls -l
$ cd ..
$ echo "Hello, World!"
$ ls | grep ".c" > output.txt
$ exit

📖 Example Session
$ pwd
/home/user/Mini_Shell
$ ls | grep README
README.md
$ cat README.md | wc -l
42
$ cd ..
$ exit

📂 Project Structure
bash
Copy code
Mini_Shell/
├── main.c        # Entry point for the shell
├── minishell.c   # Core shell logic
├── main.h        # Header file with function declarations
└── README.md     # Project documentation

📌 Future Improvements
1. Add support for pipes (|)
2. Add input/output redirection (>, <)
3. Implement background processes (&)

Add history and tab completion

🤝 Contribution
Pull requests are welcome! Here I'm your friend to help you.
For major changes, please open an issue first to discuss what you would like to change.
