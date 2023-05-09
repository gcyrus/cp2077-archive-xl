#include "Module.hpp"
#include "Red/AnimatedComponent.hpp"
#include "Red/Entity.hpp"

namespace
{
constexpr auto ModuleName = "Animations";
}

std::string_view App::AnimationsModule::GetName()
{
    return ModuleName;
}

bool App::AnimationsModule::Load()
{
    if (!HookBefore<Raw::AnimatedComponent::InitializeAnimations>(&AnimationsModule::OnInitializeAnimations))
        throw std::runtime_error("Failed to hook [AnimatedComponent::InitializeAnimations].");

    PrepareEntries();

    return true;
}

void App::AnimationsModule::Reload()
{
    PrepareEntries();
}

bool App::AnimationsModule::Unload()
{
    Unhook<Raw::AnimatedComponent::InitializeAnimations>();

    return true;
}

void App::AnimationsModule::PrepareEntries()
{
    m_animsByEntity.clear();

    auto depot = Red::ResourceDepot::Get();
    Core::Set<Red::ResourcePath> invalidPaths;

    for (const auto& unit : m_units)
    {
        for (const auto& animation : unit.animations)
        {
            auto entityPath = Red::ResourcePath(animation.entity.c_str());

            if (!depot->ResourceExists(entityPath))
            {
                if (!invalidPaths.contains(entityPath))
                {
                    LogWarning("|{}| Entity \"{}\" doesn't exist. Skipped.", ModuleName, animation.entity);
                    invalidPaths.insert(entityPath);
                }
                continue;
            }

            auto animPath = Red::ResourcePath(animation.set.c_str());

            if (!depot->ResourceExists(animPath))
            {
                if (!invalidPaths.contains(animPath))
                {
                    LogError("|{}| Animation \"{}\" doesn't exist. Skipped.", ModuleName, animation.set);
                    invalidPaths.insert(animPath);
                }
                continue;
            }

            Red::animAnimSetupEntry animSetupEntry;
            animSetupEntry.animSet = animPath;
            animSetupEntry.priority = animation.priority;

            for (const auto& var : animation.variables)
            {
                animSetupEntry.variableNames.EmplaceBack(var.c_str());
            }

            m_animsByEntity[entityPath].emplace_back(std::move(animSetupEntry));

            m_paths[entityPath] = animation.entity;
            m_paths[animPath] = animation.set;
        }
    }
}

void App::AnimationsModule::OnInitializeAnimations(Red::entAnimatedComponent* aComponent)
{
    const auto entity = Raw::IComponent::Owner::Get(aComponent);
    const auto templatePath = Raw::Entity::TemplatePath::Ref(entity);

    const auto& anims = m_animsByEntity.find(templatePath);
    if (anims != m_animsByEntity.end())
    {
        LogInfo("|{}| Initializing animations for \"{}\"...", ModuleName, m_paths[templatePath]);

        for (const auto& anim : anims.value())
        {
            LogInfo("|{}| Merging animations from \"{}\"...", ModuleName, m_paths[anim.animSet.path]);

            aComponent->animations.gameplay.PushBack(anim);
        }

        LogInfo("|{}| All animations merged.", ModuleName);
    }
}
