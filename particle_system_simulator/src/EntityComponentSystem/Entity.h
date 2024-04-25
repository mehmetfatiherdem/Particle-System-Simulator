#pragma once

#include <entt/entt.hpp>

class Entity
{
private:
	entt::registry& registry;
	entt::entity handle;

public:
	Entity(entt::registry& registry, entt::entity handle) : registry(registry), handle(handle) {}
	~Entity()
	{
		registry.destroy(handle);
	}

    template<typename T, typename... Args>
    T& addComponent(Args&&... args) {
		return registry.emplace<T>(handle, std::forward<Args>(args)...);
	}

    template<typename T>
    T& getComponent() {
        return registry.get<T>(handle);
    }

    template<typename T>
    bool hasComponent() {
        return registry.all_of<T>(handle);
    }

    template<typename T>
    void removeComponent() {
        registry.remove<T>(handle);
    }

    template<typename... Component>
    auto getEntitiesWithComponents() {
		return registry.view<Component...>();
	}

    /*
     now there are three different options to loop through the entities from our view
     1.: a basic lambda which will be called with the given components
     note: the lambda arguments need to match the components with which we create this view
     view.each([](const auto &pos, auto &vel) {})

     2.: an extended lambda which also gives us the entity if we need it
     view.each([](const auto entity, const auto& pos, auto& vel) {});

    3. a for loop by using structured bindings
    for (auto [entity, pos, vel] : view.each()) {
    }

    4. a for loop with a forward iterator
    for (auto entity : view) {
       auto& vel = view.get<velocity>(entity);
    }
    */
};

