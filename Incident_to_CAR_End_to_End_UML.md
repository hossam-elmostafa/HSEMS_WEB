# Incident / Accident → CAR → Closure — UML Documentation

> **Purpose**: Model **registering an incident**, **generating / tracking a Corrective Action Request (CAR)**, and **closing the cycle** so the original incident is **studied** (investigation) and **closed**.  
> **Sources**: HSEMS C++ (`HSEMS-Win\HSEMS.DLL`), Web (`hse` module), [`HSEMS_Use_Cases_From_Desktop_Code.md`](./HSEMS_Use_Cases_From_Desktop_Code.md), [`Incident_Reporting_Process_UML.md`](./Incident_Reporting_Process_UML.md).  
> **Date**: March 2026
<!-- RQ_HSE_23_3_26_3_36 -->
---

## 1. Scope & actors

| Actor | Role |
|-------|------|
| **Reporter / Entry user** | Registers the incident on **Incident Preliminary Entry** (`HSE_TGACDNTENT`). |
| **Reviewer** | Confirms or rejects at **Preliminary Review** (`HSE_TGACDNTCNFRMTN`). |
| **HSE / Follow-up officer** | Closes preliminary follow-up or drives investigation completion. |
| **Investigator** | Completes **Investigation Entry**; maintains **recommended actions** (may link **CAR** numbers). |
| **Approver** | **Investigation Approval** — close or reject investigation (`HSE_TGINCDNTINVSTGTNAPRVL`). |
| **CAR owner / action owners** | **CAR Entry → Review → Approval → Actions → Execution → CAR Follow-up → Close CAR** (`HSE_CRENTRY` lifecycle). |

**Data anchor**: Main incident record is **`HSE_ACDNTENT`** (`ACDNTENT_ACDNTNUM`). CAR records live in **`HSE_CRENTRY`** and link back via **CAR source**, **transaction year/number** (`CRENTRY_TXNYR`, `CRENTRY_TXNN`), and/or **recommended-action** lines (`HSE_ACDNTENTRECMNDACC` / `ACDNTENTRECMNDACC_CARNO`).

---

## 2. High-level process (activity overview)

The end-to-end story has **three coupled threads**:

1. **Incident (preliminary)**: Entry → Complete → Review (Confirm/Reject) → Follow-up (**Close** = `closeAccidentTXN`).
2. **Investigation (optional, policy-gated)**: Investigation Entry (`completeAccidentTXN`) → Investigation Approval (`closeAccidentTXN` or `rejectIncident`).
3. **CAR**: Create/link CAR → complete CAR workflow → **Close CAR** (`closeCARTXN` / status per `CRENTRY_CRSTT` in web handlers).

CAR may be **created before, during, or after** preliminary close, depending on **procedure**, **manual CAR Entry**, and **recommended actions**. Closing the **incident preliminary follow-up** is the usual **business “cycle complete”** for the original registration once investigation/CAR work is satisfied.

---

## 3. Activity diagram — end-to-end (incident register → CAR → incident closed)

