import random
import string
import sys
fin = open(sys.argv[1], "r")
# m = input("message: ")
m = fin.read()
# key = input("key: ")
key = sys.argv[2]
alp = list(string.ascii_lowercase)
alpha = []

# n = random.randint(2, 7)
# print(n)
# for i in range(n):
#     random.shuffle(alp)
#     alpha.append(alp.copy())

n = len(key)
for i in key:
    alpha.append(alp[ord(i) - ord('a'):] + alp[:ord(i) - ord('a')])
c = ""
k = 0
for i in range(len(m)):
    if(ord(m[i].lower()) >= ord('a') and ord(m[i].lower()) <= ord('z')):
        c += alpha[(i - k)%n][ord(m[i].lower()) - ord('a')]
    else:
        c += " "
        k += 1
print("alphabet:")
print(list(string.ascii_lowercase))
print()
for i in alpha:
    print(i)
print("encrypted string:")
f = open("encrypt.txt", "w")
f.write(c)
print(m)
print(c)