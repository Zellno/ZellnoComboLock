# Zellno Combo Lock

An independent DayZ addon that modernizes the vanilla four-dial combination lock with an electronic password workflow.

## Current status

Version `1.0.1` preserves the independent core and updates the RaG Hunting Cabin adapter for the current RaG door-locking behavior. The standard Workshop package already supplies the core and RaG compatibility PBOs together in `addons`; users must not move, rename or duplicate them.

The addon registers this classname:

`Zellno_CombinationLock4`

It derives directly from `CombinationLock4` and reuses only vanilla resources provided by DayZ.

The validated core:

- remembers on the server which players entered the correct password;
- opens a `Fence` without removing the lock from its slot;
- lets authorized players reopen it with `Open (Zellno)`;
- replaces the vanilla closing action with `Close (Zellno)` only on gates protected by Zellno Combo Lock;
- persists up to 64 authorized player IDs per lock;
- clears authorization when the lock is installed again;
- preserves authorization across client and server restarts;
- lets authorized players remove the intact lock with pliers;
- hides the false-positive `Detach CarLock` action only on structures protected by Zellno Combo Lock;
- provides the Zellno electronic workflow on compatible RaG doors without depending on RaG classes or assets in the main PBO;
- hides analog dial actions reintroduced by RaG doors only while the attached item is a Zellno lock.

The continuous `Remove Lock (Zellno)` action is registered directly on the pliers, as required by the DayZ main-item action system. It appears when the player aims at a closed gate while holding usable pliers. Authorization is validated again on the server when the action completes. Vanilla lock behavior releases the slot and drops the intact lock beside the gate; the gate is neither dismantled nor damaged. Locks that do not yet have a password can also be recovered.

## DayZ 1.30 compatibility

The lock has been adapted to the DayZ 1.30 `UnlockOnServer`
API. Local tests confirmed opening, closing, password configuration,
lock removal and persistence after server restart on vanilla Fence
gates and tested RaG structures.

DayZ 1.30 multiplayer validation with a second player remains pending.

Version `1.0.1` also adapts Hunting Cabin opening to the current RaG guard that rejects its native open method while a combination lock reports the cabin as locked. The adapter bypasses that guard only during an already authorized server-side Zellno opening call and otherwise preserves RaG locking behavior.

## BaseBuildingPlus compatibility

Zellno Combo Lock has not yet been tested with BaseBuildingPlus (BBP). Compatibility with BBP doors, gates and other lockable structures is not guaranteed in this version. Server administrators should not consider BBP supported until dedicated compatibility testing has been completed.

## Version 1.0.1 scope

The release validates that the new item:

- appears in VPPAdminTools under the correct classname;
- uses the vanilla lock model and inventory representation;
- can be carried in the player's hands;
- can be attached to the `Att_CombinationLock` slot of a vanilla `Fence`;
- opens a local interface for configuring a new lock;
- closes the interface through both `CANCEL` and Escape;
- configures a password with a single valid entry;
- accepts four to six digits, including passwords with a leading zero;
- rejects an incorrect password without using the physical dials;
- validates the correct password on the server without dropping the lock;
- opens and closes the gate while keeping the lock attached and logically locked;
- lets authorized players reopen it through `Open (Zellno)`;
- provides `Close (Zellno)` without duplicating the vanilla action;
- preserves `Detach CarLock` on real vehicles while hiding it on Zellno-protected structures;
- requires an unauthorized player to enter the password;
- preserves authorization across restarts;
- provides `Set Password (Zellno)`, `Open (Zellno)` and `Close (Zellno)` on compatible RaG doors;
- hides analog dial actions on those doors while the attached lock is a Zellno lock;
- hides the original localized open and close actions alongside the Zellno actions;
- restores the original RaG door actions after the Zellno lock is removed;
- continues working when RaG BaseBuilding is not installed or loaded;
- provides the Zellno workflow on the `rag_hunting_cabin` class for both normal and admin cabins;
- preserves cabin windows, lights, construction and unrelated actions;
- continues working when RaG Hunting Cabin is not installed or loaded;
- offers removal only with usable pliers;
- rejects removal of a configured lock by an unauthorized player;
- drops the intact lock beside the gate without destroying the structure;
- allows the same item to be collected and installed again.

## Deliberately safe migration

Objects persisted in the previous format are read, but they become unconfigured Zellno locks and must be configured through the new action. Previous analog combinations are never promoted automatically.

## Isolation

The vanilla `CombinationLock4` classname is not modified. This release contains no code or assets from Code Lock, ZenComboLocks or any other third-party mod.

## Build

`build.sh` follows the Linux/Wine workflow used by other Zellno projects and operates only inside `build/@ZellnoComboLock`. It builds, signs and verifies the main PBO and the RaG compatibility PBO, but it does not install the addon on the client or server.

Compiled PBOs, signatures and private signing keys are intentionally excluded from this repository.

## Installation

1. Copy `@ZellnoComboLock` to the DayZ server directory.
2. Copy `keys/Zellno.bikey` from the compiled mod package to the server `keys` directory.
3. Add `@ZellnoComboLock` to the client and server `-mod` launch parameter.
4. Keep every supplied PBO and matching signature together in `addons`.
5. Load the RaG mods before Zellno Combo Lock.
6. Restart the server.

The main PBO remains architecturally independent from RaG; the compatibility layer is packaged separately and loaded only when its declared RaG dependencies are present.

## Documentation

- [Architecture](ARCHITECTURE.md)
- [Testing](TESTING.md)
- [Third-party notice](THIRD_PARTY.md)
- [Workshop description](WORKSHOP_DESCRIPTION.md)
- [MIT License](LICENSE)

## Support

- [Zellno Mod Support — Discord](https://discord.gg/bhfBetKtqr)
- [Buy Me a Coffee — Noob Open Source](https://www.buymeacoffee.com/noobopensource)

## License

Original Zellno source code and documentation in this repository are released under the [MIT License](LICENSE).

DayZ, its assets and third-party modifications remain the property of their respective rights holders. See [THIRD_PARTY.md](THIRD_PARTY.md).

## Monetization permission

Zellno permits the use of Zellno Combo Lock on monetized DayZ servers, provided that the server operator is registered, approved and listed under Bohemia Interactive's DayZ Server Monetization program and complies with all applicable rules.

This permission applies only to the original content provided by Zellno in Zellno Combo Lock. It does not grant permission to monetize DayZ itself, RaG BaseBuilding, RaG Hunting Cabin, TraderPlus or any other third-party modification or content used alongside this mod.

Server operators are responsible for obtaining any additional permissions required by the authors of other installed mods.

- [Official monetization rules](https://www.bohemia.net/monetization)
- [Approved DayZ servers](https://www.bohemia.net/monetization/approved/dayz)

## Disclaimer

This is an unofficial community modification for DayZ. It is not affiliated with, authorized by or endorsed by Bohemia Interactive a.s. DAYZ is a registered trademark of Bohemia Interactive a.s.
