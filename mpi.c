#include <stdio.h>
#include <mpi.h>

const int MAX_STRING = 100;

void getInput(int myRank, int commSize, double *globalStart, double *globalEnd, int *n)
{

  if (myRank == 0)
  {
    printf("Enter start, end, and n :\n");
    fflush(stdout);
    scanf("%lf %lf %d",globalStart, globalEnd, n);

    for (int dest = 1; dest < commSize; dest++)
    {
      MPI_Send(globalStart, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
      MPI_Send(globalEnd, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
      MPI_Send(n, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
    }
  }
  else
  {
    MPI_Recv(globalStart, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(globalEnd, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
}

double f(double x)
{
  return x*x;
}

double trap(double localStart, double localEnd ,double h, int n )
{
  int i;
  double estimate = ( f(localStart) + f(localEnd) ) / 2.0;  
  for (i = 1; i < n; i++)
  {
    estimate+= f(localStart + i * h); 
  }
  estimate*= h;
  
  return estimate;
}


int main()
{

  int commSize, myRank;
  // char msg[MAX_STRING];

  double globalStart, globalEnd, globalInteg, localStart, localEnd, localInteg , h;
  int n, localN;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

  getInput(myRank, commSize, &globalStart, &globalEnd, &n);
  h = globalEnd - globalStart / n;
  localN = n / commSize;
  localStart = globalStart + myRank * h;
  localEnd   = localStart  + localN * h;

  localInteg = trap(localStart,localEnd,h,localN);

  if(myRank!=0){
    MPI_Send(&localInteg,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
  }else{
    globalInteg=localInteg;
    for (size_t i = 1; i < commSize; i++)
    {
      MPI_Recv(&localInteg,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      globalInteg+=localInteg;
    }
    printf("The global Integ= %lf\n",globalInteg);
    
  }

  MPI_Finalize();

  return 0;
}