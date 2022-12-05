﻿/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#include "../../../PPTXFormat/Logic/Media/WavAudioFile.cpp"
#include "../../../PPTXFormat/Logic/Media/AudioCD.cpp"
#include "../../../PPTXFormat/Logic/Media/MediaFile.cpp"

#include "../../../PPTXFormat/Logic/Path2D/ArcTo.cpp"
#include "../../../PPTXFormat/Logic/Path2D/Close.cpp"
#include "../../../PPTXFormat/Logic/Path2D/CubicBezTo.cpp"
#include "../../../PPTXFormat/Logic/Path2D/LineTo.cpp"
#include "../../../PPTXFormat/Logic/Path2D/MoveTo.cpp"
#include "../../../PPTXFormat/Logic/Path2D/PathBase.cpp"
#include "../../../PPTXFormat/Logic/Path2D/QuadBezTo.cpp"

#include "../../../PPTXFormat/Logic/Colors/SchemeClr.cpp"
#include "../../../PPTXFormat/Logic/Colors/ColorBase.cpp"
#include "../../../PPTXFormat/Logic/Colors/ColorModifier.cpp"
#include "../../../PPTXFormat/Logic/Colors/PrstClr.cpp"
#include "../../../PPTXFormat/Logic/Colors/SrgbClr.cpp"
#include "../../../PPTXFormat/Logic/Colors/SysClr.cpp"

#include "../../../PPTXFormat/Logic/Fills/Blip.cpp"
#include "../../../PPTXFormat/Logic/Fills/BlipFill.cpp"
#include "../../../PPTXFormat/Logic/Fills/GradFill.cpp"
#include "../../../PPTXFormat/Logic/Fills/NoFill.cpp"
#include "../../../PPTXFormat/Logic/Fills/PattFill.cpp"
#include "../../../PPTXFormat/Logic/Fills/SolidFill.cpp"
#include "../../../PPTXFormat/Logic/Fills/Stretch.cpp"
#include "../../../PPTXFormat/Logic/Fills/Tile.cpp"

#include "../../../PPTXFormat/Logic/Table/TableCell.cpp"
#include "../../../PPTXFormat/Logic/Timing/BuildNodeBase.cpp"
#include "../../../PPTXFormat/Logic/Timing/TimeNodeBase.cpp"
#include "../../../PPTXFormat/Logic/Transitions/TransitionBase.cpp"
#include "../../../PPTXFormat/Logic/Bg.cpp"
#include "../../../PPTXFormat/Logic/CNvGrpSpPr.cpp"
#include "../../../PPTXFormat/Logic/CxnSp.cpp"
#include "../../../PPTXFormat/Logic/EffectDag.cpp"
#include "../../../PPTXFormat/Logic/EffectLst.cpp"
#include "../../../PPTXFormat/Logic/EffectProperties.cpp"
#include "../../../PPTXFormat/Logic/EffectStyle.cpp"
#include "../../../PPTXFormat/Logic/Geometry.cpp"
#include "../../../PPTXFormat/Logic/GraphicFrame.cpp"
#include "../../../PPTXFormat/Logic/HeadingVariant.cpp"
#include "../../../PPTXFormat/Logic/GrpSpPr.cpp"
#include "../../../PPTXFormat/Logic/Hyperlink.cpp"
#include "../../../PPTXFormat/Logic/NvGraphicFramePr.cpp"
#include "../../../PPTXFormat/Logic/NvGrpSpPr.cpp"
#include "../../../PPTXFormat/Logic/Pic.cpp"
#include "../../../PPTXFormat/Logic/Shape.cpp"
#include "../../../PPTXFormat/Logic/SmartArt.cpp"
#include "../../../PPTXFormat/Logic/SpPr.cpp"
#include "../../../PPTXFormat/Logic/SpTree.cpp"
#include "../../../PPTXFormat/Logic/SpTreeElem.cpp"
#include "../../../PPTXFormat/Logic/TxBody.cpp"
#include "../../../PPTXFormat/Logic/UniColor.cpp"
#include "../../../PPTXFormat/Logic/UniEffect.cpp"
#include "../../../PPTXFormat/Logic/UniFill.cpp"
#include "../../../PPTXFormat/Logic/Runs/MathParaWrapper.cpp"
#include "../../../PPTXFormat/Logic/Controls.cpp"

