# Observation Process – Web Implementation Validation

> **Reference**: Activity Diagram §2 from `Observation_Reporting_Process_UML.md`
> **Date**: March 2026

---

## Summary

| Phase | Status | Details |
|-------|--------|---------|
| **Phase 1: Observation Entry** | ✅ Implemented | All activities present |
| **Phase 2: Observation Review** | ⚠️ Partial gap | Accept flow missing reject-reason deletion |
| **Phase 3: Observation Approval** | ✅ Implemented | All activities present |
| **Phase 4: CAR Generation Decision** | ❌ **MISSING** | Client-side CAR auto-generation not implemented |
| **Phase 5: CAR Entry** | ✅ Implemented | Entry Completed with tracing |
| **Phase 6: CAR Review** | ⚠️ Partial gap | Accept/Reject don't trigger status change via `ChangeEntityStatus` |
| **Phase 7: CAR Approval** | ⚠️ Partial gap | Approve/Reject/Cancel don't trigger status change via `ChangeEntityStatus` |
| **Phase 8: Corrective Actions** | ✅ Implemented | Entry + Review with validations |
| **Phase 9: CAR Follow-up & Close** | ✅ Implemented | Close with validations |

**Critical missing items: 3** | **Partial gaps: 4** | **Fully implemented: 12 of 19 activities**

---

## Detailed Validation by Activity

### Phase 1: Observation Entry

| Activity | Diagram | Web Code | Status |
|----------|---------|----------|--------|
| A1: Create new observation | NEW button on Entry screen | Framework handles `NEW` via toolbar; `Observation_Entry.js` registered | ✅ Implemented |
| A2: Fill details (type, location, description, hazards, causes, images) | User fills form fields | Framework field rendering; attachments, causes, required actions tabs | ✅ Implemented |
| A3: Save record (Status = 1) | SAVE button | Framework handles `SAVE` | ✅ Implemented |
| A4: Ready to submit? | User decision | N/A (user action) | ✅ N/A |
| A5: Click Complete (`NRSTMISCENT_ENTCMPLT`) | Custom button | `handleEntryCompleteButton` in `ObservationButtonHandlers.js` (line 787) | ✅ Implemented |
| A5a: Validate record saved | `pMultiSelectedRows->lCount > 0` | Checks `fullRecordArrKeys.length` (line 817–823) | ✅ Implemented |
| A5b: Update `NRSTMISCENT_RPRDSC` (RQ-2-2024.14) | `UPDATE HSE_VWNRSTMISCENT SET NRSTMISCENT_RPRDSC` | SQL UPDATE executed (line 891) | ✅ Implemented |
| A6: `completeNearMissTXN` (Status 1→4) | `EXECUTE completeNearMissTXN` | SP called with keyValue, screenName, userName (line 926) | ✅ Implemented |
| A7: Tracing: Entry Completed | SP handles internally | SP handles internally | ✅ Implemented |
| A7a: Refresh screen | `RefreshScreen("",REFRESH_SELECTED)` | `refreshData('', 'REFRESH_SELECTED')` (line 932) | ✅ Implemented |

**Phase 1 verdict: ✅ Fully implemented**

---

### Phase 2: Observation Review

