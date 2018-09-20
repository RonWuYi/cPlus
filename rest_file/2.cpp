//
// Created by HFD on 10/26/2017.
//
#include <iostream>

using namespace std;
const int Max = 5;
//int sum_arr(int arr[], int n);
//void n_chars (char, int);
int fill_array(double ar[], int limit);
void show_array(const double ar[], int n);
void revalue (double r, double ar[], int n);

int main(){
    double properties[Max];

    int size = fill_array(properties, Max);
    show_array(properties, size);
    cout << "enter revaluation factor: ";
    double factor;
    cin >> factor;
    revalue(factor, properties, size);
    show_array(properties, size);
    cout << "done. \n";

    cin.get();
    cin.get();
    cin.get();
    cin.get();
    cin.get();
    cin.get();
    return 0;
}

int fill_array(double ar[], int limit)
{
    double temp;
    int i;
    for (i = 0; i < limit; i++) {
        cout << "enter value# " << (i + 1) << ": ";
        cin >> temp;
        if (!cin)
        {
            cin.clear();
            while (cin.get() != '\n')
                continue;
            cout << "bad input; terminated. \n";
            break;
        } else if (temp < 0)
            break;
        ar[i] = temp;

    }
    return i;
}

void show_array(const double ar[], int n)
{
    for (int i = 0; i < n; i++) {
        cout << "property #" << (i+1) << ": $";
        cout << ar[i] << endl;
    }
}

void revalue(double r, double ar[], int n)
{
    for (int i = 0; i < n; i++) {
        ar[i] *= r;
    }
}