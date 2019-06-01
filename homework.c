/* mp0.c */

#include <stdio.h>

#include <stdlib.h>

#include <sys/types.h>

#include <unistd.h>

#include <sys/wait.h>



int main(void)

{

    pid_t child1, child2, child;

    

    /* Create the 1st child */

    child1 = fork();        

    /* Error processing */

    if ( WRITE_YOUR_CODE_HERE )

    {

        printf("Child1 fork error\n");

        exit(1);

    }

    else 

        if ( WRITE_YOUR_CODE_HERE ) /* Call execlp in the 1st child */

       {

           printf("In child1: execute 'ls -l'\n");

           if (execlp("ls", "ls", "-l", NULL) < 0)

           {

               printf("Child1 execlp error\n");

           }

       }

      else /* Create the 2nd child, waiting for the two processes to exit */

      {

          child2 = fork();

          if (child2 == -1) /* Error processing */

          {

              printf("Child2 fork error\n");

              exit(1);

          }

          else if( WRITE_YOUR_CODE_HERE ) /* waiting 4 seconds in the 2nd child */

          {

              printf("In child2: sleep for 4 seconds and then exit\n");

            WRITE_YOUR_CODE_HERE;

            exit(0);

        }



        printf("In father process:\n");

        child = waitpid(WRITE_YOUR_CODE_HERE, NULL, 0); /* waiting the 1st child to exit in BLOCKed way  */

          if ( child == WRITE_YOUR_CODE_HERE )

          {

              printf("Get child1 exit code\n");

          }

          else

          {

              printf("Error occured!\n");

          }

          

          do

          {

              child = WRITE_YOUR_CODE_HERE; /* waiting the 2nd child to exit in NONBLOCKed way */

              if (child == 0)

              {

                  printf("The child2 process has not exited!\n");

                  sleep(1);

              }

          } while (child == 0);

          

          if (child == child2)

        {

            printf("Get child2 exit code\n");

        }

        else

        {

            printf("Error occured!\n");

        }

    }

    return 0;

}
