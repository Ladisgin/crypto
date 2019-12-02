def euclide(a,b):
    if (b==0) :
        return(a)
    else :
        r=a%b
        return euclide(b,r)

f = open("encrypt.txt", "r")
c = f.read()
c2 = c.split()
GrammSize = 3
gramm = {}
n = 0
for i in c2:
    for j in range(len(i) - GrammSize + 1):
        if i[j:j + GrammSize] in gramm:
            gramm[i[j:j + GrammSize]].append(n + j)
        else:
            gramm[i[j:j + GrammSize]] = [n + j]
    n += len(i)

ans = set()

for i in gramm:
    if(len(gramm[i]) > 2):
        t = gramm[i].copy()
        for j in range(len(t) - 1, 0, -1):
            t[j] -= t[j - 1]
        k = t[1]
        t = t[2:]
        for j in t:
            k = euclide(j,k)
        ans.add(k)

print(ans)