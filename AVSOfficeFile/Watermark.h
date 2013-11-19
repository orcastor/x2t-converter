#pragma once
#include "stdafx.h"
#include <math.h>
#include "XmlUtils.h"
#include "MediaFormatDefine.h"
#include "ImageStudioUtils.h"

#include "GdiPlusEx.h"

#define ONE_INCH 2.54

#define MM_TO_PIX( Value, DPI )		long( Value * DPI / (10 * ONE_INCH ) )
#define PIX_TO_MM( Value, DPI )		float( 1.0 * Value * (10 * ONE_INCH ) / DPI )

#define DRAW_WIDTH( DPI )  MM_TO_PIX( 432, DPI )	//минимальые размеры картинки для конструирования вотермарка
#define DRAW_HEIGHT( DPI ) MM_TO_PIX( 559, DPI )	//перевернутый A2 - 432 × 559 (mm)

class Watermark
{
public: typedef enum{ ax_none, ax_left, ax_center, ax_right } AlignX;
public: typedef enum{ ay_none, ay_top, ay_center, ay_bottom } AlignY;
public: typedef enum{ rr_none, rr_ltrb, rr_rtlb } RelativeRotate;

public: AlignX m_eAlingX;
public: AlignY m_eAlingY;

public: int m_nMarginLeft;
public: int m_nMarginTop;
public: int m_nMarginRight;
public: int m_nMarginBottom;

public: int m_nRotate;
public: bool m_bRelativeRotate;
public: int m_nRelRotataX1; // от 0 до 10
public: int m_nRelRotataX2;
public: int m_nRelRotataY1;
public: int m_nRelRotataY2;

public: int m_nDrawWidth;
public: int m_nDrawHeight;
public: int m_nDrawDipX;
public: int m_nDrawDipY;

public: BYTE* m_pDataCopy;	
public: Gdiplus::Bitmap* m_pBitmap;

public: Watermark()
		{
			m_pBitmap = NULL;
			m_pDataCopy = NULL;
			SetDefault();
		}
public: ~Watermark()
		{
			SetDefault();
		}
public: void SetDefault()
		{
			m_eAlingX = ax_center;
			m_eAlingY = ay_center;
			m_nMarginLeft = 0;
			m_nMarginTop = 0;
			m_nMarginRight = 0;
			m_nMarginBottom = 0;
			m_nRotate = 0;
			m_bRelativeRotate = false;
			m_nRelRotataX1 = 0;
			m_nRelRotataX2 = 10;
			m_nRelRotataY1 = 5;
			m_nRelRotataY2 = 5;

			m_nDrawDipX = 96;
			m_nDrawDipY = 96;
			m_nDrawWidth = DRAW_WIDTH( m_nDrawDipX );
			m_nDrawHeight = DRAW_HEIGHT( m_nDrawDipY );

			RELEASEOBJECT( m_pBitmap );
			RELEASEARRAYOBJECTS( m_pDataCopy );
		}
public: void SetSize( int nWidth, int nHeight )
		{
			if( nWidth > DRAW_WIDTH( m_nDrawDipX ) )
				m_nDrawWidth = nWidth;
			if( nWidth > DRAW_HEIGHT( m_nDrawDipY ) )
				m_nDrawHeight = nHeight;
		}
public: void SetDPI( int nDpiX, int nDpiY )
		{
			m_nDrawDipX = nDpiX;
			m_nDrawDipY = nDpiY;
		}
public: void ParseXml( XmlUtils::CXmlReader& oXmlReader )
		{
			SetDefault();

			XML::IXMLDOMNodePtr oNode;
			oXmlReader.GetNode( oNode );
			XmlUtils::CXmlWriter oXmlWriter;
			oXmlWriter.WriteNodeBegin( CString(oNode->nodeName.GetBSTR()), TRUE );
			//убираем аттрибуты font-angle, text-dir-...
			for( int i = 0; i < oNode->attributes->Getlength(); i++ )
			{
				XML::IXMLDOMNodePtr oSubNode = oNode->attributes->Getitem( i );
				if( -1 == CString( oSubNode->nodeName.GetBSTR() ).Find( _T("font-angle") ) &&
					-1 == CString( oSubNode->nodeName.GetBSTR() ).Find( _T("text-dir-x1") ) &&
					-1 == CString( oSubNode->nodeName.GetBSTR() ).Find( _T("text-dir-x2") ) &&
					-1 == CString( oSubNode->nodeName.GetBSTR() ).Find( _T("text-dir-y1") ) &&
					-1 == CString( oSubNode->nodeName.GetBSTR() ).Find( _T("text-dir-y2") ) )
					oXmlWriter.WriteAttribute( Utils::PrepareToXML( CString(oSubNode->nodeName.GetBSTR()) ), Utils::PrepareToXML( CString(oSubNode->nodeValue.bstrVal) ) );

			}
			oXmlWriter.WriteNodeEnd( CString(oNode->nodeName.GetBSTR()), TRUE, TRUE );

			CString sAlignType = oXmlReader.ReadNodeAttribute( _T("aligntype") );
			CString sRotate = oXmlReader.ReadNodeAttribute( _T("font-angle") );
			CString sX1 = oXmlReader.ReadNodeAttribute( _T("text-dir-x1") );
			CString sX2 = oXmlReader.ReadNodeAttribute( _T("text-dir-x2") );
			CString sY1 = oXmlReader.ReadNodeAttribute( _T("text-dir-y1") );
			CString sY2 = oXmlReader.ReadNodeAttribute( _T("text-dir-y2") );

			if( _T("") != sAlignType )
			{
				int nAlignType = Strings::ToInteger( sAlignType );
				switch( nAlignType )
				{
					case 0: m_eAlingX = ax_left;		m_eAlingY = ay_top; break;
					case 1: m_eAlingX = ax_center;		m_eAlingY = ay_top; break;
					case 2: m_eAlingX = ax_right;		m_eAlingY = ay_top; break;
					case 3: m_eAlingX = ax_left;		m_eAlingY = ay_center; break;
					case 4: m_eAlingX = ax_center;		m_eAlingY = ay_center; break;
					case 5: m_eAlingX = ax_right;		m_eAlingY = ay_center; break;
					case 6: m_eAlingX = ax_left;		m_eAlingY = ay_bottom; break;
					case 7: m_eAlingX = ax_center;		m_eAlingY = ay_bottom; break;
					case 8: m_eAlingX = ax_right;		m_eAlingY = ay_bottom; break;
				}
			}
			if( _T("") != sRotate )
				m_nRotate = Strings::ToInteger( sRotate );
			if( _T("") != sX1 && _T("") != sX2 && _T("") != sY1 && _T("") != sY2 )
			{
				m_bRelativeRotate = true;
				m_nRelRotataX1 = Strings::ToInteger( sX1 );
				m_nRelRotataX2 = Strings::ToInteger( sX2 );
				m_nRelRotataY1 = Strings::ToInteger( sY1 );
				m_nRelRotataY2 = Strings::ToInteger( sY2 );
			}
			XmlUtils::CXmlReader oXmlModifyReader;
			oXmlModifyReader.OpenFromXmlString( oXmlWriter.GetXmlString() );
			prepareBitmap( oXmlModifyReader );
		}
public: void DrawWatermark( AVSGraphics::IAVSRenderer* piCommandsRenderer, float fPageWidth, float fPageHeight )
		{
			if( NULL == m_pBitmap || fPageWidth <= 0 || fPageHeight <= 0 )
				return;

			float fMarginLeft = PIX_TO_MM( m_nMarginLeft, m_nDrawDipX );
			float fMarginTop = PIX_TO_MM( m_nMarginTop, m_nDrawDipY );
			float fMarginRight = PIX_TO_MM( m_nMarginRight, m_nDrawDipX );
			float fMarginBottom = PIX_TO_MM( m_nMarginBottom, m_nDrawDipY );

			float fImageWidth = PIX_TO_MM( m_pBitmap->GetWidth(), m_nDrawDipX );
			float fImageHeight = PIX_TO_MM( m_pBitmap->GetHeight(), m_nDrawDipY );
			if( fImageWidth <= 0 || fImageHeight <= 0 )
				return;
			float fPosX = 0;
			float fPosY = 0;
			switch( m_eAlingX )
			{
				case ax_left: fPosX = fMarginLeft; break;
				case ax_right: fPosX = fPageWidth - fMarginRight - fImageWidth; break;
				default: fPosX = fPageWidth/2 - fImageWidth / 2; break;
			}
			switch( m_eAlingY )
			{
				case ay_top: fPosY = fMarginTop; break;
				case ay_bottom: fPosY = fPageHeight - fMarginBottom - fImageHeight; break;
				default: fPosY = fPageHeight/2 - fImageHeight / 2; break;
			}
			double dNewAngle = 0;
			//RelativeRotate в приоритете над простым Rotate
			if( true == m_bRelativeRotate )
			{
				int nDx = m_nRelRotataX2 - m_nRelRotataX1;
				int nDy = m_nRelRotataY2 - m_nRelRotataY1;
				if( 0 != nDx )
					dNewAngle = 180 * atan( 1.0 * ( nDy / nDx ) * ( fPageHeight / fPageWidth ) ) / M_PI;
				else
				{
					if( nDy > 0 )
						dNewAngle = 90;
					else
						dNewAngle = -90;
				}
			}
			else if( 0 != m_nRotate )
				dNewAngle = m_nRotate;
			bool bIsClone = false;
			Gdiplus::Bitmap* pBitmap = NULL;
			////если размеры выходят за границу обрезаем битмап
			//if( fPosX < 0 || fPosY < 0 || fPosX + fImageWidth > fPageWidth  || fPosY + fImageHeight > fPageHeight )
			//{
			//	float fCropLeft = 0;  //обрезка в долях 0.2
			//	float fCropTop = 0;
			//	float fCropRight = 0;
			//	float fCropBottom = 0;
			//	//вычисляем величину crop
			//	if( fPosX < 0 )
			//		fCropLeft = fabs( fPosX ) / fImageWidth;
			//	if( fPosY < 0 )
			//		fCropTop = fabs( fPosY ) / fImageHeight;
			//	if( fPosX + fImageWidth > fPageWidth )
			//		fCropRight = ( fPosX + fImageWidth - fPageWidth ) / fImageWidth;
			//	if( fPosY + fImageHeight > fPageHeight )
			//		fCropBottom = ( fPosY + fImageHeight - fPageHeight ) / fPageHeight;
			//	int nSourceImageWidth = m_pBitmap->GetWidth();
			//	int nSourceImageHeight = m_pBitmap->GetHeight();
			//	//создаем crop bitmap
			//	Rect oRect( (int)(fCropLeft * nSourceImageWidth), (int)(fCropTop * nSourceImageHeight),
			//				nSourceImageWidth - (int)( (fCropLeft + fCropRight) * nSourceImageWidth),
			//				nSourceImageHeight - (int)( (fCropTop + fCropBottom) * nSourceImageHeight) );
			//	pBitmap = m_pBitmap->Clone( oRect, PixelFormat32bppARGB );
			//	bIsClone = true;
			//	//меняем параметры под новый битмап
			//	if( fPosX < 0 )
			//	{
			//		fImageWidth +=  fPosX;
			//		fPosX = 0;
			//	}
			//	if( fPosY < 0 )
			//	{
			//		fImageHeight += fPosY;
			//		fPosY = 0;
			//	}
			//	if( fPosX + fImageWidth > fPageWidth )
			//		fImageWidth = fPageWidth - fPosX;
			//	if( fPosY + fImageHeight > fPageHeight )
			//		fImageHeight = fPageHeight - fPosY;
			//}
			//else
				pBitmap = m_pBitmap;

			MediaCore::IAVSUncompressedVideoFrame* piFrame = NULL;
			BOOL bGdiToBitmap = ImageStudioUtils::GdiPlusBitmapToMediaData( pBitmap, (IUnknown**)&piFrame, TRUE );

			if( true == bIsClone )
				delete pBitmap;
			if( FALSE == bGdiToBitmap || NULL == piFrame )
				return;
			int nFrameWidth = piFrame->Width;
			float fFrameWidth = PIX_TO_MM( nFrameWidth, m_nDrawDipX );

			//выставляем параметры поворота
			piCommandsRenderer->SetCommandParams( dNewAngle, fPosX, fPosY, fImageWidth, fImageHeight, 0 );
			piCommandsRenderer->DrawImage( (IUnknown*)piFrame, fPosX, fPosY, fImageWidth, fImageHeight );
			//устанавливаем исходные параметры
			piCommandsRenderer->SetCommandParams( 0, -1, -1, -1, -1, 0 );
			RELEASEINTERFACE( piFrame );
		}
public: bool IsEmpty()
		{
			return NULL == m_pBitmap;
		}
private: void prepareBitmap( XmlUtils::CXmlReader& oXmlReader )
		{
			CString sDrawingXml = oXmlReader.ReadNodeXml();
			MediaCore::IAVSUncompressedVideoFrame* piFrame = NULL;
			long nMaxSize = m_nDrawWidth;
			if( m_nDrawHeight > nMaxSize )
				nMaxSize = m_nDrawHeight;
			UpdateImage( nMaxSize, nMaxSize, &piFrame );
			if( NULL == piFrame )
				return;

			ImageStudio::IImageTransformsPtr piTransform = NULL;
			piTransform.CreateInstance( __uuidof( ImageStudio::ImageTransforms ) );
			if( NULL == piTransform )
			{
				RELEASEINTERFACE( piFrame );
				return;
			}

			VARIANT vImage;
			vImage.vt = VT_UNKNOWN;
			vImage.punkVal = piFrame;
			piTransform->SetSource(0,vImage);
			BSTR bstrTransformXMl = sDrawingXml.AllocSysString();
			piTransform->SetXml(bstrTransformXMl );
			piTransform->Transform();
			piTransform->GetResult( 0, &vImage );
			SysFreeString( bstrTransformXMl );
			RELEASEINTERFACE( piFrame );
			if( NULL == vImage.punkVal )
				return;
			MediaCore::IAVSUncompressedVideoFrame* piFrameTransform = NULL;
			vImage.punkVal->QueryInterface( __uuidof( MediaCore::IAVSUncompressedVideoFrame), (void**)&piFrameTransform);
			RELEASEINTERFACE( vImage.punkVal );

			if( NULL == piFrameTransform )
				return;
			bool bFrameTransformFlip = false;
			if( piFrameTransform->ColorSpace & CSP_VFLIP )
				bFrameTransformFlip = true;
			int nWidthPix = piFrameTransform->Width;
			int nHeightPix = piFrameTransform->Height;


			//выделяем регион для копирования по прозрачности
			long nBufSize = piFrameTransform->BufferSize;
			long nStart = 3;
			long nEnd = nBufSize - 1;
			long nIndex = nStart;
			while( nIndex < nBufSize && piFrameTransform->Buffer[nIndex] == 0 )
				nIndex += 4;
			nStart = nIndex;
			nIndex = nEnd;
			while( nIndex > 0 && piFrameTransform->Buffer[nIndex] == 0 )
				nIndex -= 4;
			nEnd = nIndex;
			if( nEnd < nStart )
			{
				RELEASEINTERFACE( piFrameTransform );
				return;
			}


			int nRowStart = nStart / ( 4 * nWidthPix); //row начинаются с 0
			int nRowEnd = nEnd / ( 4 * nWidthPix);
			int nColStart = (nStart % ( 4 * nWidthPix) ); //collumn начинаются с 0
			int nColEnd = (nEnd % ( 4 * nWidthPix) );
			for( int i = nRowStart; i <= nRowEnd; i++ )
			{
				int nCurRowIndex = i*4*nWidthPix ;
				int nIndex =  3;
				while( nIndex <  4*nWidthPix && piFrameTransform->Buffer[ nCurRowIndex + nIndex ] == 0 )
					nIndex += 4;
				if( nIndex < nColStart )
					nColStart = nIndex;
				nIndex = 4 * nWidthPix - 1;
				while( nIndex > 0 && piFrameTransform->Buffer[ nCurRowIndex + nIndex ] == 0 )
					nIndex -= 4;
				if( nIndex > nColEnd )
					nColEnd = nIndex;
			}
			nColStart /=4;
			nColEnd /=4;
			if( nColEnd < nColStart )
			{
				RELEASEINTERFACE( piFrameTransform );
				return;
			}
			//Выставляем margin
			m_nMarginLeft = nColStart;
			m_nMarginRight = nWidthPix - nColEnd;
			if( piFrameTransform->ColorSpace & CSP_VFLIP )
			{
				m_nMarginTop = nHeightPix - nRowEnd;
				m_nMarginBottom = nRowStart;
			}
			else
			{
				m_nMarginTop = nRowStart;
				m_nMarginBottom = nHeightPix - nRowEnd;
			}

			//копируем данные в буфер
			int nResultWidth = nColEnd - nColStart + 1;
			int nResultHeight = nRowEnd - nRowStart + 1;
			m_pDataCopy = new BYTE[4*nResultWidth*nResultHeight];
			for( int i = 0; i < nResultHeight; i++ )
			{
				if( true == bFrameTransformFlip )
					memcpy( (m_pDataCopy + i*4*nResultWidth) ,(piFrameTransform->Buffer + 4*nColStart + (nRowStart+i)*4*nWidthPix ), 4*nResultWidth);
				else
					memcpy( (m_pDataCopy + i*4*nResultWidth) ,(piFrameTransform->Buffer + 4*nColStart + (nRowStart + nResultHeight - i )*4*nWidthPix ), 4*nResultWidth);
			}
			//создаем bitmap
			m_pBitmap = new Bitmap(nResultWidth, nResultHeight, 4*nResultWidth, PixelFormat32bppARGB, m_pDataCopy);
			RELEASEINTERFACE( piFrameTransform );

		}
 	private: void UpdateImage(int nWidthPix, int nHeightPix, MediaCore::IAVSUncompressedVideoFrame** piImage)
	{
		CoCreateInstance( __uuidof( MediaCore::CAVSUncompressedVideoFrame ), NULL ,CLSCTX_INPROC_SERVER, __uuidof( MediaCore::IAVSUncompressedVideoFrame ), (void **)piImage  );

		(*piImage)->Width = nWidthPix;
		(*piImage)->Height = nHeightPix;
		(*piImage)->put_Stride(0, nWidthPix * 4);

		(*piImage)->AspectRatioX = nWidthPix;
		(*piImage)->AspectRatioY = nHeightPix;

		(*piImage)->DataSize = 4 * nWidthPix * nHeightPix;

		(*piImage)->ColorSpace = CSP_BGRA | CSP_VFLIP;
		(*piImage)->AllocateBuffer(-1);

		memset( (*piImage)->Buffer, 255, (*piImage)->BufferSize );
		for( int i = 3; i < (*piImage)->BufferSize; i += 4 )
			(*piImage)->Buffer[ i ] = 0;
	}
};

