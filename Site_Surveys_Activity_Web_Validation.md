# Site Survey Activity Diagram — Web Implementation Validation

<!-- RQ_HSE_23_3_26_22_44 -->

> **Reference diagram**: [`Site_Surveys_Process_UML.md`](./Site_Surveys_Process_UML.md) — §2 *Activity diagram -- Site survey (entry to cycle end)*  
> **Web codebase**: `HSEMS_WEB_Original/hse` (screen handlers, `ModuleButtonHandlers`, `buttonEvents.js`)  
> **Date**: March 2026

### Post–RQ_HSE_23_3_26_22_44 summary (vs §2 diagram)

Every **named phase and transition** in §2 (Start → Entry → Confirmation → Follow-up → cycle end) is implemented on the web: required-actions popup on **Entry**, complete/confirm/close stored procedures, **reject reason** then `rejectSitSrvy` on **Confirmation** (and on **Follow-up** if RJCT is used there), and **SAVE before close** on **Close**.

**Still not guaranteed by web client code** (mostly outside the diagram as explicit boxes, or desktop-only polish): desktop **`setDisableSitSrvyRqrdActnBtn`** after opening Required Actions; **CAR** remains policy/DB after close (same as desktop). **Auto-serials** for findings and required-action lines are implemented via `SubFieldChanged` (see §2 **E4**, §3, and §5). **Site Survey Inquiry** and **Required Actions from Confirmation** are **not drawn** in §2 but are implemented for desktop parity.

---

## 1. How button routing works on the web

Custom button clicks go through `ButtonClicked` in `hse/src/events/buttonEvents.js`:

1. **`handleModuleButton`** runs first for Site Survey actions `SITSRVYENT_CNFRM`, `SITSRVYENT_CLS`, and `SITSRVYENT_RJCT` (see `handleSiteSurveyButton` in `hse/src/services/ModuleButtonHandlers/index.js`).
2. **Screen handler `ButtonClicked`** runs only if the module layer returns `false` (not handled).
3. **`sendButtonClickToBackend`** is a fallback when no handler consumes the event.

This order matters for **Close** and **Reject** (historically caused SAVE/reason gaps; resolved under **RQ_HSE_23_3_26_22_44**, see §5).

---

## 2. Activity diagram §2 — node-by-node validation

Legend: **Yes** = matches the diagram step; **Partial** = user can complete the step but some desktop assist may be missing; **N/A** = not shown in §2 (implemented elsewhere for parity).

<!-- RQ_HSE_23_3_26_22_44 -->

| Diagram node(s) | Text on diagram | Web status | Evidence / notes |
|-----------------|-----------------|------------|------------------|
| **Start** | User opens … Site Survey Entry `HSE_TgSitSrvyEnt` | **Yes** | Shell navigation; handler in `screenHandlers/index.js`. |
| **E1** | Create / edit header `HSE_SITSRVYENT` | **Yes** | Generic CRUD on configured screen. |
| **E2** | Save record (`SITSRVYENT_SITSRVYNO` assigned) | **Yes** | `buttonEvents.js`: `HSE_TGSITSRVYENT` in `SCREEN_TAGS_REQUIRING_TXN_NO` → `generateNewTXNNum` after save when applicable. |
| **E3** | Enter visit details, locations, team, attachments | **Yes** | Screen metadata / tabs; no Site Survey–specific blocker. |
| **E4** | Findings tab: add rows `HSE_SITSRVYENTFNDNG` | **Yes** | **RQ_HSE_23_3_26_22_44**: `Site_Survey_Entry.js` → `SubFieldChanged` when field name contains `SITSRVYENTFNDNG` and `SITSRVYENTFNDNG_SRIL` is empty → [`siteSurveySerialUtils.js`](hse/src/screenHandlers/Performance/Site%20Survey/siteSurveySerialUtils.js) `applyFindingLineSerialIfEmpty` (same SQL as C++ `getNewSerialNoForATab`). |
| **E5–E8** | Required actions → button → popup `HSE_TgSitSrvyRqrdActn` by `SITSRVYRQRDACTN_LNK` | **Yes** | `Site_Survey_Entry.js` → `openSiteSurveyRequiredActionsPopup` ([`siteSurveyRequiredActions.js`](hse/src/screenHandlers/Performance/Site%20Survey/siteSurveyRequiredActions.js)), `TXNSTS: '1'`. Loop back to E4 is user-driven. |
| **E9–E13** | Submit for confirmation → validate saved → `completeSitSrvyTXN` → refresh | **Yes** | `handleEntryComplete` (`SITSRVYENT_ENTCMPLT`); prompts if not saved. |
| **C1–C2** | Open Confirmation `HSE_TgSitSrvyCnfrmtn`; reviewer opens record | **Yes** | Navigation + `Site_Survey_Confirmation.js` / `ShowScreen`. |
| **C3 → Reject (C4–C5)** | `SITSRVYENT_RJCT` → reject reason → `rejectSitSrvy`; return to Entry | **Yes** | On Confirmation: `openRejectReasonScreen(…,'SITSRVYENT-L1',…)` + pending `SITE_SURVEY` → `handleRejectReasonOkButton` → `rejectSitSrvy` ([`ModuleButtonHandlers/index.js`](hse/src/services/ModuleButtonHandlers/index.js)). Diagram path satisfied. |
| **C3 → Confirm (C6–C7)** | `SITSRVYENT_CNFRM` → `confirmSiteSrvy` | **Yes** | `handleSiteSurveyButton` runs `confirmSiteSrvy` before screen fallback. |
| **C8** | Refresh; confirmed; ready for follow-up | **Yes** | Server SP + `runTxnAndRefresh` / `refreshData`. |
| **F1–F2** | Open Result Entry `HSE_TgSitSrvyFlwup`; record follow-up data | **Yes** | Screen + standard edit UI. |
| **F3–F5** | Save → `SITSRVYENT_CLS` → `closeSiteSurveyTXN` | **Yes** | `doToolbarAction('SAVE',…)` then `closeSiteSurveyTXN` in `handleSiteSurveyButton` (matches diagram note “may SAVE first explicitly”). |
| **F6** | Refresh; survey closed | **Yes** | After SP in same handler path. |
| **EndNode** | Cycle end; policy may trigger CAR | **Yes** (server) | CAR generation is **not** a web UI step; driven by policy/SP after close (as on desktop). |

