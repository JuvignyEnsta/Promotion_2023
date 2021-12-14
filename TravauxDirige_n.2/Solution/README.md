# Ensemble de Mandelbrot

## Partage équitable des lignes

Stratégie : 
* on découpe l'image en images locales (`pixels_loc`) à chaque processeur.
* puis on regroupe (`MPI_Gather`) les résultats sur le processeur 0, qui fera la sauvegarde.

Vérification : 
* à l'oeil, l'image résultat semble bonne,
* pour le confirmer : `md5sum mandelbrot.tga` donne bien la même checksum sur l'image séquentielle et sur l'image issue du calcul parallèle.


Les chronos sur chacun des processeurs montrent un fort déséquilibrage de charge :

	mpirun -np 4 ./a.out
	[1] Temps calcul ensemble mandelbrot : 4.42731
	[2] Temps calcul ensemble mandelbrot : 4.44437
	[3] Temps calcul ensemble mandelbrot : 6.16051
	[0] Temps calcul ensemble mandelbrot : 6.16513
	[0] Temps total : 6.16666
	[0] Temps save : 0.030478

    mpirun -np 8 Mandelbrot_mpi.exe
	[7] Temps calcul ensemble mandelbrot : 0.284302
	[0] Temps calcul ensemble mandelbrot : 0.286832
	[4] Temps calcul ensemble mandelbrot : 2.26576
	[2] Temps calcul ensemble mandelbrot : 2.31482
	[3] Temps calcul ensemble mandelbrot : 2.33719
	[5] Temps calcul ensemble mandelbrot : 2.38992
	[1] Temps calcul ensemble mandelbrot : 6.00454
	[6] Temps calcul ensemble mandelbrot : 6.0788
	[0] Temps total : 6.07902
	[0] Temps save : 0.0326714

Ainsi, le temps total de la simulation est celui du processeur le plus lent 6s, contre 19.2s en séquentiel. 
Ce qui fait un speedup d'un peu plus de 3 sur 4 ou sur 8 processeurs. 


## Maître-esclave

On met donc en place une stratégie maître-esclave pour gérer dynamiquement ce déséquilibre.
(on peut d'ailleurs le comparer à la version `omp schedule(dynamic)` du fichier `Mandelbrot_omp.cpp`.


Pour 4 process MPI, les chronos annoncent :

	mpirun --oversubscribe -np 4 ./a.out
	[1] Temps calcul ensemble mandelbrot : 6.55791
	[1] Temps total : 6.55799
	[2] Temps calcul ensemble mandelbrot : 6.55792
	[2] Temps total : 6.558
	[3] Temps calcul ensemble mandelbrot : 6.56244
	[3] Temps total : 6.56248
	[0] Temps calcul ensemble mandelbrot : 6.56237
	[0] Temps total : 6.56241
	[0] Temps save : 0.0309291

L'équilibrage de tâche est bon : tous les processeurs ont calculé un temps similaire.
Attention pour le calcul de speedup : comme le process maître ne travaille pas, il n'y a que 3 travailleurs, donc le temps mesuré de 6.5s par rapport à 19.2s est très proche du speedup idéal (qui serait à 6.4s).


Pour 8 process MPI, les chronos annoncent :

	mpirun --oversubscribe -np 8 ./a.out
	[2] Temps calcul ensemble mandelbrot : 2.90237
	[2] Temps total : 2.90263
	[6] Temps calcul ensemble mandelbrot : 2.90308
	[6] Temps total : 2.90314
	[1] Temps calcul ensemble mandelbrot : 2.90431
	[1] Temps total : 2.90436
	[5] Temps calcul ensemble mandelbrot : 2.90475
	[5] Temps total : 2.90481
	[7] Temps calcul ensemble mandelbrot : 2.90621
	[7] Temps total : 2.90626
	[3] Temps calcul ensemble mandelbrot : 2.90847
	[3] Temps total : 2.90854
	[4] Temps calcul ensemble mandelbrot : 2.91482
	[4] Temps total : 2.91487
	[0] Temps calcul ensemble mandelbrot : 2.9146
	[0] Temps total : 2.91465
	[0] Temps save : 0.0328418

Le speedup idéal donnerait 19.2/7 = 2.7s ; à nouveau on en est très proche.
Par rapport au découpage statique, on constate le gain notable du M/S : avec 8 processeurs on descend de 6.5s à 2.9s.
