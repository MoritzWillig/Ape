#!/bin/bash

skipGLM=false
skipOpenCV=false
skipOGRE=false
skipGLFW=false
skipFastNoise=false

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

if [ "$skipGLFW" = false ] ; then
	git clone git@github.com:glfw/glfw.git glfw
	cd glfw
	git fetch --tags
	git checkout tags/3.2.1
	mkdir build
	cd build

	cd ../..

	cmake .. -DCMAKE_INSTALL_PREFIX=../../customInstall
	make
	make install
fi

if [ "$skipFastNoise" = false ] ; then
	git clone git@github.com:Auburns/FastNoise.git fastNoise
	cd fastNoise
	git fetch --tags
	git checkout tags/0.4

	cp ../misc/fastNoise_CMakeLists.txt ./CMakeLists.txt

	mkdir build
	cd build

	cmake .. -DCMAKE_INSTALL_PREFIX=../../customInstall
	make
	make install

	cd ../..
fi

