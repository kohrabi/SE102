#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int rectCount = 0;


vector<string> split(string line, string delimeter)
{
    vector<string> tokens;
    size_t last = 0; size_t next = 0;
    while ((next = line.find(delimeter, last)) != string::npos)
    {
        tokens.push_back(line.substr(last, next - last));
        last = next + delimeter.length();
    }
    tokens.push_back(line.substr(last));

    return tokens;
}

int getNumberFromString(string line, string name)
{
    size_t pos = line.find(name);
    if (pos != string::npos)
    {
        string lineNumberString = line.substr(pos + name.length() + 2);
        string numberString = "";
        for (int i = 0; i < lineNumberString.length(); i++)
        {
            if (lineNumberString[i] >= '0' && lineNumberString[i] <= '9')
            {
                numberString += lineNumberString[i];
            }
            else
                break;
        }
        return atoi(numberString.c_str());
    }
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        cout << "Currently this only support 3 argument: collisionLayer inputFile outputFile. please check again\n";
        system("pause");
        return -1;
    }
    ifstream input(argv[2]);
    string line;
    bool foundLayer = false;
    int n = 0;
    int m = 0;
    int lineI = 0;
    int** a = new int*;
    while (getline(input, line))
    {
        if (!foundLayer && line.find(argv[1]) != string::npos)
        {
            foundLayer = true;
            m = getNumberFromString(line, "width");
            n = getNumberFromString(line, "height");
            a = new int*[n];
            for (int i = 0; i < n; i++)
                a[i] = new int[m];
            getline(input, line);
            continue;
        }
        if (foundLayer)
        {
            if (lineI < n && line.length() > 0 && line[0] >= '0' && line[0] <= '9')
            {
                auto splitLine = split(line, ",");
                for (int j = 0; j < min((int)splitLine.size(), m); j++)
                {
                    if (splitLine[j] != "") 
                        a[lineI][j] = atoi(splitLine[j].c_str());
                }
                lineI++;
            }
            else
                break;
        }
    }
    if (!foundLayer)
    {
        cout << "Unable to find Layer\n";
        system("pause");
        return -1;
    }
    int** visited = new int* [n];
    for (int i = 0; i < n; i++)
        visited[i] = new int[m];

    ofstream file(argv[3]);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            visited[i][j] = 0;

    file << "[RECTANGLES]\n";
    file << '\n';
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (a[i][j] && !visited[i][j])
            {
                rectCount++;
                visited[i][j] = rectCount;

                int left = j;
                int top = i;
                int right = j + 1;
                for (; right < m && a[i][right] && !visited[i][right]; right++)
                    visited[i][right] = rectCount;

                int bottom = i + 1;
                for (; bottom < n; bottom++)
                {
                    int width = j;
                    for (; width < m && a[bottom][width] && !visited[bottom][width]; width++);
                    if (width != right || width == j)
                    {
                        break;
                    }
                    for (int k = j; k < width && a[bottom][k] && !visited[bottom][k]; k++)
                        visited[bottom][k] = rectCount;
                }
                file << rectCount << ' ' << left << ' ' << top << ' ' << right - 1 << ' ' << bottom - 1 << '\n';
            }
        }
    }
    file << '\n';

    file << "[MAP]\n";
    file << '\n';
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            file << visited[i][j] << ' ';
        }
        file << '\n';
    }
    system("pause");
    return 0;
}