# HSE Screens – Implementation List

This document lists which screens have dedicated screen handlers under `hse/src/screenHandlers` and which are implemented only in shared event modules and services. It also records which event handlers each screen uses.

---

## Event handlers in the system

These are the event handler names exported from `hse.js` and invoked by the WebInfra.Client infrastructure. When the user is on a screen, that screen may receive any of the **screen-level** events below (implementation is either in a dedicated screen handler or in shared event modules).

| Event handler name | Module | Description |
|--------------------|--------|-------------|
| ShowScreen | screenEvents | Screen/tab opened; set button state, map field types, enable tabs. |
| toolBarButtonClicked | buttonEvents | Toolbar actions (New, Save, Delete, etc.); tab enabling after save. |
| ButtonClicked | buttonEvents | Custom button clicks (e.g. Reject, Confirm, Entry Complete). |
| MainSubReposition | screenEvents | Record navigation; Comments tab management. |
| onMenuItemClicked | menuEvents | Menu item selected (e.g. set criteria). |
| SubFieldChanged | screenEvents | Field value changed (optional). |
| OnBeforeBrowseButtonClick | browseEvents | Before browse/button action. |
| UpdateAfterBrowse | browseEvents | After browse data update. |

*App-level (not per-screen):* `beforeRenderAppMenu`, `beforeRenderCustomActions`, `onAppOpen`.

---

## Screens with a dedicated screen handler

Handler paths follow the HSE menu structure (see `Webinfra.Server/AppMenus/HSE.json`).

| Screen tag(s) | Handler path | Description | Event handlers used (implemented in handler) |
|---------------|--------------|-------------|-----------------------------------------------|
| HSE_TgNrstMiscEnt / HSE_TGNRSTMISCENT | `Safety/Observation/Observation_Entry.js` | Observation Entry (menu: Safety \ Observation \ Observation Entry) | **ShowScreen**, **toolBarButtonClicked**, **ButtonClicked** |
| HSE_TgNrstMisccnfrmtn | `Safety/Observation/Observation_Review.js` | Observation Review (menu: Safety \ Observation \ Observation Review) | **ShowScreen**, **toolBarButtonClicked**, **ButtonClicked** |
| HSE_TgNrstMiscflwup | `Safety/Observation/Observation_Approval.js` | Observation Approval (menu: Safety \ Observation \ Observation Approval) | **ShowScreen**, **toolBarButtonClicked**, **ButtonClicked** |
| HSE_TgNrMisRward | `Safety/Observation/Observation_Reward.js` | Observation Reward (menu: Safety \ Observation \ Observation Reward) | **ShowScreen**, **toolBarButtonClicked**, **ButtonClicked** |
| HSE_TGNrMisTypes | `Setup/Observations/Observation_Type.js` | Observation Type (menu: Setup \ Observations \ Observation Type) | **ShowScreen**, **toolBarButtonClicked**, **ButtonClicked** |
| HSE_TgObsrvtnTrgts | `Setup/Observations/Observation_Targets.js` | Observation Targets (menu: Setup \ Observations \ Observation Targets) | **ShowScreen**, **toolBarButtonClicked**, **ButtonClicked** |
| HSE_TgNrstMiscInq | `Inquiry/Observation_Inquiry.js` | Observation Inquiry (menu: Inquiry screens \ Observation Inquiry) | **ShowScreen**, **toolBarButtonClicked**, **ButtonClicked** |

*Other events (MainSubReposition, onMenuItemClicked, SubFieldChanged, OnBeforeBrowseButtonClick, UpdateAfterBrowse) for these screens are still handled by shared code when the handler does not export them.*

---

## Screens implemented outside screenHandlers (shared code only)

### Observation module (from OBSERVATION_SCREEN_TAGS)

*All observation screens (Entry, Type, Targets, Review, Approval, Reward, Inquiry) now have dedicated handlers; paths match HSE menu: `Safety/Observation/` (Entry, Review, Approval, Reward), `Setup/Observations/` (Type, Targets), `Inquiry/` (Observation Inquiry). Reject Reason remains shared-only.*

