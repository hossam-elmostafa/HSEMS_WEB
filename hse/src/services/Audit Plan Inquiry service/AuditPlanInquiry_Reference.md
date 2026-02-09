# Audit Plan Inquiry – Requirements & Source Reference

**Task Type:** Main Screen  
**TableName (TXNTag):** `HSE_TgAdtPlnInq`  
**TrueTableName:** `HSE_AdtPlnEnt`

## Requirement IDs → Custom Buttons

| Req ID | Button Label | Button Key (C++) | Status / Behavior |
|--------|--------------|------------------|-------------------|
| RQ_HSM_27_01_26_23_00.06 | Audit Plan Inquiry Custom Buttons | (group) | Main screen custom buttons |
| RQ_HSM_27_01_26_23_00.07 | Activate | *(not in AuditInquiry.cpp)* | May be on entry/approval screen |
| RQ_HSM_27_01_26_23_00.08 | View All | `ADTPLNENT_VWALL` | No filter; show all records |
| RQ_HSM_27_01_26_23_01.01 | View Approved | `ADTPLNENT_VWAPPRV` | `ADTPLNENT_ADTPLNSTS = 5` |
| RQ_HSM_27_01_26_23_01.02 | View Closed | `ADTPLNENT_VWCLSD` | `ADTPLNENT_ADTPLNSTS = 6` |
| RQ_HSM_27_01_26_23_01.03 | View Completed | `ADTPLNENT_VWCMPLT` | `ADTPLNENT_ADTPLNSTS = 4` |
| RQ_HSM_27_01_26_23_01.04 | View Incomplete | `ADTPLNENT_VWINCMPLT` | `ADTPLNENT_ADTPLNSTS = 1` |
| RQ_HSM_27_01_26_23_01.05 | View Rejected | `ADTPLNENT_VWRJCT` | `ADTPLNENT_ADTPLNSTS = 3` |

## Audit Plan Status Codes (ADTPLNENT_ADTPLNSTS)

| Code | Meaning |
|------|--------|
| 1 | Incomplete |
| 3 | Rejected |
| 4 | Completed |
| 5 | Approved |
| 6 | Closed |

## Source Code References

### TableName / TrueTableName

- **CARCategory.cpp** (lines 188–190) – used when opening Audit Plan Inquiry from CAR (Audit basis):
  - `strTXNTag = "HSE_TgAdtPlnInq";`
  - `strTableName = "HSE_AdtPlnEnt";`
  - `strTXNKeyField = "MAINLINK";`

### Custom Button Handlers (Inquiry Main Screen)

- **AuditInquiry.cpp** – `DisplayCustomButtonClicked`:
  - Form tag used in `ChangeCriteria`: `"HSE_TGADTPLNINQ"` (uppercase in C++).
  - Filter buttons: `ADTPLNENT_VWINCMPLT`, `ADTPLNENT_VWRJCT`, `ADTPLNENT_VWCMPLT`, `ADTPLNENT_VWAPPRV`, `ADTPLNENT_VWCLSD`, `ADTPLNENT_VWALL`.
  - Other buttons: `AUDITORS`, `AUDITIES`, `ADTPLNENTACTVT_ADTCHKLSTFDBCK` (open other screens).

### C++ Snippet (AuditInquiry.cpp)

```cpp
ChangeCriteria("HSE_TGADTPLNINQ","","WHERE (ADTPLNENT_ADTPLNSTS=1)");  // View Incomplete
ChangeCriteria("HSE_TGADTPLNINQ","","WHERE (ADTPLNENT_ADTPLNSTS=3)");  // View Rejected
ChangeCriteria("HSE_TGADTPLNINQ","","WHERE (ADTPLNENT_ADTPLNSTS=4)");  // View Completed
ChangeCriteria("HSE_TGADTPLNINQ","","WHERE (ADTPLNENT_ADTPLNSTS=5)");  // View Approved
ChangeCriteria("HSE_TGADTPLNINQ","","WHERE (ADTPLNENT_ADTPLNSTS=6)");  // View Closed
ChangeCriteria("HSE_TGADTPLNINQ","","");                                  // View All
RefreshScreen("",0);
```

## Web Implementation

- **AuditPlanInquiryService.js** – routes custom button clicks for Audit Plan Inquiry screen.
- **AuditPlanInquiryButtonHandlers.js** – implements filter actions via `openScr` + criteria and `refreshData`, matching C++ `ChangeCriteria` + `RefreshScreen`.

Screen tag matching: `HSE_TgAdtPlnInq` or `HSE_TGADTPLNINQ` (case-insensitive).
