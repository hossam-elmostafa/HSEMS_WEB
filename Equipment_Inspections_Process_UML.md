# Equipment Inspections Process -- UML Documentation

<!-- RQ_HSE_23_3_26_23_15 -->

> **Source**: HSEMS C++ Desktop (`HSEMS-Win`, `EQInspctnCategory.cpp`) + Web (`hse` module screen handlers, `ModuleButtonHandlers`) + app metadata (`Test/APP_JSON/66` HSE EQ Inspection screens)
> **Scope**: Equipment inspection lifecycle from **EQ Inspection Request** (entry) through **EQ Inspection Result**, **In-Site EQ** (on-site work), until **terminal exit outcomes** (work completed, cancelled, or exit-with-defects paths driven by the database)
> **Date**: March 2026
> **See also**: [`HSEMS_Use_Cases_From_Desktop_Code.md`](./HSEMS_Use_Cases_From_Desktop_Code.md) §4.2, §11 (stored procedures summary)

---

## 1. Process overview

The **Equipment Inspection** module tracks **equipment brought onto a work site** under **Performance → EQ Inspection**. One header row in **`HSE_EQINSPCTN`** is shared across **Request**, **Result**, **In-Site**, and **Inquiry** screens; each screen filters by **`EQINSPCTN_INSPCTNSTTUS`**. Users capture **checklist lines**, optional **defects during work**, and **inspection timing** on sub-forms. Status transitions and business rules are enforced mainly by **stored procedures** invoked from **custom buttons** (`EQInspctnCategory::DisplayCustomButtonClicked` on desktop; web handlers delegate `EQINSPCTN_*` / `INSPCTNCHCKLST_*` actions to the backend where configured).

### 1.1 Operational screens

| Phase | Screen (caption) | Tag | C++ category (reference) | Primary table | Status filter (from app JSON) |
|-------|------------------|-----|---------------------------|---------------|-------------------------------|
| Request / entry | EQ Inspection Request | `HSE_EQInspctnRqust` | `EQInspctnCategory` | `HSE_EQINSPCTN` | `EQINSPCTN_INSPCTNSTTUS < 5` |
| Inspection result | EQ Inspection Result | `HSE_EQInspctnRsult` | `EQInspctnCategory` | (same) | `= 5` **or** `= 9` |
| On-site work | In-Site EQ | `HSE_EQInspctnWrk` | `EQInspctnCategory` | (same) | `= 10` **or** `= 15` |
| Inquiry | EQ Inspection Inquiry | `HSE_EQInspctnInqry` | Read-only + filter buttons | (same) | (dynamic `ChangeCriteria` / user filters) |

**Key field** (header): `EQINSPCTN_PRMKY` (listed as `EQInspctn_PrmKy` in metadata). **Business key**: `EQINSPCTN_INSPCTNYR` + `EQINSPCTN_INSPCTNN`. On **NEW**, desktop assigns the next inspection number for the current year (`getCurrentEQInspctnNo`).

### 1.2 Sub-forms and related data

| Artefact | Typical tags / tables | Purpose |
|----------|------------------------|---------|
| Inspection checklist | `HSE_EQINSPCTN_INSPCTNCHCKLST` (per-screen JSON under Request / Result / Inquiry) | Per-line checklist for the selected **equipment type**; field-change logic can set **accepted** flags from **defect found** / **action done** (see `EQInspctnCategory::DisplayFieldChange`) |
| Inspection time | `HSE_EQINSPCTN_INSPCTNTM` | Timing details for the inspection |
| Defects during work | `HSE_EQINSPCTN_DFCTSDURNGWRK` | Defect lines while equipment is on site; desktop auto-assigns line number via `getCurrentEQInspctnDfctNo` when appropriate |
| Tracing | `HSE_EQINSPCTN` tracing sub-form | Audit trail of actions on the inspection |

**Master data**: Checklist templates come from **`HSE_EQTYPS_INSPCTNCHCKLST`** (by `EQINSPCTN_EQTYP`). Choosing an equipment type with **no** related checklist rows triggers a desktop message (`IDS_INSPCTN_RQST_EMPTY_CHKLST`).

### 1.3 Cross-cutting validation (request entry)

On **SAVE** (desktop), the category runs a **duplicate open inspection** check: the same **equipment type + serial (+ plate when present)** must not already exist with **`EQInspctn_InspctnSttus < 5`**. If a match exists, the user is warned (*"This Equipment entered before!"*).

**Expected exit date** is derived from **entry date**, **working days**, and **working hours** (`DisplayFieldChange` on `EQINSPCTN_EQENTRYDT` / `EQINSPCTN_EQWRKNGDYS` / `EQINSPCTN_EQWRKNGHURS` → `EQINSPCTN_EXPCTDEXTDT`).

### 1.4 Stored procedures (transactional)

