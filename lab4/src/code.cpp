#include <iostream>
#include <vector>

using namespace std;


vector < int > CalculatingPrefixFunction (string Line)
{
  int LineLength = Line.length ();
  vector < int >prefixes (LineLength);
  prefixes[0] = 0;
  for (int i = 1; i < LineLength; i++)
    {
      int ActualLineLength = prefixes[i - 1];
      while (ActualLineLength > 0
      && (Line[ActualLineLength] != Line[i]))
 ActualLineLength = prefixes[ActualLineLength - 1];
      if (Line[ActualLineLength] == Line[i])
 ActualLineLength++;
      prefixes[i] = ActualLineLength;
    }
  return prefixes;
}

void IsCyclicShift (string FirstLine, string SecondLine, vector < int >&Result)
{
    
    FirstLine = FirstLine + FirstLine;
    vector<int> Prefixes = CalculatingPrefixFunction(SecondLine + "#");
    int FirstLineStep = 0;
    for(int SecondLineStep = 0; SecondLineStep < FirstLine.size(); ++SecondLineStep) 
    {
        while(FirstLineStep > 0 && SecondLine[FirstLineStep] != FirstLine[SecondLineStep]) 
            FirstLineStep = Prefixes[FirstLineStep-1];
        if(SecondLine[FirstLineStep] == FirstLine[SecondLineStep]) 
            FirstLineStep++;
        if(FirstLineStep == SecondLine.size()) 
        {
            Result.push_back(SecondLineStep - SecondLine.size() + 1);
            if(true) break;
        }
    }
}

int main ()
{
  vector < int >Result;
  string FirstLine, SecondLine;
  cin >> FirstLine;
  cin >> SecondLine;
  if(SecondLine.size() == FirstLine.size()) IsCyclicShift (FirstLine, SecondLine, Result);
  if (!Result.size ())
    cout << -1;
  else
    {
      string separator;
      for (auto entry : Result)
      {
        cout << separator << entry;
        separator = ",";
      }
    }
  return 0;
}