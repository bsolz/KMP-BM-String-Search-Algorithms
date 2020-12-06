#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <string.h>

#define XSIZE 20
#define ASIZE 256
//10000.txt = 12
//5000.txt = 6
//1000.txt= 7
//500.txt = 5
int KMP(char *key, int keySize, char *Text, int textSize);
void PreBadChar(char *key, int keySize, int BadChar[]);
void suffixes(char *key, int keySize, int *suff);
void PreGoodSuff(char *key, int keySize, int GoodSuff[]);
int BM(char *key, int keySize, char *text, int textSize);
void time_func(const char *name, std::string key, std::string Text);

int main()
{
    // Choose Key
    std::string key = "TATTLETALE";
    // Choose text file to work on and place contents in one string
    std::ifstream file("1000.txt");
    std::string Text;
    std::string str;

    while (getline(file, str))
    {
        Text += str;
    }

    time_func("KMP", key, Text);
    time_func("BM", key, Text);

    return 0;
}

int KMP(char *key, int keyLength, char *Text, int textLength)
{

    // Create the LPS array
    int lpsArray[keyLength];
    lpsArray[0] = 0;
    int i = 1;
    int counter = 0;
    while (i < keyLength)
    {
        if (key[i] == key[counter])
        {
            counter++;
            lpsArray[i] = counter;
            i++;
        }
        if (key[i] != key[counter])
        {
            if (counter > 0)
            {
                counter = lpsArray[counter - 1];
            }
            else
            {
                lpsArray[i] = 0;
                i++;
            }
        }
    }

    std::cout << "LPS Array: ";
    for (int i = 0; i < keyLength; i++)
    {
        std::cout << lpsArray[i] << ' ';
    }
    std::cout << '\n';
    // Use a while loop to traverse the searched string
    // Can't do for loop because the amount of i increments by is dynamic

    int Q, W;
    Q = 0;
    W = 0;

    int FinalCounter = 0;
    while (Q < textLength)
    {
        if (key[W] == Text[Q])
        {
            W++;
            Q++;
        }

        if (W == keyLength)
        {
            FinalCounter++;
            W = lpsArray[W - 1];
        }

        else if (Q < textLength && key[W] != Text[Q])
        {
            if (W == 0)
            {
                Q++;
            }
            else
            {
                W = lpsArray[W - 1];
            }
        }
    }
    return FinalCounter;
}

void PreBadChar(char *key, int keySize, int BadChar[])
{
    int i;

    for (i = 0; i < ASIZE; i++)
        BadChar[i] = keySize;
    for (i = 0; i < keySize - 1; i++)
        BadChar[key[i]] = keySize - i - 1;
}

void suffixes(char *key, int keySize, int *suff)
{
    int f, g, i;

    suff[keySize - 1] = keySize;
    g = keySize - 1;
    for (i = keySize - 2; i >= 0; i--)
    {
        if (i > g && suff[i + keySize - 1 - f] < i - g)
            suff[i] = suff[i + keySize - 1 - f];
        else
        {
            if (i < g)
                g = i;
            f = i;
            while (g >= 0 && key[g] == key[g + keySize - 1 - f])
                g--;
            suff[i] = f - g;
        }
    }
}

void PreGoodSuff(char *key, int keySize, int GoodSuff[])
{
    int i, suff[XSIZE];

    suffixes(key, keySize, suff);

    for (i = 0; i < keySize; i++)
        GoodSuff[i] = keySize;
    for (i = keySize - 1; i >= 0; i--)
        if (suff[i] == i + 1)
            for (int j = 0; j < keySize - 1 - i; j++)
                if (GoodSuff[j] == keySize)
                    GoodSuff[j] = keySize - 1 - i;
    for (i = 0; i <= keySize - 2; i++)
        GoodSuff[keySize - 1 - suff[i]] = keySize - 1 - i;
}

int BM(char *key, int keySize, char *text, int textSize)
{
    int i, j, GoodSuff[XSIZE], BadChar[ASIZE];
    int finalCounter = 0;

    PreBadChar(key, keySize, BadChar);
    PreGoodSuff(key, keySize, GoodSuff);

    //Searching
    j = 0;
    i = 0;
    while (j <= textSize - keySize)
    {
        for (i = keySize - 1; i >= 0 && key[i] == text[i + j]; i--)
            ;
        if (i < 0)
        {
            finalCounter++;
            j += GoodSuff[0];
        }
        else
            j += std::max(GoodSuff[i], BadChar[text[i + j]] - keySize + 1 + i);
    }

    return finalCounter;
}

void time_func(const char *name, std::string key, std::string Text)
{

    uint64_t val;

    char x[key.size()];
    char y[Text.size()];

    for (int i = 0; i < key.size(); i++)
    {
        x[i] = key[i];
    }

    for (int i = 0; i < Text.size(); i++)
    {
        y[i] = Text[i];
    }

    unsigned long c_start, c_end;

    if (!strcmp(name, "KMP"))
    {
        c_start = std::clock();
        val = KMP(x, key.size(), y, Text.size());
        c_end = std::clock();
        std::cout << val <<'\n';
    }
    else if (!strcmp(name, "BM"))
    {
        c_start = std::clock();
        val = BM(x, key.size(), y, Text.size());
        c_end = std::clock();
        std::cout << val <<'\n';
    }
    else
    {
        std::cout << "Invalid function call" << std::endl;
    }

    float output = 1.0 * (c_end - c_start) / CLOCKS_PER_SEC;

    std::cout << std::fixed << std::setprecision(8) << output << std::endl;
}