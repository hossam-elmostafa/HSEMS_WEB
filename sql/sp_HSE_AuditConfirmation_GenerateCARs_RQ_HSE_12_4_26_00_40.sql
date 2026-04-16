/*
  Audit Result Confirmation → HSE_CRENTRY from clause-level findings (CAR required).
  Port of CAuditModuleCategory::AuditConfirmation_AddRecordToCAREntry (HSEMS.DLL).
  RQ_HSE_12_4_26_00_40 — gap R6 / A17.

  Web calls: EXEC dbo.sp_HSE_AuditConfirmation_GenerateCARs_RQ_HSE_12_4_26_00_40 @AuditsTabPrmKy, @UserLogin

  ASSUMPTIONS (verify in target DB):
  - dbo.HSE_ADTRSLTCNFRMTN_AUDTS.MAINLINK → dbo.HSE_ADTRSLTCNFRMTN.PrmKy
  - dbo.HSE_CLUS_LVLRSULTPPUP.MainLink = dbo.HSE_ADTRSLTCNFRMTN_AUDTS.PrmKy
  - dbo.HSE_CLUS_LVLRSULTPPUP_FNDNGS.ACTCMP = 'Y' means “CAR required” (legacy filter)
  - Column names on HSE_CRENTRY match insert list (rename if your ERP uses HSE_CrEntry / aliases)
  - dbo.CopyImages exists with same signature as desktop
*/

SET NOCOUNT ON;
GO

IF OBJECT_ID(N'dbo.sp_HSE_AuditConfirmation_GenerateCARs_RQ_HSE_12_4_26_00_40', N'P') IS NOT NULL
  DROP PROCEDURE dbo.sp_HSE_AuditConfirmation_GenerateCARs_RQ_HSE_12_4_26_00_40;
GO

CREATE PROCEDURE dbo.sp_HSE_AuditConfirmation_GenerateCARs_RQ_HSE_12_4_26_00_40
  @AuditsTabPrmKy INT,
  @UserLogin      NVARCHAR(260) = NULL
