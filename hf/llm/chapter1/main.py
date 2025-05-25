from transformers import pipeline


def main():
    """This is a simple example of how to use the Hugging Face Transformers library to generate text."""
    ner = pipeline("ner", model="dslim/bert-base-NER")
    result = ner("My name is Wolfgang and I live in Berlin")
    print(result)

if __name__ == "__main__":
    main()
