#include <octave/oct.h>
#include <stdlib.h>
/*#include <tsDefs.h>*/
#include <cadef.h>
#include <ezca.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>

using namespace std;

extern "C" 
{
    extern int ezcaGet(char *pvname, char ezcatype, int nelem, void *data_buff);
    extern int ezcaGetNelem(char *pvname, int *nelem);
    extern int ezcaNewMonitorValue(char *pvname, char ezcatype);
    extern int ezcaClearMonitor(char *pvname, char ezcatype);
    extern int ezcaSetMonitor(char *pvname, char ezcatype, long unsigned int nchar);
    extern int ezcaPut(char *pvname, char ezcatype,int nelem, void *data_buff);
    extern int ezcaSetRetryCount(int retries);
    extern int ezcaGetRetryCount(void);
    extern int ezcaSetTimeout(float timeout);
    extern float ezcaGetTimeout(void);
    extern int ezcaPvToChid(char *pvname, chid **cid);
    extern  short int ca_field_type ( chid );
    extern  long unsigned int ca_element_count ( chid );
    int Ca_types[7]= {1,2,4,6,0,3,5};

// ezca doesn't treat ENUMs, so fool it and use short, since this is what it resolves to.
#define ezcaEnum 6    

}


DEFUN_DLD(caput, args, ,
	  "caput('pvname',vals). Put values of vals[] to PV 'name'.")
{
     int b=0;
     char pv[255];
     int d,n,i,j,k;
     short int catype;
     int Nelem,Error;
     void *y;
     chid *ch;
     char *y1;
     char *y2;
     int *y3;
     long *y4;
     float *y5;
     double *y6;

// Copy data to vector
     if(args.length() >= 3)
     {
        cout << "Only PV name and one value array allowed" << endl;
	return octave_value(0);
	}
     RowVector x(args(1).vector_value());

// space for PV name
     charMatrix c(args(0).string_value());

// Copy name to C array     
     n=args(0).length();
     for (i = 0; i < n; i++) {
     	pv[i]= c(i);
	}
     pv[n]=(char)NULL;

// Get chid of channel
     j=ezcaPvToChid(pv, &ch);

// Use it to get field type     
     catype= Ca_types[ca_field_type(*ch)];

// Get number of elements from channel     
     ezcaGetNelem(pv, &Nelem);

// Allocate memory for C array, copy octave data to it and make the C call, 
// free the C array.

     Error=0;

     switch(catype){
     	case ezcaString:{
		if (!args(1).is_string()){
			Error=1;
			break;
			}
		n = args(1).length();
		charMatrix y(args(1).string_value());
  		y1=(char *) calloc(n,sizeof(char));
		for(i=0;i<n;i++){
			y1[i]=y(i);
			}
		y1[n]=(char)(NULL);
		d=ezcaPut(pv, ezcaString, 1, (void *)y1);
		free(y1);
		}
		break;
		
	case ezcaByte:
		n = args(1).length(); 
		// Check that data lengths match
		if(Nelem != n){
		Error=2;
		break;
		}

		y2=(char *) calloc(n,sizeof(char));
		for (i = 0; i < n; i++) {
			y2[i]= (char) x(i);
			}
		d=ezcaPut(pv,ezcaByte, n, y2);
		free(y2);
		break;
		  		  
	case ezcaShort:
		n = args(1).length(); 
		// Check that data lengths match
		if(Nelem != n){
		Error=2;
		break;
		}
		y3=(int *) calloc(n,sizeof(int));
		for (i = 0; i < n; i++) {
			y3[i]= (int) x(i);
			}
		d=ezcaPut(pv, ezcaShort, n, y3);
		free(y3);
		break;

	case ezcaEnum:
		n = args(1).length(); 
		// Check that data lengths match
		if(Nelem != n){
		Error=2;
		break;
		}
		y3=(int *) calloc(n,sizeof(int));
		for (i = 0; i < n; i++) {
			y3[i]= (int) x(i);
			}
		d=ezcaPut(pv, ezcaShort, n, y3);
		free(y3);
		break;
		
	case ezcaLong:
		n = args(1).length(); 
		// Check that data lengths match
		if(Nelem != n){
		Error=2;
		break;
		}
		y4=(long *) calloc(n,sizeof(long));
		for (i = 0; i < n; i++) {
			y4[i]=(long) x(i);
			}
		
		d=ezcaPut(pv, ezcaLong, n, y4);
		free(y4);
		break;
		
	case ezcaFloat:
		n = args(1).length(); 
		// Check that data lengths match
		if(Nelem != n){
		Error=2;
		break;
		}
		y5=(float *) calloc(n,sizeof(float));
		for (i = 0; i < n; i++) {
			y5[i]=(float) x(i);
			}
		
		d=ezcaPut(pv, ezcaFloat, n, y5);
		free(y5);
		break;
		
	case ezcaDouble:
		n = args(1).length(); 
		// Check that data lengths match
		if(Nelem != n){
		Error=2;
		break;
		}
		y6=(double *) calloc(n,sizeof(double));
		for (i = 0; i < n; i++) {
			y6[i]=(double) x(i);
			}
		d=ezcaPut(pv, ezcaDouble, n, y6);
		free(y6);
		break;
		
	default:
		Error=3;
		break;
	}
	switch(Error){
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default: {}
	}
     if (d != 0) error("caput: Channel Access fault");

     return octave_value(Error);
}