#include "../../../PPTXFormat/Logic/Ah.cpp"
#include "../../../PPTXFormat/Logic/AhBase.cpp"
#include "../../../PPTXFormat/Logic/AhPolar.cpp"
#include "../../../PPTXFormat/Logic/AhXY.cpp"
#include "../../../PPTXFormat/Logic/Backdrop.cpp"
#include "../../../PPTXFormat/Logic/Bevel.cpp"
#include "../../../PPTXFormat/Logic/BgPr.cpp"
#include "../../../PPTXFormat/Logic/BodyPr.cpp"
#include "../../../PPTXFormat/Logic/Camera.cpp"
#include "../../../PPTXFormat/Logic/Cell3D.cpp"
#include "../../../PPTXFormat/Logic/ClrMap.cpp"
#include "../../../PPTXFormat/Logic/ClrMapOvr.cpp"
#include "../../../PPTXFormat/Logic/CNvCxnSpPr.cpp"
#include "../../../PPTXFormat/Logic/CNvGraphicFramePr.cpp"
#include "../../../PPTXFormat/Logic/CNvPicPr.cpp"
#include "../../../PPTXFormat/Logic/CNvPr.cpp"
#include "../../../PPTXFormat/Logic/CNvSpPr.cpp"
#include "../../../PPTXFormat/Logic/CSld.cpp"
#include "../../../PPTXFormat/Logic/CustGeom.cpp"
#include "../../../PPTXFormat/Logic/Cxn.cpp"
#include "../../../PPTXFormat/Logic/DefaultShapeDefinition.cpp"
#include "../../../PPTXFormat/Logic/ExtP.cpp"
#include "../../../PPTXFormat/Logic/FillStyle.cpp"
#include "../../../PPTXFormat/Logic/FontCollection.cpp"
#include "../../../PPTXFormat/Logic/FontRef.cpp"
#include "../../../PPTXFormat/Logic/Gd.cpp"
#include "../../../PPTXFormat/Logic/Gs.cpp"
#include "../../../PPTXFormat/Logic/Hf.cpp"
#include "../../../PPTXFormat/Logic/LightRig.cpp"
#include "../../../PPTXFormat/Logic/Lin.cpp"
#include "../../../PPTXFormat/Logic/LineEnd.cpp"
#include "../../../PPTXFormat/Logic/LineJoin.cpp"
#include "../../../PPTXFormat/Logic/LineStyle.cpp"
#include "../../../PPTXFormat/Logic/Ln.cpp"
#include "../../../PPTXFormat/Logic/NvCxnSpPr.cpp"
#include "../../../PPTXFormat/Logic/NvPicPr.cpp"
#include "../../../PPTXFormat/Logic/NvPr.cpp"
#include "../../../PPTXFormat/Logic/NvSpPr.cpp"
#include "../../../PPTXFormat/Logic/Paragraph.cpp"
#include "../../../PPTXFormat/Logic/PartTitle.cpp"
#include "../../../PPTXFormat/Logic/Path.cpp"
#include "../../../PPTXFormat/Logic/Path2D.cpp"
#include "../../../PPTXFormat/Logic/Ph.cpp"
#include "../../../PPTXFormat/Logic/PrstDash.cpp"
#include "../../../PPTXFormat/Logic/PrstGeom.cpp"
#include "../../../PPTXFormat/Logic/PrstTxWarp.cpp"
#include "../../../PPTXFormat/Logic/Rect.cpp"
#include "../../../PPTXFormat/Logic/Rot.cpp"
#include "../../../PPTXFormat/Logic/RunElem.cpp"
#include "../../../PPTXFormat/Logic/RunProperties.cpp"
#include "../../../PPTXFormat/Logic/Scene3d.cpp"
#include "../../../PPTXFormat/Logic/ShapeStyle.cpp"
#include "../../../PPTXFormat/Logic/Sp3d.cpp"
#include "../../../PPTXFormat/Logic/StyleRef.cpp"
#include "../../../PPTXFormat/Logic/SupplementalFont.cpp"
#include "../../../PPTXFormat/Logic/Tab.cpp"
#include "../../../PPTXFormat/Logic/TableBgStyle.cpp"
#include "../../../PPTXFormat/Logic/TablePartStyle.cpp"
#include "../../../PPTXFormat/Logic/TableStyle.cpp"
#include "../../../PPTXFormat/Logic/TcBdr.cpp"
#include "../../../PPTXFormat/Logic/TcStyle.cpp"
#include "../../../PPTXFormat/Logic/TcTxStyle.cpp"
#include "../../../PPTXFormat/Logic/TextFit.cpp"
#include "../../../PPTXFormat/Logic/TextFont.cpp"
#include "../../../PPTXFormat/Logic/TextListStyle.cpp"
#include "../../../PPTXFormat/Logic/TextParagraphPr.cpp"
#include "../../../PPTXFormat/Logic/TextSpacing.cpp"
#include "../../../PPTXFormat/Logic/TxStyles.cpp"
#include "../../../PPTXFormat/Logic/UniMedia.cpp"
#include "../../../PPTXFormat/Logic/UniPath2D.cpp"
#include "../../../PPTXFormat/Logic/Xfrm.cpp"
#include "../../../PPTXFormat/Logic/XmlId.cpp"

