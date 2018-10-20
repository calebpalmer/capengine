#ifndef CAPENGINE_PROPERTYVISITOR_H
#define CAPENGINE_PROPERTYVISITOR_H

namespace CapEngine {
	//! forward declaration
	class Property;
	
	class PropertyVisitor {
	public:
		virtual ~PropertyVisitor() = default;
		
		virtual void visitString(Property& property) = 0;
		virtual void visitInt64(Property& property) = 0;
		virtual void visitUInt64(Property& property) = 0;
		virtual void visitInt32(Property& property) = 0;
		virtual void visitUInt32(Property& property) = 0;
		virtual void visitFloat32(Property& property) = 0;
		virtual void visitFloat64(Property& property) = 0;
	};

}

#endif // PROPERTYVISITOR_H
