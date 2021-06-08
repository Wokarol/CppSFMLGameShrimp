#pragma once

#include <stdint.h>

namespace wok
{
    class world;

    typedef uint32_t actor_id;

    template <class T>
    class ActorHandle
    {
        actor_id id;
        T* ptr;

    public:
        ActorHandle() :
            id((actor_id)(-1)), ptr(nullptr)
        {}

        ActorHandle(actor_id id) :
            id(id), ptr(world::getActorPointer<T>(id))
        {}

        // No type checks are performed here, use with caution
        ActorHandle(actor_id id, T* newPtr) :
            id(id), ptr(newPtr)
        {}

        bool isValid() const
        {
            if (id == -1)
                return false;

            // If the type check in actor handle constructor or cast failed, our ptr is null
            if (ptr == nullptr)
                return false;

            // At this point we should check the type before (while casting/creation), this prevents access to dead actors
            return world::isActorAlive(id);
        }

        operator T* () const
        {
            return ptr;
        }

        T& operator*() const
        {
            return *ptr;
        }

        T* operator->() const
        {
            return ptr;
        }

        template <class NewT>
        ActorHandle<NewT> as()
        {
            // We pass the pointer as parameter to avoid world lookup for no reason
            return ActorHandle<NewT>(id, dynamic_cast<NewT*>(ptr));
        }

        void destroy() const
        {
            world::destroyActor(id);
        }

        friend world;
    };
}
