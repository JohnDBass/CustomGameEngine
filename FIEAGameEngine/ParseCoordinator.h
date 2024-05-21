#pragma once
#include <map>
#include <memory>
#include <json/json.h> 
#include "RTTI.h"

using std::shared_ptr;


namespace Fiea::GameEngine
{
    class IParseHandler;

	class ParseCoordinator
	{
        

    public:
        class Wrapper : public RTTI
        {
            RTTI_DECLARATIONS(ParseCoordinator::Wrapper, RTTI);

        public:
            Wrapper() = default;
            virtual ~Wrapper() = default;

            virtual void IncrementDepth() { ++_depth; };
            virtual void DecrementDepth() { --_depth; };

            virtual size_t Depth() const { return _depth; }


        protected:
            size_t _depth = 1;

        };



    public:
        ParseCoordinator() = delete;
        /**
         * @brief The intended method for constructing a ParseCoordinator. Requires a ParseCoordinator::Wrapper reference;
         * @param myWrapper 
        */
        ParseCoordinator(Wrapper& myWrapper) : _myWrapper(myWrapper) {};
        ~ParseCoordinator() = default;

        ParseCoordinator(const ParseCoordinator& other) = delete;
        ParseCoordinator(ParseCoordinator&& other) = delete;
        ParseCoordinator& operator=(const ParseCoordinator& other) = delete;
        ParseCoordinator& operator=(ParseCoordinator&& other) = delete;

        /**
         * @brief Adds the passed IParseHandler reference in the coordinator's handler map. Returns the map key of the passed handler.
         * @param handler 
         * @return 
        */
        size_t AddHandler(const shared_ptr<IParseHandler>& handler);
        
        /**
         * @brief Calls Cleanup() on the handler associated with the passed size_t key, then removes that handler from the map.
         * @param handlerIndex 
        */
        void RemoveHandler(size_t handlerIndex);

        /**
         * @brief Checks that the passed string is a valid JSON string, then begins parsing the JSON string.
         * @param data 
         * @return 
        */
        bool DeserializeObject(const std::string& data);        

        /**
         * @brief Thin wrapper for DeserializeObject(string) method.
         * @param filename 
         * @return 
        */
        bool DeserializeObjectFromFile(const std::string& filename);
        /**
         * @brief Thin wrapper for DeserializeObject(string) method.
         * @param filename
         * @return
        */
        bool DeserializeObjectFromStream(std::istream& stream);

        /**
         * @brief Returns the wrapper that this coordinator was constructed with.
         * @return 
        */
        Wrapper& GetWrapper() const { return _myWrapper; };

        /**
         * @brief Returns the number of handler references held by the coordinator's handler map; 
         * @return 
        */
        size_t GetHandlerMapSize() const { return _handlerList.size(); }


    private:
        size_t _handlerIndex = 1;
        std::map<size_t, shared_ptr<IParseHandler>> _handlerList;

        Wrapper& _myWrapper;

        /**
         * @brief Iterates through each member of the passed json object, calling Parse on each key value json pair.
         * @param obj 
        */
        void ParseMembers(const Json::Value& obj);

        /**
         * @brief Walk through the list of handlers, calling StartHandler on each until one returns true. 
         * Also contains logic for recursively handling nested json objects.
         * @param key 
         * @param obj 
        */
        void Parse(std::string key, const Json::Value& obj);

	};

}