class WatermarkManager
{
public: int m_nWidth;
public: int m_nHeight;
public: int m_nDipX;
public: int m_nDipY;
private: CAtlArray<Watermark> m_aWatermarks;
public: WatermarkManager()
		{
			SetDefault();
		}

public: void ParseXml( CString sXml )
		{
			Clear();
			SetDPI( m_nDipX, m_nDipY );
			SetSize( m_nWidth, m_nHeight );
			XmlUtils::CXmlReader oXmlReader;
			oXmlReader.OpenFromXmlString( sXml );
			oXmlReader.ReadNodeList( _T("*") );
			for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
			{
				XML::IXMLDOMNodePtr oNode;
				oXmlReader.GetNode( i, oNode );
				XmlUtils::CXmlReader oXmlSubReader;
				oXmlSubReader.OpenFromXmlNode( oNode );

				m_aWatermarks.Add();
				m_aWatermarks[ m_aWatermarks.GetCount() - 1 ].ParseXml( oXmlSubReader );
				if( m_aWatermarks[ i ].IsEmpty() )
					m_aWatermarks.RemoveAt( m_aWatermarks.GetCount() - 1 );
			}
		}
public: void DrawWatermark( AVSGraphics::IAVSRenderer* piCommandsRenderer, float fPageWidth, float fPageHeight )
		{
			for( int i = 0; i < (int)m_aWatermarks.GetCount(); i++ )
				m_aWatermarks[ i ].DrawWatermark( piCommandsRenderer, fPageWidth, fPageHeight );
		}
public: void SetSize( int nWidth, int nHeight )
		{
			for( int i = 0; i < (int)m_aWatermarks.GetCount(); i++ )
				m_aWatermarks[i].SetSize( nWidth, nHeight );
		}
public: void SetDPI( int nDpiX, int nDpiY )
		{
			for( int i = 0; i < (int)m_aWatermarks.GetCount(); i++ )
				m_aWatermarks[i].SetDPI( nDpiX, nDpiY );
		}
public: bool IsEmpty()
		{
			return 0 == m_aWatermarks.GetCount();
		}
public: long GetCount()
		{
			return m_aWatermarks.GetCount();
		}
private: void SetDefault()
		 {
			m_nWidth = 0;
			m_nHeight = 0;
			m_nDipX = 96;
			m_nDipY = 96;
		 }
private: void Clear()
		 {
			m_aWatermarks.RemoveAll();
		 }
};


