#!/bin/bash

skipGLM=true
skipOpenCV=true
skipbgfx=true
skipOGRE=true
skipOIS=false

if [ "$skipGLM" = false ] ; then
    git clone git@github.com:g-truc/glm.git glm

    cd glm
    git fetch --tags
    git checkout tags/0.9.8.4

    #we dont want to pollute our includes with glm helper
    #files
    mkdir include
    mv glm include/glm

    cd ..
fi

if [ "$skipOpenCV" = false ] ; then
	git clone git@github.com:opencv/opencv.git opencv
	git clone git@github.com:opencv/opencv_contrib.git opencv_contrib

	cd opencv_contrib
	git fetch --tags
	git checkout tags/3.2.0
	cd ..

	cd opencv
	git fetch --tags
	git checkout tags/3.2.0

	mkdir customInstall
	mkdir build
	cd build

	cmake -DOPENCV_EXTRA_MODULES_PATH="../../opencv_contrib/modules" -DWITH_CUDA=OFF -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../customInstall ..
	make -j4
	make install

	cd ../..
fi

if [ "$skipbgfx" = false ] ; then
	git clone git://github.com/bkaradzic/bx.git bx
	git clone git://github.com/bkaradzic/bimg.git bimg
	git clone git://github.com/bkaradzic/bgfx.git bgfx

	cd bgfx
	make linux-debug64

	cd ..
fi

if [ "$skipOIS" = false ] ; then
    clone git@github.com:wgois/OIS.git ois
    cd ois

    #Steps to build on Linux:
    ./bootstrap
    ./configure --prefix ../customInstall/
    ./make
    ./make install

    cd ..
fi

if [ "$skipOGRE" = false ] ; then
    # dependencies for ubuntu/debian
    #sudo apt-get install libfreetype6-dev libfreeimage-dev libzzip-dev libxrandr-dev libxaw7-dev freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev
	if [ ! -f 'ogre_src_v1-8-1.tar.bz2' ] ; then
		wget 'https://downloads.sourceforge.net/project/ogre/ogre/1.8/1.8.1/ogre_src_v1-8-1.tar.bz2'
	fi

	tar xjf ogre_src_v1-8-1.tar.bz2
	cd ogre_src_v1-8-1

	mkdir customInstall
	mkdir build
	cd build
	cmake -DCMAKE_INSTALL_PREFIX=../customInstall -DCMAKE_CXX_FLAGS=-std=gnu++98 ..
	make -j4
	make install

	cd ../..
fi


