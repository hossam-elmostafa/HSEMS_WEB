# Permit to Work (PTW) Process – UML Documentation

<!-- RQ_HSE_23_3_26_3_36 -->

> **Source**: HSEMS C++ Desktop (`HSEMS-Win`) + Web (`hse` module)
> **Scope**: PTW lifecycle (`HSE_PTWSRGSTRENT`): Entry → Confirmation → Close, with Reject path
> **Date**: March 2026
> **See also**: [`HSEMS_Use_Cases_From_Desktop_Code.md`](./HSEMS_Use_Cases_From_Desktop_Code.md) §2.7

---

## 1. Process overview

The **Permit to Work** track covers: **Entry → Complete → Confirmation (Confirm) → Close**, with a **Reject** path using the reject-reason dialog (`PTWSRGSTRENT-L1`).

PTW has a **three-screen** workflow (Entry, Confirmation, Close) rather than the four-phase pattern used by some other modules. The "Confirmation" screen confirms the permit; the same screen (or a dedicated Close screen) later closes it.

**Screens and tags:**

| Screen | Web screen tag | C++ Category | Primary SPs | Screen handler |
|--------|----------------|--------------|-------------|----------------|
| Entry | `HSE_TgPTWsrgstrEnt` (`HSE_TGPTWSRGSTRENT`) | `PtwCategory` | `completePTWTXN` | `PTW_Register_Entry.js` |
| Confirmation | `HSE_TgPTWsrgstrcnfrmtn` (`HSE_TGPTWSRGSTRCNFRMTN`) | `PtwConfirmationCategory` | `confirmPTWTXN` | `PTW_Register_Confirmation.js` |
| Close | `HSE_TgPTWsrgstrCls` (`HSE_TGPTWSRGSTRCLS`) | `PtwClsCategory` | `closePTW` | `PTW_Register_Close.js` (RQ_HSE_23_3_26_6_00) |
| Inquiry | `HSE_TgPTWsrgstrInq` | `PTWInquiry` | (read-only filters) | `PTW_Registry_Inquiry.js` |

**Custom Buttons** (desktop): `PTWSRGSTRENT_ENTCMPLT`, `PTWSRGSTRENT_CNFRM`, `PTWSRGSTRENT_CLSPTW`, `PTWSRGSTRENT_RJC`

**Table**: `HSE_PTWSRGSTRENT`; key field: `PTWSRGSTRENT_PTWSNUM`

---

## 2. Activity diagram – Permit to Work (end-to-end)

```mermaid
flowchart TD
    Start(["User opens PTW Register Entry"]) --> A1

    subgraph PTW_ENTRY ["Phase 1: PTW Entry"]
        A1["Create / edit permit HSE_PTWSRGSTRENT"]
        A1 --> A2["Fill details: type, location, safety instructions"]
        A2 --> A3["Save record"]
        A3 --> A4{"Click Entry Completed\nPTWSRGSTRENT_ENTCMPLT?"}
        A4 -- No --> A2
        A4 -- Yes --> A5{"Record saved?"}
        A5 -- No --> A6["Message: save first"]
        A6 --> A2
        A5 -- Yes --> A7["completePTWTXN"]
        A7 --> A8["Tracing / status via SP"]
    end

    A8 --> B1

    subgraph PTW_CONFIRM ["Phase 2: PTW Confirmation"]
        B1["Reviewer opens Confirmation screen"]
        B1 --> B2{"Confirm PTWSRGSTRENT_CNFRM\nor Reject PTWSRGSTRENT_RJC?"}
        B2 -- Reject --> B3["Reject reason popup PTWSRGSTRENT-L1"]
        B3 --> B4["rejectPTW after OK"]
        B4 --> B5["Return toward Entry"]
        B2 -- Confirm --> B6["confirmPTWTXN"]
        B6 --> B7["Tracing / status via SP"]
    end

    B7 --> C1

    subgraph PTW_CLOSE ["Phase 3: PTW Close"]
        C1["Officer opens Close screen"]
        C1 --> C2{"Close PTWSRGSTRENT_CLSPTW\nor Reject PTWSRGSTRENT_RJC?"}
        C2 -- Reject --> C3["Reject reason popup PTWSRGSTRENT-L1"]
        C3 --> C4["rejectPTW after OK"]
        C2 -- Close --> C5["closePTW"]
        C5 --> C6["Refresh"]
    end

    C6 --> End(["Permit closed or rejected path"])

    style Start fill:#4CAF50,color:white
    style End fill:#2196F3,color:white
```

---

## 3. State machine

Status values enforced by stored procedures (`completePTWTXN`, `confirmPTWTXN`, `closePTW`, `rejectPTW`):

