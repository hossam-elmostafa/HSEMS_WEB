# CAR — Web implementation map (`hse`)

**Status:** Draft — *Under review → Approved (date, reviewer)*  
**Inputs:** [CAR_Desktop_Behaviour_Spec.md](./CAR_Desktop_Behaviour_Spec.md), [CAR_Lifecycle_Process_UML.md](./CAR_Lifecycle_Process_UML.md) (approved)  
**Next document:** [CAR_Activity_Web_Validation.md](./CAR_Activity_Web_Validation.md) (after approval)

---

## 1. How button routing works (CAR)

1. [buttonEvents.js](hse/src/events/buttonEvents.js) — `ButtonClicked` calls `handleModuleButton` first (CAR is **not** implemented in [ModuleButtonHandlers/index.js](hse/src/services/ModuleButtonHandlers/index.js); comment states Observation and CAR use ObservationService + `carCustomButtons`).
2. `getScreenHandler(strScrTag).ButtonClicked(eventObj)` runs for the active screen.
3. Each CAR / Actions screen handler calls `handleCarButton` then, if it returns `false`, **`sendButtonClickToBackend`** ([ObservationService.js](hse/src/services/Observation%20service/ObservationService.js)) so the server can execute any button not handled in the client.
4. **Reject reason OK:** `ObservationService` handles `HSE_TGRJCTRSN` + `RJCTRSN_BTN_OK` — if `getPendingRejectCAR()` is set, runs **`handleRejectReasonOkForCAR`** ([carCustomButtons.js](hse/src/utils/carCustomButtons.js)); otherwise observation pending reject logic.

---

## 2. Screen tag registry

Imports in [screenHandlers/index.js](hse/src/screenHandlers/index.js):

| Export key | Handler file |
|------------|--------------|
| `HSE_TgCrEntry` | `Performance/Action Tracking/CAR_Entry.js` |
| `HSE_TgCrRvw` | `CAR_Review.js` |
| `HSE_TgCrAprvl` | `CAR_Approval.js` |
| `HSE_TgActnsEntry` | `Actions_Entry.js` |
| `HSE_TgActnsRviw` | `Actions_Review.js` |
| `HSE_TgActnsRcvd` | `Actions_Received.js` |
| `HSE_TgActnsUndrExec` | `Actions_Under_Execution.js` |
| `HSE_TgCrFloUp` | `CAR_Follow_Up.js` |
| `HSE_TgCrEdtng` | `CAR_Editing.js` |
| `HSE_TgCarmodelBas` | `Setup/CAR/CAR_Basis.js` |

---

## 3. Central CAR logic

| Concern | File | Symbols |
|---------|------|---------|
| All custom buttons (except fallback) | [carCustomButtons.js](hse/src/utils/carCustomButtons.js) | `handleCarButton`, `handleRejectReasonOkForCAR`, `setPendingRejectCAR`, `openCarReviewInfo`, `openCarApproveInfo`, `openCorrectiveActionsPopup`, `handleViewSourceTxn`, `handleEntryCompletedCarEntry`, `handleEntryCompletedActionsEntry`, `handleCloseCar`, `handleAcceptCarReview`, `handleApproveCarApproval` |
| Reject reason screen OK/Cancel | [ObservationService.js](hse/src/services/Observation%20service/ObservationService.js) | `RJCTRSN_BTN_OK`, `RJCTRSN_BTN_CANCEL`, `getPendingRejectCAR` |
| Tab NEW serials | [tabNewSerialUtils.js](hse/src/utils/tabNewSerialUtils.js) | Used from `CAR_Entry.js`, `Actions_Under_Execution.js` |

---

## 4. `handleCarButton` coverage (by button)

| Button | Screens handled |
|--------|-----------------|
| `VIEW_SOURCE_TXN` | `HSE_TGCRENTRY`, `HSE_TGCRRVW`, `HSE_TGACTNSENTRY`, `HSE_TGACTNSRVIW`, `HSE_TGCRAPRVL` |
| `CRENTRY_VWRJCTRSNS` | `HSE_TGCRENTRY` |
| `ENTRY_COMPLETED` | `HSE_TGCRENTRY`, `HSE_TGACTNSENTRY` |
| `REJECT_CAR` | `HSE_TGCRRVW`, `HSE_TGCRAPRVL`, `HSE_TGACTNSENTRY`, `HSE_TGACTNSRVIW`, `HSE_TGCRFLOUP` |
| `VIEW_CAR_REVIEW_INFO` | `HSE_TGCRAPRVL`, `HSE_TGACTNSENTRY`, `HSE_TGCREDTNG` |
| `VIEW_CAR_APPROVE_INFO` | `HSE_TGACTNSENTRY`, `HSE_TGCREDTNG` |
| `CORRECTIVE_ACTIONS` | See [CAR_Lifecycle_Process_UML.md](./CAR_Lifecycle_Process_UML.md) §4 |
| `CAR_REVIEW_INFO` | `HSE_TGCRRVW` |
| `CAR_APPROVAL_INFO` | `HSE_TGCRAPRVL` |
| `CANCEL_CAR` | `HSE_TGCRAPRVL` |
| `CLOSE_CAR` | `HSE_TGACTNSRVIW`, `HSE_TGCRFLOUP` |
| `ACCEPT_CAR` | `HSE_TGCRRVW`, `HSE_TGACTNSRVIW` |
| `APPROVE_CAR` | `HSE_TGCRAPRVL` |

Any other custom button name on these screens falls through to **`sendButtonClickToBackend`**.

---

## 5. Toolbar extras

| Screen | `toolBarButtonClicked` |
|--------|-------------------------|
| CAR Entry | `NEW` on configured tabs → `setNextSerialOnNewTab` for takings / root causes lines |
| Actions Under Execution | `NEW` on `HSE_TGCRCTVEACCUNDREXCTNCRPRCSS` → serial on `HSE_CRCTVEACCENTRQDACTN` |

---

## 6. Open questions

1. Server-side mapping of **`sendButtonClickToBackend`** for CAR/Actions screens — document endpoint and button→SP matrix in server repo if not already traced.
2. **`HSE_TgActnsRcvd`** — `handleCarButton` returns false for most buttons; confirm which actions are 100% server-driven in UAT.

---

## 7. Document control

| Version | Date | Notes |
|---------|------|--------|
| 0.1 | 2026-03-28 | Initial map from source |
