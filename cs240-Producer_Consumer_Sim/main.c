#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h> 

#define DEBUG 0
#define P_NUM 4
#define C_NUM 4

//initialize tables to store fd's accosiated with pipes
int P_tbl[P_NUM][2];
//stores fd giving char to consumer
int C_tbl_in[C_NUM][2];
//used to signal when consumer is hungry
int C_tbl_out[P_NUM][2];

//stores running average
float average;
//store list of procceses to kill
int kill_list[P_NUM+C_NUM+1];

//Filles the tables above with pipes.
void fill_tables();


//prints an error when system call fails
void e_sys();

/**
 * Implements the functionality of a producer.
 * Takes an id as an input which deterimens which
 * P_tbl pipe it will produce on and a max time
 * in ms which the random production time will
 * be less then or equal to.
 */
void producer(int id, int t_max_p);

/**
 * Implements the functionality of a Consumer.
 * Takes an id as an input which deterimens which
 * C_tbl_in pipe it will consume from and also which
 * C_tbl_out it will signal when it's ready to consumer on.
 * Also, it takes a max time
 * in ms which the random consumption time will
 * be less then or equal to.
 */
void consumer(int id, int t_max_c);

/**
 * Creates a buffer of size 1000, and continously
 * reads in char's from P_tbl and stores them in the buffer.
 * Also, when it receivers a charector on
 * C_tbl_out it will either:
 *      If the buffer has items, remove the last one
 *      and provide it on C_tbl_in to the corsponding id
 *    Or If the buffer is empty, signal that this consumer
 *     is waiting, and when the buffer has items, remove the last
 *     one and produce it on C_tbl_in.
 * 
 */
void buffer();

/**
 * Used my main to signal the buffer to print the average once the time
 * has expired
 */
void handler(int sig){
    fprintf(stderr, "Average Buffer size: %d\n",(int)average);
    exit(0);
}

/**
 *  Handles fatal errors my making sure every proccess is killed
 *  and printing the last perror msg.
 */
void error_handler(int sig){
    int i;
    perror("Last system call error");
    fprintf(stderr, "Killing all procceses...\n");
    for(i = 0; i < C_NUM+P_NUM+1; i++){
        kill(kill_list[i], SIGKILL);
    }
    exit(1);
}

int main(int argc, char *argv[] )
{
    //tell the user if the inputed invalid arguments
    if(argc !=4){
        fprintf(stderr,"Input valid arguements in the form: time(sec) p_rand_max(ms) c_rand_max(ms)");
    }

    //bind the error handler so we can signal it
    signal(SIGUSR2, error_handler);

    //stores the maximum runtime, the max production time, and consumptuon time
    int max_time = atoi(argv[1]);
    int t_max_p = atoi(argv[2]);
    int t_max_c = atoi(argv[3]);

    int i; //loop counter


    //initialize all the fd tables
    fill_tables();
    
    //fork all the requiste procceses
    if(!(kill_list[P_NUM+C_NUM] = fork())){
        buffer();
    }
    for(i = 0; i<P_NUM; i++)
    {
        if(!(kill_list[i] = fork())){
            //producer is infinite loop, so don't need to wory about executing after
            producer(i,t_max_p);
        }
    }
    for(i = 0; i<C_NUM; i++)
    {
        if(!(kill_list[i+P_NUM] = fork())){
            //producer is infinite loop, so don't need to wory about executing after
            consumer(i, t_max_c);
        }
    }

    //check to make sure all forks were succesful
    for(i = 0; i <= P_NUM+C_NUM; i++){
        if(kill_list[i] < 0) error_handler(0);
    }

    sleep(max_time);
    //kill all the procceses after we have slept for the runtime
    for(i = 0; i < C_NUM+P_NUM; i++){
        kill(kill_list[i], SIGKILL);
    }
    //send a signal to the buffer to print it's average, and exit.
    kill(kill_list[C_NUM+P_NUM], SIGUSR1);
    exit(0);

}

void e_sys(){
    perror("Error");
    exit(1);
}

