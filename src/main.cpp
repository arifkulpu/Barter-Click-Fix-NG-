#include "PCH.h"

class BarterClickFix {
    struct TransferRecord {
        RE::TESBoundObject* item;
        std::chrono::steady_clock::time_point timestamp;
    };

public:
    static void Install() {
        REL::Relocation<std::uintptr_t> refrVtable{ RE::VTABLE_TESObjectREFR[0] };
        _RemoveItem = refrVtable.write_vfunc(0x56, reinterpret_cast<std::uintptr_t>(RemoveItem));
        
        REL::Relocation<std::uintptr_t> playerVtable{ RE::VTABLE_PlayerCharacter[0] };
        _PlayerAddObjectToContainer = playerVtable.write_vfunc(0x5A, reinterpret_cast<std::uintptr_t>(PlayerAddObjectToContainer));

        logger::info("Installed Ultimate Anti-Script-Duplication Hooks (v5) - Strict Quantity Parity");
    }

private:
    static RE::ObjectRefHandle RemoveItem(
        RE::TESObjectREFR* a_this, 
        RE::TESBoundObject* a_item, 
        std::int32_t a_count, 
        RE::ITEM_REMOVE_REASON a_reason, 
        RE::ExtraDataList* a_extraList, 
        RE::TESObjectREFR* a_moveToRef, 
        const RE::NiPoint3* a_dropLoc, 
        const RE::NiPoint3* a_rotate) 
    {
        bool flagSet = false;

        if (a_moveToRef && a_moveToRef->IsPlayerRef() && a_item && a_item->GetFormID() != 0xF && a_count > 0) {
            auto ui = RE::UI::GetSingleton();
            if (ui && ui->IsMenuOpen(RE::BarterMenu::MENU_NAME)) {
                isProcessingEngineTransfer = true;
                expectedTransferItem = a_item;
                expectedTransferCount = a_count;
                flagSet = true;
                logger::info("Merchant RemoveItem: Expected {:X}, Count: {}", a_item->GetFormID(), a_count);
            }
        }

        auto result = _RemoveItem(a_this, a_item, a_count, a_reason, a_extraList, a_moveToRef, a_dropLoc, a_rotate);

        if (flagSet) {
            isProcessingEngineTransfer = false;
            expectedTransferItem = nullptr;
            expectedTransferCount = 0;
        }

        return result;
    }

    static void PlayerAddObjectToContainer(
        RE::PlayerCharacter* a_this, 
        RE::TESBoundObject* a_object, 
        RE::ExtraDataList* a_extraList, 
        std::int32_t a_count, 
        RE::TESObjectREFR* a_fromRefr) 
    {
        if (a_this && a_object && a_object->GetFormID() != 0xF && a_count > 0) {
            if (isProcessingEngineTransfer) {
                if (a_object == expectedTransferItem) {
                    if (expectedTransferCount > 0) {
                        std::int32_t toAdd = std::min(a_count, expectedTransferCount);
                        if (a_count > toAdd) {
                            logger::info("ANTI-DUPE: Clamped inline duplicate of {:X} from {} to {}!", a_object->GetFormID(), a_count, toAdd);
                            a_count = toAdd; // Clamp the count!
                        }
                        
                        expectedTransferCount -= toAdd; // Deduct fulfilled amount
                        
                        auto now = std::chrono::steady_clock::now();
                        recentTransfers.push_back({a_object, now});
                        logger::info("Legitimate Engine Purchase Granted: Item {:X}, Count {}", a_object->GetFormID(), a_count);
                    } else {
                        // Expected count already fulfilled, this is a synchronous script duplicate
                        logger::info("ANTI-DUPE: Blocked synchronous duplicate of {:X} (Count {})!", a_object->GetFormID(), a_count);
                        return; // Void it!
                    }
                }
            } else {
                // Asynchronous check
                auto now = std::chrono::steady_clock::now();
                
                recentTransfers.erase(
                    std::remove_if(recentTransfers.begin(), recentTransfers.end(),
                        [&](const TransferRecord& rec) {
                            return std::chrono::duration_cast<std::chrono::milliseconds>(now - rec.timestamp).count() > 1500;
                        }
                    ),
                    recentTransfers.end()
                );

                bool isScriptDupe = false;
                for (const auto& rec : recentTransfers) {
                    if (rec.item == a_object) {
                        isScriptDupe = true;
                        break;
                    }
                }

                if (isScriptDupe) {
                    logger::info("ANTI-DUPE: Blocked async script duplicate of {:X} (Count {})!", a_object->GetFormID(), a_count);
                    return; // Void the scripted duplicate!
                }
            }
        }

        _PlayerAddObjectToContainer(a_this, a_object, a_extraList, a_count, a_fromRefr);
    }

    static inline REL::Relocation<RE::ObjectRefHandle(RE::TESObjectREFR*, RE::TESBoundObject*, std::int32_t, RE::ITEM_REMOVE_REASON, RE::ExtraDataList*, RE::TESObjectREFR*, const RE::NiPoint3*, const RE::NiPoint3*)> _RemoveItem;
    static inline REL::Relocation<void(RE::PlayerCharacter*, RE::TESBoundObject*, RE::ExtraDataList*, std::int32_t, RE::TESObjectREFR*)> _PlayerAddObjectToContainer;
    static inline std::vector<TransferRecord> recentTransfers;
    static inline bool isProcessingEngineTransfer = false;
    static inline RE::TESBoundObject* expectedTransferItem = nullptr;
    static inline std::int32_t expectedTransferCount = 0;
};

void InitializeLog() {
    auto path = logger::log_directory();
    if (!path) {
        return;
    }

    *path /= "BarterClickFix.log"sv;
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

    auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

    log->set_level(spdlog::level::info);
    log->flush_on(spdlog::level::info);

    spdlog::set_default_logger(std::move(log));
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] %v"s);

    logger::info("Barter Click Fix v1.0.0 initialized");
}

SKSEPluginInfo(
    .Version = { 1, 0, 0, 0 },
    .Name = "Barter Click Fix",
    .Author = "Arif KULPU",
    .RuntimeCompatibility = { SKSE::VersionIndependence::AddressLibrary, true }
)

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    InitializeLog();
    SKSE::Init(a_skse);

    BarterClickFix::Install();

    return true;
}