| Procedure | Typical phase | Purpose |
|-----------|---------------|---------|
| `completeEQInspctn` | Request | Complete request / data entry; advances status so the record appears on **EQ Inspection Result** (status **5**) |
| `rjctEQInspctnDataEntry` | Request | Reject **data entry**; returns record for correction on the request side |
| `reldEQInspctnChckLst` | Request / Result | Reload checklist from equipment type (desktop prompts for confirmation; comment text passed as in C++) |
| `rjctEQInspctn` | Result | Reject the **inspection** (equipment not accepted) |
| `holdEQInspctn` | Result | Put inspection **on hold** (status **9** — still visible on Result screen) |
| `AcceptEQInspctn` | Result | **Accept** inspection; equipment cleared to proceed to **on-site** handling (status moves toward **10** / **15** per business rules) |
| `stopDefectedEQ` | In-Site | Stop defective equipment on site |
| `resumeEQWrk` | In-Site | Resume work after stop |
| `exitDefectedEQ` | In-Site | Exit flow for defective equipment |
| `canclEQWrk` | In-Site | Cancel on-site work / job |
| `cmpltEQWrk` | In-Site | Complete on-site work — **cycle completion** path |

Exact numeric **`EQINSPCTN_INSPCTNSTTUS`** values for each transition are defined in the database; inquiry **quick filters** on desktop use statuses **8** (rejected), **9** (on hold), **10** (working on site), **11** (stopped defective, current month), **18** / **19** / **20** (monthly exit categories for defective / cancelled / completed).

### 1.5 Custom buttons (desktop reference)

| Button | Purpose |
|--------|---------|
| `EQINSPCTN_ENTRYCMPLTD` | `EXECUTE completeEQInspctn` |
| `EQINSPCTN_DATAENTRYRJCTD` | `EXECUTE rjctEQInspctnDataEntry` |
| `INSPCTNCHCKLST_RELOADCHCKLST` | `EXECUTE reldEQInspctnChckLst` |
| `EQINSPCTN_EQINSPCTNRJCTD` | `EXECUTE rjctEQInspctn` |
| `EQINSPCTN_EQHOLD` | `EXECUTE holdEQInspctn` |
| `EQINSPCTN_EQACPTD` | `EXECUTE AcceptEQInspctn` |
| `EQINSPCTN_EQSTPDDFCTD` | `EXECUTE stopDefectedEQ` |
| `EQINSPCTN_EQRSMWRK` | `EXECUTE resumeEQWrk` |
| `EQINSPCTN_EQEXTDFCTS` | `EXECUTE exitDefectedEQ` |
| `EQINSPCTN_EQEXTWRKCNCLD` | `EXECUTE canclEQWrk` |
| `EQINSPCTN_EQEXTWRKCMPLTD` | `EXECUTE cmpltEQWrk` |
| `EQINSPCTN_VWINSPCTNRQSTHSTRY` | Open inquiry filtered to **prior completed** inspections for same type/serial/plate |

**Web note** (**RQ_HSE_23_3_26_23_15**): All lifecycle and inquiry buttons in §1.5 are handled by **`handleEQInspectionModuleButtons`** in [`eqInspectionButtonHandlers.js`](hse/src/services/ModuleButtonHandlers/eqInspectionButtonHandlers.js) (via **`ModuleButtonHandlers`**). SAVE-time duplicate warning and **`SubFieldChanged`** rules are on the EQ Request / Result / In-Site screen handlers. Validation: [`Equipment_Inspections_Activity_Web_Validation.md`](./Equipment_Inspections_Activity_Web_Validation.md).

---

## 2. Activity diagram -- Equipment inspection (entry to cycle end)

<!-- RQ_HSE_23_3_26_23_15 -->

**Web implementation check** (§2 nodes vs `hse`): [`Equipment_Inspections_Activity_Web_Validation.md`](./Equipment_Inspections_Activity_Web_Validation.md) — **RQ_HSE_23_3_26_23_15**.

