/*
�Զ����쳣�������	
*/

// #include <stdio.h>
// #include <tchar.h>
// #include <Windows.h>
// void myInvalidParameterHandler(const wchar_t* expression,
// 	const wchar_t* function,
// 	const wchar_t* file,
// 	unsigned int line,
// 	uintptr_t pReserved)
// {
// 	// function��file��line��Release����Ч
// 	wprintf(L"Invalid parameter detected in function %s."
// 		L" File: %s Line: %d\n", function, file, line);
// 	wprintf(L"Expression: %s\n", expression);
// 	// �����׳��쳣�������޷���λ����λ��
// 	throw 1;
// }
// void myPurecallHandler(void)
// {
// 	printf("In _purecall_handler.");
// 	// �����׳��쳣�������޷���λ����λ��
// 	throw 1;
// }
// int main()
// {
// 	char* formatString;
// 	_invalid_parameter_handler oldHandler;
// 	_purecall_handler old_pure_handle;
// 	oldHandler = _set_invalid_parameter_handler(myInvalidParameterHandler);
// 	old_pure_handle = _set_purecall_handler(myPurecallHandler);
// 	try
// 	{
// 		formatString = NULL;
// 		printf(formatString);  // �����쳣
// 	}
// 	catch (...)
// 	{
// 		// ��λ����λ�ã����log
// 		printf("catch");
// 	}
// 	getchar();
// }



/*
	�ַ���������
*/
// #include <iostream>
// #include <string>    
// int main()
// {
// 	std::string str("There are two needles in this haystack with needles.");
// 	std::string str2("needle");
// 
// 	// different member versions of find in the same order as above:
// 	std::size_t found = str.find(str2);
// 	if (found != std::string::npos)
// 		std::cout << "first 'needle' found at: " << found << '\n';
// 
// 	found = str.find("needles are small", found + 1, 6);
// 	if (found != std::string::npos)
// 		std::cout << "second 'needle' found at: " << found << '\n';
// 
// 	found = str.find("haystack");
// 	if (found != std::string::npos)
// 		std::cout << "'haystack' also found at: " << found << '\n';
// 
// 	found = str.find('.');
// 	if (found != std::string::npos)
// 		std::cout << "Period found at: " << found << '\n';
// 
// 	// let's replace the first needle:
// 	str.replace(str.find(str2), str2.length(), "preposition");
// 	std::cout << str << '\n';
// 
// 	return 0;
// }






/*
	argv[0]�뺯��������
*/
// #include <windows.h>
// #include <stdio.h>
// void main(int args, char* argv[])
// {
// 	char buf[1000];
// 	GetCurrentDirectory(1000, buf);
// 	printf("%s\n", buf);
// 	printf("%s", argv[0]);
// 	getchar();
// }



/*
CreateToolhelp32Snapshot������ʹ��ʵ��
*/
// #include <windows.h>
// #include <TlHelp32.h>
// #include <stdio.h>
// void main(int args,char* argv[])
// {
// 	HANDLE ghMutex;
// 	DWORD WINAPI WriteToDatabase(LPVOID);
// 	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
// 	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
// 	PROCESSENTRY32 pe32;
// 	MODULEENTRY32 me32;
// 	pe32.dwSize = sizeof(PROCESSENTRY32);
// 	me32.dwSize = sizeof(MODULEENTRY32);
// 	if (!Module32First(hModule, &me32))
// 	{
// 		printf("Module32First error!\n");
// 	}
// 	if (!Process32First(hProcess, &pe32))
// 	{
// 		printf("Process32First error!\n");
// 	}
// 	printf("************************Begin to print Moudle info***********\n");
// 	do
// 	{
// 		printf("\n\n   szModule:   %s", me32.szModule);
// 		printf("\n\n   hModule:   %s", me32.hModule);
// 		printf("\n\n   th32ProcessID:   %d", me32.th32ProcessID);
// 		printf("\n\n   th32ModuleID:   %d", me32.th32ModuleID);
// 		printf("\n\n   modBaseAddr:   %d", me32.modBaseAddr);
// 		printf("\n\n   modBaseSize:   %d", me32.modBaseSize);
// 		printf("\n\n   hModule:   %d", me32.GlblcntUsage);
// 	} while (Module32Next(hModule, &me32));
// 	// 	getchar();
// 	printf("************************Begin to print Process info***********\n");
// 	do
// 	{
// 		printf("\n   Process Name:   %s", pe32.szExeFile);
// 		printf("\n   Process ID:   %d", pe32.th32ProcessID);
// 
// 
// 
// 		printf("\n   cntUsage:   %d", pe32.cntUsage);
// 		printf("\n   the32ModuleID:   %d", pe32.th32ModuleID);
// 		printf("\n   Flag:   %d", pe32.dwFlags);
// 		printf("\n   pcPriClassBase:   %d", pe32.pcPriClassBase);
// 		printf("\n\n");
// 
// 	} while (Process32Next(hProcess, &pe32));
// 	getchar();
// }


