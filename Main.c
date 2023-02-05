#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 
// Some Important Libraries 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Some important constants
#define total_technicians 4
#define total_parts 6
// Indexes for the tasks array
#define task_chassis 0
#define task_painting 1
#define task_tires 2
#define task_seats 3
#define task_engine 4
#define task_topcover 5

struct arg_struct {

    int tot_days;
    // Car array to keep track of the number of cars manufactured each day
    int* cars;
};

// Semaphores array for the tasks
sem_t tasks[total_parts];
//  Operations array for each technician
int operations[total_technicians];

//Function to write the output.txt or Log file
void writing_output_file(const char* data)
{
    //file pointer for writing data
    FILE* fwriter;
    //Output file or Log file
    fwriter = fopen("output.txt", "a");
    //checking the file is opened correctly
    if (fwriter == NULL)
    {
        printf("\n--------------------------------------------------");
        printf("\nError: in opening the file output.text.");
        printf("\n--------------------------------------------------");
        exit(1);//closing the program with error flag
    }
    //wrting command onto the output.txt file
    fprintf(fwriter, "%s", data);
    //closing the file to safely open again
    fclose(fwriter);
}

// Function to handle the technician A working
void* technician_a(void* arg)
{
    char tmp_str[255];

    // Cast arg to a pointer to a struct arg_struct
    struct arg_struct* args = arg;

    // Performing the tire and painting tasks
    for (int i = 0; i < args->tot_days; i++)
    {
        // Waiting until the task available
        sem_wait(&tasks[task_tires]); // tires task
        sem_wait(&tasks[task_painting]); // painting task

        // Increment operations for Technician A
        operations[0]++;
        // Increment cars for the day
        args->cars[i]++;

        // Print the transaction
        //converting the all values into one string
        sprintf(tmp_str, "Type A - %d %d tires %d\n", operations[0], args->cars[i], i + 1);
        //writing to the file
        writing_output_file(tmp_str);
        // Print the transaction
        //converting the all values into one string
        sprintf(tmp_str, "Type A - %d %d painting %d\n", operations[0], args->cars[i], i + 1);
        //writing to the file
        writing_output_file(tmp_str);
    }

    return NULL;
}

// Function to handle the technician B working
void* technician_b(void* arg)
{
    char tmp_str[255];

    // Cast arg to a pointer to a struct arg_struct
    struct arg_struct* args = arg;
    // Performing the chassis tasks
    for (int i = 0; i < args->tot_days; i++)
    {
        // Waiting until the task available
        sem_wait(&tasks[task_chassis]); // chassis task

        // Increment operations for Technician B
        operations[1]++;
        // Increment cars for the day
        args->cars[i]++;

        //converting the all values into one string
        sprintf(tmp_str, "Type B - %d %d chassis %d\n", operations[1], args->cars[i], i + 1);
        //writing to the file
        writing_output_file(tmp_str);
    }

    return NULL;
}

// Function to handle the technician C working
void* technician_c(void* arg)
{
    char tmp_str[255];

    // Cast arg to a pointer to a struct arg_struct
    struct arg_struct* args = arg;
    // Performing the seat tasks
    for (int i = 0; i < args->tot_days; i++)
    {
        // Waiting until the task available
        sem_wait(&tasks[task_seats]); // seats task

        // Increment operations for Technician C
        operations[2]++;
        // Increment cars for the day
        args->cars[i]++;

        //converting the all values into one string
        sprintf(tmp_str, "Type C - %d %d seats %d\n", operations[2], args->cars[i], i + 1);
        //writing to the file
        writing_output_file(tmp_str);
    }

    return NULL;
}

// Function to handle the technician D working
void* technician_d(void* arg)
{
    char tmp_str[255];

    // Cast arg to a pointer to a struct arg_struct
    struct arg_struct* args = arg;
    // Performing the engine & top cover tasks
    for (int i = 0; i < args->tot_days; i++)
    {
        // Waiting until the task available
        sem_wait(&tasks[task_engine]); // engines task
        sem_wait(&tasks[task_topcover]); // top cover task

        // Increment operations for Technician D
        operations[3]++;
        // Increment cars for the day
        args->cars[i]++;

    
        //converting the all values into one string
        sprintf(tmp_str, "Type D - %d %d engine %d\n", operations[3], args->cars[i], i + 1);
        //writing to the file
        writing_output_file(tmp_str);

        //converting the all values into one string
        sprintf(tmp_str, "Type D - %d %d top cover %d\n", operations[3], args->cars[i], i + 1);
        //writing to the file
        writing_output_file(tmp_str);
    }

    return NULL;
}

