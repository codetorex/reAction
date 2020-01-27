
#include "StdAfx.h"
#include "nserver.h"

#ifdef NETWORK_SYSTEM


int myprintf(const char *lpFormat, ...);

void DoEvents() 
{ 
    MSG Msg; 
    while(PeekMessage(&Msg,0,0,0,PM_REMOVE))
	{
        if (Msg.message == WM_QUIT)break;
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    } 
}

NServer::NServer()
{
	nport = DEFAULT_PORT;
	endserver = FALSE;			// set to TRUE on CTRL-C
	restart = TRUE;				// set to TRUE to CTRL-BRK
	verbosemode = FALSE;
	threadcount = 0;		//worker thread count
	iocphandle = INVALID_HANDLE_VALUE;
	listensocket = INVALID_SOCKET;
	sockets = NULL;	
}

NIO::NIO()
{
	Overlapped.Internal = 0;
	Overlapped.InternalHigh = 0;
	Overlapped.Offset = 0;
	Overlapped.OffsetHigh = 0;
	Overlapped.hEvent = 0;
	nextio = 0;
	nTotalBytes = 0;
	nSentBytes  = 0;
}

NSocket::NSocket()
{
	prevsocket = 0;
	nextsocket = 0;
}

void NSocket::SendData(char* pdata,long datalen)
{
	pIOContext->IOOperation = ClientIoWrite;
	pIOContext->nTotalBytes = datalen;
	pIOContext->nSentBytes = 0;
	pIOContext->wsabuf.len = datalen;
	pIOContext->wsabuf.buf = pdata;

//	nRet = WSASend (Socket, &buffSend, 1, &dwSendNumBytes, dwFlags, &(pIOContext->Overlapped), NULL);

}

void NServer::StartServer(char* server_port)
{
	HANDLE hThread = INVALID_HANDLE_VALUE;
	DWORD dwThreadId = 0;
	nport = server_port;
	hThread = CreateThread(NULL, 0, MainThreadDummy, (NServer*)this , 0, &dwThreadId);

}

DWORD WINAPI MainThreadDummy(LPVOID clasr)
{
	NServer* dum;
	dum = (NServer*)clasr;
	dum->MainThread();
	return 0;
}

DWORD WINAPI WorkerThreadDummy(LPVOID WorkContext)
{
	DummyWorkerStarter* dws = (DummyWorkerStarter*)WorkContext;
	NServer* dum;
	dum = (NServer*)dws->clasr;
	dum->WorkerThread(dws->WorkContext);	
	return 0;
}

