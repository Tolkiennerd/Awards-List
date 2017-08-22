#include<iostream>
#include<string>
#include<fstream>
#include<ctime>

const int LIST_SIZE = 100;

using namespace std;

// Struct that contains the name and rank of a story.
// Used to more easily obtain the rank of a story while using fewer, more intuitive variables.
struct story
{
    int rank;
    string name;
};

// Function that puts the better story where the worse story used to be,
// puts the worse story right below the better story,
// and moves all stories in between down once.
void rankUp(story, story, story[]);

int main()
{
    // OPEN THE AWARDS FILE
    fstream awardFile;
    string awards_file_path = "/Users/matthewlofgren/Desktop/The Lofgren Awards.txt";
    awardFile.open(awards_file_path);
    
    // READ THE AWARDS FILE
    story theList[LIST_SIZE];
    story theOldList[LIST_SIZE];
    for (int i = 0; i < LIST_SIZE; i++)
    {
        awardFile >> theList[i].rank;
        getline(awardFile, theList[i].name);
        theList[i].name = theList[i].name.substr(0,theList[i].name.size()-1);
        theOldList[i].rank = theList[i].rank;
        theOldList[i].name = theList[i].name;
    }
    
    // CLOSE THE AWARDS FILE
    awardFile.close();
    
    // CONTINUALLY UPDATE LIST
    int changes = 0;
    srand(time(0));
    bool change_made = false;
    while (true)
    {
        // RANDOMLY CHOOSE TWO STORIES TO COMPARE
        story story1;
        story story2;
        do
        {
            story1 = theList[rand() % LIST_SIZE];
            story2 = theList[rand() % LIST_SIZE];
        } while (story1.rank == story2.rank);
        
        // ASK WHICH STORY IS BETTER
        cout << "Is" << story1.name << " better than" << story2.name << "? ";
        string user_input;
        cin >> user_input;
        cout << endl;
        
        // POSSIBLE USER INPUTS
        bool undo_change = ((user_input == "u") || (user_input == "undo"));
        bool story1_is_better = ((user_input == "y") || (user_input == "yes"));
        bool story2_is_better = ((user_input == "n") || (user_input == "no"));
        bool add_new_story = ((user_input == "a") || (user_input == "add"));
        bool move_story_to_bottom = ((user_input == "b") || (user_input == "bottom"));
        bool output_list = ((user_input == "l") || (user_input == "list"));
        
        // IF USER WANTS, UNDO PREVIOUS CHANGE
        if (undo_change)
        {
            for (int i = 0; i < LIST_SIZE; i++)
            theList[i] = theOldList[i];
            cout << "Undone\n";
            cout << "Is" << story1.name << " better than" << story2.name << "?\n";
            cin >> user_input;
            
            // Remove a change
            if (change_made)
                changes--;
        }
        
        // UPDATE THE LIST SO THAT ANY CHANGES REVERT TO THIS LIST
        for (int i = 0; i < LIST_SIZE; i++)
        {
            theOldList[i] = theList[i];
        }
        
        // MARK THAT NO CHANGES WERE MADE
        change_made = false;
        
        // IF USER WANTS, UPDATE LIST SO THAT STORY1 IS BETTER THAN STORY2
        if (story1_is_better)
        {
            // If user's response is different from the current list, rank up the lower story
            if (story1.rank > story2.rank)
            {
                rankUp(story1, story2, theList);
                
                // Mark a change
                changes++;
                change_made = true;
            }
        }
        
        // IF USER WANTS, UPDATE LIST SO THAT STORY2 IS BETTER THAN STORY1
        else if (story2_is_better)
        {
            // If user's response is different from the current list, rank up the lower story
            if (story2.rank > story1.rank)
            {
                rankUp(story2, story1, theList);
                
                // Mark a change
                changes++;
                change_made = true;
            }
        }
        
        // IF USER WANTS, ADD A NEW STORY TO THE LIST
        else if (add_new_story)
        {
            // GET THE NAME OF THE STORY
            cout << "Name: ";
            cin.ignore();
            string name;
            getline(cin, name);
            name = " " + name;
            
            // CHECK IF THE STORY ALREADY EXISTS
            bool story_exists = false;
            for (int i = 0; i < LIST_SIZE; i++)
            {
                if (theList[i].name == name)
                {
                    cout << "Story already exists\n";
                    story_exists = true;
                }
            }
            
            // ADD STORY IF IT DOES NOT EXIST
            if (!story_exists)
            {
                // Allow user to enter the initial rank of the story
                cout << "\nRank: ";
                int rank;
                cin >> rank;
                
                // Move all stories ranked below this new story down one to make room for it
                for (int i = LIST_SIZE - 1; i >= rank; i--)
                {
                    theList[i].name = theList[i - 1].name;
                }
                
                // Add the story
                theList[rank - 1].name = name;
            }
            
            // Mark a change
            changes++;
            change_made = true;
        }
        
        // IF USER WANTS, MOVE A STORY TO THE BOTTOM OF THE LIST
        else if (move_story_to_bottom)
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
            
            // Mark a change
            changes++;
            change_made = true;
        }
        
        // IF USER WANTS, OUTPUT THE ENTIRE LIST
        else if (output_list)
        {
            for (int i = 0; i < 100; i++)
            {
                cout << theList[i].rank << " " << theList[i].name << endl;
            }
        }
        
        // IF ANY NON-SPECIFIC INPUT IS ENTERED, EXIT THE PROGRAM
        else
        {
            // ENSURE THAT USER WANTS TO QUIT
            bool one_change_made = changes == 1;
            if (one_change_made)
                cout << "\nDo you really want to stop? (There has been 1 change)\n";
            else
                cout << "\nDo you really want to stop? (There have been " << changes << " changes)\n";
            string stop_program_verification = "y";
            cin >> stop_program_verification;
            
            // IF USER WANTS, EXIT THE PROGRAM
            bool stop_program = ((stop_program_verification == "y") || (stop_program_verification == "yes"));
            if (stop_program)
                break;
            
            // IF USER WANTS, KEEP GOING
            else
                cout << "\nWe're going to keep going then\n";
        }
        
    }
    
    // UPDATE THE AWARDS FILE WITH THE NEW LIST
    bool changes_were_made = (changes > 0);
    if (changes_were_made)
    {
        // OPEN THE AWARDS FILE
        awardFile.open(awards_file_path, ios::out | ios::trunc);
        // UPDATE THE AWARDS FILE
        for (int i = 0; i < LIST_SIZE; i++)
        {
            awardFile << theList[i].rank << theList[i].name << endl;
        }
    
        // CLOSE THE AWARDS FILE
        awardFile.close();
    }
    
    return 0;
}

// Function that puts the better story where the worse story used to be,
// puts the worse story right below the better story,
// and moves all stories in between down once.
void rankUp(story better, story worse, story theList[LIST_SIZE])
//put better where worse was, put worse right below better, and scoot everything in between down
{
    // Find the name of the previously better story (now the worse story)
    string previousBetterStory = theList[worse.rank].name;
    
    // Rename the better story with the name of the previously better story (now the worse story)
    theList[worse.rank - 1].name = better.name;
    
    // Rename the worse story with the name of the better story
    theList[worse.rank].name = worse.name;
    
    // Loop through the stories in between the better and worse stories, moving them each down once
    string currentBetterStory;
    for (int i = worse.rank + 1; i < better.rank; i++)
    {
        currentBetterStory = theList[i].name;
        theList[i].name = previousBetterStory;
        previousBetterStory = currentBetterStory;
    }
}
