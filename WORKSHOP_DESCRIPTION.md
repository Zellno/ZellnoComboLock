[h1]ZELLNO COMBO LOCK[/h1]

Zellno Combo Lock provides an electronic password workflow for its custom vanilla-derived four-dial combination lock.

Players enter a numeric password through a dedicated interface instead of rotating the physical lock dials. After entering the correct password once, that player is remembered by the lock and can open or close the protected structure without entering it again.

[h1]FEATURES[/h1]

[list]
[*]Dedicated Zellno combination-lock classname
[*]Electronic password interface
[*]Visible numeric passwords from 4 to 6 digits
[*]Server-side password validation
[*]Per-lock authorized-player memory
[*]Authorization persistence across server restarts
[*]Instant Open (Zellno) and Close (Zellno) actions for authorized players
[*]Safe lock removal with pliers by authorized players
[*]Analog dial actions hidden while a Zellno lock is attached
[*]Original structure actions restored after lock removal
[*]No modification of the vanilla CombinationLock4 classname
[/list]

[h1]VALIDATED COMPATIBILITY[/h1]

[list]
[*]Vanilla Fence gates
[*]Tested RaG BaseBuilding doors and gates
[*]RaG Hunting Cabin normal and admin variants
[*]TraderPlus CarLock coexistence on validated structures
[/list]

[h1]OPTIONAL RAG BASEBUILDING SUPPORT[/h1]

The download includes an optional compatibility PBO:

[code]optional/ZellnoComboLock_RaGCompat.pbo[/code]

Servers using RaG BaseBuilding must copy this PBO and its matching signature from [code]optional[/code] into the mod's [code]addons[/code] folder.

Load RaG BaseBuilding before Zellno Combo Lock.

Servers that do not use RaG BaseBuilding must leave these files in [code]optional[/code]. The main Zellno Combo Lock PBO remains independent.

[h1]BASEBUILDINGPLUS COMPATIBILITY[/h1]

Zellno Combo Lock has not yet been tested with BaseBuildingPlus (BBP).

Compatibility with BBP doors, gates and other lockable structures is currently not guaranteed. Server owners should not assume BBP support until dedicated compatibility testing has been completed.

[h1]SERVER INSTALLATION[/h1]

[olist]
[*]Copy the [code]@ZellnoComboLock[/code] folder to the server.
[*]Copy [code]keys/Zellno.bikey[/code] into the server's [code]keys[/code] folder.
[*]Add [code]@ZellnoComboLock[/code] to the server and client [code]-mod[/code] parameter.
[*]If RaG BaseBuilding is installed, activate the optional compatibility PBO as described above.
[/olist]

[h1]ITEM CLASSNAME[/h1]

[code]Zellno_CombinationLock4[/code]

[h1]NOTES[/h1]

The lock uses the vanilla four-dial model as its visual base. The interaction workflow and server-side behavior are provided by Zellno Combo Lock.

Version: 1.0.0
