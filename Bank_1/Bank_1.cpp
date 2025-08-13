#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

string ClientsFileName = "h.txt";
string UserFileName = "U.txt";

void ShowMainMenue();
void ShowTransctoinsMenue();
bool UserEnter();
void Login();
void GoBackToLogginSecreen();
void GoBackToMainMenue();
void ShowManageUsersMenue();

enum enMainMenueOptions
{
	eListClints = 1, eAddNewClient = 2,
	eDeleteClient = 3, eUpdateClient = 4,
	eFindeClient = 5, eShowTransactionsMenue = 6, eManageUsers = 7, eLogout = 8
};

enum enManageUserMenueOptions
{
	eListUser = 1, eAddNewUser = 2,
	eDeleteUser = 3, eUpdateUser = 4,
	eFindeUser = 5, eMainMenu = 6
};

enum enTransctionsMenueOptions {
	eDeposit = 1, eWithdraw = 2
	, eShowTotalBalance = 3, eShowMainMenue = 4
}; 

enum enMenuPermssitons
{
	eAll = -1, pListClient = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdatClient = 8
	, pFindClient = 16, pTranacitons = 32, pManageUsers = 64
};

struct sClient
{
	string AccountNumber; 
	string PinCode; 
	string Name; 
	string Phone; 
	double AccountBalance; 
	bool MarkForDelete = false;
};

struct sUser
{
	short Permissions;
	string Password;
	string UserName;
	bool MarkForDelete = false;
};

sUser CurrentUser;

vector <string> SplitString(string Str, string Delim = "#//#")
{
	vector <string> vString;
	short pos = 0;
	string sword ;
	while ((pos = Str.find(Delim)) != std::string::npos)
	{
		sword = Str.substr(0, pos);
		if (sword != "")
		{
			vString.push_back(sword);
		}
		Str.erase(0, pos + Delim.length());
	}
	if (Str != "")
	{
		vString.push_back(Str);
	}
	return vString;
}

sClient ConvertLineToRecord(string Line, string Separator = "#//#")
{
	sClient Client;
	vector <string> vClientData;
	vClientData = SplitString(Line, Separator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);

	return Client;
}

sUser ConvertLineUserToRecord(string Line, string Separator = "#//#")
{
	sUser User;
	vector <string> vUserData;
	vUserData = SplitString(Line, Separator);

	User.UserName = vUserData[0];
	User.Password = vUserData[1];
	User.Permissions = stoi(vUserData[2]);

	return User;
}

string ConvertRecordUserToLine(sUser User, string Separator = "#//#")
{
	string stUserRecord = "";
	stUserRecord += User.UserName + Separator;
	stUserRecord += User.Password + Separator;
	stUserRecord += to_string(User.Permissions);

	return stUserRecord;
}

string ConvertRecordToLine(sClient Client, string Separator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Separator;
	stClientRecord += Client.PinCode + Separator;
	stClientRecord += Client.Name + Separator;
	stClientRecord += Client.Phone + Separator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}

bool CheckAccessPermssions(enMenuPermssitons Permsstion)
{
	if (CurrentUser.Permissions == enMenuPermssitons::eAll)
		return true;

	if ((Permsstion & CurrentUser.Permissions) == Permsstion)
		return true;
	else
		return false;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	vector<sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line; 
		sClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);// ?
		}
		MyFile.close();
	}
	return false;
}

bool UserExistByUserName(string UserName, string FileName)
{
	vector <sUser> vUsers;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sUser User;

		while (getline(MyFile, Line))
		{
			User = ConvertLineUserToRecord(Line);

			if (User.UserName == UserName)
			{
				MyFile.close();
				return true;
			}
			vUsers.push_back(User);
		}
		MyFile.close();
	}
	return false;
}

void ShowAccessDeniedMassage()
{

	cout << "\n_______________________________________________________\n";
	cout << "Access Denied , \nYou Don't have Permsstions to do this \nPlease Contact Your admin.";
	cout << "\n_______________________________________________________\n";
}

