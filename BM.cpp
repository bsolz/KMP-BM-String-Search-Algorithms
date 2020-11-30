#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#define XSIZE 20
#define ASIZE 256
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
        for(i = keySize - 1; i >= 0 && key[i] == text[i + j]; i--);
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

int main()
{

    // Choose Key
    std::string key = "TATTLETALE";
    // Choose text file to work on and place contents in one string
    std::ifstream file("25.txt");
    std::string Text;
    std::string str;

    while (getline(file, str))
    {
        Text += str;
    }

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

    std::cout << BM(x, key.size(), y, Text.size()) << '\n';
    return 0;
}