[
	object,
	uuid("362225b0-ac21-11df-94e2-0800200c9a66"),
	dual,	helpstring("IAVSOfficeFileTemplate Interface"),
	pointer_default(unique)
]
__interface IAVSOfficeWatermark
{
	[id(10)] HRESULT ParseXml( [in] BSTR bstrXML );
	[id(20)] HRESULT DrawWatermark( [in] IUnknown* punkRenderer, [in] FLOAT fPageWidth, [in] FLOAT fPageHeight );
	[id(30)] HRESULT SetSize( [in]LONG nWidth, [in]LONG nHeight );
	[id(40)] HRESULT SetDPI( [in]LONG nDpiX, [in]LONG nDpiY );
	[id(50)] HRESULT GetCount( [out, retval] LONG* plVal );
};

[
	coclass,
	default(IAVSOfficeWatermark),
	threading(apartment),
	vi_progid("AVSOfficeFile.AVSDocume"),
	progid("AVSOfficeFile.AVSDocu.1"),
	version(1.0),
	uuid("4439ef70-ac21-11df-94e2-0800200c9a66"),
	helpstring("AVSOfficeFile Class")
]
class ATL_NO_VTABLE CAVSOfficeWatermark : public IAVSOfficeWatermark
{
public:
	CAVSOfficeWatermark()
	{
	}
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
public:
	STDMETHOD(ParseXml)( BSTR bstrXML )
	{
		m_oWatermarkManager.ParseXml( bstrXML );
		return S_OK;
	}
	STDMETHOD(DrawWatermark)( IUnknown* punkRenderer, float fPageWidth, float fPageHeight )
	{
		AVSGraphics::IAVSRenderer* piCommandsRenderer = NULL;
		if( NULL != punkRenderer && SUCCEEDED( punkRenderer->QueryInterface( __uuidof( AVSGraphics::IAVSRenderer ), (void**)&piCommandsRenderer ) ) )
		{
			m_oWatermarkManager.DrawWatermark( piCommandsRenderer, fPageWidth, fPageHeight );
			RELEASEINTERFACE( piCommandsRenderer );
		}
		return S_OK;
	}
	STDMETHOD(SetSize)( LONG nWidth, LONG nHeight )
	{
		m_oWatermarkManager.SetSize( nWidth, nHeight );
		return S_OK;
	}
	STDMETHOD(SetDPI)( LONG nDpiX, LONG nDpiY )
	{
		m_oWatermarkManager.SetDPI( nDpiX, nDpiY );
		return S_OK;
	}
	STDMETHOD(GetCount)( LONG* plVal )
	{
		if( NULL == plVal )
			return S_FALSE;
		(*plVal) = m_oWatermarkManager.GetCount();
		return S_OK;
	}
private:
	WatermarkManager m_oWatermarkManager;
};