short ReadPermsstionsToSet()
{
	short Permission = 0;
	char Answer='n';

	cout << "Do you want to give full access (Y/N) : " << endl;
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		return -1;
	}

	cout << "Do you want to give access to : " << endl;

	cout << "Show Client List (Y/N) : ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permission += enMenuPermssitons::pListClient;
	}

	cout << "Add New Client (Y/N) : ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permission += enMenuPermssitons::pAddNewClient;
	}

	cout << "Delete Client (Y/N) : ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permission += enMenuPermssitons::pDeleteClient;
	}

	cout << "Update Client Informations (Y/N) : ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permission += enMenuPermssitons::pUpdatClient;
	}

	cout << "Find Client (Y/N) : ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permission += enMenuPermssitons::pFindClient;
	}

	cout << "Transactions (Y/N) : ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permission += enMenuPermssitons::pTranacitons;
	}

	cout << "Manage Users (Y/N) : ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		Permission += enMenuPermssitons::pManageUsers;
	}

	return Permission;
}

sUser  ReadNewUser()
{
	sUser User;
	bool FullAccess = false;

	cout << "Enter User Name : ";
	getline(cin >> ws, User.UserName);

	while (UserExistByUserName(User.UserName, UserFileName))
	{
		cout << "\nUser with [" << User.UserName << " ] already exists , enter another user name : ";
		getline(cin >> ws, User.UserName);
	}

	cout << "Enter Password : ";
	cin >> User.Password;

	User.Permissions = ReadPermsstionsToSet();

	return User;
}

