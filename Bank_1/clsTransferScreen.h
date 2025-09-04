#pragma once
#include <iostream>
#include "clsScreen.h"
#include "clsBankClient.h"
#include "clsInputValidate.h" 
#include "Global.h"
 
class clsTransferScreen : protected clsScreen
{
private:

    static void _PrintClient(clsBankClient Client)
    {
        cout << "\nClient Card:";
        cout << "\n___________________";
        cout << "\nFull Name   : " << Client.FullName();
        cout << "\nAcc. Number : " << Client.AccountNumber();
        cout << "\nBalance     : " << Client.AccountBalance;
        cout << "\n___________________\n";
    }

    static string _ReadAccountNumber(string Message)
    {
        string AccountNumber;
        cout <<Message ;
        AccountNumber = clsInputValidate::ReadString();
        while (!clsBankClient::IsClientExist(AccountNumber))
        {
            cout << "\nAccount Number is Not Found, choose another one : ";
            AccountNumber = clsInputValidate::ReadString();
        }
        return AccountNumber;
    }

    static float _ReadAmount(clsBankClient SourceClient)
    {
        float Amount;
        cout << "\nEnter Transfer Amount : ";
        Amount = clsInputValidate::ReadFloatNumber();
        while (Amount > SourceClient.AccountBalance)
        {
            cout << "\nAmount Exceeds the available Balance, Enter another Amount ? ";
            Amount = clsInputValidate::ReadFloatNumber();
        }
        return Amount;
    }

public:

    static void ShowTransfer()
    {
        _DrawScreenHeader("\t   Transfer Screen");

        clsBankClient SourceClient = clsBankClient::Find(_ReadAccountNumber("\nPlease enter account number to transfer from : "));
        _PrintClient(SourceClient);
        
        clsBankClient DestinationClient = clsBankClient::Find(_ReadAccountNumber("\nPlease enter account number to transfer to : "));
        _PrintClient(DestinationClient);

        float Amount = _ReadAmount(SourceClient);

        cout << "\nAre you sure you want to perform this operation? y/n? ";
        char Answer = 'n';
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            if (SourceClient.Transfer(Amount, DestinationClient,CurrentUser.UserName))
            {
                cout << "\nTransfer done successfully\n";
            }
            else
            {
                cout << "\nTransfer failed \n";
            }
        }

        _PrintClient(SourceClient);
        _PrintClient(DestinationClient);
     
    }
};

