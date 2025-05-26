#include <iostream>
#include <vector>

using namespace std;

// Computes the prefix function (partial match table) for the KMP algorithm
// Input: String Line (pattern to preprocess)
// Output: Vector of prefix lengths for each position in the pattern
vector<int> CalculatingPrefixFunction(string Line)
{
    int LineLength = Line.length();
    vector<int> prefixes(LineLength);  // Initialize prefix table
    prefixes[0] = 0;  // Base case: first character has prefix length 0

    // Build prefix table for each position in the string
    for (int i = 1; i < LineLength; i++)
    {
        // Start with prefix length of previous character
        int ActualLineLength = prefixes[i - 1];
        
        // While we have a partial match and current characters don't match,
        // backtrack using the prefix table
        while (ActualLineLength > 0 && (Line[ActualLineLength] != Line[i]))
            ActualLineLength = prefixes[ActualLineLength - 1];
        
        // If characters match, extend the prefix length
        if (Line[ActualLineLength] == Line[i])
            ActualLineLength++;

        // Store the computed prefix length for current position
        prefixes[i] = ActualLineLength;
    }
    return prefixes;
}

// KMP pattern matching algorithm to find all occurrences of FirstLine in SecondLine
// Input:
//    FirstLine - pattern to search for
//    SecondLine - text to search in
// Output:
//    Result - vector containing starting indices of all matches
void KnuthMorrisPratt(string FirstLine, string SecondLine, vector<int>& Result)
{
    // Compute prefix function for pattern with special delimiter ' '
    vector<int> p = CalculatingPrefixFunction(FirstLine + " ");
    int FirstLineStep = 0;  // Current position in pattern

    // Iterate through each character in the text
    for (int SecondLineStep = 0; SecondLineStep < SecondLine.size(); ++SecondLineStep)
    {
        // While mismatch occurs, use prefix table to skip ahead
        while (FirstLineStep > 0 && FirstLine[FirstLineStep] != SecondLine[SecondLineStep])
            FirstLineStep = p[FirstLineStep - 1];
        
        // If characters match, move to next character in pattern
        if (FirstLine[FirstLineStep] == SecondLine[SecondLineStep])
            FirstLineStep++;

        // If entire pattern matched, record the starting position
        if (FirstLineStep == FirstLine.size())
            Result.push_back(SecondLineStep - FirstLine.size() + 1);
    }
}

int main()
{
    vector<int> Result;  // Stores starting positions of all matches
    string FirstLine, SecondLine;
    
    // Read input strings
    cin >> FirstLine;   // Pattern to search for
    cin >> SecondLine;  // Text to search in

    // Perform KMP search
    KnuthMorrisPratt(FirstLine, SecondLine, Result);

    // Output results
    if (!Result.size())
        cout << -1;  // No matches found
    else
    {
        // Print comma-separated list of match positions
        string separator;
        for (auto entry : Result)
        {
            cout << separator << entry;
            separator = ",";  // Only add commas after first element
        }
    }
    return 0;
}
