#include <iostream>
#include <vector>

using namespace std;

vector<int> CalculatingPrefixFunction(string Line) {
    int LineLength = Line.length();
    vector<int> prefixes(LineLength);
    prefixes[0] = 0;
    cout << "Calculating prefix function for: " << Line << endl;
    for (int i = 1; i < LineLength; i++) {
        cout << "i = " << i << ", character: " << Line[i] << endl;
        int ActualLineLength = prefixes[i - 1];
        cout << "  Initial length from previous prefix: " << ActualLineLength << endl;
        
        while (ActualLineLength > 0 && (Line[ActualLineLength] != Line[i])) {
            cout << "  Mismatch: Line[" << ActualLineLength << "] = " << Line[ActualLineLength] 
                 << " != " << Line[i] << ", checking previous prefix" << endl;
            ActualLineLength = prefixes[ActualLineLength - 1];
            cout << "  New length to check: " << ActualLineLength << endl;
        }
        
        if (Line[ActualLineLength] == Line[i]) {
            ActualLineLength++;
            cout << "  Match found, incrementing length to: " << ActualLineLength << endl;
        } else {
            cout << "  No match found, keeping length 0" << endl;
        }
        
        prefixes[i] = ActualLineLength;
        cout << "  prefixes[" << i << "] = " << prefixes[i] << endl;
    }
    cout << "Final prefix array: [";
    for (int i = 0; i < prefixes.size(); i++) {
        cout << prefixes[i];
        if (i != prefixes.size() - 1) cout << ", ";
    }
    cout << "]" << endl << endl;
    return prefixes;
}

void IsCyclicShift(string FirstLine, string SecondLine, vector<int>& Result) {
    cout << "Checking if '" << SecondLine << "' is a cyclic shift of '" << FirstLine << "'" << endl;
    cout << "Doubling first string: '" << FirstLine << "' + '" << FirstLine << "' = '" << FirstLine + FirstLine << "'" << endl;
    
    string combined = SecondLine + "#";
    cout << "Creating combined string for prefix function: '" << combined << "'" << endl;
    vector<int> Prefixes = CalculatingPrefixFunction(combined);
    
    int FirstLineStep = 0;
    cout << "Starting pattern matching process:" << endl;
    for (int SecondLineStep = 0; SecondLineStep < FirstLine.size(); ++SecondLineStep) {
        cout << "\nPosition " << SecondLineStep << " in doubled string (" << FirstLine[SecondLineStep] << ")" << endl;
        cout << "Current pattern position: " << FirstLineStep << endl;
        
        while (FirstLineStep > 0 && SecondLine[FirstLineStep] != FirstLine[SecondLineStep]) {
            cout << "  Mismatch: pattern[" << FirstLineStep << "] = " << SecondLine[FirstLineStep] 
                 << " != " << FirstLine[SecondLineStep] << endl;
            cout << "  Using prefix function to jump from " << FirstLineStep << " to " << Prefixes[FirstLineStep-1] << endl;
            FirstLineStep = Prefixes[FirstLineStep - 1];
        }
        
        if (SecondLine[FirstLineStep] == FirstLine[SecondLineStep]) {
            cout << "  Match: pattern[" << FirstLineStep << "] = " << SecondLine[FirstLineStep] 
                 << " == " << FirstLine[SecondLineStep] << endl;
            FirstLineStep++;
            cout << "  Incrementing pattern position to: " << FirstLineStep << endl;
        } else {
            cout << "  No match, keeping pattern position at 0" << endl;
        }
        
        if (FirstLineStep == SecondLine.size()) {
            int shift = SecondLineStep - SecondLine.size() + 1;
            cout << "\nFULL MATCH FOUND at position " << SecondLineStep << " in doubled string!" << endl;
            cout << "This corresponds to a cyclic shift of " << shift << endl;
            Result.push_back(shift);
            if (true) break;
        }
    }
    
    if (Result.empty()) {
        cout << "No match found after checking all positions" << endl;
    }
}

int main() {
    vector<int> Result;
    string FirstLine, SecondLine;
    cout << "Enter first string: ";
    cin >> FirstLine;
    cout << "Enter second string: ";
    cin >> SecondLine;
    
    if (SecondLine.size() == FirstLine.size()) {
        IsCyclicShift(FirstLine, SecondLine, Result);
    } else {
        cout << "Strings have different lengths, cannot be cyclic shifts" << endl;
    }
    
    if (!Result.size()) {
        cout << "Final result: -1 (no cyclic shift found)" << endl;
    } else {
        cout << "Final result: ";
        string separator;
        for (auto entry : Result) {
            cout << separator << entry;
            separator = ",";
        }
        cout << " (cyclic shift found)" << endl;
    }
    return 0;
}
