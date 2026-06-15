# Site Surveys Process -- UML Documentation

<!-- RQ_HSE_23_3_26_22_44 -->

> **Source**: HSEMS C++ Desktop (`HSEMS-Win`, `SitSrvyEntCategory.cpp`, `SitSrvyCnfrmtnCategory.cpp`, `SitSrvyFlwupCategory.cpp`, `SitSrvyCategory.cpp`) + Web (`hse` module)
> **Scope**: Site survey lifecycle from **Site Survey Entry** through **Confirmation**, **Result / Follow-up**, and **Close**, including **Reject** and **Required Actions** on findings
> **Date**: March 2026
> **See also**: [`HSEMS_Use_Cases_From_Desktop_Code.md`](./HSEMS_Use_Cases_From_Desktop_Code.md) §4.1, §10.4 (CAR auto-generation), §11 (stored procedures)

---

## 1. Process overview

The **Site Survey** module records **site visits** under **Performance → Site Survey**. A single header transaction (`HSE_SITSRVYENT`, key `SITSRVYENT_SITSRVYNO`) moves through **Entry → Complete → Confirm → Close**, with **Reject** returning the record for correction. **Findings** and **required actions** are captured on sub-forms and a popup linked to each finding line.

### 1.1 Operational screens

| Phase | Screen (caption) | Tag | C++ category (reference) | Primary table | Key field |
|-------|------------------|-----|----------------------------|---------------|-----------|
| Entry | Site Survey Entry | `HSE_TgSitSrvyEnt` | `SitSrvyEntCategory` / `SitSrvyCategory` | `HSE_SITSRVYENT` | `SITSRVYENT_SITSRVYNO` |
| Confirmation | Site Survey Confirmation | `HSE_TgSitSrvyCnfrmtn` | `SitSrvyCnfrmtnCategory` / `SitSrvyCategory` | (same header) | (same) |
| Follow-up / result | Site Survey Result Entry | `HSE_TgSitSrvyFlwup` | `SitSrvyFlwupCategory` | (same header) | (same) |
| Inquiry | Site Survey Inquiry | `HSE_TgSitSrvyInq` | Read-only filters | (same) | (same) |

### 1.2 Sub-forms and popup (findings & actions)

| Artefact | Tag / table | Purpose |
|----------|-------------|---------|
| Findings tab | `HSE_SITSRVYENTFNDNG` (`HSE_TGSITSRVYENTFNDNG`); link field `MAINLINK` | Site visit findings; desktop auto-assigns `SITSRVYENTFNDNG_SRIL` when finding fields change |
| Required actions popup | `HSE_TgSitSrvyRqrdActn` → `HSE_SITSRVYRQRDACTN` | Corrective actions per finding; `SITSRVYRQRDACTN_LNK` ties to finding; action number auto-serial on desktop |

Default **TXNSTS** passed when opening Required Actions from Entry vs Confirmation differs on desktop (`1` vs `4`), reflecting the survey’s workflow stage.

### 1.3 Stored procedures

| Procedure | Typical phase | Purpose |
|-----------|---------------|---------|
| `completeSitSrvyTXN` | Entry | Submit survey for confirmation (after record saved) |
| `confirmSiteSrvy` | Confirmation | Confirm survey findings |
| `closeSiteSurveyTXN` | Result / follow-up | Close the survey (end of cycle) |
| `rejectSitSrvy` | Entry / Confirmation / shared handler | Reject with reason (after reject-reason flow on desktop) |

Parameters match desktop/web pattern: survey key, **source screen caption**, and **user name** (argument order matches each `EXECUTE` in C++/web).

### 1.4 Custom buttons

| Button | Screen(s) | Behaviour |
|--------|-----------|-----------|
| `SITSRVYENT_ENTCMPLT` | Site Survey Entry | Validate saved record → `EXECUTE completeSitSrvyTXN` → refresh |
| `SITSRVYENT_CNFRM` | Site Survey Confirmation | `EXECUTE confirmSiteSrvy` → refresh (web: `ModuleButtonHandlers`) |
| `SITSRVYENT_CLS` | Site Survey Result Entry | Desktop: save then close; web: `SAVE` toolbar then `EXECUTE closeSiteSurveyTXN` |
| `SITSRVYENT_RJCT` | Entry / Confirmation / category | Reject reason → `EXECUTE rejectSitSrvy` (web: `ModuleButtonHandlers`) |
| `SITSRVYRQRDACTN_RQRDACTNBTN` | Entry / Confirmation / Inquiry | Open Required Actions popup filtered by current finding `MAINLINK` |

