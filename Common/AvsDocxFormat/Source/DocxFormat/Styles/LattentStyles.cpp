
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../Styles.h"


namespace OOX
{

	Styles::LattentStyles::LattentStyles()
	{
	}


	Styles::LattentStyles::~LattentStyles()
	{
	}
	

	Styles::LattentStyles::LattentStyles(const XML::XNode& node)
	{
		fromXML(node);
	}


	const Styles::LattentStyles& Styles::LattentStyles::operator =(const XML::XNode& node)
	{
		fromXML(node);
		return *this;
	}


	void Styles::LattentStyles::fromXML(const XML::XNode& node)
	{
		const XML::XElement element(node);
		DefLockedState		=	element.attribute("defLockedState").value();
		DefUIPriority			=	element.attribute("defUIPriority").value();
		DefSemiHidden			=	element.attribute("defSemiHidden").value();
		DefUnhideWhenUsed	=	element.attribute("defUnhideWhenUsed").value();
		DefQFormat	=	element.attribute("defQFormat").value();
		Count				=	element.attribute("count").value();
		XML::Fill(LsdExceptions, element);
	}


	const XML::XNode Styles::LattentStyles::toXML() const
	{
		return 
			XML::XElement(ns.w + "latentStyles",
				XML::XAttribute(ns.w + "defLockedState", DefLockedState) +
				XML::XAttribute(ns.w + "defUIPriority", DefUIPriority) +
				XML::XAttribute(ns.w + "defSemiHidden", DefSemiHidden) +
				XML::XAttribute(ns.w + "defUnhideWhenUsed", DefUnhideWhenUsed) +
				XML::XAttribute(ns.w + "defQFormat", DefQFormat) + 
				XML::XAttribute(ns.w + "count", Count) +
				XML::Write(LsdExceptions)
			);
	}

} // namespace OOX