DEFUN_DLD(caget, args, ,
	  "caget('pvname'). Get values from  PV 'name'.")
{
     int b=0;
     char pv[255];
     int d,n,i,j,k;
     short int catype;
//     void *y;
     chid *ch;
     char *y1;
     char *y2;
     int *y3;
     long *y4;
     float *y5;
     double *y6;
     octave_value_list answer;


     charMatrix c(args(0).string_value());

// Get PV name into C array
     n=args(0).length();
     for (i = 0; i < n; i++) {
     	pv[i]= c(i);
	}
     pv[n]=(char)NULL;

// Get channel ID so can access structures
     j=ezcaPvToChid(pv, &ch);

// like PV variable type     
     catype= Ca_types[ca_field_type(*ch)];

// Get number of elements in PV     
     ezcaGetNelem(pv, &n);

// Make vector to hold results
     octave_value retval;

// Allocate memory for C array, get data from PV, copy it to Octave vector,
// free up C array.
     switch(catype){

	case ezcaString: {
		  y2=(char *) calloc(256,sizeof(char));
		  d=ezcaGet(pv, catype, n, (void *)y2);
		  n=strlen(y2);
		  string y;
		  for (i = 0; i < n; i++) {
		  	y += (char) y2[i];
			}
		retval=y;
		free(y2);
		}
		break;
		  		  
     	case ezcaByte:{
		  y1=(char *) calloc(n,sizeof(char));
		  d=ezcaGet(pv, catype, n, (void *)y1);
		  RowVector y(n);
		  for (i = 0; i < n; i++) {
		  	y(i)=(char)y1[i];
			}
		retval=y;
		free(y1);
		}
		break;
		
	case ezcaShort: {
		  y3=(int *) calloc(n,sizeof(int));
		  d=ezcaGet(pv, catype, n, (void *)y3);
		  RowVector y(n);
		  for (i = 0; i < n; i++) {
		  	y(i) = (double) y3[i];
			}
		retval=y;
		free(y3);
		}
		break;
		
	case ezcaEnum: {
		  y3=(int *) calloc(n,sizeof(int));
		  d=ezcaGet(pv, ezcaShort, n, (void *)y3);
		  RowVector y(n);
		  for (i = 0; i < n; i++) {
		  	y(i) = (double) y3[i];
			}
		retval=y;
		free(y3);
		}
		break;
		
	case ezcaLong: {
		  y4=(long *) calloc(n,sizeof(long));
		  d=ezcaGet(pv, catype, n, (void *)y4);
		  RowVector y(n);
		  for (i = 0; i < n; i++) {
		  	y(i) = (double) y4[i];
			}
		retval=y;
		free(y4);
		}
		break;
		
	case ezcaFloat: {
		  y5=(float *) calloc(n,sizeof(float));
		  d=ezcaGet(pv, catype, n, (void *)y5);
		  RowVector y(n);
		  for (i = 0; i < n; i++) {
		  	y(i) = (double) y5[i];
			}
		retval=y;
		free(y5);
		}
		break;
		
	case ezcaDouble: {
		y6=(double *) calloc(n,sizeof(double));
		d=ezcaGet(pv, catype, n, (void *)y6);
		RowVector y(n);
		for (i = 0; i < n; i++) {
		    y(i)=y6[i];
		    }
		retval=y;
		free(y6);
		}
		break;
	default:
		cout << "Unknown CA type" << endl;
		return(octave_value(-1));
		break;
	}
	if(d != 0) error("caget: Channel Access fault"); 
     return retval;
}

