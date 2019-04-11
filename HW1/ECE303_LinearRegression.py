from math import exp, log
from matplotlib import pyplot as plt


def linfit(data, param):
    n = len(data[0])  # Number of data points
    sumx = 0.0  # Initializes variables to 0
    sumy = 0.0
    sumxy = 0.0
    sumx2 = 0.0
    for i in range(0,n,1):  # For i in the range the length of the data set
        sumx += float(data[0][i])  # Sum of all x val
        sumy += float(data[1][i])  # Sum of all y val
        sumxy += float(data[0][i])*float(data[1][i])  # Sum of all x*y val
        sumx2 += float((data[0][i]))**2  # Sum of all x squared val
    ynew = []
    st = 0
    sr = 0
    if param == 'lin':
        a1_lin = (float(n) * float(sumxy) - float(sumx) * float(sumy)) / (float(n) * float(sumx2) - float(sumx) ** 2)
        a0_lin = sumy / float(n) - a1_lin * sumx / float(n)
        for i in range(0,len(data[0]),1):
            ybar = a0_lin+a1_lin*float(data[0][i])
            ynew.append(ybar)
            sr += pow(float(data[1][i]) - ybar, 2)
            st += pow((float(data[1][i]) - (sumy/len(data[1]))), 2)
        a = a0_lin
        b = a1_lin
        rsq = (st - sr)/st
        title = ("Lin Reg:  y = %.4f " % a0_lin + "+ %.4f x, r^2 = %.4f" % (a1_lin, rsq))
        print(rsq)
    if param == 'linorigin':
        a1_linorg = sumxy/sumx2
        a0_linorg = 0
        for i in range(0, len(data[0]), 1):
            ybar = a1_linorg * float(data[0][i])
            ynew.append(ybar)
            sr += pow(float(data[1][i]) - ybar, 2)
            st += pow((float(data[1][i]) - (sumy/len(data[1]))), 2)
        a = a0_linorg
        b = a1_linorg
        rsq = (st - sr)/st
        title = ("Lin Reg through origin:  y = %.4f x, r^2 = %.4f" % (a1_linorg, rsq))
        print(rsq)
    # resistance = (sumx/len(x))/(sumy/len(y))
    # title = ("Estimated Resistance: %.4f" % resistance + "kΩ")
    plt.scatter(data[0], data[1])
    plt.plot(data[0], ynew)
    plt.title(title)
    # plt.legend(["Actual Resistance: 1.9603 kΩ"])
    # plt.xlabel("Voltage (V)")
    # plt.ylabel("Current (mA)")
    plt.grid()
    plt.xlabel("x")
    plt.ylabel("y")
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

fit_type = input('Should the line go through the origin? Y/N: ')
if fit_type == 'Y':
    linfit(data2, 'linorigin')
elif fit_type == 'N':
    linfit(data2, 'lin')
