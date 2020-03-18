import sys

if sys.version_info.major == 2:
    from Tkinter import *
elif sys.version_info.major == 3:
    from tkinter import *


class MyCanvas(Canvas):
    def __init__(self, master, hLineWidth=1, vLineWidth=1, radius=2, **kwargs):
        Canvas.__init__(self, master, kwargs)
        self.hLineWidth = hLineWidth
        self.vLineWidth = vLineWidth
        self.radius = radius

    def create_segment_h(self, x, y, l):
        self.create_line(x, y, x + l, y, width=self.hLineWidth)
        self.create_oval(x - self.radius, y - self.radius, x + self.radius, y + self.radius, fill='black')
        self.create_oval(x + l - self.radius, y - self.radius, x + l - self.radius, y + self.radius, fill='black')

    def create_segment_v(self, x, y, l):
        self.create_line(x, y, x, y + l, width=self.vLineWidth)
        self.create_oval(x - self.radius, y - self.radius, x + self.radius, y + self.radius, fill='black')
        self.create_oval(x - self.radius, y + l - self.radius, x + self.radius, y + l + self.radius, fill='black')

    def create_line_h(self, x, y, l):
        self.create_line(x, y, x + l, y, width=self.hLineWidth)

    def create_line_v(self, x, y, l):
        self.create_line(x, y, x, y + l, width=self.vLineWidth)

#add distance of the line
class Merger_oe:
    def __init__(self, size,dist):
        self.size = size
        self.dist = dist
        if(size > 1):
            self.oddmerger = Merger_oe(size//2,self.dist * 2)
            self.evenmerger = Merger_oe(size//2,self.dist * 2)

    def hNum(self):
        return self.oddmerger.hNum() + self.evenmerger.hNum() + 1 if self.size > 1 else 0

    def draw(self,cvs,x_begin,y_begin,hScale,vScale):
        if self.size == 2:
            cvs.create_segment_v(x_begin + 0.5 * hScale,y_begin,vScale * self.dist)
        elif self.size > 1:
            self.oddmerger.draw(cvs,x_begin,y_begin,hScale,vScale)
            self.evenmerger.draw(cvs,x_begin + self.oddmerger.hNum() * hScale, y_begin + self.dist * vScale ,hScale,vScale)
            for i in range((self.size // 2) - 1):
                cvs.create_segment_v(x_begin + (0.5 + self.oddmerger.hNum() + self.oddmerger.hNum()) * hScale,y_begin + (i * 2 + 1) * vScale * self.dist,vScale * self.dist)
        

class Sorter_oe:
    def __init__(self,size):
        self.size = size
        if(size > 1):
            self.subsorter = Sorter_oe(size // 2)
            self.merger = Merger_oe(size,1)
    
    def hNum(self):
        return self.subsorter.hNum() + self.merger.hNum() if self.size > 1 else 0

    def draw(self,cvs,x_begin,y_begin,hScale,vScale):
        if self.size > 1:
            self.subsorter.draw(cvs,x_begin,y_begin,hScale,vScale)
            self.subsorter.draw(cvs,x_begin,y_begin + (self.size // 2) * vScale,hScale,vScale)
            self.merger.draw(cvs,x_begin + self.subsorter.hNum() * hScale,y_begin,hScale,vScale)


if __name__ == '__main__':
    k = int(input('please input the number k: '))
    n = 2 ** k
    sortingNetwork = Sorter_oe(n)

    winW, winH = 1920 * 0.6, 1080 * 0.6
    hMargin, vMargin = winW // 20, winH // 20
    hScale, vScale = (winW - 2 * hMargin) // sortingNetwork.hNum(), (winH - 2 * vMargin) // (n - 1)

    root = Tk()
    root.title('Sorting Network with n=%d (Drawn by Python Tkinter)' % n)
    cvs = MyCanvas(root, bg='white', width=winW, height=winH)
    sortingNetwork.draw(cvs, hMargin, vMargin, hScale, vScale)
    for i in range(n):
        cvs.create_line_h(hMargin,vMargin + i * vScale,hScale * sortingNetwork.hNum())
    cvs.pack()
    root.mainloop()