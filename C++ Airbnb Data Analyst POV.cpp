/***
//// Last Build : 9_11_17
//// Compiler   : VS2019
//// Programmer : Yeo Yee Wen
//// Capability : Implementing a variable sized records for Airbnb so that records printed with inconsistent data can be imputed.
//// Function   : add, delete, update, search, display records
***/


/////////////     Includes    /////////////

#include <cstdlib> // system()
#include <conio.h> // getch()
#include <fstream>
#include <istream>
#include <string>
#include <sys\stat.h> // stat(status of a file) 
#include <iostream>
#include <ctime> //time()
#include <iomanip>
#pragma warning(disable: 4996)
using namespace std;


/////////////     Data types    /////////////
int AirbnbID = 0;
int searchAirbnbID;


struct Airbnb_record // Airbnb record variables
{
	int AirbnbID;
	int menuchoice;
	int package;
	int currentYear;
	int currentMonth;
	int currentDay;
	int currentHour;
	int currentMinute;
	int currentSecond;
	int now_year;
	int birth_year;
	string id;
	string name;
	string host_id;
	string neighbourhood_group;
	string neighbourhood;
	string lattitude;
	string longitude;
	string roomtype;
	string price;
	string minimumnights;
	string reviewnumber;
	string last_review;
	string reviews_per_month;
	string calculated_host_listings_count;
	bool deleted;
};


/////////////     Variable Declarations & Constants /////////////

#define AIRBNB_FILE_NAME "Airbnbs.txt" // name of the database file to store Airbnbs informations

Airbnb_record Airbnb;
char     choice; // for choice in menu
fstream *fs = NULL, *fs1 = NULL;// file streams for files : fs -> 'Airbnb', fs1 -> 'temp'
bool     deletion = false; // if any record has been deleted


						   /////////////     Function Prototypes    /////////////

void closeFile(fstream *); // closes a file with its pointer, then deletes the file pointer
bool isFileExist(const char *); // check if a file exists
double point(int marks); // GPA calculation
void current_time(); // Current time


					 /////////////     Main    /////////////