sClient  ReadNewClient()
{
	sClient Client;


	cout << "Enter Account Number : ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber,ClientsFileName))
	{
		cout << "\nClient with [" << Client.AccountNumber << " ] already exists , enter another account number : ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter Pin Code : ";
	cin >> Client.PinCode;

	cin.ignore();

	cout << "Enter Name : ";
	getline(cin, Client.Name);

	cout << "Enter Phone : ";
	cin >> Client.Phone;

	cout << "Enter Accoutn Balance : ";
	cin >> Client.AccountBalance;

	return Client;
}

sUser EnterUserData()
{
	sUser User;

	cout << "Enter user name :";
	getline(cin >> ws, User.UserName);

	cout << "Enter password : ";
	getline(cin >> ws, User.Password);
	return User;
}

vector <sClient> LoadClientsDataFromFile(string FileName)
{
	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

vector <sUser> LoadUsersDataFromFile(string FileName)
{
	vector <sUser> vUsers;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sUser User;

		while (getline(MyFile, Line))
		{
			User = ConvertLineUserToRecord(Line);
			vUsers.push_back(User);
		}
		MyFile.close();
	}
	return vUsers;
}

void PrintClientRecordLine(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
	
}

void PrintUserRecordLine(sUser User)
{
	cout << "| " << setw(15) << left << User.UserName;
	cout << "| " << setw(10) << left << User.Password;
	cout << "| " << setw(40) << left << User.Permissions;
}

void ShowAllClientScreen()
{
	if (!CheckAccessPermssions(enMenuPermssitons::pListClient))
	{
		ShowAccessDeniedMassage();
		GoBackToMainMenue();
		return;
	}
	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Clients.";
	cout << "\n_______________________________________________________";
	cout << "_______________________________________________________\n\n";

	cout << "| " << left << setw(15) <<"Account Number";
	cout << "| " << left << setw(10) <<"Pin Code";
	cout << "| " << left << setw(40) <<"Name";
	cout << "| " << left << setw(12) <<"Phone";
	cout << "| " << left << setw(12) <<"Account Balance";

	cout << "\n_______________________________________________________";
	cout << "_______________________________________________________\n\n";

	if (vClients.size() == 0)
	{
		cout << "\t\t\tNo Clietns Available in the system";
	}
	else
	{
		for (sClient Client : vClients)
		{
			PrintClientRecordLine(Client);
			cout << endl;
		}

		cout << "\n______________________________________________________________________________________________________________";
		cout << "\n______________________________________________________________________________________________________________";

	}

}

void ShowAllUsersScreen()
{
	vector <sUser> vUsers = LoadUsersDataFromFile(UserFileName);
	cout << "\n\t\t\t\t\tClient List (" << vUsers.size() << ") Users.";
	cout << "\n_______________________________________________________";
	cout << "_______________________________________________________\n\n";

	cout << "| " << left << setw(15) <<"User Name";
	cout << "| " << left << setw(10) <<"Password";
	cout << "| " << left << setw(40) <<"Permissions";

	cout << "\n_______________________________________________________";
	cout << "_______________________________________________________\n\n";

	if (vUsers.size() == 0)
	{
		cout << "\t\t\tNo Users Available in the system";
	}
	else
	{
		for (sUser User : vUsers)
		{
			PrintUserRecordLine(User);
			cout << endl;
		}

		cout << "\n______________________________________________________________________________________________________________";
		cout << "\n______________________________________________________________________________________________________________";

	}

}

void PrintClientRecordBalanceLine(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.AccountBalance;

}

void ShowTotalBalaces()
{
	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	
	cout << "\n\t\t\t\tBalance List (" << vClients.size() << ") Clients";
	cout << "\n____________________________________________________";
	cout << "____________________________________________________\n";
	cout << "| " <<left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n____________________________________________________";
	cout << "____________________________________________________\n";

	double TotaleBlance = 0;

	if  (vClients.size() == 0)
	{
		cout << "\t\t\t\tClients Avaliable in the system";
	}
	else
	{
		for (sClient Client : vClients)
		{
			PrintClientRecordBalanceLine(Client);
			TotaleBlance += Client.AccountBalance;

			cout << endl;
		}

		cout << "\n____________________________________________________";
		cout << "____________________________________________________\n";
		cout << "\t\t\t\t\t\t\t\tTotal Balances = " << TotaleBlance;
	}
}

void PrintClientCard(sClient Client)
{
	cout << "\nThe following are the client details : \n";
	cout << "_______________________________________________________";
	cout << "\nAccout Number : " << Client.AccountNumber;
	cout << "\nPin Code : " << Client.PinCode;
	cout << "\nName : " << Client.Name;
	cout << "\nPhone : " << Client.Phone;
	cout << "\nAccount Balance : " << Client.AccountBalance;
	cout << "\n_______________________________________________________\n";

}

void PrintUserCard(sUser User)
{
	cout << "\nThe following are the user details : \n";
	cout << "_______________________________________________________";
	cout << "\nUser Name : " << User.UserName;
	cout << "\nPassword : " << User.Password;
	cout << "\nPermissions : " << User.Permissions;
	cout << "\n_______________________________________________________\n";

}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
	for (sClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool FindUserByUserName(string UserName, vector <sUser> vUsers, sUser& User)
{
	vUsers = LoadUsersDataFromFile(UserFileName);

	for (sUser U : vUsers)
	{
		if (U.UserName == UserName )
		{
			User = U;
			return true;
		}
	}
	return false;
}

bool FindUserByUserNameAndPassword(string UserName, string Password, sUser& User)
{
	vector <sUser> vUsers = LoadUsersDataFromFile(UserFileName);

	for (sUser U : vUsers)
	{
		if (U.UserName == UserName && U.Password == Password)
		{
			User = U;
			return true;
		}
	}
	return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
	sClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\n\nEnter Pin Code : ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Phone : ";
	getline(cin >> ws, Client.Phone);

	cout << "Enter Name : ";
	getline(cin >> ws, Client.Name);

	cout << "Enter Account Balance : ";
	cin >>  Client.AccountBalance;

	return Client;
}

sUser ChangeUserRecord(string UserName)
{
	sUser User;
	User.UserName = UserName;

	cout << "\n\nEnter Password : ";
	getline(cin >> ws, User.Password);

	User.Permissions = ReadPermsstionsToSet();

	return User;
}

bool MarkClientForDelteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	for (sClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

bool MarkUserForDelteByUserName(string UserName, vector <sUser>& vUsers)
{
	for (sUser& U : vUsers)
	{
		if (U.UserName == UserName)
		{
			U.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector <sClient> SaveCleitnDataToFile(string FileName, vector <sClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	
	string DataLine;

	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}

vector <sUser> SaveUserDataToFile(string FileName, vector <sUser> vUsers)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	
	string DataLine;

	if (MyFile.is_open())
	{
		for (sUser U : vUsers)
		{
			if (U.MarkForDelete == false)
			{
				DataLine = ConvertRecordUserToLine(U);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vUsers;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}

void AddNewClient()
{
	sClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewUser()
{
	sUser User;
	User = ReadNewUser();
	AddDataLineToFile(UserFileName, ConvertRecordUserToLine(User));
}

void AddNewUsers()
{
	char AddMore = 'Y';
	do
	{
		cout << "Adding New User\n\n";
		AddNewUser();
		cout << "\nUser Added Successfully , do you want to add more users (Y/N) :  ";
		cin >> AddMore;

	} while (toupper(AddMore)=='Y');
}

void AddNewClients()
{
	char addMore = 'Y';
	do
	{
		cout << "Adding new Client :\n\n";
		AddNewClient();
		cout << "\nClient Added successfully , do you want to add more clietns ? Y/N ?";
		cin >> addMore;
	} while (towupper(addMore) == 'Y');
}

bool DeleteUserByUserName(string UserName, vector <sUser>& vUsers)
{
	sUser User;
	char Answr = 'n';

	if (UserName == "Admin")
	{
		cout << "\nYou can't delete this user.";
		return false;
	}
	if (FindUserByUserName(UserName, vUsers, User))
	{
		PrintUserCard(User);

		cout << "\n\nAre you sure you want delete this user ? y/n?";
		cin >> Answr;
		
		if (Answr == 'y' || Answr == 'Y')
		{
			MarkUserForDelteByUserName(UserName, vUsers);
			SaveUserDataToFile(UserFileName, vUsers);
			
			vUsers = LoadUsersDataFromFile(UserFileName);

			cout << "\n\nUser Deleted Successfully.";
			return true; 
		}
	}
	else
	{
		cout << "\nUser  (" << UserName << ") is Not Found!";
		return false;
	}
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	sClient Client;
	char Answr = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want delete this client ? y/n?";
		cin >> Answr;
		
		if (Answr == 'y' || Answr == 'Y')
		{
			MarkClientForDelteByAccountNumber(AccountNumber, vClients);
			SaveCleitnDataToFile(ClientsFileName, vClients);
			
			vClients = LoadClientsDataFromFile(ClientsFileName);

			cout << "\n\nClient Deleted Successfully.";
			return true; 
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}

bool UpdateUserByUserName(string UserName, vector <sUser>& vUsers)
{
	sUser User;
	char Answer = 'n';

	if (FindUserByUserName(UserName, vUsers, User))
	{
		PrintUserCard(User);
		cout << "\n\nAre sure you want update this user ? y/n?";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			for (sUser& U : vUsers)
			{
				if (U.UserName == UserName)
				{
					U = ChangeUserRecord(UserName);
					break;
				}
			}
			SaveUserDataToFile(UserFileName, vUsers);
			cout << "\n\nUser Updated Successfully.";
		}
	}
	else
	{
		cout << "\nUser (" << UserName << ") is Not Found!";
		return false;
	}
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		cout << "\n\nAre sure you want update this client ? y/n?";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			for (sClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveCleitnDataToFile(ClientsFileName, vClients);
			cout << "\n\nClient Updated Successfully.";
		}
	}
	else
	{
		cout << "\nClient with account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}

bool LoadUserInfo(string UsreName , string Password)
{
	if (FindUserByUserNameAndPassword(UsreName, Password,CurrentUser))
		return true;
	else
		return false;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{
	char Answer = 'n';
	cout << "\n\nAre you sure you want perfrom this transation? y/n ?";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		for (sClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveCleitnDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Succssfully. New balance is : " << C.AccountBalance;
				return true;

			}
		}
		return false;
	}
}

string ReadUserName()
{
	string UserName = "";

	cout << "\nPlease enter User Name : ";
	cin >> UserName;
	return UserName;
}

string ReadClientAccountNumber()
{
	string AccountNumber = "";

	cout << "\nPlease enter Account Number : ";
	cin >> AccountNumber;
	return AccountNumber;
}

void ShowDeletUserScreen()
{
	cout << "\n______________________________________\n";
	cout << "\tDelete User Screen";
	cout << "\n______________________________________\n";

	vector <sUser> vUsers = LoadUsersDataFromFile(UserFileName);
	string UserName = ReadUserName();
	DeleteUserByUserName(UserName, vUsers);
}

void ShowDeletClientScreen()
{

	if (!CheckAccessPermssions(enMenuPermssitons::pDeleteClient))
	{
		ShowAccessDeniedMassage();
		GoBackToMainMenue();
		return;
	}

	cout << "\n______________________________________\n";
	cout << "\tDelete Client Screen";
	cout << "\n______________________________________\n";

	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateUserScreen()
{
	cout << "\n______________________________________\n";
	cout << "\nUpdate User Information Screen";
	cout << "\n______________________________________\n";
	
	vector <sUser> vUsers = LoadUsersDataFromFile(UserFileName);
	string UserName = ReadUserName();
	UpdateUserByUserName(UserName, vUsers);
}

void ShowUpdateClientScreen()
{

	if (!CheckAccessPermssions(enMenuPermssitons::pUpdatClient))
	{
		ShowAccessDeniedMassage();
		GoBackToMainMenue();
		return;
	}

	cout << "\n______________________________________\n";
	cout << "\nUpdate Client Information Screen";
	cout << "\n______________________________________\n";
	
	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{

	if (!CheckAccessPermssions(enMenuPermssitons::pFindClient))
	{
		ShowAccessDeniedMassage();
		GoBackToMainMenue();
		return;
	}

	cout << "\n______________________________________\n";
	cout << "\nFind Client Screen";
	cout << "\n______________________________________\n";
	
	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	sClient Client;
	string AccountNumber = ReadClientAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		PrintClientCard(Client);
	else
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
}

void ShowFindUserScreen()
{

	cout << "\n______________________________________\n";
	cout << "\nFind User Screen";
	cout << "\n______________________________________\n";
	
	vector <sUser> vUsers = LoadUsersDataFromFile(UserFileName);
	sUser User;
	string UserName = ReadUserName();
	if (FindUserByUserName(UserName, vUsers, User))
		PrintUserCard(User);
	else
		cout << "\nUser (" << UserName << ") is Not Found!";
}

void ShowAddNewClientsScreen()
{

	if (!CheckAccessPermssions(enMenuPermssitons::pAddNewClient))
	{
		ShowAccessDeniedMassage();
		GoBackToMainMenue();
		return;
	}

	cout << "\n______________________________________\n";
	cout << "\nAdd New Clients Screen";
	cout << "\n______________________________________\n";

	AddNewClient();
}

void ShowAddNewUserScreen()
{
	cout << "\n______________________________________\n";
	cout << "\nAdd New User Screen";
	cout << "\n______________________________________\n";

	AddNewUsers();
}

void ShowLogoutScreen()
{
	GoBackToLogginSecreen();
}

void ShowDepositSecreen()
{
	cout << "\n____________________________________________\n";
	cout << "\tDeposit Screen";
	cout << "\n____________________________________________\n";

	sClient Client;

	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";

		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nenter deposit amount : ";
	cin >> Amount;
	
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

	
}

void ShowTotaleBalanceScreen()
{
	ShowTotalBalaces();
}

short ReadMainMenueOption()
{
	short Choice = 0;
	do
	{
		cout << "Choose what do you want to do? [1 to 8] : ";
		cin >> Choice;
		return Choice;
	} while (Choice < 1 || Choice >8);
}

short ReadManageUserMenuOption()
{
	short Choice = 0;
	do
	{
		cout << "Choose what do you want to do? [1 to 6] : ";
		cin >> Choice;
		return Choice;
	} while (Choice < 1 || Choice >6);
}

void ShowWithDrawScreen()
{
	cout << "\n_______________________________________________\n";
	cout << "\twithdraw Screen";
	cout << "\n_______________________________________________\n";

	sClient Client;

	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClinet with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter withdraw amount : ";
	cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount exceeds the balance , you can withdraw up to : " << Client.AccountBalance << endl;
		cout << "Please enter another amount : ";
		cin >> Amount;
	}
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to main menue ....";
	system("pause>0");
	ShowMainMenue();
}

void GoBackToManageUserMenu()
{
	cout << "\n\nPress any key to go back to main menue ....";
	system("pause>0");
	ShowManageUsersMenue();
}

void GoBackToTransctionsMenue()
{
	cout << "\n\nPress any key to go back to Transctions Menue...";
	system("pause>0");
	ShowTransctoinsMenue();
}

void GoBackToLogginSecreen()
{
	Login();
}

short ReadTransctionsMenueOption()
{
	cout << "Choose what do you want to do? [1 to 4] : ";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}

void PerfromTranactionsMenueOption(enTransctionsMenueOptions TransctionsMenueOptions)
{
	switch (TransctionsMenueOptions)
	{
		case
		enTransctionsMenueOptions::eDeposit:
		{
			system("cls");
			ShowDepositSecreen();
			GoBackToTransctionsMenue();
			break;
		}
		case enTransctionsMenueOptions::eWithdraw:
		{
			system("cls");
			ShowWithDrawScreen();
			GoBackToTransctionsMenue();
			break;
		}
		case enTransctionsMenueOptions::eShowTotalBalance:
		{
			system("cls");
			ShowTotalBalaces();
			GoBackToTransctionsMenue();
			break;
		}
		case enTransctionsMenueOptions::eShowMainMenue:
		{
			ShowMainMenue();
		}
	}
}

void ShowTransctoinsMenue()
{

	if (!CheckAccessPermssions(enMenuPermssitons::pListClient))
	{
		ShowAccessDeniedMassage();
		GoBackToMainMenue();
		return;
	}

	system("cls");
	cout << "==========================================\n";
	cout << "\t\tTransctions Menue Screen\n";
	cout << "==========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdaraw.\n";
	cout << "\t[3] Total Balance.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "==========================================\n";

	PerfromTranactionsMenueOption((enTransctionsMenueOptions)ReadTransctionsMenueOption());
}

void PreFromManageUserMenueOption(enManageUserMenueOptions ManageUserMenueOptions)
{
	switch (ManageUserMenueOptions)
	{
	case enManageUserMenueOptions::eListUser:
	{
		system("cls");
		ShowAllUsersScreen();
		GoBackToManageUserMenu();
		break;
	}
	case enManageUserMenueOptions::eAddNewUser:
		system("cls");
		ShowAddNewUserScreen();
		GoBackToManageUserMenu();
		break;

	case enManageUserMenueOptions::eDeleteUser:
		system("cls");
		ShowDeletUserScreen();
		GoBackToManageUserMenu();
		break;

	case enManageUserMenueOptions::eUpdateUser:
		system("cls");
		ShowUpdateUserScreen();
		GoBackToManageUserMenu();
		break;

	case enManageUserMenueOptions::eFindeUser:
		system("cls");
		ShowFindUserScreen();
		GoBackToManageUserMenu();
		break;

	case enManageUserMenueOptions::eMainMenu:
		system("cls");
		GoBackToMainMenue();
		break;

	}
}

void PreFromMainMenueOption(enMainMenueOptions MainMenueOption)
{
	switch (MainMenueOption)
	{
	case enMainMenueOptions::eListClints:
	{
		system("cls");
		ShowAllClientScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eAddNewClient:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::eDeleteClient:
		system("cls");
		ShowDeletClientScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::eUpdateClient:
		system("cls");
		ShowUpdateClientScreen();	
		GoBackToMainMenue();

		break;

	case enMainMenueOptions::eFindeClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::eShowTransactionsMenue:
		system("cls");
		ShowTransctoinsMenue();
		break;

	case enMainMenueOptions::eManageUsers:
		system("cls");
		ShowManageUsersMenue();
		break;

	case enMainMenueOptions::eLogout:
		system("cls");
		ShowLogoutScreen();
		break;
	}
}

void ShowMainMenue()
{
	system("cls");
	cout << "=====================================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "=====================================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] DeleteClient.\n";
	cout << "\t[4] Update Client Information.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "=====================================================\n";

	PreFromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

void ShowManageUsersMenue()
{
	if (!CheckAccessPermssions(enMenuPermssitons::pManageUsers))
	{
		ShowAccessDeniedMassage();
		GoBackToMainMenue();
		return;
	}

	system("cls");
	cout << "=====================================================\n";
	cout << "\t\tManage Menu Screen\n";
	cout << "=====================================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find Find.\n";
	cout << "\t[6] Main Menu.\n";
	cout << "=====================================================\n";

	PreFromManageUserMenueOption((enManageUserMenueOptions)ReadManageUserMenuOption());
}

void Login()
{
	bool LoginFaild = false;
	string UserName, PassWord;

	do
	{
		system("cls");
		cout << "________________________________________________\n";
		cout << "\t\tLogin Screen\n";
		cout << "\n________________________________________________\n";

		if (LoginFaild)
			cout << "\nInvalid Password/UserName\n\n";
	
		cout << "Enter User Name : ";
		cin >> UserName;

		cout << "Enter Password : ";
		cin >> PassWord;

		LoginFaild = !LoadUserInfo(UserName, PassWord);

	} while (LoginFaild);

	ShowMainMenue();	
}

int main()
{
	Login();
	system("pause>0");
	return 0;
}