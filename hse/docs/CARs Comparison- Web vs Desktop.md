# CARs Gap Analysis — Web vs Desktop (Missing & Partial Only)

**Purpose:** Actionable migration backlog. Only items with ⚠️ Partial or ❌ Missing status are included.
**Baseline:** Desktop C++ (`CARs- Desktop.md`). **Target:** Web (`HSEMS_WEB_Original/hse`).
**Date:** 2026-04-12

**Constraint:** The database (schema, triggers, stored procedures) is managed entirely by the desktop system and must NOT be modified from the web side. All gaps must be resolved exclusively at the application layer (frontend JavaScript screen handlers and utility files).

---

## Priority Legend

| Tier | Meaning |
|------|---------|
| **P0** | 🔥 Breaks business correctness — must fix |
| **P1** | ⚠️ Affects behavior meaningfully |
| **P2** | Minor inconsistency / hardening |

---

## SECTION A — LAYER 1: Action Tracking CAR (`HSE_CRENTRY`) Gaps

---

### GAP-15 ⚠️ Partial — CAR Entry: `ENTRY_COMPLETED` Does Not Guard Against Missing Required Header Fields

**Desktop:** `CNewCarEntryCategory::DisplayCustomButtonClicked ENTRY_COMPLETED` — before setting CRSTT=05, the desktop implicitly relies on form-level MUST attributes ensuring `CRENTRY_CRDT`, `CRENTRY_CRSRC`, and `CRENTRY_NCDSC` are populated. The C++ save phase rejects empty MUST fields at the UI framework level.

**Web:** `handleEntryCompletedCarEntry` in `carCustomButtons.js` checks `assertCrentryStatusAllows(['01','02'])` but does NOT validate that the CAR date, source, or NC description are filled. If the shell's form-level MUST enforcement is weak or absent, a CAR can advance to status 05 with empty business-critical fields.

**Risk:** P1 — CAR proceeds to review with incomplete header data, causing downstream confusion.

**Plan:**
- **Layer:** Frontend (validation logic in `carCustomButtons.js`)
- Before `executeChangeEntityStatus` in `handleEntryCompletedCarEntry`, read `CRENTRY_CRDT`, `CRENTRY_CRSRC`, `CRENTRY_NCDSC` via `FormGetField`
- If any are empty, show appropriate message and return (matching desktop message tone)

---

### GAP-16 ⚠️ Partial — Actions Entry `ENTRY_COMPLETED`: Mass-Set SQL Uses Unparameterized PrmKy

**Desktop:** The C++ code runs mass-SET updates for corrections (`CRRCTVACTNS_LnkCss_Acc=Y`, `ACTNSENTRY_Crr_ActStt=A`) and corrective actions (`CRRCTVACTNS_Acc=Y`, `CRRCTVACTNS_ACTSTT=3`) using bound parameter PrmKy.

**Web:** `handleEntryCompletedActionsEntry` builds SQL via template literals: `WHERE PrmKy=${strPrmKy}`. The `strPrmKy` comes from `getCurrentRecordPRMKY` which extracts from `fullRecord`. While currently validated as a numeric string via `parseInt` in some paths, the mass-update SQL does not use `parseInt` protection before interpolation.

**Risk:** P2 — SQL injection risk if `strPrmKy` is ever a non-numeric string. Business logic is correct, but the pattern is fragile.

**Plan:**
- **Layer:** Frontend (hardening in `carCustomButtons.js`)
- Wrap `strPrmKy` with `parseInt(strPrmKy, 10)` in all mass-update SQL statements within `handleEntryCompletedActionsEntry`
- Apply same protection pattern to `handleCloseCar` and `ACCEPT_CAR` for Actions Review

---

### GAP-17 ❌ Missing — Actions Review `ACCEPT_CAR`: No Target Date Rollup Notification

**Desktop:** `NewCarEntryCategory::AcceptActionsReviewCAR` — after updating `CRENTRY_CRTRGDT = MAX(CRRCTVACTNS_trgDt)`, the C++ code uses `RefreshScreen` which visually updates the target date on the form. Additionally, the CTE update only fires `WHERE ISNULL(MainTrgtdt,0) < RootCauseTrgtdt`.

