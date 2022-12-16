#pragma once

#include "App/Extensions/ModuleBase.hpp"
#include "Red/AttachmentSlots.hpp"
#include "Red/TPPRepresentationComponent.hpp"

namespace App
{
class AttachmentSlotsModule : public Module
{
public:
    bool Load() override;
    bool Unload() override;
    std::string_view GetName() override;

private:
    static void OnInitializeSlots(Red::game::AttachmentSlots* aComponent, Red::DynArray<Red::TweakDBID>& aSlotIDs);
    static bool OnSlotSpawningCheck(Red::game::AttachmentSlots* aComponent, Red::TweakDBID aSlotID);
    static void OnInitializeTPP(Red::game::TPPRepresentationComponent* aComponent);

    static inline Core::Map<Red::TweakDBID, Core::Set<Red::TweakDBID>> s_slots;
    static inline std::shared_mutex s_slotsMutex;
};
}
