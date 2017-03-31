#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

using namespace std;

bool File_split(const char* file_in) {
	int linenum = 50;
	int filenum = 10;
	FILE *sfp, *dfp;
	char StrLine[1024];
	string file_out;


	if ((sfp = fopen(file_in, "r+")) == NULL) 
	{
		cout << "file open failed!" << endl;
		return false;
	}

	cout << "Please input file num: "<< endl;
	cin >> filenum;
	cout << "Please input line num:" << endl;
	cin >> linenum;

	for (int i = 0; i < filenum; ++i)
	{
		file_out = "Splited_" + to_string(i+1) + ".txt";
		dfp = fopen(file_out.c_str(), "wt");
		for (int iline = 0; iline < linenum; ++iline)
		{
			if (feof(sfp))
			{
				fclose(sfp);
				fclose(dfp);
				return true;
			}
			fgets(StrLine, 1024, sfp);
			fputs(StrLine, dfp);
		}
		fclose(dfp);
	}
	return true;
}


 int main(int argc, char* argv[])
 {
	 if (File_split(argv[1])) {
		 cout << "OK" << endl;
	 }

 }