**Web:** The CTE update is present and correct in `handleCarButton` for `ACCEPT_CAR` + `HSE_TGACTNSRVIW`. However, after the update, `FormSetField` is NOT called to reflect the new `CRENTRY_CRTRGDT` on the form — only `doToolbarAction('SAVE')` + `refreshData('')` are called. If `refreshData` does not re-fetch the field value, the user sees the old target date until they manually refresh.

**Risk:** P2 — User may see stale target date on the Actions Review form after accepting.

**Plan:**
- **Layer:** Frontend (UX in `carCustomButtons.js`)
- After the CTE update, SELECT the new MAX target date and call `FormSetField('HSE_TgActnsRviw', 'CRENTRY_CRTRGDT', newDate, 'scr')` before the SAVE

---

### GAP-18 ⚠️ Partial — CAR Review: `CAR_REVIEW_INFO` Button Missing from CAR Entry Screen

**Desktop:** `CNewCarEntryCategory::DisplayCustomButtonClicked` handles `CAR_REVIEW_INFO` on BOTH `HSE_TGCRENTRY` and `HSE_TGCRRVW`. On CAR Entry, the review info popup opens without the policy check (the policy check only applies on Review ACCEPT).

**Web:** `handleCarButton` handles `CAR_REVIEW_INFO` on `HSE_TGCRRVW` (with policy check via `tryOpenCarReviewInfoWithPolicy`) and on `HSE_TGCRENTRY` (line 770). Both go through the policy-gated path. This is CORRECT.

**Status:** ✅ Match — already handled. No gap.

---

### GAP-19 ⚠️ Partial — CAR Follow-Up: `CLOSE_CAR` Fires on `HSE_TGACTNSRVIW` (Actions Review) — Unexpected

**Desktop:** `CLOSE_CAR` only appears on `HSE_TGCRFLOUP` (Follow-Up screen) and `HSE_TGCREDTNG` (CAR Editing). The Actions Review screen (`HSE_TGACTNSRVIW`) does NOT have a CLOSE_CAR button in the desktop — it only has ACCEPT_CAR and REJECT_CAR.

**Web:** `handleCarButton` routes `CLOSE_CAR` on `['HSE_TGACTNSRVIW', 'HSE_TGCRFLOUP', 'HSE_TGCREDTNG']`. This means the Actions Review screen ALSO exposes CLOSE_CAR, which the desktop does NOT support.

**Risk:** P1 — If the shell delivers a `CLOSE_CAR` button on Actions Review, a reviewer could close a CAR that hasn't been approved yet, bypassing the approval stage entirely.

**Plan:**
- **Layer:** Frontend (logic fix in `carCustomButtons.js`)
- Remove `'HSE_TGACTNSRVIW'` from the CLOSE_CAR handler array
- Only allow CLOSE_CAR on `['HSE_TGCRFLOUP', 'HSE_TGCREDTNG']`

---

### GAP-20 ⚠️ Partial — CAR Follow-Up: CLOSE_CAR Does Not Check `CRENTRY_CRSTT = 15` Before Closing

**Desktop:** The Follow-Up screen is only accessible when CRSTT=15 (Approved) via the desktop's `ChangeToDefaultCriteria`. The CLOSE_CAR button handler then validates children + close date without explicitly re-checking status — the menu filter acts as the gate.

**Web:** `handleCloseCar` validates children (corrections accepted, corrective actions accepted, close date present) but does NOT check that CRSTT is currently 15. If a CAR in status 25 (already closed) is reopened or stale in the browser, CLOSE_CAR could fire again.

**Risk:** P2 — Double-close or close from wrong status. The desktop menu filter prevents this scenario implicitly; web should add an explicit guard.

**Plan:**
- **Layer:** Frontend (guard in `carCustomButtons.js`)
- Add `assertCrentryStatusAllows(executeSQLPromise, strPrmKy, ['15'], AskYesNoMessage)` at the start of `handleCloseCar`

---

### GAP-21 ❌ Missing — CAR Editing Screen: No `VIEW_SOURCE_TXN` Button Handling

