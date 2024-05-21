#pragma once

#define GLM_ENABLE_EXPERIMENTAL
//provides acess to experimental glm string_cast

#include <string>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp" 



namespace Fiea::GameEngine
{
	class Scope;
	class RTTI;

	enum DatumType
	{
		Unknown,
		Int,
		Float,
		String,
		Vector,
		Matrix,
		Table,
		Pointer

	};

	class Datum
	{
	public:
		Datum();
		Datum(const Datum& other);
		Datum(Datum&& rhsOther) noexcept;
		virtual ~Datum();
		Datum& operator=(const Datum& other);
		Datum& operator=(Datum&& rhsOther) noexcept;

		/* Scalar constructors, used to constuct Datums that contain one data member*/
		Datum(int val);
		Datum(float val);
		Datum(const std::string& val);
		Datum(const glm::vec4& val);
		Datum(const glm::mat4x4& val);

		/* Rvalue scalar constructors*/
		Datum(std::string&& rhsVal);
		Datum(glm::vec4&& rhsVal);
		Datum(glm::mat4x4&& rhsVal);

		template<class T>
		Datum(std::initializer_list<T> list);
		
		/* Scalar assignment operators, do not allow a Datum to change its type (if not Type::Unknown)
		 * Will Clear() the contents of this Datum if this already contains data, does not affect capacity */
		Datum& operator=(int val);
		Datum& operator=(float val);
		Datum& operator=(const std::string& val);
		Datum& operator=(const glm::vec4& val);
		Datum& operator=(const glm::mat4x4& val);

		/* rvalue scalar assignment ops */
		Datum& operator=(std::string&& rhsVal);
		Datum& operator=(glm::vec4&& rhsVal);
		Datum& operator=(glm::mat4x4&& rhsVal);
		
		/**
		 * @brief 
		 * Pushes the passed int to the last index of this Datum's data, triggering an automatic resize if necessary.
		 * @param val 
		*/
		void PushBack(int val);
		/**
		 * @brief
		 * Pushes the passed float to the last index of this Datum's data, triggering an automatic resize if necessary.
		 * @param val
		*/
		void PushBack(float val);
		/**
		 * @brief
		 * Pushes the passed string to the last index of this Datum's data, triggering an automatic resize if necessary.
		 * @param val
		*/
		void PushBack(const std::string& val);
		/**
		 * @brief
		 * Pushes the passed vector to the last index of this Datum's data, triggering an automatic resize if necessary.
		 * @param val
		*/
		void PushBack(const glm::vec4& val);
		/**
		 * @brief
		 * Pushes the passed matrix to the last index of this Datum's data, triggering an automatic resize if necessary.
		 * @param val
		*/
		void PushBack(const glm::mat4x4& val);
		/**
		 * @brief
		 * Pushes the passed scope ptr to the last index of this Datum's data, triggering an automatic resize if necessary.
		 * @param val
		*/
		void PushBack(Scope* val);
		/**
		 * @brief
		 * Pushes the passed rtti ptr to the last index of this Datum's data, triggering an automatic resize if necessary.
		 * @param val
		*/
		void PushBack(RTTI* val);

		/**
		 * @brief
		 * Non-const variant of PushBack - Pushes the passed string to the last index of this Datum's data, triggering an automatic resize if necessary.
		 * @param val
		*/
		void PushBack(std::string&& rhsVal);
		/**
		 * @brief
		 * Non-const variant of PushBack - Pushes the passed vector to the last index of this Datum's data, triggering an automatic resize if necessary.
		 * @param val
		*/
		void PushBack(glm::vec4&& rhsVal);
		/**
		 * @brief
		 * Non-const variant of PushBack - Pushes the passed matrix to the last index of this Datum's data, triggering an automatic resize if necessary.
		 * @param val
		*/
		void PushBack(glm::mat4x4&& rhsVal);

		/**
		 * @brief 
		 * Removes the final element of this Datum's data from the container, reducing this Datum's size but not capacity.
		*/
		void PopBack();

		bool operator==(const Datum& rhs) const;
		bool operator!=(const Datum& rhs) const;		

		DatumType Type() const;
		size_t Size() const;
		size_t Capacity() const;
		bool IsExternal() const;

		void Reserve(size_t capacity);
		void Clear();
		void Resize(size_t size);

