#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include "includes/encryption.h"

using namespace std;

struct Account{
	
	int acc_num;
	string name;
	int pin;
	float balance;
	Account *next;
	
};

struct Transaction{
	
	int action;
	float amount;
	Transaction *next;
	
};

struct Stack{
	
	Transaction *top;
	
};


//Function to create an Account i.e allocating memory and assigning values to the variables in the struct - similar to a createNode 
Account *createAccountNode(int num, string name, int pin, float balance){
	
	Account *newAcc;
	newAcc = new Account;
	
	newAcc->acc_num = num;
	newAcc->name = name;
	newAcc->pin = pin;
	newAcc->balance = balance;
	
	newAcc->next = NULL;
	
	return newAcc;
}

//Function to insert at head of linked list
Account *insertAtHead(Account *top, int num, string name, int pin, float balance){
	
	Account *newAcc;
	
	newAcc = createAccountNode(num, name, pin, balance);
	
	newAcc->next = top;
	top = newAcc;
	
	return top;
	
}

//Function to check if an entire string is a number 
bool numCheck(string s){
	
	for (int i = 0; i < s.length(); i++){
		
		if(s.at(i) < '0' || s.at(i) > '9'){
			
			return false;
		}
	}
	
	return true;
}

//Function to convert a string into a number of int variable type
int stringToNum(string s){
	
	int digit;
	int num = 0;
	
	for(int i = 0; i < s.length(); i++){
		
		digit = s.at(i) - '0';
		num = (num *10) + digit;
		
	}
	
	return num;
}

//pointer needed to be passed by reference here - if the contents of this function was in main then it would not need this (IMPORTANT)
void loadAccounts(Account *& top){
	
	ifstream in;
	in.open("Accounts.txt");
	
	if(!in.is_open()){
		
		cout << "Error in opening Accounts file..." << endl;
		return;
		
	}
	
	int num;
	string name;
	int encrypted_pin;
	int decrypted_pin;
	float balance;
	string temp;
	
	in >> num;
	
	while(!in.eof()){
		
		in.ignore();			//needed to move to newline for getline
		getline(in, name);
		
		in >> encrypted_pin;
		decrypted_pin = decryptPIN(encrypted_pin);
		
		in >> balance;
		in.ignore(); 			//moves to next line (program still works without it) 
		getline(in, temp);
						
		top = insertAtHead(top, num, name, decrypted_pin, balance);
		
		in >> num;
	}
	
	in.close();
	
}

//Function to store all accounts in the accounts file at the end of the program
void storeAccounts (Account *&top){
	
	if(top == NULL){
		
		cout << "No Accounts...";
		return;
		
	}
	
	ofstream out;
	out.open("Accounts.txt");
	
	Account *curr;
	curr = top;
	
	int encrypted_pin;
	
	while(curr != NULL){
		
		out << curr->acc_num;
		out << endl;
		out << curr->name;
		out << endl;
		
		encrypted_pin = encryptPIN(curr->pin);
		out << encrypted_pin;
		out << endl;
		
		out << curr->balance;
		out << endl << endl;
		
		curr = curr->next;
	}
	
}


//Function to count the number of digits in a given integer
int numDigit (int n){
	
	int count = 0;
	
	while(n > 0){
		
		n = n /10;
		count++;
		
	}
	
	return count;
	
}

//Function to check if an Account Number is alread being used
bool inUseAccNum(Account *top, int acc_num){
	
	Account *curr;
	
	curr = top;
	
	while(curr != NULL){
		
		if(curr->acc_num == acc_num){
			
			return true;
		}
		
		curr = curr->next;
	}
	
	return false;
}

//Function that is used to take the user's input and create a new bank account
void createAccount(Account *&top){
	
	int acc_num;
	string name;
	int pin_int;
	string pin_str;
	float balance;
	
	int pin_length;
	
	cout << "To Create an Account Enter the following: " << endl;
	
	cout << "Account number: ";
	cin >> acc_num;
	
	while(cin.fail() || inUseAccNum(top, acc_num)){
		
		if(cin.fail()){
			
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid Account Number... Ensure that the Account Number only contains digits: ";
			cin >> acc_num;	
		}
		
		
		else if (inUseAccNum(top, acc_num)){
			
			cout << "Account Number already in use... Please enter another Account Number: ";
			cin >> acc_num;
		}
		
	}
	
	cout << "Name: ";
	cin.ignore();
	getline(cin, name);
	
	cout << "Pin (must be 4 digits): ";
	getline(cin, pin_str);
	
	
	while(!numCheck(pin_str) || pin_str.length() != 4){
		
		cout << "Invalid pin... please enter a pin that is 4 digits: ";
		cin >> pin_str;
		
	}
	
	pin_int = stringToNum(pin_str);
	
	cout << "Starting balance: $";
	cin >> balance;
	
	while(cin.fail()){
		
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid balance... Please enter only digits for starting balance: $";
		cin >> balance;
	}
	
	
	top = insertAtHead(top, acc_num, name, pin_int, balance);
	
	cout << endl;
	cout << "Account Created Successfully... " << endl;
	cout << "Please login with the same credentials to withdraw, deposit or check balance" << endl;
	cout << endl;
	
}

