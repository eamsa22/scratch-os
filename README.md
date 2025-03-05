ScratchOS Project
ScratchOS is a minimalist operating system designed for educational purposes. The project simulates basic shell operations such as user management and file permission handling. While it does not yet support full file creation or removal functionalities, the system can easily be extended to include these features. This project was developed as part of a university course.

Installation
To install ScratchOS, follow these steps:

Build the project: Run the following command to build the installation package:

bash
Copier
Modifier
$ make install
Run the installer: After building, execute the installer by running:

bash
Copier
Modifier
$ ./install
This will set up the necessary environment for ScratchOS to run.

Execution
Once ScratchOS is installed, you can start the system and interact with it by following these steps:

Build the ScratchOS executable: Run the following command to build the executable:

bash
Copier
Modifier
$ make scratch
Run ScratchOS: After building the executable, start ScratchOS by running:

bash
Copier
Modifier
$ ./scratch
This will launch the command-line interface where you can begin using ScratchOS and execute various system commands.

Available Commands
After starting ScratchOS, you can use the following commands to interact with the system:

1. ls – List Files
Lists the files present in the system. Use the -l option for detailed information.

Usage:
ls – Lists the file names and their sizes.
ls -l – Lists detailed information, including the owner, size, and timestamps.
2. cat – Display File Content
Displays the content of a specified file.

Usage:
cat <filename> – Displays the content of the file specified by <filename>.
3. chown – Change File Owner
Changes the owner of a specified file.

Usage:
chown <filename> <new_owner> – Changes the owner of the file to <new_owner>.
4. chmod – Change File Permissions
Changes the permissions of a specified file.

Usage:
chmod <filename> <permissions> – Changes the permissions of the file specified by <filename>. Use strings like rw, rW, etc. for permissions.
5. listusers – List Users
Lists all the users registered in the system.

Usage:
listusers – Lists all users in the system.
6. adduser – Add a New User
Adds a new user to the system. You will be prompted to enter the login name and password.

Usage:
adduser – Starts the process of adding a new user. Follow the prompts to enter the username and password.
7. rmuser – Remove a User
Removes an existing user from the system.

Usage:
rmuser <username> – Removes the user specified by <username> from the system.
Example Usage
Once ScratchOS is running, you can execute commands like the following:

bash
Copier
Modifier
root@ScratchOS:~$ ls
file1.txt - 512 bytes
file2.txt - 1024 bytes

root@ScratchOS:~$ cat file1.txt
This is the content of file1.txt.

root@ScratchOS:~$ chown file2.txt john
File owner changed to john.

root@ScratchOS:~$ chmod file2.txt rW
File permissions changed to read/write for the owner and read-only for others.

root@ScratchOS:~$ listusers
root
john

root@ScratchOS:~$ adduser
Enter login: benjamin
Enter password: ************
User added successfully.

root@ScratchOS:~$ rmuser benjamin
User benjamin removed successfully.