*Logic:* `buttonEvents.js`, `screenEvents.js`, `ObservationService.js`, `ObservationButtonHandlers.js`, `ObservationTabManagement.js`.

### Reject Reason (used by Observation flow)

| Screen tag | Description | Event handlers used (all via shared code) |
|------------|-------------|-------------------------------------------|
| HSE_TGRjctRsn | Reject Reason | ShowScreen, toolBarButtonClicked, ButtonClicked, MainSubReposition, onMenuItemClicked, SubFieldChanged, OnBeforeBrowseButtonClick, UpdateAfterBrowse |

*Logic:* Opened and used from `ObservationButtonHandlers.js`; no dedicated handler file.

### Screens using shared SAVE / TXN number logic (SCREEN_TAGS_REQUIRING_TXN_NO)

| Screen tag | Description | Event handlers used (all via shared code) |
|------------|-------------|-------------------------------------------|
| HSE_TGRSKASSMNTENT | Risk Assessment Entry | ShowScreen, toolBarButtonClicked, ButtonClicked, MainSubReposition, onMenuItemClicked, SubFieldChanged, OnBeforeBrowseButtonClick, UpdateAfterBrowse |
| HSE_TGPTNLHZRDENT | Potential Hazard Entry | ShowScreen, toolBarButtonClicked, ButtonClicked, MainSubReposition, onMenuItemClicked, SubFieldChanged, OnBeforeBrowseButtonClick, UpdateAfterBrowse |
| HSE_TGPERFMSRMNTENT | Performance Measurement Entry | ShowScreen, toolBarButtonClicked, ButtonClicked, MainSubReposition, onMenuItemClicked, SubFieldChanged, OnBeforeBrowseButtonClick, UpdateAfterBrowse |
| HSE_TGACDNTENT | Accident Entry | ShowScreen, toolBarButtonClicked, ButtonClicked, MainSubReposition, onMenuItemClicked, SubFieldChanged, OnBeforeBrowseButtonClick, UpdateAfterBrowse |
| HSE_TGNRSTMISCENT | Observation Entry (also has handler) | Same as Observation Entry handler + shared for events not in handler |
| HSE_TGPTWSRGSTRENT | PTW Register Entry | ShowScreen, toolBarButtonClicked, ButtonClicked, MainSubReposition, onMenuItemClicked, SubFieldChanged, OnBeforeBrowseButtonClick, UpdateAfterBrowse |
| HSE_TGCRCTVEACCENT | Creative Access Entry | ShowScreen, toolBarButtonClicked, ButtonClicked, MainSubReposition, onMenuItemClicked, SubFieldChanged, OnBeforeBrowseButtonClick, UpdateAfterBrowse |
| HSE_TGSITSRVYENT | Site Survey Entry | ShowScreen, toolBarButtonClicked, ButtonClicked, MainSubReposition, onMenuItemClicked, SubFieldChanged, OnBeforeBrowseButtonClick, UpdateAfterBrowse |
| HSE_TGINCDNTFLSHRPRT | Incident Flash Report | ShowScreen, toolBarButtonClicked, ButtonClicked, MainSubReposition, onMenuItemClicked, SubFieldChanged, OnBeforeBrowseButtonClick, UpdateAfterBrowse |

*Logic:* `buttonEvents.js` (generateTXNNo, getTXNKeyFldVal, SAVE/NEW handling).

---

## Summary

- **With handler:** 7 screens (Observation Entry, Observation Type, Observation Targets, Observation Review, Observation Approval, Observation Reward, Observation Inquiry); event handlers implemented in handler: ShowScreen, toolBarButtonClicked, ButtonClicked.
- **Reject Reason:** 1 screen; all event handlers via shared code.
- **Shared SAVE/TXN:** 9 screens (including Observation Entry); all use the same 8 screen-level event handlers via shared code.

To move a screen “into” screen handlers: add a handler module under `hse/src/screenHandlers`, export the desired event handler names (ShowScreen, toolBarButtonClicked, ButtonClicked, MainSubReposition, onMenuItemClicked, etc.), register it in `screenHandlers/index.js`, and (optionally) keep or reduce the shared logic for that screen.
