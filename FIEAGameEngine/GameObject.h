#pragma once
#include "Attributed.h"
#include <array>
#include "ScopeFactory.h"



namespace Fiea::GameEngine
{
	struct GameTime;
	class Action;

	class Transform
	{
	public:
		glm::vec4 position;
		glm::vec4 rotation;
		glm::vec4 scale;
	};


	class GameObject : public Attributed 
	{
		RTTI_DECLARATIONS(GameObject, Attributed);

	public:
		MAKE_FACTORY(GameObject);

		GameObject() : Attributed(TypeIdClass()) {};
		~GameObject() = default;

		GameObject(IdType id) : Attributed(id) {};
		GameObject(const GameObject& rhs) = default;  
		GameObject(GameObject&& rhs) noexcept = default;  
		GameObject& operator=(const GameObject& rhs) = default;  
		GameObject& operator=(GameObject&& rhs) noexcept = default;  

		[[nodiscard]] GameObject* Clone() const override;

		/**
		 * @brief 
		 * Calls update on this GameObject's children, then calls UpdateSelf();
		 * @param gameTime 
		*/
		void Update(const GameTime& gameTime);

		/**
		 * @brief 
		 * Returns a copy of this GameObject's prescribed attributes
		 * @return 
		*/
		static std::vector<Signature> Signatures();		

		/**
		 * @brief 
		 * Adds passed key, GameObject ref to this object's hash map
		 * @param name 
		 * @param childObj 
		*/
		void AddChild(const std::string& name, GameObject* childObj);
		/**
		 * @brief 
		 * Returns ref to the child GameOBject stored in this object's map associated with the passed string key.
		 * Returns nullptr if the child does not exist in the map.
		 * @param name 
		 * @return 
		*/
		const GameObject* GetChild(const std::string& name) const;
		/**
		 * @brief 
		 * Removes child 
		 * Calls Orphan on child gameobject associated with passed string name, then removes it from this object's map.
		 * Reutnrs nullptr if child was not found.
		 * @param name 
		 * @return 
		*/
		[[nodiscard]] GameObject* RemoveChild(const std::string& name) ;

		/**
		 * @brief 
		 * Mutable variant of GetTransform, allows altering member vec4s
		 * @return 
		*/
		Transform& GetTransform() { return _transform; };
		/**
		 * @brief
		 * Const variant of GetTransform, does not allow for changes to this object's transform
		 * @return
		*/
		const Transform& GetTransform() const { return _transform; };
		
		/**
		 * @brief 
		 * Returns a pointer to this object's actions datum. Retruns nullptr if that datum does not exist - wrapper for Find()
		 * @return 
		*/
		const Datum* Actions() const;

		/**
		 * @brief 
		 * 
		 * @param actionName 
		 * @param action 
		*/
		void CreateAction(const std::string& actionName, const std::string& actionClass);
		void AddAction(const std::string& actionName, Action* action);


		////test method
		//bool WasUpdateCalled() const { return bUpdateCalled; };

		

	protected:
		bool bUpdateCalled = false;
		Transform _transform{};

	private:	
		std::unordered_map<std::string, GameObject*> _childGameObjectMap;
		std::unordered_map<std::string, Action*> _actionMap;

		virtual void UpdateSelf() { bUpdateCalled = true; };
	};


}