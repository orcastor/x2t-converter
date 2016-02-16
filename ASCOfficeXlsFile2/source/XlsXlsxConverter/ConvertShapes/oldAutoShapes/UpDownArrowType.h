#pragma once
#include "../CustomShape.h"

// 70
class CUpDownArrowType : public CCustomShape
{
public:
	CUpDownArrowType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m10800,l21600@0@3@0@3@2,21600@2,10800,21600,0@2@1@2@1@0,0@0xe");

        AddGuide(_T("val #1"));
        AddGuide(_T("val #0"));
        AddGuide(_T("sum 21600 0 #1"));
        AddGuide(_T("sum 21600 0 #0"));
        AddGuide(_T("prod #1 #0 10800"));
        AddGuide(_T("sum #1 0 @4"));
        AddGuide(_T("sum 21600 0 @5"));

		m_arAdjustments.push_back(5400);
		m_arAdjustments.push_back(4320);

        LoadConnectorsList(_T("10800,0;0,@0;@1,10800;0,@2;10800,21600;21600,@2;@3,10800;21600,@0"));

		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);
		m_arConnectorAngles.push_back(0);
		m_arConnectorAngles.push_back(0);

        LoadTextRect(_T("@1,@5,@3,@6"));
        
        CHandle_ oHandle1;
        oHandle1.position = _T("#0,#1");
        oHandle1.xrange = _T("0,10800");
        oHandle1.yrange = _T("0,10800");
        m_arHandles.push_back(oHandle1);
    }
};