```mermaid
flowchart TB
    subgraph REG["A. Register incident (preliminary)"]
        R0([Start: register incident]) --> R1[Create / edit HSE_ACDNTENT]
        R1 --> R2[Save]
        R2 --> R3{Entry Completed<br/>ACDNTENT_ENTCMPLT?}
        R3 -- No --> R1
        R3 -- Yes --> R4[Validations + completeAccidentTXN]
    end

    R4 --> B0

    subgraph PREL["B. Preliminary review & follow-up"]
        B0[Reviewer: Confirmation screen] --> B1{Confirm or Reject L1?}
        B1 -- Reject --> B2[Reject reasons + rejectIncident]
        B2 --> R1
        B1 -- Confirm --> B3[confirmAccidentTXN]
        B3 --> F0[Follow-up screen]
        F0 --> F1{Close or Reject L2?}
        F1 -- Reject --> F2[rejectIncident]
        F2 --> R1
        F1 -- Close --> F3[closeAccidentTXN<br/>preliminary cycle]
    end

    F3 --> G0
    B3 --> G0

    subgraph PAR["C. Parallel / optional tracks"]
        G0{Flash / Medical / Inquiry?}
        G0 -.-> G1[Flash / Medical SPs]
        G0 -.-> G2[Accident inquiry read-only]
    end

    G0 --> INV0

    subgraph INV["D. Investigation (if required)"]
        INV0{Investigation required<br/>per policy?}
        INV0 -- Yes --> INV1[Investigation Entry:<br/>ACDNTENT_ENTRYCMPLTD → completeAccidentTXN]
        INV1 --> INV2[Investigation Approval:<br/>ACDNTENT_CLS → closeAccidentTXN<br/>or ACDNTENT_RJC → rejectIncident]
        INV0 -- No --> CAR0
        INV2 --> CAR0
    end

    subgraph CARTR["E. CAR generation & linkage"]
        CAR0{How is CAR tied to incident?}
        CAR0 -->|Manual / policy / SP| C1[CAR created HSE_CRENTRY<br/>source + TXN yr/no]
        CAR0 -->|Investigation tab| C2[Recommended actions<br/>ACDNTENTRECMNDACC_CARNO]
        CAR0 -->|Reference tab| C3[ACDNTENTRFRNCS → CAR browse]
        C1 --> C4[CAR lifecycle]
        C2 --> C4
        C3 --> C4
    end

    subgraph CARLF["F. CAR lifecycle (summary)"]
        C4 --> D1[CAR Entry completed]
        D1 --> D2[Review / Approve / Actions / Execution]
        D2 --> D3[CAR Follow-up]
        D3 --> D4[Close CAR]
    end

    D4 --> Z0

    subgraph CLOSE["G. Close original incident “cycle”"]
        Z0[Study complete:<br/>corrective actions addressed]
        Z1{Preliminary follow-up<br/>still open?}
        Z1 -- Yes --> Z2[ACDNTENT_CLS → closeAccidentTXN]
        Z1 -- No --> Z3([Incident closed / documented])
        Z2 --> Z3
    end

    style R0 fill:#E8F5E9
    style Z3 fill:#E3F2FD
```

**Notes**

- **Order flexibility**: In many deployments, **F3** (preliminary close) happens **after** CAR execution; in others, **follow-up close** is the final gate. The diagram allows **CAR (E–F)** to run **overlapping** investigation **(D)**.
- **CAR auto-generation**: HSE Policy may trigger **`sp_Generate_CARs`** (and similar) when transactions close; **observations** have explicit web CAR generation — **incidents** often rely on **DB procedures** and **manual CAR Entry** with **source** and **TXN** fields. Verify in your database.
- **Desktop reference**: `IncdntInvstgtnAprvCategory.cpp` locks **recommended action** rows when **`ACDNTENTRECMNDACC_CARNO` ≠ "0"** — CAR is already tied.

---

## 4. State machine — incident (`HSE_ACDNTENT` conceptual)

Statuses are driven by **`completeAccidentTXN`**, **`confirmAccidentTXN`**, **`closeAccidentTXN`**, **`rejectIncident`** (exact codes are in DB/SPs). Typical pattern:

```mermaid
stateDiagram-v2
    [*] --> Incomplete: New / draft
    Incomplete --> Complete: completeAccidentTXN (entry or investigation entry)
    Complete --> Confirmed: confirmAccidentTXN (review)
    Complete --> Rejected: rejectIncident (L1)
    Confirmed --> Closed: closeAccidentTXN (follow-up)
    Confirmed --> Rejected: rejectIncident (L2)
    Rejected --> Incomplete: User corrects / resubmits
    Closed --> [*]
```

**Investigation** reuses **completeAccidentTXN** on investigation entry and **closeAccidentTXN** / **rejectIncident** on approval — states should be interpreted **together** with **screen** (preliminary vs investigation).

---

## 5. State machine — CAR (`HSE_CRENTRY` simplified)

Web handlers (`carCustomButtons.js`) transition **`CRENTRY_CRSTT`** (e.g. 05 Entry completed, 19 Accepted, 25 Closed). Desktop **`NewCarEntryCategory`** documents many of the same steps.

```mermaid
stateDiagram-v2
    [*] --> Draft: Create CAR / link to incident
    Draft --> EntryCompleted: ENTRY_COMPLETED / completeCARTXN path
    EntryCompleted --> InReview: Review stage
    InReview --> Approved: Approval / confirmCARTXN
    Approved --> InExecution: Accept CAR / actions
    InExecution --> FollowUp: Actions progress
    FollowUp --> Closed: closeCARTXN / CLOSE_CAR
    Closed --> [*]
```

