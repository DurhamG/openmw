# OpenMW User Interface Overview

A comprehensive catalog of every user-facing UI window, HUD element, and dialog in the
OpenMW game engine, organized by frequency of use and category. All source files
referenced below reside under `apps/openmw/mwgui/`.

---

## Major UI Windows

These are the windows a player interacts with most frequently during normal gameplay.

| Window | Description | Primary Source File(s) |
|--------|-------------|----------------------|
| **Inventory Window** | Displays the player's carried items with category filters (All, Weapon, Apparel, Magic, Misc), a 3D character preview, encumbrance bar, and armor rating. Also used during barter and companion interactions. | `inventorywindow.hpp/cpp` |
| **Stats Window** | Shows the player's attributes, skills (major, minor, miscellaneous), health/magicka/fatigue bars, level, faction memberships, reputation, and bounty. | `statswindow.hpp/cpp` |
| **Map Window** | Dual-mode map with a local cell map and a toggleable world map. Supports custom note markers, door markers, fog of war, and zoom. | `mapwindow.hpp/cpp` |
| **Magic / Spells Window** | Lists the player's known spells and enchanted items, allowing selection of the active spell or enchantment. Supports spell deletion and search filtering. | `spellwindow.hpp/cpp` |
| **Dialogue Window** | The NPC conversation interface showing dialogue history, available topics list, disposition bar, persuasion options, and service buttons (Barter, Spells, Travel, Training, etc.). | `dialogue.hpp/cpp` |
| **Container Window** | Opened when activating a container or corpse in the world. Shows the container's contents with Take All and Dispose Corpse buttons. | `container.hpp/cpp` |
| **Journal Window** | A book-style journal with pages for quest entries, quest list, and topic index. Supports keyword search and page turning. | `journalwindow.hpp/cpp`, `journalbooks.hpp/cpp`, `journalviewmodel.hpp/cpp` |
| **Trade / Barter Window** | The merchant buying/selling interface with item filters, price negotiation controls, gold display, and offer/cancel buttons. | `tradewindow.hpp/cpp` |
| **Quick Keys Menu** | Allows the player to assign spells, items, or magic to 10 hotkey slots for quick access during gameplay. | `quickkeysmenu.hpp/cpp` |
| **Book Window** | Displays readable books with page turning, styled text, and an optional Take button. | `bookwindow.hpp/cpp` |
| **Scroll Window** | Displays readable scrolls with scrollable text content and an optional Take button. | `scrollwindow.hpp/cpp` |
| **Companion Window** | Inventory-style window for managing a companion NPC's carried items, showing their encumbrance and profit/loss tracking. | `companionwindow.hpp/cpp` |

---

## Minor UI Windows

Windows used less frequently, including service windows, crafting, and configuration screens.

| Window | Description | Primary Source File(s) |
|--------|-------------|----------------------|
| **Main Menu** | The game's title screen with buttons for New Game, Load Game, Save Game, Options, Credits, and Exit. Supports animated background video. | `mainmenu.hpp/cpp` |
| **Settings Window** | Multi-tabbed options screen covering graphics, audio, controls (keyboard and controller), language settings, and Lua script settings. | `settingswindow.hpp/cpp` |
| **Save/Load Game Dialog** | Modal dialog for saving and loading games with character selection, save slot list, screenshot preview, and cell name display. | `savegamedialog.hpp/cpp` |
| **Alchemy Window** | Potion crafting interface with apparatus and ingredient slots, effect preview, potion naming, and batch brewing count. Supports ingredient filtering by name or effect. | `alchemywindow.hpp/cpp` |
| **Spell Creation Dialog** | Service window for purchasing custom spell creation at a spellmaker NPC. Allows selecting magic effects, adjusting magnitude/duration/area, and naming the spell. | `spellcreationdialog.hpp/cpp` |
| **Enchanting Dialog** | Service window for enchanting items. Allows selecting a soul gem and an item, choosing enchantment type (Cast Once, Cast When Used, Cast When Strikes, Constant Effect), and adding magic effects. | `enchantingdialog.hpp/cpp` |
| **Spell Buying Window** | Service window listing spells available for purchase from an NPC, with costs and player gold display. | `spellbuyingwindow.hpp/cpp` |
| **Training Window** | Service window for purchasing skill training from an NPC. Shows up to 3 trainable skills with costs, capped by the trainer's skill level. | `trainingwindow.hpp/cpp` |
| **Travel Window** | Service window listing travel destinations available from a transport NPC (silt strider, boat, etc.) with prices. | `travelwindow.hpp/cpp` |
| **Merchant Repair Window** | Service window listing the player's damaged items that a merchant can repair, with per-item costs. | `merchantrepair.hpp/cpp` |
| **Repair Window** | Self-service item repair interface using repair hammers. Shows the selected repair tool's quality and remaining uses alongside a list of damaged items. | `repair.hpp/cpp` |
| **Recharge Window** | Self-service enchantment recharge interface using filled soul gems. Shows the selected soul gem and a list of depleted enchanted items. | `recharge.hpp/cpp` |
| **Wait / Rest Dialog** | Allows the player to wait or sleep for a specified number of hours using a slider, with an "Until Healed" option when sleeping in a bed. Shows a progress bar during waiting. | `waitdialog.hpp/cpp` |
| **Level Up Dialog** | Shown on level up, allowing the player to distribute attribute increases. Displays a class image, new level, and per-attribute multipliers. | `levelupdialog.hpp/cpp` |
| **Jail Screen** | Displayed when the player is sent to jail, showing a progress bar as time passes and skill decreases are applied. | `jailscreen.hpp/cpp` |
| **Console** | Developer command console for executing script commands, with command history, tab completion, and text search. Supports object selection for targeted commands. | `console.hpp/cpp` |
| **Debug Window** | Developer window with tabs for the log viewer, Lua profiler, and Bullet physics profiler. | `debugwindow.hpp/cpp` |
| **Post-Processor HUD** | Developer/power-user window for managing post-processing shader chains. Allows activating, deactivating, and reordering shader techniques with per-shader configuration. | `postprocessorhud.hpp/cpp` |
| **Loading Screen** | Displayed during cell loading and game startup, showing splash screen images and a progress bar. | `loadingscreen.hpp/cpp` |

