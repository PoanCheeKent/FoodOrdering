#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <string>
//#include <sstream> //for data validation
//#include <cstring>
#include <cmath> // for power()
#define MAX_MENU_SIZE 100
using namespace std;

typedef struct
{
	char name[50]; //name of product
	double price; //price of product
} PRODUCT;

typedef struct
{
	int number; //correspond to item number in menu
	int quantity; //quantity of same item
	double price; //price of the item
} ORDER_ITEM;

//Functions Declaration
void clrscr();
int get_menu	(PRODUCT menu[], int number);
void show_menu	(PRODUCT menu[], int number);
int order_item	(ORDER_ITEM list[], bool item_id[], int index, int volume);
double show_order	(const PRODUCT menu[], const ORDER_ITEM list[], bool item_id[], int index, int volume); //Print the order and return total bill
void get_quit();
int get_table();
int data_validate ();
char data_check ();
double write_receipt(const PRODUCT menu[], const ORDER_ITEM list[], bool item_id[], int index,int table);
void write_order(const PRODUCT menu[], const ORDER_ITEM list[], bool item_id[], int index, int table);
void get_sales(double sum);

int main()
{
	PRODUCT menu[MAX_MENU_SIZE];
	int index = get_menu(menu, MAX_MENU_SIZE); //number of item in menu
	int choice, volume = 0; //choice for selecting in menu; volume for number of types of customer's order.
	bool item_id [index] = {0}; //the id for the item ordered by customer.
	bool fail = false; //to avoid entering char into integer type, choice.
	// menu starts
	do	
	{
		ORDER_ITEM list[index];
		clrscr();
		cout << "\nWelcome to Restaurant Programming\n";
		cout << "Choice:\n";
		cout << "1. Show menu\n";
		cout << "2. Place order\n";
		cout << "3. Show ordered items\n";
		cout << "4. Confirm Order\n";
		cout << "5. Exit\n";
		do
		{
			cout << "\nEnter choice: ";
			cin >> choice;
			fail = cin.fail();
			if (fail)
				cout << "Please enter a valid choice!" << endl;
			cin.clear();
			cin.ignore(10, '\n');
		} while(fail);
		clrscr();
	
		switch (choice)
		{
			case 1: 
				char option;
				do
				{
					show_menu(menu, index); 
					cout << ("\n(Q)uit: ");
					cin >> option;
				} while (option!='Q' && option!='q');
				break;
			case 2: 
				show_menu(menu, index); 					 
				volume = order_item(list, item_id, index, volume);
				break;
			case 3:
				{
					char option2;
					do
					{
						double total = show_order(menu, list, item_id, index, volume);	
						if (volume > 0)
						{
							cout << ("\n\nIs the order correct (Y/N) : ");
							cin >> option2;
						}
						else
							option2 = 'Y';
					} while (option2 != 'Y' && option2 != 'y' && option2 != 'N' && option2 != 'n');
					
					if ((option2 == 'Y' || option2 == 'y') && volume >0)
						cout << "\nYour order has been recorded!";
					
					else if (option2 == 'N' || option2 == 'n')
					{
						for (int i = 0; i < index; i++) //Clear the ordered item
						{
							list[i].number = 0;
							list[i].price = 0;
							list[i].quantity = 0;
							item_id[i] = false;
							volume = 0;
						}
						cout<<"\nYou may now ORDER again !!";
					}
					get_quit();
					break;
				}
			case 4: 
				{	
					clrscr();
					if (volume >0)
					{
						int table = get_table();
						double sum = write_receipt(menu, list, item_id, index,table);
						write_order(menu, list, item_id, index,table);
						get_sales(sum);
						for (int i = 0; i < index; i++) //Clear the ordered item so that next customer can order
						{
							list[i].number = 0;
							list[i].price = 0;
							list[i].quantity = 0;
							item_id[i] = false;
							volume = 0;
						}
					}
					else
					{
						cout<<"\nPlease make an order first !!";
						get_quit();
					}
				}
				break;
			case 5:
				clrscr();
				cout << "\nThank you, please come again ! \n";
				break;
			default: 
				cout << "Invalid choice\n"; 
				break;
		}
	} while (choice != 5);
	
	return 0;
}

void clrscr()
{
	system("cls");
}

int get_menu (PRODUCT menu[], int number)
{	
	ifstream read_file ("menu.txt");  //To open file for input
	int index = -1;
	if (!read_file)
		cout << "Error accessing the menu!\n";
	else 
	{	
		read_file.getline(menu[++index].name, 50); //To store name of food
		while(read_file)
		{
			read_file >> menu[index].price; //To store price of food
		
			// clear unwanted whitespace
			while (read_file.peek() == '\n')
				read_file.ignore(256, '\n');
			
			read_file.getline(menu[++index].name, 50); //read next item
		} 
	}
	read_file.close();
	
	return index;
}

