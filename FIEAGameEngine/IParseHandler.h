#pragma once
#include "ParseCoordinator.h"

namespace Fiea::GameEngine
{
	class IParseHandler
	{
	public:
		IParseHandler() = default;
		virtual ~IParseHandler() = default;

		/**
		 * @brief Initialize state for parse handlers;
		*/
		virtual void Initialize() {};

		/**
		 * @brief Used to reset handler state.
		*/
		virtual void Cleanup() {};

		/**
		 * @brief 
		 * Checks if the passed wrapper and Json object are of a type that this handler knows how to deal with, then handle that value.
		 * @param wrapper 
		 * @param key 
		 * @param value 
		 * @param isArray 
		 * @return 
		*/
		virtual bool Start(ParseCoordinator::Wrapper* wrapper, const std::string& key, const Json::Value& value, bool isArray) = 0;

		/**
		 * @brief Intended for helpers that need custom functionality after a successful Start() call - mainly the scope handler.
		 * @param wrapper 
		 * @param key 
		 * @return 
		*/
		virtual bool End(ParseCoordinator::Wrapper* wrapper, const std::string& key) { return true; };

	};




}