---

## Character Creation Windows

These windows appear in sequence during new game character creation.

| Window | Description | Primary Source File(s) |
|--------|-------------|----------------------|
| **Name Input Dialog** | Text input dialog for entering the player character's name. | `textinput.hpp/cpp` |
| **Race Selection Dialog** | Lets the player pick a race and gender, with a rotatable 3D character preview and customizable face/hair. Shows racial skill bonuses and spell powers. | `race.hpp/cpp` |
| **Class Choice Dialog** | Presents three options for class selection: answer questions to generate a class, pick from a list, or create a custom class. | `class.hpp/cpp` (`ClassChoiceDialog`) |
| **Generate Class Question Dialog** | A series of situational questions whose answers determine the player's generated class. | `class.hpp/cpp` (`InfoBoxDialog`) |
| **Generate Class Result Dialog** | Displays the result of the class generation quiz with an option to accept or go back. | `class.hpp/cpp` (`GenerateClassResultDialog`) |
| **Pick Class Dialog** | Lists all available pre-made classes with their specialization, favored attributes, and major/minor skills. | `class.hpp/cpp` (`PickClassDialog`) |
| **Create Class Dialog** | Full custom class editor with name, specialization, favored attributes, and major/minor skill selection. Includes a description editor. | `class.hpp/cpp` (`CreateClassDialog`) |
| **Birthsign Selection Dialog** | Lets the player select a birthsign, displaying each sign's abilities and spell powers. | `birth.hpp/cpp` |
| **Character Review Dialog** | Summary screen at the end of character creation showing name, race, class, birthsign, attributes, and skills, with the ability to go back and change any choice. | `review.hpp/cpp` |
| **Character Creation Manager** | Not a window itself, but the state machine that orchestrates the flow between all character creation dialogs. | `charactercreation.hpp/cpp` |

---

## HUD Elements

Elements displayed directly on the game screen during normal play.

| Element | Description | Primary Source File(s) |
|---------|-------------|----------------------|
| **HUD (Head-Up Display)** | The primary gameplay overlay containing health/magicka/stamina bars, active weapon and spell displays, minimap with compass, sneak indicator, enemy health bar, crosshair, cell name popup, and active spell/weapon name popup. | `hud.hpp/cpp` |
| **Spell Effect Icons** | Active magic effect icons displayed on the HUD and in the magic window, showing current buffs/debuffs with remaining duration. | `spellicons.hpp/cpp` |
| **Tooltips** | Context-sensitive hover tooltips for items, spells, skills, attributes, birthsigns, NPCs, and map markers throughout the entire UI. | `tooltips.hpp/cpp` |
| **Screen Faders** | Full-screen overlay effects including: the general screen fader (fade to/from black for transitions), the hit fader (red flash when taking damage), the blindness fader (darkness effects), and the werewolf overlay (blood-red border during werewolf transformation). | `screenfader.hpp/cpp` |
| **Controller Buttons Overlay** | Shows contextual gamepad button prompts at the bottom of the screen when using a controller. | `controllerbuttonsoverlay.hpp/cpp` |
| **Inventory Tabs Overlay** | Displays clickable tab buttons for switching between the four inventory-mode windows (Map, Inventory, Magic, Stats) when using a controller. | `inventorytabsoverlay.hpp/cpp` |
| **Drag and Drop Indicator** | Visual feedback widget that follows the cursor when dragging an item between inventory slots, containers, or the game world. | `draganddrop.hpp/cpp` |

