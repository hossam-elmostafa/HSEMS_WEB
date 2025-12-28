-- SQL Script to Create a Test CAR Record with Auto Generated = 'Yes'
-- This will allow testing of the "View Source TXN" button
-- 
-- Usage: Execute this script in your SQL Server database
-- Make sure to adjust the values as needed for your test environment
--
-- NOTE: If you get "Invalid object name 'HSE_CRENTRY'", run find_table_name.sql
-- to find the correct table name, then update all references below.

-- First, let's check if there's an existing Observation or Audit record we can reference
-- You can modify the WHERE clause to use a different source transaction

DECLARE @CurrentYear INT = YEAR(GETDATE());
DECLARE @CurrentDate DATETIME = GETDATE();
DECLARE @TestCARNumber INT;
DECLARE @TestCARSerial INT = 1;
DECLARE @SourceTXNYear INT = @CurrentYear;
DECLARE @SourceTXNNumber INT = 1; -- Change this to match an existing Observation/Audit number
-- Based on actual column sizes:
-- CRENTRY_CRSRC: varchar(10) - MAX 10 chars!
-- CRENTRY_CRSTT: nvarchar(2) - MAX 2 chars!
-- CRENTRY_DPR: varchar(50) - MAX 50 chars
-- CRENTRY_NM: varchar(50) - MAX 50 chars
-- 
-- Actual CAR Source values in your database:
-- 'AUDIT' (5 chars), 'OBSRVATION' (10 chars), 'SITE-SURVY' (10 chars)

DECLARE @CARSource VARCHAR(10) = 'OBSRVATION'; -- Options: 'AUDIT' (5), 'OBSRVATION' (10), 'SITE-SURVY' (10)
DECLARE @Department VARCHAR(50); -- Max 50 chars
DECLARE @EmployeeCode VARCHAR(50); -- Max 50 chars
DECLARE @CARStatus NVARCHAR(2) = '1'; -- Max 2 chars (e.g., '1', '01', '16')
DECLARE @AreaID INT; -- Area ID (REQUIRED - must exist in HSE_WRKLOCWRKLOC table)
DECLARE @LocationID VARCHAR(50); -- Location ID (REQUIRED - VARCHAR, not INT! Must exist in HSE_WRKLOC.WRKLOC_LOCTYP)
DECLARE @SiteID VARCHAR(50); -- Site ID (REQUIRED - VARCHAR, not INT! Must exist in HSE_STE table)

-- CRITICAL: Get Area, Site, and Location from the SAME existing CAR record
-- This ensures they are all compatible with each other and satisfy all foreign key constraints
SELECT TOP 1 
    @AreaID = CRENTRY_AR,
    @SiteID = CRENTRY_ST,
    @LocationID = CRENTRY_LCT
FROM dbo.HSE_CRENTRY
WHERE CRENTRY_AR IS NOT NULL
  AND CRENTRY_ST IS NOT NULL
  AND CRENTRY_LCT IS NOT NULL
  AND EXISTS (SELECT 1 FROM dbo.HSE_WRKLOCWRKLOC WHERE WRKLOCWRKLOC_MAINLINK = CRENTRY_AR)
  AND EXISTS (SELECT 1 FROM dbo.HSE_STE WHERE CAST(MAINLINK AS VARCHAR(50)) = CRENTRY_ST)
  AND EXISTS (SELECT 1 FROM dbo.HSE_WRKLOC WHERE WRKLOC_LOCTYP = CRENTRY_LCT)
ORDER BY PrmKy DESC;

-- If not found, try to get them separately but ensure compatibility
IF @AreaID IS NULL OR @SiteID IS NULL OR @LocationID IS NULL
BEGIN
    -- Get Area ID
    SELECT TOP 1 @AreaID = WRKLOCWRKLOC_MAINLINK
    FROM dbo.HSE_WRKLOCWRKLOC
    WHERE WRKLOCWRKLOC_MAINLINK IS NOT NULL
    ORDER BY WRKLOCWRKLOC_MAINLINK;
    
    -- Get Site ID
    SELECT TOP 1 @SiteID = CAST(MAINLINK AS VARCHAR(50))
    FROM dbo.HSE_STE
    WHERE MAINLINK IS NOT NULL
    ORDER BY MAINLINK;
    
    -- Get Location ID that works with the Area (from a CAR record that uses this Area)
    SELECT TOP 1 @LocationID = CRENTRY_LCT
    FROM dbo.HSE_CRENTRY
    WHERE CRENTRY_LCT IS NOT NULL 
      AND CRENTRY_AR = @AreaID
      AND EXISTS (SELECT 1 FROM dbo.HSE_WRKLOC WHERE WRKLOC_LOCTYP = CRENTRY_LCT)
    ORDER BY PrmKy DESC;
END

-- Validate all IDs
IF @AreaID IS NULL
BEGIN
    PRINT 'ERROR: No valid Area ID found.';
    RETURN;
END

