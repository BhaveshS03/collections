# Collections

Nothing big, simple implentations of data structures in C.

A small C example of a nested list structure with two node types:
- `STRING_NODE` for strings
- `LIST_NODE` for child lists

## Files
- `example.c` - example usage
- `list.h` - list implementation (init, append, traversal, free)

## Build & Run
```bash
gcc -std=c11 -Wall -Wextra -o nested example.c && ./nested
```


Expected output:
```text
hello world C List
```
