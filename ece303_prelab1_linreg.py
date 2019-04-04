import linalg
from math import exp, log
from matplotlib import pyplot as plt

def linfit(data, param):
    n = len(data[0])  # Number of data points
    sumx = 0.0  # Initializes variables to 0
    sumy = 0.0
    sumxy = 0.0
    sumx2 = 0.0
    sumx2y = 0.0
    ylny = 0.0
    sumxylny = 0.0
    for i in range(0,n,1):  # For i in the range the length of the data set
        if param == 'power':
            data[0][i] = log(float(data[0][i]),10)
            data[1][i] = log(float(data[1][i]),10)
        elif param == 'exp':
            data[0][i] = log(float((data[1][i])))
            data[1][i] = float(data[1][i])**-1
        elif param == 'sat':
            data[0][i] = float((data[0][i])) ** -1
            data[1][i] = float((data[1][i])) ** -1
        elif param == 'lin':
            pass
        sumx += float(data[0][i])  # Sum of all x val
        sumy += float(data[1][i])  # Sum of all y val
        sumxy += float(data[0][i])*float(data[1][i])  # Sum of all x*y val
        sumx2 += float((data[0][i]))**2  # Sum of all x squared val
        if param == 'exp':
            sumxylny += float(data[0][i])*float(data[1][i])*log(float(data[1][i]))
            sumx2y += float((data[0][i]))**2*float(data[1][i])
            ylny += float(data[1][i])*log(float(data[1][i]))
    a1 = (float(n)*float(sumxy)-float(sumx)*float(sumy))/(float(n)*float(sumx2)-float(sumx)**2)
    a0 = sumy/float(n)-a1*sumx/float(n)
    ynew = []
    st = 0
    sr = 0
    if param == 'lin':
        for i in range(0,len(data[0]),1):
            ybar = a0+a1*float(data[0][i])
            ynew.append(ybar)
            sr += pow(float(data[1][i]) - ybar, 2)
            st += pow((float(data[1][i]) - (sumy/len(data[1]))), 2)
        title = ("y = %.4f " % a0 + "+ %.4f x" % a1)
        a = a0
        b = a1
        rsq = (st - sr)/st
        print(rsq)
    if param == 'linorigin':
        for i in range(0, len(data[0]), 1):
            ybar = 0 + a1 * float(data[0][i])
            ynew.append(ybar)
            sr += pow(float(data[1][i]) - ybar, 2)
            st += pow((float(data[1][i]) - (sumy/len(data[1]))), 2)
        title = ("y = %.4f x" % a1)
        a = 0
        b = a1
        rsq = (st - sr)/st
        print(rsq)
    if param == 'exp':
        a0 = (sumx2y*ylny-sumxy*sumxylny)/(sumy*sumx2y-sumxy**2)
        b0 = (sumy*sumxylny-sumxy*ylny)/(sumy*sumx2y-sumxy**2)
        a = exp(a0)
        b = b0
        for i in range(0,len(data[0]),1):
            ynew.append(a*exp(data[0][i]*b))
        print(ynew)
        title = ("y = %.4f" % a + "exp(%.4fx)" % b)
    if param == 'power':
        a = 10 ** a0
        b = a1
        for i in range(0,len(data[0]),1):
            ynew.append(a*float(data[1][i])**b)
        title = ("y = %.4f" % a + "x^ %.4f" % b)
    if param == 'sat':
        a = 1/a0
        b = a1 * a
        for i in range(0,len(data[0]),1):
            data[1][i] = a*float(data[1][i])/(b+float(data[1][i]))
        title = ("y = %.4f" % a + "x/(%.4f + x)" % b)
    plt.scatter(data[0], data[1])
    plt.plot(data[0], ynew)
    plt.title(title)
    plt.grid()
    plt.show()
    return a,b

filename = input("Enter file name:")
f = open(filename, "r")
x, y = [], []
for i in f:  # Creates a list of two lists - x and y vals
    row = i.split()
    x.append(row[0])
    y.append(row[1])
del x[0]  # Deletes headers
del y[0]
data2 = [x, y]

type = input('Should the line go through the origin? Y/N: ')
if type == 'Y':
    linfit(data2, 'linorigin')
elif type == 'N':
    linfit(data2,'lin')

