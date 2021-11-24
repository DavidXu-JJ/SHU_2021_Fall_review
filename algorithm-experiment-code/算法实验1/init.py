
mp= [[]]

kind = {0: 0, 1: 1, 10: 2, 11: 3}

tot=0

def case0(x, y, dep, centerx, centery, specialx, specialy): 
    global tot
    global mp
    tot = tot + 1;
    mp[centerx][centery] = tot
    mp[centerx - 1][centery] = tot
    mp[centerx][centery - 1] = tot
    color(x, y, dep - 1, specialx, specialy)
    color(centerx, y, dep - 1, centerx, centery - 1)
    color(x, centery, dep - 1, centerx - 1, centery)
    color(centerx, centery, dep - 1, centerx, centery)

def case1(x, y, dep, centerx, centery, specialx, specialy): 
    global tot
    global mp
    tot= tot + 1
    mp[centerx - 1][centery - 1]= tot
    mp[centerx][centery]= tot
    mp[centerx][centery - 1]= tot
    color(x, y, dep - 1, centerx - 1, centery - 1)
    color(centerx, y, dep - 1, centerx, centery - 1)
    color(x, centery, dep - 1, specialx, specialy)
    color(centerx, centery, dep - 1, centerx, centery)

def case2(x, y, dep, centerx, centery, specialx, specialy): 
    global tot
    global mp
    tot= tot + 1
    mp[centerx - 1][centery - 1]= tot
    mp[centerx][centery]= tot
    mp[centerx - 1][centery]= tot
    color(x, y, dep - 1, centerx - 1, centery - 1)
    color(centerx, y, dep - 1, specialx, specialy)
    color(x, centery, dep - 1, centerx - 1, centery)
    color(centerx, centery, dep - 1, centerx, centery)

def case3(x, y, dep, centerx, centery, specialx, specialy): 
    global tot
    global mp
    tot= tot + 1
    mp[centerx - 1][centery - 1]= tot
    mp[centerx][centery - 1]= tot
    mp[centerx - 1][centery]= tot
    color(x, y, dep - 1, centerx - 1, centery - 1)
    color(centerx, y, dep - 1, centerx, centery - 1)
    color(x, centery, dep - 1, centerx - 1, centery)
    color(centerx, centery, dep - 1, specialx, specialy)

switch= {
        0: case0, 
        1: case1, 
        2: case2, 
        3: case3
        }

def color(x, y, dep, specialx, specialy):
    if dep == 0:
        return
    centerx = x + (2 **(dep - 1))
    centery = y + (2 **(dep - 1))
    status = kind[10 * (specialx>=centerx) + (specialy>=centery)]
    switch.get(status)(x, y, dep, centerx, centery, specialx, specialy)


k,x,y= list( map(lambda i:int(i), input().split() ) )
N = 2**k
mp=[[0 for j in range(N+1)] for i in range(N+1)]
color(1, 1, k, x, y)
for i in range(1,N+1,1):
    for j in range(1,N+1,1):
        print(mp[i][j],end='\n'if j==N else ' ')