DWORD WINAPI NServer::MainThread () 
{
	SYSTEM_INFO systemInfo;
	WSADATA wsaData;
	SOCKET sdAccept = INVALID_SOCKET;
	NSocket* lpPerSocketContext = NULL;
	DWORD dwRecvNumBytes = 0;     
	DWORD dwFlags = 0;            
	int nRet = 0;

	for( int i = 0; i < MAX_WORKER_THREAD; i++ ) 
	{
		threads[i] = INVALID_HANDLE_VALUE;
	}

	GetSystemInfo(&systemInfo);
	threadcount = systemInfo.dwNumberOfProcessors * 2;

	if( (nRet = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0 ) 
	{
		myprintf("WSAStartup() failed: %d\n",nRet);
		return 0;
	}

	InitializeCriticalSection(&criticalsection);

	while( restart ) 
	{
		restart = false;
		endserver = false;

		__try 
		{
			iocphandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
			if( iocphandle == NULL ) 
			{
				myprintf("CreateIoCompletionPort() failed to create I/O completion port: %d\n", 
						GetLastError());
				__leave;
			}

			for( DWORD dwCPU = 0; dwCPU < threadcount; dwCPU++ ) 
			{

				//
				// Create worker threads to service the overlapped I/O requests.  The decision
				// to create 2 worker threads per CPU in the system is a heuristic.  Also,
				// note that thread handles are closed right away, because we will not need them
				// and the worker threads will continue to execute.
				//
				HANDLE hThread = INVALID_HANDLE_VALUE;
				DWORD dwThreadId = 0;
				DummyWorkerStarter dws;
				dws.clasr = (NServer*)this;
				dws.WorkContext = iocphandle;

				hThread = CreateThread(NULL, 0, WorkerThreadDummy, &dws, 0, &dwThreadId);
				if( hThread == NULL ) 
				{
					myprintf("CreateThread() failed to create worker thread: %d\n", 
							GetLastError());
					__leave;
				}
				threads[dwCPU] = hThread;
				hThread = INVALID_HANDLE_VALUE;
			}

			if( !CreateListenSocket() )
				__leave;

			while( TRUE ) 
			{

				//
				// Loop forever accepting connections from clients until console shuts down.
				//
				sdAccept = WSAAccept(listensocket, NULL, NULL, NULL, 0);
				if( sdAccept == SOCKET_ERROR ) 
				{

					//
					// If user hits Ctrl+C or Ctrl+Brk or console window is closed, the control
					// handler will close the listensocket socket. The above WSAAccept call will 
					// fail and we thus break out the loop,
					//
					myprintf("WSAAccept() failed: %d\n", WSAGetLastError());
					__leave;
				}

                //
				// we add the just returned socket descriptor to the IOCP along with its
				// associated key data.  Also the global list of context structures
				// (the key data) gets added to a global list.
				//
				lpPerSocketContext = UpdateCompletionPort(sdAccept, ClientIoRead, TRUE);
				if( lpPerSocketContext == NULL )
					__leave;

				//
				// if a CTRL-C was pressed "after" WSAAccept returns, the CTRL-C handler
				// will have set this flag and we can break out of the loop here before
				// we go ahead and post another read (but after we have added it to the 
				// list of sockets to close).
				//
				if( endserver )
					break;

                //
				// post initial receive on this socket
				//

			/*WSABUF tst;
			tst.buf = welcome;
			tst.len = 3;
			WSASend(sdAccept, &tst, 1, &dwRecvNumBytes, dwFlags, &(lpPerSocketContext->pIOContext->Overlapped), NULL);*/
				
				Accepted(lpPerSocketContext,lpPerSocketContext->pIOContext);


				nRet = WSARecv(sdAccept, &(lpPerSocketContext->pIOContext->wsabuf), 
							   1, &dwRecvNumBytes, &dwFlags,
							   &(lpPerSocketContext->pIOContext->Overlapped), NULL);
				if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
				{
					myprintf("WSARecv() Failed: %d\n", WSAGetLastError());
					CloseClient(lpPerSocketContext, FALSE);
				}
			} //while
		}

		__finally   
		{

			endserver = TRUE;

            //
			// Cause worker threads to exit
			//
			if( iocphandle ) 
			{
				for( DWORD i = 0; i < threadcount; i++ )
					PostQueuedCompletionStatus(iocphandle, 0, 0, NULL);
			}
            
			//
			//Make sure worker threads exits.
            //
			if( WAIT_OBJECT_0 != WaitForMultipleObjects( threadcount,  threads, TRUE, 1000) )
			{
				myprintf("WaitForMultipleObjects() failed: %d\n", GetLastError());
			}
			else
			{
				for( DWORD i = 0; i < threadcount; i++ ) 
				{
					if( threads[i] != INVALID_HANDLE_VALUE ) CloseHandle(threads[i]);
					threads[i] = INVALID_HANDLE_VALUE;
				}
			}

			ClearSockets();

			if( iocphandle ) 
			{
				CloseHandle(iocphandle);
				iocphandle = NULL;
			}

			if( listensocket != INVALID_SOCKET ) 
			{
				closesocket(listensocket); 
				listensocket = INVALID_SOCKET;
			}

			if( sdAccept != INVALID_SOCKET ) 
			{
				closesocket(sdAccept); 
				sdAccept = INVALID_SOCKET;
			}

		} //finally

		if( restart ) 
		{
			myprintf("\niocpserver is restarting...\n");
		} 
		else
		{
			myprintf("\niocpserver is exiting...\n");
		}

	} //while (g_bRestart)

	DeleteCriticalSection(&criticalsection);
	WSACleanup();
	return(0);
}      

void NServer::RestartServer()
{
	restart = TRUE;
}

void NServer::StopServer()
{
	SOCKET sockTemp = INVALID_SOCKET;	
	sockTemp = listensocket;
	listensocket = INVALID_SOCKET;
	endserver = TRUE;
	closesocket(sockTemp);
	sockTemp = INVALID_SOCKET;
}


bool NServer::CreateListenSocket(void)
{

	int nRet = 0;
	int nZero = 0;
	struct addrinfo hints;
	struct addrinfo *addrlocal = NULL;

	//
	// Resolve the interface
	//
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags  = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_IP;

	if( getaddrinfo(NULL, nport, &hints, &addrlocal) != 0 ) 
	{
		myprintf("getaddrinfo() failed with error %d\n", WSAGetLastError());
        return(FALSE);
	}

	if( addrlocal == NULL ) 
	{
		myprintf("getaddrinfo() failed to resolve/convert the interface\n");
        return(FALSE);
	}

	listensocket = WSASocket(addrlocal->ai_family, addrlocal->ai_socktype, addrlocal->ai_protocol, 
						   NULL, 0, WSA_FLAG_OVERLAPPED); 
	if( listensocket == INVALID_SOCKET ) 
	{
		myprintf("WSASocket(listensocket) failed: %d\n", WSAGetLastError());
		return(FALSE);
	}

	nRet = bind(listensocket, addrlocal->ai_addr, addrlocal->ai_addrlen);
	if( nRet == SOCKET_ERROR )
	{
		myprintf("bind() failed: %d\n", WSAGetLastError());
		return(FALSE);
	}

	nRet = listen(listensocket, 5);
	if( nRet == SOCKET_ERROR ) 
	{
		myprintf("listen() failed: %d\n", WSAGetLastError());
		return(FALSE);
	}

	nZero = 0;
	nRet = setsockopt(listensocket, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));
	if( nRet == SOCKET_ERROR ) 
	{
		myprintf("setsockopt(SNDBUF) failed: %d\n", WSAGetLastError());
		return(FALSE);
	}

	freeaddrinfo(addrlocal);

	return true;
}

