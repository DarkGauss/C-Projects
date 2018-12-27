To compile the program type:
    make

To run the program type:
    ./sim (int simulation run time in secconds) (int max time between production in ms) (int max time between consumption in ms)

Example:
    The following command will simulate a producer and consumer for 1 seccond, with a maximum
    time to produce as 40 ms and the maximum time to consume as 20ms.
    ./sim 1 40 20


Aditional features:
    1) Change #define debug from 0 to 1 to see verbatium output of what each proccess is doing, this
    could be usefull for grading.
    2) Change the defines for P_NUM and C_NUM to vary the amount of producers and consumers. It works best 
       for small values. Be very careful, When I tested it with 1000 producers, and 1000 consumers, my computer
       slowed to a crawl.
