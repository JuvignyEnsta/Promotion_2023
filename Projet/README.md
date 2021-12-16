# Prerequis

    sudo apt install g++-10 
    sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev

Et si besoin modifier Makefile et Make_linux.inc pour utiliser g++-10. 


# Compiler mpi avec g++-10

Le plus simple : 

    export OMPI_CXX=g++-10


Ajouter les CFLAGS et LDFLAGS dans le Make_linux.inc

    CXXFLAGS+=$(mpic++ --showme:compile)
    LIBS+=$(mpic++ --showme:link)
    
Autre option, faire un lien symbolique g++ -> /usr/bin/g++10 dans $HOME/bin et le mettre en premier dans PATH) :

     mkdir $HOME/bin
     cd $HOME/bin
     ln -s /usr/bin/g++-10 g++
     export PATH=$HOME/bin:$PATH   # dans votre .bashrc
     which -a g++
     g++ --version   # afficher 10.xx
    