//
// Worker thread that handles all I/O requests on any socket handle added to the IOCP.
//
DWORD WINAPI NServer::WorkerThread (LPVOID WorkThreadContext) {

	HANDLE hIOCP = (HANDLE)WorkThreadContext;
	BOOL bSuccess = FALSE;
	int nRet = 0;
	LPWSAOVERLAPPED lpOverlapped = NULL;
	NSocket* lpPerSocketContext = NULL;
	NIO* lpIOContext = NULL; 
	WSABUF buffRecv;
	WSABUF buffSend;
	DWORD dwRecvNumBytes = 0;
	DWORD dwSendNumBytes = 0;
	DWORD dwFlags = 0;
	DWORD dwIoSize = 0;

	while( true ) 
	{

		bSuccess = GetQueuedCompletionStatus(hIOCP, &dwIoSize,(LPDWORD)&lpPerSocketContext,(LPOVERLAPPED*)&lpOverlapped, INFINITE);
		
		if( !bSuccess )
			myprintf("GetQueuedCompletionStatus() failed: %d\n", GetLastError());

		if( lpPerSocketContext == NULL ) 
		{
			return(0);
		}

		if( endserver ) 
		{
			return(0);
		}

		if( !bSuccess || (bSuccess && (dwIoSize == 0)) ) 
		{

			//
			// client connection dropped, continue to service remaining (and possibly 
			// new) client connections
			//
			CloseClient(lpPerSocketContext, FALSE); 
			continue;
		}

        //
		// determine what type of IO packet has completed by checking the NIO 
		// associated with this socket.  This will determine what action to take.
		//
		lpIOContext = (NIO*)lpOverlapped;
		switch( lpIOContext->IOOperation ) 
		{
		case ClientIoRead:
			Received(lpPerSocketContext,lpIOContext);
			break;

		case ClientIoWrite:

			//
			// a write operation has completed, determine if all the data intended to be
			// sent actually was sent.
			//
			lpIOContext->IOOperation = ClientIoWrite;
			lpIOContext->nSentBytes  += dwIoSize;
			dwFlags = 0;
			if( lpIOContext->nSentBytes < lpIOContext->nTotalBytes ) 
			{
				buffSend.buf = lpIOContext->Buffer + lpIOContext->nSentBytes;
				buffSend.len = lpIOContext->nTotalBytes - lpIOContext->nSentBytes;
				nRet = WSASend (lpPerSocketContext->Socket, &buffSend, 1, 
								&dwSendNumBytes, dwFlags, &(lpIOContext->Overlapped), NULL);
				if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
				{
					myprintf("WSASend() failed: %d\n", WSAGetLastError());
					CloseClient(lpPerSocketContext, FALSE);
				}
				else if( verbosemode ) 
				{
					myprintf("WorkerThread %d: Socket(%d) Send partially completed (%d bytes), Recv posted\n", 
						   GetCurrentThreadId(), lpPerSocketContext->Socket, dwIoSize);
				}
			}
			else 
			{
				//
				// previous write operation completed for this socket, post another recv
				//
				lpIOContext->IOOperation = ClientIoRead; 
				dwRecvNumBytes = 0;
				dwFlags = 0;
				buffRecv.buf = lpIOContext->Buffer;
				buffRecv.len = MAX_BUFF_SIZE;
				nRet = WSARecv(lpPerSocketContext->Socket, &buffRecv, 1, 
							   &dwRecvNumBytes, &dwFlags, &lpIOContext->Overlapped, NULL);
				if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) 
				{
					myprintf("WSARecv() failed: %d\n", WSAGetLastError());
					CloseClient(lpPerSocketContext, FALSE);
				} 
				else if( verbosemode ) 
				{
					myprintf("WorkerThread %d: Socket(%d) Send completed (%d bytes), Recv posted\n", 
						   GetCurrentThreadId(), lpPerSocketContext->Socket, dwIoSize);
				}
				Sent(lpPerSocketContext,lpIOContext);
			}
			break;

		} //switch
	} //while
	return(0);
} 