---

## 6. Sequence diagram — incident registration, CAR link, closure

```mermaid
sequenceDiagram
    autonumber
    participant U as User (HSE)
    participant IE as Incident Entry / ModuleButtonHandlers
    participant IR as Review / Follow-up
    participant INV as Investigation screens
    participant CAR as CAR screens / carCustomButtons
    participant DB as Database (SPs)

    U->>IE: Save + ACDNTENT_ENTCMPLT
    IE->>DB: ValidateBeforeIncdntEntryComplt, completeAccidentTXN
    DB-->>IE: OK + refresh

    U->>IR: ACDNTENT_CNFRM (after SAVE)
    IR->>DB: confirmAccidentTXN (+ reject reason cleanup if needed)

    opt Investigation required
        U->>INV: ACDNTENT_ENTRYCMPLTD
        INV->>DB: completeAccidentTXN
        U->>INV: Edit recommended actions (CAR No.)
        U->>INV: ACDNTENT_CLS or RJC
        INV->>DB: closeAccidentTXN or rejectIncident
    end

    U->>CAR: Create / open CAR (source + incident TXN) or open linked CAR
    CAR->>DB: CAR workflow (complete / confirm / closeCARTXN, tracing)

    U->>IR: ACDNTENT_CLS on preliminary follow-up
    IR->>DB: SAVE + closeAccidentTXN
    DB-->>IR: Incident studied & closed
```

---

## 7. Component view (logical modules)

```mermaid
flowchart LR
    subgraph Incident["Incident module"]
        A[AccidentEntryCategory / handleIncidentButton]
        B[AccidentConfirmationCategory]
        C[AccidentFollowUpCategory]
        D[IncdntInvstgtnEntryCategory]
        E[IncdntInvstgtnAprvCategory]
    end

    subgraph CARmod["CAR module"]
        F[NewCarEntryCategory / carCustomButtons.js]
        G[Actions Entry / Review / Follow-up]
    end

    subgraph Data["Shared data"]
        H[(HSE_ACDNTENT)]
        I[(HSE_CRENTRY)]
        J[(HSE_ACDNTENTRECMNDACC)]
    end

    A --> H
    B --> H
    C --> H
    D --> H
    E --> H
    E --> J
    F --> I
    G --> I
    J -.->|CARNO| I
    H -.->|TXNYR/TXNN on CAR| I
```

---

## 8. Stored procedures (cross-reference)

| Concern | Procedures (typical) |
|---------|----------------------|
| Incident | `completeAccidentTXN`, `confirmAccidentTXN`, `closeAccidentTXN`, `rejectIncident` |
| CAR | `completeCARTXN`, `confirmCARTXN`, `closeCARTXN`, `rejectCARTXN`, `cancelCAR`, `sp_Generate_CARs` |
| Validation | `ValidateBeforeIncdntEntryComplt` |

See [§11 Appendix in HSEMS_Use_Cases_From_Desktop_Code.md](./HSEMS_Use_Cases_From_Desktop_Code.md) and [Incident_Reporting_Process_UML.md §6](./Incident_Reporting_Process_UML.md).

---

## 9. Web vs desktop notes

| Topic | Note |
|-------|------|
| **Incident buttons** | Centralized in **`handleIncidentButton`** (web); split by **category classes** (desktop). |
| **CAR from observation** | Web implements **client CAR insert** after close in **`ObservationButtonHandlers`**; **incident** CAR creation may be **SP/policy** or **manual** — align with DB. |
| **View Source TXN (CAR)** | Web **`carCustomButtons.js`** navigates **Observation / Near Miss / Audit**; extending to **Accident Inquiry** matches desktop intent for **incident-sourced** CARs. |
| **Medical / pop-ups** | Injury sub-flows may still be **partial** on web — see [`Incident_Process_Web_Validation.md`](./Incident_Process_Web_Validation.md). |

---

## 10. Diagram index

| # | Diagram type | Section |
|---|----------------|---------|
| 1 | **Activity** (flowchart) | §3 End-to-end |
| 2 | **State machine** | §4 Incident |
| 3 | **State machine** | §5 CAR |
| 4 | **Sequence** | §6 Registration → CAR → close |
| 5 | **Component** | §7 Modules & tables |

---

*End of document*
