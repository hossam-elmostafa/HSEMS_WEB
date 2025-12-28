-- Find the correct location table name
-- The foreign key constraint references a table with column 'WRKLOC_LOCTYP'

-- 1. Search for tables with 'WRKLOC' in the name
SELECT 
    TABLE_SCHEMA,
    TABLE_NAME
FROM INFORMATION_SCHEMA.TABLES
WHERE TABLE_NAME LIKE '%WRKLOC%'
   OR TABLE_NAME LIKE '%LOC%'
ORDER BY TABLE_NAME;

PRINT '';
PRINT '---';
PRINT '';

-- 2. Search for columns with 'LOCTYP' in the name
SELECT 
    TABLE_SCHEMA,
    TABLE_NAME,
    COLUMN_NAME,
    DATA_TYPE
FROM INFORMATION_SCHEMA.COLUMNS
WHERE COLUMN_NAME LIKE '%LOCTYP%'
   OR COLUMN_NAME LIKE '%LOC%'
ORDER BY TABLE_NAME, COLUMN_NAME;

PRINT '';
PRINT '---';
PRINT '';

-- 3. Find the foreign key constraint details
SELECT 
    fk.name AS ForeignKeyName,
    OBJECT_SCHEMA_NAME(fk.parent_object_id) AS ParentSchema,
    OBJECT_NAME(fk.parent_object_id) AS ParentTable,
    COL_NAME(fkc.parent_object_id, fkc.parent_column_id) AS ParentColumn,
    OBJECT_SCHEMA_NAME(fk.referenced_object_id) AS ReferencedSchema,
    OBJECT_NAME(fk.referenced_object_id) AS ReferencedTable,
    COL_NAME(fkc.referenced_object_id, fkc.referenced_column_id) AS ReferencedColumn
FROM sys.foreign_keys AS fk
INNER JOIN sys.foreign_key_columns AS fkc 
    ON fk.object_id = fkc.constraint_object_id
WHERE fk.name LIKE '%CrEntry%Lct%'
   OR fk.name LIKE '%CRENTRY%LCT%'
   OR (OBJECT_NAME(fk.parent_object_id) LIKE '%CRENTRY%' 
       AND COL_NAME(fkc.parent_object_id, fkc.parent_column_id) LIKE '%LCT%');

PRINT '';
PRINT '---';
PRINT '';

-- 4. Check what column CRENTRY_LCT actually references
SELECT 
    fk.name AS ForeignKeyName,
    OBJECT_SCHEMA_NAME(fk.parent_object_id) AS ParentSchema,
    OBJECT_NAME(fk.parent_object_id) AS ParentTable,
    COL_NAME(fkc.parent_object_id, fkc.parent_column_id) AS ParentColumn,
    OBJECT_SCHEMA_NAME(fk.referenced_object_id) AS ReferencedSchema,
    OBJECT_NAME(fk.referenced_object_id) AS ReferencedTable,
    COL_NAME(fkc.referenced_object_id, fkc.referenced_column_id) AS ReferencedColumn
FROM sys.foreign_keys AS fk
INNER JOIN sys.foreign_key_columns AS fkc 
    ON fk.object_id = fkc.constraint_object_id
WHERE OBJECT_NAME(fk.parent_object_id) = 'HSE_CRENTRY'
  AND COL_NAME(fkc.parent_object_id, fkc.parent_column_id) = 'CRENTRY_LCT';

