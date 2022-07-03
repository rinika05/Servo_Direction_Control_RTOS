#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 10

typedef struct
{
    int pid;
    float period;
    float exe_time;
} Process;

void print_table(Process p[], int n);
void print_gantt_chart(Process p[], int n);

int main()
{
    Process p[MAX] = {0};
    int i, j, n, y;
    float U, B;
    printf("Enter total number of process: ");
    scanf("%d", &n);
    
    for(i=0; i<n; i++) 
    {
        p[i].pid = i+1;
        printf("Enter Period for P%d :",(i+1));
        scanf("%f", &p[i].period);
        printf("Enter Execution time for P%d:",(i+1));
        scanf("%f", &p[i].exe_time);
    }

    //calculate bound value
    float c = ((float)1/(float)n);
    B=n*((pow(2,c)-1));
    printf("\nBound Value = %f",B);
    
    //calculate the utilization
    for(i=0;i<n;i++)
    {
        U += p[i].exe_time/p[i].period;
    }
    printf("\nUtilization = %f",U);
      if(U<B)
    {
        printf("\n As %f < %f ,",U,B);
        printf("\n\t\t\t\t\t The System is surely Schedulable\n\n");
        puts(""); // Empty line
        print_table(p, n);
        puts(""); // Empty Line
        
        for (i=0;i<n;i++) 
        {
            for (j = i + 1; j < n; j++)
            {
                if (p[i].period > p[j].period) 
                {
 
                    int a =  p[i].pid;
                    p[i].pid = p[j].pid;
                    p[j].pid = a;
                 }
             }
        }
        
        printf("\nThe priorities scheduled are \n");
        for (i = 0; i < n; i++)
            printf("P%d\n", p[i].pid);
    
        puts("          GANTT CHART          ");
        puts("          ***********          ");
        print_gantt_chart(p, n);
        return 0;
    }
    
    if(U>B)
    {
        printf("\n As %f > %f ,",U,B);
        printf("\n The System is not Schedulable");
    }
  
    else
        printf("\n Not Sure.....");
}


void print_table(Process p[], int n)
{
    int i;
    puts("+----+------------+------------");
    puts("|Task|  Period    |  Execution |");
    puts("+----+------------+------------");

    for(i=0; i<n; i++) {
        printf("| %d  |   %.2f   |   %.2f     |\n"
               , p[i].pid,p[i].period, p[i].exe_time );
        puts("+----+------------+------------");
    }
}


void print_gantt_chart(Process p[], int n)
{
    int i, j;
    // print top bar
    printf(" ");
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].period; j++) printf("--");
        printf(" ");
    }
    printf("\n|");

    // printing process id in the middle
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].period - 1; j++) printf(" ");
        printf("P%d", p[i].pid);
        for(j=0; j<p[i].period - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");
    // printing bottom bar
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].period; j++) printf("--");
        printf(" ");
    }
    printf("\n");

}
 