int main()
{
	ifstream input; //declaration for the "id.txt"
	input.open("id.txt"), ios::in;
	input >> AirbnbID;
	ofstream output;

	char ch;
	string pass = "";
	string keyUsrName = "";
	string password = "12345";
	string username = "Airbnb";

	cout << "\n";
	cout << "\n";
	cout << "\t#################################################################" << endl;
	cout << "																													" << endl;
	cout << "																												" << endl;
	cout << "\t\t\t\tWelcome To													" << endl;
	cout << "\t\t\tAirbnb Records Management System								" << endl;
	cout << "\t\t\t\t(New York)															" << endl;
	cout << "																										" << endl;
	cout << "																										" << endl;
	cout << "\t#################################################################" << endl;
	cout << "\n" << endl;

	do {

		pass = "";
		keyUsrName = "";
		cout << "\t\t Username:  ";
		cin >> keyUsrName;
		cout << "\t\t Password:  ";
		char pass[32];//to store password.
		int i = 0;
		char a;//a Temp char
		for (i = 0;;)//infinite loop
		{
			a = _getch();//stores char typed in a
			if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9'))
				//check if a is numeric or alphabet
			{
				pass[i] = a;//stores a in pass
				++i;
				cout << "*";


			}
			if (a == '\b' && i >= 1)//if user typed backspace
								  //i should be greater than 1.
			{
				cout << "\b \b";//rub the character behind the cursor.
				--i;
			}
			if (a == '\r')//if enter is pressed
			{
				pass[i] = '\0';//null means end of string.
				break;//break the loop
			}
		}
		cout << "\t\t You Entered : ";
		cout << "\t " << pass;
		cout << "\n\n";

		cout << endl;

		if (pass == password && keyUsrName == username)
		{
			cout << "\n\t\t Log In Successful!!" << endl;
			system("pause");
			goto mainmenu;
		}
		else
		{
			cout << "\n\t\tInvalid Username and Password" << endl;
			system("pause");
		}
	} while (pass != password || keyUsrName != username);

	system("pause");
	system("cls");

	while (true)
	{
		do ////// Menu //////
		{
		mainmenu:
			system("cls"); // clear screen

			cout << " \n\t-----------[ Airbnbs Record Inventory Database System ]-----------\n";
			cout << " \t[1] Add a new Record \n";
			cout << " \t[2] Search an existing Record\n";
			cout << " \t[3] Modify and update an existing Record\n";
			cout << " \t[4] Delete an existing Record \n";
			cout << " \t[5] Display Records \n";
			cout << " \t[6] Exit \n\n";
			cout << " \t\tEnter a choice (1-6) : " << flush;
			cout << "\n----------[ Airbnb Datatype Record ]--------------------------------------------------------------------------------------\n";
			cout << "Airbnb_ID                      : Numeric" << endl;
			cout << "Name                           : Character" << endl;
			cout << "Host_ID                        : Numeric" << endl;
			cout << "Neighbourhood_Group            : Character" << endl;
			cout << "Neighbourhood                  : Character" << endl;
			cout << "Lattitude                      : Numeric" << endl;
			cout << "Longitude                      : Numeric" << endl;
			cout << "RoomType                       : Character" << endl;
			cout << "Price                          : Numeric" << endl;
			cout << "Minimum_Nights                 : Numeric" << endl;
			cout << "Number_Of_Reviews              : Numeric" << endl;
			cout << "Last_Review                    : Character" << endl;
			cout << "Reviews_per_month              : Numeric" << endl;
			cout << "Calculated_Host_Listings_Count : Numeric" << endl;

			cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
			choice = _getch();



		} while (choice < '1' || choice > '6'); // while we have no good(between 1 and 6), show menu again

		system("cls");

		// to search, modify, delete or display records, database file should exist, then we have some records           
		if (choice == '2' || choice == '3' || choice == '4' || choice == '5')
		{
			if (!isFileExist(AIRBNB_FILE_NAME)) // if database file doesn't exist
			{
				cout << "\n-----------------------------------------------------------------------------------------------------------------" << endl;
				cout << "\t Database file ('" << AIRBNB_FILE_NAME << "') doesn't exist, then there are no records.";
				cout << "\n-----------------------------------------------------------------------------------------------------------------" << endl;

				system("pause");
				continue; // show the menu again
			}
		}

		time_t now = time(0); // time
		tm *ltm = localtime(&now); // time

		switch (choice)
		{
			int recs_num; // number of records before the record for modifying(deletion)
			int id;

		case '1': ////// Add Record //////
		loopaddcase:
			cout << "\n----------[ ADD RECORD ]-----------------------------------------------------------------------------------------\n";
			cout << "\t Enter the following information for the new record.";
			cout << "\n-----------------------------------------------------------------------------------------------------------------";
			Airbnb.AirbnbID = ++AirbnbID;
			cout << "\nRecordingID\t\t\t\t\t: " << Airbnb.AirbnbID << endl;
			if (output.open("id.txt"), ios::out) //Create "id.txt" to store id
			{
				output << AirbnbID;
			}
			else
			{
				cout << "\n----------[ ADD RECORD ]-----------------------------------------------------------------------------------------\n";
				cout << "\t Sorry, the file is empty. :[";
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
			}

			output.close();
			cout << "\n";

			cout << "Airbnb_ID                      :";
			cin >> Airbnb.id;
			while (Airbnb.id.find_first_of("1234567890") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "Airbnb_ID                     : ";
				cin >> Airbnb.id;
			}
			

			cout << "Name                           :";
			cin >> Airbnb.name;
			while (Airbnb.name.find_first_of("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLMNBVCXZ1234567890") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "Name                           :";
				cin >> Airbnb.name;
			}


			cout << "Host_ID                        :";
			cin >> Airbnb.host_id;
			while (Airbnb.host_id.find_first_of("1234567890") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "Host_ID                        :";
				cin >> Airbnb.host_id;
			}

			cout << "Neighbourhood_Group            :";
			cin >> Airbnb.neighbourhood_group;
			while (Airbnb.neighbourhood_group.find_first_of("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLMNBVCXZ1234567890") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "Neighbourhood_Group            :";
				cin >> Airbnb.neighbourhood_group;
			}

			cout << "Neighbourhood                  :";
			cin >> Airbnb.neighbourhood;
			while (Airbnb.neighbourhood.find_first_of("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLMNBVCXZ1234567890") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "Neighbourhood                  :";
				cin >> Airbnb.neighbourhood;
			}

			cout << "Lattitude                      :";
			cin >> Airbnb.lattitude;
			while (Airbnb.lattitude.find_first_of("1234567890") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "Lattitude                      :";
				cin >> Airbnb.lattitude;
			}

			cout << "Longitude                      :";
			cin >> Airbnb.longitude;
			while (Airbnb.longitude.find_first_of("1234567890") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "Longitude                      :";
				cin >> Airbnb.longitude;
			}

			cout << "RoomType                       :";
			cin >> Airbnb.roomtype;
			while (Airbnb.roomtype.find_first_of("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLMNBVCXZ1234567890") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "RoomType                       :";
				cin >> Airbnb.roomtype;
			}

			cout << "Price                          :$";
			cin >> Airbnb.price;
			while (Airbnb.price.find_first_of("1234567890") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "Price                          :$";
				cin >> Airbnb.price;
			}

			cout << "Minimum_Nights                 :";
			cin >> Airbnb.minimumnights;
			while (Airbnb.minimumnights.find_first_of("1234567890") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "Minimum_Nights                 :";
				cin >> Airbnb.minimumnights;
			}

			cout << "Number_Of_Reviews              :";
			cin >> Airbnb.reviewnumber;
			while (Airbnb.reviewnumber.find_first_of("1234567890") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "Number_Of_Reviews              :";
				cin >> Airbnb.reviewnumber;
			}

			cout << "Last_Review                    :";
			cin >> Airbnb.last_review;
			while (Airbnb.last_review.find_first_of("1234567890qwertyuiopasdfghjklmnbvcxzQWERTYUIOPASDFGHJKLZXCVBNM") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "Last_Review                    :";
				cin >> Airbnb.last_review;
			}

			cout << "Reviews_per_month              :";
			cin >> Airbnb.reviews_per_month;
			while (Airbnb.reviews_per_month.find_first_of("1234567890") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "Reviews_per_month              :";
				cin >> Airbnb.reviews_per_month;
			}

			cout << "Calculated_Host_Listings_Count :";
			cin >> Airbnb.calculated_host_listings_count;
			while (Airbnb.calculated_host_listings_count.find_first_of("1234567890") == string::npos)
			{
				cout << "========ERROR===================================================" << endl;
				cout << "\t Invalid Input! Please enter a valid ID" << endl;
				cout << "================================================================" << endl;
				cout << "Calculated_Host_Listings_Count :";
				cin >> Airbnb.calculated_host_listings_count;
			}


			current_time(); // Get current time

			cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
			cout << "This record was created on " << setfill('0') << setw(2) << Airbnb.currentDay << "/" << setfill('0') << setw(2) << Airbnb.currentMonth << "/" << Airbnb.currentYear << "\t" << setfill('0') << setw(2) << Airbnb.currentHour << ":" << setfill('0') << setw(2) << Airbnb.currentMinute << ":" << setfill('0') << setw(2) << Airbnb.currentSecond;

			Airbnb.deleted = 0;

			fs = new fstream(AIRBNB_FILE_NAME, ios::out | ios::app | ios::binary);

			if (!fs)
			{
				cout << "\n----------[ ADD RECORD ]-----------------------------------------------------------------------------------------\n";
				cout << " Can't open or create '" << AIRBNB_FILE_NAME << "' file";
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
				system("pause");
				break;
			}

			cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
			cout << " Do you want to save this record? (1=Yes, 2=No, 3=Save and Add): " << flush;

			char confirm;

		loopadd:
			confirm = getch();
			cin.ignore();
			system("cls");

			if (confirm == '1')
			{
				fs->write((char*)&Airbnb, sizeof(Airbnb));
				system("cls");
				cout << "\n----------[ ADD RECORD ]-----------------------------------------------------------------------------------------\n";
				cout << "\t Record is added. :]" << endl;
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
				system("paused");
				closeFile(fs);
				break;
			}
			if (confirm == '2')
			{
				system("cls");
				cout << "\n----------[ ADD RECORD ]-----------------------------------------------------------------------------------------\n";
				cout << "\t Record is not added. :[" << endl;
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
				system("paused");
				break;
			}
			if (confirm == '3')
			{
				fs->write((char*)&Airbnb, sizeof(Airbnb));
				system("cls");
				cout << "\n----------[ ADD RECORD ]-----------------------------------------------------------------------------------------\n";
				cout << "\t Record is added. :]" << endl;
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
				system("paused");
				closeFile(fs);

				goto loopaddcase;
			}
			else
			{
				cout << "========ERROR===================================================\n";
				cout << "\t Invalid Input! Please enter again" << endl;
				cout << "================================================================";
				cout << "\n Do you want to add this record? (1=Yes, 2=No, 3=Save and Add): " << flush;
				goto loopadd;
			}

			fs->write((char*)&Airbnb, sizeof(Airbnb));

			closeFile(fs);

			cout << "\n----------[ ADD RECORD ]-----------------------------------------------------------------------------------------\n";
			cout << "\n Record added. :]" << endl;
			cout << "\n-----------------------------------------------------------------------------------------------------------------\n";

			system("pause");

			break;

		case '2': ////// Search Record //////

			cout << "\n----------[ SEARCH RECORD ]--------------------------------------------------------------------------------------\n";
			cout << "\t Search an existing Airbnb's information.";
			cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
			cout << "\t\tEnter Airbnb ID: ";
			cin >> searchAirbnbID;
			cin.ignore();

			fs = new fstream(AIRBNB_FILE_NAME, ios::in | ios::out | ios::binary);

			if (!fs)
			{
				system("cls");
				cout << "\n----------[ SEARCH RECORD ]--------------------------------------------------------------------------------------\n";
				cout << "\t Can't open or create '" << AIRBNB_FILE_NAME << "' file";
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
				system("pause");
				goto mainmenu;
				break;
			}

			recs_num = -1;

			while (fs->read((char *)&Airbnb, sizeof(Airbnb)))
			{
				recs_num++;
				if (Airbnb.AirbnbID == searchAirbnbID && !Airbnb.deleted) //if user deleted an Airbnb then added another one with the same ID in the same instance of program runs, deleted Airbnb is still there, then we should go through all the file
					break;
			}

			if (fs->eof()) // if (the record is not in the file || it's there but it's deleted)
			{
				system("cls");
				cout << "\n----------[ SEARCH RECORD ]--------------------------------------------------------------------------------------\n";
				cout << "\t The specified Airbnb doesn't exist in file.";
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
				closeFile(fs);
				system("pause");
				goto mainmenu;
				break;
			}

			fs->seekp(sizeof(Airbnb)*recs_num, ios::beg); //go to the searched record
			fs->read((char*)&Airbnb, sizeof(Airbnb)); //display the searched record

			if (!Airbnb.deleted)
			{
				system("cls");
				cout << "\n----------[ SEARCH RECORD ]--------------------------------------------------------------------------------------\n";
				cout << "\tRecording ID\t: " << Airbnb.AirbnbID << endl;
				
				cout << "Airbnb_ID                      :" << Airbnb.id << endl;
				cout << "Name                           :" << Airbnb.name << endl;
				cout << "Host_ID                        :" << Airbnb.host_id << endl;
				cout << "Neighbourhood_Group            :" << Airbnb.neighbourhood_group << endl;
				cout << "Neighbourhood                  :" << Airbnb.neighbourhood << endl;
				cout << "Lattitude                      :" << Airbnb.lattitude << endl;
				cout << "Longitude                      :" << Airbnb.longitude << endl;
				cout << "RoomType                       :" << Airbnb.roomtype << endl;
				cout << "Price                          :" << Airbnb.price << endl; 		
				cout << "Minimum_Nights                 :" << Airbnb.minimumnights << endl;
				cout << "Number_Of_Reviews              :" << Airbnb.reviewnumber << endl;
				cout << "Last_Review                    :" << Airbnb.last_review << endl;
				cout << "Reviews_per_month              :" << Airbnb.reviews_per_month << endl;
				cout << "Calculated_Host_Listings_Count :" << Airbnb.calculated_host_listings_count << endl;

				cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
			}
			system("pause");
			closeFile(fs);
			break;

		case '3': ////// Update Record //////
			cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
			cout << "\t Modify an existing Airbnb's information.";
			cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
			cout << "\t\tEnter Airbnb ID: ";
			cin >> searchAirbnbID;
			cin.ignore();

			fs = new fstream(AIRBNB_FILE_NAME, ios::in | ios::out | ios::binary);

			if (!fs)
			{
				system("cls");
				cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
				cout << "\t Can't open or create '" << AIRBNB_FILE_NAME << "' file";
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
				system("pause");
				goto mainmenu;
				break;
			}

			recs_num = -1;

			while (fs->read((char *)&Airbnb, sizeof(Airbnb)))
			{
				recs_num++;

				if (Airbnb.AirbnbID == searchAirbnbID && !Airbnb.deleted) //if user deleted an Airbnb then added another one with the same ID in the same instance of program runs, deleted Airbnb data is still there, then we should go through all the file
					break;
			}

			if (fs->eof()) // if (the record is not in the file || it's there but it's deleted)
			{
				system("cls");
				cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
				cout << "\t The specified Airbnb doesn't exist in file.";
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
				closeFile(fs);
				system("pause");
				goto mainmenu;
				break;
			}

			fs->seekp(sizeof(Airbnb)*recs_num, ios::beg); //go to the searched record
			fs->read((char*)&Airbnb, sizeof(Airbnb)); //display the searched record

			if (!Airbnb.deleted)
			{
			update:
				system("cls");
				cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
				cout << "\tRecording ID\t: " << Airbnb.AirbnbID << endl;

				cout << "(1) . Airbnb_ID                      :" << Airbnb.id << endl;
				cout << "(2) . Name                           :" << Airbnb.name << endl;
				cout << "(3) . Host_ID                        :" << Airbnb.host_id << endl;
				cout << "(4) . Neighbourhood_Group            :" << Airbnb.neighbourhood_group << endl;
				cout << "(5) . Neighbourhood                  :" << Airbnb.neighbourhood << endl;
				cout << "(6) . Lattitude                      :" << Airbnb.lattitude << endl;
				cout << "(7) . Longitude                      :" << Airbnb.longitude << endl;
				cout << "(8) . RoomType                       :" << Airbnb.roomtype << endl;
				cout << "(9) . Price                          :" << Airbnb.price << endl;
				cout << "(10). Minimum_Nights                 :" << Airbnb.minimumnights << endl;
				cout << "(11). Number_Of_Reviews              :" << Airbnb.reviewnumber << endl;
				cout << "(12). Last_Review                    :" << Airbnb.last_review << endl;
				cout << "(13). Reviews_per_month              :" << Airbnb.reviews_per_month << endl;
				cout << "(14). Calculated_Host_Listings_Count :" << Airbnb.calculated_host_listings_count << endl;
				cout << "\n";
				cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
				cout << "\nWhich details do you want to edit? (Press 0 to save and return to main menu): ";
				cin >> Airbnb.menuchoice;

				if (Airbnb.menuchoice == 0)
				{
					goto updatefile;
				}

				switch (Airbnb.menuchoice)
				{
				case 1:
					system("cls"); // clear screen
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
					cout << "Airbnb_ID                      :";
					cin >> Airbnb.id;
					while (Airbnb.id.find_first_of("1234567890") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "Airbnb_ID                     : ";
						cin >> Airbnb.id;
					}
					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				case 2:
					system("cls"); // clear screen
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
					cout << "Name                           :";
					cin >> Airbnb.name;
					while (Airbnb.name.find_first_of("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLMNBVCXZ1234567890") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "Name                           :";
						cin >> Airbnb.name;
					}
					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				case 3:
					system("cls"); // clear screen
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
					cout << "Host_ID                        :";
					cin >> Airbnb.host_id;
					while (Airbnb.host_id.find_first_of("1234567890") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "Host_ID                        :";
						cin >> Airbnb.host_id;
					}

					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				case 4: 
					system("cls"); // clear screen
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";

					cout << "Neighbourhood_Group            :";
					cin >> Airbnb.neighbourhood_group;
					while (Airbnb.neighbourhood_group.find_first_of("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLMNBVCXZ1234567890") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "Neighbourhood_Group            :";
						cin >> Airbnb.neighbourhood_group;
					}
					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				case 5: 
					system("cls");
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";

					cout << "Neighbourhood                  :";
					cin >> Airbnb.neighbourhood;
					while (Airbnb.neighbourhood.find_first_of("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLMNBVCXZ1234567890") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "Neighbourhood                  :";
						cin >> Airbnb.neighbourhood;
					}

					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				case 6: 
					system("cls");
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
					cout << "Lattitude                      :";
					cin >> Airbnb.lattitude;
					while (Airbnb.lattitude.find_first_of("1234567890") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "Lattitude                      :";
						cin >> Airbnb.lattitude;
					}

					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				case 7: 
					system("cls"); // clear screen
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
					cout << "Longitude                      :";
					cin >> Airbnb.longitude;
					while (Airbnb.longitude.find_first_of("1234567890") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "Longitude                      :";
						cin >> Airbnb.longitude;
					}

					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				case 8: 
					system("cls"); // clear screen
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
					cout << "RoomType                       :";
					cin >> Airbnb.roomtype;
					while (Airbnb.roomtype.find_first_of("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLMNBVCXZ1234567890") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "RoomType                       :";
						cin >> Airbnb.roomtype;
					}
					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				case 9: 
					system("cls");
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
					cout << "Price                          :";
					cin >> Airbnb.price;
					while (Airbnb.price.find_first_of("1234567890") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "Price                          :$";
						cin >> Airbnb.price;
					}
					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				case 10: 
					system("cls");
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
					cout << "Minimum_Nights                 :";
					cin >> Airbnb.minimumnights;
					while (Airbnb.minimumnights.find_first_of("1234567890") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "Minimum_Nights                 :";
						cin >> Airbnb.minimumnights;
					}
					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				case 11: 
					system("cls");
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";

					cout << "Number_Of_Reviews              :";
					cin >> Airbnb.reviewnumber;
					while (Airbnb.reviewnumber.find_first_of("1234567890") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "Number_Of_Reviews              :";
						cin >> Airbnb.reviewnumber;
					}

					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				case 12: 
					system("cls");
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
					cout << "Last_Review                    :";
					cin >> Airbnb.last_review;
					while (Airbnb.last_review.find_first_of("1234567890QWERTYUIOPASDFGHJKLZXCVBNMqwertyuioplkjhgfdsazxcvbnm") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "Last_Review                    :";
						cin >> Airbnb.last_review;
					}
					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				case 13: 
					system("cls");
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
					cout << "Reviews_per_month              :";
					cin >> Airbnb.reviews_per_month;
					while (Airbnb.reviews_per_month.find_first_of("1234567890") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "Reviews_per_month              :";
						cin >> Airbnb.reviews_per_month;
					}
					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				case 14: 
					system("cls");
					cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
					cout << "Calculated_Host_Listings_Count :";
					cin >> Airbnb.calculated_host_listings_count;
					while (Airbnb.calculated_host_listings_count.find_first_of("1234567890") == string::npos)
					{
						cout << "========ERROR===================================================" << endl;
						cout << "\t Invalid Input! Please enter a valid ID" << endl;
						cout << "================================================================" << endl;
						cout << "Calculated_Host_Listings_Count :";
						cin >> Airbnb.calculated_host_listings_count;
					}
					cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
					current_time();
					system("pause");
					goto update;
					break;

				default:
					goto update;
				}
			}

		updatefile:
			fs->seekp(sizeof(Airbnb) * recs_num, ios::beg); // go to the first of the record to be modified
			fs->write((char*)&Airbnb, sizeof(Airbnb));
			closeFile(fs);
			system("cls");
			current_time();

			cout << "\n----------[ UPDATE RECORD ]--------------------------------------------------------------------------------------\n";
			cout << "\t Your record is updated. THANK YOU :]\t";
			cout << " Last modified on " << setfill('0') << setw(2) << Airbnb.currentDay << "/" << setfill('0') << setw(2) << Airbnb.currentMonth << "/" << setfill('0') << setw(2) << Airbnb.currentYear << "\t";
			cout << setfill('0') << setw(2) << Airbnb.currentHour << ":" << setfill('0') << setw(2) << Airbnb.currentMinute << ":" << setfill('0') << setw(2) << Airbnb.currentSecond;
			cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
			system("pause");


			break;

		case '4': ////// Delete Record //////
			cout << "\n----------[ DELETE RECORD ]--------------------------------------------------------------------------------------\n";
			cout << "\t Delete an exisitng Airbnb's information.";
			cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
			cout << "\t\tEnter Airbnb ID: ";
			cin >> searchAirbnbID;

			while (cin.fail())
			{
				system("cls");
				cout << "\n Enter Airbnb's ID, for deletion : ";
				cin.clear();
				cin.ignore();
				cin >> searchAirbnbID;
			}

			fs = new fstream(AIRBNB_FILE_NAME, ios::in | ios::out | ios::binary);

			if (!fs)
			{
				system("cls");
				cout << "\n----------[ DELETE RECORD ]--------------------------------------------------------------------------------------\n";
				cout << "\t Can't open or create '" << AIRBNB_FILE_NAME << "' file." << endl;
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
				system("pause");
				break;
			}

			recs_num = -1;

			while (fs->read((char *)&Airbnb, sizeof(Airbnb)))
			{
				recs_num++;
				if (Airbnb.AirbnbID == searchAirbnbID && !Airbnb.deleted) // if user deleted an Airbnb then added another one with the same ID in the same instance of program runs, deleted Airbnb is still there, then we should go through all the file
					break;
			}

			if (fs->eof()) // if (the record is not in the file || it's there but it's deleted)
			{
				system("cls");
				cout << "\n----------[ DELETE RECORD ]--------------------------------------------------------------------------------------\n";
				cout << "\t The specified Airbnb doesn't exist in database file.";
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
				closeFile(fs);
				system("pause");
				break;
			}

			Airbnb.deleted = 1;
			fs->seekp(sizeof(Airbnb) * recs_num, ios::beg);
			fs->write((char *)&Airbnb, sizeof(Airbnb));
			closeFile(fs);
			deletion = true; // we have some deleted records
			system("cls");

			cout << "\n----------[ DELETE RECORD ]--------------------------------------------------------------------------------------\n";
			cout << "\t Your specified Airbnb is deleted successfully.";
			cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
			system("pause");
			break;

		case '5': // Display Records
			fs = new fstream(AIRBNB_FILE_NAME, ios::in | ios::binary);

			if (!fs)
			{
				cout << "\n----------[ DISPLAY RECORD ]-------------------------------------------------------------------------------------\n";
				cout << "\t Can't open or create '" << AIRBNB_FILE_NAME << "' file." << endl;
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
				system("pause");
				break;
			}
			fs->read((char *)&Airbnb, sizeof(Airbnb));
			if (Airbnb.deleted && !Airbnb.deleted > 0)
			{
				cout << "\n----------[ DISPLAY RECORD ]-------------------------------------------------------------------------------------\n";
				cout << "\t Sorry, no record is found. :[ You can try enter a new record :]";
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
			}

			while (fs->read((char *)&Airbnb, sizeof(Airbnb))) // display records
			{
				if (!Airbnb.deleted)
				{
					cout << "\n----------[ DISPLAY RECORD ]-------------------------------------------------------------------------------------\n";
					cout << "\tRecording ID\t: " << Airbnb.AirbnbID << endl;

					cout << "Airbnb_ID                      :" << Airbnb.id << endl;
					cout << "Name                           :" << Airbnb.name << endl;
					cout << "Host_ID                        :" << Airbnb.host_id << endl;
					cout << "Neighbourhood_Group            :" << Airbnb.neighbourhood_group << endl;
					cout << "Neighbourhood                  :" << Airbnb.neighbourhood << endl;
					cout << "Lattitude                      :" << Airbnb.lattitude << endl;
					cout << "Longitude                      :" << Airbnb.longitude << endl;
					cout << "RoomType                       :" << Airbnb.roomtype << endl;
					cout << "Price                          :" << Airbnb.price << endl;
					cout << "Minimum_Nights                 :" << Airbnb.minimumnights << endl;
					cout << "Number_Of_Reviews              :" << Airbnb.reviewnumber << endl;
					cout << "Last_Review                    :" << Airbnb.last_review << endl;
					cout << "Reviews_per_month              :" << Airbnb.reviews_per_month << endl;
					cout << "Calculated_Host_Listings_Count :" << Airbnb.calculated_host_listings_count << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------\n";
				}
				
			}

			system("pause");
			closeFile(fs);
			break;

		case '6': // Exit
			if (deletion) // if there is any deletion, then update database file (create a new temp file that doesn't have deleted records, then remove the old database file and rename temp file to database file name)
			{
				cout << "\n----------[ EXIT ]-----------------------------------------------------------------------------------------------\n";
				cout << "\t Updating '" << AIRBNB_FILE_NAME << "' File..." << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------\n";

				fs = new fstream(AIRBNB_FILE_NAME, ios::in | ios::binary);
				if (!fs)
				{
					cout << "\n----------[ EXIT ]-----------------------------------------------------------------------------------------------\n";
					cout << "\n Can't open '" << AIRBNB_FILE_NAME << "' file, then Updating is incomplete." << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------\n";
					system("pause");

					system("cls");
					return 1;
				}

				fs1 = new fstream("temp", ios::out | ios::binary);
				if (!fs1)
				{
					cout << "\n----------[ EXIT ]-----------------------------------------------------------------------------------------------\n";
					cout << "\n Can't create temp file, then Updating is incomplete." << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------\n";
					system("pause");

					closeFile(fs);
					system("cls");
					return 1;
				}

				// write nondeleted records to the temp file
				while (fs->read((char *)&Airbnb, sizeof(Airbnb)))
					if (!Airbnb.deleted)
						fs1->write((char *)&Airbnb, sizeof(Airbnb));

				closeFile(fs);
				closeFile(fs1);

				if (remove(AIRBNB_FILE_NAME) == -1) // if there is an error
				{
					cout << "\n----------[ EXIT ]-----------------------------------------------------------------------------------------------\n";
					cout << "\n Can't delete '" << AIRBNB_FILE_NAME << "' file, then Updating is incomplete.";
					cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
					system("pause");
					system("cls");
					return 1;
				}


				struct stat st; // to check size of the temp file
				int res = stat("temp", &st);

				if (st.st_size == 0) // if all of records are deleted then the temp file size is zero                        
					remove("temp"); // we have no records, then no database file is needed, just delete the temp file
				else
					if (rename("temp", AIRBNB_FILE_NAME))
					{
						cout << "\n----------[ EXIT ]-----------------------------------------------------------------------------------------------\n";
						cout << "\t Can't rename temp file, then Updating is incomplete.";
						cout << "\n-----------------------------------------------------------------------------------------------------------------\n";

						system("pause");

						system("cls");
						return 1;
					}

				cout << "\n----------[ EXIT ]-----------------------------------------------------------------------------------------------\n";
				cout << "\t Updating database file completed.";
				cout << "\n-----------------------------------------------------------------------------------------------------------------\n";

				system("pause");
			}
			cout << "\n----------[ EXIT ]-----------------------------------------------------------------------------------------------\n";
			cout << "\t You have successfully logged out. Thank you :]";
			current_time();
			cout << "\n\t Logout Date Time: " << setfill('0') << setw(2) << Airbnb.currentDay << "/" << setfill('0') << setw(2) << Airbnb.currentMonth << "/" << setfill('0') << setw(2) << Airbnb.currentYear << "\t";
			cout << setfill('0') << setw(2) << Airbnb.currentHour << ":" << setfill('0') << setw(2) << Airbnb.currentMinute << ":" << setfill('0') << setw(2) << Airbnb.currentSecond;
			cout << "\n-----------------------------------------------------------------------------------------------------------------\n";

			return 0;

			break;
		} // end 'switch'
	} // end 'while'

	return 0;
} // end 'main()'


  /////////////     Function Definitions    /////////////

