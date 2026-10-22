# Architecture 1.0.1

## Dependencies

- `DZ_Gear_Camping` provides `CombinationLock4` and its vanilla physical assets.
- `DZ_Scripts` provides the vanilla lock script implementation.

## Class

`Zellno_CombinationLock4 : CombinationLock4`

Only `scope`, display name and description are declared. The model, textures, materials, slots, weight, size, damage and behavior remain inherited.

## Authorization core

- The authorized-player list belongs to each `Zellno_CombinationLock4` instance.
- The persisted identifier is `PlayerIdentity.GetId()` and is never synchronized to clients.
- A correct password sent through the interface authorizes the player and opens the `Fence` without releasing the slot or dropping the lock.
- The Zellno action appears on a closed gate protected by this lock; the server opens it only for an authorized player.
- Protected closing uses `ActionZellnoCloseFence`.
- Every physical reinstallation clears the Zellno password and authorization list.
- Each lock stores at most 64 authorized players.

## Electronic workflow, menu and removal

### RaG BaseBuilding compatibility

RaG doors derive from `Fence`, but register analog actions again in their own `SetActions()` and use model selections other than `gate_interact`. Zellno compatibility uses only the inherited vanilla interface: `Fence`, `GetCombinationLock`, `OpenFence` and `CloseFence`.

No RaG class is compiled as a type in the core PBO, and `RaG_BaseBuilding` is not listed in its `requiredAddons[]`. Runtime detection uses the `RaG_BB_` classname prefix, keeping the Zellno core independent when RaG is absent. A separate compatibility PBO declares the RaG dependencies while keeping the core PBO independent. The standard Workshop package places both PBOs and signatures in `addons`.

While a Zellno lock is attached, the analog actions and original open and close actions return `false` only for that door. Removing the lock naturally restores the original conditions. Vanilla locks and unrelated structures are not modified.

### RaG Hunting Cabin compatibility

RaG Hunting Cabin publishes the `RAG_HUNTING_CABIN` define. The entire adapter is guarded by this define; without the mod installed, no RaG class is compiled or required by Zellno Combo Lock.

Normal and admin cabins are instances of the same `rag_hunting_cabin` class. The admin variant differs only through its internal `m_IsAdminHuntingCabin` state, so one adapter supports both. The adapter uses the vanilla `Att_CombinationLock` slot, the `cabin_door` selection and the public `GetCombinationLock`, `OpenHuntCabDoor` and `CloseHuntCabDoor` methods.

While a Zellno Combo Lock is attached, only `ActionDialCombinationLockOnHuntingCabin`, `ActionNextCombinationLockDialOnHuntingCabin`, `ActionOpenHuntingCabin` and `ActionCloseHuntingCabin` are suppressed. Windows, lights, construction and unrelated actions remain intact. Removing the lock restores all four original actions.

Current RaG releases reject `OpenHuntCabDoor()` whenever `IsLocked()` is true. After Zellno completes its own server-side authorization, `ZellnoOpenAuthorizedDoor()` enables a private, short-lived flag and calls RaG's native opening method. The cabin's `IsLocked()` override returns false only during that call, then immediately falls back to `super.IsLocked()`. This retains RaG animation, synchronization and navmesh handling without globally weakening the cabin lock.

- The action opens the interface on the client in `Start`; opening the UI does not depend on an RPC.
- The server validates the player, identity, closed structure, attached lock, locked state and distance again. A vanilla `Fence` uses a 3-meter limit; RaG Hunting Cabin uses 5 meters because its attachments share the cabin's central origin.
- For an authorized player, the server opens the gate directly.
- The interface sends a four-to-six-character string supplied by the player; it never receives the correct password.
- If no Zellno password exists, the server stores the single valid entry and authorizes its creator.
- If a Zellno password exists, the server compares it, authorizes the player and opens the structure only on an exact match.
- `m_ZellnoCode` exists only on the server; only `m_ZellnoConfigured` is synchronized.
- Inherited analog actions are removed, and `UnlockOnServer` does not accept the vanilla dials.
- An attempt does not modify `m_Combination`, the physical dials or the lock slot.
- A simple minimum interval of one second is enforced per player and per lock. It protects the RPC endpoint but is not a progressive lockout system.
- The root and background use the current pixel resolution; the `500x320` window is explicitly centered before and after `super.OnShow()`.
- The root removes `IGNOREPOINTER` and receives `VISIBLE` and `RENDER_ALWAYS`.
- `s_ActiveMenu` is a strong reference that keeps the instance created by `ShowScriptedMenu` alive until normal closure.
- Internal components use exact positions and sizes; setup submits one entry containing four to six digits.
- Title, instruction and status use exact text sizes; button text proportions are constrained relative to button height.
- The menu uses the vanilla `Hud` API to hide the HUD and quickbar during interaction and restores both on close.
- Backspace and Delete retain the edit field's native behavior.
- `KC_ESCAPE` is still handled when it reaches the menu, although a focused field may consume the raw event.
- `Update()` polls `UAUIBack`, following the vanilla menu pattern, and calls the same idempotent close routine as `CANCEL`.
- Control, HUD and quickbar restoration is idempotent and also runs from `Cleanup` as a fallback.
- The password remains visible while typing so players can verify and deliberately share it.
- `Update` continuously normalizes the field because `PasswordEditBoxWidget.OnChange` is not emitted for every key in this interface.
- The client removes non-numeric characters and truncates input after the sixth digit; the server repeats validation before storing or comparing it.

## Pliers removal

- Because removal uses a main item, `ActionZellnoRemoveLock` is added in `Pliers.SetActions()`; registering it only on the target `Fence` is not enough for action discovery.
- The action requires non-ruined `Pliers`, a closed gate, hinges, and an attached and locked `Zellno_CombinationLock4`.
- The client may display the action but never receives the private authorized-player list.
- After the three-second action completes, the server repeats every validation and accepts only an authorized player.
- Until a Zellno password exists, the lock may be recovered to prevent accidental lockout of the structure.
- Removal explicitly calls the vanilla `CombinationLock.UnlockOnServer` behavior, which releases the slot and drops the intact item beside the gate.
- Pliers receive vanilla dismantling wear only when removal actually completes.

## Zellno closing action

- `ActionZellnoCloseFence` reproduces immediate server-side gate closing and displays `Close (Zellno)`.
- The action requires an attached `Zellno_CombinationLock4` and a valid interaction selection for the structure.
- `ActionCloseFence.ActionCondition()` is intercepted only for targets protected by Zellno Combo Lock, preventing duplication of the vanilla action.
- Gates without Zellno Combo Lock continue to use the complete vanilla action.

## Optional TraderPlus CarLock compatibility

- The patch is guarded by `#ifdef CLDefine`, published by the TraderPlus CarLock ecosystem.
- `ActionDetachCarLock.ActionCondition()` returns `false` only when the target resolves to a `Zellno_CombinationLock4`, a protected `Fence` or a protected RaG Hunting Cabin. Direct target, parent and hierarchy parent are checked to cover the formats supplied by the action system.
- For vehicles and every unrelated target, the original TraderPlus implementation is called through `super`.
- No TraderPlus PBO is modified, and Zellno Combo Lock remains usable without TraderPlus.
- Reinstalling the lock clears the previous password and authorization through the existing installation flow.

## Outside this release

- Progressive, configurable attempt lockout.
- A disposable challenge per attempt.
- Stringtable localization and translations.

These layers may be added only after practical validation of the independent core.