#include "../../../PPTXFormat/Logic/Bullets/BuAutoNum.cpp"
#include "../../../PPTXFormat/Logic/Bullets/BuBlip.cpp"
#include "../../../PPTXFormat/Logic/Bullets/BuChar.cpp"
#include "../../../PPTXFormat/Logic/Bullets/BuClr.cpp"
#include "../../../PPTXFormat/Logic/Bullets/BuClrTx.cpp"
#include "../../../PPTXFormat/Logic/Bullets/BuFontTx.cpp"
#include "../../../PPTXFormat/Logic/Bullets/Bullet.cpp"
#include "../../../PPTXFormat/Logic/Bullets/BulletColor.cpp"
#include "../../../PPTXFormat/Logic/Bullets/BulletSize.cpp"
#include "../../../PPTXFormat/Logic/Bullets/BulletTypeface.cpp"
#include "../../../PPTXFormat/Logic/Bullets/BuNone.cpp"
#include "../../../PPTXFormat/Logic/Bullets/BuSzPct.cpp"
#include "../../../PPTXFormat/Logic/Bullets/BuSzPts.cpp"
#include "../../../PPTXFormat/Logic/Bullets/BuSzTx.cpp"

#include "../../../PPTXFormat/Logic/Effects/AlphaBiLevel.cpp"
#include "../../../PPTXFormat/Logic/Effects/AlphaCeiling.cpp"
#include "../../../PPTXFormat/Logic/Effects/AlphaFloor.cpp"
#include "../../../PPTXFormat/Logic/Effects/AlphaInv.cpp"
#include "../../../PPTXFormat/Logic/Effects/AlphaMod.cpp"
#include "../../../PPTXFormat/Logic/Effects/AlphaModFix.cpp"
#include "../../../PPTXFormat/Logic/Effects/AlphaOutset.cpp"
#include "../../../PPTXFormat/Logic/Effects/AlphaRepl.cpp"
#include "../../../PPTXFormat/Logic/Effects/BiLevel.cpp"
#include "../../../PPTXFormat/Logic/Effects/Blend.cpp"
#include "../../../PPTXFormat/Logic/Effects/Blur.cpp"
#include "../../../PPTXFormat/Logic/Effects/ClrChange.cpp"
#include "../../../PPTXFormat/Logic/Effects/ClrRepl.cpp"
#include "../../../PPTXFormat/Logic/Effects/Duotone.cpp"
#include "../../../PPTXFormat/Logic/Effects/EffectElement.cpp"
#include "../../../PPTXFormat/Logic/Effects/FillEffect.cpp"
#include "../../../PPTXFormat/Logic/Effects/FillOverlay.cpp"
#include "../../../PPTXFormat/Logic/Effects/Glow.cpp"
#include "../../../PPTXFormat/Logic/Effects/Grayscl.cpp"
#include "../../../PPTXFormat/Logic/Effects/HslEffect.cpp"
#include "../../../PPTXFormat/Logic/Effects/InnerShdw.cpp"
#include "../../../PPTXFormat/Logic/Effects/LumEffect.cpp"
#include "../../../PPTXFormat/Logic/Effects/OuterShdw.cpp"
#include "../../../PPTXFormat/Logic/Effects/PrstShdw.cpp"
#include "../../../PPTXFormat/Logic/Effects/Reflection.cpp"
#include "../../../PPTXFormat/Logic/Effects/RelOff.cpp"
#include "../../../PPTXFormat/Logic/Effects/SoftEdge.cpp"
#include "../../../PPTXFormat/Logic/Effects/TintEffect.cpp"
#include "../../../PPTXFormat/Logic/Effects/XfrmEffect.cpp"


#include "../../../PPTXFormat/Presentation.cpp"
#include "../../../PPTXFormat/FileContainer.cpp"
#include "../../../PPTXFormat/FileTypes.cpp"
#include "../../../PPTXFormat/FileFactory.cpp"
#include "../../../PPTXFormat/FileMap.cpp"
#include "../../../PPTXFormat/Folder.cpp"
#include "../../../PPTXFormat/WrapperFile.cpp"
#include "../../../PPTXFormat/ViewProps.cpp"
#include "../../../PPTXFormat/Theme.cpp"
#include "../../../PPTXFormat/TableStyles.cpp"
#include "../../../PPTXFormat/SlideMaster.cpp"
#include "../../../PPTXFormat/SlideLayout.cpp"
#include "../../../PPTXFormat/Slide.cpp"
#include "../../../PPTXFormat/PresProps.cpp"
#include "../../../PPTXFormat/NotesSlide.cpp"
#include "../../../PPTXFormat/NotesMaster.cpp"
#include "../../../PPTXFormat/Namespaces.cpp"
#include "../../../PPTXFormat/LegacyDiagramText.cpp"
#include "../../../PPTXFormat/HandoutMaster.cpp"
#include "../../../PPTXFormat/Core.cpp"
#include "../../../PPTXFormat/App.cpp"
#include "../../../PPTXFormat/Comments.cpp"
#include "../../../PPTXFormat/CommentAuthors.cpp"
