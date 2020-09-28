#include<stdio.h>
#include"omp.h"
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#define BUFSIZE 1024


	int *vipBilet;
	int *normalBilet;
	int *vipBiletTalep;
	int *normalBiletTalep;
  unsigned short lfsr = 0xFBE1u;
  unsigned bit;
  static int a[10] = {0};
  static int b[10] = {0};
  
unsigned myRand()
    {
    bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    return lfsr =  (lfsr >> 1) | (bit << 15);
    }

void dosyaOku()
{
	char buf[BUFSIZE];
	FILE *fp;
	fp=fopen("input.txt","r");
	if (fp == NULL)
  	{ 
    perror("fopen source-file");
  	}
	 int count=1;
	 while(fgets(buf,sizeof(buf), fp) != NULL)
	 {
	 	buf[strlen(buf) -1 ] = '\0';
	 	//printf("%s\n", buf);

	 		if(count==1)
	 			{
	 				sscanf(buf,"%d",&vipBilet);
	 				if(vipBilet<2)
	 				{
	 					printf("Hatali Bilet Sayisi , Output.txt olusturulamadi\n");   
	 					exit(0);
					 }
				}
			else if(count==2)
	 			{
	 				sscanf(buf,"%d",&normalBilet);
					 if(normalBilet<2)
	 				{
	 					printf("Hatali Bilet Sayisi , Output.txt olusturulamadi\n");
	 					exit(0);
					 }	
				}
			else if(count==3)
	 			{
	 				sscanf(buf,"%d",&vipBiletTalep);
				}
			else if(count==4)
	 			{
	 				sscanf(buf,"%d",&normalBiletTalep);
				}
           	count++;
	 }
	  
	  fclose(fp);
}

int main()
{
	
	srand((time(NULL)));
	lfsr=rand()%65536;
	
    int i = 0;
	dosyaOku();
	int tvipBilet=vipBilet;
	int tnormalBilet=normalBilet;
	int tvipBiletTalep=vipBiletTalep;
	int tnormalBiletTalep=normalBiletTalep;
	
	 FILE *f;
     f=fopen("output.txt","w");
	//printf("\n%d\n",tvipBilet);
	//printf("\n%d\n",tnormalBilet);
	//printf("\n%d\n",tvipBiletTalep);
	//printf("\n%d\n",tnormalBiletTalep);
	fprintf(f,"// vip bileti alan threadler (sirali vip numarasi - thread numarasi þeklinde)\n");
	omp_set_num_threads(tvipBiletTalep);
     #pragma omp parallel 
    {
    		
          #pragma omp for 
            
            for ( i = 0; i < tvipBilet; i++)
            {
                #pragma omp critical 
            	b[i] = myRand()%omp_get_num_threads()+1;
            }

         #pragma omp single
         {
              printf("\n");
              for(i = 1; i<=tvipBilet;i++){
              printf("%d : %d\n",i,b[i-1]);
              fprintf(f,"%d : %d\n",i,b[i-1]);
             }
         }
     }
	fprintf(f,"// normal bileti alan threadler (sirasiz þekilde bileti alabilen thread numaralari)\n");
    omp_set_num_threads(tnormalBiletTalep);
     #pragma omp parallel 
    {
          #pragma omp for
            for ( i = 0; i < tnormalBilet; i++)
            {
                #pragma omp critical 
                a[i] = (myRand()%omp_get_num_threads() +1) +((tvipBiletTalep)+1);
            }


         #pragma omp single
         {
              printf("\n");
              for(i = 1; i<=tnormalBilet;i++){
              
              printf("%d\n",a[i-1]);
              
              fprintf(f,"%d\n",a[i-1]);
             }
         }
         
     }
     fclose(f);
	 
	 
	 
      getchar();
	 }