IF @SiteID IS NULL OR @SiteID = ''
BEGIN
    PRINT 'ERROR: No valid Site ID found.';
    RETURN;
END

IF @LocationID IS NULL OR @LocationID = ''
BEGIN
    PRINT 'ERROR: No valid Location ID found that works with Area ' + CAST(@AreaID AS VARCHAR(10)) + '.';
    RETURN;
END

-- Final validation: Ensure all IDs exist in their respective tables
IF NOT EXISTS (SELECT 1 FROM dbo.HSE_WRKLOCWRKLOC WHERE WRKLOCWRKLOC_MAINLINK = @AreaID)
BEGIN
    PRINT 'ERROR: Area ID ' + CAST(@AreaID AS VARCHAR(10)) + ' does not exist in HSE_WRKLOCWRKLOC.';
    RETURN;
END

IF NOT EXISTS (SELECT 1 FROM dbo.HSE_STE WHERE CAST(MAINLINK AS VARCHAR(50)) = @SiteID)
BEGIN
    PRINT 'ERROR: Site ID [' + @SiteID + '] does not exist in HSE_STE.';
    RETURN;
END

IF NOT EXISTS (SELECT 1 FROM dbo.HSE_WRKLOC WHERE WRKLOC_LOCTYP = @LocationID)
BEGIN
    PRINT 'ERROR: Location ID [' + @LocationID + '] does not exist in HSE_WRKLOC.WRKLOC_LOCTYP.';
    RETURN;
END

PRINT 'Using Area ID: ' + CAST(@AreaID AS VARCHAR(10));
PRINT 'Using Site ID: [' + @SiteID + ']';
PRINT 'Using Location ID: [' + @LocationID + ']';
PRINT '(All IDs retrieved from same CAR record to ensure compatibility)';

-- Site ID already retrieved above, no need to retrieve again

-- Get department and employee (max 50 chars each)
SELECT TOP 1 @Department = LEFT(LTRIM(RTRIM(DEPART_CODE)), 50) 
FROM CMN_DEPART 
WHERE ACTIVE_REC = 'Y' 
ORDER BY DEPART_CODE;

SELECT TOP 1 @EmployeeCode = LEFT(LTRIM(RTRIM(EMPLOYEE_CODE)), 50) 
FROM CMN_EMPLOYEE 
WHERE ACTIVE_REC = 'Y' 
ORDER BY EMPLOYEE_CODE;

-- Set defaults if not found
IF @Department IS NULL OR @Department = '' SET @Department = 'DEFAULT';
IF @EmployeeCode IS NULL OR @EmployeeCode = '' SET @EmployeeCode = 'ADMIN';

-- Truncate to actual column sizes
SET @Department = LEFT(LTRIM(RTRIM(@Department)), 50);
SET @EmployeeCode = LEFT(LTRIM(RTRIM(@EmployeeCode)), 50);
SET @CARSource = LEFT(LTRIM(RTRIM(@CARSource)), 10); -- CRITICAL: Max 10 chars!
SET @CARStatus = LEFT(LTRIM(RTRIM(@CARStatus)), 2); -- CRITICAL: Max 2 chars!

-- Get the next CAR number for the current year
-- Try dbo.HSE_CRENTRY if HSE_CRENTRY doesn't work, or check find_table_name.sql for correct name
SELECT @TestCARNumber = ISNULL(MAX(CRENTRY_CRN) + 1, 1) 
FROM dbo.HSE_CRENTRY 
WHERE CRENTRY_CRYR = @CurrentYear;

-- Get the next CAR serial number for the current year and source
SELECT @TestCARSerial = ISNULL(MAX(CRENTRY_CRSRLN) + 1, 1)
FROM dbo.HSE_CRENTRY
WHERE CRENTRY_CRYR = @CurrentYear 
  AND CRENTRY_CRSRC = @CARSource;

-- Create the temporary table that the After_Insert trigger expects
-- This is required by the system's tracing mechanism
IF OBJECT_ID('tempdb..##TEMP_HSE_TABLE') IS NULL 
    CREATE TABLE ##TEMP_HSE_TABLE(
        KEYrec VARCHAR(50) NOT NULL,
        VALUErec VARCHAR(50),
        CONSTRAINT PK_TEMP_HSE_TABLE PRIMARY KEY CLUSTERED (KEYrec)
    );

-- Clear any existing data
DELETE FROM ##TEMP_HSE_TABLE;

-- Insert required tracing information (as per C++ code)
INSERT INTO ##TEMP_HSE_TABLE(KEYrec, VALUErec) VALUES('ActionDescription', 'Creating new CAR');
INSERT INTO ##TEMP_HSE_TABLE(KEYrec, VALUErec) VALUES('SrcScreen', 'CAR Entry');

