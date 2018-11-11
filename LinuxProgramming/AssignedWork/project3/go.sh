# Go compiling & running this program using gcc.

export LD_LIBRARY_PATH=`pwd`
gcc jctl.c -shared -o libjctl.so
gcc test.c -L. -ljctl -o usecase
./usecase