/*
 �����л��������
*/
// #include <windows.h>
// #include <stdio.h>
// void main()
// {
// 	HDESK hDesk = CreateDesktop("NewDesktop", NULL, NULL, DF_ALLOWOTHERACCOUNTHOOK, GENERIC_ALL, NULL);
// 	HANDLE hProcess = 0;
// 	PROCESS_INFORMATION  pi;
// 	STARTUPINFO si = {sizeof(STARTUPINFO)};
// 	si.lpDesktop = "NewDesktop";
// 	CreateProcessA(NULL, "explorer.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
// 	SwitchDesktop(hDesk);
// }


/*
���߳�ʵ��
*/
// #include <stdio.h>
// #include <windows.h>
// DWORD WINAPI ThreadProc(LPVOID lpParameter);
// 
// int main(int argc, char* argv[])
// {
// 	int i = 0;
// 	DWORD test = 0;    ///< 0��ʾ��һ�����߳�
// 	HANDLE handle;
// 	DWORD numThreadId = 0;
// 
// 	for (i = 0; i < 6; ++i)
// 	{
// 		///< �̴߳�����Ϻ�����ִ�С��ɵ�5������������
// 		handle = CreateThread(NULL, 0, ThreadProc, (LPVOID)&test, 0, &numThreadId);
// 		if (handle)
// 		{
// 			Sleep(1000);        ///< Ϊ����testÿ���ǲ�ͬ��ֵ
// 			test = numThreadId;    ///< ��ʾ"��һ��"���߳��߳�ID
// 			printf("thread %lld success start...\n", numThreadId);
// 			CloseHandle(handle);
// 		}
// 	}
// 	Sleep(1000);
// 
// 	return 0;
// } 
// 
// DWORD WINAPI ThreadProc(LPVOID lpParameter)
// {  
// 	if (0 == *((DWORD *)lpParameter))
// 	{
// 		printf("currunt thread is run\n");
// 	}
// 	else
// 	{
// 		printf("forward threadID is %lld\n", *((DWORD *)lpParameter));
// 	}
// 
// 	return 0;
// }


/*
	�������WaitForSingleObject()����
*/
// #include <windows.h>
// #include <stdio.h>
// 
// #define THREADCOUNT 2
// #define  NUM_THREADS 5
// 
// HANDLE ghMutex;
// 
// DWORD WINAPI WriteToDatabase(LPVOID lpParam);
// void main()
// {
// 	HANDLE aThread[THREADCOUNT];
// 	DWORD ThreadID;
// 	int i;
// 	// Create a mutex with no initial owner
// 	ghMutex = CreateMutex(
// 		NULL, // default security attributes
// 		FALSE, // initially not owned
// 		NULL); // unnamed mutex
// 	if (ghMutex == NULL)
// 	{
// 		printf("CreateMutex error: %d\n", GetLastError());
// 		return;
// 	}
// 	// Create worker threads
// 	for (i = 0; i < THREADCOUNT; i++)
// 	{
// 		aThread[i] = CreateThread(
// 			NULL, // default security attributes
// 			0, // default stack size
// 			(LPTHREAD_START_ROUTINE)WriteToDatabase,
// 			NULL, // no thread function arguments
// 			0, // default creation flags
// 			&ThreadID); // receive thread identifier
// 		if (aThread[i] == NULL)
// 		{
// 			printf("CreateThread error: %d\n", GetLastError());
// 			return;
// 		}
// 	}
// 	// Wait for all threads to terminate
// 	WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);
// 	// Close thread and mutex handles
// 	for (i = 0; i < THREADCOUNT; i++)
// 		CloseHandle(aThread[i]);
// 	CloseHandle(ghMutex);
// 	getchar();
// }
// DWORD WINAPI WriteToDatabase(LPVOID lpParam)
// {
// 	DWORD dwCount = 0, dwWaitResult;
// 	// Request ownership of mutex.
// 	while (dwCount < 20)
// 	{
// 		dwWaitResult = WaitForSingleObject(
// 			ghMutex, // handle to mutex
// 			INFINITE); // no time-out interval
// 		switch (dwWaitResult)
// 		{
// 			// The thread got ownership of the mutex
// 		case WAIT_OBJECT_0:
// 			__try {
// 				// TODO: Write to the database
// 				printf("Thread %d writing to database...\n",
// 					GetCurrentThreadId());
// 				dwCount++;
// 			}
// 			__finally {
// 				// Release ownership of the mutex object
// 				if (!ReleaseMutex(ghMutex))
// 				{
// 					// Handle error.
// 				}
// 			}
// 			break;
// 			// The thread got ownership of an abandoned mutex
// 			// The database is in an indeterminate state
// 		case WAIT_ABANDONED:
// 			return FALSE;
// 		}
// 	}
// 	return TRUE;
// }

