# Incident / Accident – Web Implementation Validation

<!-- RQ_HSE_23_3_26_3_36 -->

> **Reference**: Activity diagrams §2–§3 in [`Incident_Reporting_Process_UML.md`](./Incident_Reporting_Process_UML.md)  
> **Web code**: [`hse/src/services/ModuleButtonHandlers/index.js`](hse/src/services/ModuleButtonHandlers/index.js) (`handleIncidentButton`), incident screen handlers, [`ObservationButtonHandlers.js`](hse/src/services/Observation%20service/ObservationButtonHandlers.js) (reject OK for `INCIDENT`)

---

## Summary

| Track | Status |
|-------|--------|
| **Preliminary incident** (Entry / Review / Follow-up) | Implemented with desktop parity fixes (Complete button, SAVE before confirm/close, L1/L2 reject, confirm + delete reject reasons) |
| **Investigation Entry** (`ACDNTENT_ENTRYCMPLTD`) | Implemented via `handleIncidentButton` |
| **Investigation Approval** (CLS / RJC) | Implemented: SAVE+close; **direct** `rejectIncident` (no reject-reason dialog) |
| **Flash Entry / Review** | Implemented in dedicated screen handlers (`CompleteFlashEntry`, etc.) |
| **Medical Entry / Follow-up** (complete / close) | Implemented; injury sub-buttons via `incidentInjuredPersonPopups.js` **RQ_HSE_13_3_26_4_18** |

---

## Activity diagram §2 — “Preliminary incident (end-to-end)” — node-by-node validation

Reference: [`Incident_Reporting_Process_UML.md`](./Incident_Reporting_Process_UML.md) §2.

### Phase 1 — Incident Preliminary Entry

| Diagram step | Meaning | Web implementation | Status |
|--------------|---------|--------------------|--------|
| **Start → A1** | Open Incident Preliminary Entry | Menu / routing (framework) | OK |
| **A1–A2** | Create/edit `HSE_ACDNTENT`, fill tabs (injured persons, causes, actions, attachments) | Form binding, tabs, `Incident_Preliminary_Entry.js` (`toolBarButtonClicked` NEW serials on tabs); injured-person buttons → `incidentInjuredPersonPopups.js` **RQ_HSE_13_3_26_4_18** | OK |
| **A3** | Save record | Toolbar SAVE (framework) | OK |
| **A4–A6** | Entry Completed vs not; “save first” if no record | `handleIncidentButton`: `ACDNTENT_ENTCMPLT` / preliminary `ACDNTENT_RJC` when `isIncidentPreliminaryEntryScreenTag(strScrTag)` (`INCIDENT_PRELIMINARY_ENTRY_SCREEN_TAGS`); `incidentHasSelectedRecord` + key; `AskYesNoMessage('You must save the Record first')` | OK |
| *(extra)* | Header injured count vs tab rows (not a separate diagram node) | Before `ValidateBeforeIncdntEntryComplt` / `completeAccidentTXN`: `validateInjuredPersonCountMatchesHeader` (`ACDNTENT_NUMINJRDPRSN` vs `COUNT(HSE_ACDNTENTINJRDPRSN)`; skipped if header empty) | OK |
| **A7–A8** | `ValidateBeforeIncdntEntryComplt`; user prompt if `N` | `SELECT dbo.ValidateBeforeIncdntEntryComplt(rid)`; same Yes/No message as desktop | OK |
| **A9–A10** | `completeAccidentTXN`; tracing/status via SP | `runIncidentTxn` → `EXECUTE completeAccidentTXN` + `refreshData('', 'REFRESH_SELECTED')` | OK |

**Note:** `ACDNTENT_ENTCMPLT` is handled in **`handleModuleButton` first** ([`buttonEvents.js`](hse/src/events/buttonEvents.js)); it does **not** fall through to `Incident_Preliminary_Entry.js` `sendButtonClickToBackend` for that button when the handler matches.

### Phase 2 — Incident Preliminary Review

| Diagram step | Meaning | Web implementation | Status |
|--------------|---------|--------------------|--------|
| **B1–B2** | Reviewer on Confirmation; Confirm vs Reject | `HSE_TGACDNTCNFRMTN`: `ACDNTENT_CNFRM` / `ACDNTENT_APRV` and `ACDNTENT_RJC` in `handleIncidentButton`; **B1 policy**: `ShowScreen` in [`Incident_Preliminary_Review.js`](../hse/src/screenHandlers/Safety/Incident/Incident_Preliminary_Review.js) uses `getAccidentConfirmationPolicyAllowsReviewScreen` (`HSEPLC_ACDNTCNFRMTNRQRD`) — if disabled, message + disabled toolbar | OK |
| **B2 Reject → B3–B4** | L1 reject reason → `rejectIncident` | `openRejectReasonScreen(..., 'ACDNTENT-L1', key)` + `setPendingRejectForModule('INCIDENT',…)` → [`handleRejectReasonOkButton`](hse/src/services/Observation%20service/ObservationButtonHandlers.js) → `EXECUTE rejectIncident` | OK |
| **B5** | Return toward Entry | Status/reject SP + refresh (user reopens entry per inquiry) | OK (business flow) |
| **B6–B11** | SAVE; pending L1 reasons; delete or abort | `doToolbarAction('SAVE')` → COUNT `HSE_RJCTRSN` → prompt → DELETE or abort | OK |
| **B10–B12** | `confirmAccidentTXN`; tracing | `runIncidentTxn` → `confirmAccidentTXN` + refresh | OK |