**Desktop:** `CNewCarEntryCategory` handles `VIEW_SOURCE_TXN` on `HSE_TGCREDTNG` (CAR Editing screen) — same handler as other Layer 1 screens.

**Web:** `handleCarButton` for `VIEW_SOURCE_TXN` includes `HSE_TGCREDTNG` in the array (line 679). This is CORRECT.

**Status:** ✅ Match — already handled. No gap.

---

### GAP-22 ⚠️ Partial — Layer 1: `onMenuItemClicked` Missing for CAR Review, Actions Review, CAR Approval, Follow-Up, Editing

**Desktop:** Each screen's `ChangeToDefaultCriteria` applies a WHERE filter:
- `HSE_TGCRRVW` (CAR Review): `WHERE CRENTRY_CRSTT IN ('05','02')` — show submitted + rejected-at-review
- `HSE_TGACTNSENTRY` (Actions Entry): `WHERE CRENTRY_CRSTT IN ('10','11','12','06')` — show review-accepted + rejected variants
- `HSE_TGACTNSRVIW` (Actions Review): `WHERE CRENTRY_CRSTT IN ('16','12')` — show actions-complete + rejected-at-actions-review
- `HSE_TGCRAPRVL` (CAR Approval): `WHERE CRENTRY_CRSTT = '19'` — show actions-reviewed
- `HSE_TGCRFLOUP` (CAR Follow-Up): `WHERE CRENTRY_CRSTT = '15'` — show approved
- `HSE_TGCREDTNG` (CAR Editing): no filter (all records)

**Web:** Only `CAR_Entry.js` has `onMenuItemClicked` (department-based filter). The remaining Layer 1 screens (`CAR_Review.js`, `Actions_Entry.js`, `Actions_Review.js`, `CAR_Approval.js`, `CAR_Follow_Up.js`, `CAR_Editing.js`) have NO `onMenuItemClicked` export, meaning they show ALL CAR records regardless of workflow status.

**Risk:** P1 — Users see CARs that are NOT at their stage in the workflow, creating confusion and potential for wrong-stage actions. The transition guards (`assertCrentryStatusAllows`) prevent actual damage, but the UX is poor and error-prone.

**Plan:**
- **Layer:** Frontend (screen handlers)
- Add `onMenuItemClicked` to each of: `CAR_Review.js`, `Actions_Entry.js`, `Actions_Review.js`, `CAR_Approval.js`, `CAR_Follow_Up.js`
- Each should apply the corresponding WHERE clause on `CRENTRY_CRSTT`
- `CAR_Editing.js`: no filter needed (matches desktop)

---

### GAP-23 ⚠️ Partial — CAR Follow-Up: No `SubFieldChanged` for `CRRCTVACTNS_LNKCSS_ACC` on Corrections Tab

**Desktop:** `NewCarEntryCategory::DisplayFieldChange` for `HSE_TGCRFLOUP` handles `CRRCTVACTNS_LNKCSS_ACC` field changes on the Corrections tab — same lock/unlock logic as Actions Review (accepted → lock reject reason, not-accepted → MUST reject reason, set ACTNSENTRY_CRR_ACTSTT = A/R).

**Web:** `CAR_Follow_Up.js` has NO `SubFieldChanged` export. The `applyActionsReviewLnkCssAccChange` utility exists but is only wired in `Actions_Review.js`. When a follow-up reviewer changes a correction's acceptance status on the Follow-Up screen, the status field is NOT auto-updated and the reject reason field is NOT locked/unlocked.

**Risk:** P1 — Follow-up corrections tab does not auto-update ACTSTT or enforce reject reason on non-acceptance, potentially allowing saves with inconsistent correction status/reason combinations.

**Plan:**
- **Layer:** Frontend (screen handler `CAR_Follow_Up.js`)
- Add `SubFieldChanged` export that calls `applyActionsReviewLnkCssAccChange` when `CRRCTVACTNS_LNKCSS_ACC` changes
- Same pattern as `Actions_Review.js`

---

### GAP-24 ⚠️ Partial — CAR Follow-Up: Corrections Actual Follow-Up Date Default Only on CRR Tab