### 1.5 Cross-cutting behaviour

- **Tracing**: Status-changing actions are traced like other HSEMS transactions (`InsertRecIntoTracingTab` / `updateTXNSts` patterns; see use-case appendix).
- **CAR auto-generation**: When the survey is **closed** (and per **HSE Policy**), the system may **auto-generate a CAR** linked to the originating survey (see [`HSEMS_Use_Cases_From_Desktop_Code.md`](./HSEMS_Use_Cases_From_Desktop_Code.md) §10.4).

---

## 2. Activity diagram -- Site survey (entry to cycle end)

<!-- RQ_HSE_23_3_26_22_44 -->

**Web implementation check** (parity vs this diagram): [`Site_Surveys_Activity_Web_Validation.md`](./Site_Surveys_Activity_Web_Validation.md) — §2 nodes **Start–End** covered, including **E4** finding serials and required-action serials on the popup (**RQ_HSE_23_3_26_22_44**).

```mermaid
flowchart TD
    Start(["User opens Performance → Site Survey → Site Survey Entry\nHSE_TgSitSrvyEnt"]) --> E1

    subgraph ENTRY ["Phase 1: Site Survey Entry"]
        E1["Create / edit header\nHSE_SITSRVYENT"]
        E1 --> E2["Save record\n(SITSRVYENT_SITSRVYNO assigned)"]
        E2 --> E3["Enter visit details, locations, team,\nattachments as per screen config"]
        E3 --> E4["Findings tab: add rows\nHSE_SITSRVYENTFNDNG"]
        E4 --> E5{"Required actions\nfor a finding?"}
        E5 -- Yes --> E6["Select finding row\n(MAINLINK populated)"]
        E6 --> E7["Click SITSRVYRQRDACTN_RQRDACTNBTN"]
        E7 --> E8["Open popup HSE_TgSitSrvyRqrdActn\nfiltered by SITSRVYRQRDACTN_LNK"]
        E8 --> E4
        E5 -- No --> E9{"Submit for confirmation?\nSITSRVYENT_ENTCMPLT"}
        E9 -- No, edit more --> E3
        E9 -- Yes --> E10{"Record saved\nand key present?"}
        E10 -- No --> E11["Prompt: save record first"]
        E11 --> E3
        E10 -- Yes --> E12["EXECUTE completeSitSrvyTXN"]
        E12 --> E13["Refresh; record advances\nto Complete / awaiting confirmation"]
    end

    E13 --> C1

    subgraph CONFIRM ["Phase 2: Site Survey Confirmation"]
        C1["Open Site Survey Confirmation\nHSE_TgSitSrvyCnfrmtn"]
        C1 --> C2["Reviewer opens record\nin confirmation queue"]
        C2 --> C3{"Decision?"}
        C3 -- Reject --> C4["SITSRVYENT_RJCT\n→ reject reason → EXECUTE rejectSitSrvy"]
        C4 --> C5["Record rejected;\nreturn to Entry for correction"]
        C5 --> E1
        C3 -- Confirm --> C6["SITSRVYENT_CNFRM"]
        C6 --> C7["EXECUTE confirmSiteSrvy"]
        C7 --> C8["Refresh; record confirmed;\nready for follow-up / results"]
    end

    C8 --> F1

    subgraph FOLLOWUP ["Phase 3: Site Survey Result Entry (follow-up)"]
        F1["Open Site Survey Result Entry\nHSE_TgSitSrvyFlwup"]
        F1 --> F2["Record follow-up / closure data\nas defined on screen"]
        F2 --> F3["Save"]
        F3 --> F4["Click SITSRVYENT_CLS\n(desktop: may SAVE first explicitly)"]
        F4 --> F5["EXECUTE closeSiteSurveyTXN"]
        F5 --> F6["Refresh; survey closed"]
    end

    F6 --> EndNode(["Cycle end: survey closed\n(policy may trigger CAR)"])

    style Start fill:#4CAF50,color:white
    style EndNode fill:#2196F3,color:white
    style C3 fill:#FF9800,color:white
    style E9 fill:#FF9800,color:white
```

