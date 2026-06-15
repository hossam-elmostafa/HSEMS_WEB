# HSE Desktop (C++) – Screens/Tabs Where Serial Number Is Auto-Calculated

This report lists all places in the **old C++ HSE** code where a **serial/line number field is automatically calculated** when the user clicks **NEW** (on the main form or on a tab). Source: `getNxtSrl`, `getNewSerialNoForATab`, and equivalent logic in `HSEMSCommonCategory.cpp`, `RscuPlnCategory.cpp`, `SitSrvyCategory.cpp`, `NewCarEntryCategory.cpp`, `EnvMntrExctnCategory.cpp`, `EnvMntrPlanCategory.cpp`, plus Audit/Equipment/other modules.

---

## Summary table

| # | Screen (form) | Tab | Serial field | C++ location |
|---|----------------|-----|---------------|--------------|
| 1 | **Awareness Course** | Course Material | AWRNSCRSCRSMTRL_LNNO | HSEMSCommonCategory.cpp |
| 2 | **Awareness Course** | Instructors | AWRNSCRSPINSTRCTRS_SRLNO | HSEMSCommonCategory.cpp |
| 3 | **Awareness Plan (Course Attendees)** | — | AWRNSPLN_CRS_ATNDS_SRLNO | HSEMSCommonCategory.cpp |
| 4 | **Accident Entry** | Accident Reasons | ACDNTENTACDNTRSN_LNRNUM | HSEMSCommonCategory.cpp |
| 5 | **Accident Entry** | Taken/Not Taken Model Actions | ACDNTENTTKNRMDLACTNS_LNRNUM | HSEMSCommonCategory.cpp |
| 6 | **Accident Entry** | Recommended Actions | ACDNTENTRECMNDACC_ACTNNO | HSEMSCommonCategory.cpp |
| 7 | **Accident Entry** | Attachments | ACDNTENTATCH_SRLNO | HSEMSCommonCategory.cpp |
| 8 | **Observation Entry** | Possible Near Miss Causes | NRSTMISCENTPSSBLNRSTMISCCUSE_LNRNUM | HSEMSCommonCategory.cpp |
| 9 | **Observation Approval** | Possible Near Miss Causes | NRSTMISCENTPSSBLNRSTMISCCUSE_LNRNUM | HSEMSCommonCategory.cpp |
| 10 | **Observation Entry / Review / Approval** | Required Actions | NRSTMISCENTRQDACTN_ACTNNO | HSEMSCommonCategory.cpp |
| 11 | **Observation Entry / Review / Approval** | Attachments | NRSTMISCENTATCH_SRLNO | HSEMSCommonCategory.cpp |
| 12 | **Audit Plan Entry** | Activities | ADTPLNENTACTVT_ACTVNUM | HSEMSCommonCategory.cpp |
| 13 | **Drill Findings** | Recommended Actions | DRLFNDNGSRCMNDACTN_ACTNNO | HSEMSCommonCategory.cpp |
| 14 | **Drill Findings (Unplanned Scenarios)** | Recommended Actions | DRLFNDNGSUNPLNDSRCMNDACTN_ACTNNO | HSEMSCommonCategory.cpp |
| 15 | **CAR Entry (Corrective Action)** | Taken Remedial Actions | CRCTVEACCENTTAKRMDALACC_LNRNUM | HSEMSCommonCategory.cpp |
| 16 | **CAR Entry** | Root Cause | CRCTVEACCENTROOTCUSE_LNRNUM | HSEMSCommonCategory.cpp |
| 17 | **Drill Plan** | Unplanned Scenarios | DRLLPLN_UNPLNDSCNROS_SCNRCD | HSEMSCommonCategory.cpp |
| 18 | **Audit Check List** (under Procedure List) | — | LINENUM (ADTCHKLST) | HSEMSCommonCategory.cpp |
| 19 | **Drill Plan** | Equipment / Materials | DRLLPLN_EMT_SRLN | HSEMSCommonCategory.cpp |
| 20 | **Drill Plan** | Emergency Response Team | DRLLPLN_ERT_SRLN | HSEMSCommonCategory.cpp |
| 21 | **Drill Plan** | Internal Resources | DRLLPLN_INTRNLRSURCS_SRLN | HSEMSCommonCategory.cpp |
| 22 | **Drill Plan** | External Support | DRLLPLN_EXTRNLSUPPRT_SRLN | HSEMSCommonCategory.cpp |
| 23 | **Drill Plan** | Execution Comments | DRLLPLN_EXCUTNCMMNTS_SRLN | HSEMSCommonCategory.cpp |
| 24 | **Drill Findings** | — (main) | DRLFNDNGS_SRLNO | HSEMSCommonCategory.cpp |
| 25 | **Drill Findings Unplanned Scenarios** | — (main) | DRLFNDNGSUNPLNDSCNROS_SRLNO | HSEMSCommonCategory.cpp |
| 26 | **Drill Plan Entry** | — (main) | DRLLPLN_DRLLN | HSEMSCommonCategory.cpp |
| 27 | **Aspects** | Aspects | ASPCTS_ASPCTS_SRLNO | HSEMSCommonCategory.cpp |
| 28 | **Environment Monitor Required Actions** | — | ENVMNTRRQRDACTN_ACTNNO | HSEMSCommonCategory.cpp |
| 29 | **Audit Required Actions** | — | ADTACTVTRQRDACTN_ACTNNO | HSEMSCommonCategory.cpp |
| 30 | **CAR Under Execution** (Process tab) | Required Actions | CRCTVEACCENTRQDACTN_LNRNO | HSEMSCommonCategory.cpp |
| 31 | **Audit Check List Plan** | — | ADTCHCKLSTPLN_TSKNUM | HSEMSCommonCategory.cpp |
| 32 | **PTW Register Entry** | Safety Instructions | PTWSRGSTRSFTYINSTRCTNS_SRLNO | HSEMSCommonCategory.cpp |
| 33 | **Rescue Plan Entry** | — (main) | RSCUPLN_PLNN | RscuPlnCategory.cpp |
| 34 | **Rescue Plan Entry** | Potential Hazards | RSCUPLN_PTNTLHZRDS_SRLN | RscuPlnCategory.cpp |
| 35 | **Rescue Plan Entry** | Attachments | RSCUPLN_ATTCHMNTS_LNN | RscuPlnCategory.cpp |
| 36 | **Rescue Plan Entry** | Required Details | RSCUPLN_RQURDTLS_SRLN | RscuPlnCategory.cpp |
| 37 | **Rescue Plan Entry** | Communication Methods | RSCUPLN_CMMUNCTNMTHDS_SRLN | RscuPlnCategory.cpp |
| 38 | **Site Survey Entry** | Findings | SITSRVYENTFNDNG_SRIL | SitSrvyCategory.cpp |
| 39 | **Site Survey Entry** | Required Actions | SITSRVYRQRDACTN_ACTNNO | SitSrvyCategory.cpp |
| 40 | **CAR Entry** | — (main, CAR Serial) | CRENTRY_CRSRLN | NewCarEntryCategory.cpp |
| 41 | **CAR Entry** | Corrective Action (tab) | ACTNSENTRY_Crr_Srl | NewCarEntryCategory.cpp |
| 42 | **CAR Entry** | Root Cause (tab) | ACTNSENTRY_RtCss_Srl | NewCarEntryCategory.cpp |
| 43 | **Environment Monitor Plan** | Activities | ENVMNTRPLANACTVTS_SRIL | EnvMntrPlanCategory.cpp |
| 44 | **Environment Monitor Execution** | Activities | ENVMNTRPLANACTVTS_SRIL | EnvMntrExctnCategory.cpp |