void closeFile(fstream *fs)
{
	fs->close(); // close the file
	delete fs;
	fs = NULL;
}


bool isFileExist(const char * file_name)
{
	struct stat st; // to check status of file
	int res = stat(file_name, &st);
	return (res == 0); // if file exists     
}

string Airbnbs(int marks) // Calculate Airbnb
{
	string Airbnb;
	if (marks >= 90)
		Airbnb = "A+";
	else if (marks >= 80)
		Airbnb = "A";
	else if (marks >= 75)
		Airbnb = "A-";
	else if (marks >= 70)
		Airbnb = "B+";
	else if (marks >= 65)
		Airbnb = "B";
	else if (marks >= 60)
		Airbnb = "B-";
	else if (marks >= 55)
		Airbnb = "C+";
	else if (marks >= 50)
		Airbnb = "C";
	else if (marks >= 45)
		Airbnb = "C-";
	else if (marks >= 40)
		Airbnb = "D";
	else
		Airbnb = "F";

	return Airbnb;
}

double point(int marks) //Calculate GPA
{
	double point;
	if (marks >= 90)
		point = 4.00;
	else if (marks >= 80)
		point = 4.00;
	else if (marks >= 75)
		point = 3.67;
	else if (marks >= 70)
		point = 3.33;
	else if (marks >= 65)
		point = 3.00;
	else if (marks >= 60)
		point = 2.67;
	else if (marks >= 55)
		point = 2.33;
	else if (marks >= 50)
		point = 2.00;
	else if (marks >= 45)
		point = 1.50;
	else if (marks >= 40)
		point = 1.00;
	else
		point = 0.00;

	return point;
}

void current_time()
{
	time_t now = time(0); //time
	tm *ltm = localtime(&now); //time

	Airbnb.currentYear = 1900 + ltm->tm_year;
	Airbnb.currentMonth = 1 + ltm->tm_mon;
	Airbnb.currentDay = ltm->tm_mday;
	Airbnb.currentHour = ltm->tm_hour;
	Airbnb.currentMinute = 1 + ltm->tm_min;
	Airbnb.currentSecond = 1 + ltm->tm_sec;
}
