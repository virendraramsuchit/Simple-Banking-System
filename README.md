This project showcases the programming skills I've acquired during the first year of my BSc Computer Science degree at UWI St. Augustine. It’s a simple console-based banking system that allows users to:

Create an account

Log in to an existing account

Delete an account

Perform transactions (withdrawals and deposits)

Undo the last transaction


Key Features & Concepts 
File I/O & Linked Lists
	- All user accounts are stored in a .txt file.

	- On program startup, accounts are loaded from the file into a linked list.

	- On exit, updates (e.g., balance changes, deleted accounts) are written back to the file.

	- The linked list structure enables efficient insertion and deletion of accounts.


Basic Encryption (PIN Security)
	- User PINs are encrypted before being saved to the file, and decrypted when loaded.

	- The encryption/decryption logic is stored in a separate file that is .gitignored to simulate security best practices (inside of includes file).

	- While this method is basic, it demonstrates an understanding of the importance of data protection.

	- A more secure approach (e.g., hashing with SHA-256) is noted as a real-world improvement.


Stack for Undo Functionality
	- Every transaction (deposit or withdrawal) is stored as a transaction node on a stack.

	- The undo operation simply pops the last transaction off the stack and reverses it.

	- This mimics how undo features work in text editors or other applications.


Additional Concepts
	- Functions and modular programming

	- Loops and conditionals

	- String manipulation

	- Use of structs for account and transaction modeling

	- Basic input validation (e.g., PIN length, numeric checks) 

Using the Program
If you'd like to try out the program:

For Windows Users
1. Download the .zip file from this repository.

2. Extract the contents to a folder.

3. Double-click on main.exe to run the program — no additional setup is required.

For macOS Users
Since .exe files are designed for Windows, you'll need to use one of the following methods:

Option 1: Wine
Install Wine to run main.exe directly on macOS.

Option 2: Windows Virtual Machine
Use a tool like VirtualBox, Parallels, or VMware Fusion to run a Windows VM and execute the program inside it.