/*
	map���ݽṹ
*/
// #include <map>
// #include <iostream>
// 
// using namespace std;
// 
// int main()
// {
// 	map <int, int> m1, m2, m3;
// 	map <int, int>::iterator m1_Iter;
// 
// 	m1.insert(pair <int, int>(1, 10));
// 	m1.insert(pair <int, int>(2, 20));
// 	m1.insert(pair <int, int>(3, 30));
// 	m2.insert(pair <int, int>(10, 100));
// 	m2.insert(pair <int, int>(20, 200));
// 	m3.insert(pair <int, int>(30, 300));
// 
// 	cout << "The original map m1 is:";
// 	for (m1_Iter = m1.begin(); m1_Iter != m1.end(); m1_Iter++)
// 		cout << " " << m1_Iter->second;
// 	cout << "." << endl;
// 
// 	// This is the member function version of swap
// 	//m2 is said to be the argument map; m1 the target map
// 	m1.swap(m2);
// 
// 	cout << "After swapping with m2, map m1is:";
// 	for (m1_Iter = m1.begin(); m1_Iter != m1.end(); m1_Iter++)
// 		cout << " " << m1_Iter->second;
// 	cout << "." << endl;
// 	cout << "After swapping with m2, map m2 is:";
// 	for (m1_Iter = m2.begin(); m1_Iter != m2.end(); m1_Iter++)
// 		cout << " " << m1_Iter->second;
// 	cout << "." << endl;
// 	// This is the specialized template version of swap
// 	swap(m1, m3);
// 
// 	cout << "After swapping with m3, map m1is:";
// 	for (m1_Iter = m1.begin(); m1_Iter != m1.end(); m1_Iter++)
// 		cout << " " << m1_Iter->second;
// 	cout << "." << endl;
// 	getchar();
// }


/*
	�ܵ�ͨ��
*/
// #include <windows.h>
// #include <stdio.h>
// #include <stdlib.h>
// 
// int runcmd(char* lpCmd);
// 
// int main(int argc, char** argv)
// {
// 	char cmd[256];
// 	printf("����������:");
// 	gets_s(cmd,sizeof(cmd));
// 	runcmd(cmd);
// 	system("pause");
// 	return 0;
// }
// int runcmd(char* lpCmd)
// {
// 	char buf[2048] = { 0 };    //������
// 	DWORD len;
// 	HANDLE hRead, hWrite;    // �ܵ���д���
// 	STARTUPINFO si;
// 	PROCESS_INFORMATION pi;
// 	SECURITY_ATTRIBUTES sa;
// 
// 	//ZeroMemory( buf, 2047 );
// 	sa.nLength = sizeof(sa);
// 	sa.bInheritHandle = TRUE;    // �ܵ�����ǿɱ��̳е�
// 	sa.lpSecurityDescriptor = NULL;
// 
// 	// ���������ܵ����ܵ�����ǿɱ��̳е�
// 	if (!CreatePipe(&hRead, &hWrite, &sa, 2048))
// 	{
// 		printf("�ܵ�����ʧ��!(%#X)\n", (unsigned int)GetLastError());
// 		return 1;
// 	}
// 
// 	ZeroMemory(&si, sizeof(si));
// 	si.cb = sizeof(si);
// 	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
// 	si.wShowWindow = SW_HIDE;
// 	si.hStdError = hWrite;
// 	si.hStdOutput = hWrite;
// 
// 	// �����ӽ���,��������,�ӽ����ǿɼ̳е�
// 	if (!CreateProcess(NULL, lpCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
// 	{
// 		printf("��������ʧ��!(%#x)\n", (unsigned int)GetLastError());
// 		CloseHandle(hRead);
// 		CloseHandle(hWrite);
// 		return 1;
// 	}
// 	// д�˾���ѱ��̳�,������Ҫ�ر�,��Ȼ���ܵ�ʱ��������
// 	CloseHandle(hWrite);
// 	// ���ܵ�����,����ʾ
// 	while (ReadFile(hRead, buf, 2047, &len, NULL))
// 	{
// 		printf(buf);
// 		ZeroMemory(buf, 2047);
// 	}
// 	CloseHandle(hRead);
// 	return 0;
// }