**Desktop:** `DisplayRecordRepostion` defaults `ACTNSENTRY_CRR_ACTFLLDT` to GETDATE() when empty on EACH record reposition within Corrections. It also defaults `HSE_CRRCTVACTNS` actual dates on the corrective actions area when viewed via root causes on follow-up.

**Web:** `applyCarFollowUpCorrectionsActFllDt` only fires for the `CRR` (corrections) tab classification. Corrective actions viewed under root causes on follow-up do NOT get a default actual follow-up date. This means the corrective actions' follow-up dates remain empty unless manually entered.

**Risk:** P2 — Minor inconsistency; corrective actions' actual follow-up dates may need manual entry. Not blocking but diverges from desktop auto-default.

**Plan:**
- **Layer:** Frontend (utility `carFollowUpToolbarParity.js`)
- Investigate if desktop also defaults corrective action dates from follow-up reposition; if yes, extend `applyCarFollowUpCorrectionsActFllDt` for corrective action tab context

---

## SECTION B — LAYER 2: Corrective Action Account (`HSE_CRCTVEACCENT`) Gaps

---

### GAP-25 ❌ Missing — Layer 2 Confirmation: No `onMenuItemClicked` for Default Criteria

**Desktop:** `CarConfirmationCategory::ChangeToDefaultCriteria` applies a WHERE filter showing only completed records (`CRCTVEACCENT_RECSTS = '4'`) that are ready for confirmation.

**Web:** `CAR_Confirmation.js` has NO `onMenuItemClicked` export. All records are shown regardless of status.

**Risk:** P1 — Confirmation reviewers see all records including incomplete, closed, etc.

**Plan:**
- **Layer:** Frontend (screen handler `CAR_Confirmation.js`)
- Add `onMenuItemClicked` with `WHERE CRCTVEACCENT_RECSTS = '4'`

---

### GAP-26 ❌ Missing — Layer 2 Job Verification: No `onMenuItemClicked` for Default Criteria

**Desktop:** `CARJobVrfctnCategory::ChangeToDefaultCriteria` — filters to show records at the HSE verification stage.

**Web:** `CAR_Job_Verification.js` has NO `onMenuItemClicked` export.

**Risk:** P1 — HSE verification staff see all corrective action accounts instead of only those awaiting verification.

**Plan:**
- **Layer:** Frontend (screen handler `CAR_Job_Verification.js`)
- Add `onMenuItemClicked` with appropriate status filter (e.g., records where job is completed but not yet accepted/rejected/cancelled)

---

### GAP-27 ❌ Missing — Layer 2 Follow-Up Visit: No `onMenuItemClicked` for Default Criteria

**Desktop:** `CarFlwupVstCategory::ChangeToDefaultCriteria` — filters to show records awaiting follow-up approval/closure.

**Web:** `CAR_Follow_Up_Visit.js` has NO `onMenuItemClicked` export.

**Risk:** P1 — Follow-up visit staff see all corrective action accounts.

**Plan:**
- **Layer:** Frontend (screen handler `CAR_Follow_Up_Visit.js`)
- Add `onMenuItemClicked` with appropriate status filter

---

### GAP-28 ⚠️ Partial — Layer 2: `sendCARNotAccptdNotification` Only Called from Layer 2 Button Handler

**Desktop:** `CarCategory::setCARNotAcptd` executes `setCARNotAcptd` SP AND calls `sendCARNotAccptdNotification(departmentMail, CARSerial)` to email the department.

**Web:** `handleCarNotAccepted` in `carAccountAcceptReject.js` calls `setCARNotAcptd` SP but does NOT call `sendCARNotAccptdNotification`. The notification email is missing.

**Risk:** P1 — Department is not notified when a CAR is marked "Not Accepted" at the account level.

**Plan:**
- **Layer:** Frontend (utility `carAccountAcceptReject.js`)
- After `setCARNotAcptd` succeeds, look up department email from `CMN_DEPARTMENT` and call `EXECUTE sendCARNotAccptdNotification '<email>','<CARSerial>'`
- Same pattern as `closeCARNotification` in `carLayer2Buttons.js` Follow-Up Visit handler

