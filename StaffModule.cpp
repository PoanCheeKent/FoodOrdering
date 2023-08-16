#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <cstring>
using namespace std;

void clrscr();
void get_quit();
double get_opening();
double get_cash();
double get_card();

int main ()
{
	int password = 20220202;
	int pass;
	do
	{
		clrscr();
		cout<<"------------------------------------------------\n";
		cout<<"                STAFF LOGIN   \n";
		cout<<"------------------------------------------------\n";
		cout<<"Enter Password : ";
		cin>>pass;
		cin.clear();
		cin.ignore(100, '\n');
		if (pass != 20220202)
		{
			cout<<"\nInvalid Password ! Please try again \n";
			get_quit();
		}
			
	} while (pass != 20220202);
	clrscr();
	cout<<"----------------------------------------------\n";
	cout<<"           Daily Sales Report               \n";
	cout<<"----------------------------------------------\n";
	double opening = get_opening();
	double cash=get_cash();
	double card=get_card();
	clrscr();
	cout<<"----------------------------------------------\n";
	cout<<"           Daily Sales Report               \n";
	cout<<"----------------------------------------------\n";
	cout<<"\nCash Sales\n";
	cout<<"----------------------------------------------";
	cout<<"\nTotal Cash       : "<<fixed<<setw(25)<<setprecision(2)<<opening+cash;
	cout<<"\nOpening          : "<<fixed<<setw(25)<<setprecision(2)<<opening;
	cout<<"\n----------------------------------------------";
	cout<<"\nGross Cash Sales : "<<fixed<<setw(25)<<setprecision(2)<<cash;
	cout<<"\n\n\n\nCard Sales\n";
	cout<<"----------------------------------------------";
	cout<<"\nGross Card Sales : "<<fixed<<setw(25)<<setprecision(2)<<card;
	cout<<"\n\n\n\nSales Summary\n";
	cout<<"----------------------------------------------";
	cout<<"\nTotal Sales Today : "<<fixed<<setw(24)<<setprecision(2)<<cash+card<<endl;
	cout<<"----------------------------------------------\n";
}

void clrscr()
{
	system("cls");
}

void get_quit ()
{
	char option;
	do
	{
		cout << ("\n(Q)uit: ");
		cin >> option;
		cin.clear();
		cin.ignore(100, '\n');
	} while (option != 'Q' && option != 'q');
}

double get_opening()
{
	double cash;
	cout<<"\nWhat is the opening cash amount ? \n";
	cout<<"\nOpening Cash = ";
	cin>>cash;
	return cash;
}

double get_cash()
{	
	double total=0;
	ifstream read_file ("cash.txt"); 
	if (!read_file)
		cout << "Error accessing cash sales !\n";
	else 
	{
		double sale;
		read_file>>sale; //Initialise
		while(read_file)
		{
			total=total+sale;
			read_file>>sale;
		}
	}
	read_file.close();
	ofstream write_file ("cash.txt"); //To clear the sales history
    write_file.close();
	return total;
}

double get_card()
{	
	double total=0;
	ifstream read_file ("card.txt"); 
	if (!read_file)
		cout << "Error accessing card sales !\n";
	else 
	{
		double sale;
		read_file>>sale;
		while(read_file)
		{
			total=total+sale;
			read_file>>sale;
		}
	}
	read_file.close();
	ofstream write_file ("card.txt"); //To clear the sales history
	write_file.close();
	return total;
}