/*
	CommandLineToArgvW�������������в���
*/
// #include <windows.h>
// #include <stdio.h>
// #include <shellapi.h>
// int __cdecl main()
// {
// 	LPWSTR *szArglist;
// 	int nArgs;
// 	int i;
// 	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
// 	if (NULL == szArglist)
// 	{
// 		wprintf(L"CommandLineToArgvW failed\n");
// 		getchar();
// 		return 0;
// 	}
// 	else
// 	{
// 		for (i = 0; i < nArgs; i++)
// 			printf("%d: %ws\n", i, szArglist[i]);
// 		LocalFree(szArglist);// Free memory allocated for CommandLineToArgvW arguments.
// 		getchar();
// 		return(1);
// 	}
// }


/*
std::invalid_argument ʹ�÷���
*/

// #include <iostream>
// #include <stdexcept>
// int main(int argc, char ** argv)
// {
// 	try
// 	{
// 		bool errorArgument;
// 		errorArgument = true;
// 		if (errorArgument)
// 		{
// 			throw std::invalid_argument("occur error!");
// 		}
// 	}
// 	catch (std::invalid_argument &ia)
// 	{
// 		//what()Ϊinvalid_argument�̳�exception��ĺ���
// 		std::cerr << " Invalid_argument " << ia.what() << std::endl;
// 	}
// 	
// 	return 0;
// }