```mermaid
stateDiagram-v2
    [*] --> Incomplete : NEW / SAVE
    Incomplete --> Complete : PTWSRGSTRENT_ENTCMPLT\ncompletePTWTXN
    Complete --> Confirmed : PTWSRGSTRENT_CNFRM\nconfirmPTWTXN
    Confirmed --> Closed : PTWSRGSTRENT_CLSPTW\nclosePTW
    Complete --> Rejected : PTWSRGSTRENT_RJC\nrejectPTW
    Confirmed --> Rejected : PTWSRGSTRENT_RJC\nrejectPTW
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

## 4. Sequence diagram – Entry Complete

```mermaid
sequenceDiagram
    participant User
    participant WebUI as Web UI
    participant Handler as handlePTWButton
    participant DB as Database

    User->>WebUI: Click PTWSRGSTRENT_ENTCMPLT
    WebUI->>Handler: ButtonClicked → handleModuleButton
    Handler->>Handler: getKeyFromEvent (PTWSRGSTRENT_PTWSNUM)
    alt No key (record not saved)
        Handler->>WebUI: AskYesNoMessage("You must save the Record first")
    else Key exists
        Handler->>DB: EXECUTE completePTWTXN @key, @srcScreen, @userName
        DB-->>Handler: OK
        Handler->>WebUI: refreshData("")
    end
```

---

## 5. Sequence diagram – Confirmation with reject flow

```mermaid
sequenceDiagram
    participant User
    participant WebUI as Web UI
    participant Handler as handlePTWButton
    participant RejectScr as Reject Reason Screen
    participant RejectOK as handleRejectReasonOkButton
    participant DB as Database

    User->>WebUI: Click PTWSRGSTRENT_CNFRM
    WebUI->>Handler: handleModuleButton
    Handler->>DB: EXECUTE confirmPTWTXN @key, @userName, @srcScreen
    DB-->>Handler: OK
    Handler->>WebUI: refreshData

    Note over User,DB: --- Alternatively, Reject ---

    User->>WebUI: Click PTWSRGSTRENT_RJC
    WebUI->>Handler: handleModuleButton
    Handler->>RejectScr: openRejectReasonScreen("PTWSRGSTRENT-L1", key)
    Handler->>Handler: setPendingRejectForModule("PTW", key, srcScreen)
    User->>RejectScr: Enter reason, click OK
    RejectScr->>RejectOK: RJCTRSN_BTN_OK
    RejectOK->>DB: EXECUTE rejectPTW @key, @srcScreen, @userName
    DB-->>RejectOK: OK
    RejectOK->>WebUI: refreshData("", "REFRESH_SELECTED")
```

---

## 6. Sequence diagram – Close

```mermaid
sequenceDiagram
    participant User
    participant WebUI as Web UI
    participant Handler as handlePTWButton
    participant DB as Database

    User->>WebUI: Click PTWSRGSTRENT_CLSPTW (Close screen)
    WebUI->>Handler: handleModuleButton
    Handler->>DB: EXECUTE closePTW @key, @srcScreen, @userName
    DB-->>Handler: OK
    Handler->>WebUI: refreshData("")
```

---

## 7. Component diagram – Web architecture

```mermaid
flowchart LR
    subgraph Screens ["Screen Handlers"]
        EntryScr["PTW_Register_Entry.js\nHSE_TgPTWsrgstrEnt"]
        ConfirmScr["PTW_Register_Confirmation.js\nHSE_TgPTWsrgstrcnfrmtn"]
        CloseScr["PTW_Register_Close.js\nHSE_TgPTWsrgstrCls"]
        InqScr["PTW_Registry_Inquiry.js\nHSE_TgPTWsrgstrInq"]
    end

    subgraph Events ["Event Layer"]
        BtnEvt["buttonEvents.js\nhandleModuleButton dispatch"]
    end

    subgraph Handlers ["Module Button Handlers"]
        PTWHandler["handlePTWButton\nin index.js"]
    end

    subgraph Utils ["Utilities"]
        ModUtils["moduleButtonHandlersUtils.js\ngetKeyFromEvent, runTxnAndRefresh,\ngetScreenCaption"]
        RejectUtils["rejectReasonUtils.js\nopenRejectReasonScreen"]
        ObsBtn["ObservationButtonHandlers.js\nsetPendingRejectForModule,\nhandleRejectReasonOkButton"]
        TabSerial["tabNewSerialUtils.js\nsetNextSerialOnNewTab"]
    end

    subgraph DB ["Database"]
        SP_Complete["completePTWTXN"]
        SP_Confirm["confirmPTWTXN"]
        SP_Close["closePTW"]
        SP_Reject["rejectPTW"]
    end

    EntryScr -->|ButtonClicked| BtnEvt
    ConfirmScr -->|ButtonClicked| BtnEvt
    CloseScr -->|ButtonClicked| BtnEvt
    BtnEvt --> PTWHandler
    PTWHandler --> ModUtils
    PTWHandler --> RejectUtils
    PTWHandler --> ObsBtn
    PTWHandler -->|executeSQLPromise| SP_Complete
    PTWHandler -->|executeSQLPromise| SP_Confirm
    PTWHandler -->|executeSQLPromise| SP_Close
    ObsBtn -->|reject OK| SP_Reject
    InqScr -->|ChangeCriteria| DB
    EntryScr -->|toolBarButtonClicked NEW| TabSerial
