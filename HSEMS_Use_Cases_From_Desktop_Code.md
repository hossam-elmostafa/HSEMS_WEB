# HSEMS Desktop Application – Use Cases Extracted from C++ Source Code

> **Source**: `HSEMS-Win` C++ codebase + `IDR_MAINFRAME MENU DISCARDABLE.vb`
> **Date**: March 2026

---

## Table of Contents

1. [System Overview](#1-system-overview)
2. [Safety Module](#2-safety-module)
3. [Environment Module](#3-environment-module)
4. [Performance Measurement Module](#4-performance-measurement-module)
5. [Inquiry Screens](#5-inquiry-screens)
6. [Reports & Analytics](#6-reports--analytics)
7. [Settings & Master Data](#7-settings--master-data)
8. [Program Management & Administration](#8-program-management--administration)
9. [Additional Records & Handover](#9-additional-records--handover)
10. [Cross-Cutting Workflows](#10-cross-cutting-workflows)
11. [Appendix: Transaction Lifecycle & Stored Procedures](#11-appendix-transaction-lifecycle--stored-procedures)

---

## 1. System Overview

HSEMS (Health, Safety & Environment Management System) is a comprehensive HSE platform covering:

- **Safety**: Observation/near miss reporting, incident/accident management, risk assessment, permit-to-work, awareness training, rescue plans
- **Environment**: Waste management, chemical registers, environmental monitoring, emissions/aspects tracking
- **Performance**: Site surveys, equipment inspections, emergency drills, auditing, performance measurement
- **Corrective Actions**: CAR (Corrective Action Request) lifecycle with multi-stage approval
- **Administration**: User privileges, policy configuration, code structures, reporting

### Common Transaction Lifecycle

Most operational screens follow this workflow:

```
Entry (Incomplete) → Complete → Confirm/Review → Approve → Close
                                    ↓
                                  Reject → Return to Entry
```

Each transition is traced in a **Tracing Tab** for full audit trail.

---

## 2. Safety Module

### 2.1 Observation / Near Miss Management

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter observation** | Observation Entry | User creates a new near miss / observation record with details (type, location, description, hazards, causes) |
| **Complete observation** | Observation Entry | User clicks "Complete" to submit the observation for review (calls `completeNearMissTXN`) |
| **Review observation** | Observation Review | Reviewer examines observation details, can accept or reject |
| **Accept observation** | Observation Review | Reviewer accepts the observation, advancing it in the workflow |
| **Reject observation** | Observation Review | Reviewer rejects with a reason (opens Reject Reason dialog, calls `rejectObservation`) |
| **Approve observation** | Observation Approval | Approver confirms the observation |
| **Close observation** | Observation Approval | Approver closes the observation (calls `closeNearMissTXN`), optionally generating a CAR |
| **Cancel observation** | Entry / Review | User cancels an incomplete observation |
| **Reward near miss reporter** | Near Miss Rewards | Manage rewards for employees who report near misses |
| **Manage attachments** | All observation screens | Upload/manage file attachments per observation |
| **Manage environment conditions** | All observation screens | Record environmental conditions at time of observation |
| **Manage required actions** | Review / Approval | Define corrective actions required for the observation |
| **View observation tracing** | All observation screens | View full audit trail of status changes |

**Custom Buttons**: `NRSTMISCENT_ENTCMPLT` (Complete), `NRSTMISCENT_ACP` (Accept), `NRSTMISCENT_CLS` (Close), `NRSTMISCENT_RJC` (Reject), `NRSTMISCENT_CNCL` (Cancel)

---

### 2.2 Incident / Accident Management

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter incident flash report** | Flash Report Entry | Quick initial report of an incident |
| **Complete flash report** | Flash Report Entry | Submit flash report for review (`CompleteFlashEntry`) |
| **Review flash report** | Flash Report Review | Reviewer accepts or rejects (`AccptFlashFromReview` / `RejectFlashFromReview`) |
| **Enter accident record** | Accident Entry | Full accident report with injury details, causes, witnesses |
| **Complete accident** | Accident Entry | Submit for confirmation (`completeAccidentTXN`) |
| **Confirm accident** | Accident Confirmation | Confirm the accident record |
| **Follow up accident** | Accident Follow-up | Track resolution and close (`closeAccidentTXN`) |
| **Reject accident** | Confirmation | Reject with reason |
| **Record injured persons** | Accident Entry | Add injured person details (body parts, injury type, PPE worn) |
| **Analyze injury** | Accident Entry | Open injury analysis screen |
| **Record accident causes** | Accident Entry | Document root causes (`GETROTCUS` button) |
| **Enter incident investigation** | Investigation Entry | Detailed investigation of an incident |
| **Approve investigation** | Investigation Approval | Approve investigation findings |

**Custom Buttons**: `ACDNTENT_ENTCMPLT`, `ACDNTENT_CNFRM`, `ACDNTENT_CLS`, `ACDNTENT_RJC`, `ACDNTENTINJRDPRSN_INJRDBODYPART`, `INJURY_ANALYSIS`

---

### 2.3 Vehicle Accident Management

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter vehicle accident** | Vehicle Accident Entry | Record vehicle accident details with questionnaire |
| **Complete vehicle accident** | Entry | Submit for confirmation (`COMPLETE_VEHICLE_ACCIDENT`) |
| **Confirm vehicle accident** | Confirmation | Confirm the vehicle accident record |
| **Close vehicle accident** | Follow-up | Close with resolution |
| **Reject vehicle accident** | Various | Reject with reason |
| **Answer vehicle questionnaire** | Entry | Fill in standard vehicle accident questionnaire |

---

### 2.4 Loss Accident Management

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter loss accident** | Loss Accident Entry | Record property/asset loss details |
| **Complete loss accident** | Entry | Submit for confirmation |
| **Confirm loss accident** | Confirmation | Confirm the loss record |

---

### 2.5 Risk Assessment

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter risk assessment** | Risk Assessment Entry | Create assessment for a job/task with hazards and controls |
| **Complete risk assessment** | Entry | Submit for approval (`completeRskAssmntTXN`) |
| **Approve risk assessment** | Approval | Approve the assessment (`confirmRskAssmntTXN`) |
| **Close risk assessment** | Follow-up | Close the assessment (`closeRskAssmntTXN`) |
| **Reject risk assessment** | Approval / Follow-up | Reject with reason |
| **Manage potential hazards** | Entry | Add/edit potential hazards (popup `HSE_TGPTNLHZRD`) |
| **Manage control measures** | Entry | Define necessary control measures |
| **View risk matrix** | Entry | Display risk severity/likelihood matrix |
| **Manage task responsibilities** | Entry | Assign task responsibilities (popup `HSE_TGTskRsp`) |
| **Manage job steps & effects** | Entry | Define job steps with associated effects |
| **Check risk ranking** | Entry | Calculate and verify risk rank |

**Custom Buttons**: `RSKASSMNTENT_ENTCMPLT`, `RSKASSMNTENT_APRV`, `RSKASSMNTENT_CLS`, `RSKASSMNTENT_SHOWMATRIX`, `POTENTIAL_HAZARDS`, `TASK_RESPONSIBILITY`

---

### 2.6 Potential Hazard Management

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter potential hazard** | Potential Hazard Entry | Record a potential hazard with severity/likelihood |
| **Complete potential hazard** | Entry | Submit for confirmation |
| **Confirm potential hazard** | Confirmation | Confirm the hazard record |
| **Close potential hazard** | Follow-up | Close after resolution |
| **Calculate risk rank** | Entry | Auto-calculate risk rank from severity × likelihood |

---

### 2.7 Permit to Work (PTW)

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Register PTW** | PTW Entry | Create a new permit to work with type, location, safety instructions |
| **Complete PTW** | Entry | Submit for confirmation (`completePTWTXN`) |
| **Confirm PTW** | Confirmation | Confirm the permit (`confirmPTWTXN`) |
| **Close PTW** | Closure | Close the permit after work completion (`closePTW`) |
| **Reject PTW** | Various | Reject with reason (`rejectPTW`) |

---

### 2.8 Awareness / Training Plans

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter awareness plan** | Awareness Plan Entry | Create training/awareness course plan with courses, instructors, target departments |
| **Approve awareness plan** | Approval | Approve the plan (`AwrnsPlnApproved`) |
| **Execute awareness plan** | Execution | Record actual execution, mark attendees |
| **Record course attendees** | Execution | Enter attendee details with grades |
| **Reject awareness plan** | Approval | Reject with reason (`AwrnsPlnRejected`) |
| **Close awareness plan** | Various | Close completed plan (`AwrnsPlnClosed`) |

---

### 2.9 Rescue Plans

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter rescue plan** | Rescue Plan Entry | Create rescue plan with hazards, tools, communication methods |
| **Close rescue plan** | Entry | Close completed plan (`closeRscuPlan`) |
| **Cancel rescue plan** | Entry | Cancel plan (`cancelRscuPlan`) |
| **Return plan to entry** | Inquiry | Reopen plan for editing (`rtrnRscuPlnToEntry`) |
| **View cancel reason** | Tracing | View why a plan was cancelled |

---

### 2.10 Safety Tools & Equipment Distribution

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Record tool/equipment distribution** | Distribution Record | Track which safety tools/equipment were distributed to whom |

---

## 3. Environment Module

### 3.1 Waste Management

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter waste receiving** | Waste Receiving Entry | Record incoming waste (type, quantity, zone, packaging) |
| **Complete waste receiving** | Entry | Submit receiving record (`Complete_Waste_Entry`) |
| **Enter waste disposal** | Waste Disposal Entry | Record waste disposal (transporter, treatment plant, destination) |
| **Complete waste disposal** | Entry | Submit disposal record (`Complete_Waste_Dspsl_Entry`) |
| **Return to entry** | Both | Revert a completed entry back to draft (`Return_compltd_waste_to_entry`) |
| **View completed entries** | Both | Filter to show only completed waste entries |

---

### 3.2 Chemical Register

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Manage chemical register** | Chemical Register | CRUD for chemical materials on-site (items, hazards, quantities, frequencies) |

---

### 3.3 Environmental Monitoring

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Create monitoring plan** | Env. Monitoring Plan | Define environmental monitoring activities and schedules |
| **Close monitoring plan** | Plan | Close plan (`closeEnvMntrPlan`) |
| **Approve monitoring plan** | Plan | Approve plan (`approveEnvMntrPlan`) |
| **Reopen monitoring plan** | Plan | Reopen closed plan (`reOpnEnvMntrPlan`) |
| **Record monitoring execution** | Env. Monitoring Execution | Enter actual measurement results |
| **Enter environment measurement** | Measurement Entry | Record specific measurement values |
| **Approve measurement** | Measurement Approval | Approve measurements (`approveEnvMsr`) |
| **Reject measurement** | Measurement Approval | Reject measurements (`rejectEnvMsr`) |

---

### 3.4 Emissions / Aspects

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter aspects (emissions)** | Aspects Entry | Record emission/aspect data (type, activity, quantity, UOM) |
| **Review aspects** | Aspects Review | Review and approve/reject aspects data |

---

## 4. Performance Measurement Module

### 4.1 Site Survey / Site Visit

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter site survey** | Site Survey Entry | Record site visit findings |
| **Complete site survey** | Entry | Submit for confirmation (`completeSitSrvyTXN`) |
| **Confirm site survey** | Confirmation | Confirm findings (`confirmSiteSrvy`) |
| **Close site survey** | Follow-up | Close the survey (`closeSiteSurveyTXN`) |
| **Reject site survey** | Various | Reject with reason (`rejectSitSrvy`) |
| **Define required actions** | Various | Add required corrective actions from survey findings |

---

### 4.2 Equipment Inspection

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Request equipment inspection** | Inspection Request | Submit request to inspect equipment before site entry |
| **Record inspection result** | Inspection Result | Enter inspection findings |
| **Accept inspection** | Result | Accept equipment (`AcceptEQInspctn`) |
| **Reject inspection** | Result | Reject equipment (`rjctEQInspctn`) |
| **Hold inspection** | Result | Put on hold (`holdEQInspctn`) |
| **View equipment on site** | Equipment Inside Site | Track equipment currently inside the work site |

---

### 4.3 Emergency Drill Plans

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter drill plan** | Drill Plan Entry | Create an emergency drill/exercise plan with scenarios |
| **Complete drill plan** | Entry | Submit plan (`COMPLETE_DRILL_PLAN`) |
| **Approve drill plan** | Approval | Approve the plan (`ApproveDrillPlan`) |
| **Reject drill plan** | Approval | Reject the plan (`RejectDrillPlan`) |
| **Execute drill** | Drill Execution | Record actual drill execution and findings |
| **Close drill** | Execution | Close completed drill (`CloseDrill`) |
| **Cancel drill** | Various | Cancel a drill (`CancelDrill`) |
| **Create unplanned drill** | Entry | Create an ad-hoc unplanned drill |
| **Record drill findings** | Execution | Document findings and unplanned scenarios |

---

### 4.4 Audit Management

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter audit plan** | Audit Plan Entry | Create audit plan with standards, clauses, schedules |
| **Approve audit plan** | Audit Plan Approval | Approve the audit plan (`completeAuditTXN`, `confirmAuditTXN`) |
| **Send audit notification** | Audit Notification | Notify auditees about upcoming audit |
| **Record audit results** | Audit Execution Result | Enter findings against checklist clauses |
| **Confirm audit results** | Audit Result Confirmation | Confirm the audit findings |
| **Reject audit plan** | Approval | Reject with reason |
| **Manage audit checklist** | Checklist screens | Define and fill audit checklist items |
| **Manage auditors** | Auditors screen | Assign auditors to audit plans |
| **Manage auditees** | Auditees screen | Assign auditees to audit plans |

---

### 4.5 Performance Measurement Visits

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter performance visit** | Performance Entry | Record a performance measurement visit |
| **Complete performance visit** | Entry | Submit (`completePrfrmncMsrmntTXN`) |
| **Evaluate performance visit** | Evaluation | Score/evaluate the visit (`confirmPrfrmncMsrmntTXN`) |
| **Follow up performance visit** | Follow-up | Track resolution and close (`closePrfrmncMsrmntTXN`) |
| **Reject performance visit** | Various | Reject with reason (`rejectPerformaceTXN`) |
| **View checklist details** | Checklist | Open and manage performance checklist items |

---

### 4.6 Corrective Action Requests (CAR)

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Enter CAR** | CAR Entry | Create a corrective action request (can be auto-generated from observations, audits, etc.) |
| **Complete CAR** | Entry | Submit CAR (`completeCARTXN`) |
| **Review CAR** | CAR Review | Review the CAR details |
| **Approve CAR** | CAR Approval | Approve the CAR (`confirmCARTXN`) |
| **Accept CAR** | Various | Accept the CAR for execution (`setCARAcptd`) |
| **Reject CAR** | Various | Reject CAR (`rejectCARTXN`, `setCARNotAcptd`) |
| **Cancel CAR** | Various | Cancel a CAR (`cancelCAR`) |
| **Close CAR** | Follow-up | Close the CAR (`closeCARTXN`) |
| **Enter corrective actions** | Actions Entry | Define specific corrective action items |
| **Review corrective actions** | Actions Review | Review action items |
| **Receive corrective actions** | Actions Received | Acknowledge receipt of assigned actions |
| **Execute corrective actions** | Actions Under Execution | Track action execution (`AcceptCARExe`, `RejectCARExe`) |
| **Complete action** | Execution | Mark action completed (`CARJobCmpltd`) |
| **Follow up CAR** | CAR Follow-up | Track overall CAR completion |
| **Edit CAR** | CAR Editing | Modify existing CAR details |
| **View source transaction** | Various | Navigate to the originating record (e.g., observation, audit) |
| **View CAR approval info** | Various | Display approval/review information |

---

## 5. Inquiry Screens

All inquiry screens provide **read-only** views with **filter buttons** to view records by status:

| Inquiry Screen | Filters Available |
|---------------|-------------------|
| Observation Inquiry | Incomplete, Rejected, Complete, Confirmed, Closed, All |
| Incident Flash Report Inquiry | Similar filters |
| Accident/Incident Inquiry | Incomplete, Rejected, Complete, Approved, Closed, All |
| Risk Assessment Inquiry | Incomplete, Rejected, Complete, Approved, Closed, All |
| Site Survey Inquiry | Similar filters |
| PTW Inquiry | Incomplete, Rejected, Complete, Confirmed, Closed, All |
| Performance Measurement Inquiry | Incomplete, Rejected, Complete, Approved, Closed, All |
| Audit Plan Inquiry | Similar filters |
| Corrective Actions Inquiry | Incomplete, Rejected, Complete, Confirmed, Closed, All |
| Equipment Inspection Inquiry | - |
| Awareness Plan Inquiry | - |
| Rescue Plan Inquiry | - |
| Drill Inquiry | - |
| Incident Medical Inquiry | - |
| Audit Procedures Inquiry | - |

---

## 6. Reports & Analytics

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Manage report groups** | Report Groups | Define groups/categories of reports |
| **Browse and run reports** | Reports | Select and execute configured reports |
| **View report information** | Report Info | View metadata about available reports |
| **Generate monthly reports** | Monthly Reports | Standard monthly HSE statistics |
| **Generate IFC report** | IFC Report | International Finance Corporation report format |
| **Generate EGPC monthly reports** | EGPC Monthly | Egyptian General Petroleum Corporation format |
| **Generate TG-KPI report** | TG-KPI Report | Key Performance Indicator report |
| **Generate flash forms** | Flash Forms | Quick summary forms |
| **Export to Excel** | Various | Export data/reports to Excel |
| **Print reports** | Various | Print any configured report |

---

## 7. Settings & Master Data

### 7.1 General Settings

| Category | Items |
|----------|-------|
| **Company** | Company profile, First company data, HSE Policy |
| **Organization** | Projects, Departments, Department groups |
| **HR** | Employees, Jobs/Trades, Signatures, Contact methods |
| **Geography** | Countries, Sites, Site types, Work locations |
| **General** | Currencies, Units of measure, Document types, Attachment file control, Roles, Data rejection reasons |

### 7.2 Incident Settings

| Items |
|-------|
| Incident types, Incident causes, Contributing factors, Loss types, Incident period |
| Vehicle/equipment types, Vehicle incident questionnaire |
| Injury causes, Body parts, Environmental conditions, PPE |
| Asset types, Potential severity, Medical authority, Medical decisions |
| Related operations, Spill types, Containment types, Material release classification |
| Released materials, Material release to, Spill classification |

### 7.3 Near Miss / Observation Settings

| Items |
|-------|
| Near miss types, Life-saving rules, Reward types, Observation targets |

### 7.4 Hazard / Risk Settings

| Items |
|-------|
| Risk assessment task list, Job step definitions, Consequence definitions |
| Hazards, Precautions, Control measures |
| Risk severity rating, Risk level, Risk matrix, Risk likelihood rating |
| Basic reference, Operation types, Area hazards, Risk types |

### 7.5 Other Module Settings

| Module | Items |
|--------|-------|
| **Performance** | Performance measurement scores, Performance measurement checklist |
| **Audit** | Standards, Clause groups, Clause list, Auditees, Certificate type/provider, Audit role, Unplanned audit justifications, Schedule change reason, Default distribution, Certified auditors |
| **PTW** | PTW types, PTW responsibility, Safety instructions |
| **Emissions/Aspects** | Aspect type, Aspect activity, Aspect frequency, Aspect UOM, Aspect state |
| **Chemicals** | Chemical hazards, Chemical items definition, Item frequency |
| **Emergency Plans** | Main drill types, Drill types, External support, Emergency management team, ERT list, Drill role, Internal resource, Involved parties |
| **Waste** | Packaging methods, Waste items, Waste zones, Waste transporters, Waste treatment plants |
| **Env. Monitoring** | Environmental activities, Environmental measurements |
| **Equipment Inspection** | Equipment types |
| **Awareness** | Awareness courses, Attendee grades |
| **Rescue Plans** | Rescue entities, Confined space types, Rescue hazards, Rescue tools, Rescue communication methods |
| **Safety Tools** | Safety tools and equipment |
| **Contracts** | Contractors |
| **Corrective Actions** | CAR action classification, CAR source (basis), Top events classification, Action reason |
| **Site Visits** | Site survey types |

---

## 8. Program Management & Administration

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Switch language** | Menu | Toggle between English and Arabic |
| **Configure application setup** | Application Setup | System-wide configuration |
| **Manage application users** | Application Users | Create and manage user accounts and privileges |
| **Configure menu privileges** | Menu Privilege | Control which menu items each user/role can access |
| **Configure field privileges** | Field Privilege | Control field-level read/write access per user/role |
| **Configure report privileges** | Report Privilege | Control which reports each user/role can access |
| **Manage code structure** | Code Structure | Configure auto-numbering/coding patterns |
| **Define user checks** | User-Defined Checks | Custom validation rules |
| **Configure mail server** | Mail Server Settings | Email server connection settings |
| **Use email wizard** | Email Wizard | Configure automated email notifications |
| **Open documentation folder** | File menu | Open the physical documentation folder |

---

## 9. Additional Records & Handover

| Use Case | Screen | Description |
|----------|--------|-------------|
| **Define additional logs** | Logs Definition | Define custom log structures |
| **Enter additional log data** | Logs Entry | Enter data into custom logs |
| **Manage subject types** | Subject Type (Handover) | Define handover subject categories |
| **Manage open handovers** | Opened Handover | Track pending handover tasks |
| **Manage closed handovers** | Closed Handover | View completed handover records |

---

## 10. Cross-Cutting Workflows

### 10.1 Common User Actions (Available on Most Screens)

| Action | Description |
|--------|-------------|
| **New** | Create a new record |
| **Save** | Save current record |
| **Delete** | Delete current record (with confirmation) |
| **Cancel** | Cancel current edit |
| **Print** | Print current record/report |
| **Find** | Search for records |
| **Filter / Unfilter** | Apply or remove data filters |
| **Navigate** | First / Previous / Next / Last record |
| **Copy / Paste** | Copy record data |
| **Sort** | Sort records |
| **Show Details** | Open detailed view |

### 10.2 Status-Based Workflow

Every transactional screen enforces a status-based workflow:

| Status | Code | Meaning |
|--------|------|---------|
| Incomplete | 1 | Draft, editable |
| Rejected | 3 | Sent back with reason |
| Complete | 4 | Submitted for next stage |
| Confirmed | 5 | Reviewed and confirmed |
| Closed | 6 | Fully resolved |

### 10.3 Rejection Workflow

1. User clicks Reject button → Reject Reason popup opens
2. User enters reason and confirms
3. System calls `rejectXxx` stored procedure
4. Status is set to Rejected (3)
5. Action is recorded in Tracing Tab
6. Record returns to previous stage for correction

### 10.4 CAR Auto-Generation

When certain transactions are closed (e.g., Observations, Site Surveys), the system can **automatically generate a CAR** based on HSE Policy settings. This links the originating record to a new Corrective Action Request.

### 10.5 Tracing / Audit Trail

Every significant action is recorded in a tracing table:
- **Actions traced**: Created, Complete, Confirm, Approve, Reject, Close, Delete, Cancel, Activate
- **Data recorded**: User, timestamp, action type, source screen, status change

### 10.6 Policy-Gated Features

HSE Policy controls which features are enabled:
- Whether confirmation is required for various modules
- Whether follow-up is required
- Whether CAR generation is automatic
- Various module-specific settings

### 10.7 Cascading Browse (Location Hierarchy)

Many screens support cascading field selection:
```
Site → Location → Work Area
```

### 10.8 Email Notifications

The system can send email notifications for:
- Completed observations/incidents
- Approval requests
- Rejection notifications
- Other configurable events

---

## 11. Appendix: Transaction Lifecycle & Stored Procedures

### Complete Summary of Stored Procedures by Module

| Module | Complete | Confirm | Close | Reject | Other |
|--------|----------|---------|-------|--------|-------|
| Near Miss/Observation | completeNearMissTXN | confirmNearMissTXN | closeNearMissTXN | rejectObservation | – |
| Accident | completeAccidentTXN | – | closeAccidentTXN | rejectIncident | – |
| Vehicle Accident | (Complete button) | (Confirm button) | (Close button) | – | – |
| Risk Assessment | completeRskAssmntTXN | confirmRskAssmntTXN | closeRskAssmntTXN | rejectRiskAssessment | Rsk_Asmnt_entry_Save |
| Site Survey | completeSitSrvyTXN | confirmSiteSrvy | closeSiteSurveyTXN | rejectSitSrvy | – |
| Performance | completePrfrmncMsrmntTXN | confirmPrfrmncMsrmntTXN | closePrfrmncMsrmntTXN | rejectPerformaceTXN | – |
| PTW | completePTWTXN | confirmPTWTXN | closePTW | rejectPTW | – |
| CAR | completeCARTXN | confirmCARTXN | closeCARTXN | rejectCARTXN | cancelCAR, sp_Generate_CARs |
| Audit | completeAuditTXN | confirmAuditTXN | – | – | – |
| Flash Report | CompleteFlashEntry | – | – | RejectFlashFromReview | AccptFlashFromReview |
| Drill Plan | COMPLETE_DRILL_PLAN | ApproveDrillPlan | CloseDrill | RejectDrillPlan | CancelDrill |
| Awareness Plan | AwrnsPlnEntryCompleted | AwrnsPlnApproved | AwrnsPlnClosed | AwrnsPlnRejected | – |
| Rescue Plan | – | – | closeRscuPlan | – | cancelRscuPlan, rtrnRscuPlnToEntry |
| Waste | Complete_Waste_Entry | – | – | – | Return_compltd_waste_to_entry |
| Env. Monitoring | completeEnvMsrEntry | approveEnvMntrPlan | closeEnvMntrPlan | rejectEnvMsr | reOpnEnvMntrPlan |
| Equipment | completeEQInspctn | AcceptEQInspctn | – | rjctEQInspctn | holdEQInspctn |

### Common Stored Procedures

| Procedure | Purpose |
|-----------|---------|
| InsertRecIntoTracingTab | Record action in audit trail |
| updateTXNSts | Update transaction status |
| generateNewTXNNum | Generate new transaction number |
| ChangeEntityStatus | Change entity status (CAR, etc.) |
| GetPolicyValue | Read HSE Policy configuration |

---

*End of document*
