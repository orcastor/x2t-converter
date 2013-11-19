#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CFlowChartManualOperation : public CPPTXShape
	{
		public:
			CFlowChartManualOperation()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"x3\" fmla=\"*/ w 4 5\" />")
					  _T("<gd name=\"x4\" fmla=\"*/ w 9 10\" />")
					_T("</gdLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"hc\" y=\"t\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd2\">")
						_T("<pos x=\"wd10\" y=\"vc\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"hc\" y=\"b\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"0\">")
						_T("<pos x=\"x4\" y=\"vc\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"wd5\" t=\"t\" r=\"x3\" b=\"b\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path w=\"5\" h=\"5\">")
						_T("<moveTo>")
						  _T("<pt x=\"0\" y=\"0\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"5\" y=\"0\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"4\" y=\"5\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"1\" y=\"5\" />")
						_T("</lnTo>")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}