---

## By screen (grouped)

### Observation (Near Miss)

| Screen | Tab | Serial field |
|--------|-----|--------------|
| Observation Entry | Possible Near Miss Causes | NRSTMISCENTPSSBLNRSTMISCCUSE_LNRNUM |
| Observation Approval | Possible Near Miss Causes | NRSTMISCENTPSSBLNRSTMISCCUSE_LNRNUM |
| Observation Entry / Review / Approval | Required Actions | NRSTMISCENTRQDACTN_ACTNNO |
| Observation Entry / Review / Approval | Attachments | NRSTMISCENTATCH_SRLNO (Line No.) |

### Accident

| Screen | Tab | Serial field |
|--------|-----|--------------|
| Accident Entry | Accident Reasons | ACDNTENTACDNTRSN_LNRNUM |
| Accident Entry | Taken/Not Taken Model Actions | ACDNTENTTKNRMDLACTNS_LNRNUM |
| Accident Entry | Recommended Actions | ACDNTENTRECMNDACC_ACTNNO |
| Accident Entry | Attachments | ACDNTENTATCH_SRLNO |

### Awareness

| Screen | Tab | Serial field |
|--------|-----|--------------|
| Awareness Course | Course Material | AWRNSCRSCRSMTRL_LNNO |
| Awareness Course | Instructors | AWRNSCRSPINSTRCTRS_SRLNO |
| Awareness Plan (Course Attendees) | — | AWRNSPLN_CRS_ATNDS_SRLNO |

### Drill Plan / Drill Findings