---

### GAP-29 ⚠️ Partial — Layer 2 Corrective Action Entry: No `SubFieldChanged` for Location Fields Fill-Down

**Desktop:** `CarEntryCategory::DisplayFieldChange` — when `CRCTVEACCENT_CARMODELBAS` changes, it reads source TXN fields (site, department, location, area, description) from the source transaction table and fills them into the account form.

**Web:** `Creative_Access_Entry.js` has a `SubFieldChanged` for `CRCTVEACCENT_CARMODELBAS` that calls `fillFromCarBasis`. However, `fillFromCarBasis` only maps a subset of fields from the source TXN. The exact field mapping depends on `BASIS_QUERY_MAP` in `carEntryFillFromBasis.js`.

**Risk:** P2 — Some auto-fill fields from the desktop may be missing. Need to verify `BASIS_QUERY_MAP` covers all desktop fill-down columns.

**Plan:**
- **Layer:** Frontend (verification in `carEntryFillFromBasis.js`)
- Compare `BASIS_QUERY_MAP` against desktop `CarEntryCategory::FillLocations` field assignments for each CAR basis code
- Add any missing column mappings

---

### GAP-30 ⚠️ Partial — Layer 2 Under Execution: `CRCTVEACCENT_JOBPNDNG` Does Not Re-Check Status After Guard

**Desktop:** `CARUndrExctnCategory` checks `CRCTVEACCENT_RECSTS != '26'` before allowing `CARJobPndng` — preventing double-pending.

**Web:** `handleCarFoundationTxnButtons` checks `CRCTVEACCENT_RECSTS == '26'` and blocks with a message. This is CORRECT but uses string comparison with `trim()`. If the DB column has trailing spaces or different casing, the check may not match.

**Risk:** P2 — Edge case only. Current implementation is functionally correct for clean data.

**Plan:**
- **Layer:** Frontend (hardening)
- Use `LTRIM(RTRIM(...))` in the SQL query (already done) — acceptable as-is

---

## SECTION C — CROSS-MODULE INTEGRATION Gaps

---

### GAP-31 ⚠️ Partial — Observation-Close CAR Generation: No `##TEMP_HSE_TABLE` Context in Web INSERT Path

**Desktop:** Observation close → `closeNearMissTXN` SP → INSERT into `HSE_CRENTRY` with `##TEMP_HSE_TABLE` seeded for trigger context ("Creating new CAR" / "NearMiss Follow-Up").

**Web:** `generateCARFromObservation` in `ObservationButtonHandlers.js` does the INSERT via `executeSQLPromise` but the trigger context (`##TEMP_HSE_TABLE`) seeding may not reach the same connection as the INSERT due to connection pooling.

**Risk:** P2 — Known limitation (same as GAP-5 in prior report). The trigger's audit trail may have empty `ActionDescription`/`SrcScreen` fields.

**Status:** Known limitation — documented in `carEntrySaveDesktopParity.js`. No code change needed.

---

### GAP-32 ⚠️ Partial — Audit CAR Generation: `CopyImages` Error Handling

**Desktop:** `CopyImages` runs synchronously on the same connection after INSERT. Images are always copied.

**Web:** `handleAuditResultConfirmationCarButton` in `auditResultConfirmationCar.js` inserts the CAR then calls `CopyImages` SP. If the PrmKy retrieval after INSERT fails (connection pooling timing), `CopyImages` silently fails.

**Risk:** P2 — Same as prior GAP-6. Images may not be copied for audit-generated CARs.

**Status:** Known limitation — retry mechanism recommended but not critical.

---

### GAP-33 ⚠️ Partial — Site Survey: Button Visibility Toggle Not Implemented

**Desktop:** `SitSrvyRqrdActn.cpp` toggles button visibility based on the current record's state — showing/hiding GENERATECAR, ViewRelatedCAR, UpdtCAR, CnlCAR, WrkAcptd, WrkNotAcptd dynamically using `EnableCustomButton`.

