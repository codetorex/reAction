#ifndef NSERVER_H
#define NSERVER_H

#ifdef NETWORK_SYSTEM


#define DEFAULT_PORT        "5001"
#define MAX_BUFF_SIZE       8192
#define MAX_WORKER_THREAD   16

enum NIOOperation
{
	ClientIoAccept,
	ClientIoRead,
	ClientIoWrite
};


class NIO
{
public:
	NIO();

    WSAOVERLAPPED		Overlapped;
    char				Buffer[MAX_BUFF_SIZE];
    WSABUF				wsabuf;
    int					nTotalBytes;
    int					nSentBytes;
    NIOOperation		IOOperation;
    SOCKET				SocketAccept; 
    NIO					*nextio;
};


class NSocket
{
public:
	NSocket();

    SOCKET				Socket;
    LPFN_ACCEPTEX		fnAcceptEx;
    NIO					*pIOContext;  
    NSocket				*prevsocket; 
    NSocket				*nextsocket;

	void SendData(char* pdata,long datalen);
};

struct DummyWorkerStarter
{
	LPVOID WorkContext;
	LPVOID clasr;
};


// io completion based server class
class NServer
{
protected:
	// Constants
	char				*nport;	
	BOOL				endserver;
	BOOL				restart;
	BOOL				verbosemode;
	DWORD				threadcount;
	HANDLE				iocphandle;
	SOCKET				listensocket;
	HANDLE				threads[MAX_WORKER_THREAD];
	NSocket				*sockets;
	CRITICAL_SECTION	criticalsection;

public:
	
	NServer();
	
	void StartServer					(char* server_port);
	void RestartServer					();
	void StopServer						();
	
	virtual void Received				(NSocket* s,NIO* lpIOContext) {};
	virtual void Sent					(NSocket* s,NIO* lpIOContext) {};
	virtual void Accepted				(NSocket* s,NIO* lpIOContext) {};
	virtual void Disconnected			(NSocket* s,NIO* lpIOContext) {};
	
	DWORD WINAPI MainThread				();
	DWORD WINAPI WorkerThread			(LPVOID WorkContext);

private:
	// Functions
	void		CloseClient				(NSocket* lpPerSocketContext,bool bGraceful);
	NSocket*	CreateSocket			(SOCKET s, NIOOperation ClientIO);
	void		ClearSockets			();
	void		DeleteSocket			(NSocket* lpPerSocketContext);
	void		AddSocket				(NSocket* lpPerSocketContext);
	NSocket*	UpdateCompletionPort	(SOCKET s,NIOOperation ClientIo,bool bAddToList);
	bool		CreateAcceptSocket		(bool fUpdateIOCP);
	bool		CreateListenSocket		();
	
};

DWORD WINAPI MainThreadDummy(LPVOID clasr);
DWORD WINAPI WorkerThreadDummy (LPVOID WorkContext);

#endif

#endif