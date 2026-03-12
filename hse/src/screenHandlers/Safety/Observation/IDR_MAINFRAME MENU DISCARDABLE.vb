IDR_MAINFRAME MENU DISCARDABLE 
BEGIN
    POPUP "ملف"
    BEGIN
        MENUITEM "فتح مجلد التوثيق",   ID_HSE_OPN_DCMNTN_FLDR
        MENUITEM "خروج",                        ID_FILE_EXIT
    END
    POPUP "الإعدادات"
    BEGIN
        POPUP "عامة"
        BEGIN
            MENUITEM "بيانات عامة عن الشركة",             ID_HSE_COMPPRF
            MENUITEM "سياسة البرنامج",                  ID_HSE_HSEPLC
            MENUITEM "العملات",                    ID_SYSTEMSETTINGS_CURRENCY

            MENUITEM "المشروعات",                    ID_HSE_Prjcts
            MENUITEM "الدور/الوظيفة",                        ID_HSE_ROL
            MENUITEM "أسباب رفض البيانات",        ID_HSE_ENTRJCRSN
            MENUITEM "وحدات القياس",                         ID_HSE_UOM
            MENUITEM "أنواع المستندات",              ID_HSE_DOCTYP
            MENUITEM "مراقبة الملفات المرفقة",          ID_HSE_ATCHMNTFRMTS
            MENUITEM "بيانات شركة فرست",                  ID_HSE_FIRSTINFO
            POPUP "الموارد البشرية"
            BEGIN
                MENUITEM "مجموعة الأقسام",            ID_SETUP_GENERAL_DEPARTMENTGROUP

                MENUITEM "الإدارات",                  ID_HSE_DPRT
                MENUITEM "الموظفين",                    ID_HSE_EMP
                MENUITEM "الوظائف/المهن/التخصصات",                      ID_HSE_TRADES
                MENUITEM "التواقيع",                  ID_SYSTEMSETTINGS_SIGNATURES

                MENUITEM "طرق الاتصال",             ID_HSE_CNTCT_MTHDS
                MENUITEM "أنواع المواقع",                   ID_HSE_SITE_TYPE
                MENUITEM "المواقع",                        ID_HSE_STE
                MENUITEM "مناطق العمل",               ID_HSE_WRKLOC
                MENUITEM "الدول",                     ID_HSE_CNTRY
            END
        END
        POPUP "الحوادث"
        BEGIN
            MENUITEM "أنواع الحوادث",              ID_HSE_ACDNTTYP
            MENUITEM "سبب الحادث",             ID_HSE_ACDNTRSN
            MENUITEM "الأسباب المؤدية للحادث",            ID_HSE_ACDNTMDM
            MENUITEM "أنواع الخسائر",                ID_HSE_TYPLOSS
            MENUITEM "فترة الحادث",             ID_HSE_IncdntPrd
            MENUITEM "أنواع السيارات / المعدات",                ID_HSE_VCL_TYPE
            MENUITEM "استجواب حادث سيارة",  ID_HSE_VCLQSTNS
            MENUITEM "أسباب الإصابة",                ID_SYSTEMSETTINGS_INJURYTYPE

            MENUITEM "أجزاء الجسم",                  ID_HSE_HUMNBDYPRT
            MENUITEM "الحالة البيئية",       ID_SYSTEMSETTINGS_ENVIRONMENTCONDITION

            MENUITEM "معدات الوقاية الشخصية", 
                                                    ID_SYSTEMSETTINGS_PERSONALPROTECTIVEEQUIPMENT

            MENUITEM "أنواع الأصول",                  ID_HSE_AstTyp
            MENUITEM "الشدة المحتملة",          ID_SYSTEMSETTINGS_POTENTIALSEVERITY

            MENUITEM "الهيئة الطبية",           ID_HSE_MdclAuthrty
            MENUITEM "القرارات الطبية",            ID_HSE_MdclDcsn
            MENUITEM SEPARATOR
            MENUITEM "العمليات ذات الصلة",          ID_HSE_RltdOprtns
            MENUITEM "أنواع التسرب",       ID_HSE_MtrlRlsTyp
            MENUITEM "أنواع الاحتواء",            ID_HSE_CntnmntTyp
            MENUITEM "تصنيف تسرب المواد",   ID_HSE_MtrlRlsCat
            MENUITEM "المواد المسربة",           ID_HSE_RlsMtrl
            MENUITEM "تسرب المواد إلى",        ID_HSE_MtrlRlsTo
            MENUITEM "تصنيف التسرب",        ID_HSE_SplClsfctn
        END
        POPUP "حادث وشيك"
        BEGIN
            MENUITEM "أنواع الحوادث الوشيكة",            ID_HSE_NearMissTypes
            MENUITEM "قواعد الحفاظ على الحياة",           ID_HSE_LfSavngRls
            MENUITEM "أنواع التقدير للإبلاغ عن المشاكل الوشيكة",                 ID_SYSTEMSETTINGS_REWARDTYPE

            MENUITEM "الأعداد المستهدفة للإبلاغ عن المشاهدات",         ID_HSE_ObsrvtnTrgts
        END
        POPUP "المخاطر"
        BEGIN
            MENUITEM "قائمة المهام الخاصة بتقييم المخاطر", ID_HSE_TSK
            MENUITEM "خطوات تنفيذ العمل",           ID_HSE_JOB_STP_DFNTN
            MENUITEM "الآثار المترتبة",             ID_HSE_CNSQ_DFNTN
            MENUITEM "المخاطر",                      ID_HSE_HZRD
            MENUITEM "الاحتياطيات",                 ID_HSE_PRECAUTIONS
            MENUITEM "التدابير الرقابية",            ID_HSE_CTRLMSRMNT
            MENUITEM "تقييم درجة الخطورة",             ID_HSE_RskRat
            MENUITEM "مستوى المخاطر",                  ID_HSE_RskLvl
            MENUITEM "مصفوفة المخاطر",                 ID_HSE_RSKRNKDESC
            MENUITEM "تصنيف احتمالية المخاطر",           ID_HSE_RSKLKLYHODRAT
            MENUITEM "المرجع الأساسي",            ID_SYSTEMSETTINGS_BASICREFERENCE

            MENUITEM "أنواع العمليات",              ID_Hzrd_Procs
            MENUITEM "مخاطر المناطق",                ID_Area_Hzrds
            MENUITEM "أنواع المخاطر",                   ID_HSE_RskTyp
        END
        POPUP "قياس الأداء"
        BEGIN
            MENUITEM "درجات قياس الأداء",   ID_HSE_PERFMSRMNTSCOR
            MENUITEM "قائمة مراجعة قياس الأداء", 
                                                    ID_HSE_PERFMSRMNTCHKLIST
        END
        POPUP "المراجعة"
        BEGIN
            MENUITEM "معايير المراجعة",             ID_AUDITSTANDARDS
            MENUITEM "مجموعة الجوانب الخاضعة للمراجعة",                ID_SETUP_AUDITS_CLAUSEGROUP

            MENUITEM "قائمة إجراءات المراجعة",               ID_AUDITCLAUSES
            MENUITEM "المراجعين",                     ID_AUDITEE
            MENUITEM "نوع الشهادة",            ID_CERTIFICATETYPE
            MENUITEM "دور المراجعة",                  ID_AUDITROLE
            MENUITEM "مزود الشهادة",        ID_CERTIFICATEPROVIDER
            MENUITEM "مبررات المراجعة غير المخططة", 
                                                    ID_UNPLANNEDAUDITJUSTIFICATION

            MENUITEM "سبب تغيير الجدول",      ID_SCHEDULECHANGEREASON
            MENUITEM "الإشعارات الافتراضية",       ID_DEFAULTDISTRIBUTION
            MENUITEM "المراجعين المعتمدين",          ID_CERTIFIEDAUDITORS
        END
        POPUP "تصاريح العمل"
        BEGIN
            MENUITEM "أنواع تصاريح العمل",                    ID_HSE_PTWSTYP
            MENUITEM "مسؤولية تصاريح العمل",          ID_HSE_PTWSRSPNSBLTY
            MENUITEM "تعليمات الأمان",         ID_SAFETY_INSTRUCTIONS
        END
        POPUP "الانبعاثات"
        BEGIN
            MENUITEM "نوع المخلفات",                 HSE_ASPECT_TYPE
            MENUITEM "أنشطة المخلفات",             HSE_ASPECT_ACTIVITY
            MENUITEM "دورية المخلفات",            HSE_ASPECT_FREQUENCY
            MENUITEM "وحدة قياس المخلفات",                  HSE_ASPECT_UOM
            MENUITEM "حالة المخلفات",                HSE_ASPECT_STATE
        END
        POPUP "المواد الكيميائية"
        BEGIN
            MENUITEM "المخاطر الكيميائية",             HSE_CHEMICAL_HAZARD
            MENUITEM "تعريف المواد الكيميائية",   HSE_CHEMICAL_ITEMS_DEFINITION

            MENUITEM "دورية الصنف",     HSE_CHEMICAL_ITEM_FREQUENCY

        END
        POPUP "خطط الطوارئ"
        BEGIN
            MENUITEM "الأنواع الرئيسية لخطط الطوارئ",             ID_HSE_MnDrlTyp
            MENUITEM "أنواع خطة الطوارئ",           HSE_DRILL_TYPE
            MENUITEM "الدعم الخارجي",            HSE_EXTERNAL_SUPPORT
            MENUITEM "فريق إدارة خطة الطوارئ",                    HSE_EMT_LIST
            MENUITEM "فريق متابعة التجارب الوهمية",                    ID_HSE_ERTList
            MENUITEM "الدور المنوط به في خطة الطوارئ",                  HSE_DRILL_ROLE
            MENUITEM "الدعم الداخلي",           HSE_INTERNAL_RESOURCE
            MENUITEM "الجهات المعنية",             ID_HSE_INVLVDPRTS
        END
        POPUP "المخلفات"
        BEGIN
            MENUITEM "طرق التغليف والتعبئة",             ID_HSE_PckngMthds
            MENUITEM "أصناف المخلفات",                 ID_HSE_WstItms
            MENUITEM "مناطق تجميع المخلفات",                 ID_HSE_WstZns
            MENUITEM "الشركات الناقلة للمخلفات",          ID_HSE_WstTrnsprtrs
            MENUITEM "محطات معالجة المخلفات", ID_HSE_WstTrtmntPlnts
        END
        POPUP "القياسات البيئية"
        BEGIN
            MENUITEM "الأنشطة البيئية", 
                                                    ID_HSE_EnvMntrActvts
            MENUITEM "القياسات البيئية",      ID_HSE_EnvrnmntlMsrs
        END
        POPUP "تفتيش المعدات قبل دخول الموقع"
        BEGIN
            MENUITEM "أنواع المعدات",                     ID_HSE_EQTyps
        END
        POPUP "الدورات التثقيفية"
        BEGIN
            MENUITEM "الدورات التثقيفية",           ID_HSE_AwrnsCrs
            MENUITEM "درجات الحضور",             ID_SETUP_AWARENESS_ATTENDEEGRADES

        END
        POPUP "خطط الإنقاذ"
        BEGIN
            MENUITEM "جهات الإنقاذ",             ID_HSE_RscuEntts
            MENUITEM "أنواع الأماكن المغلقة",        ID_HSE_CnfndSpcTyps
            MENUITEM "مخاطر عمليات الإنقاذ",              ID_HSE_RscuHzrds
            MENUITEM "الأدوات اللازمة لعمليات الإنقاذ",                ID_HSE_RscuTls
            MENUITEM "طرق الاتصال في عمليات الإنقاذ", 
                                                    ID_HSE_RscuCmmunctnMthds
        END
        POPUP "أدوات ومعدات السلامة"
        BEGIN
            MENUITEM "أدوات ومعدات السلامة",        ID_HSE_TOOLEQUIP
        END
        POPUP "العقود"
        BEGIN
            POPUP "العقود"
            BEGIN
                MENUITEM "المقاولون",                  ID_HSE_CNTRCTR
            END
            POPUP "نماذج"
            BEGIN
                MENUITEM "غير منفذ بعد",         ID_NOT_Implemented_yet

            END
        END
        POPUP "الإجراءات التصحيحية"
        BEGIN
            MENUITEM "تصنيف مخاطر الإجراءات التصحيحية",    ID_SETUP_CAR_ACTIONCLASSIFICATION

            MENUITEM "مصادر الإجراء التصحيحي",                   ID_HSE_CARMODELBAS
            POPUP "تصنيف مخاطر الإجراءات التصحيحية"
            BEGIN
                MENUITEM "تصنيف الأحداث",                  ID_HSE_TopEvnts
            END
            MENUITEM "سبب الإجراء",               ID_CMS_ActnRsn
        END
        POPUP "الزيارات الميدانية"
        BEGIN
            MENUITEM "أنواع الزيارات الميدانية",                ID_HSE_Srvy_Typs
        END
    END
    POPUP "البيئة"
    BEGIN
        POPUP "إدارة المخلفات"
        BEGIN
            MENUITEM "تسجيل استلام المخلفات",       ID_HSE_WstRcvngEntry
            MENUITEM "تسجيل خروج المخلفات",        ID_HSE_WstDspslEntry
        END
        POPUP "سجل المواد الكيميائية"
        BEGIN
            MENUITEM "سجل المواد الكيميائية",           HSE_CHEMICAL_REGISTER
        END
        POPUP "القياسات البيئية"
        BEGIN
            MENUITEM "خطة الرقابة البيئية", ID_HSE_EnvMntrPlan
            MENUITEM "القياسات البيئية",      ID_HSE_EnvMntrExctn
        END
        POPUP "سجل الانبعاثات"
        BEGIN
            MENUITEM "تسجيل حركة المخلفات",               HSE_ASPECTS_ENTRY_ENTRY
            MENUITEM "مراجعة حركة المخلفات",              HSE_ASPECTS_REVIEW_REVIEW

        END
    END
    POPUP "السلامة"
    BEGIN
        POPUP "حادث وشيك"
        BEGIN
            MENUITEM "تسجيل بيانات المشاهدات",           ID_HSE_NRSTMISCENT
            MENUITEM "مراجعة بيانات المشاهدات",          ID_HSE_NRSTMISCCNFRMTN
            MENUITEM "اعتماد بيانات المشاهدات",        ID_HSE_NRSTMISCFLWUP
            MENUITEM "مكافآت الإبلاغ عن مشكلة وشيكة",  ID_NEARMISS_NEARMISSREWARD

        END
        POPUP "الحوادث"
        BEGIN
            MENUITEM "تسجيل تقرير سريع عن الحادث",        ID_HSE_IncdntFlshRprt
            MENUITEM "مراجعة تقرير سريع عن الحادث",       ID_HSE_IncdntFlshRprtRvw
            MENUITEM SEPARATOR
            MENUITEM "التقرير الطبي عن الحادث", ID_HSE_IncdntMdclRprt
            MENUITEM "مراجعة التقرير الطبي عن الحادث", 
                                                    ID_HSE_IncdntMdclRprtFlwUp

            MENUITEM SEPARATOR
            MENUITEM "تسجيل سجل الحادث",            ID_HSE_ACDNTENT
            MENUITEM "اعتماد سجل الحادث",           ID_HSE_ACDNTCNFRMTN
            MENUITEM "مراجعة سجل الحادث",           ID_HSE_ACDNTFOLLWUPDT
            MENUITEM SEPARATOR
            MENUITEM "تسجيل بيانات تحليل الحادث", 
                                                    ID_HSE_AcdntIncdntInvstgtnEntry

            MENUITEM "اعتماد تحليل الحادث", 
                                                    ID_HSE_AcdntIncdntInvstgtnAprv

        END
        POPUP "تقييم المخاطر"
        BEGIN
            MENUITEM "تسجيل بيانات تقييم المخاطر",       ID_HSE_RSKASSMNTENT
            MENUITEM "تأكيد بيانات تقييم المخاطر", ID_HSE_RSKASSMNTAPR
        END
        POPUP "الدورات التثقيفية"
        BEGIN
            MENUITEM "تسجيل خطة الدورات التثقيفية",        ID_HSE_AwrnsPlnEntry
            MENUITEM "اعتماد خطة الدورات التثقيفية",     ID_HSE_AwrnsPlnAprvl
            MENUITEM "تنفيذ خطة الدورات التثقيفية",    ID_HSE_AwrnsPlnExctn
        END
        POPUP "خطط الإنقاذ"
        BEGIN
            MENUITEM "تسجيل خطة الإنقاذ",           ID_HSE_RscuPlnEntry
        END
        MENUITEM "سجل أدوات ومعدات السلامة",                ID_HSE_TOOLSEQUIPDSTRBTN
        POPUP "تصاريح العمل"
        BEGIN
            MENUITEM "تسجيل تصريح العمل",          ID_HSE_PTWSRGSTRENT
            MENUITEM "تأكيد تصريح العمل",   ID_HSE_PTWSRGSTRCNFRMTN
            MENUITEM "إغلاق تصريح العمل",        ID_HSE_PTWSRGSTRCLS
        END
    END
    POPUP "قياس الأداء"
    BEGIN
        POPUP "الزيارات الميدانية"
        BEGIN
            MENUITEM "تسجيل بيانات الزيارة الميدانية",           ID_HSE_SitSrvyEnt
            MENUITEM "تأكيد بيانات الزيارة الميدانية",    ID_HSE_SitSrvyCnfrmtn
            MENUITEM "متابعة سجلات الزيارة الميدانية",    ID_HSE_SitSrvyFlwup
        END
        POPUP "تفتيش المعدات"
        BEGIN
            MENUITEM "طلب التفتيش على معدة",       ID_HSE_EQInspctnRqust
            MENUITEM "نتيجة التفتيش على معدة",       ID_HSE_EQInspctnRsult
            MENUITEM "المعدات الموجودة داخل الموقع",                  ID_HSE_EQInspctnWrk
        END
        POPUP "خطط الطوارئ"
        BEGIN
            MENUITEM "تسجيل خطط الطوارئ",            HSE_DRILL_PLAN_ENTRY_ENTRY

            MENUITEM "اعتماد خطة الطوارئ",         HSE_DRILL_PLAN_APPROVAL_APPROVAL

            MENUITEM "تنفيذ خطة الطوارئ",             HSE_DRILL_EXECUTION_EXECUTION

        END
        POPUP "المراجعة"
        BEGIN
            MENUITEM "إعداد خطة المراجعة",            ID_AUDITPLANENTRY
            MENUITEM "اعتماد خطط المراجعة",         ID_AUDITPLANAPPROVAL
            MENUITEM "إشعار المراجعة",          ID_AUDITNOTIFICATION
            MENUITEM "تسجيل نتيجة المراجعة",          ID_AUDITEXECUTIONRESULT
            MENUITEM "تأكيد نتيجة المراجعة",   ID_PERFORMANCE_AUDITING_AUDITRESULTCONFIRMATION

        END
        POPUP "قياس الأداء"
        BEGIN
            MENUITEM "تسجيل زيارة قياس الأداء", ID_HSE_PERFMSRMNTENT
            MENUITEM "تقييم زيارة قياس الأداء", 
                                                    ID_HSE_PERFMSRMNTEVAL
            MENUITEM "متابعة زيارات قياس الأداء", 
                                                    ID_HSE_PERFMSRMNTFLWUP
        END
        POPUP "متابعة الإجراءات"
        BEGIN
            MENUITEM "تسجيل الإجراء التصحيحي",                   ID_NEWCARMODULE_CARENTRY
            MENUITEM "مراجعة الإجراء التصحيحي",                  ID_NEWCARMODULE_CARREVIEW

            MENUITEM "اعتماد الإجراء التصحيحي",                ID_NEWCARMODULE_CARAPPROVAL

            MENUITEM "",                            1
            MENUITEM "تسجيل الإجراءات",               ID_NEWCARMODULE_ACTIONSENTRY

            MENUITEM "مراجعة الإجراءات",              ID_NEWCARMODULE_ACTIONSREVIEW

            MENUITEM "استلام الإجراءات التصحيحية",            ID_NEWCARMODULE_ACTIONSRECEIVED

            MENUITEM "الإجراءات التصحيحية تحت التنفيذ",     ID_NEWCARMODULE_ACTIONSUNDEREXECUTION

            MENUITEM "",                            2
            MENUITEM "متابعة الإجراء التصحيحي",               ID_NEWCARMODULE_CARFOLLOWUP

            MENUITEM "تعديل الإجراء التصحيحي",                 ID_PERFORMANCE_ACTIONTRACKING_CAREDITING

        END
    END
    POPUP "شاشات الاستعلام"
    BEGIN
        MENUITEM "استعلام عن بيانات تقييم المخاطر",     ID_HSE_RSKASSMNTINQ
        MENUITEM "استعلام عن زيارات قياس الأداء", ID_HSE_PERFMSRMNTINQ
        MENUITEM "استعلام عن بيانات الزيارة الميدانية",         ID_HSE_SITSRVYINQ
        MENUITEM "استعلام عن بيانات المشاهدات",         ID_HSE_NRSTMISCINQ
        MENUITEM "استعلام عن التقارير السريعة عن الحوادث",      ID_HSE_IncdntFlshRprtInq
        MENUITEM "استعلام عن سجل الحادث",           ID_HSE_ACDNTINQ
        MENUITEM "استعلام عن خطة المراجعة",          ID_HSE_ADTPLNINQ
        MENUITEM "استعلام عن تصريح العمل",        ID_HSE_PPTWINQ
        MENUITEM "استعلام عن الإجراءات التصحيحية",   ID_HSE_CRCTVEACCINQ
        MENUITEM "استعلام عن إجراءات المراجعة", ID_HSE_HSEPROCDRSLSTINQ
        MENUITEM "استعلام عن التفتيش على المعدات",       ID_HSE_EQInspctnInqry
        MENUITEM "استعلام عن خطة الدورات التثقيفية",      ID_HSE_AwrnsPlnInq
        MENUITEM "استعلام عن خطط الإنقاذ",         ID_HSE_RscuPlnInqury
        MENUITEM "استعلام عن خطط الطوارئ",               ID_Drill_Inquiry
        MENUITEM "استعلام حالة الحادثة الصحية",    ID_INQUIRYSCREENS_INCIDENTMEDICALINQUIRY

    END
    POPUP "سجلات إضافية"
    BEGIN
        MENUITEM "تعريف السجلات الإضافية",             ID_HSE_LOGS_DEF
        MENUITEM "إدخال بيانات السجلات الإضافية",                  ID_HSE_LOGS_ENT
        POPUP "تسليم المهام"
        BEGIN
            MENUITEM "نوع الموضوع",                ID_ADVANCED_HANDOVERMODULE_SUBJECTTYPE

            MENUITEM "تسليم مهام مفتوح",            ID_ADVANCED_HANDOVERMODULE_OPENEDHANDOVER

            MENUITEM "تسليم مهام مغلق",             ID_ADVANCED_HANDOVERMODULE_CLOSEDHANDOVER

        END
    END
    POPUP "التقارير"
    BEGIN
        MENUITEM "مجموعات التقارير                Alt+F11", 
                                                ID_ANALYSIS_REPORTTYPES
        MENUITEM "&التقارير                     Ctrl+F11", 
                                                ID_ANALYSIS_REPORTS
        MENUITEM "معلومات التقارير",                ID_Reports_Info
        POPUP "التقارير الدورية"
        BEGIN
            MENUITEM "التقرير الشهري",              ID_Monthly_Reports
        END
        POPUP "تقارير أخرى"
        BEGIN
            MENUITEM "تقرير IFC",                  ID_ADVANCEDREPORTS_IFCREPORT
            , GRAYED
            MENUITEM "التقارير الشهرية EGPC",        ID_ADVANCEDREPORTS_EGPCMONTHLYREPORTS
            , GRAYED
            MENUITEM "تقرير TG-KPI",               ID_ADVANCEDREPORTS_TGKPIREPORT
            , GRAYED
            MENUITEM "النماذج السريعة",                 ID_ADVANCEDREPORTS_FLASHFORMS
            , GRAYED
        END
    END
    POPUP "إدارة البرنامج"
    BEGIN
        MENUITEM "English",                      ID_UTILITIES_ARABIC
        POPUP "صلاحيات المستخدمين"
        BEGIN
            MENUITEM "إعداد التطبيق",           ID_UTILITIES_APPLICATIONSETUP

            MENUITEM "مستخدمي التطبيق",           ID_UTILITIES_PRIVILAGE_ADDUSERPRIVILAGE

            MENUITEM "صلاحية القائمة",              ID_UTILITIES_MENUPRIVILEDGE

            MENUITEM "صلاحية الحقول",            ID_UTILITIES_FIELDSPRIVILEGE

            MENUITEM "صلاحية التقارير",           ID_ANALYSIS_REPORTSPRIVELEGE

        END
        MENUITEM "الترميز التلقائي للنظام",     ID_UTILITIES_CODESTRUCTRE
        MENUITEM "الفحوصات المعرفة بالمستخدم",         ID_UTILITIES_USERDEFINEDCHECKS

        POPUP "إعدادات الإيميل"
        BEGIN
            MENUITEM "إظهار إعدادات خادم البريد",    ID_UTILITIES_EMAILING_SHOWMAILSERVERSETTING

            MENUITEM "إظهار معالج البريد الإلكتروني",          ID_UTILITIES_EMAILING_SHOWEMAILWIZARD

        END
    END
    POPUP "&النافذة"
    BEGIN
        MENUITEM "&ترتيب متتابع",                    ID_WINDOW_CASCADE_NEW
        MENUITEM "&ترتيب أفقي",            ID_WINDOW_TILE_HORZ_NEW
        MENUITEM "ترتيب &عمودي",              ID_WINDOW_TILE_VERT_NEW
        MENUITEM SEPARATOR
        MENUITEM "إخفاء شريط القوائم       Ctrl+Alt+M", 
                                                ID_WINDOW_SHOWTOOLBARS
        MENUITEM "إخفاء شريط البحث         Ctrl+Alt+S", ID_HIDE_SEARCH_BAR
        MENUITEM "تخصيص القائمة",              ID_FILE_CUSTOMIZELIST
        MENUITEM SEPARATOR
        MENUITEM "&إغلاق الكل",                  ID_WINDOW_CLOSEALL
        MENUITEM SEPARATOR
    END
    POPUP "&المساعدة"
    BEGIN
        MENUITEM "&حول HSEMS...",             ID_APP_ABOUT
        MENUITEM "المحتويات               F1",   ID_HELP_CONTENTS
    END
END