### Phase 3 — Incident Preliminary Follow-up

| Diagram step | Meaning | Web implementation | Status |
|--------------|---------|--------------------|--------|
| **C1–C2** | Follow-up; Close vs Reject | `HSE_TGACDNTFOLLWUPDT`: `ACDNTENT_CLS` / `ACDNTENT_RJC` | OK |
| **C2 Reject → C3–C4** | L2 reject reason → `rejectIncident` | `ACDNTENT-L2` + same pending + `rejectIncident` | OK |
| **C5–C7** | SAVE; `closeAccidentTXN`; refresh | `doToolbarAction('SAVE')` + `runIncidentTxn` → `closeAccidentTXN` + `REFRESH_SELECTED` | OK |

### Verdict (post **RQ_HSE_13_3_26_4_18** + §2 gap-close: policy, injured count, entry tags, pop-up locks, get root causes)

| Category | Coverage |
|----------|----------|
| **Workflow buttons** in the diagram (`ENTCMPLT`, `CNFRM`/`APRV`, `RJC`, `CLS`) | **Implemented** in `handleIncidentButton` + reject OK handler |
| **Tab data capture** (save, recommended actions, attachments) | **OK** via framework + tab serial helpers (`Incident_Preliminary_Entry.js` `toolBarButtonClicked`) |
| **“Fill … injured persons” (body parts / injury type / analysis / PPE)** | **OK** — [`incidentInjuredPersonPopups.js`](../hse/src/utils/incidentInjuredPersonPopups.js) + preliminary entry / review / follow-up handlers; accident path pop-ups use **locked** `openScr` (C++ `bLocked=true` for `HSE_ACDNTENT`) |
| **Diagram §2 scope** | **All nodes A1–C7** are covered by web behaviour; **remaining gaps** are **outside** the diagram’s explicit steps or **optional parity** (see **§ Known remaining gaps**) |

---

## Preliminary incident – activity validation (summary table)

| Activity | Diagram | Web | Status |
|----------|---------|-----|--------|
| Entry: save, fill tabs | User / framework | Framework | OK |
| Entry: Entry Completed `ACDNTENT_ENTCMPLT` | `completeAccidentTXN` + validations | `handleIncidentButton`: saved-record check, `validateInjuredPersonCountMatchesHeader`, `ValidateBeforeIncdntEntryComplt`, prompt, SP, `REFRESH_SELECTED` | OK |
| Review: SAVE before confirm | Desktop `DoToolBarAction(SAVE)` | `doToolbarAction('SAVE', formTag)` before confirm | OK |
| Review: pending reject reasons L1 | `IsNewRjcRsnExist` / delete flow | SQL COUNT, `AskYesNoMessage`, DELETE, `confirmAccidentTXN` | OK |
| Review: Confirm `ACDNTENT_CNFRM` | `confirmAccidentTXN` | Same | OK |
| Review: Reject `ACDNTENT_RJC` | `Reject` L1 → OK → `rejectIncident` | `openRejectReasonScreen` + `setPendingRejectForModule` + RJCTRSN OK | OK |
| Follow-up: SAVE before close | Desktop | `doToolbarAction('SAVE')` before `closeAccidentTXN` | OK |
| Follow-up: Close `ACDNTENT_CLS` | `closeAccidentTXN` | `runIncidentTxn` | OK |
| Follow-up: Reject `ACDNTENT_RJC` | L2 module type | `ACDNTENT-L2` + pending + `rejectIncident` | OK |
| Entry: Reject `ACDNTENT_RJC` | Desktop `rejectRecord` does not open dialog (no-op) | Handler returns true without action | OK (parity) |

---

## Investigation – activity validation

| Activity | Web | Status |
|----------|-----|--------|
| `ACDNTENT_ENTRYCMPLTD` | `completeAccidentTXN` in `handleIncidentButton` | OK |
| Approval `ACDNTENT_CLS` | SAVE + `closeAccidentTXN` | OK |
| Approval `ACDNTENT_RJC` | Direct `rejectIncident` (no HSE_RJCTRSN flow) | OK |

---

## Flash report

| Screen | Buttons | Web file | Status |
|--------|---------|----------|--------|
| Flash Entry | `NCDNTFLSHRPRT_ENTRYCMPLT` | `Incident_Flash_Entry.js` → `CompleteFlashEntry` | OK |
| Flash Review | Accept / Reject | `Incident_Flash_Review.js` | OK |

---

## Medical report

