#! /usr/bin/python3

import sys
import random
from socket import *

def help():
  s = '''
  server.py - server program for d-dimensional coordination value in CSV stream

  USAGE:
    server.py -h
    server.py <#dim> <#data> <min> <max>

  OPTIONS:
    -h   get this help page
    <#dim> number of dimensions data (floating numbers in [0, 1] and CSV format)
           (default is 1)
    <#data> number of data (default is 100000)
    <min> minimum delay (default is 1000)
    <max> maximum delay (default is 2000)

  EXAMPLE:
    server.py -h
    server.py 2 1000 500 1500

  CONTACT:
    Ming-Hwa Wang, Ph.D. 408/805-4175  m1wan@scu.edu
  '''
  print(s)
  raise SystemExit(1)

dim, num, min, max = (1, 100000, 1000, 2000)
if len(sys.argv) == 2 and sys.argv[1] == "-h":
  help()
elif len(sys.argv) == 5:
  dim = int(sys.argv[1])
  num = int(sys.argv[2])
  min = int(sys.argv[3])
  max = int(sys.argv[4])
  if num <= 0 or dim <= 0 or min > max:
    help()
else:
  help()

s = socket(AF_INET, SOCK_STREAM)
s.bind(('', 0))
host, port = s.getsockname()
print("connect to port number %s" % port)
s.listen(10)
while True:
  client, addr = s.accept()
  print("Got a connection from %s" % str(addr))
  random.seed(32767)
  for i in range(num):
    x = ''
    for k in range(dim):
      j = random.randint(0,65535) / random.randint(4095, 32767)
#      print(j)
      x = x + str(j)
      x = x + ","
    x = x[0:-2]
    x = x + "\n"
#    print(x)
    client.send(x.encode('ascii'))
    for k in range(random.randint(min,max)):
      j = j + k
  client.close()