```mermaid
flowchart TD
    Start(["User opens Performance → EQ Inspection →\nEQ Inspection Request\nHSE_EQInspctnRqust"]) --> R1

    subgraph REQUEST ["Phase 1: EQ Inspection Request (status < 5)"]
        R1["NEW / edit header HSE_EQINSPCTN\n(year + number, equipment, site, dates)"]
        R1 --> R2["Select EQINSPCTN_EQTYP\n(checklist template must exist)"]
        R2 --> R3["Fill checklist / timing / attachments\nas per screen"]
        R3 --> R4["SAVE"]
        R4 --> R4a{"Duplicate open inspection\nsame type+serial+plate?"}
        R4a -- Yes, warn --> R3
        R4a -- No / proceed --> R5{"Reject data entry?\nEQINSPCTN_DATAENTRYRJCTD"}
        R5 -- Yes --> R6["EXECUTE rjctEQInspctnDataEntry"]
        R6 --> R1
        R5 -- No --> R7{"Reload checklist?\nINSPCTNCHCKLST_RELOADCHCKLST"}
        R7 -- Yes --> R8["Confirm → EXECUTE reldEQInspctnChckLst"]
        R8 --> R3
        R7 -- No --> R9{"Submit request complete?\nEQINSPCTN_ENTRYCMPLTD"}
        R9 -- No, edit more --> R3
        R9 -- Yes --> R10["EXECUTE completeEQInspctn"]
        R10 --> R11["Status → awaiting result\n(appears on Result screen)"]
    end

    R11 --> Q1

    subgraph RESULT ["Phase 2: EQ Inspection Result (status 5 or 9)"]
        Q1["Open EQ Inspection Result\nHSE_EQInspctnRsult"]
        Q1 --> Q2["Record inspection outcome,\nchecklist lines, findings"]
        Q2 --> Q3{"Decision?"}
        Q3 -- Hold --> Q4["EQINSPCTN_EQHOLD\nEXECUTE holdEQInspctn"]
        Q4 --> Q2
        Q3 -- Reject inspection --> Q5["EQINSPCTN_EQINSPCTNRJCTD\nEXECUTE rjctEQInspctn"]
        Q5 --> EndRejected(["Cycle end (rejected inspection)\nstatus e.g. 8"])
        Q3 -- Reload checklist --> Q6["INSPCTNCHCKLST_RELOADCHCKLST"]
        Q6 --> Q2
        Q3 -- Accept --> Q7["EQINSPCTN_EQACPTD\nEXECUTE AcceptEQInspctn"]
        Q7 --> Q8["Equipment cleared for on-site phase\n(status 10 / 15 on In-Site screen)"]
    end

    Q8 --> W1

    subgraph INSITE ["Phase 3: In-Site EQ (status 10 or 15)"]
        W1["Open In-Site EQ\nHSE_EQInspctnWrk"]
        W1 --> W2["Monitor / record defects during work,\nstop & resume as needed"]
        W2 --> W3{"On-site action?"}
        W3 -- Stop defective --> W4["EQINSPCTN_EQSTPDDFCTD"]
        W4 --> W5["EXECUTE stopDefectedEQ"]
        W5 --> W2
        W3 -- Resume --> W6["EQINSPCTN_EQRSMWRK"]
        W6 --> W7["EXECUTE resumeEQWrk"]
        W7 --> W2
        W3 -- Exit defective --> W8["EQINSPCTN_EQEXTDFCTS"]
        W8 --> W9["EXECUTE exitDefectedEQ"]
        W9 --> EndExitDef(["Terminal: exited with defects\n(e.g. status 18)"])
        W3 -- Cancel work --> W10["EQINSPCTN_EQEXTWRKCNCLD"]
        W10 --> W11["EXECUTE canclEQWrk"]
        W11 --> EndCancelled(["Terminal: work cancelled\n(e.g. status 19)"])
        W3 -- Complete work --> W12["EQINSPCTN_EQEXTWRKCMPLTD"]
        W12 --> W13["EXECUTE cmpltEQWrk"]
        W13 --> EndCompleted(["Cycle end: on-site work completed\n(e.g. status 20)"])
    end

    style Start fill:#4CAF50,color:white
    style EndCompleted fill:#2196F3,color:white
    style EndRejected fill:#f44336,color:white
    style EndExitDef fill:#FF9800,color:white
    style EndCancelled fill:#9E9E9E,color:white
    style Q3 fill:#FF9800,color:white
    style W3 fill:#FF9800,color:white
```

---

## 3. State machine -- Inspection status (logical)

<!-- RQ_HSE_23_3_26_23_15 -->

Numeric **`EQINSPCTN_INSPCTNSTTUS`** values are authoritative in the database. The diagram summarizes **stages** implied by screen `WhereClause` filters and inquiry shortcuts in `EQInspctnCategory.cpp`.

```mermaid
stateDiagram-v2
    [*] --> RequestEntry : NEW / save\n(status < 5)

    RequestEntry --> RequestEntry : rjctEQInspctnDataEntry\n(DATAENTRYRJCTD)

    RequestEntry --> AwaitingResult : completeEQInspctn\n(ENTRYCMPLTD)\n→ typical: 5

    AwaitingResult --> OnHold : holdEQInspctn\n(EQHOLD)\n→ 9

    OnHold --> AwaitingResult : User continues\non Result screen

    AwaitingResult --> RejectedInspection : rjctEQInspctn\n(EQINSPCTNRJCTD)\n→ e.g. 8

    AwaitingResult --> OnSite : AcceptEQInspctn\n(EQACPTD)\n→ 10 / 15

    OnSite --> OnSite : stopDefectedEQ / resumeEQWrk\n(EQSTPDDFCTD / EQRSMWRK)

    OnSite --> TerminalExitDefective : exitDefectedEQ\n(EQEXTDFCTS)\n→ e.g. 18

    OnSite --> TerminalCancelled : canclEQWrk\n(EQEXTWRKCNCLD)\n→ e.g. 19

    OnSite --> TerminalCompleted : cmpltEQWrk\n(EQEXTWRKCMPLTD)\n→ e.g. 20

    RejectedInspection --> [*]
    TerminalExitDefective --> [*]
    TerminalCancelled --> [*]
    TerminalCompleted --> [*]

    note right of OnSite
        Inquiry filters also highlight
        monthly cohorts (e.g. 11, 18–20).
    end note
```