// Driver funtion to test everything
int main(int argc, char* argv[])
{

    //Some important declarations
    int i = 0;
    char tmp_str[255];
    int pt_a, pt_b, pt_c, pt_d, tot_days;
    int ops_per_day[total_parts];//operations per day

    //deleting old output file programmatically
    remove("output.txt");

    // Opening the input text file
    FILE* input_file = fopen("input.txt", "r");
    // Check if file opening is successfull
    if (input_file == NULL)
    {
        printf("\n--------------------------------------------------");
        printf("\nError: opening input file went wrong.");
        printf("\n--------------------------------------------------");
        //closing the program with error flag
        exit(1);
    }
    // Reading the number of technicians from text file
    else
    {
        fscanf(input_file, "%d%d%d%d%d", &pt_a, &pt_b, &pt_c, &pt_d, &tot_days);
        // Read the number of operations per day for each task    
        for (i = 0; i < total_parts; i++)
        {
            fscanf(input_file, "%d", &ops_per_day[i]);
        }
        //closing the file
        fclose(input_file);
    }
    //Multiple arguments to function 
    struct arg_struct args;
    args.tot_days = tot_days;
    args.cars = (int*)malloc(tot_days*sizeof(int));

    // Initializing the operations array to 0
    for (i = 0; i < total_technicians; i++)
    {
        operations[i] = 0;
    }
    // Initializing the cars array to 0
    for (i = 0; i < tot_days; i++)
    {
        args.cars[i] = 0;
    }

    // Initializing the semaphores for every task
    for (int i = 0; i < total_parts; i++)
    {
        sem_init(&tasks[i], 0, ops_per_day[i]);
    }

    //writing header row to the file
    writing_output_file("<Technician_ID Car_ID Operation Simulation_Day>\n");

    // Creating the threads for every technician
    pthread_t thread_a[pt_a];
    pthread_t thread_b[pt_b];
    pthread_t thread_c[pt_c];
    pthread_t thread_d[pt_d];
    i = 0;
    while (i < tot_days)
    {
        if (i < pt_a)
            pthread_create(&thread_a[i], NULL, technician_a, ((void*)&args));
        if (i < pt_b)
            pthread_create(&thread_b[i], NULL, technician_b, ((void*)&args));
        if (i < pt_c)
            pthread_create(&thread_c[i], NULL, technician_c, ((void*)&args));
        if (i < pt_d)
            pthread_create(&thread_d[i], NULL, technician_d, ((void*)&args));
        // To make the program run for a certain number of days, 
        // We can use the sleep function to pause for 3 seconds (1 day) 
        sleep(3);//delay of 3 seconds equal to 1day
        printf("\nDay %d is passed", i + 1);
        i++;
    }

    // Waiting for all threads to finish properly
    for (int i = 0; i < pt_a; i++)
    {
        pthread_join(thread_a[i], NULL);
    }
    for (int i = 0; i < pt_b; i++)
    {
        pthread_join(thread_b[i], NULL);
    }
    for (int i = 0; i < pt_c; i++)
    {
        pthread_join(thread_c[i], NULL);
    }
    for (int i = 0; i < pt_d; i++)
    {
        pthread_join(thread_d[i], NULL);
    }

    // Printing the number of operations for each technician
    for (int i = 0; i < total_technicians; i++) {

        //converting the all values into one string
        sprintf(tmp_str, "Technician %d: %d operations\n", i + 1, operations[i]);
        //writing to the file
        writing_output_file(tmp_str);
        //printf("Technician %d: %d operations\n", i + 1, operations[i]);
    }

    // Printing the number of cars manufactured each day
    for (int i = 0; i < tot_days; i++)
    {
        //converting the all values into one string
        sprintf(tmp_str, "Day %d: %d cars\n", i + 1, args.cars[i]);
        //writing to the file
        writing_output_file(tmp_str);
        //printf("Day %d: %d cars\n", i + 1, cars[i]);
    }

    printf("\nProgram ended successfully!");
    printf("\nPlease check the output file! Good bye\n\n");
    return 0;
}

