#include "PCH.h"

class BarterClickFix {
public:
    static void Install() {
        // Hook BarterMenu::ProcessMessage
        // VTable index 1 for ProcessMessage
        REL::Relocation<std::uintptr_t> vtable{ RE::VTABLE_BarterMenu[0] };
        _ProcessMessage = vtable.write_vfunc(0x1, reinterpret_cast<std::uintptr_t>(ProcessMessage));
        
        logger::info("Hooked BarterMenu::ProcessMessage at VTable index 1");
    }

private:
    static RE::UI_MESSAGE_RESULTS ProcessMessage(RE::BarterMenu* a_this, RE::UIMessage& a_message) {
        static auto lastTime = std::chrono::steady_clock::now();
        
        // Potential messages that trigger item transfer:
        // kUserEvent: Triggered by various input mappings
        // kScaleformEvent: Triggered by UI interaction (mouse clicks, etc.)
        if (a_message.type == RE::UI_MESSAGE_TYPE::kUserEvent || 
            a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent) {
            
            auto now = std::chrono::steady_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count();
            
            // 150ms debounce is the sweet spot for filtering accidental double/triple clicks
            // while remaining responsive to intentional fast clicking.
            if (diff < 150) {
                return RE::UI_MESSAGE_RESULTS::kHandled;
            }
            
            lastTime = now;
        }

        return _ProcessMessage(a_this, a_message);
    }

    static inline REL::Relocation<RE::UI_MESSAGE_RESULTS(RE::BarterMenu*, RE::UIMessage&)> _ProcessMessage;
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
