/**
 * Screen handlers registry
 * Maps screen tags to per-screen handler modules.
 * Add handler modules and register them in SCREEN_HANDLERS below.
 */

import * as HSE_TgNrstMiscEnt from './Safety/Observation/Observation_Entry.js';
import * as HSE_TgNrstMisccnfrmtn from './Safety/Observation/Observation_Review.js';
import * as HSE_TgNrstMiscflwup from './Safety/Observation/Observation_Approval.js';
import * as HSE_TgNrMisRward from './Safety/Observation/Observation_Reward.js';
import * as HSE_TGNrMisTypes from './Setup/Observations/Observation_Type.js';
import * as HSE_TgObsrvtnTrgts from './Setup/Observations/Observation_Targets.js';
import * as HSE_TgNrstMiscInq from './Inquiry/Observation_Inquiry.js';
import * as HSE_TgCompPrf from './Setup/General/Company_Profile.js';
import * as HSE_DrllPlnEntryAtEntry from './Performance/Emergency Drills/Drill_Plan_Entry.js';
import * as HSE_TgHSEPlc from './Setup/General/HSE_Policy.js';
import * as CMN_CURENCY from './Setup/General/Currency.js';
import * as HSE_TgPrjcts from './Setup/General/Projects.js';
import * as HSE_Tgrol from './Setup/General/Role.js';
import * as HSE_TgEntRjcRsn from './Setup/General/Entry_Reject_Reasons.js';
import * as HSE_TgUOM from './Setup/General/UOM.js';
import * as CMN_DOCTYP from './Setup/General/Document_Types.js';
import * as CMN_AtchmntFrmt from './Setup/General/Attachment_Formats.js';
import * as HSE_TgFirstInfo from './Setup/General/First_Info.js';
import * as HSE_TgDprtmntGrp from './Setup/General/HR/Department_Group.js';
import * as CMN_DEPART from './Setup/General/HR/Department.js';
import * as CMN_EMPLOYEE from './Setup/General/HR/Employee.js';
import * as CMN_TRADES from './Setup/General/HR/Trades.js';
import * as HSE_TGSGNTUR from './Setup/General/HR/Signatures.js';
import * as CMN_CntctMthds from './Setup/General/HR/Contact_Methods.js';
import * as HSE_TgSiteType from './Setup/General/HR/Site_type.js';
import * as HSE_TGSTE from './Setup/General/HR/Site.js';
import * as HSE_TgWrkLoc from './Setup/General/HR/Work_Location.js';
import * as CMN_COUNTRIES from './Setup/General/HR/Country.js';
import * as HSE_TgAcdntTyp from './Setup/Incidents/Incidents_Type.js';
import * as HSE_TgAcdntRsn from './Setup/Incidents/Incidents_Cause.js';
import * as HSE_TgAcdntmdm from './Setup/Incidents/Leading_Activity.js';
import * as HSE_TgTypLoss from './Setup/Incidents/Type_of_Loss.js';
import * as HSE_TgIncdntPrd from './Setup/Incidents/Incident_Period.js';
import * as HSE_TGVCLTYP from './Setup/Incidents/Vehicle_Type.js';
import * as HSE_TGVCLQSTNS from './Setup/Incidents/Vehicle_Accident_Questions.js';
import * as HSE_TGINJRYTYP from './Setup/Incidents/Injury_Cause.js';
import * as HSE_TghumnBdyPrt from './Setup/Incidents/Body_Parts.js';
import * as HSE_TGENVRNMNTCND from './Setup/Incidents/Environment_Condition.js';
import * as HSE_TGPRSNLPRTCTVEQ from './Setup/Incidents/Personal_Protective_Equipment.js';
import * as HSE_TgAstTyp from './Setup/Incidents/Asset_Type.js';
import * as HSE_TGPTNTLSVRTY from './Setup/Incidents/Potential_Severity.js';
import * as HSE_TgMdclAuthrty from './Setup/Incidents/Medical_Authority.js';
import * as HSE_TgMdclDcsn from './Setup/Incidents/Medical_Decision.js';
import * as HSE_TgRltdOprtns from './Setup/Incidents/Related_Operations.js';
import * as HSE_TgMtrlRlsTyp from './Setup/Incidents/Material_Release_Type.js';
import * as HSE_TgCntnmntTyp from './Setup/Incidents/Containment_Type.js';
import * as HSE_TgMtrlRlsCat from './Setup/Incidents/Material_Release_Category.js';
import * as HSE_TgRlsMtrl from './Setup/Incidents/Released_Material.js';
import * as HSE_TgMtrlRlsTo from './Setup/Incidents/Material_Released_To.js';
import * as HSE_TgSplClsfctn from './Setup/Incidents/Spill_Classification.js';
import * as HSE_TGRWRDTYP from './Setup/Observations/Reward_Type.js';
import * as HSE_TgTsk from './Setup/Risks/Jobs.js';
import * as HSE_TgJobStpDfntn from './Setup/Risks/Tasks.js';
import * as HSE_TGCnsqncsDfntn from './Setup/Risks/Effects.js';
import * as HSE_Tghzrd from './Setup/Risks/Hazard.js';
import * as HSE_TGPrctns from './Setup/Risks/Precautions.js';
import * as HSE_TGHRCTRL from './Setup/Risks/Control_Measures.js';
import * as HSE_TgRskRat from './Setup/Risks/Severity_Rating.js';
import * as HSE_TgRskLvl from './Setup/Risks/Risk_Level.js';
import * as HSE_TgrskRnkDesc from './Setup/Risks/Risk_Matrix.js';
import * as HSE_TgrsklklyhodRat from './Setup/Risks/Likelihood_Rating.js';
import * as HSE_TGBSCREF from './Setup/Risks/Basic_References.js';
import * as HSE_TgHzrdPrcs from './Setup/Risks/Hazard_Process.js';
import * as HSE_TgAreaHzrds from './Setup/Risks/Area_Hazards.js';
import * as HSE_TgRskTyp from './Setup/Risks/Risk_Type.js';
import * as HSE_TgperfMsrmntscor from './Setup/Performance Measure/Performance_Measure_Score.js';
import * as HSE_TgperfMsrmntChkList from './Setup/Performance Measure/Performance_Measurement_Check_List.js';
import * as HSE_AudtStndrds from './Setup/Audits/Audit_Standards.js';
import * as HSE_TgClausGrp from './Setup/Audits/Clause_Group.js';
import * as HSE_AudtCluss from './Setup/Audits/Audit_Clauses.js';
import * as HSE_Audt from './Setup/Audits/Auditee.js';
import * as HSE_AudtRl from './Setup/Audits/Audit_Role.js';
import * as HSE_CrtfctPrvdr from './Setup/Audits/Certificate_Provider.js';
import * as HSE_Un_PlnndAudtJustfctn from './Setup/Audits/Un-Planned_Audit_Justification.js';
import * as HSE_SchdulChngRsn from './Setup/Audits/Schedule_Change_Reason.js';
import * as HSE_DfultDstrbutnLst from './Setup/Audits/Default_Notifications.js';
import * as HSE_vw_CRTFDAUDTRS from './Setup/Audits/Certified_Auditors.js';
import * as HSE_TgPTWsTyp from './Setup/Permit to Work/PTW_Type.js';
import * as HSE_TgPTWsRspnsblty from './Setup/Permit to Work/PTW_Responsibility.js';
import * as HSE_TgSftyInstrctn from './Setup/Permit to Work/Safety_Instructions.js';
import * as HSE_AspctTyp from './Setup/Aspect/Aspect_Type.js';
import * as HSE_AspctActvty from './Setup/Aspect/Aspect_Activity.js';
import * as HSE_AspctFrquncy from './Setup/Aspect/Aspect_Frequency.js';
import * as HSE_AspctUOM from './Setup/Aspect/Aspect_UOM.js';
import * as HSE_AspctStt from './Setup/Aspect/Aspect_State.js';
import * as HSE_ChmclHzrd from './Setup/Chemical/Chemical_Hazard.js';
import * as HSE_ChmclItmsDfntn from './Setup/Chemical/Chemical_Items_Definition.js';
import * as HSE_ChmclItmFrquncy from './Setup/Chemical/Chemical_Item_Frequency.js';
import * as HSE_TgMnDrlTyp from './Setup/Emergency Drills/Main_Drill_Type.js';
import * as HSE_DRLLTYP_VW from './Setup/Emergency Drills/Detail_Drill_Type.js';
import * as HSE_ExtrnlSupprt from './Setup/Emergency Drills/External_Support.js';
import * as HSE_EMTLst from './Setup/Emergency Drills/EMT_List.js';
import * as HSE_TgERTLst from './Setup/Emergency Drills/ERT_List.js';
import * as HSE_DrllRl from './Setup/Emergency Drills/Drill_Role.js';
import * as HSE_IntrnlRsurc from './Setup/Emergency Drills/Internal_Resource.js';
import * as HSE_TgInvlvdPrts from './Setup/Emergency Drills/Invited_Parties.js';
import * as HSE_TgPckngMthds from './Setup/Waste/Packing_Methods.js';
import * as HSE_TgWstItms from './Setup/Waste/Waste_Items.js';
import * as HSE_TgWstZns from './Setup/Waste/Waste_Zones.js';
import * as HSE_TgWstTrnsprtrs from './Setup/Waste/Waste_Transporters.js';
import * as HSE_TgWstTrtmntPlnts from './Setup/Waste/Waste_TreatmentDisposal_Plant.js';
import * as HSE_TgEnvMntrActvts from './Setup/Environment Measures/Environmental_Monitoring_Activities.js';
import * as HSE_TgEnvrnmntlMsrs from './Setup/Environment Measures/Environmental_Measures.js';
import * as HSE_EQTyps from './Setup/EQ Inspection/EQ_Type.js';
import * as HSE_TgAwrnsCrs from './Setup/Awareness/Awareness_Courses.js';
import * as HSE_TgRscuEntts from './Setup/Rescue Plans/Rescue_Entities.js';
import * as HSE_TgCnfndSpcTyps from './Setup/Rescue Plans/Confined_Space_Types.js';
import * as HSE_TgRscuHzrds from './Setup/Rescue Plans/Rescue_Hazards.js';
import * as HSE_TgRscuTls from './Setup/Rescue Plans/Rescue_Tools.js';
import * as HSE_TgRscuCmmunctnMthds from './Setup/Rescue Plans/Rescue_Communication_Methods.js';
import * as HSE_TGTOOLEQUIP from './Setup/Safety Tools/HSE_Tools_Definition.js';
import * as HSE_TgCntrctr from './Setup/Contracts/Contracts/Contractor.js';
import * as HSE_TgActnClssfctn from './Setup/CAR/Action_Hierarchy_Control.js';
import * as HSE_TgCarmodelBas from './Setup/CAR/CAR_Basis.js';
import * as HSE_TgTopEvnts from './Setup/CAR/CAR Threats/Top_Events.js';
import * as CMS_ActnRsn from './Setup/CAR/Action_Reason.js';
import * as HSE_TgWstRcvngEntry from './Environment/Waste Management/Waste_Receiving_Entry.js';
import * as HSE_TgWstDspslEntry from './Environment/Waste Management/Waste_Disposal_Entry.js';
import * as HSE_ChmclRgstr from './Environment/Chemical Register/Chemical_Register.js';
import * as HSE_TgEnvMntrPlan from './Environment/Environment Measures/Environmental_Monitoring_Plan.js';
import * as HSE_TgEnvMntrExctn from './Environment/Environment Measures/Environmental_Measures.js';
import * as HSE_TgEnvMsrEntry from './Environment/Environment Measures/Environmental_Measure_Entry.js'; // RQ_HSE_23_3_26_21_14
import * as HSE_TgEnvMntrActvtPltnt from './Environment/Environment Measures/Environmental_Measures_Popup.js'; // RQ_HSE_23_3_26_21_14
import * as HSE_AspctsEntryAtEntry from './Environment/Aspects Register/Aspects_Entry.js';
import * as HSE_AspctsRvwAtRvw from './Environment/Aspects Register/Aspects_Review.js';
import * as HSE_TgIncdntFlshRprt from './Safety/Incident/Incident_Flash_Entry.js';
import * as HSE_TgIncdntFlshRprtRvew from './Safety/Incident/Incident_Flash_Review.js';
import * as HSE_TgIncdntMdclRprt from './Safety/Incident/Incident_Medical_Report_Entry.js';
import * as HSE_TgIncdntMdclRprtFlwUp from './Safety/Incident/Incident_Medical_Report_Follow-Up.js';
import * as HSE_TgAcdntEnt from './Safety/Incident/Incident_Preliminary_Entry.js';
import * as HSE_TgAcdntcnfrmtn from './Safety/Incident/Incident_Preliminary_Review.js';
import * as HSE_TgAcdntfollwUpdt from './Safety/Incident/Incident_Preliminary_Follow-Up.js';
import * as HSE_TgIncdntInvstgtnEntry from './Safety/Incident/Incident_Investigation_Entry.js';
import * as HSE_TgIncdntInvstgtnAprvl from './Safety/Incident/Incident_Investigation_Approval.js';
import * as HSE_TgrskassmntEnt from './Safety/Risk Assessment/Risk_Assessment_Entry.js';
import * as HSE_TgrskassmntApr from './Safety/Risk Assessment/Risk_Assessment_Confirmation.js';
import * as HSE_TgRskAssmntFollwUpdt from './Safety/Risk Assessment/Risk_Assessment_Follow-Up.js'; // RQ_HSE_23_3_26_6_00
import * as HSE_TgAwrnsPlnEntry from './Safety/Awareness/Awareness_Plan_Entry.js';
import * as HSE_TgAwrnsPlnAprvl from './Safety/Awareness/Awareness_Plan_Approval.js';
import * as HSE_TgAwrnsPlnExctn from './Safety/Awareness/Awareness_Plan_Execution.js';
import * as HSE_TgRscuPlnEntry from './Safety/Rescue Plan/Rescue_Plan_Entry.js';
import * as HSE_TgToolsEquipDstrbtn from './Safety/Safety_Tools.js';
import * as HSE_TgPTWsrgstrEnt from './Safety/Summary Permits/PTW_Register_Entry.js';
import * as HSE_TgPTWsrgstrcnfrmtn from './Safety/Summary Permits/PTW_Register_Confirmation.js';
import * as HSE_TgPTWsrgstrCls from './Safety/Summary Permits/PTW_Register_Close.js'; // RQ_HSE_23_3_26_6_00
import * as HSE_TgSitSrvyEnt from './Performance/Site Survey/Site_Survey_Entry.js';
import * as HSE_TgSitSrvyCnfrmtn from './Performance/Site Survey/Site_Survey_Confirmation.js';
import * as HSE_TgSitSrvyFlwup from './Performance/Site Survey/Site_Survey_Result_Entry.js';
import * as HSE_TgSitSrvyRqrdActn from './Performance/Site Survey/Site_Survey_Required_Actions_Popup.js'; // RQ_HSE_23_3_26_22_44
import * as HSE_EQInspctnRqust from './Performance/EQ Inspection/EQ_Inspection_Request.js';
import * as HSE_EQInspctnRsult from './Performance/EQ Inspection/EQ_Inspection_Result.js';
import * as HSE_EQInspctnWrk from './Performance/EQ Inspection/In-Site_EQ.js';
import * as HSE_DrllPlnApprvlAtApprvl from './Performance/Emergency Drills/Drill_Plan_Approval.js';
import * as HSE_DrllExcutnAtExcutn from './Performance/Emergency Drills/Drill_Execution.js';
import * as HSE_AudtPlnEntry from './Performance/Auditing/Audit_Plan_Entry.js';
import * as HSE_AudtPlnApprvl from './Performance/Auditing/Audit_Plan_Approval.js';
import * as HSE_AudtNtfctn from './Performance/Auditing/Audit_Notification.js';
import * as HSE_AudtExcutnRsult from './Performance/Auditing/Audit_Result_Entry.js';
import * as HSE_ADTRSLTCNFRMTN from './Performance/Auditing/Audit_Result_Confirmation.js';
import * as HSE_TgperfMsrmntEnt from './Performance/Performance Measure/Performance_Measurement_Entry.js';
import * as HSE_TgperfMsrmntEval from './Performance/Performance Measure/Performance_Measurement_Evaluation.js';
import * as HSE_TgperfMsrmntflwup from './Performance/Performance Measure/Performance_Measurement_Follow-up.js';
import * as HSE_TgCrEntry from './Performance/Action Tracking/CAR_Entry.js';
import * as HSE_TgCrRvw from './Performance/Action Tracking/CAR_Review.js';
import * as HSE_TgCrAprvl from './Performance/Action Tracking/CAR_Approval.js';
import * as HSE_TgActnsEntry from './Performance/Action Tracking/Actions_Entry.js';
import * as HSE_TgActnsRviw from './Performance/Action Tracking/Actions_Review.js';
import * as HSE_TgActnsRcvd from './Performance/Action Tracking/Actions_Received.js';
import * as HSE_TgActnsUndrExec from './Performance/Action Tracking/Actions_Under_Execution.js';
import * as HSE_TgCrFloUp from './Performance/Action Tracking/CAR_Follow_Up.js';
import * as HSE_TgCrEdtng from './Performance/Action Tracking/CAR_Editing.js';
import * as HSE_TgrskassmntInq from './Inquiry screens/Risk_Assessment_Inquiry.js';
import * as HSE_TgperfMsrmntInq from './Inquiry screens/Performance_Measurement_Inquiry.js';
import * as HSE_TgSitSrvyInq from './Inquiry screens/Site_Survey_Inquiry.js';
import * as HSE_TgIncdntFlshRprtInq from './Inquiry screens/Incident_Flash_Inquiry.js';
import * as HSE_TgAcdntInq from './Inquiry screens/Incidents_Inquiry.js';
import * as HSE_TgAdtPlnInq from './Inquiry screens/Audit_Plan_Inquiry.js';
import * as HSE_TgPTWsrgstrInq from './Inquiry screens/PTW_Registry_Inquiry.js';
import * as HSE_TgcrctveAccInq from './Inquiry screens/Corrective_Action_Inquiry.js';
import * as HSE_TGHSEPRCDRLISTINQ from './Inquiry screens/HSE_Procedures_List_Inquiry.js';
import * as HSE_EQInspctnInqry from './Inquiry screens/EQ_Inspection_Inquiry.js';
import * as HSE_TgAwrnsPlnInq from './Inquiry screens/Awareness_Plan_Inquiry.js';
import * as HSE_TgRscuPlnInqury from './Inquiry screens/Rescue_Plan_Inquiry.js';
import * as HSE_DRLLPLNINQ from './Inquiry screens/Drill_Inquiry.js';
import * as HSE_TgLogsDfntn from './Logs/Logs_Definition.js';
import * as HSE_TgLogsEnt from './Logs/Logs_Entry.js';
import * as ReportTypes from './Reports/Report_Groups.js';