//Function that is used to take the user's input and sign them into an existing account
//since not modifying the list you do not need to pass top by reference i.e Account *& top
Account *login(Account *top){
	
	Account *curr;
	int num, pin_int;
	string pin_str;
	
	if(top == NULL){
		
		cout << "No accounts";
		return NULL;
		
	}
	
	curr = top;
	
	cout << "To Login enter the following: " << endl;
	cout << "Account number: ";
	cin >> num;
	
	while(cin.fail()){
		
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid Account Number... Ensure that the Account Number only contains digits: ";
		cin >> num;
	}
	
	cin.ignore();
	cout << "Pin: ";
	getline(cin, pin_str);
	
	while(!numCheck(pin_str) || pin_str.length() != 4){
		
		cout << "Invalid pin... please enter a pin that is 4 digits: ";
		cin >> pin_str;
		
	}
	
	pin_int = stringToNum(pin_str);
	
	
	while(curr !=NULL){
		
		if(curr->acc_num == num && curr->pin == pin_int){
			
			return curr;
			
		}
		
		curr = curr->next;
	}
	
	return NULL;
	
}

//Function which takes the user's input and deletes an already existing account 
Account *deleteAccount(Account *&top){
	
	int num, pin;
	char choice;
	
	cout << "Enter account details to DELETE account" << endl;
	cout << "Account Number: ";
	cin >> num;
	
	while(cin.fail()){
		
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid Account Number... Ensure that the Account Number only contains digits: ";
		cin >> num;
	}
	
	cout << "Pin: ";
	cin >> pin;
	
	//Doesn't give as detailed error message here since the user should already be familiar with the program if they are deleting an account which also can help with fraud
	while(cin.fail()){
		
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid pin... Try again: ";
		cin >> pin;
	}
	
	cout << endl;
	cout << "Are you sure you want to delete this account? [Y/N]: ";
	cin >> choice;
	
	if(top == NULL){
		
		cout << "There are no accounts";
		return top;
		
	}

	if(toupper(choice) == 'Y'){
		
		Account *curr;
		Account *prev = NULL;
	
		curr = top;
	
		while(curr != NULL){
		
			if(curr->pin == pin && curr->acc_num == num){
			
				if(prev == NULL){
				
				top = top->next;
				
				}
			
				else{
				
					prev->next = curr->next;
				
				}
			
				delete(curr);
				cout << "Account deleted successfully..." << endl;
				return top;
			
			}
		
			prev = curr;
			curr = curr->next;
		}
		
		cout << "Account Not Found..." << endl;
		return top;	
		
	}	
	
	else{
		
		cout << "Account Deletion Cancelled..." << endl;
		
	}
	
	return top;

}

//Function to initialize the stack
Stack *initStack(){
	
	Stack *s;
	s = new Stack;
	s->top = NULL;
	
	return s;
}

//Function to check if stack is empty
bool isEmpty(Stack *s){
	
	return (s->top ==NULL);
	
}

//Function to allocate memory for a new transaction and assign values to the variables of the struct 
Transaction *createTrans(int act, float amt){
	
	Transaction *newTrans;
	newTrans = new Transaction;
	newTrans->action = act;
	newTrans->amount = amt;
	
	newTrans->next = NULL;
	
	return newTrans;
	
}

//Function to add transactions to the stack of transactions 
void push(Stack *s, int act, float amt){
	
	Transaction *newTrans;
	newTrans = createTrans(act, amt);
	
	newTrans->next = s->top;
	s->top = newTrans;
	
}

//Function to remove transactions from the stack of transactions
Transaction *pop(Stack *s){
	
	Transaction *t;
	
	if(isEmpty(s)){
		
		cout << "Stack is empty... " << endl;
		return s->top;
		
	}
	
	t = s->top;
	Transaction *curr;
	curr = s->top;
	
	s->top = s->top->next;
	
	return t;
	
}

//Function which allows the user to withdraw money from their account after logging in 
//Need to pass the account by reference instead of making a copy of it
void withdraw(Account &a, Stack *s, int act){
	
	float amt;
	cout << endl;
	cout << "Enter the amount of money to withdraw: $";
	cin >> amt;
	
	while(cin.fail() || a.balance < amt){
		
		if(cin.fail()){
			
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid amount... Ensure that you enter only digits: $";
		}
		
		else if(a.balance < amt){
			
		cout << "Account does not have enough money for this withdrawal" << endl;
		cout << "Please enter a smaller amount to withdraw: $";
		}
		
		cin >> amt;
		cout << endl;
	}
	
	cout << endl;
	a.balance = a.balance - amt;
	push(s, act, amt);
	
}

//Function which allows the user to deposit money into their account after logging in
//Need to also pass the account by reference here as well instead of making a copy of it
void deposit(Account &a, Stack *s, int act){
	
	float amt;
	cout << "Enter the amount of money to deposit: $";
	cin >> amt;
	cout << endl << endl;
	
	while(cin.fail()){
		
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid amount... Ensure that you enter only digits: $";
		cin >> amt;
		cout << endl;
	}
	
	push(s, act, amt);
	
	a.balance = a.balance + amt;
	
}