| Activity | Diagram | Web Code | Status |
|----------|---------|----------|--------|
| B1: Reviewer opens Review screen | Load observations (Status=4) | Framework loads screen; `Observation_Review.js` registered | ✅ Implemented |
| **B3: Click Reject (`NRSTMISCENT_RJC`)** | Custom button | `handleRejectButton` (line 148) | ✅ Implemented |
| B4: Enter rejection reason | Opens Reject Reason dialog | Opens `HSE_TGRjctRsn` with criteria and defValObj (line 270) | ✅ Implemented |
| B4a: Store pending reject info | C++: `setgbUpdateRejectedRecord()` flag | `setPendingRejectObservation(...)` stores context (line 252) | ✅ Implemented |
| B5: `rejectObservation` (Status 4→3) | `EXECUTE rejectObservation` after OK click | `handleRejectReasonOkButton` executes SP when `RJCTRSN_BTN_OK` clicked (line 1140) | ✅ Implemented |
| B6: Tracing: Rejected from Review | SP handles | SP handles | ✅ Implemented |
| B7: Record returns to Entry | Status change | Status 3 sent back | ✅ Implemented |
| **B8: Click Accept (`NRSTMISCENT_ACP`)** | Custom button | `handleConfirmButton` (line 296) | ✅ Implemented |
| B8a: Save first | C++: `DoToolBarAction(TOOLBAR_SAVE)` | **Not present** – web does NOT call `doToolbarAction('SAVE')` before confirm | ⚠️ **Missing** |
| B9: Reject reasons exist? | `IsNewRjcRsnExist` | SQL `SELECT COUNT(0) FROM HSE_RJCTRSN WHERE ...` (line 354) | ✅ Implemented |
| B10: Prompt: Delete reject reasons? | `AfxMessageBox(IDS_RJCT_RSN_EXST)` | `AskYesNoMessage('Prompt', ...)` (line 389) | ✅ Implemented |
| **B11: Delete reject reasons** | C++: Deletes reject reasons before SP | **Web does NOT delete reject reasons** – proceeds directly to `confirmNearMissTXN` | ❌ **Missing** |
| B12: Abort accept (user says No) | Returns | Returns early (line 392) | ✅ Implemented |
| B13: `confirmNearMissTXN` (Status 4→5) | `EXECUTE confirmNearMissTXN` | SP called (line 397) | ✅ Implemented |
| B14: Tracing: Confirmed | SP handles | SP handles | ✅ Implemented |

**Phase 2 verdict: ⚠️ Two gaps:**
1. **Save before Accept is missing** – C++ calls `DoToolBarAction(TOOLBAR_SAVE)` before checking reject reasons; web skips this.
2. **Reject reason deletion is missing** – When user confirms "Yes, delete reject reasons", the web does NOT execute `DELETE FROM HSE_RJCTRSN` before calling `confirmNearMissTXN`. The C++ explicitly deletes them. This means orphaned reject reasons remain in the database.

---

### Phase 3: Observation Approval

| Activity | Diagram | Web Code | Status |
|----------|---------|----------|--------|
| C1: Approver opens Approval screen | Load observations (Status=5) | Framework loads; `Observation_Approval.js` registered | ✅ Implemented |
| C2: Close or Reject decision | Buttons | Both buttons handled in `sendButtonClickToBackend` | ✅ Implemented |
| C3–C6: Reject flow | Same as Phase 2 Reject | Same `handleRejectButton` with module type `NRSTMISCENT-L2` (line 211) | ✅ Implemented |
| C7: Set Require CAR = Y or N | User sets field | Framework field (user edits before clicking Close) | ✅ Implemented |
| C8: Click Close (`NRSTMISCENT_CLS`) | Custom button | `handleCloseButton` (line 968) | ✅ Implemented |
| C9: Save first | `DoToolBarAction(TOOLBAR_SAVE)` | `doToolbarAction('SAVE', formTag, '')` (line 997) | ✅ Implemented |
| C10: Key field valid? | Check `KeyFieldValue` | Checks `keyFieldValue` with fallback reads (line 1013–1033) | ✅ Implemented |
| C11: Abort close | `RefreshScreen` only | `refreshData('', 'REFRESH_SELECTED')` + return (line 1031) | ✅ Implemented |
| C12: Employee code found? | `GetEmployeeCodeForLoginUser()` | `getEmployeeCodeForLoginUser()` (line 1037) with BUG_HSE_HSM_14_3_26 | ✅ Implemented |
| C13: `closeNearMissTXN` (Status 5→6) | `EXECUTE closeNearMissTXN` | SP called (line 1060) | ✅ Implemented |
| C14: Tracing: Closed | SP handles | SP handles | ✅ Implemented |

**Phase 3 verdict: ✅ Fully implemented** (Observation close itself works correctly)

---

### Phase 4: CAR Generation Decision — ❌ CRITICAL MISSING

