<<<<<<< HEAD
# Go compiling server & client
gcc socket_client.c -o client
gcc socket_server.c -o server
./server
=======
# Go compiling & running.
gcc -D_REENTRANT thread_work.c -lpthread -o thread
./thread
>>>>>>> 20a2d3c9f689cbcfd92cf7e98f1e4495106b0edf
