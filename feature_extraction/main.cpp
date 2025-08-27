#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>

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
    if (tokens[i][n - 1] == '.' || tokens[i][n - 1] == ',' ||
        tokens[i][n - 1] == '?') {
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

void create_tokens(char **tokens, char *inp, int &tokens_index) {
  int i = 0, start;
  while (inp[i] != '\0') {
    if (inp[i] == ' ') {
      i++;
      continue;
    } else {
      start = i;
      while (inp[i] != ' ' && inp[i] != '\0') {
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
}

bool check_in(char **vocab, char *word, int vocab_index) {
  for (int i = 0; i < vocab_index; i++) {
    if (strcmp(vocab[i], word) == 0) {
      return true;
    }
  }
  return false;
}

void vocab(char ***sentences, int *tokens_index, int sentence_index,
           char **vocab, int &vocab_index) {
  for (int i = 0; i < sentence_index; i++) {
    for (int j = 0; j < tokens_index[i]; j++) {
      if (!check_in(vocab, sentences[i][j], vocab_index)) {
        vocab[vocab_index] = new char[max_string_size];
        strcpy(vocab[vocab_index], sentences[i][j]);
        vocab_index++;
      }
    }
  }
}

int get_freq(char **tokens, char *word, int tokens_index) {
  int res = 0;
  for (int i = 0; i < tokens_index; i++) {
    if (strcmp(tokens[i], word) == 0) {
      res += 1;
    }
  }
  return res;
}

void tf(char ***sentences, char **vocab, float **tf, int *tokens_index,
        int vocab_index, int sentence_index) {
  for (int i = 0; i < vocab_index; i++) {
    for (int j = 0; j < sentence_index; j++) {
      int w_freq = get_freq(sentences[j], vocab[i], tokens_index[j]);
      tf[i][j] = (float)w_freq / tokens_index[i];
    }
  }
}

int sentences_cotaining(char ***sentences, char *word, int *tokens_index,
                        int sentence_index) {
  int res = 0;
  for (int i = 0; i < sentence_index; i++) {
    for (int j = 0; j < tokens_index[i]; j++) {
      if (strcmp(sentences[i][j], word) == 0) {
        res += 1;
        break;
      }
    }
  }
  return res;
}

void get_idf(float *idf, char ***sentences, char **voc, int vocab_index,
             int sentence_index, int *tokens_index) {
  for (int i = 0; i < vocab_index; i++) {
    int c_sentences =
        sentences_cotaining(sentences, voc[i], tokens_index, sentence_index);
    idf[i] = log(sentence_index / (float)c_sentences);
  }
}

int main() {
  srandom(time(NULL));
  char inp[max_string_size];

  int sentence_index = 0;
  int *tokens_index = new int[max_string_size];
  char ***sentences = new char **[max_string_size];

  ifstream file("temp.txt");
  while (file.getline(inp, max_string_size)) {
    sentences[sentence_index] = new char *[max_string_size];
    tokens_index[sentence_index] = 0;
    create_tokens(sentences[sentence_index], inp, tokens_index[sentence_index]);
    sentence_index++;
  }

  // cout << "=== Sentences ===" << "\n";
  // for (int i = 0; i < sentence_index; i++) {
  //   printf("=== Tokens %d ===\n", i + 1);
  //   for (int j = 0; j < tokens_index[i]; j++) {
  //     cout << sentences[i][j] << "\n";
  //   }
  // }

  char **voc = new char *[max_string_size];
  int vocab_index = 0;
  vocab(sentences, tokens_index, sentence_index, voc, vocab_index);

  printf("=== Vocab ===\n");
  for (int i = 0; i < vocab_index; i++) {
    printf("%s\n", voc[i]);
  }

  float **term_freq = new float *[vocab_index];
  for (int i = 0; i < vocab_index; i++) {
    term_freq[i] = new float[sentence_index];
  }
  tf(sentences, voc, term_freq, tokens_index, vocab_index, sentence_index);

  printf("\n=== Term Frequency ===\n");
  printf("%-11s", "");
  for (int i = 0; i < sentence_index; i++) {
    printf("S%-3d ", i + 1);
  }
  printf("\n");
  for (int i = 0; i < vocab_index; i++) {
    printf("%-10s ", voc[i]);
    for (int j = 0; j < sentence_index; j++) {
      printf("%.2f ", term_freq[i][j]);
    }
    printf("\n");
  }

  float *idf = new float[vocab_index];
  get_idf(idf, sentences, voc, vocab_index, sentence_index, tokens_index);

  printf("\n=== IDF ===\n");
  printf("%-10s IDF", "");
  printf("\n");
  for (int i = 0; i < vocab_index; i++) {
    printf("%-10s %.2f\n", voc[i], idf[i]);
  }

  printf("\n=== TD-IDF ===\n");
  printf("%-11s", "");
  for (int i = 0; i < sentence_index; i++) {
    printf("S%-3d ", i + 1);
  }
  printf("\n");
  for (int i = 0; i < vocab_index; i++) {
    printf("%-10s ", voc[i]);
    for (int j = 0; j < sentence_index; j++) {
      printf("%.2f ", term_freq[i][j]*idf[i]);
    }
    printf("\n");
  }

  return 0;
}
