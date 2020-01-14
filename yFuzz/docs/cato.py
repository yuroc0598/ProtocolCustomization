#!/usr/bin/env python
import sys

def sep(fn):
	with open(fn)  as f:
		for line in f:
			addr = line.split()[0]
			f = open(addr+'.strace',"a+")
			f.write(line)

def main():
	sep(sys.argv[1])

if __name__ == "__main__":
	main()
