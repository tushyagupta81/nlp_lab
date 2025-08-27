#include <iostream>

using namespace std;
size_t max_string_size = 1024;

void shift_back(char **tokens, int index, int token_len) {
  for (int i = index; i < token_len - 1; i++) {
    tokens[i] = tokens[i + 1];
  }
}

void remove_stop_words(char **tokens, int &token_len) {
  int n;
  for (int i = 0; i < token_len; i++) {
    n = strlen(tokens[i]);
    if (tokens[i][n - 1] == '.' || tokens[i][n - 1] == ',') {
      tokens[i][n - 1] = '\0';
    } else if (strcmp(tokens[i], "is") == 0 || strcmp(tokens[i], "an") == 0 ||
               strcmp(tokens[i], "the") == 0) {
      shift_back(tokens, i, token_len);
      token_len--;
    }
  }
}

int get_index(char **tokens, char *token, int tokens_len) {
  for (int i = 0; i < tokens_len; i++) {
    if (strcmp(tokens[i], token) == 0) {
      return i;
    }
  }
  return -1;
}

void count_freq(int *freq, char **tokens, int tokens_len) {
  int index;
  for (int i = 0; i < tokens_len; i++) {
    index = get_index(tokens, tokens[i], tokens_len);
    if (index == -1) {
      cout << "Can't get index for " << tokens[i] << "\n";
      continue;
    } else {
      freq[index] += 1;
    }
  }
}

int main() {
  cout << "Enter a string: ";
  char inp[max_string_size];
  cin.getline(inp, max_string_size);

  int tokens_index = 0;
  char **tokens = new char *[max_string_size];
  int i = 0, start;
  while (inp[i] != '\0') {
    if (inp[i] == ' ') {
      i++;
      continue;
    } else {
      start = i;
      while (inp[i] != ' ') {
        i++;
      }
      char *result = new char[max_string_size];
      int j = 0;
      for (int k = start; k < i; k++) {
        result[j++] = inp[k];
      }
      tokens[tokens_index++] = result;
    }
  }

  remove_stop_words(tokens, tokens_index);

  int *freq = new int[tokens_index];
  for (int i = 0; i < tokens_index; i++) {
    freq[i] = 0;
  }
  count_freq(freq, tokens, tokens_index);

  cout << "=== Tokens ===" << "\n";
  for (int i = 0; i < tokens_index; i++) {
    cout << tokens[i] << "\n";
  }

  cout << "=== Frequency ===" << "\n";
  for (int i = 0; i < tokens_index; i++) {
    if (freq[i] == 0) {
      continue;
    }
    cout << tokens[i] << " = " << freq[i] << "\n";
  }
  return 0;
}
