# Interprettable OF App

**Requisites :** 

 - Must be installed on Ubuntu 18
 - User should be `interpretable`

  

**Installation steps :**    
Navigate to Desktop :   

    cd ~/Bureau

Download installation script : 

    wget -O install.sh https://gist.githubusercontent.com/martial/41d58672af223042d7b366895d1ac3e5/raw/2a2a940f41c5887de0ee3b9079f45039cf9ccf14/interpretable.sh
    sh ./install.sh

Download, compile and install openCV :

    cd inteprettable
    sh ./build-opencv.sh

Compile app : 

    cd openFrameworks/apps/InterprettableApp/Interprettable/
    make -j4
    make RunRelease

Configure app :

    nano  openFrameworks/apps/InterprettableApp/Interprettable/bin/data/config.json
Configure `machine-id` with right id in database
