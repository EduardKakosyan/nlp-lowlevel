from collections import defaultdict
import re
import json
import os


class BytePairEncoder:
    """
    Byte Pair Encoding (BPE) tokenizer for subword tokenization.
    
    This class implements the BPE algorithm for training on a corpus,
    encoding text to tokens, and decoding tokens back to text.
    """
    
    def __init__(self):
        """Initialize the BPE tokenizer with empty vocabulary and merges."""
        self.vocab = {}
        self.id_to_token = {}
        self.merges = []
        self.merge_ranks = {}
        self.punctuation_no_space = set(['.', ',', '!', '?', ':', ';', ')', ']', '}', "'", '"'])
        
    def train(self, corpus, vocab_size=None, num_merges=None):
        """
        Train the BPE tokenizer on the given corpus.
        
        Args:
            corpus (str or list): The text corpus to train on or a list of strings.
            vocab_size (int, optional): Target vocabulary size (takes precedence over num_merges).
            num_merges (int, optional): Number of merge operations to learn (used if vocab_size is None).
            
        Returns:
            self: The trained tokenizer instance.
        """
        if vocab_size is None and num_merges is None:
            raise ValueError("Either vocab_size or num_merges must be specified")
            
        if isinstance(corpus, str):
            # If corpus is a single string, split it into words
            words = self._split_text_into_words(corpus)
        else:
            # If corpus is a list of strings (e.g., lines from a file)
            words = []
            for text in corpus:
                words.extend(self._split_text_into_words(text))
        
        # Initialize vocabulary with individual characters
        chars = set()
        for word in words:
            chars.update(list(word))
        
        # Initialize vocabulary with unique characters and special start/end markers
        self.vocab = {"<": 0, ">": 1, " ": 2}  # Add space as a token
        self.id_to_token = {0: "<", 1: ">", 2: " "}
        
        next_id = 3
        for char in sorted(chars):
            if char not in self.vocab:  # Skip if already in vocab
                self.vocab[char] = next_id
                self.id_to_token[next_id] = char
                next_id += 1
        
        # Initialize each word as a list of character tokens with markers
        tokenized_corpus = [["<"] + list(word) + [">"] for word in words if word]
        
        # Determine the number of merges to perform
        if vocab_size is not None:
            # Target vocabulary size minus current vocab size
            remaining_merges = vocab_size - len(self.vocab)
        else:
            remaining_merges = num_merges
            
        # Don't try to do more merges than possible
        max_possible_merges = sum(max(0, len(word) - 1) for word in tokenized_corpus)
        remaining_merges = min(remaining_merges, max_possible_merges)
        
        # Learn merges until we reach the target or run out of pairs
        for merge_rank in range(remaining_merges):
            # Get pair statistics
            pair_stats = self._get_pair_stats(tokenized_corpus)
            
            if not pair_stats:
                print(f"No more pairs to merge. Stopping after {merge_rank} merges.")
                break
                
            # Find most frequent pair
            most_frequent = max(pair_stats, key=lambda x: pair_stats[x])
            frequency = pair_stats[most_frequent]
            
            # Stop if frequency is too low (optional)
            if frequency < 2:
                print(f"No pairs occur more than once. Stopping after {merge_rank} merges.")
                break
                
            self.merges.append(most_frequent)
            self.merge_ranks[most_frequent] = merge_rank  # Lower rank = higher priority
            
            # Create new token and add to vocabulary
            new_token = most_frequent[0] + most_frequent[1]
            self.vocab[new_token] = next_id
            self.id_to_token[next_id] = new_token
            next_id += 1
            
            # Progress indicator for long training runs
            if merge_rank % 100 == 0 and merge_rank > 0:
                print(f"Completed {merge_rank} merges. Most recent: {most_frequent} -> {new_token} (freq: {frequency})")
            
            # Update corpus with the merge
            tokenized_corpus = self._merge_corpus(tokenized_corpus, most_frequent, new_token)
        
        print(f"Finished training with {len(self.vocab)} tokens in vocabulary.")
        return self
    
    def _split_text_into_words(self, text):
        """
        Split text into words for tokenization.
        
        Args:
            text (str): Text to split into words.
            
        Returns:
            list: List of words.
        """
        # Simple whitespace and punctuation splitting for now
        words = []
        # First, normalize whitespace and split by common sentence delimiters
        for sentence in re.split(r'([.!?])', text):
            if not sentence.strip():
                continue
                
            # If this part is just a punctuation delimiter, add it as its own word
            if sentence in ['.', '!', '?']:
                words.append(sentence)
                continue
                
            # Split by common punctuation and whitespace, preserving punctuation
            tokens = re.findall(r'\w+|[^\w\s]', sentence.strip())
            
            for token in tokens:
                # Skip empty tokens
                if not token:
                    continue
                words.append(token)
        
        return words
    
    def _get_pair_stats(self, tokenized_corpus):
        """
        Count frequencies of adjacent token pairs in the corpus.
        
        Args:
            tokenized_corpus (list): List of tokenized words, where each word
                                     is a list of tokens.
                                     
        Returns:
            dict: Dictionary mapping token pairs to their frequencies.
        """
        pair_stats = defaultdict(int)
        
        for word in tokenized_corpus:
            # Skip words with less than 2 tokens
            if len(word) < 2:
                continue
                
            # Count adjacent pairs
            for i in range(len(word) - 1):
                pair = (word[i], word[i + 1])
                pair_stats[pair] += 1
                
        return pair_stats
    
    def _merge_corpus(self, tokenized_corpus, pair, new_token):
        """
        Apply a merge operation to the entire corpus.
        
        Args:
            tokenized_corpus (list): List of tokenized words.
            pair (tuple): The pair of tokens to merge (token1, token2).
            new_token (str): The new token that replaces the pair.
            
        Returns:
            list: Updated tokenized corpus with merges applied.
        """
        updated_corpus = []
        
        for word in tokenized_corpus:
            new_word = []
            i = 0
            
            while i < len(word):
                # If this token and the next one form the pair to merge
                if i < len(word) - 1 and (word[i], word[i + 1]) == pair:
                    new_word.append(new_token)
                    i += 2  # Skip both tokens that were merged
                else:
                    new_word.append(word[i])
                    i += 1
                    
            updated_corpus.append(new_word)
            
        return updated_corpus
    
    def encode(self, text):
        """
        Encode text to token IDs using learned BPE merges.
        
        Args:
            text (str): The text to encode.
            
        Returns:
            list: List of token IDs.
        """
        # Pre-tokenize into words
        if not text.strip():
            return []
            
        words = self._split_text_into_words(text)
        all_token_ids = []
        
        for i, word in enumerate(words):
            # Add space between words (except before the first word or punctuation that doesn't need a space)
            if i > 0 and self.vocab.get(" ") is not None and word not in self.punctuation_no_space:
                all_token_ids.append(self.vocab[" "])
            
            # Start with character-level tokenization
            tokens = ["<"] + list(word) + [">"]
            
            # Iteratively apply merges based on priority
            can_merge = True
            while can_merge:
                can_merge = False
                
                # Check each position in the sequence
                i = 0
                while i < len(tokens) - 1:
                    pair = (tokens[i], tokens[i + 1])
                    
                    # If this pair is in our learned merges
                    if pair in self.merges:
                        # Merge them
                        merged = tokens[i] + tokens[i + 1]
                        tokens = tokens[:i] + [merged] + tokens[i+2:]
                        can_merge = True
                        # Don't increment i, try to merge at the same position again
                    else:
                        i += 1
            
            # Convert tokens to IDs and add to result
            word_ids = []
            for token in tokens:
                if token in self.vocab:
                    word_ids.append(self.vocab[token])
                # Skip tokens not in the vocabulary for now
            
            all_token_ids.extend(word_ids)
            
        return all_token_ids
    
    def decode(self, token_ids):
        """
        Decode token IDs back to text.
        
        Args:
            token_ids (list): List of token IDs.
            
        Returns:
            str: Decoded text.
        """
        if not token_ids:
            return ""
            
        # Convert token IDs to strings
        tokens = []
        for token_id in token_ids:
            token = self.id_to_token.get(token_id, "")
            if token:  # Skip empty tokens
                tokens.append(token)
        
        # Process tokens to handle special markers and create words
        result_words = []
        current_word = []
        
        for token in tokens:
            # Handle space tokens - they mark word boundaries
            if token == " ":
                if current_word:
                    # Process and clean up the current word
                    word = self._clean_token("".join(current_word))
                    if word:  # Only add non-empty words
                        result_words.append(word)
                    current_word = []
            else:
                current_word.append(token)
        
        # Handle the last word if there is one
        if current_word:
            word = self._clean_token("".join(current_word))
            if word:
                result_words.append(word)
        
        # Join words with spaces, fixing punctuation spacing
        result = ""
        for i, word in enumerate(result_words):
            # Don't add a space before the first word
            if i == 0:
                result += word
            # Don't add a space before punctuation
            elif word in self.punctuation_no_space:
                result += word
            # Add a space before other words
            else:
                result += " " + word
        
        return result
    
    def _clean_token(self, token):
        """
        Clean a token by removing special markers.
        
        Args:
            token (str): Token to clean.
            
        Returns:
            str: Cleaned token.
        """
        # Remove start and end markers
        token = token.replace("<", "").replace(">", "")
        
        # Clean up any instances where we might have double spaces
        token = re.sub(r'\s+', ' ', token)
        
        return token.strip()
    
    def save(self, filepath):
        """
        Save the tokenizer's vocabulary and merges to a file.
        
        Args:
            filepath (str): Path to save the tokenizer (without extension).
                           Will create {filepath}.model and {filepath}.vocab files.
        """
        # Create the model file (for loading)
        model_data = {
            "vocab": self.vocab,
            "merges": self.merges
        }
        
        with open(f"{filepath}.model", "w", encoding="utf-8") as f:
            json.dump(model_data, f, ensure_ascii=False, indent=2)
        
        # Create the vocabulary file (for human viewing)
        vocab_info = {
            "vocab_size": len(self.vocab),
            "merges": len(self.merges),
            "tokens": [
                {"id": token_id, "token": token, "token_hex": token.encode("utf-8").hex()}
                for token, token_id in sorted(self.vocab.items(), key=lambda x: x[1])
            ]
        }
        
        with open(f"{filepath}.vocab", "w", encoding="utf-8") as f:
            json.dump(vocab_info, f, ensure_ascii=False, indent=2)
            
        print(f"Tokenizer saved to {filepath}.model and {filepath}.vocab")
    
    def load(self, filepath):
        """
        Load a saved tokenizer.
        
        Args:
            filepath (str): Path to the .model file to load.
            
        Returns:
            self: The loaded tokenizer instance.
        """
        with open(filepath, "r", encoding="utf-8") as f:
            model_data = json.load(f)
        
        self.vocab = {str(k): int(v) if isinstance(v, str) and v.isdigit() else v
                     for k, v in model_data["vocab"].items()}
        
        # Convert string keys back to integers for id_to_token
        self.id_to_token = {}
        for token, token_id in self.vocab.items():
            if isinstance(token_id, int):
                self.id_to_token[token_id] = token
        
        # Convert string tuples back to actual tuples for merges
        self.merges = []
        for i, pair in enumerate(model_data["merges"]):
            if isinstance(pair, list) and len(pair) == 2:
                tuple_pair = (pair[0], pair[1])
                self.merges.append(tuple_pair)
                self.merge_ranks[tuple_pair] = i
            else:
                raise ValueError(f"Invalid merge pair format: {pair}")
        
        print(f"Loaded tokenizer with {len(self.vocab)} tokens and {len(self.merges)} merges.")
        return self


