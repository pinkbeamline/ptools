#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <dbDefs.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <aSubRecord.h>
#include <epicsExport.h>

//Global variables


static long linspace(aSubRecord *precord)
{
	double *lvala=precord->vala;   // Pointer to output A
	double *la=precord->a;
	double *lb=precord->b;
	double *N=precord->c;
	double step;

	if(*N>0){
		step=(*lb-*la)/(*N-1);
		for(int i=0; i<*N; i++){
			lvala[i]=*la+(i*step);
		}
		precord->neva=*N;
	}else{
		return 1;
	}

	return 0;
}

static long calcspec(aSubRecord *precord)
{
	double *profile=precord->a;
	double *height=precord->b;
//	double *izero=precord->c;
	double *size=precord->d;
//	double *convertion=precord->e;
	double *spec=precord->vala;

	if((*size>0) && (*size<=1100)){
		for(int i=0;i<*size;i++){
			//spec[i]=profile[i]*(*height)*(*convertion)/(*izero);
			spec[i]=profile[i]*(*height);
		}
		precord->neva=*size;
	}else{
		return 1;
	}

	return 0;
}

static long profilex(aSubRecord *precord)
{
        double *image=precord->a;
	double *width=precord->b;
        double *height=precord->c;
        double *spec=precord->vala;
	int index,i,j;


	if( (*width * *height) > 0.0 ){

		precord->neva=*width;

		for(i=0;i<*width;i++){
			spec[i]=0.0;
		}

		for(j=0;j<*height;j++){
			for(i=0;i<*width;i++){
				index=(j * *width)+i;
				spec[i]+=image[index];
			}
		}

                return 0;
        }else{

        	return 1;
	}
}

static long uintprofilex(aSubRecord *precord)
{
        uint32_t *image=precord->a;
        double *width=precord->b;
        double *height=precord->c;
        double *spec=precord->vala;
        int index,i,j;


        if( (*width * *height) > 0.0 ){

                precord->neva=*width;

                for(i=0;i<*width;i++){
                        spec[i]=0.0;
                }

                for(j=0;j<*height;j++){
                        for(i=0;i<*width;i++){
                                index=(j * *width)+i;
                                spec[i]+=(double)image[index];
                        }
                }

                return 0;
        }else{

                return 1;
        }
}

static long sumspec(aSubRecord *precord)
{
        double *spectrum=precord->a;
        double *reset=precord->b;
        double *size=precord->c;
	double *output=precord->vala;
	double *outB=precord->valb;

       if((*size>0) && (*size<=1100)){
		if(*reset==0){
			for(int i=0;i<*size;i++){
				output[i]=0.0;
			}
			*outB=0;
		}else{
			if(*outB==0){
				*outB=1;
				 return 1;
			}
			for(int i=0;i<*size;i++){
                                output[i]+=spectrum[i];
                        }
		}
		precord->neva=*size;
	}else{
		return 1;
	}
	return 0;
}

static long range(aSubRecord *precord)
{
  double *output=precord->vala;   // Pointer to output A
  double *A=precord->a;
  double *B=precord->b;

	double diff, N, MAX=2000;
	int i, sign;

	diff=*B-*A;
	N = abs(diff)+1;
	sign = 1;

	if((N>1) & (N<=MAX)){
		if(diff<0) sign=-1;
		for(i=0;i<N;i++){
			output[i]= *A+(i*sign);
		}
		precord->neva=N;
  }else{
  	return 1;
  }

	return 0;
}

static long threshold(aSubRecord *precord)
{
	double *output=precord->vala;   // Pointer to output A
	double *vec=precord->a;
	double noa=(double) precord->noa;
	double *thr=precord->b;
	double *size=precord->c;

	if((*size>0) & (*size<=noa)){
		for(int i=0;i<*size;i++){
			if(vec[i]>=*thr){
				output[i]=vec[i];
			}else{
				output[i]=0;
			}
		}
		precord->neva=*size;
	}else{
		return 1;
	}

	return 0;
}

static long sumroi(aSubRecord *precord)
{
        double *roiarray=precord->a;
        double *reset=precord->b;
        double *size=precord->c;
	double *width=precord->d;
	double *heigth=precord->e;
        double *output=precord->vala;
        double *outB=precord->valb;
	double *outX=precord->valc;
	double *outY=precord->vald;

       if((*size>0) && (*size<=262144)){
                if(*reset==0){
                        for(int i=0;i<*size;i++){
                                output[i]=0.0;
                        }
                        *outB=0;
                }else{
                        if(*outB==0){
                                *outB=1;
                                 return 1;
                        }
                        for(int i=0;i<*size;i++){
                                output[i]+=roiarray[i];
                        }
                }
                precord->neva=*size;
		*outX=*width;
		*outY=*heigth;
        }else{
        	return 1;
        }
        return 0;
}

static long execmd(aSubRecord *precord)
{
  char *instr=precord->a;
	char shellcmd[41]=":\0";
	//int stat;

	shellcmd[40]='\0';

	strcpy(shellcmd, instr);
	//printf("Shell cmd:\n%s\n", shellcmd);
	//stat=system(shellcmd);
	system(shellcmd);
	//printf("stat: %d\n", stat);
	//printf("OK\n");

	return 0;
}

/* Register these symbols for use by IOC code: */
//epicsExportAddress(int, <VAR>);
epicsRegisterFunction(linspace);
epicsRegisterFunction(calcspec);
epicsRegisterFunction(sumspec);
epicsRegisterFunction(range);
epicsRegisterFunction(threshold);
epicsRegisterFunction(sumroi);
epicsRegisterFunction(execmd);
epicsRegisterFunction(profilex);
epicsRegisterFunction(uintprofilex);

