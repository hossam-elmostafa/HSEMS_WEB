# CAR — Implementation log

**Status:** Draft  
**Related:** [CAR_Activity_Web_Validation.md](./CAR_Activity_Web_Validation.md) §7

---

Use this file for **code change history** when implementing items from the validation **Gap backlog** (G1–G4), if you prefer not to inline long entries in the validation doc.

| Date | ID | Summary | Files | Reviewer |
|------|-----|---------|-------|----------|
| 2026-03-28 | — | Plan applied: documentation deliverables only; no implementation entries yet. | — | — |
| 2026-03-28 | RQ_HSE_30_3_26_14_57 | Named SP execution for Creative Access (`completeCARTXN`), Actions Received (`AcceptCARExe`), Under Execution (`CARJobCmpltd`, `CARJobPndng`); reject paths deferred; VIEW_SOURCE extended for additional `CRENTRY_CRSRC` mappings. | [carFoundationTxnButtons.js](hse/src/utils/carFoundationTxnButtons.js), [Creative_Access_Entry.js](hse/src/screenHandlers/Performance/Action%20Tracking/Creative_Access_Entry.js), [index.js](hse/src/screenHandlers/index.js), [Actions_Received.js](hse/src/screenHandlers/Performance/Action%20Tracking/Actions_Received.js), [Actions_Under_Execution.js](hse/src/screenHandlers/Performance/Action%20Tracking/Actions_Under_Execution.js), [carCustomButtons.js](hse/src/utils/carCustomButtons.js) | — |
| 2026-03-31 | car_save_temp_hse_table_72a50b1e | Policy: no WebInfra Client/Server edits for this item. Delivered SSMS discovery + T-SQL guard template for `##TEMP_HSE_TABLE` (DBA applies to live triggers). Reliable web save fix remains database-side or HSE-layer only. | [HSE_TEMP_HSE_TABLE_trigger_hardening.sql](sql/HSE_TEMP_HSE_TABLE_trigger_hardening.sql) | — |
| 2026-03-31 | CAR_Entry_SAVE_desktop_parity | Web HSE: `NewCarEntryCategory` SAVE parity — pre-save batch seeds `##TEMP_HSE_TABLE` + `UpdateYearAndSerial` when `CRENTRY_CRYR` empty; post-save sets `CRENTRY_CRSTT='01'` for new records. App layer only. | [carEntrySaveDesktopParity.js](hse/src/utils/carEntrySaveDesktopParity.js), [CAR_Entry.js](hse/src/screenHandlers/Performance/Action%20Tracking/CAR_Entry.js) | — |
