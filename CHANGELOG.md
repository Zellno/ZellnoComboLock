# Changelog

## 1.0.1 — 2026-10-22

### Fixed

- Restored authorized opening on the current RaG Hunting Cabin release.
- Added a narrowly scoped server-side adapter that bypasses RaG's locked-door guard only during an already authorized Zellno opening call.
- Applied the corrected opening path both after password acceptance and for players already remembered by the lock.

### Validated

- Clean local DayZ 1.30 server startup without script compilation errors.
- Opening and closing on RaG Hunting Cabin.
- Pliers removal, reinstallation, password reconfiguration, opening and closing after reinstallation.
- Final `1.0.1` build after the laboratory candidate passed the same workflow.

### Unchanged

- Vanilla Fence behavior.
- Per-lock authorization persistence.
- Unauthorized-player password requirements.
- Lock removal authorization.
- Public classname: `Zellno_CombinationLock4`.

## 1.0.0 — 2026-10-05

- Initial public release.
- Added the electronic password workflow, server-side validation and per-lock authorization memory.
- Added validated support for vanilla Fence gates and tested RaG structures.