---

## Modal Dialogs / Popups

Dialogs that appear on top of other windows and require interaction before returning to the previous screen.

| Dialog | Description | Primary Source File(s) |
|--------|-------------|----------------------|
| **Message Box** | Simple notification messages that appear at the bottom of the screen and fade after a set duration (e.g., "You don't have enough gold", locked door messages). | `messagebox.hpp/cpp` (`MessageBox`) |
| **Interactive Message Box** | Modal message box with clickable button choices, used for yes/no questions and multi-option prompts (e.g., "Do you want to save?"). | `messagebox.hpp/cpp` (`InteractiveMessageBox`) |
| **Confirmation Dialog** | Generic OK/Cancel confirmation popup used throughout the UI for destructive actions (e.g., confirming overwrite of a save, deleting a spell, resetting controls). | `confirmationdialog.hpp/cpp` |
| **Count Dialog** | Numeric input dialog with a slider for specifying quantities when splitting item stacks (e.g., "How many items to drop/buy/sell?"). | `countdialog.hpp/cpp` |
| **Soulgem Dialog** | Popup that appears when using a filled soul gem, asking the player whether to recharge an item or enchant a new one. | `soulgemdialog.hpp/cpp` |
| **Persuasion Dialog** | Sub-dialog of the Dialogue Window offering persuasion options: Admire, Intimidate, Taunt, and Bribe (10/100/1000 gold). | `dialogue.hpp/cpp` (`PersuasionDialog`) |
| **Item Selection Dialog** | Modal item picker used by Alchemy, Repair, Recharge, Enchanting, and Quick Keys to let the player choose an item from their inventory. | `itemselection.hpp/cpp` |
| **Quick Keys Assign Dialog** | Sub-dialog of Quick Keys Menu asking whether to assign an Item, Magic, or Unassign a quick key slot. | `quickkeysmenu.hpp/cpp` (`QuickKeysMenuAssign`) |
| **Magic Selection Dialog** | Sub-dialog of Quick Keys Menu listing all available spells and enchanted items for quick key assignment. | `quickkeysmenu.hpp/cpp` (`MagicSelectionDialog`) |
| **Edit Note Dialog** | Small dialog for creating or editing custom map markers/notes on the map. | `mapwindow.hpp/cpp` (`EditNoteDialog`) |
| **Select Specialization Dialog** | Character creation sub-dialog for choosing Combat, Magic, or Stealth specialization when creating a custom class. | `class.hpp/cpp` (`SelectSpecializationDialog`) |
| **Select Attribute Dialog** | Character creation sub-dialog for picking an attribute (Strength, Intelligence, etc.) when creating a custom class or spell. | `class.hpp/cpp` (`SelectAttributeDialog`) |
| **Select Skill Dialog** | Character creation sub-dialog for picking a skill when creating a custom class. | `class.hpp/cpp` (`SelectSkillDialog`) |
| **Class Description Dialog** | Small text editor for writing a custom class description. | `class.hpp/cpp` (`DescriptionDialog`) |
| **Edit Effect Dialog** | Sub-dialog of the Spell Creation and Enchanting windows for configuring a single magic effect's magnitude, duration, area, and range. | `spellcreationdialog.hpp/cpp` (`EditEffectDialog`) |
| **Wait/Rest Progress Bar** | Progress bar overlay shown during waiting or resting, also used during training time passage. | `waitdialog.hpp/cpp` (`WaitDialogProgressBar`) |

---

## Supporting / Infrastructure Classes

These are not standalone windows but provide shared infrastructure used by the windows above.