| Activity | Diagram | Web Code | Status |
|----------|---------|----------|--------|
| **D1: Check Require CAR = Y?** | C++: `FormGetFieldEx(m_strFormTag,"NRSTMISCENT_RQRCR")` | **Not implemented** – web does NOT read `NRSTMISCENT_RQRCR` after close | ❌ **Missing** |
| **D3: Auto-generate CAR** | C++: Direct `INSERT INTO HSE_CRENTRY` | **Not implemented** – no INSERT logic exists in web | ❌ **Missing** |
| **D4: Link CAR to observation** | `CRENTRY_TXNYR = OBS Year, CRENTRY_TXNN = OBS Number` | **Not implemented** | ❌ **Missing** |
| **D5: Set CAR fields from observation + policy** | Reads policy fields (`HSEOBSRVTN_GnrCrStt`, `HSEOBSRVTN_CrBss`) | **Not implemented** | ❌ **Missing** |
| **D6: Copy images** | `EXEC CopyImages` from observation to CAR | **Not implemented** | ❌ **Missing** |
| **D7: Toast "CAR generated"** | `AfxMessageBox("CAR is generated with CAR No. = ...")` | **Not implemented** | ❌ **Missing** |
| **D8: Tracing: CAR Created** | Insert into tracing | **Not implemented** | ❌ **Missing** |

**Phase 4 verdict: ❌ Entirely missing**

The web `handleCloseButton` (line 968–1078) calls `closeNearMissTXN` and then immediately refreshes. The comment at line 1065 states:
> "Desktop does not insert tracing after SP – closeNearMissTXN does status + tracing (and CAR if required)"

**This comment is incorrect.** The C++ source code (`NearMissFollowUpCategory.cpp` lines 69–230) clearly shows that:
1. `closeNearMissTXN` only handles the observation status change (5→6) and tracing for that status change.
2. CAR generation is performed **entirely client-side** in the C++ code **after** `closeNearMissTXN` returns.
3. The C++ reads `NRSTMISCENT_RQRCR`, and if `"Y"`, executes a direct `INSERT INTO HSE_CRENTRY` with fields from the observation, policy, and `GetNextCarNumForYear()`.

**Unless the stored procedure `closeNearMissTXN` was modified on the database server to include CAR generation logic** (which we cannot confirm from the codebase), this functionality is completely absent from the web application.

---

### Phase 5: CAR Entry

| Activity | Diagram | Web Code | Status |
|----------|---------|----------|--------|
| E1: CAR Owner opens CAR Entry | HSE_TGCRENTRY | `CAR_Entry.js` screen handler registered | ✅ Implemented |
| E2: Review/edit data | Framework fields | Framework handles editing | ✅ Implemented |
| E3: Click Entry Completed | `ENTRY_COMPLETED` button | `handleEntryCompletedCarEntry` in `carCustomButtons.js` (line 115) | ✅ Implemented |
| E4: Status 01→05, Tracing | Insert tracing + set status | Inserts into `HSE_CRENTRY_TRC`, sets `CRENTRY_CRSTT` to `'05'`, saves (lines 122–124) | ✅ Implemented |

**Phase 5 verdict: ✅ Fully implemented**

---

### Phase 6: CAR Review

| Activity | Diagram | Web Code | Status |
|----------|---------|----------|--------|
| F1: CAR Reviewer opens CAR Review | HSE_TGCRRVW | `CAR_Review.js` screen handler registered | ✅ Implemented |
| **F2a: Accept CAR (ACCEPT_CAR)** | C++: `ChangeEntityStatus` → status 10 + tracing | Web only opens CAR Review Info screen (line 310–312). **Does not call `ChangeEntityStatus` or set status to 10** | ⚠️ **Partial** |
| **F2b: Reject CAR (REJECT_CAR)** | C++: `ChangeEntityStatus` → status 02 + tracing | Web opens reject reason screen (line 252–257) but **has no pending mechanism to execute `ChangeEntityStatus` for CAR** when OK is clicked | ⚠️ **Partial** |

**Phase 6 verdict: ⚠️ Partial gaps**

In C++:
- `ACCEPT_CAR` on `HSE_TGCRRVW` calls `InsertIntoTracingTabProcess` which executes `ChangeEntityStatus` to set status=10.
- `REJECT_CAR` calls `ChangeEntityStatus` to set status=02.

In the web:
- `ACCEPT_CAR` on `HSE_TGCRRVW` only opens the Review Info sub-screen. The actual status change (05→10) is **not triggered**.
- `REJECT_CAR` calls `openRejectReasonScreen()` which opens the popup, but there is **no pending reject handler for CAR module**. The observation module uses `setPendingRejectObservation` + `handleRejectReasonOkButton`, but the CAR reject flow does not set any pending action. When the user clicks OK on the reject reason screen for a CAR, `handleRejectReasonOkButton` finds `pendingRejectObservation === null` and returns without doing anything.

