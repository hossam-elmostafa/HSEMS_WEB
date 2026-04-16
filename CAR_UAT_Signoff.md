# CAR — UAT & sign-off (desktop vs web)

**Status:** Draft — *Complete after paired walkthrough*  
**Inputs:** [CAR_Activity_Web_Validation.md](./CAR_Activity_Web_Validation.md) (approved), approved desktop spec

---

## 1. Environment

| Field | Desktop | Web |
|-------|---------|-----|
| Build / version | | |
| Database | | |
| Tester | | |
| Date | | |

---

## 2. Scripted scenarios (check when passed)

### 2.1 CAR Entry

- [ ] Create CAR (manual)
- [ ] `ENTRY_COMPLETED` — status 05, tracing row, save
- [ ] `VIEW_SOURCE_TXN` — observation-linked CAR opens inquiry criteria
- [ ] `VIEW_SOURCE_TXN` — audit-linked CAR opens audit inquiry
- [ ] `CRENTRY_VWRJCTRSNS` — reject reason screen opens with correct criteria
- [ ] Tab NEW — line serials for configured tabs

### 2.2 CAR Review

- [ ] `CAR_REVIEW_INFO` popup with link type `CRCTVEACCENT-RV`
- [ ] `ACCEPT_CAR` — policy gate when review info required; status 10
- [ ] `REJECT_CAR` — reject reason → status 02

### 2.3 CAR Approval

- [ ] `CAR_APPROVAL_INFO`
- [ ] `APPROVE_CAR` — policy gate; status 15; dept/project copy from approval info
- [ ] `REJECT_CAR` — status 06
- [ ] `CANCEL_CAR` — status 20 after reject reason OK

### 2.4 Actions Entry

- [ ] `ENTRY_COMPLETED` — root cause / corrections validations
- [ ] `VIEW_CAR_REVIEW_INFO` / `VIEW_CAR_APPROVE_INFO`
- [ ] `CORRECTIVE_ACTIONS` popup
- [ ] `REJECT_CAR` — status 11 after reject flow

### 2.5 Actions Review

- [ ] `ACCEPT_CAR` — corrections/actions checks; status 19; target date update SQL
- [ ] `CLOSE_CAR` — acceptance checks; closed date; status 25
- [ ] `REJECT_CAR` — status 12

### 2.6 Actions Received / Under Execution

- [ ] Buttons exercised per **test plan** (server-driven paths)
- [ ] Under Execution — tab NEW serial

### 2.7 CAR Follow-up / Editing

- [ ] Follow-up: `CLOSE_CAR`, `REJECT_CAR`, corrective actions
- [ ] Editing: view info + corrective actions

### 2.8 Cross-cutting

- [ ] Reject reason **Cancel** clears pending CAR context
- [ ] Desktop same record: same final `CRENTRY_CRSTT` and tracing rows after each step (where applicable)

---

## 3. Sign-off

| Role | Name | Signature / date | Comments |
|------|------|------------------|----------|
| Technical reviewer | | | |
| Product / module owner | | | |

---

## 4. Document control

| Version | Date | Notes |
|---------|------|--------|
| 0.1 | 2026-03-28 | Checklist template |
