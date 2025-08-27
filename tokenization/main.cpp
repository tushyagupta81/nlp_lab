#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
vector<string> remove_stop_words(vector<string> tokens) {
  vector<string> result;
  int n;
  for (auto t : tokens) {
    n = t.size();
    if (t[n - 1] == '.' || t[n - 1] == ',') {
      result.push_back(t.substr(0, n - 1));
    } else if (t == "is" || t == "an" || t == "the") {
      continue;
    } else {
      result.push_back(t);
    }
  }

  return result;
}

unordered_map<string, int> count_freq(vector<string> tokens) {
  unordered_map<string, int> result;
  for (auto t : tokens) {
    if (result.find(t) != result.end()) {
      result[t] += 1;
    } else {
      result[t] = 1;
    }
  }
  return result;
}

int main() {
  cout << "Enter a string: ";
  string inp;
  getline(cin, inp);

  vector<string> tokens;
  int i = 0, start;
  while (i < inp.length()) {
    if (inp[i] == ' ') {
      i++;
      continue;
    } else {
      start = i;
      while (inp[i] != ' ') {
        i++;
      }
      tokens.push_back(inp.substr(start, i - start));
    }
  }

  tokens = remove_stop_words(tokens);
  unordered_map<string, int> freq = count_freq(tokens);

  cout << "=== Tokens ===" << "\n";
  for (auto t : tokens) {
    cout << t << "\n";
  }

  cout << "=== Frequency ===" << "\n";
  for (auto c : freq) {
    cout << c.first << " = " << c.second << "\n";
  }
  return 0;
}
