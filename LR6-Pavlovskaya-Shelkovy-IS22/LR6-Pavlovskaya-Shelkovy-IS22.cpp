#include <iostream>
#include <string>
#include <string.h>
#include <exception>

using namespace std;

int charToInt(char c)
{
    return c - 48;
}

char intToChar(int n)
{
    return 48 + n;
}

string deleteEnterI(string str, int lenSub, int iStart)
{
    string result = "";
    int lenStr = (int)str.length();

    for (int i = 0; i < lenStr; i++)
    {
        if (i >= iStart && i < iStart + lenSub) continue;
        else result += str[i];
    }

    return result;
}

class List
{
public:
    int number = 0;
    List* next = NULL;

    List() {}

    List(int n)
    {
        number = n;
    }

    void AppStart(int n)
    {
        if (number == 0) number = n;
        else if (next == NULL)
        {
            next = new List(number);
            number = n;
        }
        else
        {
            next->AppStart(number);
            number = n;
        }
    }
};

class SymbString
{
private:
    List moveHistory;

protected:
    string syms = "";
    int move = 0;

public:
    SymbString() {}

    SymbString(string str)
    {
        syms = str;
    }

    string GetMoveInfo()
    {
        string result = "";
        if (moveHistory.number != 0) result += "sdvig" + to_string(moveHistory.number) + " ";

        List* curr = moveHistory.next;
        while (curr != NULL)
        {
            result += "sdvig" + to_string(curr->number) + " ";
            curr = curr->next;
        }

        return result;
    }

    string GetSyms()
    {
        return GetSymsWithMove();
    }

    int GetMove()
    {
        return move;
    }

    void MoveCycle(int amount)
    {
        move += amount;
        moveHistory.AppStart(amount);
    }

    string GetSymsWithMove()
    {
        int l = (int)syms.length();
        int realMove = move % (l - 1);
        string leftPart = "";
        string rightPart = "";
        for (int i = 0; i < l; i++)
        {
            if (i < move) rightPart += syms[i];
            else leftPart += syms[i];
        }

        return leftPart + rightPart;
    }

    SymbString operator + (SymbString other)
    {
        return SymbString(GetSymsWithMove() + other.GetSymsWithMove());
    }

    SymbString operator - (SymbString other)
    {
        string result = GetSymsWithMove();
        string subString = other.GetSymsWithMove();
        int lenSub = (int)subString.length();

        while (true)
        {
            size_t i = result.find(subString);
            if (i != std::string::npos) result = deleteEnterI(result, lenSub, i);
            else break;
        }

        return SymbString(result);
    }
};

class OctString : public SymbString
{
public:
    OctString() {}

    OctString(string str)
    {
        syms = str;
    }

    OctString operator -(OctString other)
    {
        string bigger = "";
        string smaller = "";

        if (*this > other)
        {
            bigger = GetSymsWithMove();
            smaller = other.GetSymsWithMove();
        }
        else
        {
            bigger = other.GetSymsWithMove();
            smaller = GetSymsWithMove();
        }

        int bLen = (int)bigger.length();
        int lDiff = bLen - smaller.length();

        for (int i = 0; i < lDiff; i++)
        {
            smaller = '0' + smaller;
        }

        string temp = "";
        int debt = 0;
        int sum = 0;

        for (int i = bLen - 1; i >= 0; i--)
        {
            int b = charToInt(bigger[i]);
            int s = charToInt(smaller[i]);

            sum = b - s - debt;
            if (sum < 0)
            {
                temp = intToChar(8 + sum) + temp;
                debt = 1;
            }
            else
            {
                temp = intToChar(sum) + temp;
                debt = 0;
            }
        }

        bool zeroSkiped = false;
        string result = "";

        for (int i = 0; i < bLen; i++)
        {
            if (!zeroSkiped && temp[i] != '0')
            {
                zeroSkiped = true;
                result += temp[i];
            }
            else if (zeroSkiped)
                result += temp[i];
        }
        return OctString(result);
    }

    bool operator > (OctString other)
    {
        int lThis = (int)syms.length();
        int lOther = (int)other.syms.length();

        if (lThis != lOther) return lThis > lOther;

        string thisSyms = GetSymsWithMove();
        string otherSyms = other.GetSymsWithMove();

        for (int i = 0; i < lThis; i++)
        {
            if (thisSyms[i] != otherSyms[i]) return charToInt(thisSyms[i]) > charToInt(otherSyms[i]);
        }

        return false;
    }

    string GetMoveInfo()
    {

        if (move != 0) return "sdvig" + to_string(move) + " ";
        return "";
    }

    void MoveCycle(int amount)
    {
        move += amount;
    }
};

class Factory
{
private:
    string* keysSymb = NULL;
    SymbString* valuesSymb = NULL;
    int sizeSymb = 0;

