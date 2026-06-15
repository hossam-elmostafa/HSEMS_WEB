# CARs Gap Implementation Report — RQ_HSE_12_4_26_00_40

**Date:** 2026-04-12  
**Constraint:** NO database changes. All fixes at application layer only.  
**Marker:** Every change tagged with `RQ_HSE_12_4_26_00_40`

---

## Summary of Changes

| GAP | Priority | Status | Files Modified |
|-----|----------|--------|----------------|
| **GAP-2** | P0 | ✅ Done | `screenHandlers/index.js` |
| **GAP-1+12** | P0 | ✅ Done | NEW: `Corrective_Actions_Popup.js`, `screenHandlers/index.js` |
| **GAP-5** | P0 | ✅ Done | REWRITTEN: `auditResultConfirmationCar.js` |
| **GAP-6/9** | P1 | ✅ Done | `carCustomButtons.js` |
| **GAP-8** | P1 | ✅ Done | `carCustomButtons.js` |
| **GAP-10** | P1 | ✅ Done | `carCustomButtons.js` (merged with GAP-6/9) |
| **GAP-3** | P1 | ✅ Done | REWRITTEN: `Corrective_Action_Inquiry.js` (AT), `screenHandlers/index.js` |
| **GAP-4** | P2 | ✅ Done | `browseEvents.js` |
| **GAP-11** | P2 | ✅ Documented | `carEntrySaveDesktopParity.js` (known limitation) |
| **GAP-14** | P2 | ✅ Already OK | No changes needed (already wrapped in try/catch) |

---

## Phase 1 — P0 Critical Fixes

### GAP-2: Register Layer 2 Screen Handlers

**File:** `src/screenHandlers/index.js`  
**Change:** Added imports and registry entries for 3 Layer 2 screens:

| Screen Tag | Handler File | Desktop Class |
|------------|-------------|---------------|
| `HSE_TgCrctveAccCnfrmtn` | `CAR_Confirmation.js` | `CarConfirmationCategory` |
| `HSE_TgCrctveAccJobVrfctn` | `CAR_Job_Verification.js` | `CARJobVrfctnCategory` |
| `HSE_TgCARFlwUpVsts` | `CAR_Follow_Up_Visit.js` | `CarFlwupVstCategory` |

**Impact:** These screens are now loadable from the menu. Previously the handler files existed but were not registered.

---

### GAP-1 + GAP-12: Corrective Actions Popup Screen Handler

**NEW File:** `src/screenHandlers/Performance/Action Tracking/Corrective_Actions_Popup.js`  
**File:** `src/screenHandlers/index.js`

**What:** Created a unified popup screen handler that wires `carCorrectiveActionPopupButtons.js` into all 4 popup tag variants:
- `HSE_TGCRRCTVACTNS` (generic)
- `HSE_TgCrrctvActns_Rvw` (review context)
- `HSE_TgCrrctvActns_ActEnt` (actions entry context)
- `HSE_TgCrrctvActnsFlwUp` (follow-up context)

**Exports:**
- `ButtonClicked` → delegates to `handleCorrectiveActionPopupButton` (REJECT_ACTION, ACCEPT_ACTION, KEEP_ACTION_PENDING, ACTION_COMPLETED, VIEW_SOURCE_CAR)
- `SubFieldChanged` → delegates to `applyCorrectiveActionPopupFieldChange` (CRRCTVACTNS_ACC, CRRCTVACTNS_ACTACC rules)

**Impact:** Popup buttons are now functional. Previously the utility code existed but no screen handler connected it.

---

### GAP-5: Audit Result Confirmation CAR Generation

**REWRITTEN File:** `src/utils/auditResultConfirmationCar.js`

**Problem:** Old version called `dbo.sp_HSE_AuditConfirmation_GenerateCARs_RQ_HSE_12_4_26_00_40` which does NOT exist in the database and cannot be created (constraint).

**Fix:** Rewrote to use client-side INSERT logic, following the proven pattern from `generateCARFromObservation`:
1. Fetch qualifying findings from `HSE_CLUS_LVLRSULTPPUP`
2. Read audit policy (`HSE_HSEPLC_ADT`) for CAR status and source
3. Read owner department from `HSE_HSEPLC`
4. Resolve employee from login user
5. For each finding: allocate serial via `getNextAllocatedCrentryCrn` + `getNextAllocatedCrentryCrsrln`, INSERT into `HSE_CRENTRY`
6. Retry up to 5 times on duplicate key errors
7. CopyImages per finding (best-effort, wrapped in try/catch)
8. Seed `##TEMP_HSE_TABLE` + `SESSION_CONTEXT` for trigger context

---

## Phase 2 — P1 Significant Fixes

### GAP-6/9/10: Actions Entry Re-submit Guard

**File:** `src/utils/carCustomButtons.js`  
**Function:** `handleEntryCompletedActionsEntry`

**Before:** `assertCrentryStatusAllows(executeSQLPromise, strPrmKy, ['10'], AskYesNoMessage)`  
**After:** `assertCrentryStatusAllows(executeSQLPromise, strPrmKy, ['10', '11', '12', '06'], AskYesNoMessage)`

**Rationale:**
- `10` = first-time entry after Review Accept (existing)
- `11` = re-entry after Actions Entry rejection (GAP-6)
- `12` = re-entry after Actions Review rejection (GAP-9)
- `06` = re-entry after Approval rejection (GAP-10)

