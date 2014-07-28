#!/usr/bin/env python
# encoding: utf-8

import re
import sys
import matplotlib.pyplot as plt

if __name__ == '__main__' :
    plt.subplot(211)
    for string in open('linetest.txt', 'r') :
        string = string.strip()
        string = string.split()
        xArray = []
        yArray = []
        for strTmp in string :
            strTmp = strTmp.split(',')
            xArray.append(strTmp[0])
            yArray.append(strTmp[1])

        plt.plot(xArray, yArray, "g--")

    for string in open('pointtest.txt', 'r') :
        string = string.strip()
        string = string.split(",")
        plt.plot(string[0], string[1], "r.")

    plt.subplot(212)
    for string in open('anstest.txt', 'r') :
        string = string.strip()
        string = string.split()
        xArray = []
        yArray = []
        for strTmp in string :
            strTmp = strTmp.split(',')
            xArray.append(strTmp[0])
            yArray.append(strTmp[1])

        plt.plot(xArray, yArray, "g--")

    for string in open('pointtest.txt', 'r') :
        string = string.strip()
        string = string.split(",")
        plt.plot(string[0], string[1], "r.")

    plt.show()
