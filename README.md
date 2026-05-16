# Barter Click Fix (NG)

An SKSE plugin that fixes the frustrating inventory duplication and "double buy" bugs in the Barter menu, often caused by heavily modded setups, asynchronous script lag, or UI desyncs.

## The Problem
In Skyrim, especially in modded setups with heavy script load or custom UI overhauls (like SkyUI, TrueHUD, or economy mods), buying an item can sometimes result in receiving 2 or even 3 copies of that item, while only paying for 1 (or paying for multiple unexpectedly). This duplication occurs because background scripts or buggy UI elements force additional items into the player's inventory during or immediately after the legitimate engine transfer.

## The Solution
This plugin abandons simple UI-level click blocking and implements an **Ultimate Engine-Level Strict Quantity Parity** mechanism. 
It hooks directly into the core engine functions (`TESObjectREFR::RemoveItem` and `PlayerCharacter::AddObjectToContainer`). When you purchase an item:
1. It records exactly what item and how many the merchant removed from their inventory.
2. It monitors the player's inventory in real-time.
3. It strictly forces the player's incoming items to match the merchant's outgoing items 1-to-1.
Any asynchronous Papyrus script, synchronous event, or buggy mod that tries to double the item count or silently sneak duplicates into your inventory is instantly blocked and voided.

## Features
- **Strict Quantity Parity:** 100% guarantees you only receive what you actually bought.
- **Call-Stack Isolation:** Flawlessly differentiates between legitimate engine transactions and rogue script additions.
- Compatible with Skyrim SE (1.5.97), AE (1.6.xxx), and VR.
- Uses CommonLibSSE-NG for cross-version compatibility.
- Extremely lightweight with zero performance impact.

## Requirements
- [SKSE64](https://skse.silverlock.org/)
- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)

## Credits
- Developed by Arif KULPU.
- Built with CommonLibSSE-NG.
- **Source Code:** [GitHub](https://github.com/arifkulpu/Barter-Click-Fix-NG-)

---

# Barter Click Fix (NG) (Türkçe)

Takas (Barter) menüsündeki can sıkıcı eşya kopyalanması, "ikili satın alma" ve "hayalet eşya" hatalarını çözen bir SKSE eklentisi. Genellikle ağır modlanmış oyunlarda, script gecikmelerinde veya arayüz (UI) senkronizasyon kayıplarında ortaya çıkan sorunları tamamen ortadan kaldırır.

## Sorun
Özellikle çok fazla script içeren veya arayüz modları (SkyUI, TrueHUD, ekonomi modları vs.) kullanan Skyrim kurulumlarında, tüccardan 1 eşya aldığınızda envanterinize 2 veya 3 tane aynı eşyadan gelebilir. Bu kopyalama hatası, oyun motoru eşyayı size verirken, arka planda çalışan hatalı scriptlerin veya UI modlarının aynı eşyayı gizlice cebinize bir kez daha sokmaya çalışmasından kaynaklanır.

## Çözüm
Bu eklenti, basit arayüz tıklama engellemelerini bir kenara bırakarak **Motor Seviyesinde Katı Miktar Eşitliği (Strict Quantity Parity)** mekanizmasını kullanır.
Oyun motorunun en derin fonksiyonlarına (`TESObjectREFR::RemoveItem` ve `PlayerCharacter::AddObjectToContainer`) kanca (hook) atar. Bir eşya satın aldığınızda:
1. Tüccarın (satıcının) envanterinden tam olarak hangi eşyanın kaç adet çıktığını kaydeder.
2. Oyuncunun envanterini eşzamanlı olarak izler.
3. Oyuncuya giren eşya miktarını, tüccardan çıkan eşya miktarına 1'e 1 oranında zorla eşitler.
Eşya miktarını ikiye katlamaya çalışan veya gizlice fazladan kopya eklemeye kalkan tüm asenkron Papyrus scriptleri, senkronize olaylar veya hatalı modlar anında tespit edilir ve engellenir.

## Özellikler
- **Katı Miktar Eşitliği (Strict Quantity Parity):** Yalnızca parasını verdiğiniz eşyayı almanızı %100 garanti eder.
- **Çağrı Yığını İzolasyonu (Call-Stack Isolation):** Oyun motorunun yasal transferleri ile bozuk scriptlerin eklemelerini kusursuzca ayırt eder.
- Skyrim SE (1.5.97), AE (1.6.xxx) ve VR ile uyumludur.
- Sürümler arası uyumluluk için CommonLibSSE-NG kullanır.
- Son derece hafiftir ve performansa sıfır etkisi vardır.

## Gereksinimler
- [SKSE64](https://skse.silverlock.org/)
- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)

## Krediler
- Arif KULPU tarafından geliştirilmiştir.
- CommonLibSSE-NG kullanılarak derlenmiştir.
- **Kaynak Kod:** [GitHub](https://github.com/arifkulpu/Barter-Click-Fix-NG-)
