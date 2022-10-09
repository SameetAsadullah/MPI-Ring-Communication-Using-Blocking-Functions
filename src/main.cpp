#include <iostream>
#include "mpi.h"
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <unistd.h>
using namespace std;

int main(int argc, char **argv)
{
    int rank, root = 0, nprocs, namelen;
    char processorName[10];

    // getting information of process and machine
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Get_processor_name(processorName, &namelen);

    if (rank == root) {     // master process
        int value_to_send = 9;  // value to send
        MPI_Send(&value_to_send, 1, MPI_INT, 1, 1234, MPI_COMM_WORLD);  // sending value from process 0 to process 1

        MPI_Status status;
        char data[9];
        MPI_Recv(data, 9, MPI_CHAR, 1, 1235, MPI_COMM_WORLD, &status); // receiving acknowledgement message from process 1
        cout << data << endl;
        
    } else {    // slave processes
        MPI_Status status;
        int value_to_send;
        MPI_Recv(&value_to_send, 1, MPI_INT, MPI_ANY_SOURCE, 1234, MPI_COMM_WORLD, &status);    // receiving value sent from any process
        
        char data[9] = "Received";
        MPI_Send(data, 9, MPI_CHAR, status.MPI_SOURCE, 1235, MPI_COMM_WORLD);   // sending acknowledgement message

        if (rank != nprocs - 1) {   // if its not the last process then send and receive message
            MPI_Send(&value_to_send, 1, MPI_INT, rank + 1, 1234, MPI_COMM_WORLD);   // sending message to next process
            MPI_Recv(data, 9, MPI_CHAR, MPI_ANY_SOURCE, 1235, MPI_COMM_WORLD, &status); // receiving acknowlegement message from that process
            cout << data << endl;
        }
    }
    MPI_Finalize();
}
