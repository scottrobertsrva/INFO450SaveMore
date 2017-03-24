// Info450SaveMoreScottRoberts.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <iostream>
#include <string>
#include <time.h>

#pragma warning(disable : 4996)

using namespace std;

class BankAccount {
protected:
	static int idGenerator;
	string accountType = "";
	int accountNumber = 0;
	double interestRate = 0;
	double monthlyInterestRate = 0;
	double balance = 0;
	time_t rawTime;
	char* charTime;

public:
	BankAccount();
	BankAccount(double);
	bool baseWithdrawal(double);
	string getAccountType();
	void addMonthlyInterest();
	bool baseDeposit(double);
	virtual void orderChecks() {}
	virtual void display() = 0;
	virtual bool withdrawal(double) = 0;
	virtual bool deposit(double) = 0;

};

int BankAccount::idGenerator = 1001;

void BankAccount::addMonthlyInterest() {
	float mrate = interestRate / 12.0;
	float mInt = balance * mrate;
	float newSavingBalance = balance + mInt;
	balance = newSavingBalance;
	cout << "-------------------------------------" << endl;
	cout << "ACCOUNT #: " << accountNumber << endl;
	cout << endl << "Account Interest Added: $" << mInt << endl;
	cout << "New Account Balance: $" << balance << endl;
	cout << "-------------------------------------" << endl;
}

BankAccount::BankAccount() {
	accountNumber = idGenerator++;
	rawTime = time(NULL);
	charTime = ctime(&rawTime);
}

BankAccount::BankAccount(double bal) {
	balance = bal;
	accountNumber = idGenerator++;
	rawTime = time(NULL);
	charTime = ctime(&rawTime);
}

string BankAccount::getAccountType() {
	return accountType;
}

bool BankAccount::baseWithdrawal(double amt) {
	if (balance - amt < 0) {
		cout << " Balance is too low to support this withdrawal." << endl;
		return false;
	}
	else {
		balance -= amt;
		return true;
	}
}

bool BankAccount::baseDeposit(double amtIn) {
	balance += amtIn;
	return true;
}

class SavingAccount : public BankAccount {
public:
	SavingAccount() 
	{ 
		accountType = "Savings";
		updateRate();
	};
	SavingAccount(double b) :
		BankAccount(b)
	{
		accountType = "Savings";
		updateRate();
	}
	void updateRate();
	bool withdrawal(double);
	bool deposit(double damt);
	void SavingAccount::display();
};


void SavingAccount::updateRate() {
	if (balance < 10000) {
		interestRate = .01;
	}
	if (balance >= 10000) {
		interestRate = .02;
	}
}
bool SavingAccount::withdrawal(double wamt) {
	if (BankAccount::baseWithdrawal(wamt + 2.0)) {
		updateRate();
		return true;
	}
	else {
		return false;
	}
}

bool SavingAccount::deposit(double damt) {
	cout.precision(2);
	cout << "-------------------------------------" << endl;
	cout << "ACCOUNT #: " << accountNumber << " Deposit: $" << fixed << damt << endl;
	cout << "-------------------------------------" << endl;
	BankAccount::baseDeposit(damt);
	updateRate();
	return true;
}

void SavingAccount::display() {
	cout.precision(2);
	cout << "-------------------------------------" << endl;
	cout << "ACCOUNT TYPE: " << accountType << endl;
	cout << "ACCOUNT #: " << accountNumber << " BALANCE $" << fixed << balance << endl;
	cout << "INTEREST RATE: " << interestRate << endl;
	cout << "OPEN DATE: " << charTime << endl;
	cout << "-------------------------------------" << endl;
}

class CheckingAccount : public BankAccount {
public:
	CheckingAccount() { accountType = "Checking"; };
	CheckingAccount(double b) :
		BankAccount(b) {
		accountType = "Checking";
		deductWithdrawalFee();
	}
	void deductWithdrawalFee();
	void orderChecks();
	bool withdrawal(double);
	bool deposit(double damt);
	void CheckingAccount::display();
};

void CheckingAccount::deductWithdrawalFee() {
	if (balance < 500) {
		balance -= 5;
		cout << "Account: " << accountNumber << " Low Balance..." << endl;
		cout << "Deducting $5.00 Fee for account balance under $500.00." << endl;
	}
}

void CheckingAccount::orderChecks() {
	balance -=15 ;
	cout << "-------------------------------------" << endl;
	cout << "ACCOUNT: " << accountNumber << " Checks ordered.  $15.00 fee deducted.  New balance: " << balance << " " << endl;
	cout << "-------------------------------------" << endl;
}