---

### Phase 7: CAR Approval

| Activity | Diagram | Web Code | Status |
|----------|---------|----------|--------|
| G1: CAR Approver opens CAR Approval | HSE_TGCRAPRVL | `CAR_Approval.js` screen handler registered | ✅ Implemented |
| **G2a: Approve CAR (APPROVE_CAR)** | C++: `ChangeEntityStatus` → status 15 + tracing | Web only opens CAR Approve Info screen (line 344–348). **Does not set status 15** | ⚠️ **Partial** |
| **G2b: Reject CAR (REJECT_CAR)** | C++: `ChangeEntityStatus` → status 06 | Opens reject reason screen but **no status change mechanism** (same issue as Phase 6) | ⚠️ **Partial** |
| **G2c: Cancel CAR (CANCEL_CAR)** | C++: `ChangeEntityStatus` → status 20 + tracing | Opens reject reason screen (line 295–299) but **does not set status 20** | ⚠️ **Partial** |

**Phase 7 verdict: ⚠️ Partial gaps** – All three actions only open sub-screens; none trigger `ChangeEntityStatus`.

---

### Phase 8: Corrective Actions

| Activity | Diagram | Web Code | Status |
|----------|---------|----------|--------|
| H1: Actions Entry | HSE_TGACTNSENTRY | `Actions_Entry.js` registered | ✅ Implemented |
| H2: Define corrective actions | Framework fields | Framework handles | ✅ Implemented |
| H3: Entry Completed | `ENTRY_COMPLETED` | `handleEntryCompletedActionsEntry` (line 133) | ✅ Implemented |
| H3a: Validate root causes | `COUNT(*) FROM HSE_ACTNSENTRY_RtCss` | Validated (line 145) | ✅ Implemented |
| H3b: Validate each root cause has actions | Cross-table check | Validated (line 149) | ✅ Implemented |
| H3c: Validate corrections exist | `COUNT(*) FROM HSE_ACTNSENTRY_CRR` | Validated (line 153) | ✅ Implemented |
| H4: Status 15→16 + tracing | Insert tracing, set status, update flags | Inserts tracing, sets status '16', updates acceptance flags (lines 164–168) | ✅ Implemented |
| H5: Actions Review | HSE_TGACTNSRVIW | `Actions_Review.js` registered | ✅ Implemented |
| **H6a: Accept Actions (ACCEPT_CAR on HSE_TGACTNSRVIW)** | Validate + status 19 + target date update | Full validation, tracing, status 19, target date update (lines 314–341) | ✅ Implemented |
| **H6b: Reject Actions** | `ChangeEntityStatus` → status 12 | Opens reject reason screen but **no status change to 12** (same pending-reject gap) | ⚠️ **Partial** |

**Phase 8 verdict: ⚠️ Mostly implemented, one gap on Reject**

---

### Phase 9: CAR Follow-up & Close

| Activity | Diagram | Web Code | Status |
|----------|---------|----------|--------|
| I1: Follow-up officer opens CAR Follow-up | HSE_TGCRFLOUP | `CAR_Follow_Up.js` registered | ✅ Implemented |
| I2: Verify corrections accepted | `UPDATE ... WHERE ... ACTACC <> 'Y'` | Validation via update rowcount check (line 188) | ✅ Implemented |
| I2a: Verify corrective actions accepted | Cross-table check | Validation (line 193) | ✅ Implemented |
| I4: Set CAR Closed Date | Check `CRENTRY_CRCLSDT` | Checks close date not empty (line 199) | ✅ Implemented |
| I5: Click Close CAR | `CLOSE_CAR` | `handleCloseCar` (line 177) | ✅ Implemented |
| I6: Status 19→25 + Save | `FormSetField CRENTRY_CRSTT = 25` + SAVE | Sets status '25' and saves (lines 205–206) | ✅ Implemented |

**Phase 9 verdict: ✅ Fully implemented**

---

## Gap Summary

### ❌ Critical Gaps (Functionality Completely Missing)

