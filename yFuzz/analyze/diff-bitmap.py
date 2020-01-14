#!/usr/bin/env python

import sys
import struct
fpre = sys.argv[1]
flat = sys.argv[2]

qpre = []
qlat = []

new = 0
update = 0

def q_from_file(filename, q, chunksize=8192):
    with open(filename, "rb") as f:
        while True:
            chunk = f.read(chunksize)
            if chunk:
                for b in chunk:
                     q.append(b)
            else:
                break

# example:


print "file name is %s" % fpre

q_from_file(fpre,qpre)
q_from_file(flat,qlat)

for i in xrange(len(qpre)):
	bpre = qpre[i]
	blat = qlat[i]
	valpre = struct.unpack('{}B'.format(len(bpre)), bpre)[0]
	vallat = struct.unpack('{}B'.format(len(blat)), blat)[0]
	if valpre == 255 and vallat!=255:
		new += 1
		print "found new"
	elif valpre != vallat:
		update += 1
		print "count changes"
print "size in total: %d\n" % len(qlat)
print "new in total: %d\n" % new
print "updated in total:%d\n" % update
