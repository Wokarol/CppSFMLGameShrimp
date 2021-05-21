#pragma once

#include <actorSystem/actorInterfaces.h>
#include <actorSystem/actorHandle.h>
#include <actorSystem/group.h>
#include <string>
#include <console.h>

namespace wok
{
    class world;

    class Actor
    {
    public:
        std::shared_ptr<Group> group;
        std::string name;

        virtual void start() {};
        virtual ~Actor() = default;
        ActorHandle<Actor> getHandle() { return handle; }

    protected:
        ActorHandle<Actor> handle;

        friend world;
    };
}
