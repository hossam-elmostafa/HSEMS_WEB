# Awareness / Training Plan Process – UML Documentation

<!-- RQ_HSE_23_3_26_15_42 -->

> **Source**: HSEMS C++ Desktop (`HSEMS-Win`) + Web (`hse` module)
> **Scope**: Awareness Plan lifecycle (`HSE_AWRNSPLN`): Entry → Approval → Execution → Close, with Reject path
> **Date**: March 2026
> **See also**: [`HSEMS_Use_Cases_From_Desktop_Code.md`](./HSEMS_Use_Cases_From_Desktop_Code.md) §2.8

---

## 1. Process overview

The **Awareness / Training Plan** track covers: **Entry → Entry Completed → Approval (Approve / Reject) → Execution → Close**.

The lifecycle manages training/awareness course plans including courses, instructors, and target departments. Once a plan is entered and completed, it goes through an approval gate. Approved plans move to execution where actual course delivery is recorded. After execution, the plan is closed.

**Screens and tags:**

| Screen | Web screen tag | C++ Category | Primary SPs | Screen handler |
|--------|----------------|--------------|-------------|----------------|
| Entry | `HSE_TgAwrnsPlnEntry` | `AwrnsPlnEntryCategory` | `AwrnsPlnEntryCompleted` | `Awareness_Plan_Entry.js` |
| Approval | `HSE_TgAwrnsPlnAprvl` | `AwrnsPlnAprvlCategory` | `AwrnsPlnApproved`, `AwrnsPlnRejected` | `Awareness_Plan_Approval.js` |
| Execution | `HSE_TgAwrnsPlnExctn` | `AwrnsPlnExctnCategory` | `AwrnsPlnClosed` | `Awareness_Plan_Execution.js` |
| Inquiry | `HSE_TgAwrnsPlnInq` | (read-only) | — | `Awareness_Plan_Inquiry.js` |

**Custom buttons** (desktop):

| Button | Description |
|--------|-------------|
| `AWRNSPLNENTRY_PLNENTRYCMPLTD` | Entry completed — submit for approval |
| `AWRNSPLNAPRVL_PLNENTRYAPRVD` | Approve plan |
| `AWRNSPLNAPRVL_PLNENTRYRJCTD` | Reject plan (with reasons) |
| `AWRNSPLNEXCTN_CRS_CRTUNPLNDCRS` | Create unplanned course during execution |
| `AWRNSPLNEXCTN_PLNCLSD` | Close plan after execution |

**Table**: `HSE_AWRNSPLN`; key field: `SerialPK`

---

## 2. Activity diagram – Awareness / Training Plan (end-to-end)

```mermaid
flowchart TD
    Start(["User opens Awareness Plan Entry"]) --> A1

    subgraph ENTRY ["Phase 1: Plan Entry"]
        A1["Create / edit awareness plan\nHSE_AWRNSPLN"]
        A1 --> A2["Fill details: courses, instructors,\ntarget departments"]
        A2 --> A3["Save record"]
        A3 --> A4{"Click Entry Completed\nAWRNSPLNENTRY_PLNENTRYCMPLTD?"}
        A4 -- No --> A2
        A4 -- Yes --> A5{"Record saved?\n(SerialPK exists)"}
        A5 -- No --> A6["No action (key guard)"]
        A6 --> A2
        A5 -- Yes --> A7["AwrnsPlnEntryCompleted"]
        A7 --> A8["Status updated via SP"]
    end

    A8 --> B1

    subgraph APPROVAL ["Phase 2: Plan Approval"]
        B1["Reviewer opens Approval screen"]
        B1 --> B2{"Approve or Reject?"}
        B2 -- Reject --> B3["Open reject reasons\nAWRNSPLNAPRVL_PLNENTRYRJCTD"]
        B3 --> B4["AwrnsPlnRejected\n(desktop: rjctAwrnsPln)"]
        B4 --> B5["Return to Entry for correction"]
        B2 -- Approve --> B6["AwrnsPlnApproved\nAWRNSPLNAPRVL_PLNENTRYAPRVD"]
        B6 --> B7["Status updated via SP"]
    end

    B7 --> C1

    subgraph EXECUTION ["Phase 3: Plan Execution"]
        C1["Executor opens Execution screen"]
        C1 --> C2["Record course execution\nand attendees"]
        C2 --> C3{"Need unplanned course?"}
        C3 -- Yes --> C4["Create unplanned course\nAWRNSPLNEXCTN_CRS_CRTUNPLNDCRS"]
        C4 --> C2
        C3 -- No --> C5{"All courses executed?"}
        C5 -- No --> C2
        C5 -- Yes --> C6{"Close plan?\nAWRNSPLNEXCTN_PLNCLSD"}
        C6 -- No --> C2
        C6 -- Yes --> C7["AwrnsPlnClosed"]
        C7 --> C8["Refresh / status updated"]
    end

    C8 --> End(["Plan closed"])

    style Start fill:#4CAF50,color:white
    style End fill:#2196F3,color:white
```