/*
	��������fixed-point���͵Ĵ洢��ʽ
*/
// #include <stdio.h> 
// #include <assert.h> 
// #include <math.h> 
// typedef struct
// {
// 	unsigned short uSpare : 3; // Ϊ�˶��� 
// 
// 	unsigned short uSignificand : 12;
// 	short nSign : 1;
// }FIXED_BIT;
// typedef union
// {
// 	unsigned short uFixed;
// 	FIXED_BIT tFixedBit;
// }FIXED;
// typedef struct
// {
// 	unsigned int uSignificand : 23;
// 	int nExponent : 8;
// 	int nSign : 1;
// }FLOAT_BIT;
// typedef union
// {
// 	float fFloat;
// 	FLOAT_BIT tFloatBit;
// 	unsigned int uFloat;
// }FLOAT;
// unsigned short FloatToFixed(float fFloat); // ������λ��ʵ�� 
// float FixedToFloat(unsigned short uFixed); // ������λ��ʵ�� 
// unsigned short FloatToFixedEx(float fFloat); // ȫ������λʵ�� 
// float FixedToFloatEx(unsigned short uFixed); // ȫ������λʵ�� 
// unsigned short FloatToFixedSimple(float fFloat); // ���ʵ�� 
// float FixedToFloatSimple(unsigned short uFixed); // ���ʵ�� 
// 
// int main()
// {
// 	float x = (float)-0.175;
// 	unsigned short y;
// 
// 	printf("x = %f\n", x);
// 	y = FloatToFixed(x);
// 	printf("fixed = 0x%x\n", y);
// 	printf("float = %f\n", FixedToFloat(y));
// 
// 	y = FloatToFixedEx(x);
// 	printf("fixed = 0x%x\n", y);
// 	printf("float = %f\n", FixedToFloatEx(y));
// 
// 	y = FloatToFixedSimple(x);
// 	printf("fixed = 0x%x\n", y);
// 	printf("float = %f\n", FixedToFloatSimple(y));
// 
// 	getchar();
// 	return 0;
// }
// 
// unsigned short FloatToFixed(float fFloat)
// {
// 	FLOAT unFloat;
// 	FIXED unFixed;
// 	int nExponent;
// 
// 	unFloat.fFloat = fFloat;
// 
// 	nExponent = (int)unFloat.tFloatBit.nExponent - 0x7f;
// 
// 	// float��significand��ʾ1~2֮�������expoent=0x7f��ʾָ��0 
// 
// 
// 	// ��̫�����������ʲôҪ�� 
// 
// 
// 	// ָ���Ƿ���� 
// 
// 	assert((nExponent >= -12) && (nExponent <= 0));
// 
// 	unFixed.tFixedBit.uSpare = 0;
// 
// 	unFixed.tFixedBit.nSign = unFloat.tFloatBit.nSign;
// 
// 	unFixed.tFixedBit.uSignificand = ((int)unFloat.tFloatBit.uSignificand + 0x800000)
// 		>> (11 - nExponent);
// 
// 	return unFixed.uFixed;
// }
// 
// float FixedToFloat(unsigned short uFixed)
// {
// 	FLOAT unFloat;
// 	FIXED unFixed;
// 
// 	unsigned int uSignificand;
// 
// 	unFixed.uFixed = uFixed;
// 	uSignificand = unFixed.tFixedBit.uSignificand << 11;
// 
// 	unFloat.tFloatBit.nExponent = 0x7f;
// 
// 	while ((uSignificand & 0x800000) == 0)
// 	{
// 		uSignificand <<= 1;
// 		unFloat.tFloatBit.nExponent--;
// 	}
// 	uSignificand -= 0x800000;
// 
// 	unFloat.tFloatBit.uSignificand = uSignificand;
// 	unFloat.tFloatBit.nSign = unFixed.tFixedBit.nSign;
// 
// 	return unFloat.fFloat;
// }
// 
// unsigned short FloatToFixedEx(float fFloat)
// {
// 	FLOAT unFloat;
// 	unsigned int uFloat;
// 	int nSign;
// 	int nExponent;
// 	int nSignificand;
// 
// 	unFloat.fFloat = fFloat;
// 	uFloat = unFloat.uFloat;
// 
// 	nSign = (int)uFloat < 0;
// 	nExponent = ((uFloat & 0x7fffffff) >> 23) - 0x7f;
// 	nSignificand = (uFloat & 0x7fffff) + 0x800000;
// 
// 	// float��significand��ʾ1~2֮�������expoent=0x7f��ʾָ��0 
// 
// 
// 	// ��̫�����������ʲôҪ�� 
// 
// 
// 	// ָ���Ƿ���� 
// 
// 	assert((nExponent >= -12) && (nExponent <= 0));
// 
// 	nSignificand >>= (8 - nExponent);
// 	nSignificand &= 0xfffff8; // �����3λ 
// 
// 
// 	if (nSign == 0)
// 	{
// 		return (unsigned short)nSignificand;
// 	}
// 	else
// 	{
// 		return (unsigned short)nSignificand | 0x8000;
// 	}
// 	//return (unsigned short)( (nSign << 15) | nSignificand ); 
// 
// }
// 
// float FixedToFloatEx(unsigned short uFixed)
// {
// 	FLOAT unFloat;
// 	int nSign;
// 	int nExponent;
// 	int nSignificand;
// 
// 	nSign = (short)uFixed < 0;
// 	nExponent = 0x7f;
// 	nSignificand = (uFixed & 0x7fff) << 8;
// 
// 	while ((nSignificand & 0x800000) == 0)
// 	{
// 		nSignificand <<= 1;
// 		nExponent--;
// 	}
// 	nSignificand -= 0x800000;
// 
// 	unFloat.uFloat = (nSign << 31) | (nExponent << 23) | nSignificand;
// 
// 	return unFloat.fFloat;
// }
// 
// unsigned short FloatToFixedSimple(float fFloat)
// {
// 	int nSign = fFloat < 0.0;
// 	int nSignificand;
// 
// 	if (nSign == 0)
// 	{
// 		nSignificand = (int)(fFloat * 4096);
// 		assert(nSignificand < 4096);
// 		return (unsigned short)(nSignificand << 3);
// 	}
// 	else
// 	{
// 		nSignificand = (int)(-fFloat * 4096);
// 		assert(nSignificand < 4096);
// 		return (unsigned short)(nSignificand << 3) | 0x8000;
// 	}
// 
// }
// 
// float FixedToFloatSimple(unsigned short uFixed)
// {
// 	int nSign;
// 	int nSignificand;
// 
// 	nSign = (short)uFixed < 0;
// 	nSignificand = ((int)uFixed & 0x7fff) >> 3;
// 
// 	if (nSign == 0)
// 	{
// 		return (float)(nSignificand / 4096.0);
// 	}
// 	else
// 	{
// 		return -(float)(nSignificand / 4096.0);
// 	}
// }

#include "stdio.h"
int main()
{
	printf("Hello World");
	getchar();
}