**Web:** `Site_Survey_Required_Actions_Popup.js` calls `handleTxnRequiredActionCarButton` for all button clicks but does NOT implement `ShowScreen` logic to enable/disable buttons based on record state.

**Risk:** P2 — Users see all CAR-related buttons regardless of whether they are applicable to the current record state. The handlers themselves validate prerequisites before acting, so no incorrect action can occur — but UX is suboptimal.

**Plan:**
- **Layer:** Frontend (screen handler `Site_Survey_Required_Actions_Popup.js`)
- Add `ShowScreen` logic that reads the current record's status and calls `setScreenDisableBtn` / `FormEnableButton` for relevant buttons

---

## SECTION D — INFRASTRUCTURE / FRAMEWORK Gaps (Application Layer Only)

---

### GAP-34 ⚠️ Partial — Browse Filter: HSEVRFDBY Depends on `globalThis.__hseDevInterface`

**Desktop:** `DisplayBeforBrowsingButtonClick` filters HSEVRFDBY lookup fields by site → company → owner department chain.

**Web:** `browseEvents.js` / `carBrowsingFilters.js` relies on `globalThis.__hseDevInterface` which may not be set by the shell.

**Risk:** P2 — Browse filter may be non-functional. Not business-critical but affects data quality of lookup selections.

**Plan:**
- **Layer:** Frontend (framework integration)
- Cache `devInterfaceObj` in a module-scoped variable from `setDevInterface` in `buttonEvents.js` and export it for browse events

---

### GAP-35 ⚠️ Partial — `##TEMP_HSE_TABLE` Connection Pool Limitation

**Desktop:** Same-connection temp table for trigger context.

**Web:** Dual-write (`##TEMP_HSE_TABLE` + `SESSION_CONTEXT`) is the best application-layer mitigation.

**Status:** Known limitation — no further action possible without DB trigger modification.

---

## SECTION E — NEWLY IDENTIFIED Gaps (Not in Previous Report)

---

### GAP-36 ❌ Missing — CAR Editing Screen: No `CLOSE_CAR` Status Guard

**Desktop:** `HSE_TGCREDTNG` (CAR Editing) with CLOSE_CAR — the desktop only shows CARs with `CRENTRY_CRSTT = '15'` on the editing screen when the close button is available. The editing screen's `ChangeToDefaultCriteria` does NOT filter by status (shows all), but CLOSE_CAR is context-dependent.

**Web:** `handleCloseCar` is invoked from `HSE_TGCREDTNG` without any status check. A user on CAR Editing could potentially close a CAR in ANY status (01, 05, 10, etc.) if child validations pass.

**Risk:** P0 🔥 — A CAR could be closed from the Editing screen while still in Draft or Review status, completely bypassing the approval workflow.

**Plan:**
- **Layer:** Frontend (guard in `carCustomButtons.js`)
- Add `assertCrentryStatusAllows(executeSQLPromise, strPrmKy, ['15'], AskYesNoMessage)` at the start of `handleCloseCar` (addresses both GAP-20 and GAP-36)

---

### GAP-37 ❌ Missing — Corrective Action Inquiry: No Default Criteria Filter

**Desktop:** `CARInquiry::ChangeToDefaultCriteria` provides filtered views: "View Incomplete" (RECSTS=1), "View Completed" (RECSTS=4), "View Confirmed" (RECSTS=5), "View Closed" (RECSTS=6), etc. The inquiry screen has a menu with sub-items for each filter.

**Web:** `Corrective_Action_Inquiry.js` (both under `Performance/Action Tracking/` and `Inquiry screens/`) has only `ButtonClicked` + `ShowScreen`. No `onMenuItemClicked` and no sub-menu handling for status-filtered views.

**Risk:** P2 — Inquiry shows all records without convenient status filtering. Not business-breaking but reduces usability.

**Plan:**
- **Layer:** Frontend (screen handler)
- Add `onMenuItemClicked` or investigate if the shell supports sub-menu-item differentiation for filtered views

---

### GAP-38 ⚠️ Partial — CAR Follow-Up: `REJECT_CAR` Blocking Confirmation Dialog Inconsistency

