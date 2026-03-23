# Risk Assessment Process – UML Documentation

<!-- RQ_HSE_23_3_26_3_36 -->

> **Source**: HSEMS C++ Desktop (`HSEMS-Win`) + Web (`hse` module)
> **Scope**: Risk Assessment lifecycle (`HSE_RSKASSMNTENT`): Entry → Approval → Follow-up (Close), with Reject paths
> **Date**: March 2026
> **See also**: [`HSEMS_Use_Cases_From_Desktop_Code.md`](./HSEMS_Use_Cases_From_Desktop_Code.md) §2.5

---

## 1. Process overview

The **Risk Assessment** track covers: **Entry → Complete → Approval (Confirm) → Follow-up (Close)**, with a **Reject** path from Approval/Follow-up using the reject-reason dialog (`RSKASSMNTENT-L1`).

**Screens and tags:**

| Screen | Web screen tag | C++ Category | Primary SPs | Screen handler |
|--------|----------------|--------------|-------------|----------------|
| Entry | `HSE_TgRskAssmntEnt` (`HSE_TGRSKASSMNTENT`) | `RiskAssessmentEntryCategory` | `completeRskAssmntTXN` | `Risk_Assessment_Entry.js` |
| Approval | `HSE_TgRskAssmntApr` (`HSE_TGRSKASSMNTAPR`) | `RiskAssessmentApprovalCategory` | `confirmRskAssmntTXN` | `Risk_Assessment_Confirmation.js` |
| Follow-up | `HSE_TgRskAssmntFollwUpdt` (`HSE_TGRSKASSMNTFOLLWUPDT`) | `RiskAssessmentFollowUpCategory` | `closeRskAssmntTXN` | `Risk_Assessment_Follow-Up.js` **RQ_HSE_23_3_26_6_00** |
| Inquiry | `HSE_TgRskAssmntInq` | `RiskAssesmentInquiry` | (read-only filters) | `Risk_Assessment_Inquiry.js` |

**Custom Buttons** (desktop): `RSKASSMNTENT_ENTCMPLT`, `RSKASSMNTENT_APRV`, `RSKASSMNTENT_CLS`, `RSKASSMNTENT_RJC`, `RSKASSMNTENT_SHOWMATRIX`, `POTENTIAL_HAZARDS`, `TASK_RESPONSIBILITY`

**Table**: `HSE_RSKASSMNTENT`; key field: `RSKASSMNTENT_ASSMNTNO`

---

## 2. Activity diagram – Risk Assessment (end-to-end)

```mermaid
flowchart TD
    Start([User opens Risk Assessment Entry]) --> A1

    subgraph RA_ENTRY ["Phase 1: Risk Assessment Entry"]
        A1["Create / edit assessment HSE_RSKASSMNTENT"]
        A1 --> A2["Fill details: hazards, controls, job steps, task responsibilities"]
        A2 --> A3["Save record"]
        A3 --> A4{"Click Entry Completed\nRSKASSMNTENT_ENTCMPLT?"}
        A4 -- No --> A2
        A4 -- Yes --> A5{"Record saved?"}
        A5 -- No --> A6["Message: save first"]
        A6 --> A2
        A5 -- Yes --> A7["completeRskAssmntTXN"]
        A7 --> A8["Tracing / status via SP"]
    end

    A8 --> B1

    subgraph RA_APPROVAL ["Phase 2: Risk Assessment Approval"]
        B1["Approver opens Approval screen"]
        B1 --> B2{"Approve RSKASSMNTENT_APRV\nor Reject RSKASSMNTENT_RJC?"}
        B2 -- Reject --> B3["Reject reason popup RSKASSMNTENT-L1"]
        B3 --> B4["rejectRiskAssessment after OK"]
        B4 --> B5["Return toward Entry"]
        B2 -- Approve --> B6["confirmRskAssmntTXN"]
        B6 --> B7["Tracing / status via SP"]
    end

    B7 --> C1

    subgraph RA_FOLLOWUP ["Phase 3: Risk Assessment Follow-up"]
        C1["Officer opens Follow-up"]
        C1 --> C2{"Close RSKASSMNTENT_CLS\nor Reject RSKASSMNTENT_RJC?"}
        C2 -- Reject --> C3["Reject reason popup RSKASSMNTENT-L1"]
        C3 --> C4["rejectRiskAssessment after OK"]
        C2 -- Close --> C5["closeRskAssmntTXN"]
        C5 --> C6["Refresh"]
    end

    C6 --> End(["Cycle complete or rejected path"])

    style Start fill:#4CAF50,color:white
    style End fill:#2196F3,color:white
```

---

## 3. State machine

Status values enforced by stored procedures (`completeRskAssmntTXN`, `confirmRskAssmntTXN`, `closeRskAssmntTXN`, `rejectRiskAssessment`):