void show_menu (PRODUCT menu[],int number)
{
	clrscr();
	cout<<"\n                             MENU                            \n";
	cout <<"------------------------------------------------------------\n"
		<< "NO    Name                                            Price\n"
		 << "------------------------------------------------------------\n";
	for (int i = 0; i < number; i++)
	{	
		cout	<< right << setw(2) << i + 1 << ". "
				<< left << setw(50) << menu[i].name
				<< fixed << setprecision(2)
				<< setw(6) << right << menu[i].price << endl;
	}
	cout << endl;
}

int order_item	(ORDER_ITEM list[], bool item_id[], int index, int volume)
{
	int count = -1, quantity = 0; //count + 1 is number of times user order item 
	bool ans = false; // To check whether user want to add new item.
	// ans1 is answer from user regarding additional order for same item
	// ans2 is answer from user regarding to add new item in list.
	char ans1, ans2;
	
	while (!ans)
	{
		do
		{
			cout << "\nWhat do you want to order?\n";
			cout << "Item NO : ";
			list[++count].number = data_validate ();
			
			while (list[count].number < 1 || list[count].number > index)
			{
				cout << "Please only enter 1 - " << index << endl;
				cin.clear();
				cout << "Item NO : ";
				list[count].number = data_validate ();
			}
				
			if (item_id[list[count].number - 1] == true)
			{
				do
				{
					cout << "You have ordered " << list[list[count].number - 1].quantity << " for this item.\n"
						 << "Would you like to add? Y/N\n";
					fflush(stdin);
					ans1 = data_check ();
					if (ans1 == 'Y' || ans1 == 'y')
					{
						cout << "How much more would you like to add?\n";
						quantity = data_validate ();
						if (quantity < 1)
							cout << "Quantity cannot be less than 1\n";
					}
					
					if (ans1 == 'N'|| ans1 == 'n')
						quantity =0;
					
					if (ans1 != 'Y' && ans1 != 'y' && ans1 != 'N' && ans1 != 'n')
						cout << "Please only enter Y or N\n";
				} while (ans1 != 'Y' && ans1 != 'y' && ans1 != 'N' && ans1 != 'n');
			}
			else
			{
				do
				{
				cout << "Quantity : ";
				quantity = data_validate ();
				if (quantity < 1)
					cout << "Quantity cannot be less than 1\n";
				} while (quantity < 1);
			}
			
			if (list[list[count].number - 1].quantity == 0)
			{
				item_id[list[count].number - 1] = true,
				volume++;
			}
			
			list[list[count].number - 1].quantity += quantity;

			if (list[list[count].number - 1].quantity >= 100)
				cout << "Maximum order per item is 100\n";
				
			} while (list[list[count].number - 1].quantity >= 100);
		
		do
		{
			cout << "Do you still want to order? Y/N\n";
			ans2 = data_check ();
			if (ans2 == 'N' || ans2 == 'n')
				ans = true;
			
			if (ans2 != 'Y' && ans2 != 'y' && ans2 != 'N' && ans2 != 'n')
						cout << "Please only enter Y or N\n";
		} while (ans2 != 'Y' && ans2 != 'y' && ans2 != 'N' && ans2 != 'n');
	}
	cout << endl;
	
	return volume;	
}

double show_order	(const PRODUCT menu[], const ORDER_ITEM list[], bool item_id[], int index, int volume)
{
	double total = 0;
	clrscr();
	
	if(volume <= 0)
	{
		cout << "\nNo record.\n";
	}
	else
	{
		cout << " No              Name                              Quantity\n"
			 << "------------------------------------------------------------\n";
		
		int i = 0;	
		for (int j = 0; j < index; j++)
			if (item_id[j])
			{
				cout << right << setw(2) << ++i  << ". " << left << setw(50);
				cout << menu[j].name << setw(2) << right << list[j].quantity << endl;
				
				total = total + menu[j].price * list[j].quantity;
			}
		cout << "\nTotal = RM " << total;
	}
	
	return total;
}

int get_table()
{
	int table = 0;
	while (table <= 0 || table > 10) 
	{
		cout<<"\nEnter your table number (1-10): ";
		cin >> table;
		if (table <= 0 || table > 10)
		{
			cout << "Invalid Table Number \n";
			cout << "Please key in between 1-10";
			get_quit();
			clrscr();
		}
	}
	return table;
}