---

## 3. State machine

Status transitions enforced by stored procedures:

```mermaid
stateDiagram-v2
    [*] --> Incomplete : NEW / SAVE
    Incomplete --> EntryCompleted : AWRNSPLNENTRY_PLNENTRYCMPLTD\nAwrnsPlnEntryCompleted
    EntryCompleted --> Approved : AWRNSPLNAPRVL_PLNENTRYAPRVD\nAwrnsPlnApproved
    EntryCompleted --> Rejected : AWRNSPLNAPRVL_PLNENTRYRJCTD\nAwrnsPlnRejected
    Approved --> Closed : AWRNSPLNEXCTN_PLNCLSD\nAwrnsPlnClosed
    Rejected --> Incomplete : User corrects and re-saves
```

---

## 4. Sequence diagram – Entry Completed

```mermaid
sequenceDiagram
    participant User
    participant WebUI as Web UI
    participant EntryScr as Awareness_Plan_Entry.js
    participant ModHandler as handleAwarenessButton
    participant DB as Database

    User->>WebUI: Click AWRNSPLNENTRY_PLNENTRYCMPLTD
    WebUI->>ModHandler: handleModuleButton (Step 1)
    ModHandler->>ModHandler: getKeyFromEvent (SerialPK)
    alt Key exists
        ModHandler->>DB: EXECUTE AwrnsPlnEntryCompleted @pk
        DB-->>ModHandler: OK
        ModHandler->>WebUI: refreshData("")
    else No key
        ModHandler->>WebUI: AskYesNoMessage("You must save the Record first")
        ModHandler-->>WebUI: return true (handled, RQ_HSE_23_3_26_15_42)
    end
```

> **Note**: Both `handleAwarenessButton` (module handler, Step 1) and `Awareness_Plan_Entry.js` (screen handler, Step 2) implement this SP call when a key exists. The module handler runs first; if it succeeds the screen handler is skipped. If there is no key, the module handler shows **save first** and returns `true` so Step 2 does not run (**RQ_HSE_23_3_26_15_42**).

---

## 5. Sequence diagram – Approval and Reject

```mermaid
sequenceDiagram
    participant User
    participant WebUI as Web UI
    participant ModHandler as handleAwarenessButton
    participant RejectScr as Reject reason screen
    participant RejectOK as handleRejectReasonOkButton
    participant DB as Database

    Note over User,DB: --- Approve path ---

    User->>WebUI: Click AWRNSPLNAPRVL_PLNENTRYAPRVD
    WebUI->>ModHandler: handleModuleButton (Step 1)
    ModHandler->>DB: EXECUTE AwrnsPlnApproved @pk
    DB-->>ModHandler: OK
    ModHandler->>WebUI: refreshData("")

    Note over User,DB: --- Reject path (RQ_HSE_23_3_26_15_42) ---

    User->>WebUI: Click AWRNSPLNAPRVL_PLNENTRYRJCTD
    WebUI->>ModHandler: handleModuleButton (Step 1)
    ModHandler->>RejectScr: openRejectReasonScreen AWRNSPLN-L1
    ModHandler->>ModHandler: setPendingRejectForModule AWARENESS
    User->>RejectScr: Enter reason, OK
    RejectScr->>RejectOK: RJCTRSN_BTN_OK
    RejectOK->>DB: EXECUTE AwrnsPlnRejected @pk, ''
    DB-->>RejectOK: OK
    RejectOK->>WebUI: refreshData REFRESH_SELECTED
```

---

## 6. Sequence diagram – Execution and Close

