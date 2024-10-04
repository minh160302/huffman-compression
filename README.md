# Huffman compression

Huffman compresssion algorithm as part of CS241 assignment @ Oberlin College

Instructions:

```
% ./encode book.txt   book.huf      # encodes book.txt and writes it to
                                    # book.huf

% ./encode book.txt > book.huf      # encodes book.txt and writes it to
                                    # stdout (redirected to book.huf)

% ./decode book.huf   book.txt.2    # decodes the file, writing to book.txt.2

% diff -q book.txt book.txt.2       # check to see if files are the same
                                    # should print nothing if they are
```