**Desktop:** When rejecting from Follow-Up (status stays 15), the desktop shows a blocking `AfxMessageBox` confirmation dialog that must be acknowledged before the reject reason popup opens.

**Web:** `handleCarButton` for `REJECT_CAR` on `HSE_TGCRFLOUP` uses `globalThis.confirm()` with fallback to `AskYesNoQuestion` then `AskYesNoMessage`. The `confirm()` call IS blocking, but `AskYesNoMessage` is NOT blocking — if neither `confirm()` nor `AskYesNoQuestion` is available, the reject proceeds without user confirmation.

**Risk:** P2 — In some shell configurations, the follow-up reject may proceed without confirmation. The existing code attempts all three fallbacks, which is reasonable.

**Status:** Acceptable risk — the current 3-level fallback is a reasonable mitigation.

---

### GAP-39 ⚠️ Partial — ENV-Measure CAR Basis Code (13): VIEW_SOURCE_TXN Not Mapped on Layer 1

**Desktop:** `CNewCarEntryCategory` VIEW_SOURCE_TXN maps CRENTRY_CRSRC text strings to inquiry screens. The map includes all source types: Observation, Audit, Performance, Site Survey, Risk, Drill, Potential Hazard, Vehicle, Loss, Incident.

**Web:** `handleViewSourceTxn` in `carCustomButtons.js` maps most sources but does NOT explicitly handle `ENV-MEASURE` / `ENVIRONMENTAL` source text. If a CAR is sourced from an environmental measure, VIEW_SOURCE_TXN will show the "CAR source not supported" error.

**Risk:** P2 — ENV-Measure sourced CARs cannot navigate to their source transaction from Layer 1 screens. This is a rare CAR source.

**Plan:**
- **Layer:** Frontend (`carCustomButtons.js`)
- Add a handler for `strCARSrc.includes('ENV')` that opens `HSE_TGENVMNTREXCTN` inquiry

---

### GAP-40 ⚠️ Partial — Layer 2 VIEW_SOURCE_TXN: Basis Code 13 (ENV-Measure) Missing in `carViewSourceTxnLayer2.js`

**Desktop:** `CarCategory::OpenTXNInquiryScreen` maps basis code 13 to `HSE_TGENVMNTREXCTN` with key field `ENVMNTRPLAN_YR`.

**Web:** `handleViewSourceTxnLayer2` and/or `BASIS_QUERY_MAP` in `carEntryFillFromBasis.js` — need to verify if basis code 13 is included in the Layer 2 VIEW_SOURCE_TXN mapping.

**Risk:** P2 — Same as GAP-39 but for Layer 2 screens.

**Plan:**
- **Layer:** Frontend (utility)
- Verify and add basis code 13 mapping if missing

---

## Summary Table

| GAP | Area | Priority | Status | Action Required |
|-----|------|----------|--------|-----------------|
| **GAP-15** | CAR Entry header validation on ENTRY_COMPLETED | P1 | ⚠️ Partial | Add header field checks before submit |
| **GAP-16** | Actions Entry SQL parameterization | P2 | ⚠️ Partial | Wrap PrmKy with parseInt in mass-update SQL |
| **GAP-17** | Actions Review target date visual refresh | P2 | ❌ Missing | FormSetField CRENTRY_CRTRGDT after CTE update |
| **GAP-19** | CLOSE_CAR on Actions Review — unexpected | P1 | ⚠️ Partial | Remove HSE_TGACTNSRVIW from CLOSE_CAR handler |
| **GAP-20** | CLOSE_CAR no CRSTT=15 guard | P2 | ⚠️ Partial | Add status 15 check in handleCloseCar |
| **GAP-22** | Layer 1 screens missing onMenuItemClicked | P1 | ❌ Missing | Add status-filtered default criteria to 5 screens |
| **GAP-23** | Follow-Up no SubFieldChanged for corrections ACC | P1 | ⚠️ Partial | Wire applyActionsReviewLnkCssAccChange |
| **GAP-24** | Follow-Up corrective actions date default | P2 | ⚠️ Partial | Investigate and extend if needed |
| **GAP-25** | L2 Confirmation no default criteria | P1 | ❌ Missing | Add onMenuItemClicked |
| **GAP-26** | L2 Job Verification no default criteria | P1 | ❌ Missing | Add onMenuItemClicked |
| **GAP-27** | L2 Follow-Up Visit no default criteria | P1 | ❌ Missing | Add onMenuItemClicked |
| **GAP-28** | L2 CAR Not Accepted email notification | P1 | ⚠️ Partial | Add sendCARNotAccptdNotification call |
| **GAP-29** | L2 Entry fill-from-basis field coverage | P2 | ⚠️ Partial | Verify BASIS_QUERY_MAP completeness |
| **GAP-33** | Site Survey button visibility | P2 | ⚠️ Partial | Add ShowScreen button enable/disable logic |
| **GAP-34** | Browse filter devInterface dependency | P2 | ⚠️ Partial | Cache devInterface for browse events |
| **GAP-36** | CAR Editing CLOSE_CAR no status guard | P0 🔥 | ❌ Missing | Add CRSTT=15 guard to handleCloseCar |
| **GAP-37** | CA Inquiry no status filter views | P2 | ❌ Missing | Add filtered view support |
| **GAP-39** | L1 VIEW_SOURCE_TXN ENV-Measure mapping | P2 | ⚠️ Partial | Add ENV source handler |
| **GAP-40** | L2 VIEW_SOURCE_TXN basis code 13 | P2 | ⚠️ Partial | Verify and add mapping |

