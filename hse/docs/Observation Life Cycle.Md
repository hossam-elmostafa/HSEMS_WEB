1. Business lifecycle description
Plain-language process

Capture (Observation Entry)
Someone records an observation: where it happened, description, hazards, possible causes, recommended actions, attachments, signatures, etc. The record can stay incomplete while being edited.

Submit for workflow (Complete)
When ready, the user runs Complete. The system runs a transaction that advances the observation into the “completed / submitted” pool so HSE can work it (email hooks exist but the template send is commented out in code).

Review (Observation Review)
A reviewer accepts (confirm transaction) or rejects with reasons. Rejection sends the observation back for correction (status rejected in inquiry).

Approval / follow-up (Observation Approval)
An approver finalizes the case: Close runs a closing transaction. If Require CAR is Yes, the client builds a CAR (Corrective Action Request) row and copies images—linking CAR basis Observation (3) for downstream CAR screens.

Optional: Reward
A separate screen can mark a reward flag on the observation.

Cancel
From Review or Approval, Cancel (with confirmation) sets status cancelled and writes an audit/trace row.

Reporting / inquiry
Users filter lists by lifecycle status (incomplete, rejected, complete, confirmed, closed).

Stages and purpose

Stage (business)	Purpose
Incomplete
Draft / data capture
Complete (submitted)
Locked into formal HSE queue for review
Rejected
Returned to originator with reasons
Confirmed
Review accepted; ready for final approval step
Closed
Case finalized; CAR created if required
Cancelled (99)
Voluntarily stopped from Review/Approval
2. State machine / workflow
States (from NearMissInquiry.cpp filters on NRSTMISCENT_RECSTS)

Code	Inferred business name
1
Incomplete
3
Rejected
4
Complete / submitted (awaiting review)
5
Confirmed (post-review; awaiting approval/close)
6
Closed
99
Cancelled (set in UI, not exposed as an inquiry quick filter in this file)
Note: SetRecordStatus maps child line TXNSTS when saving under main status 2 or 3 to 1 for sub-rows—status 2 may exist in DB or legacy paths but has no dedicated inquiry button here. Assumption: 2 is an internal or legacy intermediate main status.

UML-style state diagram (text)

                    ┌─────────────┐
                    │ 1 Incomplete │
                    └──────┬──────┘
                           │ User: Save / edit (toolbar)
                           │ User: Complete (NRSTMISCENT_ENTCMPLT)
                           │ System: EXEC completeNearMissTXN
                           ▼
                    ┌─────────────┐
         ┌─────────│ 4 Complete   │─────────┐
         │         └──────┬──────┘         │
         │ reject         │ accept         │ (inquiry views)
         │ User: RJC      │ User: ACP      │
         │ EXEC           │ EXEC           │
         │ rejectObservation│ confirmNearMissTXN
         ▼                ▼
    ┌─────────┐     ┌─────────────┐
    │ 3 Reject │     │ 5 Confirmed  │
    └────┬────┘     └──────┬──────┘
         │                 │ User: Close (NRSTMISCENT_CLS)
         │                 │ EXEC closeNearMissTXN
         │                 │ [+ CAR insert if RQRCR=Y]
         │                 ▼
         │            ┌─────────┐
         │            │ 6 Closed │
         │            └─────────┘
         │
         └──► back to entry / correction (process)
From 4 or 5 (Review/Approval forms):
  User: Cancel (NRSTMISCENT_CNCL) → confirm → NRSTMISCENT_RECSTS = 99 + trace INSERT "Canceled"
Transitions and triggers

Transition	Trigger	Condition / notes
→ 4 (complete)
User Complete on Entry
Record must be saved (pMultiSelectedRows); completeNearMissTXN
→ 5 (confirm)
User Accept on Review
confirmNearMissTXN; if reject reasons exist, prompt to delete them first
→ 3 (reject)
User Reject on Review or Approval
Reject() UI + rejectObservation when update flag set; L1 vs L2 reject context by form
→ 6 (close)
User Close on Approval
closeNearMissTXN; login user must resolve to employee code
→ 99 (cancel)
User Cancel
Not already 99; trace insert; toolbar Save
CAR created
After successful close
NRSTMISCENT_RQRCR == "Y" → insert HSE_CRENTRY, temp table context, CopyImages
3. Sequence flows
3.1 Creating an observation
Open Entry (ShowNearMissEntry → ParentManage on HSE_TGNRSTMISCENT / HSE_vwNRSTMISCENT).
New on main form: optional default Concerned Department from user’s department (NearMissCategory.cpp).
New on Recommended Actions tab: default Require CAR on line items from policy HSE_HSEPLCCAR.HSEPLCCAR_DFLRQRCR (checkRequireCAR()).
User fills fields; Save may sync child TXNSTS with main NRSTMISCENT_RECSTS for certain tables (SetRecordStatus).
Tabs (possible causes, expected hazards, required actions, …) may be disabled if HSE_HSEOBSRVTN.HSEOBSRVTN_LCKENTRYTABS = 'YES' (isObservationTabsEnabled).
Optional: Get root causes / Get expected hazards → getRootCauses / getExpectedHazards procs + refresh.
3.2 Updating an observation
Same Entry (or return after rejection): edit fields; Save / Delete on subforms with toolbar rules (reject-reason duplicate check on save for certain tabs).
Observation Accepted (NRSTMISCENT_OBSACC): Y enables Require CAR (NRSTMISCENT_RQRCR); N locks Require CAR and forces value aligned to N (NearMissCategory::DisplayFieldChange).
Browsing filters restrict pickers (e.g. location hierarchy, owner-department employees for preview by).
3.3 Approval / rejection
Review (NearMissConfirmationCategory)

