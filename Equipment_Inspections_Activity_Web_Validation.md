# Equipment Inspection Activity Diagram — Web Implementation Validation

<!-- RQ_HSE_23_3_26_23_15 -->

> **Reference diagram**: [`Equipment_Inspections_Process_UML.md`](./Equipment_Inspections_Process_UML.md) — §2 *Activity diagram -- Equipment inspection (entry to cycle end)*  
> **Web codebase**: `HSEMS_WEB_Original/hse` (`buttonEvents.js`, `ModuleButtonHandlers/index.js`, `Observation service/ObservationService.js`, EQ Inspection screen handlers)  
> **Date**: March 2026

### Executive summary (vs §2 diagram)

**Post–RQ_HSE_23_3_26_23_15**: Every **§2 activity-diagram** step has a corresponding web behaviour: **`handleEQInspectionModuleButtons`** runs the named stored procedures; **`EQ_Inspection_Request`** runs SAVE duplicate check and **NEW** inspection-number assignment; **`eqInspectionSubFieldChanged.js`** covers checklist warning, checklist accepted flags, expected exit date, and defect line serial (any field change when line empty, per desktop); **`eqInspectionDialogs.js`** provides **OK-only** information dialogs and a robust **Yes/No** confirm for reload checklist.

**Not part of §2 diagram** (but implemented): Inquiry **`EQINSPCTN_VW*`** / **`EQINSPCTN_VWINSPCTNRQSTHSTRY`** — see §3 and [`eqInspectionButtonHandlers.js`](hse/src/services/ModuleButtonHandlers/eqInspectionButtonHandlers.js).

**Deployment UAT** — see **§4** (inquiry screen tag must match **`HSE_EQInspctnInqry`**).

---

## 1. How button routing works for EQ Inspection

<!-- RQ_HSE_23_3_26_23_15 -->

1. **`ButtonClicked`** ([`hse/src/events/buttonEvents.js`](hse/src/events/buttonEvents.js)) runs **`handleModuleButton`** first.
2. **`handleEQInspectionButton`** delegates to **`handleEQInspectionModuleButtons`** in [`eqInspectionButtonHandlers.js`](hse/src/services/ModuleButtonHandlers/eqInspectionButtonHandlers.js).
3. EQ Request / Result / In-Site handlers **do not** use **`sendButtonClickToBackend`** for `EQINSPCTN_*` / `INSPCTNCHCKLST_*` (those actions are satisfied in step 2).

---

## 2. Activity diagram §2 — node-by-node validation

<!-- RQ_HSE_23_3_26_23_15 -->

Legend: **Yes** = behaviour is supported by web `hse` client in line with the diagram (**RQ_HSE_23_3_26_23_15** implementation).

| Diagram ref | Text on diagram | Web status | Evidence / notes |
|-------------|-----------------|------------|------------------|
| **Start** | Open EQ Inspection Request `HSE_EQInspctnRqust` | **Yes** | Shell + registry in [`screenHandlers/index.js`](hse/src/screenHandlers/index.js). |
| **R1** | NEW / edit header `HSE_EQINSPCTN` | **Yes** | Generic CRUD + [`EQ_Inspection_Request.js`](hse/src/screenHandlers/Performance/EQ%20Inspection/EQ_Inspection_Request.js) **`toolBarButtonClicked`**: **NEW** `complete===1` → next **`EQINSPCTN_INSPCTNN`** for **`EQINSPCTN_INSPCTNYR`** (SQL as desktop `getCurrentEQInspctnNo`). |
| **R2** | Select `EQINSPCTN_EQTYP` (checklist must exist) | **Yes** | [`eqInspectionSubFieldChanged.js`](hse/src/screenHandlers/Performance/EQ%20Inspection/eqInspectionSubFieldChanged.js) — count on `HSE_EQTYPS_INSPCTNCHCKLST`; message if zero. |
| **R3** | Fill checklist / timing / attachments | **Yes** | Sub-forms from app JSON. |
| **R4** | SAVE | **Yes** | Standard toolbar. |
| **R4a** | Duplicate open inspection (type+serial+plate) | **Yes** | [`EQ_Inspection_Request.js`](hse/src/screenHandlers/Performance/EQ%20Inspection/EQ_Inspection_Request.js) — `toolBarButtonClicked`, SAVE `complete===0`, same SQL as desktop. |
| **R5–R6** | `EQINSPCTN_DATAENTRYRJCTD` → `rjctEQInspctnDataEntry` | **Yes** | [`eqInspectionButtonHandlers.js`](hse/src/services/ModuleButtonHandlers/eqInspectionButtonHandlers.js). |
| **R7–R8** | `INSPCTNCHCKLST_RELOADCHCKLST` → confirm → `reldEQInspctnChckLst` | **Yes** | [`confirmEqYesNo`](hse/src/utils/eqInspectionDialogs.js) then `reldEQInspctnChckLst` with desktop reason text. |
| **R9–R11** | `EQINSPCTN_ENTRYCMPLTD` → `completeEQInspctn` | **Yes** | `getEQInspectionPrimaryKey` + “save first” if empty; `completeEQInspctn`. |
| **Q1–Q2** | Open Result `HSE_EQInspctnRsult`; record outcome | **Yes** | Screen + edit UI. |
| **Q3 → Q4** | Hold → `holdEQInspctn` | **Yes** | Module handler. |
| **Q3 → Q5** | Reject inspection → `rjctEQInspctn` | **Yes** | Module handler. |
| **Q3 → Q6** | Reload checklist on Result | **Yes** | Same as R7–R8. |
| **Q3 → Q7–Q8** | Accept → `AcceptEQInspctn` | **Yes** | Module handler. |
| **EndRejected** | Cycle end rejected | **Yes** | SP from Result screen. |
| **W1–W2** | Open In-Site `HSE_EQInspctnWrk`; defects / monitor | **Yes** | Screen + sub-forms; `SubFieldChanged` assigns defect line serial on any field change when line empty (desktop parity). |
| **W3 → stop/resume** | `stopDefectedEQ` / `resumeEQWrk` | **Yes** | Module handler. |
| **W3 → exit defective** | `exitDefectedEQ` | **Yes** | Module handler. |
| **W3 → cancel** | `canclEQWrk` | **Yes** | Module handler. |
| **W3 → complete** | `cmpltEQWrk` | **Yes** | Module handler. |
| **EndExitDef / EndCancelled / EndCompleted** | Terminal outcomes | **Yes** | Driven by SPs above. |

