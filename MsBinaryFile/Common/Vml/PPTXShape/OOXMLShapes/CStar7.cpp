/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "CStar7.h"

namespace OOXMLShapes
{
			CStar7::CStar7()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj\" fmla=\"val 34601\" />")
					  _T("<gd name=\"hf\" fmla=\"val 102572\" />")
					  _T("<gd name=\"vf\" fmla=\"val 105210\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"a\" fmla=\"pin 0 adj 50000\" />")
					  _T("<gd name=\"swd2\" fmla=\"*/ wd2 hf 100000\" />")
					  _T("<gd name=\"shd2\" fmla=\"*/ hd2 vf 100000\" />")
					  _T("<gd name=\"svc\" fmla=\"*/ vc  vf 100000\" />")
					  _T("<gd name=\"dx1\" fmla=\"*/ swd2 97493 100000\" />")
					  _T("<gd name=\"dx2\" fmla=\"*/ swd2 78183 100000\" />")
					  _T("<gd name=\"dx3\" fmla=\"*/ swd2 43388 100000\" />")
					  _T("<gd name=\"dy1\" fmla=\"*/ shd2 62349 100000\" />")
					  _T("<gd name=\"dy2\" fmla=\"*/ shd2 22252 100000\" />")
					  _T("<gd name=\"dy3\" fmla=\"*/ shd2 90097 100000\" />")
					  _T("<gd name=\"x1\" fmla=\"+- hc 0 dx1\" />")
					  _T("<gd name=\"x2\" fmla=\"+- hc 0 dx2\" />")
					  _T("<gd name=\"x3\" fmla=\"+- hc 0 dx3\" />")
					  _T("<gd name=\"x4\" fmla=\"+- hc dx3 0\" />")
					  _T("<gd name=\"x5\" fmla=\"+- hc dx2 0\" />")
					  _T("<gd name=\"x6\" fmla=\"+- hc dx1 0\" />")
					  _T("<gd name=\"y1\" fmla=\"+- svc 0 dy1\" />")
					  _T("<gd name=\"y2\" fmla=\"+- svc dy2 0\" />")
					  _T("<gd name=\"y3\" fmla=\"+- svc dy3 0\" />")
					  _T("<gd name=\"iwd2\" fmla=\"*/ swd2 a 50000\" />")
					  _T("<gd name=\"ihd2\" fmla=\"*/ shd2 a 50000\" />")
					  _T("<gd name=\"sdx1\" fmla=\"*/ iwd2 97493 100000\" />")
					  _T("<gd name=\"sdx2\" fmla=\"*/ iwd2 78183 100000\" />")
					  _T("<gd name=\"sdx3\" fmla=\"*/ iwd2 43388 100000\" />")
					  _T("<gd name=\"sx1\" fmla=\"+- hc 0 sdx1\" />")
					  _T("<gd name=\"sx2\" fmla=\"+- hc 0 sdx2\" />")
					  _T("<gd name=\"sx3\" fmla=\"+- hc 0 sdx3\" />")
					  _T("<gd name=\"sx4\" fmla=\"+- hc sdx3 0\" />")
					  _T("<gd name=\"sx5\" fmla=\"+- hc sdx2 0\" />")
					  _T("<gd name=\"sx6\" fmla=\"+- hc sdx1 0\" />")
					  _T("<gd name=\"sdy1\" fmla=\"*/ ihd2 90097 100000\" />")
					  _T("<gd name=\"sdy2\" fmla=\"*/ ihd2 22252 100000\" />")
					  _T("<gd name=\"sdy3\" fmla=\"*/ ihd2 62349 100000\" />")
					  _T("<gd name=\"sy1\" fmla=\"+- svc 0 sdy1\" />")
					  _T("<gd name=\"sy2\" fmla=\"+- svc 0 sdy2\" />")
					  _T("<gd name=\"sy3\" fmla=\"+- svc sdy3 0\" />")
					  _T("<gd name=\"sy4\" fmla=\"+- svc ihd2 0\" />")
					  _T("<gd name=\"yAdj\" fmla=\"+- svc 0 ihd2\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefY=\"adj\" minY=\"0\" maxY=\"50000\">")
						_T("<pos x=\"hc\" y=\"yAdj\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"0\">")
						_T("<pos x=\"x5\" y=\"y1\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"0\">")
						_T("<pos x=\"x6\" y=\"y2\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"x4\" y=\"y3\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"x3\" y=\"y3\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd2\">")
						_T("<pos x=\"x1\" y=\"y2\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd2\">")
						_T("<pos x=\"x2\" y=\"y1\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"hc\" y=\"t\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"sx2\" t=\"sy1\" r=\"sx5\" b=\"sy3\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"x1\" y=\"y2\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"sx1\" y=\"sy2\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"x2\" y=\"y1\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"sx3\" y=\"sy1\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"hc\" y=\"t\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"sx4\" y=\"sy1\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"x5\" y=\"y1\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"sx6\" y=\"sy2\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"x6\" y=\"y2\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"sx5\" y=\"sy3\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"x4\" y=\"y3\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"hc\" y=\"sy4\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"x3\" y=\"y3\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"sx2\" y=\"sy3\" />")
						_T("</lnTo>")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
}
