#! /usr/bin/env python3

import sys
import numpy as np
import matplotlib.pyplot as plt
from time import sleep

def plot_courbe():
    jours, grippe, patho = np.loadtxt('Courbe.dat').T
    plt.plot(jours, grippe)
    plt.plot(jours, patho)
    plt.xlabel('Duree (jours)')
    plt.ylabel('Nb cas')
    plt.draw()


if __name__ == '__main__':

    if '-h' in sys.argv:
        print(f'Usage: {sys.argv[0]} [-r|--refresh]')
        exit(0)

    if '-r' in sys.argv or '--refresh' in sys.argv:
        while True:
            plot_courbe()
            plt.pause(0.2)
            plt.clf()
            
    else:
        plot_courbe()
        plt.show()