Save toolbar forced before custom actions.
Accept (NRSTMISCENT_ACP): check pending reject reasons → optional delete → confirmNearMissTXN → refresh.
Reject (NRSTMISCENT_RJC in base class): Reject("NRSTMISCENT-L1",…) + rejectObservation.
Approval (NearMissFollowUpCategory)

Save then Close (NRSTMISCENT_CLS): closeNearMissTXN.
If Require CAR = Yes: compute CAR fields from observation + policy HSE_HSEOBSRVTN (generated CAR status, CAR basis), insert HSE_CRENTRY, set session ##TEMP_HSE_TABLE (SrcScreen = Observation Approval), copy observation image to CAR via CopyImages, show CAR number message.
3.4 Closing an observation
Same as Approval Close path above. Assumption: closeNearMissTXN sets main status to 6 and tracing is handled in DB or framework (legacy CloseNearMiss with UpdateStatus(...,6) exists but is marked not used).

4. Data model extraction (logical)
Header (main observation) — view/table family HSE_vwNRSTMISCENT / NRSTMISCENT_*

Field / pattern	Role
NRSTMISCENT_NRSTMISCNUM
Surrogate key
NRSTMISCENT_RECSTS
Lifecycle status
NRSTMISCENT_NRMISTYP
Type (drives root cause / hazard procs)
NRSTMISCENT_NRSTMISCDESC
Description (synced to reporting description on complete)
NRSTMISCENT_SIT, LOCTYP, WRKLOC
Site / location hierarchy
NRSTMISCENT_CNCRNDDEP, PRJCT
Concerned department, project
NRSTMISCENT_YR, NRSTMISCENT_NO
Observation year/number (CAR linkage)
NRSTMISCENT_OBSACC
Observation accepted (Y/N) — gates Require CAR
NRSTMISCENT_RQRCR
Require CAR on header
NRSTMISCENT_SFTYOBSRV
Evidence text (mapped to CAR)
NRSTMISCENT_EMP / name fields
Employee vs non-employee reporter
NRSTMISCENT_ISUDAT
Issue date (drives target month STD from HSE_OBSRVTNTRGTSTRGTS)
NRSTMISCENT_TRGTMNTHSTD
Target derived when reporting department chosen
NRSTMISCENT_RWRDFLG
Reward flag (NearMissReward)
NRSTMISCENT_PRVWBY
Preview by (browsing: active employees in owner department)
Child collections (tabs)

Possible causes — HSE_NRSTMISCENTPSSBLNRSTMISCCUSE (with TXNSTS, line numbers).
Expected hazards — HSE_NRSTEXPCTDHRZ (TXNSTS 4 vs 5 locks fields on confirm vs follow-up tabs).
Required actions — HSE_NRSTMISCENTRQDACTN (NRSTMISCENTRQDACTN_RQRCAR, verification fields, etc.).
Attachments, signatures, environment, investigation items, comments, tracing — matching NRSTMISCENT* tables.
Required vs optional
Client marks many fields MUST / OPTIONAL / ALWAYSLOCKED depending on tab and TXNSTS (e.g. confirmation vs follow-up). Authoritative requiredness may also live in the form metadata / DB. Assumption: server-side validation exists in procs not shown in C++.