| Class | Description | Primary Source File(s) |
|-------|-------------|----------------------|
| **WindowManager** | The central manager that owns all windows, handles GUI mode transitions, and coordinates visibility and input routing. | `windowmanagerimp.hpp/cpp` |
| **WindowBase** | Base class for all game windows, providing common functionality like resize handling, controller support, and Lua window IDs. | `windowbase.hpp/cpp` |
| **WindowPinnableBase** | Extended base class for the four inventory-mode windows (Inventory, Map, Magic, Stats) adding pin-to-screen and maximize support. | `windowpinnablebase.hpp/cpp` |
| **Layout** | Wrapper around MyGUI layout XML files, handling widget loading and management. | `layout.hpp/cpp` |
| **ReferenceInterface** | Mixin for windows that reference a game world object (e.g., Container, Trade, Dialogue), ensuring the reference is cleaned up when invalid. | `referenceinterface.hpp/cpp` |
| **StatsWatcher** | Monitors player stat changes and notifies listener windows (HUD, Stats Window, Character Creation) of updates. | `statswatcher.hpp/cpp` |
| **ItemModel / SortFilterItemModel / TradeItemModel** | Abstract item data models that provide items to ItemView widgets, with sorting, filtering, and trade-specific logic. | `itemmodel.hpp/cpp`, `sortfilteritemmodel.hpp/cpp`, `tradeitemmodel.hpp/cpp` |
| **InventoryItemModel** | Item model backed by an actor's inventory. | `inventoryitemmodel.hpp/cpp` |
| **ContainerItemModel** | Item model backed by a container's contents. | `containeritemmodel.hpp/cpp` |
| **CompanionItemModel** | Item model for companion NPCs with profit tracking. | `companionitemmodel.hpp/cpp` |
| **PickpocketItemModel** | Item model for the pickpocket attempt interface, filtering items by steal chance. | `pickpocketitemmodel.hpp/cpp` |
| **WorldItemModel** | Item model representing the game world as a drop target. | `worlditemmodel.hpp/cpp` |
| **ItemView** | Scrollable grid widget for displaying items from an ItemModel. | `itemview.hpp/cpp` |
| **ItemWidget** | Single item icon widget with quality, charge, and count display. | `itemwidget.hpp/cpp` |
| **SpellWidget** | Single spell icon widget for the HUD spell display. | `itemwidget.hpp/cpp` |
| **ItemChargeView** | Scrollable list of items with charge/condition bars, used in Repair and Recharge windows. | `itemchargeview.hpp/cpp` |
| **SpellModel / SpellView** | Data model and list widget for displaying spells and enchanted items. | `spellmodel.hpp/cpp`, `spellview.hpp/cpp` |
| **BookPage / BookTypesetter** | Formatted text rendering system for books, scrolls, the journal, and the dialogue window. | `bookpage.hpp/cpp` |
| **Formatting** | HTML-like text formatting parser for book content. | `formatting.hpp/cpp` |
| **BackgroundImage** | ImageBox variant with aspect-ratio-correct rendering for splash screens and menu backgrounds. | `backgroundimage.hpp/cpp` |
| **VideoWidget** | Widget for playing video files (intro videos, main menu background). | `videowidget.hpp/cpp` |
| **Cursor** | Custom cursor rendering and management. | `cursor.hpp/cpp` |
| **KeyboardNavigation** | Manages keyboard/controller focus traversal across UI widgets. | `keyboardnavigation.hpp/cpp` |
| **TextColours** | Loads and provides text color settings from game configuration. | `textcolours.hpp/cpp` |
| **TimeAdvancer** | Helper for advancing game time during wait/rest/training/jail with progress callbacks. | `timeadvancer.hpp/cpp` |
| **ResourceSkin** | Custom MyGUI skin type that resolves textures through the VFS. | `resourceskin.hpp/cpp` |
| **Controllers** | MyGUI controller classes for animated UI effects. | `controllers.hpp/cpp` |
| **Widgets** | Shared custom MyGUI widget types (skill, attribute, spell effect, dynamic stat bars, etc.). | `widgets.hpp/cpp` |
| **ExposedWindow** | MyGUI Window subclass that exposes the action bar for move/resize handling. | `exposedwindow.hpp/cpp` |
| **Settings (GUI)** | Window position and size persistence helpers, and setting value definitions for each tracked window. | `settings.hpp/cpp` |
| **WeightedSearch** | Fuzzy/weighted string matching utility used for item and spell filtering. | `weightedsearch.hpp` |
| **ItemTransfer** | Logic for transferring items between ItemViews (container to inventory, etc.) with controller support. | `itemtransfer.hpp` |
| **CustomMarkerCollection** | Storage for player-placed map markers, shared between the HUD minimap and the full Map Window. | `mapwindow.hpp` |
| **LocalMapBase** | Shared base class providing local map rendering, fog of war, and marker management for both the HUD minimap and the Map Window. | `mapwindow.hpp/cpp` |