| Screen | Tab | Serial field |
|--------|-----|--------------|
| Drill Plan | Unplanned Scenarios | DRLLPLN_UNPLNDSCNROS_SCNRCD |
| Drill Plan | Equipment/Materials | DRLLPLN_EMT_SRLN |
| Drill Plan | Emergency Response Team | DRLLPLN_ERT_SRLN |
| Drill Plan | Internal Resources | DRLLPLN_INTRNLRSURCS_SRLN |
| Drill Plan | External Support | DRLLPLN_EXTRNLSUPPRT_SRLN |
| Drill Plan | Execution Comments | DRLLPLN_EXCUTNCMMNTS_SRLN |
| Drill Plan Entry | — (main) | DRLLPLN_DRLLN |
| Drill Findings | — (main) | DRLFNDNGS_SRLNO |
| Drill Findings | Recommended Actions | DRLFNDNGSRCMNDACTN_ACTNNO |
| Drill Findings (Unplanned Scenarios) | — (main) | DRLFNDNGSUNPLNDSCNROS_SRLNO |
| Drill Findings (Unplanned Scenarios) | Recommended Actions | DRLFNDNGSUNPLNDSRCMNDACTN_ACTNNO |

### CAR (Corrective Action) / Actions

| Screen | Tab | Serial field |
|--------|-----|--------------|
| CAR Entry | — (main, CAR Serial) | CRENTRY_CRSRLN |
| CAR Entry | Corrective Action (tab) | ACTNSENTRY_Crr_Srl |
| CAR Entry | Root Cause (tab) | ACTNSENTRY_RtCss_Srl |
| CAR Entry | Taken Remedial Actions | CRCTVEACCENTTAKRMDALACC_LNRNUM |
| CAR Entry | Root Cause | CRCTVEACCENTROOTCUSE_LNRNUM |
| CAR Under Execution | Required Actions (Process tab) | CRCTVEACCENTRQDACTN_LNRNO |

### Audit

| Screen | Tab | Serial field |
|--------|-----|--------------|
| Audit Plan Entry | Activities | ADTPLNENTACTVT_ACTVNUM |
| Audit Check List (under Procedure List) | — | LINENUM |
| Audit Required Actions | — | ADTACTVTRQRDACTN_ACTNNO |
| Audit Check List Plan | — | ADTCHCKLSTPLN_TSKNUM |

### Environment Monitor

| Screen | Tab | Serial field |
|--------|-----|--------------|
| Environment Monitor Plan | Activities | ENVMNTRPLANACTVTS_SRIL |
| Environment Monitor Execution | Activities | ENVMNTRPLANACTVTS_SRIL |
| Environment Monitor Required Actions | — | ENVMNTRRQRDACTN_ACTNNO |

### Other (Rescue Plan, Site Survey, PTW, Aspects)

| Screen | Tab | Serial field |
|--------|-----|--------------|
| Rescue Plan Entry | — (main) | RSCUPLN_PLNN |
| Rescue Plan Entry | Potential Hazards | RSCUPLN_PTNTLHZRDS_SRLN |
| Rescue Plan Entry | Attachments | RSCUPLN_ATTCHMNTS_LNN |
| Rescue Plan Entry | Required Details | RSCUPLN_RQURDTLS_SRLN |
| Rescue Plan Entry | Communication Methods | RSCUPLN_CMMUNCTNMTHDS_SRLN |
| Site Survey Entry | Findings | SITSRVYENTFNDNG_SRIL |
| Site Survey Entry | Required Actions | SITSRVYRQRDACTN_ACTNNO |
| PTW Register Entry | Safety Instructions | PTWSRGSTRSFTYINSTRCTNS_SRLNO |
| Aspects | Aspects | ASPCTS_ASPCTS_SRLNO |

---

## C++ reference

- **Common handler:** `CHSEMSCommonCategory::DisplayToolBarButtonClicked` in `HSEMSCommonCategory.cpp` (NEW, complete=1): uses `strFormTag` / `strSubFormTag` and calls `getNxtSrl(...)` then `FormSetField(...)`.
- **getNxtSrl:** `CHSEMSCommonCategory::getNxtSrl` in `HSEMSCommonCategory.cpp` – `SELECT ISNULL(MAX(serialColumn)+1,1) FROM table [WHERE linkColumn=parentKey]`.
- **getNewSerialNoForATab:** `CHSEMSCommonCategory::getNewSerialNoForATab` – same idea for tab context.
- **Screen-specific:** `RscuPlnCategory`, `SitSrvyCategory`, `NewCarEntryCategory`, `EnvMntrPlanCategory`, `EnvMntrExctnCategory` implement their own NEW/serial logic and call `getNxtSrl` or `getNewSerialNoForATab`.

---

## Notes for web parity

- Observation **Attachments** and **Required Actions** are already covered in the web `TAB_NEW_SERIAL_CONFIG` for Entry, Review, and Approval.
- Other rows in this report are candidates for implementing the same auto-serial behaviour on the web (same pattern: NEW on tab → get next serial from DB → set field via FormSetField / tab state).
