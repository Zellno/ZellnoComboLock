# Zellno Combo Lock 1.0.0 Validation

## Preconditions

- Stop the local client and server before installing a candidate.
- Back up the local mission profile and persistence storage.
- Load `@ZellnoComboLock` on both client and server.
- Do not modify the hosted production server during local validation.
- Use new locks and structures when validating installation state.

## Vanilla Fence gate

1. Create a new `Zellno_CombinationLock4` through VPPAdminTools.
2. Attach it to a vanilla `Fence` converted into a gate without changing its physical dials.
3. Confirm that `Set Password (Zellno)` appears.
4. Open the menu and verify that it is centered and that the HUD, quickbar and world action prompt are hidden.
5. Confirm that letters, spaces and symbols are removed immediately.
6. Confirm that three digits are rejected and that four, five and six digits are accepted.
7. Confirm that a seventh digit is removed immediately.
8. Verify native Backspace and Delete editing without closing the client or menu.
9. Verify that both `CANCEL` and Escape close the menu and restore player controls, HUD and quickbar.
10. Set the password once and confirm that the menu closes after the positive server response.
11. Confirm that the lock remains attached and that `Open (Zellno)` opens the gate for the creator.
12. Close it with `Close (Zellno)` and verify that the localized vanilla close action is not duplicated.
13. Restart client and server cleanly and confirm that the authorized player can still reopen the gate without re-entering the password.

## Additional player

1. Use a player who has never entered the password for the same closed gate.
2. Confirm that `Open (Zellno)` opens the password interface.
3. Verify that an incorrect password does not open the gate.
4. Enter the correct password.
5. Confirm that the gate opens and that this player becomes authorized for later access.

## Pliers removal

1. Keep the configured Zellno lock attached to a closed gate.
2. Confirm that the removal action is absent with empty hands.
3. Confirm that ruined pliers cannot complete removal.
4. Hold usable pliers and aim at the gate interaction area.
5. Confirm that `Remove Lock (Zellno)` appears.
6. Hold the action for three seconds.
7. Verify that the gate remains built, closed and undamaged.
8. Verify that the intact lock is dropped beside the gate and can be collected.
9. Confirm that the pliers receive only the expected dismantling wear.
10. Install the same lock again and confirm that it requires a new password.
11. In a separate test, confirm that an unauthorized player cannot remove a configured lock.

## RaG BaseBuilding compatibility

Use a new RaG door or gate and a new `Zellno_CombinationLock4`:

1. Attach the lock without configuring its dials.
2. Confirm that `Set Password (Zellno)` appears.
3. Confirm that `Dial number`, `Next digit` and the original open and close actions are hidden.
4. Configure a password containing four to six digits.
5. Open with `Open (Zellno)` and close with `Close (Zellno)`.
6. Confirm that the lock remains attached.
7. Remove it with usable pliers and verify that the intact item is released.
8. Confirm that the original RaG door actions return after removal.
9. Restart without deleting the structure and validate opening, closing and persistence again.
10. Start once without RaG BaseBuilding and confirm that the main Zellno PBO still loads.

A structure from which an attachment was deleted administratively may retain inconsistent state and must not be used to validate the workflow.

## RaG Hunting Cabin compatibility

Run the following cases on both normal and admin cabin variants:

1. Attach a new `Zellno_CombinationLock4` to a closed cabin.
2. Confirm that `Set Password (Zellno)` appears.
3. Confirm that the analog dial actions and original door open and close actions are hidden.
4. Configure a password containing four to six digits.
5. Open with `Open (Zellno)` and close with `Close (Zellno)`.
6. Verify that windows, lights, construction and unrelated cabin interactions remain available.
7. Restart client and server and repeat opening and closing.
8. Remove the lock with pliers and verify that it drops intact.
9. Confirm that the original cabin and analog-lock actions return after removal.
10. Confirm that `Death CarLock` or `Detach CarLock` does not appear while a Zellno lock is attached.
11. Start once without `@RaG_Hunting_Cabin` and confirm that Zellno Combo Lock still loads.

## TraderPlus CarLock regression

1. Aim at a Zellno-protected structure while holding pliers and confirm that the false-positive `Detach CarLock` action is hidden.
2. On a real vehicle protected by CarLock, confirm that `Detach CarLock` remains available under its normal permission rules.
3. Confirm that no TraderPlus PBO was modified.

## Vanilla regression

1. Confirm that the vanilla `CombinationLock4` retains its original behavior, including dropping after a normal unlock.
2. Confirm that a gate without Zellno Combo Lock retains its localized vanilla open and close actions.
3. Confirm that installing Zellno Combo Lock does not globally alter unrelated structures or locks.

## Log review

After every candidate test, inspect the latest client and server script logs and RPT files for:

- `ZellnoComboLock` errors;
- script compilation errors;
- `NULL pointer` involving Zellno actions;
- `ActionDetachCarLock` errors;
- `ActivateZellnoActionSet` errors;
- `OnItemLocationChanged` errors.

## Outside the validated scope

- BaseBuildingPlus (BBP).
- Controller/gamepad and virtual keyboard support.
- Stringtable localization.
- Progressive attempt lockout.
