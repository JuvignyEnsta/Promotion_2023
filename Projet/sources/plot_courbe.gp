#! /usr/bin/env gnuplot

set xlabel 'Temps (jours)'

# 1e version, echelle y commune
set ylabel 'Nb cas'
plot "Courbe.dat" using 1:2  t 'Grippe', \
     "Courbe.dat" using 1:3  t 'Pathogene'


# 2e version, y2axis
# set ylabel 'Grippe (nb cas)'
# set y2label 'Pathogene (nb cas)'
# set y2tics nomirror
# plot "Courbe.dat" using 1:2 axis x1y1 t 'Grippe', \
#      "Courbe.dat" using 1:3 axis x1y2 t 'Pathogene'

pause mouse key "Tapez n'importe quelle touche pour quitter"
