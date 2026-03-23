# Incident / Accident (Preliminary) Process – UML Documentation

<!-- RQ_HSE_23_3_26_3_36 -->

> **Source**: HSEMS C++ Desktop (`HSEMS-Win`) + Web (`hse` module)  
> **Scope**: Preliminary incident lifecycle (`HSE_ACDNTENT`), investigation entry/approval, plus pointers to Flash & Medical tracks  
> **Date**: March 2026  
> **See also**: [`Incident_to_CAR_End_to_End_UML.md`](./Incident_to_CAR_End_to_End_UML.md) — **register incident → CAR → close cycle** (activity, state, sequence, component diagrams).

---

## 1. Process overview

The **Incident Preliminary** track covers: **Entry → Complete → Review (Confirm) → Follow-up (Close)**, with **Reject** paths from Review and Follow-up using the reject-reason dialog (`ACDNTENT-L1` / `ACDNTENT-L2`).

**Separate tracks** (different tables / SPs):

| Track | Web screen tags (examples) | Primary SPs |
|-------|----------------------------|-------------|
| Flash report | `HSE_TgIncdntFlshRprt`, `HSE_TgIncdntFlshRprtRvew` | `CompleteFlashEntry`, `RejectFlashFromReview`, `AccptFlashFromReview` |
| Medical report | `HSE_TgIncdntMdclRprt`, `HSE_TgIncdntMdclRprtFlwUp` | `completeIncdntMdclRprt`, `closeIncdntMdclRprt` |
| Investigation | `HSE_TgIncdntInvstgtnEntry`, `HSE_TgIncdntInvstgtnAprvl` | `completeAccidentTXN` (entry), `closeAccidentTXN` / **direct** `rejectIncident` (approval) |

---

## 2. Activity diagram – Preliminary incident (end-to-end)

**Policy (not shown as nodes):** Desktop permits **Preliminary Review** only if HSE policy **Accident Confirmation** is enabled (`HSEPLC_ACDNTCNFRMTNRQRD`). Web mirrors this in [`Incident_Preliminary_Review.js`](../hse/src/screenHandlers/Safety/Incident/Incident_Preliminary_Review.js) `ShowScreen`.

```mermaid
flowchart TD
    Start([Reporter opens Incident Preliminary Entry]) --> A1

    subgraph INC_ENTRY ["Phase 1: Incident Preliminary Entry"]
        A1[Create / edit accident HSE_ACDNTENT]
        A1 --> A2[Fill details, tabs: injured persons, causes, actions, attachments]
        A2 --> A3[Save record]
        A3 --> A4{Click Entry Completed ACDNTENT_ENTCMPLT?}
        A4 -- No --> A2
        A4 -- Yes --> A5{Record saved?}
        A5 -- No --> A6[Message: save first]
        A5 -- Yes --> A7{ValidateBeforeIncdntEntryComplt = Y?}
        A7 -- No --> A8{User confirms proceed?}
        A8 -- No --> A2
        A8 -- Yes --> A9
        A7 -- Yes --> A9["completeAccidentTXN"]
        A9 --> A10[Tracing / status via SP]
    end

    A10 --> B1

    subgraph INC_REVIEW ["Phase 2: Incident Preliminary Review"]
        B1[Reviewer opens Confirmation screen]
        B1 --> B2{Confirm ACDNTENT_CNFRM or Reject ACDNTENT_RJC?}
        B2 -- Reject --> B3[Reject reason popup ACDNTENT-L1]
        B3 --> B4["rejectIncident after OK"]
        B4 --> B5[Return toward Entry]
        B2 -- Confirm --> B6[SAVE toolbar]
        B6 --> B7{Pending reject reasons ACDNTENT-L1?}
        B7 -- Yes --> B8[Prompt delete / abort]
        B8 -- Delete --> B9[Delete HSE_RJCTRSN rows]
        B9 --> B10
        B7 -- No --> B10["confirmAccidentTXN"]
        B8 -- No --> B11[Abort]
        B10 --> B12[Tracing / status via SP]
    end

    B12 --> C1

    subgraph INC_FOLLOW ["Phase 3: Incident Preliminary Follow-up"]
        C1[Officer opens Follow-up]
        C1 --> C2{Close ACDNTENT_CLS or Reject ACDNTENT_RJC?}
        C2 -- Reject --> C3[Reject reason popup ACDNTENT-L2]
        C3 --> C4["rejectIncident after OK"]
        C2 -- Close --> C5[SAVE toolbar]
        C5 --> C6["closeAccidentTXN"]
        C6 --> C7[Refresh]
    end

    C7 --> End([Cycle complete or rejected path])

    style Start fill:#4CAF50,color:white
    style End fill:#2196F3,color:white
```