---

## Detailed Execution Plan (Priority Order)

### Phase 0 — P0 Critical

**1. GAP-36 + GAP-20: `handleCloseCar` Status Guard**
- File: `src/utils/carCustomButtons.js`
- Add `assertCrentryStatusAllows(executeSQLPromise, strPrmKy, ['15'], AskYesNoMessage)` at start of `handleCloseCar`
- This single change addresses BOTH GAP-36 (Editing screen close bypass) and GAP-20 (double-close prevention)

### Phase 1 — P1 Must-Fix

**2. GAP-19: Remove Actions Review from CLOSE_CAR**
- File: `src/utils/carCustomButtons.js`
- Change `['HSE_TGACTNSRVIW', 'HSE_TGCRFLOUP', 'HSE_TGCREDTNG']` to `['HSE_TGCRFLOUP', 'HSE_TGCREDTNG']`

**3. GAP-22: Layer 1 Screen Default Criteria**
- Files: `CAR_Review.js`, `Actions_Entry.js`, `Actions_Review.js`, `CAR_Approval.js`, `CAR_Follow_Up.js`
- Add `onMenuItemClicked` with status WHERE clauses to each

**4. GAP-23: Follow-Up SubFieldChanged for Corrections**
- File: `src/screenHandlers/Performance/Action Tracking/CAR_Follow_Up.js`
- Add `SubFieldChanged` export wiring `applyActionsReviewLnkCssAccChange`

**5. GAP-15: CAR Entry Header Validation**
- File: `src/utils/carCustomButtons.js`
- Add header field checks in `handleEntryCompletedCarEntry`

**6. GAP-25/26/27: Layer 2 Screen Default Criteria**
- Files: `CAR_Confirmation.js`, `CAR_Job_Verification.js`, `CAR_Follow_Up_Visit.js`
- Add `onMenuItemClicked` with appropriate status filters

**7. GAP-28: CAR Not Accepted Email**
- File: `src/utils/carAccountAcceptReject.js`
- Add `sendCARNotAccptdNotification` call after SP

### Phase 2 — P2 Hardening

**8. GAP-16: SQL Parameterization Hardening**
**9. GAP-17: Target Date Visual Refresh**
**10. GAP-24: Corrective Actions Follow-Up Date Default**
**11. GAP-29: Fill-from-Basis Field Verification**
**12. GAP-33: Site Survey Button Visibility**
**13. GAP-34: Browse Filter devInterface Cache**
**14. GAP-37: Inquiry Status Filters**
**15. GAP-39/40: ENV-Measure Source Mapping**

---

*This document was generated from exhaustive code analysis of the current web implementation against the legacy C++ desktop baseline. Validate against runtime behavior before production deployment.*
