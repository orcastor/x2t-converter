#ifndef CELLIPSE_H
#define CELLIPSE_H


#include "CObjectBase.h"

namespace SVG
{
	class CEllipse : public CObjectBase
	{
	public:
		CEllipse(CObjectBase* pParent = NULL);

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer, const CGeneralStyle* pBaseStyle) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath, const CGeneralStyle* pBaseStyle) const override;

		double m_dCx;
		double m_dCy;
		double m_dRx;
		double m_dRy;
	};
}

#endif // CELLIPSE_H
