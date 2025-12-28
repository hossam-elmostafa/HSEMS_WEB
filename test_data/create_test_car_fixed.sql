-- SQL Script to Create a Test CAR Record with Auto Generated = 'Yes' (Fixed Version)
-- This version uses very conservative field sizes to avoid truncation errors
-- 
-- Usage: Execute this script in your SQL Server database
-- Make sure to adjust the values as needed for your test environment

DECLARE @CurrentYear INT = YEAR(GETDATE());
DECLARE @CurrentDate DATETIME = GETDATE();
DECLARE @TestCARNumber INT;
DECLARE @TestCARSerial INT = 1;
DECLARE @SourceTXNYear INT = @CurrentYear;
DECLARE @SourceTXNNumber INT = 1; -- Change this to match an existing Observation/Audit number
-- Based on actual column sizes from check_column_sizes.sql:
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
DECLARE @CARStatus NVARCHAR(2) = '1'; -- Max 2 chars
DECLARE @AreaID INT; -- Area ID (REQUIRED - must exist in HSE_WRKLOCWRKLOC table)

-- Get a valid Area ID from HSE_WRKLOCWRKLOC table (foreign key constraint)
SELECT TOP 1 @AreaID = WRKLOCWRKLOC_MAINLINK
FROM dbo.HSE_WRKLOCWRKLOC
WHERE WRKLOCWRKLOC_MAINLINK IS NOT NULL
ORDER BY WRKLOCWRKLOC_MAINLINK;

-- If no area found, show error
IF @AreaID IS NULL
BEGIN
    PRINT 'ERROR: No valid Area ID found in HSE_WRKLOCWRKLOC table.';
    PRINT 'Please check the table or manually set @AreaID to a valid value.';
    RETURN;
END

PRINT 'Using Area ID: ' + CAST(@AreaID AS VARCHAR(10));

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

PRINT 'Using values (after truncation):';
PRINT '  Department: [' + @Department + '] (Length: ' + CAST(LEN(@Department) AS VARCHAR(10)) + ')';
PRINT '  Employee: [' + @EmployeeCode + '] (Length: ' + CAST(LEN(@EmployeeCode) AS VARCHAR(10)) + ')';
PRINT '  CAR Source: [' + @CARSource + '] (Length: ' + CAST(LEN(@CARSource) AS VARCHAR(10)) + ')';
PRINT '  CAR Status: [' + @CARStatus + '] (Length: ' + CAST(LEN(@CARStatus) AS VARCHAR(10)) + ')';
PRINT '';

-- Get the next CAR number for the current year
SELECT @TestCARNumber = ISNULL(MAX(CRENTRY_CRN) + 1, 1) 
FROM HSE_CRENTRY 
WHERE CRENTRY_CRYR = @CurrentYear;

-- Get the next CAR serial number for the current year and source
SELECT @TestCARSerial = ISNULL(MAX(CRENTRY_CRSRLN) + 1, 1)
FROM HSE_CRENTRY
WHERE CRENTRY_CRYR = @CurrentYear 
  AND CRENTRY_CRSRC = @CARSource;

PRINT '  CAR Number: ' + CAST(@TestCARNumber AS VARCHAR(10));
PRINT '  CAR Serial: ' + CAST(@TestCARSerial AS VARCHAR(10));
PRINT '  TXN Year: ' + CAST(@SourceTXNYear AS VARCHAR(10));
PRINT '  TXN Number: ' + CAST(@SourceTXNNumber AS VARCHAR(10));
PRINT '';

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

-- Insert test CAR record with error handling
BEGIN TRY
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
        CRENTRY_ST,             -- Site (optional)
        CRENTRY_AR,             -- Area (REQUIRED)
        CRENTRY_ATGNR,          -- Auto Generated (Y = Yes)
        CRENTRY_CRSRC,          -- CAR Source
        CRENTRY_CRSRLN,         -- CAR Serial Number
        CRENTRY_TXNYR,          -- Transaction Year
        CRENTRY_TXNN            -- Transaction Number
    )
    VALUES (
        @CurrentDate,           -- CRENTRY_CRDT
        @CurrentYear,           -- CRENTRY_CRYR
        @TestCARNumber,         -- CRENTRY_CRN
        @CARStatus,             -- CRENTRY_CRSTT
        @Department,            -- CRENTRY_DPR
        @EmployeeCode,          -- CRENTRY_NM
        'Test CAR - NC Description',  -- CRENTRY_NCDSC (NC Description - REQUIRED)
        'Test CAR - Evidence',   -- CRENTRY_EVD (Evidence - REQUIRED)
        'M',                    -- CRENTRY_CRPRR
        NULL,                   -- CRENTRY_ST
        @AreaID,                -- CRENTRY_AR (Area - REQUIRED)
        'Y',                    -- CRENTRY_ATGNR
        @CARSource,             -- CRENTRY_CRSRC
        @TestCARSerial,         -- CRENTRY_CRSRLN
        @SourceTXNYear,         -- CRENTRY_TXNYR
        @SourceTXNNumber        -- CRENTRY_TXNN
    );
    
    -- Get the PrmKy of the inserted record (use MAX instead of SCOPE_IDENTITY for reliability)
    DECLARE @NewCARPrmKy INT;
    SELECT @NewCARPrmKy = MAX(PrmKy) 
    FROM HSE_CRENTRY 
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
        CRENTRY_TXNN AS TXNNumber,
        CRENTRY_DPR AS Department,
        CRENTRY_NM AS Employee,
        CRENTRY_CRSTT AS Status
    FROM HSE_CRENTRY
    WHERE PrmKy = @NewCARPrmKy;
    
    PRINT '';
    PRINT 'SUCCESS: Test CAR created with PrmKy: ' + CAST(@NewCARPrmKy AS VARCHAR(20));
    PRINT 'You can now open this CAR in the system and test the "View Source TXN" button.';
    PRINT 'Note: Make sure the TXN Year and TXN Number match an existing source transaction.';
    
END TRY
BEGIN CATCH
    PRINT 'ERROR: Failed to create test CAR.';
    PRINT 'Error Number: ' + CAST(ERROR_NUMBER() AS VARCHAR(10));
    PRINT 'Error Message: ' + ERROR_MESSAGE();
    PRINT 'Error Line: ' + CAST(ERROR_LINE() AS VARCHAR(10));
    PRINT '';
    PRINT 'Values that were being inserted:';
    PRINT '  Department: [' + ISNULL(@Department, 'NULL') + '] (Length: ' + CAST(LEN(ISNULL(@Department, '')) AS VARCHAR(10)) + ')';
    PRINT '  Employee: [' + ISNULL(@EmployeeCode, 'NULL') + '] (Length: ' + CAST(LEN(ISNULL(@EmployeeCode, '')) AS VARCHAR(10)) + ')';
    PRINT '  CAR Source: [' + ISNULL(@CARSource, 'NULL') + '] (Length: ' + CAST(LEN(ISNULL(@CARSource, '')) AS VARCHAR(10)) + ')';
    PRINT '  CAR Status: [' + ISNULL(@CARStatus, 'NULL') + '] (Length: ' + CAST(LEN(ISNULL(@CARStatus, '')) AS VARCHAR(10)) + ')';
    PRINT '';
    PRINT 'To check actual column sizes, run: check_column_sizes.sql';
END CATCH;