		/**
		 * @brief Returns the int value at passed index of DatumData.
		 * @param idx 
		 * @return 
		*/
		int GetInt(size_t idx = 0) const;
		/**
		 * @brief Returns the float value at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		float GetFloat(size_t idx = 0) const;
		/**
		 * @brief Returns the string value at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		const std::string& GetString(size_t idx = 0) const;
		/**
		 * @brief Returns the vector value at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		const glm::vec4& GetVector(size_t idx = 0) const;
		/**
		 * @brief Returns the matrix value at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		const glm::mat4x4& GetMatrix(size_t idx = 0) const;
		/**
		 * @brief Returns the scope ptr at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		Scope* GetTable(size_t idx = 0) const;
		/**
		 * @brief Returns the RTTI ptr at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		RTTI* GetRTTI(size_t idx = 0) const;
		/**
		 * @brief Set this Datum's type and reserve space for 1 owned member of that type. Supports int, float, string, vector, and matrix types.
		 * @param  
		*/
		void SetType(DatumType type);

		/**
		 * @brief Sets the int value at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		void SetInt(int val, size_t idx = 0);
		/**
		 * @brief Sets the float value at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		void SetFloat(float val, size_t idx = 0);
		/**
		 * @brief Sets the string value at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		void SetString(const std::string& val, size_t idx = 0);
		/**
		 * @brief Sets the vector value at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		void SetVector(const glm::vec4& val, size_t idx = 0);
		/**
		 * @brief Sets the matrix value at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		void SetMatrix(const glm::mat4x4& val, size_t idx = 0);
		/**
		 * @brief Non-const variant of SetString. Sets the string value at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		void SetString(std::string&& val, size_t idx = 0);
		/**
		 * @brief Non-const variant of SetVector. Sets the vector value at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		void SetVector(glm::vec4&& val, size_t idx = 0);
		/**
		 * @brief Non-const variant of SetMatrix. Sets the matrix value at passed index of DatumData.
		 * @param idx
		 * @return
		*/
		void SetMatrix(glm::mat4x4&& val, size_t idx = 0);

		/**
		 * @brief 
		 * Add passed integer to the int stored at passed index of this datum.
		 * Index defaults to 0
		 * @param intToAdd 
		 * @param idx 
		*/
		void AddInt(int intToAdd, size_t idx = 0);

		/**
		 * @brief 
		 * Add passed integer to the int stored at passed index of this datum.
		 * Index defaults to 0
		 * @param floatToAdd 
		 * @param idx 
		*/
		void AddFloat(float floatToAdd, size_t idx = 0);
		/**
		 * @brief Returns the int value at passed index of DatumData as a string.
		 * @param idx
		 * @return
		*/
		std::string GetIntAsString(size_t idx = 0) const;
		/**
		 * @brief Returns the float value at passed index of DatumData as a string.
		 * @param idx
		 * @return
		*/
		std::string GetFloatAsString(size_t idx = 0) const;
		/**
		 * @brief Returns the vector value at passed index of DatumData as a string.
		 * @param idx
		 * @return
		*/
		std::string GetVectorAsString(size_t idx = 0) const;
		/**
		 * @brief Returns the matrix value at passed index of DatumData as a string.
		 * @param idx
		 * @return
		*/
		std::string GetMatrixAsString(size_t idx = 0) const;

		/**
		 * @brief Sets the int value at passed index of DatumData with a passed string.
		 * @param idx
		 * @return
		*/
		void SetIntAsString(const std::string& str, size_t idx = 0);
		/**
		 * @brief Sets the float value at passed index of DatumData with a passed string.
		 * @param idx
		 * @return
		*/
		void SetFloatAsString(const std::string& str, size_t idx = 0);
		/**
		 * @brief Sets the vector value at passed index of DatumData with a passed string.
		 * @param idx
		 * @return
		*/
		void SetVectorAsString(const std::string& str, size_t idx = 0);
		/**
		 * @brief Sets the matrix value at passed index of DatumData with a passed string.
		 * @param idx
		 * @return
		*/
		void SetMatrixAsString(const std::string& str, size_t idx = 0);

		/**
		 * @brief 
		 * Wraps the passed external data pointer with this Datum. This Datum will not own that external memory, but can get and set the data at that address. 
		 * @tparam T 
		 * @param arr 
		 * @param size 
		*/
		template<class T>
		inline void SetStorage(T* arr, size_t size = 1);

		void SetStorage(void* ptr, DatumType type, size_t size);

		Scope& operator[](std::uint32_t index) { return *GetTable(index); }

		/**
		 * @brief 
		 * Will remove the scope pointer at passed index from this Datum's table data.
		 * @param idx 
		*/
		void EraseScope(size_t idx);

	private:
		DatumType _mType = DatumType::Unknown;

		union DatumData
		{
			void* vPtr;
			int* i;
			float* f;
			std::string* str;
			glm::vec4* gvec;
			glm::mat4x4* gmat;
			Scope** scp;
			RTTI** rtti;

		} _mData{};


		size_t _mSize = 0;
		size_t _mCapacity = 0;
		bool _bExternalDatum = false;

		size_t geo_cap_growth(size_t minCap) const;
	};	

}

#include "Datum.inl"