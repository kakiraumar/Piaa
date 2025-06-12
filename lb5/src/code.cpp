#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct Node {
    map<char, int> children;
    int suffixLink = -1;
    int outputLink = -1;
    int parent = -1;
    char parentChar = 0;
    vector<int> patternIndices;
};

class AhoCorasick {
private:
    vector<Node> trie;

    string buildLabel(int idx) {
        string label;
        while (idx > 0) {
            label = trie[idx].parentChar + label;
            idx = trie[idx].parent;
        }
        return label.empty() ? "" : label;
    }

    void checkMatches(int pos, int node, const vector<string>& patterns,
                      vector<pair<int, int>>& result) {
        for (int temp = node; temp != -1; temp = trie[temp].outputLink) {
            for (int pid : trie[temp].patternIndices) {
                int patternLength = patterns[pid].size();
                result.emplace_back(pos - patternLength + 1, pid + 1);
            }
        }
    }

public:
    AhoCorasick() {
        trie.emplace_back();  // root
    }

    void addPattern(const string& pattern, int index) {
        int node = 0;
        for (char ch : pattern) {
            if (!trie[node].children.count(ch)) {
                trie[node].children[ch] = trie.size();
                trie.emplace_back();
                trie.back().parent = node;
                trie.back().parentChar = ch;
            }
            node = trie[node].children[ch];
        }
        trie[node].patternIndices.push_back(index);
    }

    void buildAutomaton() {
        queue<int> q;
        trie[0].suffixLink = 0;
        trie[0].outputLink = -1;

        for (auto& it : trie[0].children) {
            int child = it.second;
            trie[child].suffixLink = 0;
            trie[child].outputLink = -1;
            q.push(child);
        }

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (auto& it : trie[current].children) {
                char ch = it.first;
                int child = it.second;

                int fallback = trie[current].suffixLink;
                while (fallback != 0 && !trie[fallback].children.count(ch)) {
                    fallback = trie[fallback].suffixLink;
                }
                if (trie[fallback].children.count(ch)) {
                    fallback = trie[fallback].children[ch];
                }
                trie[child].suffixLink = fallback;

                int ol = (!trie[fallback].patternIndices.empty())
                             ? fallback
                             : trie[fallback].outputLink;
                trie[child].outputLink = (ol == child) ? -1 : ol;

                q.push(child);
            }
        }
    }

    void printAutomaton() {
        cout << "\n--- Automaton States ---\n";
        for (int i = 0; i < trie.size(); ++i) {
            cout << "Node " << i << " (\"" << buildLabel(i) << "\") ";
            cout << ", Parent: " << trie[i].parent
                 << ", Char: '" << trie[i].parentChar
                 << "', Suffix Link: " << trie[i].suffixLink
                 << ", Output Link: " << trie[i].outputLink
                 << ", Children: ";
            for (auto& p : trie[i].children) {
                cout << "'" << p.first << "'->" << p.second << " ";
            }
            if (!trie[i].patternIndices.empty()) {
                cout << ", Patterns: ";
                for (int pid : trie[i].patternIndices)
                    cout << pid << " ";
            }
            cout << endl;
        }
        cout << "-------------------------\n";
    }

    vector<int> searchWithJoker(const string& text, const string& pattern,
                                vector<pair<string, int>>& parts) {
        vector<vector<int>> positions(parts.size());
        int node = 0;

        cout << "\n--- Step-by-Step Matching ---\n";
        for (int i = 0; i < text.size(); ++i) {
            char ch = text[i];
            while (node != 0 && !trie[node].children.count(ch)) {
                node = trie[node].suffixLink;
            }
            if (trie[node].children.count(ch)) {
                node = trie[node].children[ch];
            }

            for (int temp = node; temp != -1; temp = trie[temp].outputLink) {
                for (int pid : trie[temp].patternIndices) {
                    int plen = parts[pid].first.size();
                    int pos = i - plen + 1;
                    if (pos >= 0) {
                        int fullMatchPos = pos - parts[pid].second;
                        positions[pid].push_back(fullMatchPos);

                        // Debug output
                        cout << "✔ Matched part \"" << parts[pid].first
                             << "\" at text index " << pos
                             << " → contributes to full match at index "
                             << (fullMatchPos + 1) << endl;
                    }
                }
            }
        }

        map<int, int> matchCount;
        for (int i = 0; i < parts.size(); ++i) {
            for (int p : positions[i]) {
                matchCount[p]++;
            }
        }

        vector<int> result;
        for (auto& match : matchCount) {
            int pos = match.first;
            int count = match.second;
            if (count == parts.size() && pos >= 0 &&
                pos + pattern.size() <= text.size()) {
                result.push_back(pos + 1);  // 1-based index
                cout << "✅ Full pattern match at position " << (pos + 1) << endl;
            } else {
                cout << "❌ Rejected position " << (pos + 1)
                     << " (matched " << count << "/" << parts.size() << " parts)" << endl;
            }
        }

        cout << "-------------------------------\n";
        return result;
    }
};

int main() {
    string text, pattern;
    char joker;
    cin >> text >> pattern >> joker;

    vector<pair<string, int>> parts;
    int start = -1;
    for (int i = 0; i <= pattern.size(); ++i) {
        if (i < pattern.size() && pattern[i] != joker) {
            if (start == -1)
                start = i;
        } else {
            if (start != -1) {
                parts.emplace_back(pattern.substr(start, i - start), start);
                start = -1;
            }
        }
    }

    AhoCorasick ac;

    if (parts.empty()) {
        cout << "No valid parts to match.\n";
        return 0;
    }

    for (int i = 0; i < parts.size(); ++i) {
        ac.addPattern(parts[i].first, i);
    }
    ac.buildAutomaton();
    ac.printAutomaton();

    auto positions = ac.searchWithJoker(text, pattern, parts);
    cout << "\n--- Final Match Positions ---\n";
    for (int position : positions) {
        cout << position << endl;
    }

    return 0;
}
