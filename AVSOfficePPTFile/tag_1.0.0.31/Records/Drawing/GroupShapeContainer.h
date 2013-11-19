#pragma once
#include "ShapeContainer.h"

class CRecordGroupShapeContainer : public CRecordsContainer
{
private:
	RECT m_rcGroupBounds;
	RECT m_rcGroupClientAnchor;

public:
	
	CRecordGroupShapeContainer()
	{
		m_rcGroupBounds.left = 0;
		m_rcGroupBounds.top = 0;
		m_rcGroupBounds.right = 0;
		m_rcGroupBounds.bottom = 0;

		m_rcGroupClientAnchor.left = 0;
		m_rcGroupClientAnchor.top = 0;
		m_rcGroupClientAnchor.right = 0;
		m_rcGroupClientAnchor.bottom = 0;
	}

	~CRecordGroupShapeContainer()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		CRecordsContainer::ReadFromStream(oHeader, pStream);

		// ���... � ������ ����� ����� � ������ ���������� ������� ���������
		CSimpleArray<CRecordShapeContainer*> oArrayShapes;
		this->GetRecordsByType(&oArrayShapes, false, false);

		int nIndexBreak = -1;
		for (int nIndex = 0; nIndex < oArrayShapes.GetSize(); ++nIndex)
		{
			CSimpleArray<CRecordGroupShape*> oArrayGroupShapes;
			oArrayShapes[nIndex]->GetRecordsByType(&oArrayGroupShapes, false, true);

			if (0 != oArrayGroupShapes.GetSize())
			{
				m_rcGroupBounds.left	= oArrayGroupShapes[0]->m_oBounds.left;
				m_rcGroupBounds.top		= oArrayGroupShapes[0]->m_oBounds.top;
				m_rcGroupBounds.right	= oArrayGroupShapes[0]->m_oBounds.right;
				m_rcGroupBounds.bottom	= oArrayGroupShapes[0]->m_oBounds.bottom;

				CSimpleArray<CRecordClientAnchor*> oArrayClients;
				oArrayShapes[nIndex]->GetRecordsByType(&oArrayClients, false, true);

				if (0 != oArrayClients.GetSize())
				{
					m_rcGroupClientAnchor.left		= (LONG)oArrayClients[0]->m_oBounds.Left;
					m_rcGroupClientAnchor.top		= (LONG)oArrayClients[0]->m_oBounds.Top;
					m_rcGroupClientAnchor.right		= (LONG)oArrayClients[0]->m_oBounds.Right;
					m_rcGroupClientAnchor.bottom	= (LONG)oArrayClients[0]->m_oBounds.Bottom;
				}
				else
				{
					CSimpleArray<CRecordChildAnchor*> oArrayChilds;
					oArrayShapes[nIndex]->GetRecordsByType(&oArrayChilds, false, true);
					
					if (0 != oArrayChilds.GetSize())
					{
						m_rcGroupClientAnchor.left		= (LONG)oArrayChilds[0]->m_oBounds.left;
						m_rcGroupClientAnchor.top		= (LONG)oArrayChilds[0]->m_oBounds.top;
						m_rcGroupClientAnchor.right		= (LONG)oArrayChilds[0]->m_oBounds.right;
						m_rcGroupClientAnchor.bottom	= (LONG)oArrayChilds[0]->m_oBounds.bottom;
					}
				}

				nIndexBreak = nIndex;
				break;
			}
		}

		LONG lW1 = m_rcGroupBounds.right - m_rcGroupBounds.left;
		LONG lH1 = m_rcGroupBounds.bottom - m_rcGroupBounds.top;
		LONG lW2 = m_rcGroupClientAnchor.right - m_rcGroupClientAnchor.left;
		LONG lH2 = m_rcGroupClientAnchor.bottom - m_rcGroupClientAnchor.top;

		BOOL bIsRecalc = ((lW1 > 0) && (lH1 > 0) && (lW2 > 0) && (lH2 > 0));
		if (bIsRecalc)
		{
			for (int nIndex = 0; nIndex < oArrayShapes.GetSize(); ++nIndex)
			{
				if (nIndex != nIndexBreak)
				{
					oArrayShapes[nIndex]->m_pGroupBounds = &m_rcGroupBounds;
					oArrayShapes[nIndex]->m_pGroupClientAnchor = &m_rcGroupClientAnchor;
				}
			}
		}
	}
	virtual CString ToString()
	{
		return CRecordsContainer::ToString();
	}
};