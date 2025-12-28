-- Check the foreign key constraint on HSE_CRENTRY.CRENTRY_LCT
-- This will help us understand what the constraint is checking

-- 1. Get the foreign key constraint details
SELECT 
    fk.name AS ForeignKeyName,
    OBJECT_NAME(fk.parent_object_id) AS TableName,
    COL_NAME(fkc.parent_object_id, fkc.parent_column_id) AS ColumnName,
    OBJECT_NAME(fk.referenced_object_id) AS ReferencedTableName,
    COL_NAME(fkc.referenced_object_id, fkc.referenced_column_id) AS ReferencedColumnName
FROM sys.foreign_keys AS fk
INNER JOIN sys.foreign_key_columns AS fkc 
    ON fk.object_id = fkc.constraint_object_id
WHERE fk.name = 'fk_HSE_CrEntry_CrEntry_Lct';

PRINT '';
PRINT '---';
PRINT '';

-- 2. Check if Location 302 exists in HSE_WRKLOC
SELECT 
    'Location 302 exists in HSE_WRKLOC:' AS Check_Result,
    COUNT(*) AS Count
FROM dbo.HSE_WRKLOC
WHERE WRKLOC_LOCTYP = 302;

PRINT '';
PRINT '---';
PRINT '';

-- 3. Check the structure of HSE_WRKLOC table
SELECT 
    COLUMN_NAME,
    DATA_TYPE,
    CHARACTER_MAXIMUM_LENGTH,
    IS_NULLABLE
FROM INFORMATION_SCHEMA.COLUMNS
WHERE TABLE_NAME = 'HSE_WRKLOC'
ORDER BY ORDINAL_POSITION;

PRINT '';
PRINT '---';
PRINT '';

-- 4. Get a sample of HSE_WRKLOC records to see the data
SELECT TOP 10 *
FROM dbo.HSE_WRKLOC
ORDER BY WRKLOC_LOCTYP;

PRINT '';
PRINT '---';
PRINT '';

-- 5. Check what Location IDs are actually used in existing CAR records with Area 1628 and Site '03'
SELECT DISTINCT
    CRENTRY_AR AS Area,
    CRENTRY_ST AS Site,
    CRENTRY_LCT AS Location,
    COUNT(*) AS Count
FROM dbo.HSE_CRENTRY
WHERE CRENTRY_AR = 1628
  AND CRENTRY_ST = '03'
  AND CRENTRY_LCT IS NOT NULL
GROUP BY CRENTRY_AR, CRENTRY_ST, CRENTRY_LCT
ORDER BY Count DESC;

PRINT '';
PRINT '---';
PRINT '';

-- 6. Verify that the combination exists in HSE_WRKLOC
SELECT 
    'Checking if Location 302 with Area 1628 exists in HSE_WRKLOC:' AS Check_Result,
    COUNT(*) AS Count
FROM dbo.HSE_WRKLOC w
WHERE w.WRKLOC_LOCTYP = 302
  AND EXISTS (
      SELECT 1 FROM dbo.HSE_CRENTRY c 
      WHERE c.CRENTRY_LCT = w.WRKLOC_LOCTYP 
        AND c.CRENTRY_AR = 1628
  );