---

## 3. Desktop parity not shown as separate §2 nodes

<!-- RQ_HSE_23_3_26_23_15 -->

| Topic | Desktop (`EQInspctnCategory.cpp`) | Web (`hse`) |
|--------|-----------------------------------|-------------|
| **Expected exit date** from entry date + days + hours | `DisplayFieldChange` | **Yes** — [`eqInspectionSubFieldChanged.js`](hse/src/screenHandlers/Performance/EQ%20Inspection/eqInspectionSubFieldChanged.js) |
| **Checklist line** defect/action → accepted flag | `DisplayFieldChange` | **Yes** — same file |
| **Defect-during-work line number** | `getCurrentEQInspctnDfctNo` on field change | **Yes** — same file (any field change when line empty; desktop parity) |
| **Inquiry quick filters / history** (`EQINSPCTN_VW*`, `VWINSPCTNRQSTHSTRY`) | `ChangeCriteria` / `viewInspctnRqstHstry` | **Yes** — [`eqInspectionButtonHandlers.js`](hse/src/services/ModuleButtonHandlers/eqInspectionButtonHandlers.js); tag **`HSE_EQInspctnInqry`** |

---

## 4. §2 validation — what is missing or environment-dependent

<!-- RQ_HSE_23_3_26_23_15 -->

**Nothing material is missing** from the **§2 flowchart** relative to the current `hse` code paths above: each labelled button or SP in the diagram is implemented.

**Desktop parity refinements (same § / RQ):**

| Topic | Web behaviour |
|-------|----------------|
| **Informational messages (R2, R4a, save-first)** | [`eqInspectionDialogs.js`](hse/src/utils/eqInspectionDialogs.js) **`showEqInformationDialog`** — **OK-only** via `openCustomDlg` (desktop `AfxMessageBox` information), not a mis-titled Yes/No dialog. |
| **Reload checklist confirm** | **`confirmEqYesNo`** wraps `AskYesNoMessage('Prompt', msg)` and supports both **Promise** and synchronous return values. |
| **Monthly VW_* filters** | `MONTH(EQINSPCTN_EQENTRYDT)` aligned with APP JSON **`DataFieldName`** for entry date. |
| **Defect line serial** | Assigned on **any** field change when the defect line number is empty and header **MAINLINK** exists — matches desktop `DisplayFieldChange` (tab filter was removed). |

**UAT (environment):**

| Topic | Note |
|-------|------|
| **Inquiry tag** | **`ChangeCriteria`** / **`openScr`** use **`HSE_EQInspctnInqry`**; must match the list screen tag in your shell. |
| **R2 / R4a** | Still **non-blocking** (inform only), same as desktop. |

---

## 5. Implementation record (RQ_HSE_23_3_26_23_15)

<!-- RQ_HSE_23_3_26_23_15 -->

| Item | Location |
|------|----------|
| All EQ custom buttons + inquiry filters/history | [`eqInspectionButtonHandlers.js`](hse/src/services/ModuleButtonHandlers/eqInspectionButtonHandlers.js); [`eqInspectionDialogs.js`](hse/src/utils/eqInspectionDialogs.js) — info + confirm helpers |
| Module wiring | [`ModuleButtonHandlers/index.js`](hse/src/services/ModuleButtonHandlers/index.js) — `handleEQInspectionButton` |
| Primary key resolution | [`moduleButtonHandlersUtils.js`](hse/src/services/ModuleButtonHandlers/moduleButtonHandlersUtils.js) — `getEQInspectionPrimaryKey` |
| SAVE duplicate warning; NEW → next inspection no. | [`EQ_Inspection_Request.js`](hse/src/screenHandlers/Performance/EQ%20Inspection/EQ_Inspection_Request.js) — `toolBarButtonClicked` |
| Field-change parity | [`eqInspectionSubFieldChanged.js`](hse/src/screenHandlers/Performance/EQ%20Inspection/eqInspectionSubFieldChanged.js); exported from Request / Result / In-Site handlers |
| Screen captions (tracing) | [`moduleButtonHandlersUtils.js`](hse/src/services/ModuleButtonHandlers/moduleButtonHandlersUtils.js) — `getScreenCaption` EQ tags |
| Dispatch order | [`buttonEvents.js`](hse/src/events/buttonEvents.js) — `handleModuleButton` first |

---

## 6. Historical note

<!-- RQ_HSE_23_3_26_23_15 -->

Before **RQ_HSE_23_3_26_23_15**, EQ handlers called **`sendButtonClickToBackend`**, which **no-opped** for non-observation screens. That path is **removed** for EQ inspection buttons.

---

*End of document*
