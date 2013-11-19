#pragma once

#include "IRunItem.h"
#include "CSymbolOperand.h"
#include "Constants.h"

namespace AVSDocFileFormat
{
	class FootnoteReference: public IRunItem
	{
	private:
		static const byte CFSpec = 1;
		short aFtnIdx;

		wstring text;
		list<Prl> properties;

	public:
		explicit FootnoteReference( short _aFtnIdx = 0, unsigned short _ftc = 0, WCHAR _xchar = 0x0000 ):
		aFtnIdx(_aFtnIdx)
		{
			if ( this->aFtnIdx == 0 )
			{
				//!!!TODO!!!
				this->properties.push_back( Prl( (short)DocFileFormat::sprmCSymbol, (byte*)CSymbolOperand( _ftc, _xchar ) ) );
			}
			else
			{
				this->text = wstring( &TextMark::AutoNumberedFootnoteReference );
				this->properties.push_back( Prl( (short)DocFileFormat::sprmCFSpec, (byte*)(&CFSpec) ) );
			}
		}

		FootnoteReference( const FootnoteReference& _footnoteReference ):
		aFtnIdx(_footnoteReference.aFtnIdx), text(_footnoteReference.text), properties(_footnoteReference.properties)
		{
		}

		short GetIndex() const
		{
			return this->aFtnIdx;
		}

		virtual ~FootnoteReference()
		{
		}

		virtual IVirtualConstructor* New() const
		{
			return new FootnoteReference();
		}

		virtual IVirtualConstructor* Clone() const
		{
			return new FootnoteReference(*this);
		}

		virtual unsigned long GetTextSize() const
		{
			return this->text.size();
		}

		virtual wstring GetAllText() const
		{
			return this->text;
		}

		virtual vector<Prl> GetRunProperties() const
		{
			vector<Prl> prls;

			for ( list<Prl>::const_iterator iter = this->properties.begin(); iter != this->properties.end(); iter++ )
			{
				prls.push_back( *iter );
			}

			return prls;
		}

		virtual unsigned int PrlSize() const
		{
			return (unsigned int)properties.size();
		}
	};
}