DEFUN_DLD(cagetnelem, args, ,
	  "cagetnelem('pvname'). Get number of values for PV 'name'.")
{
     char pv[255];
     int n,i,j;
     chid *ch;


     charMatrix c(args(0).string_value());

     n=args(0).length();
     for (i = 0; i < n; i++) {
     	pv[i]= c(i);
	}
     pv[n]=(char)NULL;
     ezcaGetNelem(pv, &n);
     return octave_value(n);
}

DEFUN_DLD(casetmonitor, args, ,
	  "casetmonitor('pvname'). Set a monitor on PV 'name'.")
{
     char pv[255];
     int n,i,j;
     long unsigned nchar;
     chid *ch;


     charMatrix c(args(0).string_value());

     n=args(0).length();
     for (i = 0; i < n; i++) {
     	pv[i]= c(i);
	}
     pv[n]=(char)NULL;
     j=ezcaPvToChid(pv, &ch);
     n = Ca_types[ca_field_type(*ch)];
     if(n==ezcaEnum) n=ezcaShort;
     ezcaSetMonitor(pv, (char)n, nchar);
     return octave_value(0);
}

DEFUN_DLD(caclrmonitor, args, ,
	  "caclrmonitor('pvname'). Clear a monitor on PV 'name'.")
{
     char pv[255];
     int n,i,j;
     chid *ch;


     charMatrix c(args(0).string_value());

     n=args(0).length();
     for (i = 0; i < n; i++) {
     	pv[i]= c(i);
	}
     pv[n]=(char)NULL;
     j=ezcaPvToChid(pv, &ch);
     n = Ca_types[ca_field_type(*ch)];
     if(n==ezcaEnum) n=ezcaShort;
     ezcaClearMonitor(pv,(char)n);
     return octave_value(0);
}

DEFUN_DLD(canewmonval, args, ,
	  "canewmonval('pvname'). check for a new value of PV 'name'.\n If no monitors are set, always returns 1, i.e. new data available.")
{
     char pv[255];
     int n,i,j;
     chid *ch;
     octave_value retval;

     charMatrix c(args(0).string_value());

     n=args(0).length();
     for (i = 0; i < n; i++) {
     	pv[i]= c(i);
	}
     pv[n]=(char)NULL;
     j=ezcaPvToChid(pv, &ch);
     n = Ca_types[ca_field_type(*ch)];
     if(n==ezcaEnum) n=ezcaShort;
     retval=ezcaNewMonitorValue(pv, (char)n);
     return retval;
}

DEFUN_DLD(cagetTimeout, args, ,
	  "cagetTimeout(). Get value of Ezca library timeout.")
{
float x;

     if(args.length() >= 1)
     {
        cout << "cagetTimeout() Function takes no arguments." << endl;
	return octave_value(0);
	}
     x=ezcaGetTimeout();
     return octave_value(x);
}

DEFUN_DLD(cagetRetryCount, args, ,
	  "cagetRetryCount(). Get value of Ezca library retry count.")
{
int x;

     if(args.length() >= 1)
     {
        cout << "cagetRetryCount() Function takes no arguments." << endl;
	return octave_value(0);
	}
     x=ezcaGetRetryCount();
     return octave_value(x);
}

DEFUN_DLD(casetTimeout, args, ,
	  "casetTimeout(n). Set value of Ezca library timeout.")
{
float x;

     if((args.length() >= 2) && (args(0).length() >1 ))
     {
        cout << "casetTimeout(t) Function takes exactly one scalar argument." << endl;
	return octave_value(0);
	}
     x=(float)(args(0).double_value());     
     ezcaSetTimeout(x);
     return octave_value(0);
}


DEFUN_DLD(casetRetryCount, args, ,
	  "casetRetryCount(n). Set value of Ezca library retry count.")
{
int x;

     if((args.length() >= 2) && (args(0).length() >1 ))
     {
        cout << "caputRetryCount() Function takes exactly one scalar argument." << endl;
	return octave_value(0);
	}
     x=(int)(args(0).int_value());     
     ezcaSetRetryCount(x);
     return octave_value(0);
}
