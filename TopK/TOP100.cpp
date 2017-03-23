#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
/*
* Take the max one hundred number from the one hundred million number(Limited memory):
* 1. Because the memeory limited, separated read 0.1 million data from the input file each time;
* 2. For the first part number, using sort function order them and push the top 100 number into candidate result;
* 3. For the follow n times 0.1 million number, compare them with the lowest number in the candidate group;
*    If it larger than the lowest num, insert into the candidate group, sort the group again to take the top onehundred.
* 4. After all , the candidate group is the result and output to outfile.
*
* Author:Jiajun Wen
* Data  :17th March 2017
*
* */

// 定义缓存限制，最大N个数，
const int BUFFERCAPACITY = 100000;
const int RESULT_LEN = 100;
const char* filename = "testdata.db";

// 设置降序模式
int cmp(const int &a, const int &b) {
	return a>b;
}


int main(int argc, char* argv[]) {
	vector<int> candidate;
	vector<int>::iterator it;

	ifstream filein;
	ofstream fileout;

	int index = 0;			//循环计数器

	int* pbuffer = (int*)malloc(BUFFERCAPACITY * sizeof(int));
	
	
	if (pbuffer == NULL) {
		cout << "Failed to allocat the memory!" << endl;
		return -1;
	}	

	filein.open(filename, ios::binary);
	fileout.open(string(string(filename).append(".sorted")).c_str(), ios::binary);
	if (!filein ) {
		cout << "Failed to open the source file" << endl;
		return -1;
	}
	if (!fileout) {
		cout << "Failed to open the output file!" << endl;
		return -1;
	}
	
	// 第一个BUFFERCAPACITY个数

	// **先排序第一组BUFFERCAPACITY个数，找出其中最大的100个，压入candidate
	filein.read((char*)pbuffer, BUFFERCAPACITY * sizeof(int));
	
	// 进行降序排序 
	sort(pbuffer, pbuffer+BUFFERCAPACITY+1,cmp);


	cout << pbuffer[3] << endl;

	for (; index < BUFFERCAPACITY; index++) {
		candidate.push_back(pbuffer[index]);
	}

	// 另外的思路： 直接默认最开始的100个位candidate向量，然后遍历其他的.从**处替换
	//filein.read((char*)pbuffer, RESULT_LEN * sizeof(int));
	//for (int index = 0; index < BUFFERCAPACITY; index++) {
	//	candidate.push_back(pbuffer[index]);
	//}
	//sort(candidate.begin(), candidate.end(), cmp);
	
	// 分COUNT次读取和遍历接下来的数据，然后和vector中最小的比较。如果大于则加入vector并将vector排序截取前100个
	while(!filein.eof()){
		filein.read((char*)pbuffer, BUFFERCAPACITY *sizeof (int));    
		for (index = 0; index < BUFFERCAPACITY; index++) {
			if (pbuffer[index] > candidate[RESULT_LEN-1]) {
				candidate.push_back(pbuffer[index]);
				sort(candidate.begin(), candidate.end(),cmp);
				candidate.erase(candidate.end());
			}
		}
	}


	// 另外的思路：当大于最小的元素时，直接遍历candidate向量，插入到对应的位置。
	//while (!filein.eof()) {
	//	filein.read((char*)pbuffer, BUFFERCAPACITY * sizeof(int));
	//	for (index = 0; index < BUFFERCAPACITY; index++) {
	//		if (pbuffer[index] > candidate[result_len - 1]) {
	//			for (int elenum=0;elenum < result_len; elenum++)
	//			{
	//				if (pbuffer[index] > candidate[elenum]) {
	//					candidate.insert(candidate.begin() + elenum, pbuffer[index]);
	//					candidate.erase(candidate.end());
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}

	
	// 输出最大的100个数。





	for (it = candidate.begin(); it != candidate.end(); it++) {
		fileout << *it << endl;
	}

	// 关闭文件和清空指针
	filein.close();
	fileout.close();
	pbuffer = NULL;

	return 0;
}