bool CheckingAccount::withdrawal(double wamt) {
	if (BankAccount::baseWithdrawal(wamt + 2.0)) {
		cout.precision(2);
		cout << "-------------------------------------" << endl;
		cout << "ACCOUNT: " << accountNumber << " Withdrawal + $2.00 fee.  New balance: " << fixed << balance << " " << endl;
		cout << "-------------------------------------" << endl;
		deductWithdrawalFee();
		return true;
	}
	else {
		cout << "-------------------------------------" << endl;
		cout << "ACCOUNT: " << accountNumber << "Balance to low to withdrawal this amount." << endl;
		cout << "-------------------------------------" << endl;
		return false;
	}
}

bool CheckingAccount::deposit(double damt) {
	cout.precision(2);
	cout << "-------------------------------------" << endl;
	cout << "ACCOUNT #: " << accountNumber << " Deposit: $" << fixed << damt << endl;
	cout << "-------------------------------------" << endl;
	BankAccount::baseDeposit(damt);
	return true;
}

void CheckingAccount::display() {
	cout.precision(2);
	cout << "-------------------------------------" << endl;
	cout << "ACCOUNT TYPE: " << accountType << endl;
	cout << "ACCOUNT #: " << accountNumber << " BALANCE $" << fixed << balance << endl;
	cout << "INTEREST RATE: " << interestRate << endl;
	cout << "OPEN DATE: " << charTime << endl;
	cout << "-------------------------------------" << endl;
}

class CertificateOfDeposit : public BankAccount {
private:
	int term = 0;
public:
	CertificateOfDeposit() { accountType = "Certificate of Deposit"; };
	CertificateOfDeposit(int t) {
		accountType = "Certificate of Deposit";
		term = t;
		if (term >= 5) {
			interestRate = .1;
		}
		else {
			interestRate = .05;
		}
	}
	bool withdrawal(double);
	bool deposit(double damt);
	void CertificateOfDeposit::display();
};


bool CertificateOfDeposit::withdrawal(double wamt) {
	double fee = balance*.1;
	double totalWithdrawalAmt = (fee + wamt);
	cout.precision(2);
	cout << "-------------------------------------" << endl;
	cout << "CD withdrawal: " << endl;
	if (balance < totalWithdrawalAmt) {
		cout << "Balance is too low to withdrawal this amount." << endl;
		return false;
	}
	else {
		BankAccount::baseWithdrawal(totalWithdrawalAmt);
		cout << "CD early withdrawal fee $" << fixed << fee << endl;
		cout << "Withdrawal Amount $" << fixed << totalWithdrawalAmt << endl;
		cout << "New balance $" << fixed << balance << endl;
		cout << "-------------------------------------" << endl;
		return true;
	}
}

bool CertificateOfDeposit::deposit(double damt) {
	cout.precision(2);
	cout << "-------------------------------------" << endl;
	cout << "ACCOUNT #: " << accountNumber << " Deposit: $" << fixed << damt << endl;
	cout << "-------------------------------------" << endl;
	BankAccount::baseDeposit(damt);
	return true;
}

void CertificateOfDeposit::display() {
	cout.precision(2);
	cout << "-------------------------------------" << endl;
	cout << "ACCOUNT TYPE: " << accountType << endl;
	cout << "ACCOUNT #: " << accountNumber << " BALANCE $" << fixed << balance << endl;
	cout << "INTEREST RATE: " << interestRate << endl;
	cout << "OPEN DATE: " << charTime << endl;
	cout << "-------------------------------------" << endl;
}

int main()
{
	BankAccount **bList;
	const int MAXACCOUNTS = 50;
	int count = 0;
	bList = new BankAccount*[MAXACCOUNTS];
	bList[count] = new SavingAccount();
	bList[count]->deposit(10000);
	
	count++;
	bList[count] = new CheckingAccount();
	bList[count]->deposit(600);
	
	count++;
	bList[count] = new CertificateOfDeposit(3);
	bList[count]->deposit(10000);

	for (int i = 0; i < count + 1; i++) {
		bList[i]->display();
	}
	
	bList[0]->addMonthlyInterest();
	bList[2]->addMonthlyInterest();

	for (int i = 0; i < count + 1; i++) {
		bList[i]->display();
	}

	bList[1]->orderChecks();

	bList[1]->withdrawal(200);
	bList[0]->withdrawal(1000);
	bList[2]->withdrawal(2000);

	for (int i = 0; i < count + 1; i++) {
		bList[i]->display();
	}
	return 0;
}
