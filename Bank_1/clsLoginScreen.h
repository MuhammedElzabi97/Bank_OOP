#pragma once

#include <iostream>
#include "clsScreen.h"
#include "clsUser.h"
#include <iomanip>
#include "clsMainScreen.h"
#include "Global.h"

class clsLoginScreen : protected clsScreen
{
private:	

	static bool _LoginScreen()
	{
		bool LoginFailed = false;
		string UserName, Password;
		short FailedLoginCount = 0;

		do
		{
			if (LoginFailed)
			{
				FailedLoginCount++;
				cout << "\nInvalid UserName / Password.\n\n";
				cout << "you have " << (3 - FailedLoginCount) << " Trials to login" << endl;
			}

			if (FailedLoginCount == 3)
			{
				cout << "You are locked after " << FailedLoginCount << " failed trials." << endl;
				return false;
			}
			
			cout << "Enter User Name : ";
			cin >> UserName;

			cout << "Enter Password : ";
			cin >> Password; 

			CurrentUser = clsUser::Find(UserName, Password);
			LoginFailed = CurrentUser.IsEmpty();	

		} while (LoginFailed);

		CurrentUser.RegisterLogIn();
		clsMainScreen::ShowMainMenu();
	}

public :

	static bool ShowLoginScreen()
	{
		system("cls");
		_DrawScreenHeader("\t    Login Screen");
		return	_LoginScreen();
	}
};