const SCREEN_HANDLERS = {
  'HSE_TgNrstMiscEnt': HSE_TgNrstMiscEnt,
  'HSE_TGNrMisTypes': HSE_TGNrMisTypes,
  'HSE_TgObsrvtnTrgts': HSE_TgObsrvtnTrgts,
  'HSE_TgNrstMisccnfrmtn': HSE_TgNrstMisccnfrmtn,
  'HSE_TgNrstMiscflwup': HSE_TgNrstMiscflwup,
  'HSE_TgNrMisRward': HSE_TgNrMisRward,
  'HSE_TgNrstMiscInq': HSE_TgNrstMiscInq,
  'HSE_TgCompPrf': HSE_TgCompPrf,
  'HSE_DrllPlnEntryAtEntry': HSE_DrllPlnEntryAtEntry,
  'HSE_TgHSEPlc': HSE_TgHSEPlc,
  'CMN_CURENCY': CMN_CURENCY,
  'HSE_TgPrjcts': HSE_TgPrjcts,
  'HSE_Tgrol': HSE_Tgrol,
  'HSE_TgEntRjcRsn': HSE_TgEntRjcRsn,
  'HSE_TgUOM': HSE_TgUOM,
  'CMN_DOCTYP': CMN_DOCTYP,
  'CMN_AtchmntFrmt': CMN_AtchmntFrmt,
  'HSE_TgFirstInfo': HSE_TgFirstInfo,
  'HSE_TgDprtmntGrp': HSE_TgDprtmntGrp,
  'CMN_DEPART': CMN_DEPART,
  'CMN_EMPLOYEE': CMN_EMPLOYEE,
  'CMN_TRADES': CMN_TRADES,
  'HSE_TGSGNTUR': HSE_TGSGNTUR,
  'CMN_CntctMthds': CMN_CntctMthds,
  'HSE_TgSiteType': HSE_TgSiteType,
  'HSE_TGSTE': HSE_TGSTE,
  'HSE_TgWrkLoc': HSE_TgWrkLoc,
  'CMN_COUNTRIES': CMN_COUNTRIES,
  'HSE_TgAcdntTyp': HSE_TgAcdntTyp,
  'HSE_TgAcdntRsn': HSE_TgAcdntRsn,
  'HSE_TgAcdntmdm': HSE_TgAcdntmdm,
  'HSE_TgTypLoss': HSE_TgTypLoss,
  'HSE_TgIncdntPrd': HSE_TgIncdntPrd,
  'HSE_TGVCLTYP': HSE_TGVCLTYP,
  'HSE_TGVCLQSTNS': HSE_TGVCLQSTNS,
  'HSE_TGINJRYTYP': HSE_TGINJRYTYP,
  'HSE_TghumnBdyPrt': HSE_TghumnBdyPrt,
  'HSE_TGENVRNMNTCND': HSE_TGENVRNMNTCND,
  'HSE_TGPRSNLPRTCTVEQ': HSE_TGPRSNLPRTCTVEQ,
  'HSE_TgAstTyp': HSE_TgAstTyp,
  'HSE_TGPTNTLSVRTY': HSE_TGPTNTLSVRTY,
  'HSE_TgMdclAuthrty': HSE_TgMdclAuthrty,
  'HSE_TgMdclDcsn': HSE_TgMdclDcsn,
  'HSE_TgRltdOprtns': HSE_TgRltdOprtns,
  'HSE_TgMtrlRlsTyp': HSE_TgMtrlRlsTyp,
  'HSE_TgCntnmntTyp': HSE_TgCntnmntTyp,
  'HSE_TgMtrlRlsCat': HSE_TgMtrlRlsCat,
  'HSE_TgRlsMtrl': HSE_TgRlsMtrl,
  'HSE_TgMtrlRlsTo': HSE_TgMtrlRlsTo,
  'HSE_TgSplClsfctn': HSE_TgSplClsfctn,
  'HSE_TGRWRDTYP': HSE_TGRWRDTYP,
  'HSE_TgTsk': HSE_TgTsk,
  'HSE_TgJobStpDfntn': HSE_TgJobStpDfntn,
  'HSE_TGCnsqncsDfntn': HSE_TGCnsqncsDfntn,
  'HSE_Tghzrd': HSE_Tghzrd,
  'HSE_TGPrctns': HSE_TGPrctns,
  'HSE_TGHRCTRL': HSE_TGHRCTRL,
  'HSE_TgRskRat': HSE_TgRskRat,
  'HSE_TgRskLvl': HSE_TgRskLvl,
  'HSE_TgrskRnkDesc': HSE_TgrskRnkDesc,
  'HSE_TgrsklklyhodRat': HSE_TgrsklklyhodRat,
  'HSE_TGBSCREF': HSE_TGBSCREF,
  'HSE_TgHzrdPrcs': HSE_TgHzrdPrcs,
  'HSE_TgAreaHzrds': HSE_TgAreaHzrds,
  'HSE_TgRskTyp': HSE_TgRskTyp,
  'HSE_TgperfMsrmntscor': HSE_TgperfMsrmntscor,
  'HSE_TgperfMsrmntChkList': HSE_TgperfMsrmntChkList,
  'HSE_AudtStndrds': HSE_AudtStndrds,
  'HSE_TgClausGrp': HSE_TgClausGrp,
  'HSE_AudtCluss': HSE_AudtCluss,
  'HSE_Audt': HSE_Audt,
  'HSE_AudtRl': HSE_AudtRl,
  'HSE_CrtfctPrvdr': HSE_CrtfctPrvdr,
  'HSE_Un_PlnndAudtJustfctn': HSE_Un_PlnndAudtJustfctn,
  'HSE_SchdulChngRsn': HSE_SchdulChngRsn,
  'HSE_DfultDstrbutnLst': HSE_DfultDstrbutnLst,
  'HSE_vw_CRTFDAUDTRS': HSE_vw_CRTFDAUDTRS,
  'HSE_TgPTWsTyp': HSE_TgPTWsTyp,
  'HSE_TgPTWsRspnsblty': HSE_TgPTWsRspnsblty,
  'HSE_TgSftyInstrctn': HSE_TgSftyInstrctn,
  'HSE_AspctTyp': HSE_AspctTyp,
  'HSE_AspctActvty': HSE_AspctActvty,
  'HSE_AspctFrquncy': HSE_AspctFrquncy,
  'HSE_AspctUOM': HSE_AspctUOM,
  'HSE_AspctStt': HSE_AspctStt,
  'HSE_ChmclHzrd': HSE_ChmclHzrd,
  'HSE_ChmclItmsDfntn': HSE_ChmclItmsDfntn,
  'HSE_ChmclItmFrquncy': HSE_ChmclItmFrquncy,
  'HSE_TgMnDrlTyp': HSE_TgMnDrlTyp,
  'HSE_DRLLTYP_VW': HSE_DRLLTYP_VW,
  'HSE_ExtrnlSupprt': HSE_ExtrnlSupprt,
  'HSE_EMTLst': HSE_EMTLst,
  'HSE_TgERTLst': HSE_TgERTLst,
  'HSE_DrllRl': HSE_DrllRl,
  'HSE_IntrnlRsurc': HSE_IntrnlRsurc,
  'HSE_TgInvlvdPrts': HSE_TgInvlvdPrts,
  'HSE_TgPckngMthds': HSE_TgPckngMthds,
  'HSE_TgWstItms': HSE_TgWstItms,
  'HSE_TgWstZns': HSE_TgWstZns,
  'HSE_TgWstTrnsprtrs': HSE_TgWstTrnsprtrs,
  'HSE_TgWstTrtmntPlnts': HSE_TgWstTrtmntPlnts,
  'HSE_TgEnvMntrActvts': HSE_TgEnvMntrActvts,
  'HSE_TgEnvrnmntlMsrs': HSE_TgEnvrnmntlMsrs,
  'HSE_EQTyps': HSE_EQTyps,
  'HSE_TgAwrnsCrs': HSE_TgAwrnsCrs,
  'HSE_TgRscuEntts': HSE_TgRscuEntts,
  'HSE_TgCnfndSpcTyps': HSE_TgCnfndSpcTyps,
  'HSE_TgRscuHzrds': HSE_TgRscuHzrds,
  'HSE_TgRscuTls': HSE_TgRscuTls,
  'HSE_TgRscuCmmunctnMthds': HSE_TgRscuCmmunctnMthds,
  'HSE_TGTOOLEQUIP': HSE_TGTOOLEQUIP,
  'HSE_TgCntrctr': HSE_TgCntrctr,
  'HSE_TgActnClssfctn': HSE_TgActnClssfctn,
  'HSE_TgCarmodelBas': HSE_TgCarmodelBas,
  'HSE_TgTopEvnts': HSE_TgTopEvnts,
  'CMS_ActnRsn': CMS_ActnRsn,
  'HSE_TgWstRcvngEntry': HSE_TgWstRcvngEntry,
  'HSE_TgWstDspslEntry': HSE_TgWstDspslEntry,
  'HSE_ChmclRgstr': HSE_ChmclRgstr,
  'HSE_TgEnvMntrPlan': HSE_TgEnvMntrPlan,
  'HSE_TgEnvMntrExctn': HSE_TgEnvMntrExctn,
  'HSE_TgEnvMsrEntry': HSE_TgEnvMsrEntry, // RQ_HSE_23_3_26_21_14
  'HSE_TgEnvMntrActvtPltnt': HSE_TgEnvMntrActvtPltnt, // RQ_HSE_23_3_26_21_14
  'HSE_AspctsEntryAtEntry': HSE_AspctsEntryAtEntry,
  'HSE_AspctsRvwAtRvw': HSE_AspctsRvwAtRvw,
  'HSE_TgIncdntFlshRprt': HSE_TgIncdntFlshRprt,
  'HSE_TgIncdntFlshRprtRvew': HSE_TgIncdntFlshRprtRvew,
  'HSE_TgIncdntMdclRprt': HSE_TgIncdntMdclRprt,
  'HSE_TgIncdntMdclRprtFlwUp': HSE_TgIncdntMdclRprtFlwUp,
  'HSE_TgAcdntEnt': HSE_TgAcdntEnt,
  'HSE_TgAcdntcnfrmtn': HSE_TgAcdntcnfrmtn,
  'HSE_TgAcdntfollwUpdt': HSE_TgAcdntfollwUpdt,
  'HSE_TgIncdntInvstgtnEntry': HSE_TgIncdntInvstgtnEntry,
  'HSE_TgIncdntInvstgtnAprvl': HSE_TgIncdntInvstgtnAprvl,
  'HSE_TgrskassmntEnt': HSE_TgrskassmntEnt,
  'HSE_TgrskassmntApr': HSE_TgrskassmntApr,
  'HSE_TgRskAssmntFollwUpdt': HSE_TgRskAssmntFollwUpdt, // RQ_HSE_23_3_26_6_00
  'HSE_TgAwrnsPlnEntry': HSE_TgAwrnsPlnEntry,
  'HSE_TgAwrnsPlnAprvl': HSE_TgAwrnsPlnAprvl,
  'HSE_TgAwrnsPlnExctn': HSE_TgAwrnsPlnExctn,
  'HSE_TgRscuPlnEntry': HSE_TgRscuPlnEntry,
  'HSE_TgToolsEquipDstrbtn': HSE_TgToolsEquipDstrbtn,
  'HSE_TgPTWsrgstrEnt': HSE_TgPTWsrgstrEnt,
  'HSE_TgPTWsrgstrcnfrmtn': HSE_TgPTWsrgstrcnfrmtn,
  'HSE_TgPTWsrgstrCls': HSE_TgPTWsrgstrCls, // RQ_HSE_23_3_26_6_00
  'HSE_TgSitSrvyEnt': HSE_TgSitSrvyEnt,
  'HSE_TgSitSrvyCnfrmtn': HSE_TgSitSrvyCnfrmtn,
  'HSE_TgSitSrvyFlwup': HSE_TgSitSrvyFlwup,
  'HSE_TgSitSrvyRqrdActn': HSE_TgSitSrvyRqrdActn,
  'HSE_EQInspctnRqust': HSE_EQInspctnRqust,
  'HSE_EQInspctnRsult': HSE_EQInspctnRsult,
  'HSE_EQInspctnWrk': HSE_EQInspctnWrk,
  'HSE_DrllPlnApprvlAtApprvl': HSE_DrllPlnApprvlAtApprvl,
  'HSE_DrllExcutnAtExcutn': HSE_DrllExcutnAtExcutn,
  'HSE_AudtPlnEntry': HSE_AudtPlnEntry,
  'HSE_AudtPlnApprvl': HSE_AudtPlnApprvl,
  'HSE_AudtNtfctn': HSE_AudtNtfctn,
  'HSE_AudtExcutnRsult': HSE_AudtExcutnRsult,
  'HSE_ADTRSLTCNFRMTN': HSE_ADTRSLTCNFRMTN,
  'HSE_TgperfMsrmntEnt': HSE_TgperfMsrmntEnt,
  'HSE_TgperfMsrmntEval': HSE_TgperfMsrmntEval,
  'HSE_TgperfMsrmntflwup': HSE_TgperfMsrmntflwup,
  'HSE_TgCrEntry': HSE_TgCrEntry,
  'HSE_TgCrRvw': HSE_TgCrRvw,
  'HSE_TgCrAprvl': HSE_TgCrAprvl,
  'HSE_TgActnsEntry': HSE_TgActnsEntry,
  'HSE_TgActnsRviw': HSE_TgActnsRviw,
  'HSE_TgActnsRcvd': HSE_TgActnsRcvd,
  'HSE_TgActnsUndrExec': HSE_TgActnsUndrExec,
  'HSE_TgCrFloUp': HSE_TgCrFloUp,
  'HSE_TgCrEdtng': HSE_TgCrEdtng,
  'HSE_TgrskassmntInq': HSE_TgrskassmntInq,
  'HSE_TgperfMsrmntInq': HSE_TgperfMsrmntInq,
  'HSE_TgSitSrvyInq': HSE_TgSitSrvyInq,
  'HSE_TgIncdntFlshRprtInq': HSE_TgIncdntFlshRprtInq,
  'HSE_TgAcdntInq': HSE_TgAcdntInq,
  'HSE_TgAdtPlnInq': HSE_TgAdtPlnInq,
  'HSE_TgPTWsrgstrInq': HSE_TgPTWsrgstrInq,
  'HSE_TgcrctveAccInq': HSE_TgcrctveAccInq,
  'HSE_TGHSEPRCDRLISTINQ': HSE_TGHSEPRCDRLISTINQ,
  'HSE_EQInspctnInqry': HSE_EQInspctnInqry,
  'HSE_TgAwrnsPlnInq': HSE_TgAwrnsPlnInq,
  'HSE_TgRscuPlnInqury': HSE_TgRscuPlnInqury,
  'HSE_DRLLPLNINQ': HSE_DRLLPLNINQ,
  'HSE_TgLogsDfntn': HSE_TgLogsDfntn,
  'HSE_TgLogsEnt': HSE_TgLogsEnt,
  'ReportTypes': ReportTypes,
};

// Uppercase-keyed map for case-insensitive lookup (SCREEN_HANDLERS keys may be mixed/camel case)
const SCREEN_HANDLERS_BY_UPPERCASE = Object.fromEntries(
  Object.entries(SCREEN_HANDLERS).map(([tag, handler]) => [tag.toUpperCase(), handler])
);

/**
 * Get screen handler for screen tag, if any.
 * Lookup is case-insensitive: registered keys (camel/mixed case) are normalized to uppercase for matching.
 * @param {string} strScrTag - Screen tag (any case)
 * @returns {Object|null} Handler module or null
 */
export function getScreenHandler(strScrTag) {
  if (!strScrTag) return null;
  const normalized = strScrTag.toString().trim().toUpperCase();
  return SCREEN_HANDLERS_BY_UPPERCASE[normalized] ?? null;
}

export { SCREEN_HANDLERS };