### 2.1 Behaviours not drawn in §2 but implemented on web (desktop parity)

<!-- RQ_HSE_23_3_26_22_44 -->

| Item | Note |
|------|------|
| **Required Actions on Confirmation** | Same button → `openSiteSurveyRequiredActionsPopup` with `TXNSTS: '4'` (`Site_Survey_Confirmation.js`). |
| **Required Actions on Site Survey Inquiry** | Same button → popup with `bLockScr: true` (`Site_Survey_Inquiry.js`). |
| **Reject on Follow-up** | §2 only shows reject in **Confirmation**; web also opens reject reason + `SITSRVYENT-L2` on **Flwup** if RJCT is used (`handleSiteSurveyButton`). |

---

## 3. Additional parity notes (not drawn as separate nodes)

<!-- RQ_HSE_23_3_26_22_44 -->

| Topic | Desktop (reference) | Web (`hse`) |
|--------|---------------------|-------------|
| **Finding line serial** (`SITSRVYENTFNDNG_SRIL`) | Auto on field change in `SitSrvyCategory.cpp` | **Yes** — `Site_Survey_Entry.js` `SubFieldChanged` + `applyFindingLineSerialIfEmpty` (WHERE `SitSrvyEntFndng_LNK` or `SITSRVYENTFNDNG_LNK` = header survey no.). |
| **Required action serial** (`SITSRVYRQRDACTN_ACTNNO`) | Auto on field change in `SitSrvyCategory.cpp` | **Yes** — `Site_Survey_Required_Actions_Popup.js` (`HSE_TgSitSrvyRqrdActn`) `SubFieldChanged` + `applyRequiredActionSerialIfEmpty`. |
| **`SITSRVYENT_RJCT` on Entry** | Desktop `rejectRecord` does not call `Reject()` for `HSE_TGSITSRVYENT` (only L1/L2 on Confirmation/Follow-up). | Web: RJCT on Entry (if present) uses **direct** `rejectSitSrvy` in `handleSiteSurveyButton` fallback — aligns with desktop’s no–reject-reason path for Entry. |
| **Site Survey Inquiry** | Required Actions button with read-only/blocked popup | **RQ_HSE_23_3_26_22_44**: `Site_Survey_Inquiry.js` → `ButtonClicked` → `openSiteSurveyRequiredActionsPopup` with `bLockScr: true`. |

---

## 4. Summary — remaining gaps (after implementation)

<!-- RQ_HSE_23_3_26_22_44 -->

1. **`setDisableSitSrvyRqrdActnBtn`**: Desktop toggles Required Actions button state after opening the popup; **no** web equivalent unless the shell exposes a similar flag.
2. **Physical schema drift**: Serial SQL uses C++/desktop table and column names (`HSE_SITSRVYENTFNDNG`, `HSE_SITSRVYRQRDACTN`, link columns with fallbacks). If a deployment uses different object names, adjust [`siteSurveySerialUtils.js`](hse/src/screenHandlers/Performance/Site%20Survey/siteSurveySerialUtils.js).

---

## 5. Implementation record

<!-- RQ_HSE_23_3_26_22_44 -->

**RQ_HSE_23_3_26_22_44** (March 2026) — closed in web `hse`:

| Item | Location |
|------|----------|
| SAVE before close | [`ModuleButtonHandlers/index.js`](hse/src/services/ModuleButtonHandlers/index.js) — `handleSiteSurveyButton`, `SITSRVYENT_CLS` |
| Reject reason → `rejectSitSrvy` | Same file — `SITSRVYENT_RJCT` with `SITSRVYENT-L1` / `L2`; [`ObservationButtonHandlers.js`](hse/src/services/Observation%20service/ObservationButtonHandlers.js) — `SITE_SURVEY` case in `handleRejectReasonOkButton` |
| Required Actions (Entry / Confirmation / Inquiry) | [`siteSurveyRequiredActions.js`](hse/src/screenHandlers/Performance/Site%20Survey/siteSurveyRequiredActions.js) + callers in `Site_Survey_Entry.js`, `Site_Survey_Confirmation.js`, `Site_Survey_Inquiry.js` |
| Findings + Required Actions auto-serials | [`siteSurveySerialUtils.js`](hse/src/screenHandlers/Performance/Site%20Survey/siteSurveySerialUtils.js), `Site_Survey_Entry.js` `SubFieldChanged`, [`Site_Survey_Required_Actions_Popup.js`](hse/src/screenHandlers/Performance/Site%20Survey/Site_Survey_Required_Actions_Popup.js), registry `HSE_TgSitSrvyRqrdActn` |

---

*End of document*
