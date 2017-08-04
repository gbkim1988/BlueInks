#define _WIN32_WINNT 0x0501 /* _WIN32_WINNT_WINXP */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <tchar.h>
#include <thread>
#include <chrono>         // std::chrono::system_clock
#include <ctime>          // std::time_t, std::tm, std::localtime, std::mktime
#include <iomanip>        // std::put_time
#include <iostream>       // std::cout
#include <libssh/libssh.h>

using namespace std::literals::chrono_literals;
#define SERVICE_NAME _T("SawTooth")

int _tmain(int argc, TCHAR *argv[])
{
	/// ������ ��� �ϱ⿡ ���� õ�������� �� �� ����
	using std::chrono::system_clock;
	std::time_t tt = system_clock::to_time_t(system_clock::now());

	struct std::tm * ptm = std::localtime(&tt);
	std::cout << "Current time: " << std::put_time(ptm, "%X") << '\n';

	std::cout << "Waiting for the next minute to begin...\n";
	ptm->tm_min; 
	ptm->tm_sec += 0; /// ���߿� �����Ҷ� ����
	std::this_thread::sleep_until(system_clock::from_time_t(mktime(ptm)));

	ssh_session my_ssh_session = ssh_new();
	
	return 0;
}