5. Rules and business logic
Complete requires saved record (multi-select count / message: “You must save the Record first”).
Observation Accepted vs Require CAR coupling (RQ-1-2024.6).
Default Require CAR on new recommended-action lines from CAR policy (HSE_HSEPLCCAR).
Lock observation entry tabs when HSEOBSRVTN_LCKENTRYTABS = 'YES'.
Reject reasons: cannot add duplicate reject reason rows (toolbar save path); on Accept, if reasons exist, user prompted to delete.
Cancel: confirmation dialog; trace row in HSE_NRSTMISCENTTRC with SRCSCRN = Observation Review or Observation Approval.
Department target STD: getTargetMonthSTD reads HSE_OBSRVTNTRGTSTRGTS for expected observations by department/month/year.
HSE Observation policy exit check on policy screen: HSEOBSRVTN_GnrCrStt must be non-empty (dummy UPDATE used to validate).
CAR from observation: status padded to 2 chars; serial = max+1 per year and CAR source; priority fixed Medium in client; auto-generated flag Y.
Email: ID_MAIL_Observation_* constants map HSE_NRSTMISCENTRQDACTN to CAR-related mail templates; Observation Complete mail send is commented out in NearMissEntryCategory.cpp (notification may be DB/job elsewhere).
6. External dependencies
Dependency	Role
SQL Server stored procs
completeNearMissTXN, confirmNearMissTXN, rejectObservation, closeNearMissTXN, getRootCauses, getExpectedHazards, CopyImages
Tables
HSE_vwNRSTMISCENT, HSE_NRSTMISCENT*, HSE_NRSTMISCENTTRC, HSE_HSEOBSRVTN, HSE_HSEPLCCAR, HSE_HSEPLC, HSE_OBSRVTNTRGTS*, HSE_CRENTRY, CMN_EMPLOYEE, CMN_DEPART, …
CAR module
CARCategory — CAR basis 3 = Observation; opens HSE_TgNrstMiscInq for linked TXN
Setup
NearMissTypesCategory (types / expected hazards / root cause templates), ShowObsrvtnTrgts (observation targets)
Framework
CHSEMSCommonCategory — Reject, InsertActionRecordIntoTracingTab, generateCAR / mail helpers for required-action tables
COM / UI host
CHSEObj exposes ShowNearMissEntry, ShowNearMissConfirmation, ShowNearMissFollowUp, ShowNearMissInquiry, ShowNearMissReward, ShowNearMissTypes
7. Suggested modernization mapping
Legacy concept	REST / service sketch	Frontend (e.g. React)
Entry / Review / Approval / Inquiry
POST/GET/PATCH /observations, POST .../complete, .../confirm, .../reject, .../close, .../cancel
Wizard or role-based routes: Draft → Submitted → Under review → Confirmed → Closed
Status
status enum mirroring 1,3,4,5,6,99 with named constants
Route guards + read-only modes per status
Child collections
Sub-resources /observations/:id/required-actions, /possible-causes, /hazards, /attachments
Tabbed forms; optimistic updates or section saves
Policy
GET /hse-policy/observation, GET /hse-policy/car
Cache in React Query / context
CAR spawn
POST /observations/:id/close returns { carId } if requireCar
Modal + deep link to CAR app
Tracing / audit
Append-only audit-events or DB trigger
Timeline component
State management:
Server is source of truth (status + proc side effects). Client holds draft state only for incomplete edits; use React Query (or similar) for refetch after each transition command.

Node.js backend:
Thin orchestration layer calling the same stored procedures (or replaced domain service) preserves behavior; gradually inline proc logic into transactional services with explicit state machine tests.

Source anchors (C++)
Lifecycle buttons and reject/cancel: 273:327:D:\Work\CodeOnly\Node_React\Products\HSEMS_WEB_Original\HSEMS-Win\HSEMS.DLL\NearMissCategory.cpp
Entry complete + completeNearMissTXN: 29:179:D:\Work\CodeOnly\Node_React\Products\HSEMS_WEB_Original\HSEMS-Win\HSEMS.DLL\NearMissEntryCategory.cpp
Review accept + confirmNearMissTXN: 44:101:D:\Work\CodeOnly\Node_React\Products\HSEMS_WEB_Original\HSEMS-Win\HSEMS.DLL\NearMissConfirmationCategory.cpp
Approval close + CAR insert: 43:210:D:\Work\CodeOnly\Node_React\Products\HSEMS_WEB_Original\HSEMS-Win\HSEMS.DLL\NearMissFollowUpCategory.cpp
Inquiry status filters: 39:62:D:\Work\CodeOnly\Node_React\Products\HSEMS_WEB_Original\HSEMS-Win\HSEMS.DLL\NearMissInquiry.cpp
CAR ↔ Observation link (basis 3): 192:195:D:\Work\CodeOnly\Node_React\Products\HSEMS_WEB_Original\HSEMS-Win\HSEMS.DLL\CARCategory.cpp
Assumptions recap: (1) Stored procedures implement the same status codes as inquiry filters. (2) Meaning of status 2 is not user-visible in this client. (3) Some validation and notifications may exist only in SQL or jobs, not in C++.

Your repo also contains HSEMS_WEB_Original\Observation_Reporting_Process_UML.md, which aligns with this reading and adds web-layer detail if you want a single consolidated spec.