**Desktop state transitions enabled:** S11→S16, S12→S16, S06→S16

---

### GAP-8: CAR Entry Re-submit Guard

**File:** `src/utils/carCustomButtons.js`  
**Function:** `handleEntryCompletedCarEntry`

**Before:** `assertCrentryStatusAllows(executeSQLPromise, strPrmKy, ['01'], AskYesNoMessage)`  
**After:** `assertCrentryStatusAllows(executeSQLPromise, strPrmKy, ['01', '02'], AskYesNoMessage)`

**Rationale:** Desktop allows re-submission from draft (`01`) AND from review-rejected (`02`) state.

**Desktop state transitions enabled:** S01→S05, S02→S05

---

### GAP-3: Corrective Action Inquiry (Action Tracking)

**REWRITTEN File:** `src/screenHandlers/Performance/Action Tracking/Corrective_Action_Inquiry.js`  
**File:** `src/screenHandlers/index.js`

**Changes:**
1. Fixed `ChangeCriteria` call signature from 1 arg to 3 args (matching framework convention)
2. Added support for both button naming conventions (`CRCTVEACCENT_VW*` and `CRCTVEACCINQ_VIEW*`)
3. Registered under tag `HSE_TgCrctveAccInq_AT` in `index.js`
4. Set `setScreenDisableBtn(true, false, true)` to disable NEW/DELETE (read-only inquiry)

---

## Phase 3 — P2 Minor Fixes

### GAP-4: Browsing Filters

**File:** `src/events/browseEvents.js`

**What:** Wired `carBrowsingFilters.js::getHseVerifiedByBrowseFilter` into the global `OnBeforeBrowseButtonClick` event handler.

**Logic:**
1. Detects HSEVRFDBY fields on CAR screens (`HSE_TGCRCTVEACCJOBVRFCTN`, `HSE_TGACTNSUNDREXEC`, `HSE_TGCRCTVEACCCNFRMTN`, `HSE_TGCARFLWUPVSTS`)
2. Reads site value from the current record
3. Queries company from site, owner department from policy
4. Appends `EMPLOYEE_COMPANY = x AND EMPLOYEE_WSHOP = y` to `brwsObj.BrowsingSQL`

**Impact:** HSEVRFDBY browse now shows filtered employees by site/company/department chain.

---

### GAP-11: Temp Table Limitation (Documented)

**File:** `src/utils/carEntrySaveDesktopParity.js`

Updated header comment to clearly document the known limitation:
- `##TEMP_HSE_TABLE` may not be visible to triggers under connection pooling
- `SESSION_CONTEXT` fallback is set but triggers may not read it
- Database cannot be modified per constraint
- Audit trail may have empty `ActionDescription`/`SrcScreen` fields

---

### GAP-14: CopyImages (No Change Needed)

Already wrapped in try/catch in `ObservationButtonHandlers.js:1204-1213`. Image copy failure is logged but does not prevent CAR creation.

---

## Files Changed Summary

| File | Action | GAPs |
|------|--------|------|
| `src/screenHandlers/index.js` | UPDATED | GAP-2, GAP-1/12, GAP-3 |
| `src/screenHandlers/Performance/Action Tracking/Corrective_Actions_Popup.js` | NEW | GAP-1/12 |
| `src/utils/auditResultConfirmationCar.js` | REWRITTEN | GAP-5 |
| `src/utils/carCustomButtons.js` | UPDATED | GAP-6/9, GAP-8, GAP-10 |
| `src/screenHandlers/Performance/Action Tracking/Corrective_Action_Inquiry.js` | REWRITTEN | GAP-3 |
| `src/events/browseEvents.js` | UPDATED | GAP-4 |
| `src/utils/carEntrySaveDesktopParity.js` | UPDATED (comment) | GAP-11 |

---

## Testing Strategy

### P0 Tests (Critical)

1. **Layer 2 Screens:** Navigate to Confirmation, Job Verification, Follow-Up Visit from menu → verify screens load with correct buttons
2. **Corrective Actions Popup:** Open popup from any CAR screen → verify REJECT, ACCEPT, KEEP PENDING, COMPLETE, VIEW SOURCE buttons work
3. **Audit CAR Generation:** Confirm audit results → verify CARs are inserted into `HSE_CRENTRY` with correct policy values

### P1 Tests (Significant)

4. **CAR Entry re-submit:** Create CAR (01), get rejected at review (02) → verify ENTRY_COMPLETED works at 02
5. **Actions Entry re-submit from 11:** Submit at Actions Entry, get rejected (11) → verify ENTRY_COMPLETED re-submits
6. **Actions Entry re-submit from 12:** Submit at Actions Review, get rejected (12) → verify ENTRY_COMPLETED on Actions Entry re-submits
7. **Actions Entry re-submit from 06:** Submit at Approval, get rejected (06) → verify ENTRY_COMPLETED on Actions Entry re-submits
8. **Inquiry filters:** Open Corrective Action Inquiry (Action Tracking) → verify all 6 filter buttons change criteria

### P2 Tests (Minor)

9. **Browse filter:** On Job Verification/Under Execution screens, browse HSEVRFDBY field → verify filtered by site/company/dept
10. **CopyImages resilience:** Generate CAR from observation where image source is empty → verify CAR still created

---

*All changes tagged with `RQ_HSE_12_4_26_00_40`. No database modifications made.*
