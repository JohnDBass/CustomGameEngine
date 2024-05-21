
#include "pch.h"
#include "IParseHandler.h"
#include "ParseCoordinator.h"
#include <iostream>
#include <fstream>


namespace Fiea::GameEngine
{
    RTTI_DEFINITIONS(ParseCoordinator::Wrapper);


    size_t ParseCoordinator::AddHandler(const shared_ptr<IParseHandler>& handler)
    {
        handler->Initialize();
        _handlerList[_handlerIndex++] = handler;
        return (_handlerIndex - 1);
    }


    void ParseCoordinator::RemoveHandler(size_t handlerIndex)
    {
        if (handlerIndex > _handlerList.size() || _handlerList.find(handlerIndex) == _handlerList.end())
            throw std::exception("Invalid index passed to RemoveHandler");

        _handlerList[handlerIndex]->Cleanup();
        _handlerList.erase(handlerIndex); 
    }


    bool ParseCoordinator::DeserializeObject(const std::string& data)
    {
        //deserialize json into a Json cpp obj
        Json::CharReaderBuilder builder;
        Json::CharReader* reader = builder.newCharReader();

        Json::Value root;
        std::string errors;

        //this method is used to catch invalid json syntax
        bool parsingSuccessful = reader->parse(data.c_str(), data.c_str() + data.size(), &root, &errors);
        if (!parsingSuccessful)
        {
            std::cout << data << std::endl;
            std::cout << errors << std::endl;
            return false;
        }

        ParseMembers(root);

        delete(reader);
        for (auto handler : _handlerList)
        {
            handler.second->Cleanup();
        }
        return true;
    }


    bool ParseCoordinator::DeserializeObjectFromFile(const std::string& fileName)
    {
        std::ifstream fileStream(fileName);
        return DeserializeObjectFromStream(fileStream);
    }


    bool ParseCoordinator::DeserializeObjectFromStream(std::istream& stream)
    {
        std::string string(std::istreambuf_iterator<char>(stream), {});
        return DeserializeObject(string);
    }


    void ParseCoordinator::ParseMembers(const Json::Value& obj)
    {
        for (auto iter = obj.begin(); iter != obj.end(); ++iter)
        {
            Parse(iter.name(), obj[iter.name()]);
        }

    }


    void ParseCoordinator::Parse(std::string key, const Json::Value& obj)
    {     
        for (auto iter = _handlerList.begin(); iter != _handlerList.end(); ++iter)
        {
            if (iter->second->Start(&_myWrapper, key, obj, obj.isArray()))
            {
                if (obj.isObject())
                    ParseMembers(obj);


                if (obj.isArray() && obj[0].isObject())
                {
                    for (const Json::Value& obj : obj)
                    {
                        _myWrapper.IncrementDepth();
                        ParseMembers(obj);
                    }
                }

                iter->second->End(&_myWrapper, key);
                break;
            }
        }
    }




}




