# 0,1,2
# 3,4,5
# 6,7,8

import pygame, sys, random
from pygame.locals import *

# 一些常量
WINDOWWIDTH = 500
WINDOWHEIGHT = 500
BACKGROUNDCOLOR = (255, 255, 255)
BLUE = (0, 0, 255)
BLACK = (0, 0, 0)
FPS = 40

VHNUMS = 3
CELLNUMS = VHNUMS*VHNUMS  #总共的格子数
MAXRANDTIME = 100 #足够的次数保证能够打乱拼图

# 退出
def terminate():
    pygame.quit()
    sys.exit()
    
# 随机生成游戏盘面
def newGameBoard():
    board = []   #存放分割图片的列表
    for i in range(CELLNUMS):#默认从0开始 等价于range(0,9)
        board.append(i)
    blackCell = CELLNUMS-1#将空白格放到最后一个上面
    board[blackCell] = -1
#为了让其随机一些……哈哈
    for i in range(MAXRANDTIME):
        direction = random.randint(0, 3)
        if (direction == 0):
            blackCell = moveLeft(board, blackCell)
        elif (direction == 1):
            blackCell = moveRight(board, blackCell)
        elif (direction == 2):
            blackCell = moveUp(board, blackCell)
        elif (direction == 3):
            blackCell = moveDown(board, blackCell)
    return board, blackCell
#从0开始，一直到8表示九宫格这里面的九个拼图
# 若空白图像块不在最左边，则将空白块左边的块移动到空白块位置  
def moveRight(board, blackCell):
    if blackCell % VHNUMS == 0:#判断是否在最左边 即是否是0,3,6
        return blackCell
    board[blackCell-1], board[blackCell] = board[blackCell], board[blackCell-1]
    return blackCell-1

# 若空白图像块不在最右边，则将空白块右边的块移动到空白块位置  
def moveLeft(board, blackCell):
    if blackCell % VHNUMS == VHNUMS-1:#判断是否在最右边 即是否是2,5,8
        return blackCell
    board[blackCell+1], board[blackCell] = board[blackCell], board[blackCell+1]
    return blackCell+1

# 若空白图像块不在最上边，则将空白块上边的块移动到空白块位置  
def moveDown(board, blackCell):
    if blackCell < VHNUMS:#判断是否在最上面，即是否是0,1,2
        return blackCell
    board[blackCell-VHNUMS], board[blackCell] = board[blackCell], board[blackCell-VHNUMS]
    return blackCell-VHNUMS
    
# 若空白图像块不在最下边，则将空白块下边的块移动到空白块位置  
def moveUp(board, blackCell):
    if blackCell >= CELLNUMS-VHNUMS:#判断是否在最下面，即是否是6,7,8
        return blackCell
    board[blackCell+VHNUMS], board[blackCell] = board[blackCell], board[blackCell+VHNUMS]
    return blackCell+VHNUMS

# 是否完成
def isFinished(board, blackCell):
    for i in range(CELLNUMS-1):
        if board[i] != i: #0到7是否一一对应
            return False
    return True

# 初始化
pygame.init()
mainClock = pygame.time.Clock()

# 加载图片
gameImage = pygame.image.load('jay1.jpg')
gameRect = gameImage.get_rect()
    

# 设置窗口
windowSurface = pygame.display.set_mode((gameRect.width, gameRect.height))
pygame.display.set_caption('拼图')

cellWidth = int(gameRect.width / VHNUMS)
cellHeight = int(gameRect.height / VHNUMS)

finish = False

gameBoard, blackCell = newGameBoard()


# 游戏主循环
while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            terminate()
        if finish:
            continue

        if event.type == MOUSEBUTTONDOWN and event.button == 1:
            x, y = pygame.mouse.get_pos()
            col = int(x / cellWidth)
            row = int(y / cellHeight)
            index = col + row*VHNUMS#计算出所所点击的地方 的标号是第几个 index保存这个号

#判断index是否和空白格相邻
            if (index == blackCell-1 or index == blackCell+1 or index == blackCell-VHNUMS or index == blackCell+VHNUMS):
                gameBoard[blackCell],gameBoard[index] = gameBoard[index],gameBoard[blackCell]#即交换
                blackCell = index

    if (isFinished(gameBoard, blackCell)):
        gameBoard[blackCell] = CELLNUMS-1
        finish = True
    
    windowSurface.fill(BACKGROUNDCOLOR)
    #重绘
    for i in range(CELLNUMS):
        rowDst = int(i / VHNUMS)
        colDst = int(i % VHNUMS)
        rectDst = pygame.Rect(colDst*cellWidth, rowDst*cellHeight, cellWidth, cellHeight)

        if gameBoard[i] == -1:#判断空白块 若是空白就不画了
            continue

        rowArea = int(gameBoard[i] / VHNUMS)
        colArea = int(gameBoard[i] % VHNUMS)
        rectArea = pygame.Rect(colArea*cellWidth, rowArea*cellHeight, cellWidth, cellHeight)
        windowSurface.blit(gameImage, rectDst, rectArea)#？？？

    pygame.display.update()
    mainClock.tick(FPS)
