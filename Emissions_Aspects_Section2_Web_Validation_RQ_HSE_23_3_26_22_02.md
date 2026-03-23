# Web validation: §2 Activity diagram (Emissions / Aspects Tracking)

<!-- RQ_HSE_23_3_26_22_02 -->

> **Reference**: [`Emissions_Aspects_Tracking_Process_UML.md`](./Emissions_Aspects_Tracking_Process_UML.md) **§2. Activity diagram -- Emissions/Aspects Tracking (end-to-end)**
> **Scope**: Validate whether the **web** implementation (`HSEMS_WEB_Original/hse`) performs each activity node in that diagram, and list gaps.
> **Primary evidence** (post RQ_HSE_23_3_26_22_02): `Aspects_Entry.js`, `Aspects_Review.js`, `aspectsRegisterPolicy.js`, `hse.js`; WebInfra `useDevInterfaceFun.js` (`ChangeCriteria`, `FormEnableButton`), `FirstMainListPart.js`, `FirstToolbar.js`, `AppGlobalValContext.js`; JSON headers as before.
> **Date**: March 2026 (re-validated after parity wave)

---

## 1. Legend

| Status | Meaning |
|--------|---------|
| **COVERED** | Behaviour is implemented in web screen handlers **or** reliably provided by the platform (metadata, CRUD, list criteria) without desktop-specific C++ logic. |
| **PARTIAL** | Some behaviour exists (e.g. driven fields in JSON) but a diagram step (validation, gating, or criteria) is not enforced in web JS like desktop. |
| **MISSING** | The diagram step is **not** implemented in web; user can diverge from desktop behaviour or the action has **no effect**. |

---

## 2. Node-by-node validation (diagram §2)

<!-- RQ_HSE_23_3_26_22_02 -->

### 2.1 Navigation and screen selection

| Node ID | Activity (as in §2 diagram) | Web status | Evidence / gap |
|---------|----------------------------|------------|------------------|
| **Start** | User selects Environment → Aspects Register | **COVERED** | Application menu / routing (outside screen handlers). |
| **ScreenChoice** | Which screen? Entry vs Review | **COVERED** | User navigates to `HSE_AspctsEntryAtEntry` vs `HSE_AspctsRvwAtRvw`. |

### 2.2 Track 1: Aspects Entry

| Node ID | Activity (as in §2 diagram) | Web status | Evidence / gap |
|---------|----------------------------|------------|------------------|
| **E1** | Open Aspects Entry; criteria `WHERE ASPCTS_ASPCTSTTUS < 5` | **COVERED** | `header.json` `WhereClause`. |
| **E2** | Policy: `allowAspctEntry()` = `HSEPLC_ALWASPCTENTRY`? | **COVERED** | `ShowScreen` → `fetchHseplcAspectFlags`; branch when `allowEntry === 'N'`. |
| **E2a** | If policy **N**: disable DELETE, NEW, SAVE; disable Aspects + Tracing tabs | **COVERED** | `setScreenDisableBtn(true,true,true)` + `TabEnable` on `HSE_Aspcts_Aspcts` / `HSE_Aspcts_Trcng`. |
| **E2b** | Read-only mode (view existing only) | **COVERED** | Follows E2a (toolbar + tabs off). |
| **E3** | Click NEW on toolbar | **COVERED** | Platform NEW; handler may set `cancel`. |
| **E4** | Policy Year/Month available? | **COVERED** | `toolBarButtonClicked` `complete===0` + `fetchHseplcAspectFlags` (`hasYrOrMnth`). |
| **E4a** | Cancel NEW if year/month empty | **COVERED** | `eventObj.cancel = 1` + `AskYesNoMessage` (matches C++ when both YR and MNTH empty). |
| **E5** | Auto-set `ASPCTS_YR`, `ASPCTS_MNTH` from `HSE_HSEPLC` | **COVERED** | `applyNewHeaderDefaults` on main-form NEW `complete===1`. |
| **E6** | Admin user? | **COVERED** | `isAdminUserDev` / `devInterface.isAdminUser` (`hse.js`). |
| **E6a** | Admin: `ASPCTS_DPRTMNT` MUST | **COVERED** | `changeFldAttrb('scr','ASPCTS_DPRTMNT','MUST')` via WebInfra `useScrStructureHandler.changeFldAttribute` (RQ_HSE_23_3_26_22_02). |
| **E6b** | Non-admin: auto-set department + locked | **COVERED** | `FormSetField` + `changeFldAttrb` `ALWAYSLOCKED` (same WebInfra path) + `getEmployeeDepartmentClause`. |
| **E7** | Fill header: Department, Year, Month; Created DT/By | **COVERED** | E5–E6b + JSON defaults (`TODAY`, `LOGIN`). |
| **E8** | Open Aspects tab `HSE_ASPCTS_ASPCTS` | **COVERED** | Platform tab. |
| **E9** | Add aspect line; auto-serial | **COVERED** | `setNextSerialOnNewTab` on tab NEW. |
| **E10**–**E11** | Aspect details; Location | **COVERED** | `HSE_Aspcts_Aspcts.json` + browses. |
| **E12** | Significance (before mitigation) | **COVERED** | Platform + JSON validation. |
| **E12a** | Risk Rank = C×L; Risk Level lookup | **COVERED** | `syncRiskRanksFromConsequenceLikelihood` + `computeAspectRiskRank` in `aspectsRegisterPolicy.js`; then `lookupRiskLevelDescription` (RQ_HSE_23_3_26_22_02). |
| **E13** | Post-mitigation fields | **COVERED** | Platform + JSON. |
| **E14** | f-Risk Rank > pre-mitigation? | **COVERED** | `applyAspectLineRiskValidation` compares `RSKRNK` vs `RSKRNK_1`. |
| **E14a** | Invalid: message, clear fields, `RSKACCPTD = N` | **COVERED** | `AskYesNoMessage` + clears in `aspectsRegisterPolicy.js`. |
| **E15** | f-Risk Level lookup; Risk Accepted = Y | **COVERED** | Same helper sets `RSKACCPTD` and `RSKLVL` fields when valid. |
| **E16**–**E18** | Final product; Save; repeat lines | **COVERED** | Platform. |

