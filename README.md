# c++-11-thread-pool
This is a simple thread pool implemented with c++11

## a simple description
I implement a sync queue and a thread pool here. The users just put packaged_task into the queue, then the underneath threads will take over these tasks and run them. Thus you can benefit from future and packaged_tasks and controll the number of threads used.

For detail usage, see the main.cpp.

## Attention
use -std=c++11 or higher standard if you use old versions of compiler.
This is just for learning :)  
I just write it for practice. 
