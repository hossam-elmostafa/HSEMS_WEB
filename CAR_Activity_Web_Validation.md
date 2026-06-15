# CAR — Activity / web parity validation

**Status:** Draft — *Under review → Approved (date, reviewer)*  
**Inputs:** [CAR_Web_Implementation_Map.md](./CAR_Web_Implementation_Map.md) (approved)  
**Routing summary:** §1 also in [CAR_Web_Implementation_Map.md](./CAR_Web_Implementation_Map.md).

---

## 1. Executive summary

The web `hse` module implements a **large share** of the CAR / Action Tracking lifecycle in **[carCustomButtons.js](hse/src/utils/carCustomButtons.js)** with explicit **status codes**, **policy checks**, **tracing inserts**, and **reject-reason** orchestration. **RQ_HSE_30_3_26_14_57** adds named **`EXECUTE`** paths for **Creative Access Entry** (`completeCARTXN`), **Actions Received** (`AcceptCARExe`), and **Actions Under Execution** (`CARJobCmpltd`, `CARJobPndng`) via [carFoundationTxnButtons.js](hse/src/utils/carFoundationTxnButtons.js); see [CAR_Server_Button_Map.md](./CAR_Server_Button_Map.md). Reject-on-received/under-execution and some other catalogue calls (`confirmCARTXN`, `closeCARTXN` on CAR Entry, etc.) remain **Partial** until UAT / Profiler confirmation.

---

## 2. Legend

| Status | Meaning |
|--------|---------|
| **Yes** | Client (or documented server fallback) implements the behaviour with clear evidence. |
| **Partial** | Implemented differently (SQL/ChangeEntityStatus vs named SP) or needs server/UAT confirmation. |
| **No** | Not found in client map; may still exist server-only. |
| **N/A** | Environment / policy-dependent. |

---

## 3. Parity matrix (use case rows vs web)

| Ref | Desktop / use-case behaviour | Web status | Evidence |
|-----|------------------------------|------------|----------|
| U1 | Enter CAR | Yes | Generic shell CRUD + [CAR_Entry.js](hse/src/screenHandlers/Performance/Action%20Tracking/CAR_Entry.js) tab serials |
| U2 | Complete CAR (`completeCARTXN`) | Partial | **Creative Access** (`HSE_TgcrctveAccEnt`, `CRCTVEACCENT_ENTCMPLT`): **Yes** — [handleCompleteCorrectiveActionEntry](hse/src/utils/carFoundationTxnButtons.js) calls `completeCARTXN`. **CAR Entry** (`HSE_TGCRENTRY` ENTRY_COMPLETED): [handleEntryCompletedCarEntry](hse/src/utils/carCustomButtons.js) — tracing + status 05 + SAVE; does not call `completeCARTXN` |
| U3 | Review CAR | Yes | [CAR_Review.js](hse/src/screenHandlers/Performance/Action%20Tracking/CAR_Review.js) + `handleCarButton` |
| U4 | Accept at review (`setCARAcptd`) | Partial | [handleAcceptCarReview](hse/src/utils/carCustomButtons.js) — policy + `ChangeEntityStatus` + status 10 |
| U5 | Approve CAR (`confirmCARTXN`) | Partial | [handleApproveCarApproval](hse/src/utils/carCustomButtons.js) — policy + `ChangeEntityStatus` + status 15 + dept/project copy |
| U6 | Reject CAR | Yes | `REJECT_CAR` + [handleRejectReasonOkForCAR](hse/src/utils/carCustomButtons.js) + [ObservationService](hse/src/services/Observation%20service/ObservationService.js) |
| U7 | Cancel CAR (`cancelCAR`) | Yes | `CANCEL_CAR` on `HSE_TGCRAPRVL` + pending reject flow → status 20 |
| U8 | Close CAR (`closeCARTXN`) | Partial | [handleCloseCar](hse/src/utils/carCustomButtons.js) — validations + status 25 + SAVE |
| U9 | Actions — define / complete entry | Partial | [handleEntryCompletedActionsEntry](hse/src/utils/carCustomButtons.js) — validations + status 16 + updates |
| U10 | Execute actions (`AcceptCARExe`, `RejectCARExe`, `CARJobCmpltd`) | Partial | **Accept / job complete / job pending:** [handleCarFoundationTxnButtons](hse/src/utils/carFoundationTxnButtons.js) on [Actions_Received.js](hse/src/screenHandlers/Performance/Action%20Tracking/Actions_Received.js) + [Actions_Under_Execution.js](hse/src/screenHandlers/Performance/Action%20Tracking/Actions_Under_Execution.js). **Reject** (`RJCTEXECTN`, `RJCT_UNDREXCTN`): not wired (desktop `rejectCARTXN` + reasons); other buttons may still use `sendButtonClickToBackend` |
| U11 | Follow up CAR | Yes | [CAR_Follow_Up.js](hse/src/screenHandlers/Performance/Action%20Tracking/CAR_Follow_Up.js) — reject/close/corrective actions |
| U12 | Edit CAR | Partial | [CAR_Editing.js](hse/src/screenHandlers/Performance/Action%20Tracking/CAR_Editing.js) — view review/approve info + corrective actions; other buttons → backend |
| U13 | View source transaction | Partial | [handleViewSourceTxn](hse/src/utils/carCustomButtons.js) — extended **RQ_HSE_30_3_26_14_57** to performance, site survey, risk, drill, potential hazard, vehicle, loss, incident/accident (requires `CRENTRY_TXNN`); UAT per source + inquiry screen availability (e.g. `HSE_TgptnlhzrdInq` JSON may exist before handler registration) |
| U14 | View reject reasons | Yes | `CRENTRY_VWRJCTRSNS` on CAR Entry |
| U15 | Review / approval info popups | Yes | `CAR_REVIEW_INFO`, `CAR_APPROVAL_INFO`, `VIEW_CAR_*` |
| U16 | Actions Review accept | Partial | `ACCEPT_CAR` on `HSE_TGACTNSRVIW` — SQL + tracing + target date update |
| U17 | Corrective Actions Inquiry filters | N/A | Shell APP JSON / `ChangeCriteria` — verify in environment |