void get_quit ()
{
	char option;
	do
	{
		cout << ("\n(Q)uit: ");
		option = data_check();
	} while (option != 'Q' && option != 'q');
}

int data_validate () //for integer
{
	char line[100]; //line for input
	char str1[100], str2[100];
	int a = 0; //output
	int num_len = 0; //length of number entered
	
	fflush(stdin);
	cin.get(line, 99);
	
	// to get the first word before space (i.e. if user enter "12w 23", this loop will return "12w")
	for (int i = 0; line[i] != ' ' && line[i] != '\0'; i++) 
    	str1[i] = line[i];
	
	// to get numbers until first character is detected 
	// (i.e. following the example given above, "12w" will turn into "12" after this loop)
    for (int i = 0; str1[i] >= '0' && str1[i] <= '9'; i++) 
    	{
    		str2[i] = str1[i];
			num_len++;
		}
		
	// convert str2 to integer
	// (i.e. following the example given above, "12" (array) will turn into 12 (integer) after this loop)
	for (int j = 0; num_len > 0; j++) // convert str2 to integer
		{
			a += (str2[j] - '0') * pow(10,num_len - 1);
			num_len--;
		}

	return a;   
}

char data_check () //for char
{
	char line[100];
	
	fflush(stdin);
	cin.get(line, 99);
	
	return line[0];    
}

double write_receipt(const PRODUCT menu[], const ORDER_ITEM list[], bool item_id[], int index, int table)
{
	double sum = 0;
	
	ofstream write_file("receipt.txt");
	if (!write_file)
		cout<<"Error writing receipt ! ";
	else
	{
		write_file << "                                   RESTAURANT PROGRAMMING                                      \n";
		write_file << "Table NO : " << table << endl;
		write_file << "---------------------------------------------------------------------------------------------------\n";
		write_file << "\n No              Name                                  Price           Quantity       Total\n";
		write_file << "---------------------------------------------------------------------------------------------------\n";
		
		int i = 0;	

		for (int j = 0; j < index; j++)
			if (item_id[j])
			{
				write_file << right << setw(2) << ++i  << ". " << left << setw(50);
				write_file << menu[j].name <<fixed<<setprecision(2) << setw(6) << right << menu[j].price << "\t\t  " << list[j].quantity;
				write_file <<fixed << setprecision(2) << setw(16) << menu[j].price * list[j].quantity << endl;
				sum = sum + menu[j].price * list[j].quantity;
			}
		write_file << "---------------------------------------------------------------------------------------------------\n";
		write_file << " \n\nTotal Bill : \t\t\t\t\t\t\t\t\t   RM " << setw(5) << sum << endl;
		write_file << "---------------------------------------------------------------------------------------------------\n";
		cout << "\nReceipt is printed !!";
	}
	write_file.close();
	
	return sum;
	
}

void write_order(const PRODUCT menu[], const ORDER_ITEM list[], bool item_id[], int index, int table) //ORDER SEND TO KITCHEN
{
	ofstream write_file("kitchen.txt");
	if (!write_file)
		cout<<"Error sending order to kitchen ! ";
	else
	{
		write_file	<< " Table No : " << table;
		write_file	<< "\n No              Name                              Quantity\n"
			 		<< "------------------------------------------------------------\n";
		
		int i = 0;	
		for (int j = 0; j < index; j++)
			if (item_id[j])
			{
				write_file << right << setw(2) << ++i  << ". " << left << setw(50);
				write_file << menu[j].name << setw(2) << right << list[j].quantity << endl;
			}
	}
	write_file.close();
}

void get_sales(double sum)
{
	int payment;
	do
	{
		cout << "\n\nPayment Option : ";
		cout << "\n1. Cash ";
		cout << "\n2. Card ";
		cout << "\nPayment Option (1/2)= ";
		cin >> payment;
		if (payment < 1 || payment > 2)
		{
			clrscr();
			cout<<"\n Please enter 1 or 2 only \n";
		}
	} while (payment < 1 || payment > 2);
	
	if (payment == 1)
	{
		ofstream write_file("cash.txt",ios::app);
		if (!write_file)
			cout << "Error adding cash sales ! ";
		else
		{
			write_file << sum << endl;
		}
		write_file.close();
	}
	else
	{
		ofstream write_file("card.txt",ios::app);
		if (!write_file)
			cout<<"Error adding card sales ! ";
		else
		{
			write_file << sum <<endl;
		}
		write_file.close();
	}
	clrscr();
	cout<<"\n\nPlease enjoy your meal , we wish to serve you again in the future !! ^ ^ " ;
	cout<<"\n\n ------------------------Thank You ------------------------------\n";
	get_quit();
}
		