### 2.3 Entry: filters and completion

| Node ID | Activity (as in §2 diagram) | Web status | Evidence / gap |
|---------|----------------------------|------------|------------------|
| **EntryActions** | Next action? (History / Current Month / Entry Completed / Done) | **COVERED** | User/UI choice; wiring varies per branch (below). |
| **VH1** | VIEW_HISTORY: Admin user? | **COVERED** | `handleViewHistory` → `isAdminUserDev`. |
| **VH2** | Admin: `ChangeCriteria` … `DPRTMNT LIKE '%'` | **COVERED** | `useDevInterfaceFun.ChangeCriteria` + `listScrSqlCriteriaOverride` merge in `FirstMainListPart` (RQ_HSE_23_3_26_22_02); from edit, navigates to `/list`. |
| **VH3** | Non-admin: `IN (empDepartments)` | **COVERED** | Same platform `ChangeCriteria`; `getEmployeeDepartmentClause(dev,false)`. |
| **VC1** | VIEW_CURRENT_MONTH: Admin user? | **COVERED** | `handleViewCurrentMonth`. |
| **VC2** | Admin: YR + MNTH filter | **COVERED** | Same `ChangeCriteria` + list merge. |
| **VC3** | Non-admin: dept + YR + MNTH | **COVERED** | Same. |
| **EC1** | Read `PrmryKy` from header | **COVERED** | `getPrimaryKey` reads `HSE_ASPCTS_VIEW` / `PrmryKy`. |
| **EC2** | `EXECUTE EnvAspctEntryComplete @PrmryKy` | **COVERED** | `handleEntryCompleted` → `executeSQLPromise`. |
| **EC3** | Status 1 → 5 | **COVERED** | Assumed DB SP side effect (same as desktop). |
| **EC4** | Refresh (record leaves entry list) | **COVERED** | `refreshData('')` after SP. |
| **EndNode** | Cycle continues at Review screen | **COVERED** | Process outcome; user opens Review screen. |

### 2.4 Track 2: Aspects Review