```mermaid
stateDiagram-v2
    [*] --> Incomplete : NEW / SAVE
    Incomplete --> Complete : RSKASSMNTENT_ENTCMPLT\ncompleteRskAssmntTXN
    Complete --> Confirmed : RSKASSMNTENT_APRV\nconfirmRskAssmntTXN
    Confirmed --> Closed : RSKASSMNTENT_CLS\ncloseRskAssmntTXN
    Complete --> Rejected : RSKASSMNTENT_RJC\nrejectRiskAssessment
    Confirmed --> Rejected : RSKASSMNTENT_RJC\nrejectRiskAssessment
    Rejected --> Incomplete : User corrects and re-saves

    state Incomplete {
        [*] : Status 1
    }
    state Complete {
        [*] : Status 4
    }
    state Confirmed {
        [*] : Status 5
    }
    state Closed {
        [*] : Status 6
    }
    state Rejected {
        [*] : Status 3
    }
```

---

## 4. Sequence diagram – Entry Complete (desktop parity)

```mermaid
sequenceDiagram
    participant User
    participant WebUI as Web UI
    participant Handler as handleRiskAssessmentButton
    participant DB as Database

    User->>WebUI: Click RSKASSMNTENT_ENTCMPLT
    WebUI->>Handler: ButtonClicked → handleModuleButton
    Handler->>Handler: getKeyFromEvent (RSKASSMNTENT_ASSMNTNO)
    alt No key (record not saved)
        Handler->>WebUI: AskYesNoMessage("You must save the Record first")
    else Key exists
        Handler->>DB: EXECUTE completeRskAssmntTXN @key, @srcScreen, @userName
        DB-->>Handler: OK
        Handler->>WebUI: refreshData("", "REFRESH_SELECTED")
    end
```

---

## 5. Sequence diagram – Approval with reject flow

```mermaid
sequenceDiagram
    participant User
    participant WebUI as Web UI
    participant Handler as handleRiskAssessmentButton
    participant RejectScr as Reject Reason Screen
    participant RejectOK as handleRejectReasonOkButton
    participant DB as Database

    User->>WebUI: Click RSKASSMNTENT_APRV
    WebUI->>Handler: handleModuleButton
    Handler->>DB: EXECUTE confirmRskAssmntTXN @key, @srcScreen, @userName
    DB-->>Handler: OK
    Handler->>WebUI: refreshData

    Note over User,DB: --- Alternatively, Reject ---

    User->>WebUI: Click RSKASSMNTENT_RJC
    WebUI->>Handler: handleModuleButton
    Handler->>RejectScr: openRejectReasonScreen("RSKASSMNTENT-L1", key)
    Handler->>Handler: setPendingRejectForModule("RISK", key, srcScreen)
    User->>RejectScr: Enter reason, click OK
    RejectScr->>RejectOK: RJCTRSN_BTN_OK
    RejectOK->>DB: EXECUTE rejectRiskAssessment @key, @srcScreen, @userName
    DB-->>RejectOK: OK
    RejectOK->>WebUI: refreshData("", "REFRESH_SELECTED")
```

---

## 6. Sequence diagram – Follow-up Close

```mermaid
sequenceDiagram
    participant User
    participant WebUI as Web UI
    participant Handler as handleRiskAssessmentButton
    participant DB as Database

    User->>WebUI: Click RSKASSMNTENT_CLS (Follow-up screen)
    WebUI->>Handler: handleModuleButton
    Handler->>DB: EXECUTE closeRskAssmntTXN @key, @userName, @srcScreen
    DB-->>Handler: OK
    Handler->>WebUI: refreshData("", "REFRESH_SELECTED")
```

---

## 7. Component diagram – Web architecture

```mermaid
flowchart LR
    subgraph Screens ["Screen Handlers"]
        EntryScr["Risk_Assessment_Entry.js\nHSE_TgRskAssmntEnt"]
        ApprScr["Risk_Assessment_Confirmation.js\nHSE_TgRskAssmntApr"]
        FollowScr["Risk_Assessment_Follow-Up.js\nHSE_TgRskAssmntFollwUpdt"]
        InqScr["Risk_Assessment_Inquiry.js\nHSE_TgRskAssmntInq"]
    end

    subgraph Events ["Event Layer"]
        BtnEvt["buttonEvents.js\nhandleModuleButton dispatch"]
        MenuEvt["menuEvents.js"]
    end

    subgraph Handlers ["Module Button Handlers"]
        RiskHandler["handleRiskAssessmentButton\nin index.js"]
    end

    subgraph Utils ["Utilities"]
        ModUtils["moduleButtonHandlersUtils.js\ngetKeyFromEvent, runTxnAndRefresh,\ngetScreenCaption"]
        RejectUtils["rejectReasonUtils.js\nopenRejectReasonScreen"]
        ObsBtn["ObservationButtonHandlers.js\nsetPendingRejectForModule,\nhandleRejectReasonOkButton"]
    end

    subgraph DB ["Database"]
        SP_Complete["completeRskAssmntTXN"]
        SP_Confirm["confirmRskAssmntTXN"]
        SP_Close["closeRskAssmntTXN"]
        SP_Reject["rejectRiskAssessment"]
    end

    EntryScr -->|ButtonClicked| BtnEvt
    ApprScr -->|ButtonClicked| BtnEvt
    FollowScr -->|ButtonClicked| BtnEvt
    BtnEvt --> RiskHandler
    RiskHandler --> ModUtils
    RiskHandler --> RejectUtils
    RiskHandler --> ObsBtn
    RiskHandler -->|executeSQLPromise| SP_Complete
    RiskHandler -->|executeSQLPromise| SP_Confirm
    RiskHandler -->|executeSQLPromise| SP_Close
    ObsBtn -->|reject OK| SP_Reject
    InqScr -->|ChangeCriteria| DB
```

