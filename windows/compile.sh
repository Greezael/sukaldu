MXE_HOME="mxe"
export PATH=$PATH:"$MXE_HOME/usr/bin/"

mkdir release

cp ../*.h .
cp ../*.cpp .
cp ../*.ui .
mkdir release/translations
cp ../translations/* ./release/translations/
mkdir release/data
cp ../data/* ./release/data/
mkdir translations
cp ../Sukaldu.pro .
cp ../Sukaldu.pro.user .

i686-w64-mingw32.static-qmake-qt5
make

