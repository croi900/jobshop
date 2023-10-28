import random
import matplotlib.pyplot as plt
import numpy as np

from itertools import zip_longest
def grouper(n, iterable, fillvalue=None):
    "grouper(3, 'ABCDEFG', 'x') --> ABC DEF Gxx"
    args = [iter(iterable)] * n
    return zip_longest(fillvalue=fillvalue, *args)

def parse_data(path):
    x = []
    lens = []
    y = []
    text = []
    with open(path,"r") as f:
        mach = 1
        for line in f.readlines():
            # print(line.split())
            for pid,time,dur in grouper(3,line.split()):
                pid,time,dur = map(float,(pid,time,dur))
                x.append(mach)
                y.append(time)
                text.append(str(pid))
                lens.append(dur)
            mach = mach + 1
    # x = eval("np.arange(1,5,2.5,1.5,1.5])")
    # pcols = ['orange', 'red', 'green', 'blue', 'cyan']
    
    cols = []
    # cols.resize((len(x)))
    for pid in x:
        cols.append((pid/max(x) * 1, 0.2 + pid/max(x) * 0.3,0,0.6))
    random.shuffle(cols)
    return x,y,lens, np.repeat(0.5,len(x)), cols, text

def main():
    x,y,lens,widths,cols,text = parse_data("scripts/test_matrix_sort_T.sched")


    fig = plt.figure() 
    fig.set_figheight(100)
    fig.set_figwidth(2)
    # plt.figure()
    bar = plt.bar(x,lens,widths,y,color=cols)

    idx = 0
    for rect in bar:
        plt.text(rect.get_x(),rect.get_y(),text[idx])
        
        idx = idx + 1
    # plt.yticks(np.arange(min(lens), max(lens)+1, 10000.0))
    # plt.show()
    plt.savefig("schedule.pdf", format="pdf", bbox_inches="tight")

# if __name__ == "__main__":
main()