---

## 8. Entry sub-features (desktop custom buttons)

Desktop `RiskAssessmentEntryCategory` exposes additional custom buttons beyond the workflow transitions:

| Button | Desktop behaviour | Web status |
|--------|-------------------|------------|
| `RSKASSMNTENT_ENTCMPLT` | `completeRskAssmntTXN` | **Implemented** in `handleRiskAssessmentButton` |
| `RSKASSMNTENT_APRV` | SAVE + `confirmRskAssmntTXN` | **Implemented** — `doToolbarAction('SAVE')` before SP **RQ_HSE_23_3_26_6_00** |
| `RSKASSMNTENT_CLS` | SAVE + `closeRskAssmntTXN` | **Implemented** — `doToolbarAction('SAVE')` before SP **RQ_HSE_23_3_26_6_00** |
| `RSKASSMNTENT_SHOWMATRIX` | Opens `RiskAssessment.jpg` from working folder | **Handled** (returns `true`, no-op in web; could open an asset) |
| `POTENTIAL_HAZARDS` | Opens potential hazard popup (`HSE_TGPTNLHZRD`) via `openScr` | **Implemented** — `openScr('HSE_TGPTNLHZRD', '', true, key)` **RQ_HSE_23_3_26_6_00** |
| `TASK_RESPONSIBILITY` | Opens task responsibility popup (`HSE_TGTskRsp`) via `openScr` | **Implemented** — `openScr('HSE_TGTskRsp', '', true, key)` **RQ_HSE_23_3_26_6_00** |
| `RSKASSMNTENT_RJC` | Reject with reason | **Implemented** in `handleRiskAssessmentButton` |

---

## 9. Inquiry screen

`Risk_Assessment_Inquiry.js` (`HSE_TgRskAssmntInq`) filters on `RSKASSMNTENT_RECSTS`:

| Button | Filter | Status |
|--------|--------|--------|
| `RSKASSMNTENT_VWINCMPLT` | `WHERE (RSKASSMNTENT_RECSTS=1)` | Incomplete |
| `RSKASSMNTENT_VWRJCT` | `WHERE (RSKASSMNTENT_RECSTS=3)` | Rejected |
| `RSKASSMNTENT_VWCMPLT` | `WHERE (RSKASSMNTENT_RECSTS=4)` | Complete |
| `RSKASSMNTENT_VWAPPRV` | `WHERE (RSKASSMNTENT_RECSTS=5)` | Approved |
| `RSKASSMNTENT_VWCLSD` | `WHERE (RSKASSMNTENT_RECSTS=6)` | Closed |
| `RSKASSMNTENT_VWALL` | (no filter) | All |

---

## 10. Setup screens (master data)

| Screen | Tag | Purpose |
|--------|-----|---------|
| Risk Level | `HSE_TgRskLvl` | Define risk severity levels |
| Risk Matrix | `HSE_TgRskRnkDesc` | Define risk ranking descriptions (severity x likelihood) |
| Risk Type | `HSE_TgRskTyp` | Define risk type categories |

These are data-entry screens with toolbar only (`ShowScreen` enables New/Save/Delete).

---

## 11. Transaction number generation

On `SAVE` when the record is in new mode, `buttonEvents.js` generates a transaction number for `HSE_TGRSKASSMNTENT`:

```
SCREEN_TAGS_REQUIRING_TXN_NO includes 'HSE_TGRSKASSMNTENT'
getTXNKeyFldVal → { table: 'HSE_RSKASSMNTENT', keyFld: 'RSKASSMNTENT_ASSMNTNO' }
→ EXECUTE generateNewTXNNum ...
```

This mirrors the C++ `Rsk_Asmnt_entry_Save` / `generateNewTXNNum` pattern.

---

*End of Risk Assessment UML documentation*