def load_corpus_from_file(filepath):
    """
    Load a corpus from a text file.
    
    Args:
        filepath (str): Path to the text file.
        
    Returns:
        list: List of strings (lines from the file).
    """
    with open(filepath, 'r', encoding='utf-8') as f:
        return [line.strip() for line in f if line.strip()]


def main():
    """Example usage of the BytePairEncoder class."""
    
    # Example with small hardcoded corpus
    print("\n=== Basic Example with Small Corpus ===")
    small_corpus = "ab bc bcd cde"
    tokenizer_small = BytePairEncoder()
    tokenizer_small.train(small_corpus, num_merges=3)
    print("Learned Merges:", tokenizer_small.merges[:5])  # Show up to 5 merges
    print("Vocabulary Size:", len(tokenizer_small.vocab))
    
    test_text = "bcd"
    token_ids = tokenizer_small.encode(test_text)
    print(f"Encoded '{test_text}' to token IDs:", token_ids)
    
    decoded_text = tokenizer_small.decode(token_ids)
    print(f"Decoded back to text: '{decoded_text}'")
    
    # Example with file-based corpus
    print("\n=== Training on File-Based Corpus ===")
    corpus_path = "samples/corpus.txt"
    
    if os.path.exists(corpus_path):
        # Load corpus from file
        corpus_lines = load_corpus_from_file(corpus_path)
        print(f"Loaded corpus with {len(corpus_lines)} lines")
        
        # Train tokenizer with a larger vocabulary
        tokenizer_file = BytePairEncoder()
        tokenizer_file.train(corpus_lines, vocab_size=500)
        
        # Save the tokenizer
        tokenizer_file.save("bpe_nlp")
        
        # Test the tokenizer
        test_sentences = [
            "Natural language processing uses BPE tokenization for efficient encoding.",
            "This is a test sentence to check decoding quality.",
            "BPE handles rare words by breaking them into subword units."
        ]
        
        for i, test_sentence in enumerate(test_sentences):
            print(f"\nTest sentence {i+1}: '{test_sentence}'")
            encoded = tokenizer_file.encode(test_sentence)
            print(f"Encoded to {len(encoded)} tokens")
            
            decoded = tokenizer_file.decode(encoded)
            print(f"Decoded: '{decoded}'")
            
            # Check correctness
            if decoded == test_sentence:
                print("✓ Perfect round-trip!")
            else:
                print("⚠ Differences detected:")
                print(f"  Original: '{test_sentence}'")
                print(f"  Decoded:  '{decoded}'")
        
        # Check for model loading
        print("\n=== Testing Model Loading ===")
        loaded_tokenizer = BytePairEncoder()
        loaded_tokenizer.load("bpe_nlp.model")
        
        # Test with the loaded tokenizer
        test_sentence = test_sentences[0]
        loaded_encoded = loaded_tokenizer.encode(test_sentence)
        loaded_decoded = loaded_tokenizer.decode(loaded_encoded)
        print(f"Loaded tokenizer decoded: '{loaded_decoded}'")
    else:
        print(f"Corpus file {corpus_path} not found. Skipping file-based example.")


if __name__ == "__main__":
    main()