---

## 3. State machine -- Transaction lifecycle (logical)

Inquiry screens use filters aligned with **Incomplete, Rejected, Complete, Confirmed, Closed** (see use-case §5). Exact numeric **TXNSTS** values are defined in the database and updated by the stored procedures below; the diagram names **business states** those stages represent.

<!-- RQ_HSE_23_3_26_22_44 -->

```mermaid
stateDiagram-v2
    [*] --> Incomplete : NEW / initial save

    Incomplete --> Complete : SITSRVYENT_ENTCMPLT\ncompleteSitSrvyTXN

    Complete --> Confirmed : SITSRVYENT_CNFRM\nconfirmSiteSrvy

    Confirmed --> Closed : SITSRVYENT_CLS\ncloseSiteSurveyTXN

    Complete --> Rejected : SITSRVYENT_RJCT\nrejectSitSrvy
    Confirmed --> Rejected : SITSRVYENT_RJCT\nrejectSitSrvy

    Rejected --> Incomplete : User corrects record\nand resubmits from Entry

    note right of Closed
        End of survey cycle.
        HSE Policy may auto-generate CAR.
    end note
```

---

## 4. Sequence diagram -- Happy path (complete → confirm → close)

<!-- RQ_HSE_23_3_26_22_44 -->

```mermaid
sequenceDiagram
    participant User
    participant EntryUI as Site Survey Entry
    participant ConfUI as Site Survey Confirmation
    participant FlwUI as Site Survey Result Entry
    participant Web as Web layer\n(screen handlers + ModuleButtonHandlers)
    participant DB as Database

    User->>EntryUI: Create survey, findings, save
    User->>EntryUI: Click SITSRVYENT_ENTCMPLT
    EntryUI->>Web: ButtonClicked (Site_Survey_Entry.js)
    Web->>DB: EXECUTE completeSitSrvyTXN(key, caption, user)
    DB-->>Web: OK
    Web->>EntryUI: refreshData

    User->>ConfUI: Open confirmation record
    User->>ConfUI: Click SITSRVYENT_CNFRM
    ConfUI->>Web: Route to handleSiteSurveyButton
    Web->>DB: EXECUTE confirmSiteSrvy(key, caption, user)
    DB-->>Web: OK
    Web->>ConfUI: refreshData

    User->>FlwUI: Enter follow-up, save
    User->>FlwUI: Click SITSRVYENT_CLS
    FlwUI->>Web: SAVE then button path
    Web->>DB: EXECUTE closeSiteSurveyTXN(key, user, caption)
    DB-->>Web: OK
    Web->>FlwUI: refreshData
    Note over DB: Policy may create linked CAR
```

---

## 5. Sequence diagram -- Reject path

<!-- RQ_HSE_23_3_26_22_44 -->

```mermaid
sequenceDiagram
    participant User
    participant UI as Entry or Confirmation
    participant Web as Web layer\n(ModuleButtonHandlers / reject flow)
    participant DB as Database

    User->>UI: Click SITSRVYENT_RJCT
    UI->>Web: Button event with current key
    Note over Web: Desktop: rejectRecord + reason dialog;\nWeb: same SP after reason captured
    Web->>DB: EXECUTE rejectSitSrvy(key, caption, user)
    DB-->>Web: OK
    Web->>UI: refreshData
    User->>UI: Return to Site Survey Entry\nto correct and complete again
```

---

## 6. Use case summary (actors vs system)

| Actor step | System response |
|------------|-----------------|
| HSE user creates site survey and findings | Persist `HSE_SITSRVYENT` / `HSE_SITSRVYENTFNDNG`; optional required actions in `HSE_SITSRVYRQRDACTN` |
| User completes entry | `completeSitSrvyTXN`; status moves toward confirmation queue |
| Reviewer confirms | `confirmSiteSrvy` |
| Owner records follow-up and closes | `closeSiteSurveyTXN`; cycle ends; CAR may be generated |
| Reviewer rejects | `rejectSitSrvy`; record returns for correction at entry |

---

*End of document*
