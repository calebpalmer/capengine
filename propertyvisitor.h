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
		virtual void visitList(Property& property) = 0;
	};

}

/** 
 \fn PropetyVisitor::visitString()
 \brief Visitis a string property
 \param property
   The property
*/

/** 
 \fn PropetyVisitor::visitInt64()
 \brief Visitis an int64 property
 \param property
   The property
*/

/** 
 \fn PropetyVisitor::visitUInt64()
 \brief Visitis an uint64 property
 \param property
   The property
*/

/** 
 \fn PropetyVisitor::visitInt32()
 \brief Visitis an int32 property
 \param property
   The property
*/

/** 
 \fn PropetyVisitor::visitUInt32()
 \brief Visitis an uint32 property
 \param property
   The property
*/

/** 
 \fn PropetyVisitor::visitFloat32()
 \brief Visitis an float32 property
 \param property
   The property
*/

/** 
 \fn PropetyVisitor::visitFloat64()
 \brief Visitis an float64 property
 \param property
   The property
*/

#endif // PROPERTYVISITOR_H