```mermaid
sequenceDiagram
    participant User
    participant WebUI as Web UI
    participant ExecScr as Awareness_Plan_Execution.js
    participant ModHandler as handleAwarenessButton
    participant DB as Database

    Note over User,DB: --- Create unplanned course ---

    User->>WebUI: Click AWRNSPLNEXCTN_CRS_CRTUNPLNDCRS
    WebUI->>ExecScr: ButtonClicked (via screen handler)
    ExecScr->>WebUI: sendButtonClickToBackend

    Note over User,DB: --- Close plan ---

    User->>WebUI: Click AWRNSPLNEXCTN_PLNCLSD
    WebUI->>ModHandler: handleModuleButton (Step 1, RQ_HSE_23_3_26_15_42)
    ModHandler->>DB: EXECUTE AwrnsPlnClosed @pk
    DB-->>ModHandler: OK
    ModHandler->>WebUI: refreshData("")
    Note over User,DB: Fallback: Awareness_Plan_Execution.js if Step 1 did not handle
```

---

## 7. Component diagram – Web architecture

```mermaid
flowchart LR
    subgraph Screens ["Screen Handlers"]
        EntryScr["Awareness_Plan_Entry.js\nHSE_TgAwrnsPlnEntry"]
        ApprovalScr["Awareness_Plan_Approval.js\nHSE_TgAwrnsPlnAprvl"]
        ExecScr["Awareness_Plan_Execution.js\nHSE_TgAwrnsPlnExctn"]
        InqScr["Awareness_Plan_Inquiry.js\nHSE_TgAwrnsPlnInq"]
    end

    subgraph Events ["Event Layer"]
        BtnEvt["buttonEvents.js\nhandleModuleButton dispatch\n→ screen handler fallback"]
    end

    subgraph Handlers ["Module Button Handlers"]
        AwrnsHandler["handleAwarenessButton\nin index.js"]
    end

    subgraph Utils ["Utilities"]
        ModUtils["moduleButtonHandlersUtils.js\ngetKeyFromEvent,\nrunTxnAndRefresh,\ngetScreenCaption"]
        RejectUtils["rejectReasonUtils.js\nopenRejectReasonScreen"]
        ObsBtn["ObservationButtonHandlers.js\nsetPendingRejectForModule,\nhandleRejectReasonOkButton"]
        ObsSvc["ObservationService.js\nsendButtonClickToBackend"]
    end

    subgraph DB ["Database"]
        SP_Complete["AwrnsPlnEntryCompleted"]
        SP_Approve["AwrnsPlnApproved"]
        SP_Reject["AwrnsPlnRejected"]
        SP_Close["AwrnsPlnClosed"]
    end

    EntryScr -->|ButtonClicked| BtnEvt
    ApprovalScr -->|ButtonClicked| BtnEvt
    ExecScr -->|ButtonClicked| BtnEvt
    BtnEvt --> AwrnsHandler
    BtnEvt --> EntryScr
    BtnEvt --> ApprovalScr
    BtnEvt --> ExecScr
    AwrnsHandler --> ModUtils
    AwrnsHandler -->|executeSQLPromise| SP_Complete
    AwrnsHandler -->|executeSQLPromise| SP_Approve
    AwrnsHandler -->|executeSQLPromise| SP_Close
    AwrnsHandler --> RejectUtils
    AwrnsHandler --> ObsBtn
    ObsBtn -->|executeSQLPromise| SP_Reject
    EntryScr -->|executeSQLPromise| SP_Complete
    ApprovalScr -->|executeSQLPromise| SP_Approve
    ExecScr -->|executeSQLPromise| SP_Close
    ExecScr -->|sendButtonClickToBackend| ObsSvc
```

---

## 8. Setup screens (master data)

| Screen | Tag | Purpose | Handler |
|--------|-----|---------|---------|
| Awareness Courses | `HSE_TgAwrnsCrs` | Define available training courses | `Awareness_Courses.js` (toolbar only) |

---

## 9. Button dispatch — dual handler analysis

The Awareness module has an unusual pattern: some buttons are handled by **both** the centralized `handleAwarenessButton` (Step 1 in `buttonEvents.js`) **and** the screen-specific `ButtonClicked` (Step 2). Since Step 1 runs first and returns `true` on success, Step 2 only fires as a fallback.