//Function which gives the user the option to undo their transaction by making use of the created stack for transactions
//Same concept as the 2 functions above i.e need to pass account by reference 
void undo(Account &a, Stack *s){
	
	Transaction *t;
	
	t = pop(s);
	
	if(t->action == 1){
		
		a.balance = a.balance + t->amount;
		
	}
	
	else if(t->action == 2){
		
		a.balance = a.balance - t->amount;
		
	}
	
	delete t;
	
}

//Function to check the balance of an account
void checkBalance(Account a){
	
	cout << "Balance: $" << a.balance;
	
}

void printList(Account *top){
	
	if (top == NULL){
		
		cout << "List is empty" << endl;
		return;
		
	}
	
	Account *curr;
	curr = top;
	
	while(curr != NULL){
		
		cout << "Account #: " << curr->acc_num << endl;
		cout << "Name: " << curr->name << endl;
		cout << "Pin: " << curr->pin << endl;
		cout << "Balance: " << curr->balance << endl;
		cout << endl;
		
		curr = curr->next;
		
	}
}



int main (){
	
	Account *top;
	top = NULL;
	
	loadAccounts(top);
	
	int action;
	int action_login;
	char choice;
	char choice2;
	
	Stack *s;
	s = initStack();
	
	cout << "===========================================================" << endl;
	cout << "             Hello, how can i assit you today?             " << endl;
	cout << "===========================================================" << endl;
	cout << endl; 
	
	
	cout << "Enter the corresponding number to perform the action" << endl;
	cout << "1. Create Account" << endl;
	cout << "2. Login" << endl;
	cout << "3. Delete Account" << endl;
	cout << "Enter your action (1, 2 or 3): ";
	cin >> action;
	cout << endl;
	cout << "===========================================================" << endl;
	cout << endl;
	
	if(action == 1){
		
		createAccount(top);
		cout << "===========================================================" << endl;
		cout << endl;
		
		action = 2;
	}
	
	if(action == 2){
		
		if(top == NULL){
			
			cout << "There are no accounts... Please create an account" << endl;
			return 0;
			
		}
		
		Account *Acc;
		Acc = NULL;
		
		while(Acc == NULL){
			
			Acc = login(top);
			
			if(Acc == NULL){
				
				cout << "Login Unsuccessful... Please try to login again" << endl << endl;
				
			}
			
		}
		
		cout << "Login Successful" << endl;
		cout << endl;
		
		cout << "===========================================================" << endl;
		cout << "What transaction will you like to do? " << endl;
		cout << "1. Withdraw" << endl;
		cout << "2. Deposit" << endl;
		cout << "3. Exit" << endl;
		cout << "Enter the corresponding number for the transaction (1, 2 or 3): ";
		cin >> action_login;
		
		if(action_login == 1){
			
			cout << endl;
			cout << "===========================================================" << endl;
			cout << "                         Withdrawal                        " << endl;
			cout << "===========================================================" << endl;
			
			withdraw(*Acc, s, 1);
			
			cout << "Do you want to check your balance? [Y/N]: ";
			cin >> choice;
			
			if(toupper(choice) == 'Y'){
				
				checkBalance(*Acc);
				
			}
			
			cout << endl << endl;
			cout << "Do you want to undo the withdrawl? [Y/N]: ";
			cin >> choice2;
			
			if(toupper(choice2) == 'Y'){
				
				undo(*Acc, s);
				
				cout << "Your ";
				checkBalance(*Acc);
				cout << " is unchanged " << endl;
				
			}
			
		}
		
		
		if(action_login == 2){
			
			cout << endl;
			cout << "===========================================================" << endl;
			cout << "                         Deposit                           " << endl;
			cout << "===========================================================" << endl;
			
			deposit(*Acc, s, 2);
			
			cout << "Do you want to check your balance? [Y/N]: ";
			cin >> choice;
			
			if(toupper(choice) == 'Y'){
				
				checkBalance(*Acc);
				
			}
			
			cout << endl << endl;
			cout << "Do you want to undo the deposit? [Y/N]: ";
			cin >> choice2;
			
			if(toupper(choice2) == 'Y'){
				
				undo(*Acc, s);
				
				cout << "Your ";
				checkBalance(*Acc);
				cout << " is unchanged " << endl;
				
			}
			
		}
		
		if(action_login == 3){
			
			cout << endl;
			cout << "Program Exited..." << endl;
			storeAccounts(top);
			return 0;
			
		}
		
	}
	
	if(action == 3){
			
			if(top == NULL){
				
				cout << "There are no accounts in the database..." << endl;
				return 0;
			}
			
			top = deleteAccount(top);
			
	}
		
		cout << endl;
		cout <<	"End of Program... I hope I was of good use" << endl;
		storeAccounts(top);
	
	printList(top);
	
	return 0; 
}