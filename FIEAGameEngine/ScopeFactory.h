#pragma once
#include <unordered_map>
#include <string>
#include <memory>

namespace Fiea::GameEngine
{
	class Scope;

	class ScopeFactory
	{
	public:
		/**
		 * @brief 
		 * Returns a pointer to a newly default constructed object of this factory's owning type
		 * @return 
		*/
		[[nodiscard]] virtual Scope* Create() const = 0; 
		virtual std::string ClassName() const = 0;
	};


	class ScopeFactoryManager
	{
	public:
		

		/**
		 * @brief 
		 * Given a class name string, return a new object of that type by calling Create() on that class's associated factory.
		 * @param className 
		 * @return 
		*/
		[[nodiscard]] static Scope* Create(const std::string& className);

		/**
		 * @brief 
		 * Stores the passed factory reference in this class's map using the factory's ClassName() as its key.
		 * @param factoryPtr 
		*/
		static void Add(std::unique_ptr<ScopeFactory>&& factoryPtr);


	private:
		inline static std::unique_ptr<std::unordered_map<std::string, std::unique_ptr<ScopeFactory>>> _factories = nullptr;

		/**
		 * @brief
		 * Given a string className, return a pointer to the ScopeFactory that matches that key in this manager's hash map.
		 * Returns a nullptr if the map does not contain a pair with that class name key.
		 * @param className
		 * @return
		*/
		static const ScopeFactory* Find(const std::string& className);

		/**
		 * @brief
		 * Given a reference to a concrete factory, remove it from the this manager's hash map. Will result in that factory's destruction.
		 * @param factoryKey
		*/
		static void Remove(const std::string& factoryKey);
	};


#pragma region FactoryMacro

#define MAKE_FACTORY(_Concrete) \
		class _Concrete##Factory final : public ScopeFactory { \
		public: \
			[[nodiscard]] Scope* Create() const override { \
				return new _Concrete(); \
			} \
			std::string ClassName() const override { \
				return #_Concrete; \
			} \
		};

#pragma endregion FactoryMacro


}