void buffer()
{
    //setup signal handler to print the average
    signal(SIGUSR1, handler);
    char temp_msg;
    //counter
    int i;
    //read count for avg
    long int read_count = 0;

    //stores which consumer tried to consume when the buffer is empty
    //in order to allow it to consume when new charectors arrive
    int waiting[C_NUM] = {0};

    //implemented a circular queue instead of a stack at the request of axel
    int front=0;
    int rear=999;
    char buffer[1000];
    int cur_size = 0;
    average = 0;

    while(1)
    {
        //read in from producer pipes
        for(i = 0; i < P_NUM; i++)
        {
            //since non blocking, see if something is there
            if(read(P_tbl[i][0], &temp_msg, 1) > 0)
            {
                read_count++;
                cur_size++;
                if(cur_size >= 1000){
                    fprintf(stderr, "FAILURE! exceed buffer size of 1000. Axel said we didn't have to handle this case.\n");
                    fprintf(stderr, "If there is to large of an inbalance between the p_max and c_max,\nconsider runnning for a shorter time\n");
                    kill(getppid(), SIGUSR2);
                    exit(1);
                }
                if(DEBUG) fprintf(stderr, "--Buffer read in %c from %d and the current size is %d.\n",temp_msg, i, cur_size);

                //incrimantly recalc the average with the forumale avg_old*(n_old/n_new)+new_size/n_new
                //calculated in this since we don't want a divide by zero before the first charector is read
                average = average*((float)(read_count-1)/(float)read_count)+(float)cur_size/(float)read_count;
                rear = (rear+1)%1000;
                buffer[rear] = temp_msg;
            }
        }

        for(i = 0; i < C_NUM; i++)
        {
            //since non blocking, see if something is there
            if(((read(C_tbl_out[i][0], &temp_msg, 1) > 0)&& (temp_msg == 'R')) || waiting[i] == 1)
            {
                if(DEBUG && !waiting[i]) fprintf(stderr, "--Buffer recived consume request from %d and the buffer size is %d.\n", i, cur_size);
                //if the buffer is empty, queue em up
                if(cur_size == 0){
                    waiting[i] = 1;
                    //if(DEBUG) fprintf(stderr, "--Buffer waiting till buffer filles for %d.\n", i);
                    continue;
                }
                waiting[i] = 0;
                read_count++;
                cur_size--;


                //incrimantly recalc the average with the forumale avg_old*(n_old/n_new)+new_size/n_new
                //calculated in this since we don't want a divide by zero before the first charector is read
                average = average*((float)(read_count-1)/(float)read_count)+(float)cur_size/(float)read_count;
                write(C_tbl_in[i][1], &(buffer[front]), 1);
                front = (front +1)%1000;
                if(DEBUG) fprintf(stderr, "--Buffer wrote out %c to %d and the buffer size is %d.\n",buffer[cur_size], i, cur_size);
            }
        }

    }
}
void producer(int id, int t_max_p)
{
    srand(id);
    int sleep_time;
    char msg;
    while(1)
    {
        sleep_time = rand() % (t_max_p+1);
        msg = (char)((rand()%26)+65);
        if(DEBUG) fprintf(stderr, "--Producer %d is sleeping for %d ms with messege %c.\n",id,sleep_time, msg);

        //usleep uses microseconds, thus 1000
        usleep(1000*sleep_time);
        fprintf(stderr, "Producer %d: value = %c, tp=%dms\n",id, msg, sleep_time); 
        write(P_tbl[id][1], &msg, 1);
    }
}

void consumer(int id, int t_max_p)
{
    srand(id);
    int sleep_time;
    char msg;
    char ready_to_send = 'R';
    while(1)
    {
        sleep_time = rand() % (t_max_p+1);
        if(DEBUG) fprintf(stderr, "--consumer %d is sleeping for %d ms\n",id,sleep_time);
        //usleep uses microseconds, thus 1000
        usleep(1000*sleep_time);
        if(DEBUG) fprintf(stderr, "--consumer %d sent a msg to the buffer saying it's ready.\n",id);
        write(C_tbl_out[id][1], &ready_to_send, 1);


        //busy waiting wooooooo
        while(read(C_tbl_in[id][0], &msg, 1) <0) ;
        fprintf(stderr, "Consumer %d: value = %c, tc=%dms\n",id, msg, sleep_time); 
    }
}

void fill_tables()
{
    int i;

    for(i = 0; i < P_NUM; i++)
    {
        if(pipe(P_tbl[i]) < 0) e_sys();
        //set read ends to non blocking
        if(fcntl(P_tbl[i][0], F_SETFL, O_NONBLOCK)<0) e_sys();
    }
    for(i = 0; i < C_NUM; i++)
    {
        if(pipe(C_tbl_in[i]) < 0) e_sys();
        if(fcntl(C_tbl_in[i][0], F_SETFL, O_NONBLOCK)<0) e_sys();
    }
    for(i = 0; i < C_NUM; i++)
    {
        if(pipe(C_tbl_out[i]) < 0) e_sys();
        if(fcntl(C_tbl_out[i][0], F_SETFL, O_NONBLOCK)<0) e_sys();
    }
}