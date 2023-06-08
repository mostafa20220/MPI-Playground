#include <stdio.h>
#include <mpi.h>

const int MAX_STRING = 100;

int main(){

  int comm_size, my_rank;
  char msg [MAX_STRING];

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

  if (my_rank != 0)
  {
    sprintf(msg,"This is process: %d of %d\n",my_rank,comm_size);
    MPI_Send(msg,MAX_STRING,MPI_CHAR,0,0,MPI_COMM_WORLD);
  }
  else{

    printf("This is process: %d of %d\n",my_rank,comm_size);

    for (int src = 1; src < comm_size; src++)
    {
      MPI_Recv(msg,MAX_STRING,MPI_CHAR,src,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      printf("%s",msg);
    }
    
  }
  
  MPI_Finalize();

  return 0;
}