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

cmake -DOPENCV_EXTRA_MODULES=../../opencv_contrib/modules -DWITH_CUDA=OFF -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../customInstall ..
make -j4
make install