AS
BEGIN
  SET NOCOUNT ON;
  SET XACT_ABORT ON;

  IF @AuditsTabPrmKy IS NULL OR @AuditsTabPrmKy <= 0
  BEGIN
    RAISERROR(N'RQ_HSE_12_4_26_00_40: @AuditsTabPrmKy is required.', 16, 1);
    RETURN;
  END;

  IF OBJECT_ID(N'dbo.HSE_CRENTRY', N'U') IS NULL
  BEGIN
    RAISERROR(N'RQ_HSE_12_4_26_00_40: dbo.HSE_CRENTRY not found. Create synonym or change procedure to your CAR table name.', 16, 1);
    RETURN;
  END;

  DECLARE @HdrPrmKy INT;
  SELECT TOP 1 @HdrPrmKy = MAINLINK
  FROM dbo.HSE_ADTRSLTCNFRMTN_AUDTS WITH (NOLOCK)
  WHERE PrmKy = @AuditsTabPrmKy;

  IF @HdrPrmKy IS NULL
  BEGIN
    RAISERROR(N'RQ_HSE_12_4_26_00_40: HSE_ADTRSLTCNFRMTN_AUDTS.MAINLINK not found for PrmKy.', 16, 1);
    RETURN;
  END;

  DECLARE @AdtYr NVARCHAR(20);
  DECLARE @PlnN NVARCHAR(50);
  SELECT TOP 1
    @AdtYr = NULLIF(LTRIM(RTRIM(AUDTPLNENTRY_AUDTYR)), N''),
    @PlnN  = NULLIF(LTRIM(RTRIM(AUDTPLNENTRY_PLNN)), N'')
  FROM dbo.HSE_ADTRSLTCNFRMTN WITH (NOLOCK)
  WHERE PrmKy = @HdrPrmKy;

  DECLARE @AudtUnt NVARCHAR(50);
  DECLARE @AuditNo NVARCHAR(50);
  DECLARE @Prj NVARCHAR(200);
  SELECT TOP 1
    @AudtUnt = NULLIF(LTRIM(RTRIM(AudtPlnEntry_Audts_AudtUnt)), N''),
    @AuditNo = NULLIF(LTRIM(RTRIM(AUDTPLNENTRY_AUDTS_AUDTN)), N''),
    @Prj     = NULLIF(LTRIM(RTRIM(AUDTPLNENTRY_AUDTS_PRJCT)), N'')
  FROM dbo.HSE_ADTRSLTCNFRMTN_AUDTS WITH (NOLOCK)
  WHERE PrmKy = @AuditsTabPrmKy;

  DECLARE @ConcDep NVARCHAR(200) = NULL;
  IF @AudtUnt IS NOT NULL AND OBJECT_ID(N'dbo.HSE_Audt_AudtUnt', N'U') IS NOT NULL
  BEGIN
    SELECT TOP 1 @ConcDep = NULLIF(LTRIM(RTRIM(Audt_AudtUnt_Dprtmnt)), N'')
    FROM dbo.HSE_Audt_AudtUnt WITH (NOLOCK)
    WHERE TRY_CAST(PRMKY AS INT) = TRY_CAST(@AudtUnt AS INT);
  END;

  DECLARE @CARStatus NVARCHAR(10);
  DECLARE @CARSrc NVARCHAR(200);
  SELECT TOP 1
    @CARStatus = NULLIF(LTRIM(RTRIM(HSEPLC_ADT_GNRCRSTT)), N''),
    @CARSrc    = NULLIF(LTRIM(RTRIM(HSEPLC_ADT_CRBSS)), N'')
  FROM dbo.HSE_HSEPLC_ADT WITH (NOLOCK);

  IF LEN(ISNULL(@CARStatus, N'')) < 2 AND @CARStatus IS NOT NULL
    SET @CARStatus = RIGHT(N'00' + @CARStatus, 2);

  DECLARE @OwnerDep NVARCHAR(200);
  SELECT TOP 1 @OwnerDep = NULLIF(LTRIM(RTRIM(HSEPLC_OWNRDPRT)), N'')
  FROM dbo.HSE_HSEPLC WITH (NOLOCK);

  DECLARE @EmpCode NVARCHAR(200) = N'';
  IF @UserLogin IS NOT NULL AND OBJECT_ID(N'dbo.CMN_EMPLOYEE', N'U') IS NOT NULL
  BEGIN
    SELECT TOP 1 @EmpCode = ISNULL(
      NULLIF(LTRIM(RTRIM(EMPLOYEE_USRID)), N''),
      NULLIF(LTRIM(RTRIM(EMPLOYEE_LOGINNAME)), N'')
    )
    FROM dbo.CMN_EMPLOYEE WITH (NOLOCK)
    WHERE EMPLOYEE_USRID = @UserLogin OR EMPLOYEE_LOGINNAME = @UserLogin;
  END;
  IF @EmpCode = N'' SET @EmpCode = LEFT(ISNULL(@UserLogin, N''), 200);

  DECLARE @Cryr NVARCHAR(4) = CAST(YEAR(GETDATE()) AS NVARCHAR(4));
  DECLARE @CrDt DATE = CAST(GETDATE() AS DATE);

  DECLARE @MaxCrn INT = ISNULL((
    SELECT MAX(TRY_CAST(CRENTRY_CRN AS INT))
    FROM dbo.HSE_CRENTRY WITH (UPDLOCK, HOLDLOCK)
    WHERE LTRIM(RTRIM(CRENTRY_CRYR)) = @Cryr
  ), 0);

  DECLARE @MaxSrl INT = ISNULL((
    SELECT MAX(TRY_CAST(CRENTRY_CRSRLN AS INT))
    FROM dbo.HSE_CRENTRY WITH (UPDLOCK, HOLDLOCK)
    WHERE LTRIM(RTRIM(CRENTRY_CRYR)) = @Cryr
      AND LTRIM(RTRIM(CRENTRY_CRSRC)) = LTRIM(RTRIM(@CARSrc))
  ), 0);

  /* Temp context for CAR triggers (web parity) */
  IF OBJECT_ID(N'tempdb..##TEMP_HSE_TABLE') IS NULL
  BEGIN
    CREATE TABLE ##TEMP_HSE_TABLE (
      KEYrec   VARCHAR(50) NOT NULL,
      VALUErec VARCHAR(50) NULL,
      CONSTRAINT PK_TEMP_HSE_TABLE_RQ124 PRIMARY KEY CLUSTERED (KEYrec)
    );
  END;
  DELETE FROM ##TEMP_HSE_TABLE;
  INSERT INTO ##TEMP_HSE_TABLE(KEYrec, VALUErec) VALUES (N'ActionDescription', N'Creating new CAR');
  INSERT INTO ##TEMP_HSE_TABLE(KEYrec, VALUErec) VALUES (N'SrcScreen', N'Audit Result Confirmation');
  IF @UserLogin IS NOT NULL
    INSERT INTO ##TEMP_HSE_TABLE(KEYrec, VALUErec) VALUES (N'UserID', LEFT(@UserLogin, 50));

  DECLARE @Ins TABLE (FndPk INT NOT NULL);

  ;WITH num AS (
    SELECT
      f.PrmKy AS FndPk,
      ROW_NUMBER() OVER (ORDER BY f.PrmKy) AS rn
    FROM dbo.hse_CLUS_LVLRSULTPPUP_FNDNGS AS f WITH (NOLOCK)
    WHERE f.MainLink IN (
        SELECT clu.PrmKy
        FROM dbo.HSE_CLUS_LVLRSULTPPUP AS clu WITH (NOLOCK)
        WHERE clu.MainLink = @AuditsTabPrmKy
      )
      AND f.CLUS_LVLRSULTPPUP_FNDNGS_ACTCMP = N'Y'
  )
  INSERT INTO dbo.HSE_CRENTRY (
    CRENTRY_CLUS_LVLRSULTPPUP_FNDNGS_PK,
    CRENTRY_CRDT,
    CRENTRY_CRYR,
    CRENTRY_CRPRR,
    CRENTRY_ATGNR,
    CRENTRY_CRSTT,
    CRENTRY_DPR,
    CRENTRY_CRSRC,
    CRENTRY_ST,
    CRENTRY_LCT,
    CRENTRY_AR,
    CRENTRY_EXCAR,
    CRENTRY_NCDSC,
    CRENTRY_EVD,
    CRENTRY_CNCDPR,
    CRENTRY_PRJ,
    CRENTRY_DTLN,
    CRENTRY_TXNYR,
    CRENTRY_TXNN,
    CRENTRY_CRN,
    CRENTRY_CRSRLN,
    CRENTRY_NM
  )
  OUTPUT inserted.CRENTRY_CLUS_LVLRSULTPPUP_FNDNGS_PK INTO @Ins (FndPk)
  SELECT
    f.PrmKy,
    @CrDt,
    @Cryr,
    N'M',
    N'Y',
    @CARStatus,
    @OwnerDep,
    @CARSrc,
    f.CLUS_LVLRSULTPPUP_FNDNGS_ST,
    f.CLUS_LVLRSULTPPUP_FNDNGS_LCT,
    f.CLUS_LVLRSULTPPUP_FNDNGS_AR,
    f.CLUS_LVLRSULTPPUP_FNDNGS_EXCAR,
    f.CLUS_LVLRSULTPPUP_FNDNGS_FNDNGDSCRPTN,
    f.CLUS_LVLRSULTPPUP_FNDNGS_EVDNCS,
    @ConcDep,
    @Prj,
    @AuditNo,
    @AdtYr,
    @PlnN,
    CAST(@MaxCrn + num.rn AS NVARCHAR(20)),
    CAST(@MaxSrl + num.rn AS NVARCHAR(20)),
    @EmpCode
  FROM dbo.hse_CLUS_LVLRSULTPPUP_FNDNGS AS f WITH (NOLOCK)
  INNER JOIN num ON num.FndPk = f.PrmKy;

  DECLARE @rc INT = @@ROWCOUNT;
  IF @rc = 0
    RETURN;

  /* Evidence photos — dbo.CopyImages (same as desktop) */
  DECLARE @fp INT;
  DECLARE c CURSOR LOCAL FAST_FORWARD FOR SELECT FndPk FROM @Ins;
  OPEN c;
  FETCH NEXT FROM c INTO @fp;
  WHILE @@FETCH_STATUS = 0
  BEGIN
    IF OBJECT_ID(N'dbo.CopyImages', N'P') IS NOT NULL
    BEGIN
      BEGIN TRY
        EXEC dbo.CopyImages
          N'HSEMS',
          N'HSE_CLUS_LVLRSULTPPUP_FNDNGS',
          N'PrmKy',
          CAST(@fp AS NVARCHAR(32)),
          N'CLUS_LVLRSULTPPUP_FNDNGS_EVDNCPHTS',
          N'HSE_CRENTRY',
          N'CRENTRY_CLUS_LVLRSULTPPUP_FNDNGS_PK',
          CAST(@fp AS NVARCHAR(32)),
          N'CRENTRY_NCPHT';
      END TRY
      BEGIN CATCH
        /* RQ_HSE_12_4_26_00_40 — do not fail whole batch if image copy unsupported */
      END CATCH;
    END;
    FETCH NEXT FROM c INTO @fp;
  END;
  CLOSE c;
  DEALLOCATE c;
END;
GO
