# Barter Click Fix (NG)

An SKSE plugin that fixes the annoying "triple buy" or "double buy" bug in the Barter menu.

## The Problem
In Skyrim, sometimes when you click to buy or sell an item in the Barter menu, the game registers multiple clicks in a single frame or in extremely rapid succession. This results in buying 3 items when you only wanted 1, which is especially frustrating with non-stackable items like weapons and armor.

## The Solution
This plugin hooks into the `BarterMenu` input processing and implements a **debounce mechanism**. It ignores any "Accept" or "Purchase" inputs that occur within 150ms of each other. This effectively filters out the duplicate events caused by engine or UI lag while still feeling responsive to natural player clicks.

## Features
- Compatible with Skyrim SE (1.5.97), AE (1.6.xxx), and VR.
- Uses CommonLibSSE-NG for cross-version compatibility.
- Extremely lightweight.

## Requirements
- [SKSE64](https://skse.silverlock.org/)
- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)

## Credits
- Developed by Arif KULPU.
- Built with CommonLibSSE-NG.
- **Source Code:** [GitHub](https://github.com/arifkulpu/Barter-Click-Fix-NG-)

---

# Barter Click Fix (NG) (Türkçe)

Takas (Barter) menüsündeki can sıkıcı "üçlü satın alma" veya "ikili satın alma" hatasını düzelten bir SKSE eklentisi.

## Sorun
Skyrim'de, takas menüsünde bir eşya satın almak veya satmak için tıkladığınızda, oyun bazen tek bir karede veya son derece hızlı bir şekilde art arda birden fazla tıklama kaydeder. Bu durum, yalnızca 1 eşya almak isterken 3 eşya almanıza neden olur ve bu özellikle silah veya zırh gibi üst üste istiflenemeyen (non-stackable) eşyalarda oldukça sinir bozucudur.

## Çözüm
Bu eklenti, `BarterMenu` giriş işlemine kanca (hook) atar ve bir **gecikme (debounce) mekanizması** uygular. Birbirinden 150ms daha kısa süre içinde gerçekleşen tüm "Kabul Et" veya "Satın Al" girişlerini yok sayar. Bu sayede, oyuncunun doğal tıklamalarına duyarlı kalırken, oyun motoru veya arayüz gecikmesinden kaynaklanan yinelenen tıklama olaylarını etkili bir şekilde filtreler.

## Özellikler
- Skyrim SE (1.5.97), AE (1.6.xxx) ve VR ile uyumludur.
- Sürümler arası uyumluluk için CommonLibSSE-NG kullanır.
- Son derece hafiftir.

## Gereksinimler
- [SKSE64](https://skse.silverlock.org/)
- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)

## Krediler
- Arif KULPU tarafından geliştirilmiştir.
- CommonLibSSE-NG kullanılarak derlenmiştir.
- **Kaynak Kod:** [GitHub](https://github.com/arifkulpu/Barter-Click-Fix-NG-)
