# CAR / Action Tracking — client button → stored procedure map

**Tag:** RQ_HSE_30_3_26_14_57  
**Scope:** Client-side `EXECUTE` calls and explicit handlers in `hse` (not exhaustive for every toolbar action).

| Screen tag | Button (name) | Handler | Procedure / behaviour |
|------------|---------------|---------|----------------------|
| `HSE_TGCRCTVEACCENT` / `HSE_TgcrctveAccEnt` | `CRCTVEACCENT_ENTCMPLT` | [Creative_Access_Entry.js](hse/src/screenHandlers/Performance/Action%20Tracking/Creative_Access_Entry.js) → [handleCompleteCorrectiveActionEntry](hse/src/utils/carFoundationTxnButtons.js) | `EXECUTE completeCARTXN` (3 string args: prmky, caption, user) |
| `HSE_TGACTNSRCVD` | `CRCTVEACCENT_ACPTDEXECTN` | [handleCarFoundationTxnButtons](hse/src/utils/carFoundationTxnButtons.js) | `AcceptCARExe` |
| `HSE_TGACTNSRCVD` | `CRCTVEACCENT_RJCTEXECTN` | — | **Not implemented** (desktop: reject reasons → `rejectCARTXN` 5-arg) |
| `HSE_TGACTNSUNDREXEC` | `CRCTVEACCENT_JOBCMPLTD` | [handleCarFoundationTxnButtons](hse/src/utils/carFoundationTxnButtons.js) | `CARJobCmpltd` |
| `HSE_TGACTNSUNDREXEC` | `CRCTVEACCENT_JOBPNDNG` | same | `CARJobPndng` |
| `HSE_TGACTNSUNDREXEC` | `CRCTVEACCENT_RJCT_UNDREXCTN` | — | **Not implemented** (same reject flow as above) |

**Wiring:** [Actions_Received.js](hse/src/screenHandlers/Performance/Action%20Tracking/Actions_Received.js) and [Actions_Under_Execution.js](hse/src/screenHandlers/Performance/Action%20Tracking/Actions_Under_Execution.js) call `handleCarFoundationTxnButtons` before `handleCarButton`.

**VIEW_SOURCE_TXN:** [handleViewSourceTxn](hse/src/utils/carCustomButtons.js) — inquiry tags and criteria by `CRENTRY_CRSRC` (see comment in file).
