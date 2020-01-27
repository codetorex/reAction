
#ifndef ARRAY_H
#define ARRAY_H

#include "StdAfx.h"
#include <stdio.h>
#include <memory.h>
#include "../../reAction/rcabinet.h"
#include "../../reAction/rtools.h"


template <class Ty,int iSize>
class carray
{
private:
    Ty *m_pArray;
    int m_iSize;
    int m_iCount;
 
public:
    carray<Ty,iSize>()
	{
		m_iSize=iSize;
		m_iCount=0;
		m_pArray = new Ty[iSize];
	};

    ~carray()
	{
		if (m_pArray)delete []m_pArray;
	};

    inline int GetSize() const { return m_iSize; }
    inline int GetCount() const { return m_iCount; }

    void Resize(int iSize)
	{
		Ty *pTemp = m_pArray;
		if (!m_pArray || iSize < 1) return;
		m_pArray = new Ty[iSize];
		if (m_pArray || !iSize)
		{
			memcpy(m_pArray, pTemp, sizeof(Ty) * (iSize > m_iSize ? m_iSize : iSize));
			m_iSize = iSize;
			m_iCount = (m_iCount > m_iSize ? m_iSize : m_iCount);
			delete []pTemp;
		}
		else
		{
			m_pArray = pTemp;
		}
	};

    void ClearAll(){m_iCount = 0;};

    void InsertElement(int iElement, Ty iValue)
	{
		if (!m_pArray || iElement < 0 || iElement > m_iCount)return ;
		if (m_iCount == m_iSize) Resize(m_iSize + 1);
		for (int i = m_iCount; i > iElement; --i) 
			m_pArray[i] = m_pArray[i-1];
		m_pArray[iElement] = iValue;++m_iCount;
	};

    void RemoveElement(int iElement)
	{
		if (!m_pArray || iElement < 0 || iElement >= m_iCount)return;
		for (int i = iElement; i < m_iCount - 1; ++i)
			m_pArray[i] = m_pArray[i+1];--m_iCount;
	};

    void AddElement(Ty iValue)
	{
		if (!m_pArray)return;
		if (m_iCount == m_iSize)
			Resize(m_iSize + 1);
		m_pArray[m_iCount++] = iValue;
	};

    Ty &operator[](int iElement)
	{
		static Ty iError = -1;
		if (!m_pArray || iElement < 0 || iElement > m_iSize - 1)return iError;
		return m_pArray[iElement];
	};

	void EditElement(int iElement, Ty iValue)
	{
		memcpy(&m_pArray[iElement],&iValue,4);
	}; 
};

CString		GetItemPath(CTreeCtrl* kr, HTREEITEM tzrt,HTREEITEM rt);
long		GetItemDepth(CTreeCtrl* kr, HTREEITEM tzrt,HTREEITEM rt);
HTREEITEM	IsItemExist(CTreeCtrl* kr,HTREEITEM rt,char* nick);
void		CreatePath(CTreeCtrl* kr,HTREEITEM rt,char* path);
void		LoadTree(CTreeCtrl* kr,HTREEITEM rt,RCabinet* rcb);
void		LoadDir(CListView* kr,RCabinet* rcb,char* dir);


#endif