//
//  Allocate a context structures for the socket and add the socket to the IOCP.  
//  Additionally, add the context structure to the global list of context structures.
//
NSocket* NServer::UpdateCompletionPort(SOCKET sd, NIOOperation ClientIo,bool bAddToList) 
{
	NSocket* lpPerSocketContext;

	lpPerSocketContext = CreateSocket(sd, ClientIo);
	if( lpPerSocketContext == NULL )
		return(NULL);

	iocphandle = CreateIoCompletionPort((HANDLE)sd, iocphandle, (DWORD_PTR)lpPerSocketContext, 0);
	if( iocphandle == NULL ) 
	{
		myprintf("CreateIoCompletionPort() failed: %d\n", GetLastError());
		if( lpPerSocketContext->pIOContext )
			delete [] lpPerSocketContext->pIOContext;
		delete [] lpPerSocketContext;
		return(NULL);
	}

    //
	//The listening socket context (bAddToList is FALSE) is not added to the list.
	//All other socket contexts are added to the list.
	//
	if( bAddToList ) AddSocket(lpPerSocketContext);

	if( verbosemode )
		myprintf("UpdateCompletionPort: Socket(%d) added to IOCP\n", lpPerSocketContext->Socket);

	return(lpPerSocketContext);
}

//
//  Close down a connection with a client.  This involves closing the socket (when 
//  initiated as a result of a CTRL-C the socket closure is not graceful).  Additionally, 
//  any context data associated with that socket is free'd.
//
VOID NServer::CloseClient (NSocket* lpPerSocketContext,bool bGraceful) 
{

	EnterCriticalSection(&criticalsection);

	if( lpPerSocketContext ) 
	{
		if( verbosemode )
			myprintf("CloseClient: Socket(%d) connection closing (graceful=%s)\n",
				   lpPerSocketContext->Socket, (bGraceful?"TRUE":"FALSE"));
		if( !bGraceful ) 
		{

			//
			// force the subsequent closesocket to be abortative.
			//
			LINGER  lingerStruct;

			lingerStruct.l_onoff = 1;
			lingerStruct.l_linger = 0;
			setsockopt(lpPerSocketContext->Socket, SOL_SOCKET, SO_LINGER,(char *)&lingerStruct, sizeof(lingerStruct) );
		}
		closesocket(lpPerSocketContext->Socket);
		lpPerSocketContext->Socket = INVALID_SOCKET;
		DeleteSocket(lpPerSocketContext);
		lpPerSocketContext = NULL;
	} 
	else 
	{
		myprintf("CloseClient: lpPerSocketContext is NULL\n");
	}

	LeaveCriticalSection(&criticalsection);
	return;    
} 

//
// Allocate a socket context for the new connection.  
//
NSocket* NServer::CreateSocket(SOCKET sd, NIOOperation ClientIO) {

	NSocket* lpPerSocketContext;

	EnterCriticalSection(&criticalsection);

	lpPerSocketContext = new NSocket; //(NSocket*)xmalloc(sizeof(NSocket));
	if( lpPerSocketContext ) {
		lpPerSocketContext->pIOContext = new NIO;
		if( lpPerSocketContext->pIOContext ) 
		{
			lpPerSocketContext->Socket = sd;
			lpPerSocketContext->pIOContext->IOOperation  = ClientIO;
			lpPerSocketContext->pIOContext->wsabuf.buf  = lpPerSocketContext->pIOContext->Buffer;
			lpPerSocketContext->pIOContext->wsabuf.len  = sizeof(lpPerSocketContext->pIOContext->Buffer);

			ZeroMemory(lpPerSocketContext->pIOContext->wsabuf.buf, lpPerSocketContext->pIOContext->wsabuf.len);
		} 
		else 
		{
			delete [] lpPerSocketContext;
			myprintf("HeapAlloc() NIO failed: %d\n", GetLastError());
		}

	} 
	else 
	{
		myprintf("HeapAlloc() NSocket failed: %d\n", GetLastError());
	}

	LeaveCriticalSection(&criticalsection);

	return(lpPerSocketContext);
} 