    string* keysOct = NULL;
    OctString* valuesOct = NULL;
    int sizeOct = 0;
public:
    Factory(int sSymb, int sOct)
    {
        sizeSymb = sSymb;
        keysSymb = new string[sizeSymb];
        valuesSymb = new SymbString[sizeSymb];
        for (int i = 0; i < sizeSymb; i++)
        {
            keysSymb[i] = "";
        }

        sizeOct = sOct;
        keysOct = new string[sizeOct];
        valuesOct = new OctString[sizeOct];
        for (int i = 0; i < sizeOct; i++)
        {
            keysOct[i] = "";
        }
    }

    void AddSymbString(string str, string name)
    {
        for (int i = 0; i < sizeSymb; i++)
        {
            if (keysSymb[i] == "")
            {
                keysSymb[i] = name;
                valuesSymb[i] = SymbString(str);
                break;
            }
        }
    }

    void AddOctString(string str, string name)
    {
        for (int i = 0; i < sizeOct; i++)
        {
            if (keysOct[i] == "")
            {
                keysOct[i] = name;
                valuesOct[i] = OctString(str);
                break;
            }
        }
    }

    void DeleteSymbString(string name)
    {
        for (int i = 0; i < sizeSymb; i++)
        {
            if (keysSymb[i] == name)
            {
                keysSymb[i] = "";
                break;
            }
        }
    }

    void DeleteOctString(string name)
    {
        for (int i = 0; i < sizeOct; i++)
        {
            if (keysOct[i] == name)
            {
                keysOct[i] = "";
                break;
            }
        }
    }

    SymbString GetSymbString(string name)
    {
        for (int i = 0; i < sizeSymb; i++)
            if (keysSymb[i] == name)
                return valuesSymb[i];
        return SymbString("");
    }

    OctString GetOctString(string name)
    {
        for (int i = 0; i < sizeOct; i++)
            if (keysOct[i] == name)
                return valuesOct[i];
        return OctString("");
    }

    void CycleMoveSymbString(string name, int amount)
    {
        for (int i = 0; i < sizeSymb; i++)
        {
            if (keysSymb[i] == name)
            {
                valuesSymb[i].MoveCycle(amount);
                break;
            }
        }
    }

    void CycleMoveOctString(string name, int amount)
    {
        for (int i = 0; i < sizeOct; i++)
        {
            if (keysOct[i] == name)
            {
                valuesOct[i].MoveCycle(amount);
                break;
            }
        }
    }

    void Clear()
    {
        delete[] valuesSymb;
        delete[] keysSymb;
        sizeSymb = 0;

        delete[] valuesOct;
        delete[] keysOct;
        sizeOct = 0;
    }
};

void printSymbStringInfo(string key1, string key2, Factory factory)
{
    SymbString str1 = factory.GetSymbString(key1);
    SymbString str2 = factory.GetSymbString(key2);
    cout << str1.GetMoveInfo() << key1 << " " << str1.GetSyms() << endl;
    cout << str2.GetMoveInfo() << key2 << " " << str2.GetSyms() << endl;

    SymbString plusRes = str1 + str2;
    SymbString subRes = str1 - str2;
    cout << str1.GetMoveInfo() << key1 << " + " << str2.GetMoveInfo() << key2 << " " << plusRes.GetSyms() << endl;
    cout << str1.GetMoveInfo() << key1 << " - " << str2.GetMoveInfo() << key2 << " " << subRes.GetSyms() << endl;
}

void printOctStringInfo(string key1, string key2, Factory factory)
{
    OctString str1 = factory.GetOctString(key1);
    OctString str2 = factory.GetOctString(key2);
    cout << str1.GetMoveInfo() << key1 << " " << str1.GetSyms() << endl;
    cout << str2.GetMoveInfo() << key2 << " " << str2.GetSyms() << endl;

    OctString subRes = str1 - str2;
    cout << str1.GetMoveInfo() << key1 << " - " << str2.GetMoveInfo() << key2 << " " << subRes.GetSyms() << endl;
}

int main()
{
    Factory factory = Factory(2, 2);
    string sKeys[2];
    string value = "";

    for (int i = 0; i < 2; i++)
    {
        cin >> sKeys[i] >> value;
        factory.AddSymbString(value, sKeys[i]);
    }

    string oKeys[2];
    for (int i = 0; i < 2; i++)
    {
        cin >> oKeys[i] >> value;
        factory.AddOctString(value, oKeys[i]);
    }
    printSymbStringInfo(sKeys[0], sKeys[1], factory);
    printOctStringInfo(oKeys[0], oKeys[1], factory);
    int op = -1;
    int arg = 0;
    while (op != 0)
    {
        cin >> op;
        if (op == 0) continue;
        else if (op <= 2)
        {
            cin >> arg;
            factory.CycleMoveSymbString(sKeys[op - 1], arg);
            printSymbStringInfo(sKeys[0], sKeys[1], factory);
        }
        else
        {
            cin >> arg;
            factory.CycleMoveOctString(oKeys[op - 3], arg);
            printOctStringInfo(oKeys[0], oKeys[1], factory);
            factory.CycleMoveOctString(oKeys[op - 3], -arg);
        }
    }

    factory.Clear();
}
