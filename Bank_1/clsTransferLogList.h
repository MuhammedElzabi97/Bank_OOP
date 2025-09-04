#pragma once
#include "clsScreen.h"
#include "clsBankClient.h"
#include<iostream>
#include<iomanip>


class clsTransferLogList : protected clsScreen
{

private:
	static void _PrintTanrsferLogList(clsBankClient::stTransferLog TransferLogRecord)
	{
		cout << setw(8) << left << "" << "| " << setw(35) << left<<TransferLogRecord.DateTime ;
		cout << "| " << setw(10) << left <<TransferLogRecord.sAccountNumber;
		cout << "| " << setw(10) << left <<TransferLogRecord.dAccountNumber;
		cout << "| " << setw(10) << left <<TransferLogRecord.Amount;
		cout << "| " << setw(10) << left <<TransferLogRecord.sAccountBalance;
		cout << "| " << setw(10) << left <<TransferLogRecord.dAccountBalance;
		cout << "| " << setw(10) << left <<TransferLogRecord.UserName;	
	}
    
public:

	static void ShowTransferLogList()
	{
		vector<clsBankClient::stTransferLog> vTransferLogRecord = clsBankClient::GetTransferLogList();


        string Title = "\tTransfer Register List Screen";
        string SubTitle = "\t\t(" + to_string(vTransferLogRecord.size()) + ") Record(s).";

        _DrawScreenHeader(Title, SubTitle);

        cout << setw(8) << left << "" << "\n\t_______________________________________________________________";
        cout << "_____________________________________________\n" << endl;

        cout << setw(8) << left << "" << "| " << left << setw(35) << "Date/Time";
        cout << "| " << left << setw(10) << "s.Acct";
        cout << "| " << left << setw(10) << "d.Acct";
        cout << "| " << left << setw(10) << "Amount";
        cout << "| " << left << setw(10) << "s.Balance";
        cout << "| " << left << setw(10) << "d.Balance";
        cout << "| " << left << setw(10) << "User";
        cout << setw(8) << left << "" << "\n\t_______________________________________________________________";
        cout << "_____________________________________________\n" << endl;

        if (vTransferLogRecord.size() == 0)
            cout << "\t\t\t\tNo Logins Available In the System!";
        else

            for (clsBankClient::stTransferLog Record : vTransferLogRecord)
            {
                _PrintTanrsferLogList(Record);
                cout << endl;
            }

        cout << setw(8) << left << "" << "\n\t_______________________________________________________";
        cout << "______________________________________________________\n" << endl;

	}
};

