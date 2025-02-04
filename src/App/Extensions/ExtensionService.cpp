#include "ExtensionService.hpp"
#include "App/Extensions/Animations/Module.hpp"
#include "App/Extensions/AppearanceSwap/Module.hpp"
#include "App/Extensions/AttachmentSlots/Module.hpp"
#include "App/Extensions/Customization/Module.hpp"
#include "App/Extensions/FactoryIndex/Module.hpp"
#include "App/Extensions/InkSpawner/Module.hpp"
#include "App/Extensions/Journal/Module.hpp"
#include "App/Extensions/Localization/Module.hpp"
#include "App/Extensions/GarmentOverride/Module.hpp"
#include "App/Extensions/PuppetState/Module.hpp"
#include "App/Extensions/Streaming/Module.hpp"
#include "Red/GameApplication.hpp"
#include "Red/GameEngine.hpp"
#include "Red/ResourceLoader.hpp"

void App::ExtensionService::OnBootstrap()
{
    m_loader = Core::MakeUnique<ModuleLoader>("", L".xl");

    m_loader->Add<AnimationsModule>();
    m_loader->Add<AppearanceSwapModule>();
    m_loader->Add<AttachmentSlotsModule>();
    m_loader->Add<CustomizationModule>();
    m_loader->Add<FactoryIndexModule>();
    m_loader->Add<InkSpawnerModule>();
    m_loader->Add<JournalModule>();
    m_loader->Add<LocalizationModule>();
    m_loader->Add<GarmentOverrideModule>();
    m_loader->Add<PuppetStateModule>();
    m_loader->Add<StreamingModule>();

    HookOnceAfter<Raw::GameApplication::InitResourceDepot>([&]() {
        m_loader->Configure();
        m_loader->Load();
    });

    HookOnceAfter<Raw::CBaseEngine::LoadGatheredResources>([&]() {
        m_loader->PostLoad();
    });
}

void App::ExtensionService::OnShutdown()
{
    m_loader->Unload();
    m_loader = nullptr;
}

void App::ExtensionService::Configure()
{
    m_loader->Configure();

    HookOnceAfter<Raw::ResourceLoader::OnUpdate>([&]() {
        m_loader->Reload();
    });
}