| Button | handleAwarenessButton (Step 1) | Screen handler (Step 2) | Effective path |
|--------|-------------------------------|-------------------------|----------------|
| `AWRNSPLNENTRY_PLNENTRYCMPLTD` | `AwrnsPlnEntryCompleted` → `runTxnAndRefresh` | `AwrnsPlnEntryCompleted` → `executeSQLPromise` | Step 1 (module handler) |
| `AWRNSPLNAPRVL_PLNENTRYAPRVD` | `AwrnsPlnApproved` → `runTxnAndRefresh` | `AwrnsPlnApproved` → `executeSQLPromise` | Step 1 (module handler) |
| `AWRNSPLNAPRVL_PLNENTRYRJCTD` | `openRejectReasonScreen` + `setPendingRejectForModule('AWARENESS')` → `AwrnsPlnRejected` (**RQ_HSE_23_3_26_15_42**) | `sendButtonClickToBackend` (fallback if Step 1 skipped) | Step 1 (module handler) |
| `AWRNSPLNEXCTN_CRS_CRTUNPLNDCRS` | Not handled | `sendButtonClickToBackend` | Step 2 (screen handler) |
| `AWRNSPLNEXCTN_PLNCLSD` | `AwrnsPlnClosed` → `runTxnAndRefresh` (**RQ_HSE_23_3_26_15_42**) | `AwrnsPlnClosed` → `executeSQLPromise` | Step 1 (module handler) |

---

## 10. Workflow buttons – implementation status

| Button | Desktop behaviour | Web implementation | Status |
|--------|-------------------|--------------------|--------|
| `AWRNSPLNENTRY_PLNENTRYCMPLTD` | `AwrnsPlnEntryCompleted` | Module handler + screen handler both call SP | **OK** |
| `AWRNSPLNAPRVL_PLNENTRYAPRVD` | `AwrnsPlnApproved` | Module handler + screen handler both call SP | **OK** |
| `AWRNSPLNAPRVL_PLNENTRYRJCTD` | `OpenReasonsScr` → `rjctAwrnsPln` (`AwrnsPlnRejected`) | Reject reason screen + `handleRejectReasonOkButton` `case 'AWARENESS'` → `EXECUTE AwrnsPlnRejected` (**RQ_HSE_23_3_26_15_42**) | **OK** |
| `AWRNSPLNEXCTN_CRS_CRTUNPLNDCRS` | `createUnPlndCrs` (backend) | Screen handler: `sendButtonClickToBackend` | **OK** (backend delegation) |
| `AWRNSPLNEXCTN_PLNCLSD` | `AwrnsPlnClosed` | Module handler `runTxnAndRefresh`; screen handler fallback (**RQ_HSE_23_3_26_15_42**) | **OK** |

---

## 11. Known gaps vs desktop

| # | Gap | Impact | Resolution |
|---|-----|--------|------------|
| 1 | ~~**Reject has no reasons popup**~~ | ~~Medium~~ | **Resolved (RQ_HSE_23_3_26_15_42):** `openRejectReasonScreen('AWRNSPLN-L1', key)` + `setPendingRejectForModule('AWARENESS', …)` + `handleRejectReasonOkButton` → `EXECUTE AwrnsPlnRejected`. Reason text stored in `HSE_RJCTRSN`; SP second parameter `''` (desktop passes dialog text). |
| 2 | ~~**Close button not in module handler**~~ | ~~Low~~ | **Resolved (RQ_HSE_23_3_26_15_42):** `AWRNSPLNEXCTN_PLNCLSD` handled in `handleAwarenessButton` via `runTxnAndRefresh` / `AwrnsPlnClosed`. |
| 3 | ~~**No key guard message on Entry Complete**~~ | ~~Low~~ | **Resolved (RQ_HSE_23_3_26_15_42):** `AskYesNoMessage('You must save the Record first')` when no `SerialPK` on `AWRNSPLNENTRY_PLNENTRYCMPLTD`. |

**Screen captions for tracing:** `getScreenCaption` maps for `HSE_TGAWRNSPLNENTRY`, `HSE_TGAWRNSPLNAPRVL`, `HSE_TGAWRNSPLNEXCTN` in [`moduleButtonHandlersUtils.js`](hse/src/services/ModuleButtonHandlers/moduleButtonHandlersUtils.js) (**RQ_HSE_23_3_26_15_42**).

---

*End of Awareness / Training Plan UML documentation*
<!-- RQ_HSE_23_3_26_15_42 -->
