#pragma once
#include "Scope.h"
#include "Signature.h"

namespace Fiea::GameEngine
{
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

	public:
		/**
		 * @brief 
		 * An Attributed class should not exist without its RTTI type declared
		*/
		Attributed() = delete;
		
		virtual [[nodiscard]] Attributed* Clone() const override = 0;

		/**
		 * @brief 
		 * Wrapper for Scope::Find(), returns bool indicating whether this object holds a datum associated with key str
		 * @param str 
		 * map key 
		 * @return 
		*/
		bool IsAttribute(const std::string& str) const;

		/**
		 * @brief 
		 * Returns bool indicating whether the passed string is the name of one of this class's signatures
		 * @param str 
		 * map key
		 * @return 
		*/
		bool IsPrescribedAttribute(const std::string& str) const;

		/**
		 * @brief 
		 * Returns bool indicating if this object contains a datum associated with the string key and that datum is not one of this class's signatures
		 * @param str 
		 * @return 
		*/
		bool IsAuxiliaryAttribute(const std::string& str) const;
		
		/**
		 * @brief 
		 * Thin wrapper for Scope::Append(), appends datum to this object at key string if that string is not one of this class's signatures
		 * @param  
		 * @return 
		 * Reference to new appended datum, or existing datum at that string key
		*/
		Datum& AppendAuxiliaryAttribute(const std::string&);

	protected:
		/* All copy constructors/assignment ops are thin wrappers for AppendAttributes(). */
		Attributed(size_t type);
		Attributed(const Attributed& other);
		Attributed(Attributed&& other) noexcept;
		Attributed& operator=(const Attributed& other);
		Attributed& operator=(Attributed&& other) noexcept;
		virtual ~Attributed() = default;

		/**
		 * @brief
		 * Appends external Datums to this class matching this class's prescribed attributes.
		 * In the case of copy or assignment this method will also re-target the external Datum pointers to this Attributed class.
		*/
		void AppendAttributes(size_t typeID);

	private:
		size_t _mTypeID;

		
	};
}