| # | Gap | Impact | Diagram Activity |
|---|-----|--------|-----------------|
| **1** | **CAR auto-generation from observation** | When an observation is closed with "Require CAR = Y", no CAR record is created. The entire Phase 4 is missing. Users must manually create CARs. | D1–D8 |
| **2** | **CAR REJECT_CAR has no status-change mechanism** | When a user rejects a CAR from Review, Approval, Actions Entry, Actions Review, or Follow-up, the reject reason screen opens but clicking OK does NOT execute `ChangeEntityStatus`. The CAR status never changes. | F2b, G2b, H6b |
| **3** | **CAR CANCEL_CAR has no status-change mechanism** | Same as above – Cancel on CAR Approval opens the screen but never sets status 20. | G2c |

### ⚠️ Partial Gaps (Logic Partially Missing)

| # | Gap | Impact | Diagram Activity |
|---|-----|--------|-----------------|
| **4** | **Accept on Review does not delete reject reasons** | In C++, when the reviewer accepts and acknowledges existing reject reasons, those reasons are deleted before calling `confirmNearMissTXN`. The web skips this deletion, leaving orphaned reject reasons in `HSE_RJCTRSN`. | B11 |
| **5** | **Accept on Review missing Save-before-confirm** | C++ calls `DoToolBarAction(TOOLBAR_SAVE)` before checking reject reasons and confirming. Web does not save first. | B8a |
| **6** | **CAR ACCEPT_CAR (Review) only opens info screen** | C++ changes status from 05→10 via `ChangeEntityStatus`. Web only opens the Review Info sub-screen without changing status. | F2a |
| **7** | **CAR APPROVE_CAR only opens info screen** | C++ changes status from 10→15 via `ChangeEntityStatus`. Web only opens the Approve Info sub-screen without changing status. | G2a |

---

## Root Cause Analysis

### Gap 1: CAR Auto-Generation

The web code comment at `ObservationButtonHandlers.js:1065` states:
> *"closeNearMissTXN does status + tracing (and CAR if required)"*

This assumption is **incorrect** based on the C++ source. In the desktop application, `closeNearMissTXN` only changes the observation status and inserts tracing. CAR generation is entirely **client-side C++ code** in `NearMissFollowUpCategory::DisplayCustomButtonClicked` (lines 69–230), which:
1. Reads `NRSTMISCENT_RQRCR` from the form
2. Reads policy fields (`HSEOBSRVTN_GnrCrStt`, `HSEOBSRVTN_CrBss`)
3. Calls `GetNextCarNumForYear()`
4. Executes `INSERT INTO HSE_CRENTRY` with ~20 fields
5. Calls `EXEC CopyImages`
6. Shows a success message

### Gaps 2–3 & 6–7: CAR Status Changes via `ChangeEntityStatus`

The CAR module uses `ChangeEntityStatus` stored procedure for most status transitions (Accept, Approve, Reject, Cancel). In the web:
- The Observation module has a `pendingRejectObservation` mechanism that stores context and executes the SP when the reject-reason OK button is clicked.
- The CAR module has **no equivalent mechanism**. It opens sub-screens (Review Info, Approve Info, Reject Reason) but does not hook into any callback to execute the status change.
- Additionally, `handleRejectReasonOkButton` in `ObservationButtonHandlers.js` only supports module types: OBSERVATION, PTW, INCIDENT, RISK, VEHICLE_ACCIDENT, POTENTIAL_HAZARD, LOSS_ACCIDENT. There is **no CAR case**.

---

## Recommendations

1. **Implement client-side CAR generation** in `handleCloseButton` after `closeNearMissTXN` succeeds, mirroring the C++ logic.
2. **Add a pending-reject mechanism for CAR** (similar to the observation module) so that `REJECT_CAR` and `CANCEL_CAR` properly execute `ChangeEntityStatus` when the reject reason OK button is clicked.
3. **Implement ACCEPT_CAR status change** on Review screen: after opening Review Info, execute `ChangeEntityStatus` to set status 10.
4. **Implement APPROVE_CAR status change** on Approval screen: after opening Approve Info, execute `ChangeEntityStatus` to set status 15.
5. **Add reject-reason deletion** in `handleConfirmButton` when the user confirms deletion.
6. **Add save-before-confirm** in `handleConfirmButton` to match desktop behavior.

---

*End of validation report*
