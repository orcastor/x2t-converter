#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CActionButtonMovie : public CPPTXShape
	{
		public:
			CActionButtonMovie()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"dx2\" fmla=\"*/ ss 3 8\" />")
					  _T("<gd name=\"g9\" fmla=\"+- vc 0 dx2\" />")
					  _T("<gd name=\"g10\" fmla=\"+- vc dx2 0\" />")
					  _T("<gd name=\"g11\" fmla=\"+- hc 0 dx2\" />")
					  _T("<gd name=\"g12\" fmla=\"+- hc dx2 0\" />")
					  _T("<gd name=\"g13\" fmla=\"*/ ss 3 4\" />")
					  _T("<gd name=\"g14\" fmla=\"*/ g13 1455 21600\" />")
					  _T("<gd name=\"g15\" fmla=\"*/ g13 1905 21600\" />")
					  _T("<gd name=\"g16\" fmla=\"*/ g13 2325 21600\" />")
					  _T("<gd name=\"g17\" fmla=\"*/ g13 16155 21600\" />")
					  _T("<gd name=\"g18\" fmla=\"*/ g13 17010 21600\" />")
					  _T("<gd name=\"g19\" fmla=\"*/ g13 19335 21600\" />")
					  _T("<gd name=\"g20\" fmla=\"*/ g13 19725 21600\" />")
					  _T("<gd name=\"g21\" fmla=\"*/ g13 20595 21600\" />")
					  _T("<gd name=\"g22\" fmla=\"*/ g13 5280 21600\" />")
					  _T("<gd name=\"g23\" fmla=\"*/ g13 5730 21600\" />")
					  _T("<gd name=\"g24\" fmla=\"*/ g13 6630 21600\" />")
					  _T("<gd name=\"g25\" fmla=\"*/ g13 7492 21600\" />")
					  _T("<gd name=\"g26\" fmla=\"*/ g13 9067 21600\" />")
					  _T("<gd name=\"g27\" fmla=\"*/ g13 9555 21600\" />")
					  _T("<gd name=\"g28\" fmla=\"*/ g13 13342 21600\" />")
					  _T("<gd name=\"g29\" fmla=\"*/ g13 14580 21600\" />")
					  _T("<gd name=\"g30\" fmla=\"*/ g13 15592 21600\" />")
					  _T("<gd name=\"g31\" fmla=\"+- g11 g14 0\" />")
					  _T("<gd name=\"g32\" fmla=\"+- g11 g15 0\" />")
					  _T("<gd name=\"g33\" fmla=\"+- g11 g16 0\" />")
					  _T("<gd name=\"g34\" fmla=\"+- g11 g17 0\" />")
					  _T("<gd name=\"g35\" fmla=\"+- g11 g18 0\" />")
					  _T("<gd name=\"g36\" fmla=\"+- g11 g19 0\" />")
					  _T("<gd name=\"g37\" fmla=\"+- g11 g20 0\" />")
					  _T("<gd name=\"g38\" fmla=\"+- g11 g21 0\" />")
					  _T("<gd name=\"g39\" fmla=\"+- g9 g22 0\" />")
					  _T("<gd name=\"g40\" fmla=\"+- g9 g23 0\" />")
					  _T("<gd name=\"g41\" fmla=\"+- g9 g24 0\" />")
					  _T("<gd name=\"g42\" fmla=\"+- g9 g25 0\" />")
					  _T("<gd name=\"g43\" fmla=\"+- g9 g26 0\" />")
					  _T("<gd name=\"g44\" fmla=\"+- g9 g27 0\" />")
					  _T("<gd name=\"g45\" fmla=\"+- g9 g28 0\" />")
					  _T("<gd name=\"g46\" fmla=\"+- g9 g29 0\" />")
					  _T("<gd name=\"g47\" fmla=\"+- g9 g30 0\" />")
					  _T("<gd name=\"g48\" fmla=\"+- g9 g31 0\" />")
					_T("</gdLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"0\">")
						_T("<pos x=\"r\" y=\"vc\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"hc\" y=\"b\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd2\">")
						_T("<pos x=\"l\" y=\"vc\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"hc\" y=\"t\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"l\" t=\"t\" r=\"r\" b=\"b\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path stroke=\"false\" extrusionOk=\"false\">")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"t\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"t\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"b\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"l\" y=\"b\" />")
						_T("</lnTo>")
						_T("<close />")
						_T("<moveTo>")
						  _T("<pt x=\"g11\" y=\"g39\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g11\" y=\"g44\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g31\" y=\"g44\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g32\" y=\"g43\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g33\" y=\"g43\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g33\" y=\"g47\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g35\" y=\"g47\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g35\" y=\"g45\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g36\" y=\"g45\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g38\" y=\"g46\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g12\" y=\"g46\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g12\" y=\"g41\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g38\" y=\"g41\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g37\" y=\"g42\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g35\" y=\"g42\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g35\" y=\"g41\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g34\" y=\"g40\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g32\" y=\"g40\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g31\" y=\"g39\" />")
						_T("</lnTo>")
						_T("<close />")
					  _T("</path>")
					  _T("<path stroke=\"false\" fill=\"darken\" extrusionOk=\"false\">")
						_T("<moveTo>")
						  _T("<pt x=\"g11\" y=\"g39\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g11\" y=\"g44\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g31\" y=\"g44\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g32\" y=\"g43\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g33\" y=\"g43\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g33\" y=\"g47\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g35\" y=\"g47\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g35\" y=\"g45\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g36\" y=\"g45\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g38\" y=\"g46\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g12\" y=\"g46\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g12\" y=\"g41\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g38\" y=\"g41\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g37\" y=\"g42\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g35\" y=\"g42\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g35\" y=\"g41\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g34\" y=\"g40\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g32\" y=\"g40\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g31\" y=\"g39\" />")
						_T("</lnTo>")
						_T("<close />")
					  _T("</path>")
					  _T("<path fill=\"none\" extrusionOk=\"false\">")
						_T("<moveTo>")
						  _T("<pt x=\"g11\" y=\"g39\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g31\" y=\"g39\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g32\" y=\"g40\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g34\" y=\"g40\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g35\" y=\"g41\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g35\" y=\"g42\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g37\" y=\"g42\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g38\" y=\"g41\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g12\" y=\"g41\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g12\" y=\"g46\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g38\" y=\"g46\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g36\" y=\"g45\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g35\" y=\"g45\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g35\" y=\"g47\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g33\" y=\"g47\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g33\" y=\"g43\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g32\" y=\"g43\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g31\" y=\"g44\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"g11\" y=\"g44\" />")
						_T("</lnTo>")
						_T("<close />")
					  _T("</path>")
					  _T("<path fill=\"none\">")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"t\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"t\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"b\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"l\" y=\"b\" />")
						_T("</lnTo>")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}