| Screen | Buttons | Status |
|--------|---------|--------|
| Entry | `INCDNTMDCLRPRT_ENTRYCMPLT` | OK (`completeIncdntMdclRprt`) |
| Follow-up | `INCDNTMDCLRPRT_CLS` | OK (`closeIncdntMdclRprt`) |
| Entry / Follow-up | `INCDNTMDCLRPRTINJRDPRSNS_*` | **OK** — `incidentInjuredPersonPopups.js` (`openScr` body parts / injury type / injury analysis) **RQ_HSE_13_3_26_4_18** |

---

## Known remaining gaps (optional follow-up)

1. ~~**Medical injured-person pop-ups**~~ — Implemented in [`incidentInjuredPersonPopups.js`](../hse/src/utils/incidentInjuredPersonPopups.js) (**RQ_HSE_13_3_26_4_18**).
2. ~~**Preliminary Review / Entry** — injured-person buttons~~ — Same utility + wired from preliminary entry / review / follow-up handlers.
3. ~~**Entry Complete — header vs tab injured count**~~ — [`validateInjuredPersonCountMatchesHeader`](../hse/src/services/ModuleButtonHandlers/moduleButtonHandlersUtils.js) compares **`ACDNTENT_NUMINJRDPRSN`** to `COUNT(HSE_ACDNTENTINJRDPRSN)` before `completeAccidentTXN` (with `ValidateBeforeIncdntEntryComplt` unchanged). Skips check if the header field is empty.
4. ~~**Investigation Entry** injured-person buttons~~ — [`Incident_Investigation_Entry.js`](../hse/src/screenHandlers/Safety/Incident/Incident_Investigation_Entry.js) calls `handleAccidentInjuredPersonCustomButton` first; **HSE_TGINCDNTINVSTGTNENTRY** is entry-like in the helper (**RQ_HSE_13_3_26_4_18**).
5. ~~**Preliminary Review policy gate**~~ — [`getAccidentConfirmationPolicyAllowsReviewScreen`](../hse/src/services/ModuleButtonHandlers/moduleButtonHandlersUtils.js) + [`Incident_Preliminary_Review.js`](../hse/src/screenHandlers/Safety/Incident/Incident_Preliminary_Review.js) `ShowScreen` (C++ `CheckPolicy("HSEPLC_ACDNTCNFRMTNRQRD")`). ~~**Menu parity**~~ — [`onMenuItemClicked`](../hse/src/screenHandlers/Safety/Incident/Incident_Preliminary_Review.js) blocks navigation + [`menuEvents.js`](../hse/src/events/menuEvents.js) `beforeRenderAppMenu` disables menu item [`INCIDENT_PRELIMINARY_REVIEW_MENU_TAG`](../hse/src/services/ModuleButtonHandlers/moduleButtonHandlersUtils.js) when policy is off.
6. ~~**Screen tag alias for preliminary entry**~~ — [`isIncidentPreliminaryEntryScreenTag`](../hse/src/services/ModuleButtonHandlers/moduleButtonHandlersUtils.js) (`INCIDENT_PRELIMINARY_ENTRY_SCREEN_TAGS`); extend the set if a second tag is confirmed in config.
7. ~~**Get root causes button**~~ — [`executeGetAccidentRootCauses`](../hse/src/services/ModuleButtonHandlers/moduleButtonHandlersUtils.js) from preliminary entry (`ACDNTENTACDNTRSN_GETROTCUS`).
8. ~~**Causes / actions / attachments (browse parity)**~~ — **Documented web inventory** in [`Incident_Preliminary_Entry.js`](../hse/src/screenHandlers/Safety/Incident/Incident_Preliminary_Entry.js) file header (tab tags, `TAB_NEW_SERIAL_CONFIG`, `ACDNTENTACDNTRSN_GETROTCUS`). Standard field browse uses the host + [`browseEvents.js`](../hse/src/events/browseEvents.js); extend `OnBeforeBrowseButtonClick` only if UAT shows a criteria mismatch vs desktop. If APP JSON adds new custom ButtonIDs on these tabs, add them to `INCIDENT_ENTRY_BUTTONS` and delegate like existing buttons.

---

## Technical notes

- **Button order**: [`buttonEvents.js`](hse/src/events/buttonEvents.js) runs `handleModuleButton` **before** screen `ButtonClicked`; incident workflow must live in `handleIncidentButton`.
- **Refresh**: Incident SPs use `refreshData('', 'REFRESH_SELECTED')` via `runIncidentTxn`.
- **Captions**: [`moduleButtonHandlersUtils.js`](hse/src/services/ModuleButtonHandlers/moduleButtonHandlersUtils.js) `getScreenCaption` includes preliminary + investigation + flash + medical tags.
- **Reject OK**: `handleRejectReasonOkButton` resolves `getScreenCaption(formTag)` when `formTag` is still an `HSE_TG…` tag for `INCIDENT` / `PTW` / `RISK`.
- **Menu**: [`onMenuItemClicked`](hse/src/events/menuEvents.js) delegates to the screen handler when implemented (`getDevInterface` in [`buttonEvents.js`](hse/src/events/buttonEvents.js)); handlers must invoke `callBackFn` exactly once.

---

*End of validation report*