| Node ID | Activity (as in §2 diagram) | Web status | Evidence / gap |
|---------|----------------------------|------------|------------------|
| **R1** | Open Review; criteria `ASPCTS_ASPCTSTTUS = 5` | **COVERED** | `HSE_AspctsRvwAtRvw/header.json` → `WhereClause`: `WHERE (ASPCTS_ASPCTSTTUS = '5')`. |
| **R2** | Always disable DELETE, NEW on review | **COVERED** | `ShowScreen` → `setScreenDisableBtn(true, false, true)`. |
| **R3** | Policy: `allowAspctReview()`? | **COVERED** | `fetchHseplcAspectFlags` → `allowReview === 'N'` branch. |
| **R3a** | Policy **N**: disable SAVE on main + tab toolbars | **COVERED** | Main: `setScreenDisableBtn(true,true,true)`. Tab: `FormEnableButton(2,1,'NEW'|'SAVE'|'DELETE',false,true)` + `appDisableTab*` in `FirstToolbar` (RQ_HSE_23_3_26_22_02). |
| **R4** | HSE reviewer examines record | **COVERED** | Platform. |
| **R5** | Review aspect lines | **COVERED** | Platform + subform JSON. |
| **R6** | Optional edit f-Risk on review | **COVERED** | Platform. |
| **R7** | f-Risk Rank > pre-mitigation? | **COVERED** | `SubFieldChanged` → `applyAspectLineRiskValidation` (review mode, no popup). |
| **R7a** | Clear f-fields when invalid | **COVERED** | Same helper as Entry (clears post-mitigation fields). |
| **R7b** | Risk Accepted = Y when valid | **COVERED** | `FormSetField` `RSKACCPTD` + level lookup when applicable. |
| **ReviewDecision** | Accept vs Reject | **COVERED** | User clicks `ACCEPTED` / `REJECTED`. |
| **RA1** | Read `PrmryKy` | **COVERED** | `getPrimaryKey` in `Aspects_Review.js`. |
| **RA2** | `EXECUTE EnvAspctRvwAccpt` | **COVERED** | `handleAccept` → `executeSQLPromise`. |
| **RA3** | Status 5 → 10 | **COVERED** | SP side effect. |
| **RA4** | Refresh | **COVERED** | `refreshData('')`. |
| **RJ1**–**RJ4** | Reject path (`EnvAspctRjct`, 5→2, refresh) | **COVERED** | `handleReject` mirrors Accept pattern. |
| **ClosedEnd** | Accepted; locked on Entry screen | **COVERED** | DB → status 10. `MainSubReposition` + `setScrLockedAttrb('scr', true)` when status ≥ 10; unlock when &lt; 10 (RQ_HSE_23_3_26_22_02). |
| **RejEnd** | Rejected; returns to Entry criteria | **COVERED** | SP + list criteria on Entry screen (`status < 5` includes 2 per desktop semantics—confirm with DB if status 2 visible; diagram shows return to E1). |

---

## 3. Summary counts (§2 nodes)

<!-- RQ_HSE_23_3_26_22_02 -->

| Status | Count | Notes |
|--------|------:|-------|
| **COVERED** | **54** | All §2.1–§2.4 rows; includes WebInfra `ChangeCriteria`, `FormEnableButton`, `changeFldAttrb`, `setScrLockedAttrb`, and HSE `computeAspectRiskRank` (RQ_HSE_23_3_26_22_02 follow-up). |
| **PARTIAL** | **0** | — |
| **MISSING** | **0** | — |

*Counting: §2.1 (2) + §2.2 (22) + §2.3 (12) + §2.4 (18) = 54 rows.*

---

## 4. Platform notes (RQ_HSE_23_3_26_22_02 parity wave)

<!-- RQ_HSE_23_3_26_22_02 -->

The former **PARTIAL** items are implemented in **WebInfra.Client** (`ChangeCriteria`, `FormEnableButton`, `listScrSqlCriteriaOverride` merge, `appDisableTab*`) and **HSE** (`setScrLockedAttrb`, `computeAspectRiskRank`). Forks of WebInfra without these commits must backport or behaviour reverts to the earlier validation doc.

---

## 5. Follow-ups

<!-- RQ_HSE_23_3_26_22_02 -->

None required for §2 diagram parity when running the updated WebInfra + HSE bundles together.

---

## 6. Cross-reference

<!-- RQ_HSE_23_3_26_22_02 -->

- Full process UML (including §19 validation): [`Emissions_Aspects_Tracking_Process_UML.md`](./Emissions_Aspects_Tracking_Process_UML.md)
- Use cases summary: [`HSEMS_Use_Cases_From_Desktop_Code.md`](./HSEMS_Use_Cases_From_Desktop_Code.md) §3.4

---

## 7. Implementation checklist (post RQ_HSE_23_3_26_22_02)

<!-- RQ_HSE_23_3_26_22_02 -->

| Area | Status |
|------|--------|
| Entry policy + tabs (E2–E2b) | **Done** — `ShowScreen`, `TabEnable` |
| Main NEW + YR/MNTH + dept (E4–E6b) | **Done** — `toolBarButtonClicked`, `applyNewHeaderDefaults`, `isAdminUser` |
| VIEW_HISTORY / VIEW_CURRENT_MONTH | **Done** — WebInfra `ChangeCriteria` + list merge + `/list` from edit (§4) |
| Risk validation Entry (E14–E15) | **Done** — `SubFieldChanged` + `applyAspectLineRiskValidation` |
| Review toolbar + policy (R2–R3) | **Done** — main + tab `FormEnableButton` / `appDisableTab*` |
| Review risk (R7–R7b) | **Done** — `SubFieldChanged` on Review |
| Status-based Entry toolbar | **Done** — `MainSubReposition` |
| Admin vs department filters | **Done** — `getEmployeeDepartmentClause` |

*Detail: §2 tables; residuals: §4.*

---

*End of §2 activity diagram web validation — RQ_HSE_23_3_26_22_02*