---

## 4. Parity outside main flow

| Topic | Web status | Notes |
|-------|------------|-------|
| Policy gates for review/approval info | Yes | `HSE_HSEPLC_ADT` queries in `handleAcceptCarReview` / `handleApproveCarApproval` |
| Reject module types | Yes | `REJECT_SOURCE_BY_SCREEN` / `openRejectReasonScreen` |
| Tracing tab inserts | Yes | `HSE_CRENTRY_TRC` inserts where implemented |
| Tab NEW line serials | Yes | CAR Entry, Actions Under Execution |

---

## 5. Gap backlog (from Partial / No)

| ID | Item | Owner | Notes |
|----|------|-------|-------|
| G1 | Confirm `completeCARTXN` / `confirmCARTXN` / `closeCARTXN` equivalence | Dev + DBA | Compare SQL Profiler desktop vs web on same record |
| G2 | Document server handlers for Actions Received / Under Execution buttons | Backend | Trace `sendButtonClickToBackend` |
| G3 | View Source — additional CAR sources | Product | Extend `handleViewSourceTxn` if desktop supports more |
| G4 | UAT scripted walkthrough | QA | See [CAR_UAT_Signoff.md](./CAR_UAT_Signoff.md) |

---

## 6. Separate requirements (infrastructure)

**Packaging:** WebInfra consumes `hse` via `file:modules/hse-*.tgz`; after changing `HSEMS_WEB_Original/hse`, repack and refresh the client dependency so runtime matches source. If G2 surfaces gaps only solvable server-side, add rows here before implementing outside `hse`.

---

## 7. Implementation record (Phase 5)

| Date | Change | Matrix refs updated | PR / issue |
|------|--------|---------------------|------------|
| 2026-03-28 | **Plan application — documentation only.** No `hse` code changes in this pass. | N/A | — |
| 2026-03-28 | **RQ_HSE_30_3_26_14_57:** `carFoundationTxnButtons.js`, Creative Access screen handler, Actions Received/Under Execution wiring, `handleViewSourceTxn` extensions, `index.js` registration. | U2, U10, U13, §5 | — |

*After code fixes: add rows and flip matrix cells from Partial → Yes with file/line evidence.*

---

## 8. Document control

| Version | Date | Notes |
|---------|------|--------|
| 0.1 | 2026-03-28 | Initial matrix + gaps |
