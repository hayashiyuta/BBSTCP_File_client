#include <iostream>
#include <string>

#include <cstdlib>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include<fstream>

#pragma comment( lib, "ws2_32.lib" )

// �|�[�g�ԍ�
const unsigned short SERVERPORT = 8888;

// ����M���郁�b�Z�[�W�̍ő�l
const unsigned int MESSAGELENGTH = 1024;

int sock;
bool Init();
bool Socket(int sockType);
bool Connect(std::string serverIpv4Address, unsigned short port);
bool Send(char buff[MESSAGELENGTH]);
bool Exit();


int main()
{
    // ������
    if (!Init())
    {
        std::cout << "Error: Init()" << std::endl;
    }
    std::cout << "Success: Init()" << std::endl;

    // �m���u���b�L���O�\�P�b�g�̍쐬
    if (!Socket(SOCK_STREAM))
    {
        std::cout << "Error:Socket()" << std::endl;
    }
    std::cout << "Success: Socket()" << std::endl;

    // �ڑ��v��
    if (!Connect("127.0.0.1", SERVERPORT))
    {
        std::cout << "Error:Connect()" << std::endl;
    }
    std::cout << "Success: Connect()" << std::endl;

    // ���M
    while (true)
    {
        char buff[MESSAGELENGTH];
        std::cout << "Input message:";
        std::cin >> buff;
        Send(buff);
    }

    if (!Exit())
    {
        std::cout << "Error: Exit()" << std::endl;
    }

    std::cout << "Success: Exit()" << std::endl;
    system("pause");

    return 0;
}

// ������
bool Init()
{
    WSADATA wsaData;
    // WinSock����Ȃ̂ŁASOCKET_ERROR�����Ďg�����Ⴄ
    return (WSAStartup(MAKEWORD(2, 2), &wsaData) != SOCKET_ERROR);
}

// �\�P�b�g�쐬
bool Socket(int sockType)
{
    int tempSock = socket(AF_INET, sockType, 0);
    if (tempSock < 0)
    {
        return false;
    }

    unsigned long cmdArg = 0x01;
    if (ioctlsocket(tempSock, FIONBIO, &cmdArg) == 0)
    {
        sock = tempSock;
        return true;
    }
    return false;
}

// �ڑ��v��
bool Connect(std::string serverIpv4Address, unsigned short port)
{
    // �ڑ���T�[�o�̃\�P�b�g�A�h���X���i�[
    SOCKADDR_IN toAddr;
    const int tolen = sizeof(toAddr);
    memset(&toAddr, 0, sizeof(toAddr));
    toAddr.sin_family = AF_INET;
    toAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverIpv4Address.c_str(), &toAddr.sin_addr.s_addr);

    return (connect(sock, (struct sockaddr*)&toAddr, sizeof(toAddr)) == 0);
}

// �����񑗐M
bool Send(char buff[MESSAGELENGTH])
{
    /*std::ifstream imageFile(buff,
                            std::ios::binary);//�f�[�^��ǂݍ���
    std::string imageData((std::istreamb_iterator<char>(imageFile)), std::istreambuf_iterator<char>());//�t�@�C������ǂݍ��񂾃f�[�^���i�[
    int ret = send(sock, imageData.c_str(), imageData.size(), 0);*/
    //int ret = send(sock, buff, MESSAGELENGTH, 0);//�������邾��
    std::ifstream text(buff);
    std::string   file((std::istreambuf_iterator<char>(text)), std::istreambuf_iterator<char>());
    int ret = send(sock, file.c_str(), file.size(), 0);
    return (ret == MESSAGELENGTH);

}

// �I������
bool Exit()
{
    if (shutdown(sock, 0x02) != 0)
        return false;

    if (closesocket(sock) != 0)
        return false;

    return (WSACleanup() != SOCKET_ERROR);
}