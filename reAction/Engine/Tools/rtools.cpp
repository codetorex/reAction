#include "StdAfx.h"
#include "rtools.h"


int PowerOfTwo(dword num)
{
	int r =0 ;
	while ( num != 1)
	{
		num >>=1; // num / 2
		r++;
	}

	return r;
}

long RTools::shash(char *key)
{
	long hash = 0;
	int key_len = strlen(key);
	int i;
	
	for (i = 0; i < key_len; i++) 
	{
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

long RTools::CharSum(char* adr,long len)
{

	long rets;
	_asm
	{
		xor eax,eax					// eaxi sifirla
		mov ecx,len					// ecxe uzunlugu at
		mov edx,adr					// edxe veri adresini at
		dec edx						// edxi bir dusur \0 icin
		xor ebx,ebx					// ebxi bosalt

	lop:mov bl,BYTE PTR [edx+ecx]	// bl ye sondan 1 byte kopyala
		add eax,ebx					// eaxe bl yi ekle
		loop lop					// ecx 0 olana kadar lop a atla

		mov rets,eax				// eaxi rets'e yaz

	}

	return rets;					// retsi geri dondur

}




long RTools::CharSumPath(char* adr,long len)
{
	long rets;

	_asm
	{
		xor eax,eax
		mov ecx,len
		mov edx,adr
		dec edx
		xor ebx,ebx

lop:
		mov bl, byte ptr [edx+ecx]
		cmp bl, 0x2F
		je lop2
		cmp bl, 0x5C
		je lop2
		loop lop
		jmp endss
lop2:
		mov bl, byte ptr [edx+ecx]
		add eax,ebx
		loop lop2

endss:
		mov rets,eax
	}

	return rets;
}


char* RTools::GetFileName(char* adr,long len)
{
	char* rets;
	long datalen;
	long apt;

	_asm
	{
		xor eax,eax
		mov ecx,len
		mov edx,adr
		dec edx
		xor ebx,ebx

lop:
		mov bl, byte ptr [edx+ecx]
		cmp bl, 0x2F	// "/"
		je conti
		cmp bl, 0x5C	// "\"
		je conti
		loop lop
		jmp endss

conti:
		mov eax,adr
		add eax,ecx
		mov apt,eax
		mov eax,len
		sub eax,ecx
		inc eax			// 0 i da kat
		mov datalen, eax
	}
	rets = new char[datalen];
	memcpy(rets,(char*)apt,datalen);
	return rets;
	
	_asm{
endss:
		mov rets,0
	}
	return rets;
}

void RTools::QuickInfo(const char* fmt, ...)
{
	char text[256];
	va_list ap;
	if (fmt == NULL) return;
	va_start(ap,fmt);
		vsprintf(text,fmt,ap);
	va_end(ap);
	MessageBox(0,text,"reAction Info",MB_OK | MB_ICONASTERISK);
}
/*
	*************************************
	RLINKEDLIST - FUNCTIONS
*/



/*
	*************************************
	FARRAY - FUNCTIONS
*/

#define MOD_64	& 0x3F

RArray::RArray()
{
	size = 0;
	count = 0;
	pArray = 0;
}

RArray::~RArray()
{
	if (pArray != 0)
	{
		delete [] pArray;
	}
}

void RArray::add(void *data)
{
	// Allocate Memory
	if (size == 0)	// if it is never allocated
	{
		size = 64;//increase size variable
		pArray = new void * [size]; // allocate new empty space
	}
	else
	{
		if ((count MOD_64) == 0) // expand memory
		{
			size+=64;	// increase size variable
			void ** bos = new void * [size]; // allocate new empty space
			memcpy(bos,pArray,count<<2);	// copy old memory block to new
			delete [] pArray;	// delete old block
			pArray = bos;	// attach new memory block
		}
	}
	pArray[count] = data;	// add data's pointer to array list
	count++; // increase count
}

void RArray::remove(int id) // sloow one
{
	if (size == 0)
	{
		return;
	}
	if (id > count)
	{
		return;
	}
	if (id < 0)
	{
		return;
	}
	if (id == count) // son itemse
	{
		count--;
	}
	else
	{
		memcpy(pArray+id,pArray+id+1,(--count - id)<<2);
	}
}

/*
	*************************************
	FARRAY - END
*/

/*
	*************************************
	RSTRING - FUNCTIONS
*/

RArray kp;
int usedstr = 0;
int freestr = 0;

RString::RString()
{
	cstr = 0;
	slen = 0;
	usedstr++;
}
#define str_dealloc() if(cstr) { delete [] cstr; cstr=0; }
#define str_alloc(n) str_dealloc();cstr = new char [n];

RString::RString(char* asig)
{
	slen = strlen(asig);
	str_alloc(slen+1);
	memcpy(cstr,asig,slen+1);
}

RString::RString(RString* rst)
{
	slen = rst->slen;
	str_alloc(slen+1);
	memcpy(cstr,rst->cstr,slen+1);
}

RString::~RString()
{
	str_dealloc();
	freestr++;
}

void RString::setstring(char* st,...)
{
	char text[256];
	va_list ap;
	if (st == NULL) return;
	va_start(ap,st);
		slen = vsprintf(text,st,ap);
	va_end(ap);

	str_alloc(slen+1);
	strcpy(cstr,text);
}

RString & RString::operator = (char* asig)
{
	slen = strlen(asig);
	str_alloc(slen+1);
	memcpy(cstr,asig,slen+1);
	return *this;
}

RString &RString::operator + (char* asig)
{
	int slen2;
	slen2 = strlen(asig);

	char* tmpdata;
	tmpdata = new char [slen2+ slen +1];
	strncpy(tmpdata,cstr,slen);
	strncpy(tmpdata+slen,asig,slen2);
	str_dealloc();
	slen += slen2;
	cstr = tmpdata;
	cstr[slen] = 0;
	return *this;
}

RString &RString::operator + (RString* rst)
{
	char* tmpdata;
	tmpdata = new char [ slen + rst->slen + 1];
	memcpy(tmpdata,cstr,slen);
	memcpy(tmpdata+slen,rst->cstr,rst->slen);
	str_dealloc();
	slen += rst->slen;
	cstr = tmpdata;
	cstr[slen] = 0;
	return *this;
}

void RString::makelower()
{
	if (cstr) strlwr(cstr);
}

void RString::makeupper()
{
	if (cstr) strupr(cstr);
}

RString &RString::operator = (RString* rst)
{
	slen = rst->slen;
	str_alloc(rst->slen+1);
	memcpy(cstr,rst->cstr,slen);
	return  *this;
}

RString& RString::operator + (int ik)
{
	char tmp[16];
	RString tmpstr;
	tmpstr.cstr = tmp;
	tmpstr.slen = sprintf(tmp,"%i",ik);
	this->operator + (&tmpstr);
	tmpstr.cstr = 0;
	return *this;
}

RString& RString::operator + (float flt)
{
	char tmp[16];
	RString tmpstr;
	tmpstr.cstr = tmp;
	tmpstr.slen = sprintf(tmp,"%f",flt);
	this->operator + (&tmpstr);
	tmpstr.cstr = 0;
	return *this;
}

RString& RString::operator + (double dbl)
{
	char tmp[16];
	RString tmpstr;
	tmpstr.cstr = tmp;
	tmpstr.slen = sprintf(tmp,"%f",dbl);
	this->operator + (&tmpstr);
	tmpstr.cstr = 0;
	return *this;
}

RString& RString::operator + (char ch)
{
	char tmp[2];
	RString tmpstr;
	tmpstr.cstr = tmp;
	tmpstr.slen = 1;
	tmp[0] = ch;
	this->operator + (&tmpstr);
	tmpstr.cstr = 0;
	return *this;
}

int RString::instr(int start,char* ct)
{
	char* kp;
	kp = strstr(cstr+start,ct);
	if (kp > 0)
	{
		return (int)kp - (int)cstr;
	}
	else
	{
		return -1;
	}
}

int RString::instr(int start,RString* ct)
{
	return instr(start,ct->cstr);
}

RString RString::substr(int strt,int ln)
{
	// bound checking
	RString* p;
	p = new RString;
	p->cstr = new char [ln+1];
	memcpy(p->cstr,cstr+strt,ln);
	p->cstr[ln] = 0;
	p->slen = ln;
	return *p;
}

RString RString::left(int ln)
{
	RString* p;
	p = new RString;
	p->slen = ln;
	p->cstr = new char [ln+1];
	memcpy(p->cstr,cstr,ln);
	p->cstr[ln] = 0;
	return *p;
}

RString RString::right(int ln)
{
	RString* p;
	p = new RString;
	p->slen = ln;
	p->cstr = new char [ln+1];
	memcpy(p->cstr,cstr+slen-ln,ln+1);
	p->cstr[ln] = 0;
	return *p;
}

void RString::ltrim()
{
	char k;
	int st=slen;
	for (int i=0;i<slen;i++)
	{
		k = cstr[i];
		if ( (k == 0x20) || (k == 0x09) || (k == 0x0A) || (k == 0x0D) || (k == 0x0B))
		{
			continue;
		}
		else
		{
			st = i;
			break;
		}
	}
	
	if (st == 0)
	{
		return; // nothing to do
	}
	else
	{
		char* newar;
		slen = slen - st;
		if (slen == 0)
		{
			str_dealloc();
			cstr = new char [1];
			cstr[0] = 0;
		}
		else
		{
			newar = new char [slen+1];
			memcpy(newar,cstr+st,slen+1);
			str_dealloc();
			cstr = newar;
		}
	}
}

void RString::rtrim()
{
	char k;
	int st=-1;
	for (int i=slen-1;i>=0;i--)
	{
		k = cstr[i];
		if ( (k == 0x20) || (k == 0x09) || (k == 0x0A) || (k == 0x0D) || (k == 0x0B))
		{
			continue;
		}
		else
		{
			st = i+1;
			break;
		}
	}
	
	if (st == -1)
	{
		str_dealloc();
		cstr = new char [1];
		cstr[0] = 0;
	}
	else
	{
		char* newar;
		int oldslen = slen;
		slen = st;
		if (slen == oldslen)
		{
			return; // nothing to do
		}
		else
		{
			newar = new char [slen+1];
			memcpy(newar,cstr,slen);
			str_dealloc();
			cstr = newar;
			cstr[slen] = 0;
		}
	}

}

void RString::trim()
{
	int i;
	char k;
	int soldan=slen;
	int sagdan=slen;
	for (i=0;i<slen;i++)
	{
		k = cstr[i];
		if ( (k == 0x20) || (k == 0x09) || (k == 0x0A) || (k == 0x0D) || (k == 0x0B))
		{
			continue;
		}
		else
		{
			soldan = i;
			break;
		}
	}
	if (soldan == slen) // eger tamami bosluksa siktir et
	{
		str_dealloc();
		cstr = new char [1];
		cstr[0] = 0;
		return;
	}

	for (i=slen-1;i>=0;i--)
	{
		k = cstr[i];
		if ( (k == 0x20) || (k == 0x09) || (k == 0x0A) || (k == 0x0D) || (k == 0x0B))
		{
			continue;
		}
		else
		{
			sagdan = i+1;
			break;
		}
	}

	char* newar;
	slen = sagdan - soldan;

	newar = new char [slen+1];
	memcpy(newar,cstr+soldan,slen);
	str_dealloc();
	cstr = newar;
	cstr[slen] = 0;
}

void RString::remove(int st,int ed)
{
	if (ed-st == 0) return;

	int nslen = slen - (ed-st);
	char* newar;
	newar = new char [nslen+1];
	memcpy(newar,cstr,st);
	memcpy(newar+st,cstr+ed,slen-ed);
	str_dealloc();
	slen = nslen;
	cstr = newar;
	cstr[slen] = 0;
}

RString &RString::operator = (RString& rst)
{
	if (&rst != this)
	{
		slen = rst.slen;
		str_alloc(rst.slen+1);
		memcpy(cstr,rst.cstr,slen+1);
	}
	return  *this;
}

int RString::toint()
{
	return atoi(cstr);
}

float RString::tofloat()
{
	return atof(cstr);
}

void RString::memstatus()
{
	RTools::QuickInfo("Used: %i Free: %i",usedstr,freestr);
}