---

## 3. Activity diagram – Investigation approval (subset)

```mermaid
flowchart LR
    A[Investigation Approval screen] --> B{ACDNTENT_CLS or ACDNTENT_RJC?}
    B -->|CLS| C[SAVE]
    C --> D["closeAccidentTXN"]
    B -->|RJC| E["rejectIncident directly no reject-reason dialog"]
    D --> F[Refresh]
    E --> F
```

*C++ reference*: `IncdntInvstgtnAprvCategory.cpp` — `CloseAccident` / `RejectAccident` (no `Reject()` helper).

---

## 4. State machine (conceptual)

Exact numeric statuses are enforced in DB / SPs (`completeAccidentTXN`, `confirmAccidentTXN`, `closeAccidentTXN`, `rejectIncident`). Typical pattern (verify against your DB):

| Phase | Action | Direction |
|-------|--------|-------------|
| Entry | Complete | Incomplete → Submitted for review |
| Review | Confirm | → Approved for follow-up |
| Review | Reject | → Back for correction |
| Follow-up | Close | → Closed |
| Follow-up | Reject | → Prior state per SP |

---

## 5. Sequence diagram – Preliminary confirm (desktop parity)

```mermaid
sequenceDiagram
    participant U as Reviewer
    participant W as Web handler
    participant DB as Database

    U->>W: ACDNTENT_CNFRM
    W->>W: doToolbarAction SAVE
    W->>DB: COUNT HSE_RJCTRSN ACDNTENT-L1 pending
    alt Pending reasons
        U->>W: Confirm delete
        W->>DB: DELETE pending RJCTRSN
    end
    W->>DB: EXECUTE confirmAccidentTXN key, caption, user
    W->>W: refresh REFRESH_SELECTED
```

---

## 6. Stored procedure reference (preliminary + investigation)

| Procedure | Typical parameters | Called from |
|-----------|-------------------|-------------|
| `completeAccidentTXN` | key, screen caption, user | Preliminary Entry (`ACDNTENT_ENTCMPLT`), Investigation Entry (`ACDNTENT_ENTRYCMPLTD`) |
| `confirmAccidentTXN` | key, screen caption, user | Preliminary Review (`ACDNTENT_CNFRM` / `ACDNTENT_APRV`) |
| `closeAccidentTXN` | key, user, screen caption | Follow-up (`ACDNTENT_CLS`), Investigation Approval (`ACDNTENT_CLS`) |
| `rejectIncident` | key, screen caption, user | After reject-reason OK (L1/L2), or **direct** from Investigation Approval |

### Flash / Medical (handlers in screen JS)

| Procedure | Screen |
|-----------|--------|
| `CompleteFlashEntry` | Flash Entry |
| `RejectFlashFromReview` / `AccptFlashFromReview` | Flash Review |
| `completeIncdntMdclRprt` | Medical Entry |
| `closeIncdntMdclRprt` | Medical Follow-up |

---

## 7. Reject reason module types (HSE_RJCTRSN)

| Screen | `RJCTRSN_MODULETYPE` |
|--------|----------------------|
| Preliminary Review | `ACDNTENT-L1` |
| Preliminary Follow-up | `ACDNTENT-L2` |

**Investigation Approval** does not use this popup in desktop; it calls `rejectIncident` immediately.

---

## 8. C++ reference files

| File | Responsibility |
|------|----------------|
| `AccidentEntryCategory.cpp` | `ACDNTENT_ENTCMPLT`, validations, `CompleteAccident` |
| `AccidentConfirmationCategory.cpp` | SAVE, `ACDNTENT_CNFRM`, reject reasons, `confirmAccidentTXN` |
| `AccidentFollowUpCategory.cpp` | SAVE, `ACDNTENT_CLS`, `closeAccidentTXN` |
| `AccidentCategory.cpp` | `ACDNTENT_RJC`, `rejectRecord` → L1/L2, `rejectIncident` after dialog |
| `IncdntInvstgtnEntryCategory.cpp` | `ACDNTENT_ENTRYCMPLTD`, `completeAccidentTXN` |
| `IncdntInvstgtnAprvCategory.cpp` | `ACDNTENT_CLS`, `ACDNTENT_RJC` direct SPs |

---

*End of document*
