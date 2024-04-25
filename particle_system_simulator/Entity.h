#pragma once

#include <entt/entt.hpp>

/*
	entt::registry registry;
    entt::entity entityObject = registry.create();
    Entity e{registry, entityObject};
    TranssformComponent tc{e};
    tc->addComponent<TransformComponent>(1, 2, 3);
    
*/

class Entity
{
private:
	entt::registry& registry; // to store the entities
	entt::entity handle; // entity itself
public:
	Entity(entt::registry& registry, entt::entity handle) : registry(registry), handle(handle) {}

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

    /*
    
        // destroys all the entities in a range
        auto view = registry.view<a_component, another_component>();
        registry.destroy(view.begin(), view.end());
    
    */

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

    void destroy() {
		registry.destroy(handle);
	}

    entt::entity getHandle() const { return handle; }
};