//
//  Add a client connection context structure to the global list of context structures.
//
VOID NServer::AddSocket (NSocket* lpPerSocketContext) {

	NSocket*     pTemp;

	EnterCriticalSection(&criticalsection);

	if( sockets == NULL ) 
	{

		//
		// add the first node to the linked list
		//
		lpPerSocketContext->prevsocket    = NULL;
		lpPerSocketContext->nextsocket = NULL;
		sockets = lpPerSocketContext;
	} 
	else 
	{

		//
		// add node to head of list
		//
		pTemp = sockets;

		sockets = lpPerSocketContext;
		lpPerSocketContext->prevsocket    = pTemp;
		lpPerSocketContext->nextsocket = NULL; 

		pTemp->nextsocket = lpPerSocketContext;
	}

	LeaveCriticalSection(&criticalsection);
	return;
}

//
//  Remove a client context structure from the global list of context structures.
//
VOID NServer::DeleteSocket(NSocket* lpPerSocketContext) {

	NSocket* pBack;
	NSocket* pForward;
	NIO*     pNextIO     = NULL;
	NIO*     pTempIO     = NULL;

	EnterCriticalSection(&criticalsection);

	if( lpPerSocketContext ) {
		pBack       = lpPerSocketContext->prevsocket;
		pForward    = lpPerSocketContext->nextsocket;


		if( ( pBack == NULL ) && ( pForward == NULL ) ) 
		{

			//
			// This is the only node in the list to delete
			//
			sockets = NULL;
		} 
		else if ( ( pBack == NULL ) && ( pForward != NULL ) ) 
		{

			//
			// This is the start node in the list to delete
			//
			pForward->prevsocket = NULL;
			sockets = pForward;
		} 
		else if ( ( pBack != NULL ) && ( pForward == NULL ) ) 
		{

			//
			// This is the end node in the list to delete
			//
			pBack->nextsocket = NULL;
		} 
		else if( pBack && pForward ) 
		{

			//
			// Neither start node nor end node in the list
			//
			pBack->nextsocket = pForward;
			pForward->prevsocket = pBack;
		}

		//
		// Free all i/o context structures per socket
		//
		pTempIO = (NIO*)(lpPerSocketContext->pIOContext);
		do {
			pNextIO = (NIO*)(pTempIO->nextio);
			if( pTempIO ) {

				//
				//The overlapped structure is safe to free when only the posted i/o has
				//completed. Here we only need to test those posted but not yet received 
				//by PQCS in the shutdown process.
				//
				if( endserver )
					while( !HasOverlappedIoCompleted((LPOVERLAPPED)pTempIO) ) Sleep(0);
				delete [] pTempIO;
				pTempIO = NULL;
			}
			pTempIO = pNextIO;
		} while( pNextIO );

		delete lpPerSocketContext;
		lpPerSocketContext = NULL;

	} else {
		myprintf("CtxtListDeleteFrom: lpPerSocketContext is NULL\n");
	}

	LeaveCriticalSection(&criticalsection);
	return;
}

//
//  Free all context structure in the global list of context structures.
//
VOID NServer::ClearSockets() 
{

	NSocket*     pTemp1,* pTemp2;

	EnterCriticalSection(&criticalsection);

	pTemp1 = sockets; 
	while( pTemp1 ) 
	{
		pTemp2 = pTemp1->prevsocket;
		CloseClient(pTemp1, false);
		pTemp1 = pTemp2;
	}

	LeaveCriticalSection(&criticalsection);
	return;
}

//
// Our own printf. This is done because calling printf from multiple
// threads can AV. The standard out for WriteConsole is buffered...
//
int myprintf (const char *lpFormat, ... ) 
{
	int nLen = 0;
	int nRet = 0;
	char cBuffer[512] ;
	va_list arglist ;
	HANDLE hOut = NULL;

	ZeroMemory(cBuffer, sizeof(cBuffer));

	va_start(arglist, lpFormat);

	nLen = lstrlen( lpFormat ) ;
	nRet = wvsprintf( cBuffer, lpFormat, arglist );

	if( nRet >= nLen || GetLastError() == 0 ) {
		hOut = GetStdHandle(STD_OUTPUT_HANDLE) ;
		//if( hOut != INVALID_HANDLE_VALUE )
		//	WriteConsole( hOut, cBuffer, lstrlen(cBuffer), (LPDWORD)&nLen, NULL ) ;
	}

	return nLen ;
}

#endif
