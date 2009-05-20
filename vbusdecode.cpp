// sample  cat ../raw.log | ./a.out  0,15,0.1 2,15,0.1 4,15,0.1 6,15,0.1 8,7,1 9,7,1 20,16,1 22,16,1000 24,16,1000000
// will give temp of s1,s2,s3,s4 pumpspeed pump1,pump2 and watts, Kw, Mw for a resol deltasol bs plus
#include <stdio.h>
#include <iostream>
using namespace std;

int model;
unsigned char frame[4];
unsigned char allframes[1026];
string presults;

void giveresults(char parray[])
{
	float f;
	char results[24];
	int offset = atoi(strtok (parray,","));
	int length = atoi(strtok (NULL, ","));
	float multiplier =  atof(strtok (NULL, ","));
	if ( (length -1) /8) {
		f = (allframes[offset] + (allframes[offset+1]*0x100)) * multiplier;
	} else {
		f = (allframes[offset] ) * multiplier;
	}

	sprintf(results, "%#.1f ",f);
	presults.append(results);
}

int decodeheader()
{
	char  buffer[10];
	unsigned char a;
	unsigned char b;
	if (scanf("%c%c%c%c%c%c%c%c%c",&buffer[0],&buffer[1],&buffer[2],&buffer[3],&buffer[4],&buffer[5],&buffer[6],&buffer[7],&buffer[8],&buffer[9]) != 1)

	{
		a = buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6] + buffer[7];
		b = ~ a;
		a = buffer[8];
	}
	if (  a == b )
	{
		model = buffer[3] * 0x100  + buffer[2];
		a = buffer[7];
		return a;
	}
	else
	{
		return 0;
	}
}

unsigned char decodeframe(int x)
{
	char  buffer[7] ;
	unsigned char a;
	unsigned char b;
	if (scanf("%c%c%c%c%c%c",&buffer[0],&buffer[1],&buffer[2],&buffer[3],&buffer[4],&buffer[5]) != 1)
	{

		a = buffer[0]  + buffer[1]  + buffer[2] + buffer[3] + buffer[4] ;
		b = 0;
		b =  (( ~a | 0x80) - 0x80 ) ;
		a = buffer[5];
		if (  a == b )
		{
			frame[0] =  buffer[0] + (( buffer[4] & 0x01 ) * 0x80 ) ;
			frame[1] =  buffer[1] + (( buffer[4] >> 1 & 0x01 ) * 0x80 ) ;
			frame[2] =  buffer[2] + (( buffer[4] >> 2 & 0x01 ) * 0x80 )  ;
			frame[3] =  buffer[3] + (( buffer[4] >> 3 & 0x01 ) * 0x80 ) ; 
		}
		else
		{
			frame[0] = 0;
			frame[1] = 0;
			frame[2] = 0;
			frame[3] = 0;
		}

	}

}

int main(int argc, char* argv[])
{
	char  buffer[2];
	unsigned char a;	
	char* arg_dup;
	int framecount;
	FILE * pFile;

	while ( fgets(buffer, 2, stdin) != NULL)
	{
		a = buffer[0];
		if ( a ==  0xAA) {
			framecount  =    decodeheader();
			if (framecount !=0 ) 
			{
				for ( int x = 0; x < framecount ; x++ ) 
				{
					decodeframe(x);
					allframes[4*x] =frame[0];
					allframes[(4*x)+1] =frame[1];
					allframes[(4*x)+2] =frame[2];
					allframes[(4*x)+3] =frame[3];
				}
				string filen = "stdout";
				for(int i = 1; i < argc; i++)
				{
					string sw = argv[i];
					if (sw == "-f") {
						i++;
						filen = argv[i]; 
					} 
					else
					{
						arg_dup = strdup(argv[i]);
						giveresults(arg_dup);
						free(arg_dup);
					}
				}
				if (filen == "stdout")
				{
					 fprintf(stdout,"%s\n",presults.c_str());
				} else {
					pFile = fopen (filen.c_str(),"w");
					fprintf (pFile,"%s\n",presults.c_str());
					fclose (pFile);
				}
				presults = "";
			}
		}
	}
	
	return 0; 
}

