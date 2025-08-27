import nltk
import spacy
from nltk.stem import PorterStemmer
from rich import print

# %%

nlp = spacy.load("en_core_web_sm")
nltk.download("punkt")
stemmer = PorterStemmer()

# %%

text = "Operation Sindoor was carried out on May 7, 2025, between 1:04 AM and 1:24 AM. It involved precision strikes against nine terrorist camps located in Pakistan and Pakistan-occupied Jammu and Kashmir (PoJK). These strikes targeted the headquarters of groups like Lashkar-e-Taiba and Jaish-e-Mohammed. The operation utilized advanced weaponry like SCALP Missiles, HAMMER bombs, and Loitering Munitions (drones)."

# %%

doc = nlp(text)

# %%

print("=== Tokens ===")
print([token.text for token in doc])

# %%

print("=== Stemming ===")
for token in doc:
    print(f"{token.text:15} | {stemmer.stem(token.text)}")

# %%

print("=== Lemmatization ===")
for token in doc:
    print(f"{token.text:15} | {token.lemma_}")

# %%

filtered_tokens = [token.text for token in doc if not token.is_stop and token.is_alpha]
print("Tokens after Stopword removal:")
print(filtered_tokens)

# %%

print("=== POS tagging ===")
for token in doc:
    print(f"{token.text:15} | {token.pos_:10} | {token.tag_}")

# %%

print("=== Noun Phrase Chunk ===")
for chunk in doc.noun_chunks:
    print(f"{chunk.text}")

# %%

print("=== Syntax (Dependency Parsing) ===")
for token in doc:
    print(f"{token.text:15} | {token.dep_:15} | Head: {token.head.text}")

# %%

print("=== Named Entities ===")
for ent in doc.ents:
    print(f"{ent.text:20} | {ent.label_}")