-- Insert test CAR record
-- Based on C++ code pattern from NearMissFollowUpCategory.cpp and AuditModuleCategory.cpp
-- If you get "Invalid object name", try: dbo.HSE_CRENTRY or run find_table_name.sql
INSERT INTO dbo.HSE_CRENTRY (
    CRENTRY_CRDT,           -- CAR Date
    CRENTRY_CRYR,           -- CAR Year
    CRENTRY_CRN,            -- CAR Number
    CRENTRY_CRSTT,          -- CAR Status
    CRENTRY_DPR,            -- Department
    CRENTRY_NM,             -- Employee Number
    CRENTRY_NCDSC,          -- NC Description (REQUIRED)
    CRENTRY_EVD,            -- Evidence (REQUIRED)
    CRENTRY_CRPRR,          -- CAR Priority (M = Medium)
    CRENTRY_ST,             -- Site (REQUIRED)
    CRENTRY_LCT,            -- Location (REQUIRED)
    CRENTRY_AR,             -- Area (REQUIRED)
    CRENTRY_ATGNR,          -- Auto Generated (Y = Yes)
    CRENTRY_CRSRC,          -- CAR Source (e.g., 'OBSRVATION', 'AUDIT')
    CRENTRY_CRSRLN,         -- CAR Serial Number
    CRENTRY_TXNYR,          -- Transaction Year (from source)
    CRENTRY_TXNN            -- Transaction Number (from source) - Note: TXNN not TXNNUM
)
VALUES (
    @CurrentDate,           -- CRENTRY_CRDT
    @CurrentYear,           -- CRENTRY_CRYR
    @TestCARNumber,         -- CRENTRY_CRN
    @CARStatus,             -- CRENTRY_CRSTT (Status)
    @Department,            -- CRENTRY_DPR
    @EmployeeCode,           -- CRENTRY_NM
    'Test CAR - NC Description',  -- CRENTRY_NCDSC (NC Description - REQUIRED)
    'Test CAR - Evidence',  -- CRENTRY_EVD (Evidence - REQUIRED)
    'M',                    -- CRENTRY_CRPRR (Medium priority)
    @SiteID,                -- CRENTRY_ST (Site - REQUIRED, must be INT)
    @LocationID,            -- CRENTRY_LCT (Location - REQUIRED, must be INT)
    @AreaID,                -- CRENTRY_AR (Area - REQUIRED, must be INT)
    'Y',                    -- CRENTRY_ATGNR (Yes - Auto Generated)
    @CARSource,             -- CRENTRY_CRSRC
    @TestCARSerial,         -- CRENTRY_CRSRLN
    @SourceTXNYear,          -- CRENTRY_TXNYR
    @SourceTXNNumber        -- CRENTRY_TXNN (Transaction Number)
);

-- Get the PrmKy of the inserted record (use MAX instead of SCOPE_IDENTITY for better reliability)
DECLARE @NewCARPrmKy INT;
SELECT @NewCARPrmKy = MAX(PrmKy) 
FROM dbo.HSE_CRENTRY 
WHERE CRENTRY_CRYR = @CurrentYear 
  AND CRENTRY_CRN = @TestCARNumber
  AND CRENTRY_CRSRC = @CARSource
  AND CRENTRY_ATGNR = 'Y';

-- Display the created CAR information
SELECT 
    'Test CAR Created Successfully!' AS Message,
    @NewCARPrmKy AS PrmKy,
    @TestCARNumber AS CARNumber,
    @CurrentYear AS CARYear,
    @TestCARSerial AS CARSerial,
    @CARSource AS CARSource,
    @SourceTXNYear AS TXNYear,
    @SourceTXNNumber AS TXNNumber;

-- Verify the record was created
SELECT 
    PrmKy,
    CRENTRY_CRN AS CARNumber,
    CRENTRY_CRYR AS CARYear,
    CRENTRY_CRSRLN AS CARSerial,
    CRENTRY_ATGNR AS AutoGenerated,
    CRENTRY_CRSRC AS CARSource,
    CRENTRY_TXNYR AS TXNYear,
    CRENTRY_TXNN AS TXNNumber
FROM dbo.HSE_CRENTRY
WHERE PrmKy = @NewCARPrmKy;

IF @NewCARPrmKy IS NOT NULL
BEGIN
    PRINT 'SUCCESS: Test CAR created with PrmKy: ' + CAST(@NewCARPrmKy AS VARCHAR(20));
    PRINT 'CAR Number: ' + CAST(@TestCARNumber AS VARCHAR(10)) + ' / ' + CAST(@CurrentYear AS VARCHAR(10));
    PRINT 'CAR Source: ' + @CARSource;
    PRINT 'Auto Generated: Yes';
    PRINT 'TXN Year: ' + CAST(@SourceTXNYear AS VARCHAR(10)) + ', TXN Number: ' + CAST(@SourceTXNNumber AS VARCHAR(10));
    PRINT '';
    PRINT 'You can now open this CAR in the system and test the "View Source TXN" button.';
    PRINT 'Note: Make sure the TXN Year and TXN Number match an existing source transaction.';
END
ELSE
BEGIN
    PRINT 'WARNING: CAR record may have been created but PrmKy could not be retrieved.';
    PRINT 'Please check the HSE_CRENTRY table manually for the new record.';
END