```

---

## 8. Entry sub-features

### Safety Instructions tab serial

`PTW_Register_Entry.js` handles `toolBarButtonClicked`: when NEW on the Safety Instructions tab (`HSE_TGPTWSRGSTRENTSFTYINSTRCTNS`), it sets the next serial number on `HSE_PTWSRGSTRSFTYINSTRCTNS.PTWSRGSTRSFTYINSTRCTNS_SRLNO` via `setNextSerialOnNewTab`.

### Transaction number generation

On `SAVE` when the record is in new mode, `buttonEvents.js` generates a transaction number for `HSE_TGPTWSRGSTRENT`:

```
SCREEN_TAGS_REQUIRING_TXN_NO includes 'HSE_TGPTWSRGSTRENT'
getTXNKeyFldVal → { table: 'HSE_PTWSRGSTRENT', keyFld: 'PTWSRGSTRENT_PTWSNUM' }
→ EXECUTE generateNewTXNNum ...
```

---

## 9. Workflow buttons – implementation status

| Button | Desktop behaviour | Web implementation | Status |
|--------|-------------------|--------------------|--------|
| `PTWSRGSTRENT_ENTCMPLT` | `completePTWTXN` with "save first" guard | `handlePTWButton`: key check + `runTxnAndRefresh` | **OK** |
| `PTWSRGSTRENT_CNFRM` | `confirmPTWTXN` | `handlePTWButton`: `runTxnAndRefresh` | **OK** |
| `PTWSRGSTRENT_CLSPTW` | `closePTW` | `handlePTWButton`: `runTxnAndRefresh` | **OK** |
| `PTWSRGSTRENT_RJC` | Reject with reason (`rejectPTW`) | `handlePTWButton`: `openRejectReasonScreen` + `setPendingRejectForModule('PTW')` → `handleRejectReasonOkButton` `case 'PTW'` → `EXECUTE rejectPTW` | **OK** |

---

## 10. Inquiry screen

`PTW_Registry_Inquiry.js` (`HSE_TgPTWsrgstrInq`) filters on `PTWSRGSTRENT_RECSTS`:

| Button | Filter | Status |
|--------|--------|--------|
| `PTWSRGSTRENT_VWINCMPLT` | `WHERE (PTWSRGSTRENT_RECSTS=1)` | Incomplete |
| `PTWSRGSTRENT_VWRJCT` | `WHERE (PTWSRGSTRENT_RECSTS=3)` | Rejected |
| `PTWSRGSTRENT_VWCMPLT` | `WHERE (PTWSRGSTRENT_RECSTS=4)` | Complete |
| `PTWSRGSTRENT_VWCNFRM` | `WHERE (PTWSRGSTRENT_RECSTS=5)` | Confirmed |
| `PTWSRGSTRENT_VWCLSD` | `WHERE (PTWSRGSTRENT_RECSTS=6)` | Closed |
| `PTWSRGSTRENT_VWALL` | (no filter) | All |

---

## 11. Setup screens (master data)

| Screen | Tag | Purpose | Handler |
|--------|-----|---------|---------|
| PTW Type | `HSE_TgPTWsTyp` | Define permit types | `PTW_Type.js` (toolbar only) |
| PTW Responsibility | `HSE_TgPTWsRspnsblty` | Define responsibility roles | `PTW_Responsibility.js` (toolbar only) |

---

## 12. Known gaps vs desktop

| # | Gap | Impact | Resolution |
|---|-----|--------|------------|
| 1 | ~~**No SAVE before Confirm / Close**~~ | ~~Medium~~ | **Resolved (RQ_HSE_23_3_26_6_00):** Added `doToolbarAction('SAVE')` before `confirmPTWTXN` and `closePTW` in `handlePTWButton`, matching the Risk Assessment pattern. |
| 2 | ~~**No dedicated Close screen handler**~~ | ~~Low~~ | **Resolved (RQ_HSE_23_3_26_6_00):** Created `PTW_Register_Close.js` for `HSE_TgPTWsrgstrCls` and registered in `screenHandlers/index.js`. |

---

*End of PTW UML documentation*
