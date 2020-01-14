#!/usr/bin/env python
import sys
from random import randint
root_dir = sys.argv[1]
fn1=root_dir+'/p1/plot_data'
fn2=root_dir+'/p2/plot_data'
fn3=root_dir+'/p3/plot_data'
fn4=root_dir+'/p4/plot_data'



fnout = root_dir+'/plot_data.trim'
fout = open(fnout,'w+')

ln = 0
first = 0
curh = 0
curm = 0


def get1000(nums):
	L = len(nums)
	inds = []
	res = []
	if L < 1000:
		print "total log is less than 1000, use all of them, and use the last one to fill\n"
		return nums + [nums[-1]]*(1000-L)
	seen = set()
	while len(seen) < 1000:
		seen.add(randint(0,L-1))
	inds = sorted(list(seen))
	for ele in inds:
		res.append(nums[ele])
	return res
	


def file_to_list(fn):
	res = []
	with open(fn) as f:
		for line in f:
			if line.startswith('#'): continue
			line = line.replace(' ','').split(',')
			res.append(line[6][:-1])
	return res

def list_to_file(nums1,nums2,nums3,nums4):
	for i in range(1000):
		n1,n2,n3,n4 = nums1[i],nums2[i],nums3[i],nums4[i]
		fout.write(n1+' ')
		fout.write(n2+' ')
		fout.write(n3+' ')
		fout.write(n4+'\n')

def combine():
	lf1 = file_to_list(fn1)
	lf2 = file_to_list(fn2)
	lf3 = file_to_list(fn3)
	lf4 = file_to_list(fn4)
	lf1 = get1000(lf1)
	lf2 = get1000(lf2)
	lf3 = get1000(lf3)
	lf4 = get1000(lf4)
	list_to_file(lf1,lf2,lf3,lf4)



def main():
	combine()

if __name__ == '__main__':
	main()
#def combine():
#	
#
#
#
#
#def trim(fn):
#	with open(fn) as f:
#		for line in f:
#			ln += 1
#			if ln == 1: continue
#			line = line.strip().split(',')
#			t = line[0]
#			if not first:
#				first = int(t)
#				t = 0
#			else:
#				t = int(t)-first
#	
#			if ln % 2:
#				h=t/60/60
#				m=(t/60)%60
#				if curh or curm:
#					if h==curh and m==curm: continue
#					curh = h
#					curm = m
#					fout.write(str(t/60)+','+line[6])
#					fout.write("\n")
#			fout.write(str(t/60)+','+line[6])
#			fout.write("\n")
#	


		

	
