//
// Created by hdc on 12/28/17.
//
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;
const int NUM = 7;
const string wordlist[NUM] = {"apiary", "beetle",
                              "cereal", "danger",
                              "ensign", "florid",
                              "garage"};

int main()
{
    srand(time(0));
    char play;
    cout << "play? <y/n>";
    cin >> play;
    while (play == 'y' || play == 'Y')
    {
        string target = wordlist[rand()%NUM];
        int length = target.length();
        string attempt(length, '-');
        string badchars;
        int guesses = 6;
        cout << "word length is " << length
                                  <<" letters, and you guess\n"
                                  <<" one letter at a time. You get " << guesses
                                  << " wrong guesses.\n";
        cout << "your word: " << attempt << endl;

        while (guesses > 0 && attempt != target)
        {
            char letter;
            cout << "guess a letter: ";
            cin >> letter;
            if (badchars.find(letter) != string::npos
                || attempt.find(letter) != string::npos)
            {
                cout << "already guessed that, try again\n";
                continue;
            }
            int loc =target.find(letter);
            if (loc == string::npos)
            {
                cout << "bad guess!\n";
                --guesses;
                badchars += letter; // add to string
            } else
            {
                cout << "good guess!\n";
                attempt[loc]=letter;
                // check if letter appears again
                loc = target.find(letter, loc +1);
                while (loc != string::npos)
                {
                    attempt[loc]=letter;
                    loc = target.find(letter, loc + 1);
                }
            }
            cout << "your word: " << attempt << endl;
            if (attempt != target)
            {
                if (badchars.length() > 0)
                    cout << "Bad choices: " << "bad guesses left\n";
            }
        }
        if (guesses > 0)
            cout << "that's right!\n";
        else
            cout << "sorry, the word is " << target << ".\n";
        cout << "will you play another? <y/n>";
        cin >> play;
        play = tolower(play);
    }
    cout << "Bye!\n";
    cin.get();
    return 0;
}

