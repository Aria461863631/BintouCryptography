class Oracle:
    def __init__(self, init):
        args = []
        for line in init:
            args.append(int(line))

        self.e = args[0]
        self.__d__ = args[1]
        self.n = args[2]
        self.cipher = args[3]
        self.k = self.n.bit_length()

    def __decryption__(self, c):
        return pow(c, self.__d__, self.n)

    def encryption(self, m):
        return pow(m, self.e, self.n)

    def query(self, c):
        return self.__decryption__(c) >> 1009 & 1
