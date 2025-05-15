# Lupra
A minimalistic, high-performance C library providing core data structures and algorithms for systems-level development.

## Data Structures
- Array list
- Bit array
- Bloom filter
- Hash table
- Heap
- Linked list

## Algorithms
- Search
  - Binary search (array)
- Hash
  - MurmurHash3

## Utilities
- Network
  - Convert MAC address from long to string
  - Convert IPv4 string to/from long
  - Check if IPV4 address matches another given a bitmask

## Getting Started

### Dev Environment
- Install dependencies
  - [macOS] `brew bundle`
  - [Linux] TODO
- Run the following:
    ```shell
    asdf plugin add cmake
    asdf install
    ```
- Run `cmake build`

Quick note on thread safety: The data structures in this library are not inherently thread safe. If you need thread
safety, you'll need to control thread access yourself (e.g. using a mutex).