---

## 4. Sequence diagram -- Happy path (request → accept → complete work)

<!-- RQ_HSE_23_3_26_23_15 -->

```mermaid
sequenceDiagram
    participant User
    participant Req as EQ Inspection Request
    participant Res as EQ Inspection Result
    participant Wrk as In-Site EQ
    participant UI as Desktop / Web layer
    participant DB as Database

    User->>Req: Create inspection, equipment type, checklist, SAVE
    User->>Req: Click EQINSPCTN_ENTRYCMPLTD
    Req->>UI: Custom button handler
    UI->>DB: EXECUTE completeEQInspctn(prmky)
    DB-->>UI: OK (status → 5)
    UI->>Req: Refresh

    User->>Res: Open record, enter results, SAVE as needed
    User->>Res: Click EQINSPCTN_EQACPTD
    Res->>UI: Button → backend
    UI->>DB: EXECUTE AcceptEQInspctn(prmky)
    DB-->>UI: OK (on-site statuses)
    UI->>Res: Refresh

    User->>Wrk: Manage on-site defects / stops / resume
    User->>Wrk: Click EQINSPCTN_EQEXTWRKCMPLTD
    Wrk->>UI: Button → backend
    UI->>DB: EXECUTE cmpltEQWrk(prmky)
    DB-->>UI: OK
    UI->>Wrk: Refresh
    Note over User,DB: Cycle end: work completed (terminal status, e.g. 20)
```

---

## 5. Sequence diagram -- Reject / hold / reload checklist

<!-- RQ_HSE_23_3_26_23_15 -->

```mermaid
sequenceDiagram
    participant User
    participant Req as EQ Inspection Request
    participant Res as EQ Inspection Result
    participant UI as Desktop / Web layer
    participant DB as Database

    alt Reject data entry (still on Request)
        User->>Req: Click EQINSPCTN_DATAENTRYRJCTD
        Req->>UI: Handler
        UI->>DB: EXECUTE rjctEQInspctnDataEntry(prmky)
        DB-->>UI: OK
        UI->>Req: Refresh
    end

    alt Reload checklist
        User->>Req: INSPCTNCHCKLST_RELOADCHCKLST (confirm on desktop)
        Req->>UI: Handler
        UI->>DB: EXECUTE reldEQInspctnChckLst(prmky, reason)
        DB-->>UI: OK
        UI->>Req: Refresh
    end

    alt Hold inspection
        User->>Res: Click EQINSPCTN_EQHOLD
        Res->>UI: Handler
        UI->>DB: EXECUTE holdEQInspctn(prmky)
        DB-->>UI: OK (status 9)
        UI->>Res: Refresh
    end

    alt Reject inspection
        User->>Res: Click EQINSPCTN_EQINSPCTNRJCTD
        Res->>UI: Handler
        UI->>DB: EXECUTE rjctEQInspctn(prmky)
        DB-->>UI: OK (e.g. status 8)
        UI->>Res: Refresh
    end
```

---

## 6. Use case summary (actors vs system)

| Actor step | System response |
|------------|-----------------|
| User registers equipment for site entry on **EQ Inspection Request** | Persist `HSE_EQINSPCTN`; load checklist from equipment type; validate duplicates (open inspections) |
| User completes request | `completeEQInspctn`; record moves to **EQ Inspection Result** queue |
| User rejects own data entry | `rjctEQInspctnDataEntry`; correct on Request |
| Inspector records results | Update checklist / timing / defects sub-forms; optional `reldEQInspctnChckLst` |
| Inspector holds | `holdEQInspctn`; status **9** |
| Inspector rejects equipment | `rjctEQInspctn`; inspection rejected (e.g. status **8**) |
| Inspector accepts | `AcceptEQInspctn`; equipment eligible for **In-Site EQ** |
| Site team manages on-site work | `stopDefectedEQ`, `resumeEQWrk`, `exitDefectedEQ`, `canclEQWrk`, `cmpltEQWrk` |
| User views history for same equipment | `EQINSPCTN_VWINSPCTNRQSTHSTRY` opens inquiry with SQL for **completed** prior inspections |

---

*End of document*
