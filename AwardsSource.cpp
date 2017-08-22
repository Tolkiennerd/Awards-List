#include<iostream>
#include<string>
#include<fstream>
#include<ctime>

const int LIST_SIZE = 100;

using namespace std;

struct story
{
	int rank;
	string name; 
};

void rankUp(story, story, story[]);

int main()
{
	story story1;
	story story2;
	string keepGoing = "y";
	string yn;
	fstream awardFile;
	story theList[LIST_SIZE];
	story theOldList[LIST_SIZE];
	srand(time(0));
	int changes = 0;

	awardFile.open("The Lofgren Awards.txt");

	for (int i = 0; i < LIST_SIZE; i++)
	{
		awardFile >> theList[i].rank;
		getline(awardFile, theList[i].name);
		theOldList[i].rank = theList[i].rank;
		theOldList[i].name = theList[i].name;
	}

	awardFile.close();

	while (true)
	{
		//cout << rand();
		do
		{
			story1 = theList[rand() % LIST_SIZE];
			story2 = theList[rand() % LIST_SIZE];
		} while (story1.rank == story2.rank);
		//cout << flush << string(story2.name) << flush;
		//cout << theList[rand()%LIST_SIZE].name << endl;
		//cout.flush();
		//string question = "Is " + story1.name + " better than " + story2.name + "?\n";
		//cout << flush << question << flush;
		//printf(question);
		//cout << "Is " << story1.name << " better than " << story2.name << "?\n";
		//cout << "test\n";
		//cout << flush << "Is " << flush;
		//cout << flush << theList[0].name.substr(0,story1.name.size()-1) << flush;
		//cout << flush << " better than " << flush;
		//cout << story2.name.substr(0,story2.name.size()-1) << flush;
		//cout << "?" << flush;
		cout << "Is" << story1.name.substr(0,story1.name.size()-1) << " better than" << story2.name.substr(0,story2.name.size()-1) << "? ";
		cin >> yn;
		cout << endl;		

		if (yn == "u")
		{
			for (int i = 0; i < LIST_SIZE; i++)
				theList[i] = theOldList[i];
			cout << "Undone\n";
			cout << "Is" << story1.name << " better than" << story2.name << "?\n";
			cin >> yn;
			changes--;
		}

		for (int i = 0; i < LIST_SIZE; i++)
		{
			theOldList[i] = theList[i];
		}
		if (yn == "y") 
		{
			if (story1.rank > story2.rank)
			{
				rankUp(story1, story2, theList);
				changes++;
			}
		}
		else if (yn == "n")
		{
			if (story2.rank > story1.rank)
			{
				rankUp(story2, story1, theList);
				changes++;
			}
		}
		else if (yn == "add")
		{
			string name;
			int rank;
			bool exists = false;
			cout << "Name: ";
			cin.ignore();
			getline(cin, name);
			name = " " + name;
			for (int i = 0; i < LIST_SIZE; i++)
			{
				if (theList[i].name == name)
				{
					cout << "Story already exists\n";
					exists = true;
				}
			}
			if (!exists) 
			{
				cout << "\nRank: ";
				cin >> rank;
				for (int i = LIST_SIZE - 1; i >= rank; i--)
				{
					theList[i].name = theList[i - 1].name;
				}
				theList[rank - 1].name = name;
			}
			changes++;
		}
		else if (yn == "bottom")
		{
			string name;
			cout << "Move what to bottom?\n";
			cin.ignore();
			getline(cin, name);
			name = " " + name;
			story worst;
			bool exists = false;
			for (int i = 0; i < LIST_SIZE; i++)
			{
				if (theList[i].name == name)
				{
					worst.name = theList[i].name;
					worst.rank = theList[i].rank;
					exists = true;
				}
				else if (i == LIST_SIZE - 1 && exists == false)
				{
					cout << "Story did not exist\n";
				}
			}
			if (exists)
			{
				for (int i = worst.rank - 1; i < LIST_SIZE - 1; i++)
				{
					theList[i].name = theList[i + 1].name;
				}
				theList[LIST_SIZE - 1].name = worst.name;
			}
			changes++;
		}
		else if (yn == "list")
		{
			for (int i = 0; i < 100; i++)
			{
				cout << theList[i].rank << " " << theList[i].name << endl;
			}
		}
		else if (yn == "test")
		{
			cout << theList[90].name << "!" << endl;
		}
		else
		{
			cout << "\nDo you really want to stop? (There have been " << changes << " changes)\n";
			cin >> keepGoing;
			if (keepGoing == "y")
				break;
			else
				cout << "\nWe're going to keep going then\n";
		}

	}

	awardFile.open("The Lofgren Awards.txt", ios::out | ios::trunc);

	for (int i = 0; i < LIST_SIZE; i++)
	{
		awardFile << theList[i].rank << theList[i].name << endl;
	}

	awardFile.close();

	return 0;
}

void rankUp(story better, story worse, story theList[LIST_SIZE])
//put better where worse was, put worse right below better, and scoot everything in between down
{
	string prevBetterStory = theList[worse.rank].name;		//the value that better originally had
	theList[worse.rank - 1].name = better.name;
	theList[worse.rank].name = worse.name;
	string currBetterStory;

	for (int i = worse.rank + 1; i < better.rank; i++)
	{
		currBetterStory = theList[i].name;
		theList[i].name = prevBetterStory;
		prevBetterStory = currBetterStory;
	}
}
