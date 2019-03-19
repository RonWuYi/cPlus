#include <stdio.h>
#include <string>
#include <vector>
#include <regex>
#include <iostream>

using namespace std;
int main()
{
    string var = "first second third forth";
    const regex r("(.*) (.*) (.*) (.*)");
    smatch sm;

    if (regex_search(var, sm, r))
    {
        for (int i = 1; i < sm.size(); i++) {
            cout << sm[i] <<endl;
            cout << endl;
        }
    }

    regex wsaq_re("\\s+");
    copy( sregex_token_iterator(var.begin(), var.end(), wsaq_re, -1),
          sregex_token_iterator(),
          ostream_iterator<string>(cout, "\n")
            );
    return 0;
}