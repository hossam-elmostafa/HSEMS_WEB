# Test CAR Record Creation Scripts

These SQL scripts help you create or update CAR records for testing the "View Source TXN" button.

## Files:

1. **create_test_car.sql** - Creates a new test CAR record with Auto Generated = 'Yes'
2. **create_test_car_from_existing.sql** - Updates an existing CAR record to be auto-generated

## How to Use:

### Option 1: Create a New Test CAR

1. Open `create_test_car.sql` in SQL Server Management Studio
2. Review and adjust the variables:
   - `@CARSource` - Set to 'Observation', 'Audit', 'Incident', etc.
   - `@SourceTXNYear` - Set to a year that has source transactions
   - `@SourceTXNNumber` - Set to an existing Observation/Audit/Incident number
3. Execute the script
4. Note the `PrmKy` value returned
5. Open that CAR in the system using the PrmKy

### Option 2: Update an Existing CAR

1. Find an existing CAR record's PrmKy (you can query HSE_CRENTRY table)
2. Open `create_test_car_from_existing.sql`
3. Update the `@CARPrmKy` variable with the CAR's PrmKy
4. Update `@CARSource`, `@TXNYear`, and `@TXNNumber` to match an existing source transaction
5. Execute the script
6. Open that CAR in the system

## Important Notes:

- **CRITICAL: Column Size Constraints:**
  - `CRENTRY_CRSRC` is `varchar(10)` - **MAX 10 characters!**
  - `CRENTRY_CRSTT` is `nvarchar(2)` - **MAX 2 characters!**
  - `CRENTRY_DPR` is `varchar(50)` - Max 50 characters
  - `CRENTRY_NM` is `varchar(50)` - Max 50 characters

- The TXN Year and TXN Number must match an existing source transaction (Observation, Audit, etc.)
- The CAR Source must match one of the supported types (and be ≤ 10 chars):
  - 'Audit' (5 chars) ✓
  - 'Incident' (8 chars) ✓
  - 'Observation' (11 chars) ✗ **TOO LONG - Use 'Audit' or 'Incident' instead**
  - 'Site Survey' (11 chars) ✗ **TOO LONG**
  - Or use numeric codes: '1', '2', '3', etc. (if supported)

## Finding Source Transaction Numbers:

To find valid TXN numbers for testing:

```sql
-- For Observations
SELECT TOP 10 NRSTMISCENT_YR AS Year, NRSTMISCENT_NO AS Number, PrmKy
FROM HSE_NrstMiscEnt
ORDER BY PrmKy DESC;

-- For Audits
SELECT TOP 10 YEAR(ADTPLNENT_PLNDAT) AS Year, ADTPLNENT_PLNNO AS Number, PrmKy
FROM HSE_AdtPlnEnt
ORDER BY PrmKy DESC;
```

## After Creating/Updating:

1. Open the CAR in the system (you may need to search by CAR Number or use the PrmKy)
2. Verify that:
   - Auto Generated = "Yes"
   - CAR Source is populated
   - TXN Year and TXN Number are populated
3. Click the "View Source TXN" button
4. It should open the source transaction inquiry screen

