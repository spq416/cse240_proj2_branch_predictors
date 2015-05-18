

import sys
import re

print sys.argv[1] 

filename = (sys.argv[1])

f = open(filename)

#lines = f.readlines()

a = 0.0 
i = 0

for line in f:
    m = re.search('(\w+...)=(...\d+.\d+)',line)
    if m:
#       print line
#       print m.group()
       a = a + float(m.group(2))
#       print a
       i = i + 1
#       print i

avg = a / i 

print avg 




