from manimlib import *
import numpy as np

mp= 0
kind = {0: 0, 1: 1, 10: 2, 11: 3}
colors= {0: RED_E, 1: BLUE, 2: GREEN, 3: DARK_BROWN, 4: PINK, 5: ORANGE, 6: GOLD, 7: PURPLE , 8:YELLOW, 9:GOLD_E, 10:BLUE_E, 11:TEAL_E}
vec = 0
tot=0

def case0(x, y, dep, centerx, centery, specialx, specialy): 
    global tot
    global mp
    global vec
    tot = tot + 1
    mp[centerx][centery] = tot
    mp[centerx - 1][centery] = tot
    mp[centerx][centery - 1] = tot
    vec[tot].append([centerx, centery, x, y, dep])
    vec[tot].append([centerx - 1, centery, x, y, dep])
    vec[tot].append([centerx, centery - 1, x, y, dep])
    color(x, y, dep - 1, specialx, specialy)
    color(centerx, y, dep - 1, centerx, centery - 1)
    color(x, centery, dep - 1, centerx - 1, centery)
    color(centerx, centery, dep - 1, centerx, centery)

def case1(x, y, dep, centerx, centery, specialx, specialy): 
    global tot
    global mp
    global vec
    tot = tot + 1
    mp[centerx - 1][centery - 1]= tot
    mp[centerx][centery]= tot
    mp[centerx][centery - 1]= tot
    vec[tot].append([centerx - 1, centery - 1, x, y, dep])
    vec[tot].append([centerx, centery, x, y, dep])
    vec[tot].append([centerx, centery - 1, x, y, dep])
    color(x, y, dep - 1, centerx - 1, centery - 1)
    color(centerx, y, dep - 1, centerx, centery - 1)
    color(x, centery, dep - 1, specialx, specialy)
    color(centerx, centery, dep - 1, centerx, centery)

def case2(x, y, dep, centerx, centery, specialx, specialy): 
    global tot
    global mp
    global vec
    tot = tot + 1
    mp[centerx - 1][centery - 1]= tot
    mp[centerx][centery]= tot
    mp[centerx - 1][centery]= tot
    vec[tot].append([centerx - 1, centery - 1, x, y, dep])
    vec[tot].append([centerx, centery, x, y, dep])
    vec[tot].append([centerx - 1, centery, x, y, dep])
    color(x, y, dep - 1, centerx - 1, centery - 1)
    color(centerx, y, dep - 1, specialx, specialy)
    color(x, centery, dep - 1, centerx - 1, centery)
    color(centerx, centery, dep - 1, centerx, centery)

def case3(x, y, dep, centerx, centery, specialx, specialy): 
    global tot
    global mp
    global vec
    tot = tot + 1
    mp[centerx - 1][centery - 1]= tot
    mp[centerx][centery - 1]= tot
    mp[centerx - 1][centery]= tot
    vec[tot].append([centerx - 1, centery - 1, x, y, dep])
    vec[tot].append([centerx, centery - 1, x, y, dep])
    vec[tot].append([centerx - 1, centery, x, y, dep])
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


N = 0
x = 0
y = 0

class Vedio(Scene):
    def construct(self):
        global N
        global x
        global y
        intro_words = Text(" 棋盘覆盖问题 徐京伟 ", height = 1)
        thanks =Text("Powered by manimlib",height =1)
        self.play(Write(intro_words))
        self.wait()
        kw = {"run_time": 1.3, "path_arc": PI / 2}
        self.play(TransformMatchingShapes(intro_words, thanks, **kw))
        self.play(TransformMatchingShapes(thanks, intro_words, **kw))
        self.wait()

        grid = VGroup(*[Square() for x in range(N * N)])
        grid.arrange_in_grid(N, N, buff = 0)
        grid.set_stroke(WHITE,1)
        grid.set_height(6.5)
        grid.to_edge(DOWN,buff=0.5)
        plane_transform_words= Text("在一个平面上，有一个特殊方格")
        plane_transform_words.to_edge(UP)
        plane_transform_words.set_stroke(BLACK, 15, background=True)

        self.play(
            ShowCreation(grid), 
            grid.animate.scale(0.75),
            FadeTransform(intro_words, plane_transform_words)
        )

        specialx=x 
        specialy=y 

        rects=VGroup(
            grid[(N - specialy) * N + specialx - 1].copy().set_fill(WHITE, 0.5)
        )
        self.add(rects)
        self.wait(0.8)
        rects=VGroup(
            grid[(N - specialy) * N + specialx - 1].copy().set_fill(BLACK, 0.5)
        )
        self.add(rects)
        self.wait(0.8)
        rects=VGroup(
            grid[(N - specialy) * N + specialx - 1].copy().set_fill(WHITE, 0.5)
        )
        self.add(rects)
        self.wait(0.8)
        rects=VGroup(
            grid[(N - specialy) * N + specialx - 1].copy().set_fill(BLACK, 0.5)
        )
        self.add(rects)


        up= (N * N - 1)  // 3
        for i in range(1, up + 1, 1):
            specialx = vec[i][1][2]
            specialy = vec[i][1][3]
            dep = vec[i][1][4]
            rect = VGroup()
            for nowx in range(specialx, specialx + (2 ** dep), 1):
                for nowy in range(specialy, specialy + (2 ** dep), 1):
                    rect.add(grid[(N - nowy) * N + nowx - 1].copy().set_fill(YELLOW, 0.5))
            self.wait(0.3)
            self.play(
                     *[FadeIn(g) for g in rect]
            )
            self.wait(0.3)
            self.play(
                     *[FadeOut(g) for g in rect]
            )
            block= VGroup()
            for j in range(3):
                block.add(grid[(N - vec[i][j][1]) * N + vec[i][j][0] - 1].copy().set_fill(colors[i % 12], 0.8))
            self.play(
                     *[FadeIn(g) for g in block]
            )
            self.wait(0.8)
                

if __name__=="__main__":
    print("请输入你希望出现的方格大小和特殊点坐标")
    k,x,y= list( map(lambda i:int(i), input().split() ) )
    N = 2**k
    mp = [[0 for j in range(N+1)] for i in range(N+1)]
    up= (N * N - 1)  // 3
    vec = {i: [] for i in range(1, up + 1, 1)}
    color(1, 1, k, x, y)
    vedio=Vedio()
    vedio.construct()
