#!/bin/python3
from dec import Oracle
from cca import cca

if __name__ == "__main__":
    init = open('./init')
    oracle = Oracle(init)
    print("plaintext:", cca(oracle))
