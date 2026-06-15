# CAR — Desktop behaviour specification

**Status:** Draft — *Under review → Approved (date, reviewer)*  
**Inputs:** [HSEMS_Use_Cases_From_Desktop_Code.md](./HSEMS_Use_Cases_From_Desktop_Code.md) §4.6, §10.4, §11 appendix  
**Next document:** [CAR_Lifecycle_Process_UML.md](./CAR_Lifecycle_Process_UML.md) (after approval)

---

## 1. Purpose

Single source of truth for **expected desktop (HSEMS-Win)** behaviour of the **Corrective Action Request (CAR)** and **Action Tracking** lifecycle, before comparing to the web `hse` module.

---

## 2. Screen inventory (desktop names → web tags)

| Desktop / menu concept | Web screen tag (handler registry) | Primary C++ reference (from web comments) |
|------------------------|-----------------------------------|-------------------------------------------|
| CAR Entry | `HSE_TgCrEntry` | `NewCarEntryCategory` |
| CAR Review | `HSE_TgCrRvw` | `NewCarEntryCategory` |
| CAR Approval | `HSE_TgCrAprvl` | `NewCarEntryCategory` |
| Actions Entry | `HSE_TgActnsEntry` | `NewCarEntryCategory` |
| Actions Review | `HSE_TgActnsRviw` | `NewCarEntryCategory` |
| Actions Received | `HSE_TgActnsRcvd` | CAR / Action Tracking |
| Actions Under Execution | `HSE_TgActnsUndrExec` | CAR / Action Tracking |
| CAR Follow-up | `HSE_TgCrFloUp` | `NewCarEntryCategory` |
| CAR Editing | `HSE_TgCrEdtng` | `NewCarEntryCategory` |
| Reject reason | `HSE_TGRJCTRSN` | `CRejectReason` |
| CAR Review Info popup | `HSE_CrRvwInf` | — |
| CAR Approval Info popup | `HSE_CrApprvlInf` | — |
| Corrective actions popups | Per-screen (see web map) | — |

---

## 3. Use cases vs backend operations (from product use-case doc)

| # | Use case | Typical screen | Named operation (desktop / DB catalogue) |
|---|----------|----------------|-------------------------------------------|
| U1 | Enter CAR | CAR Entry | (data entry; may tie to `sp_Generate_CARs` when auto-generated — §10.4) |
| U2 | Complete CAR (submit from entry) | CAR Entry | `completeCARTXN` (catalogue §11) |
| U3 | Review CAR | CAR Review | Review workflow |
| U4 | Accept CAR (review stage) | CAR Review | `setCARAcptd` (catalogue) |
| U5 | Approve CAR | CAR Approval | `confirmCARTXN` (catalogue) |
| U6 | Reject CAR | Multiple | `rejectCARTXN`, `setCARNotAcptd` |
| U7 | Cancel CAR | CAR Approval | `cancelCAR` |
| U8 | Close CAR | Follow-up / Actions Review | `closeCARTXN` |
| U9 | Corrective actions — define | Actions Entry | — |
| U10 | Corrective actions — review / receive / execute | Actions Review, Received, Under Execution | `AcceptCARExe`, `RejectCARExe`, `CARJobCmpltd` |
| U11 | Follow up CAR | CAR Follow-up | — |
| U12 | Edit CAR | CAR Editing | — |
| U13 | View source transaction | Entry, Review, Actions Entry, Actions Review, Approval | Navigate to observation / audit |
| U14 | View reject reasons | CAR Entry | Locked reject reason list |
| U15 | View / capture review & approval info | Review, Approval, later screens | Policy-gated info tabs |

**Cross-cutting:** `ChangeEntityStatus` (tracing / status transitions) — listed in use-case appendix.

---

## 4. Policy and integration (desktop)

- **HSE Policy** can enable **CAR Review Information** and **CAR Approval Information** for CARs whose source matches **CAR basis** (`HSE_HSEPLC_ADT`: `HSEPLC_ADT_CRBSS`, `HSEPLC_ADT_ENBCRRVWINF`, `HSEPLC_ADT_ENBCRAPPINF`).
- **Auto-generated CAR** from closed observations / site surveys / policy — §10.4.

---

## 5. Open questions (resolve before “Approved”)

1. Confirm in **HSEMS-Win** whether `completeCARTXN` / `confirmCARTXN` / `closeCARTXN` are always invoked for the same user actions as web’s `ENTRY_COMPLETED`, `APPROVE_CAR`, `CLOSE_CAR`, or whether web intentionally uses direct SQL + `ChangeEntityStatus` as a parity implementation.
2. Map **Actions Received** and **Actions Under Execution** custom buttons to exact **SP names** on desktop (web defers unhandled buttons via `sendButtonClickToBackend`).
3. **Corrective Actions Inquiry** filter buttons — parity with desktop `ChangeCriteria` (see §5 inquiry table in main use-case doc).

---

## 6. Document control

| Version | Date | Notes |
|---------|------|--------|
| 0.1 | 2026-03-28 | Initial extract from `HSEMS_Use_Cases_From_Desktop_Code.md` + web code comments; C++ file-level confirmation pending |
