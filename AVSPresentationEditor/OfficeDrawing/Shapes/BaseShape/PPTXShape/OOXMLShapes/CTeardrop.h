#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CTeardrop : public CPPTXShape
	{
		public:
			CTeardrop()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj\" fmla=\"val 100000\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"a\" fmla=\"pin 0 adj 200000\" />")
					  _T("<gd name=\"r2\" fmla=\"sqrt 2\" />")
					  _T("<gd name=\"tw\" fmla=\"*/ wd2 r2 1\" />")
					  _T("<gd name=\"th\" fmla=\"*/ hd2 r2 1\" />")
					  _T("<gd name=\"sw\" fmla=\"*/ tw a 100000\" />")
					  _T("<gd name=\"sh\" fmla=\"*/ th a 100000\" />")
					  _T("<gd name=\"dx1\" fmla=\"cos sw 2700000\" />")
					  _T("<gd name=\"dy1\" fmla=\"sin sh 2700000\" />")
					  _T("<gd name=\"x1\" fmla=\"+- hc dx1 0\" />")
					  _T("<gd name=\"y1\" fmla=\"+- vc 0 dy1\" />")
					  _T("<gd name=\"x2\" fmla=\"+/ hc x1 2\" />")
					  _T("<gd name=\"y2\" fmla=\"+/ vc y1 2\" />")
					  _T("<gd name=\"idx\" fmla=\"cos wd2 2700000\" />")
					  _T("<gd name=\"idy\" fmla=\"sin hd2 2700000\" />")
					  _T("<gd name=\"il\" fmla=\"+- hc 0 idx\" />")
					  _T("<gd name=\"ir\" fmla=\"+- hc idx 0\" />")
					  _T("<gd name=\"it\" fmla=\"+- vc 0 idy\" />")
					  _T("<gd name=\"ib\" fmla=\"+- vc idy 0\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefX=\"adj\" minX=\"0\" maxX=\"200000\">")
						_T("<pos x=\"x1\" y=\"t\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"0\">")
						_T("<pos x=\"r\" y=\"vc\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"ir\" y=\"ib\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"hc\" y=\"b\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"il\" y=\"ib\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd2\">")
						_T("<pos x=\"l\" y=\"vc\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"il\" y=\"it\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"hc\" y=\"t\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"x1\" y=\"y1\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"il\" t=\"it\" r=\"ir\" b=\"ib\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"vc\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"wd2\" hR=\"hd2\" stAng=\"cd2\" swAng=\"cd4\" />")
						_T("<quadBezTo>")
						  _T("<pt x=\"x2\" y=\"t\" />")
						  _T("<pt x=\"x1\" y=\"y1\" />")
						_T("</quadBezTo>")
						_T("<quadBezTo>")
						  _T("<pt x=\"r\" y=\"y2\" />")
						  _T("<pt x=\"r\" y=\"vc\" />")
						_T("</quadBezTo>")
						_T("<arcTo wR=\"wd2\" hR=\"hd2\" stAng=\"0\" swAng=\"cd4\" />")
						_T("<arcTo wR=\"wd2\" hR=\"hd2\" stAng=\"cd4\" swAng=\"cd4\" />")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}