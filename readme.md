<B>Process Resource Simulator </B>

This is a program written for Linux/Unix formatted text files that simulates Process and Resource Management.
The program takes into account three different states, ready, running and blocked.

The process have different operations such as create, request, release, and destroy.
This aspect is simulated by a text file input. There are 4 resources, having {{1},{2},{3},{4}} resources.
When process request resources, it checks whether or not the resource is available, if not then it will become blocked.

The process also have a priority assigned, and there is only a single process active (a single core machine).
The process also have a "parent" process and potentially a "child" process, with the topmost being the "init".

Destroying a process also destroys its children and releases resources. The process will be unblocked based on
FIFO order of a queue of the particular resource they are blocked on.
