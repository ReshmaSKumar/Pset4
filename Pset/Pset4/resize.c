#include<stdio.h>
#include<stdlib.h>
#include "bmp.H"

int main(int argc,char *argv[])

{

	if(argc!=4)
	{
		fprintf(stderr,"Usage:./resize n infile outfile.\n");
		return 1;
	}

	char *n=argv[1];
	char *infile=argv[2];
	char *outfile=argv[3];

	int num=atoi(n);

	FILE *inptr=fopen(infile,"r");
	if(inptr==NULL)
	{
		fprintf(stderr,"Could not open %s.\n",infile);
		return 2;
	}

	FILE *outptr=fopen(outfile,"w");
	if(outptr==NULL)
	{
		fprintf(stderr,"Could not create %s.\n",outfile);
		return 3;
	}

	BITMAPFILEHEADER bf;
	fread(&bf,sizeof(BITMAPFILEHEADER),1,inptr);
	BITMAPINFOHEADER bi;
	fread(&bi,sizeof(BITMAPINFOHEADER),1,inptr);

	if(bf.bfType!=0x4d42||bf.bfOffBits!=54||bi.biSize!=40||bi.biBitCount!=40||bi.biCompression!=0)
	{
		fclose(outptr);
		fclose(inptr);
		fprintf(stderr,"Unsupported file format.\n");
		return 4;
	}
	

	int origwidth=bi.biWidth;
	int origpadding=(4-(bi.biWidth*sizeof(RGBTRIPLE))%4)%4;

	bi.biWidth*=num;
	bi.biHeight+=num;

	int padding=(4-(bi.biWidth*sizeof(RGBTRIPLE))%4)%4;

	bi.biSizeImage=(bi.biWidth*abs(bi.biHeight)*3)+(padding*(abs(bi.biHeight)));
	bf.bfSize=bi.biSizeImage+bf.bfOffBits;

	fwrite(&bf,sizeof(BITMAPFILEHEADER),1,outptr);
	fwrite(&bi,sizeof(BITMAPINFOHEADER),1,outptr);


}


