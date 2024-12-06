/**------------------------------------------
    Program 1: Library Management System
    Course: CS 251, Fall 2023, UIC
    System: Windows 10 and VS Code
    Author: Alex Arroyo
 ---------------------------------------------**/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

using namespace std;

// Function prototypes for all the glorious function decomposition
void printMenu();
string returnStatus(string statusNum);
string calcValue(string ISBN);

int main()
{
    string command;
    string title;
    string isbn;
    string databaseIn;
    string databaseOut;
    string bookInfo;
    string choice;
    string search;

    ifstream infile;
    ofstream outfile;

    int start;
    int end;
    int count;
    int library;
    int checked;
    int loaned;
    int unknown;
    int other;
    int found;

    vector<string> bookTitles;
    vector<string> bookISBNs;
    vector<string> bookStatuses;

    cout << "Welcome to the Library Management System" << endl;
    cout << "----------------------------------------" << endl;
    cout << endl;

    do 
    {
        // Output the menu and acquire a user selection
        printMenu();
        cout << endl << "Enter a command (case does not matter): ";

        // We use getline for all user input to avoid needing to handle
        // input buffer issues relating to using both >> and getline
        getline(cin, command);
        cout << endl;

        //Add item
        if(command == "a" || command == "A"){
            cout << "What is the book title? " << endl;
            getline(cin, title);

            //If there are no commas found
            if (title.find(",") == string::npos){
                cout << "What is the 13-digit ISBN (with hyphens)? " << endl;
                getline(cin, isbn);

                //Add book info to vectors
                bookTitles.push_back(title);
                bookISBNs.push_back(isbn);
                bookStatuses.push_back("0");
                cout << "The Following Entry Was Added" << endl;
                cout << "-----------------------------" << endl;
                cout << bookTitles.at(bookTitles.size() - 1) << " --- ";
                cout << bookISBNs.at(bookISBNs.size() - 1) << " --- ";
                cout << returnStatus(bookStatuses.at(bookStatuses.size() - 1)) << endl;
            }

            //Else print error message
            else{
                cout << "The book title cannot contain commas." << endl;
            }      
        }

        //Clear library
        else if(command == "c" || command == "C"){
            bookTitles.clear();
            bookISBNs.clear();
            bookStatuses.clear();
            cout << "Your library is now empty." << endl;
        }

        //Display database
        else if(command == "d" || command == "D"){
            
            cout << "Your Current Library" << endl;
            cout << "--------------------" << endl;

            //If there are any books, print out entire library
            if (bookTitles.size() > 0){
                for (int i = 0; i < bookTitles.size(); i++){
                    cout << bookTitles.at(i) << " --- ";
                    cout << bookISBNs.at(i) << " --- ";
                    cout << returnStatus(bookStatuses.at(i)) << endl; 
                }
            }

            //Else print error message
            else {
                cout << "The library has no books." << endl;
            }
        }

        //List invalid entries
        else if(command == "i" || command == "I"){
            found = 0;
            cout << "The Following Library Entries Have Invalid Data" << endl;
            cout << "-----------------------------------------------" << endl;

            //If the library has no books, print error message
            if (bookTitles.size() == 0){
                cout << "The library has no books." << endl;
            }

            else{
                for (int i = 0; i < bookTitles.size(); i++){

                    //Print entry if ISBN is not the appropriate size
                    if (bookISBNs.at(i).size() != 17){
                        cout << bookTitles.at(i) << " --- ";
                        cout << bookISBNs.at(i) << " --- ";
                        cout << returnStatus(bookStatuses.at(i)) << endl;
                        found++;
                    }
   
                    //Print entry if the book status is invalid
                    else if (returnStatus(bookStatuses.at(i)) == "Invalid State"){
                        cout << bookTitles.at(i) << " --- ";
                        cout << bookISBNs.at(i) << " --- ";
                        cout << returnStatus(bookStatuses.at(i)) << endl;
                        found++;
                    }
                }

                //If no invalid entries were found, print error message
                if (found == 0){
                    cout << "The library has no invalid entries." << endl;
                }
            }
        }

        //Load library file
        else if(command == "l" || command == "L"){

            //Read and open database
            cout << "What database to read from? " << endl;
            getline(cin, databaseIn);
            infile.open(databaseIn);

            //Print error message if file does not open
            if (!infile.is_open()) {
                cout << "Could not find the database file." << endl;
            }

            //Else take and parse info from text file and place in associated vectors
            else{
                getline(infile, bookInfo);
                int count = 0;
                while (!infile.fail()) {

                    //Retrieve start and end indexes to create substrings for book title
                    start = 0;
                    end = bookInfo.find(",", start);
                    bookTitles.push_back(bookInfo.substr(start, end - start));

                    //If no comma is found to indicate isbn, set isbn to default value
                    if(end == string::npos){
                        bookISBNs.push_back("000-0-00-000000-0");
                    }

                    //Else update start and end indexes and obtain isbn
                    else{
                        start = end + 2;
                        end = bookInfo.find(",", end + 1);
                        bookISBNs.push_back(bookInfo.substr(start, end - start));
                    }

                    //If no comma is found to indicate status, set status to default value
                    if(end == string::npos){
                        bookStatuses.push_back("0");
                    }

                    //Else update start and end indexes and obtain status
                    else{
                        start = end + 2;
                        end = bookInfo.size();
                        bookStatuses.push_back(bookInfo.substr(start, end - start));
                    }
                    count++;
                    getline(infile, bookInfo);
                }
            infile.close();
            cout << "Read in " << count << " lines from the file." << endl;
            }
        }

        //Output library to file
        else if(command == "o" || command == "O"){

            //read and open output file
            cout << "Where should the database output to? " << endl;
            getline(cin, databaseOut);
            outfile.open(databaseOut);

            //print error if file does not open
            if (!outfile.is_open()) {
                cout << "Could not find the output file."<< endl;
            }

            //Else output library to output file
            else{
                for (int i = 0; i < bookTitles.size(); i++){
                    outfile << bookTitles.at(i) << ", ";
                    outfile << bookISBNs.at(i) << ", ";
                    outfile << bookStatuses.at(i) << endl; 
                }

                outfile.close();
                cout << bookTitles.size() <<  " lines of data written to " << databaseOut << endl;
            }
        }

        //Print checkout stats
        else if(command == "p" || command == "P"){

            //reset status values
            library = 0;
            checked = 0;
            loaned = 0;
            unknown = 0;
            other = 0;

            cout << "Your Current Library's Stats" << endl;
            cout << "----------------------------" << endl;
            cout << "Total Books: " << bookTitles.size() << endl;

            //Read through book status vector to count each type of status
            for (int i = 0; i < bookStatuses.size(); i++){
                if (bookStatuses.at(i) == "0"){
                    library++;
                }
                else if (bookStatuses.at(i) == "1"){
                    checked++;
                }
                else if (bookStatuses.at(i) == "2"){
                    loaned++;
                }
                else if (bookStatuses.at(i) == "3"){
                    unknown++;
                }
                else{
                    other++;
                }
            }
            cout << "\tIn Library: " << library << endl;
            cout << "\tChecked Out: " << checked << endl;
            cout << "\tOn Loan: " << loaned << endl;
            cout << "\tUnknown: " << unknown << endl;
            cout << "\tOther: " << other << endl;
        }

        //Remove library entry
        else if(command == "r" || command == "R"){
            found = 0;
            cout << "Would you like remove by (1) name or (2) ISBN." << endl;
            cout << "Enter the numeric choice: " << endl;
            getline(cin, choice);
            if (choice == "1"){
                cout << "Enter the book name: " << endl;
                getline(cin, search);
                cout << "The Following Entry Was Removed From The Library" << endl;
                cout << "------------------------------------------------" << endl;
                for (int i = bookTitles.size() - 1; i >= 0; i--){
                    if (bookTitles.at(i) == search && found == 0){
                        cout << bookTitles.at(i) << " --- ";
                        cout << bookISBNs.at(i) << " --- ";
                        cout << returnStatus(bookStatuses.at(i)) << endl;
                        bookTitles.erase(bookTitles.begin() + i);
                        bookISBNs.erase(bookISBNs.begin() + i);
                        bookStatuses.erase(bookStatuses.begin() + i);
                        found = 1;
                    }
                }
                if (found == 0){
                    cout << "No matching entry found in the library." << endl;
                }
            }
            else if (choice == "2"){
                cout << "Enter the book 13-digit ISBN (with dashes): " << endl;
                getline(cin, search);
                cout << "The Following Entry Was Removed From The Library" << endl;
                cout << "------------------------------------------------" << endl;
                for (int i = bookISBNs.size() - 1; i >= 0; i--){
                    if (bookISBNs.at(i) == search && found == 0){
                        cout << bookTitles.at(i) << " --- ";
                        cout << bookISBNs.at(i) << " --- ";
                        cout << returnStatus(bookStatuses.at(i)) << endl;
                        bookTitles.erase(bookTitles.begin() + i);
                        bookISBNs.erase(bookISBNs.begin() + i);
                        bookStatuses.erase(bookStatuses.begin() + i);
                        found = 1;
                    }
                }
                if (found == 0){
                    cout << "No matching entry found in the library." << endl;
                }
            }
            else{
                cout << "Invalid remove by choice option." << endl;
            }
        }

        //Search for library entry
        else if(command == "s" || command == "S"){
            found = 0;
            cout << "Would you like to search by (1) name or (2) ISBN." << endl;
            cout << "Enter the numeric choice: " << endl;
            getline(cin, choice);
            if (choice == "1"){
                cout << "Enter the book name: " << endl;
                getline(cin, search);
                cout << "The Following Are Your Search Results" << endl;
                cout << "-------------------------------------" << endl;
                for (int i = bookTitles.size() - 1; i >= 0; i--){
                    if (bookTitles.at(i) == search && found == 0){
                        cout << bookTitles.at(i) << " --- ";
                        cout << bookISBNs.at(i) << " --- ";
                        cout << returnStatus(bookStatuses.at(i)) << endl;
                        found = 1;
                    }
                }
                if (found == 0){
                    cout << "No matching entry found in the library." << endl;
                }
            }
            else if (choice == "2"){
                cout << "Enter the book 13-digit ISBN (with dashes): " << endl;
                getline(cin, search);
                cout << "The Following Are Your Search Results" << endl;
                cout << "-------------------------------------" << endl;
                for (int i = bookTitles.size() - 1; i >= 0; i--){
                    if (bookISBNs.at(i) == search && found == 0){
                        cout << bookTitles.at(i) << " --- ";
                        cout << bookISBNs.at(i) << " --- ";
                        cout << returnStatus(bookStatuses.at(i)) << endl;
                    found = 1;
                    }
                }
                if (found == 0){
                    cout << "No matching entry found in the library." << endl;
                }
            }
            else{
                cout << "Invalid search by choice option." << endl;
            }
        }

        //Unknown command error message
        else if (!(command == "x" || command == "X")){
            cout << "Unknown command." << endl;
            printMenu();
        }

        cout << endl;

        //Exit program
    } while (!(command == "x" || command == "X"));

    return 0;
}

//Calculate a check-digit value from an ISBN
string calcValue(string ISBN){
    
}

/// @brief print out the main menu of the management system
void printMenu()
{
    cout << "Library Management Menu" << endl;
    cout << "-----------------------" << endl;
    cout << "A - Add Item To Library" << endl;
    cout << "C - Clear The Library Of All Entries" << endl;
    cout << "D - Display Library Entries" << endl;    
    cout << "I - List Invalid Library Entries" << endl;
    cout << "L - Load Library From File" << endl;    
    cout << "O - Output Library To File" << endl;
    cout << "P - Print Out Checkout Stats" << endl;
    cout << "R - Remove A Library Entry" << endl;      
    cout << "S - Search For A Library Entry" << endl;    
    cout << "X - Exit Program" << endl;
}

//Return the status of a book based on their status number
string returnStatus(string statusNum){
    string status;
    if(statusNum == "0"){
        status = "In Library";
    }
    else if(statusNum == "1"){
        status = "Checked Out";
    }
    else if(statusNum == "2"){
        status = "On Loan";
    }
    else if(statusNum == "3"){
        status = "Unknown State";
    }
    else{
        status = "Invalid State";
    }
    return status;
}