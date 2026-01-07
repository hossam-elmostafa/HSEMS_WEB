# Incident Service - Quick Start Guide

## ✅ Integration Complete!

The Incident service has been successfully integrated into the HSE application.

## What Was Done

### 1. Service Files Created
- ✅ `IncidentButtonHandlers.js` - All button handler functions
- ✅ `IncidentService.js` - Main service export

### 2. Integration Points Updated
- ✅ `hse/hse.js` - Added `initializeIncidentService()` call
- ✅ `hse/src/events/buttonEvents.js` - Added `handleIncidentButtonClick()` call

## Supported Buttons

| Button ID | Description | Screen |
|-----------|-------------|--------|
| `ACDNTENT_ENTCMPLT` | Entry Completed | Incident Entry |
| `ACDNTENT_RJC` | Reject | Confirmation/Follow-up |
| `ACDNTENT_CNFRM` | Confirm | Confirmation |
| `ACDNTENT_ACCEPT` | Accept | Confirmation |
| `ACDNTENT_CLOSE` | Close | Follow-up |
| `AcdntEntTrc_VWRjctRsns` | View Reject Reasons | Tracing Tab |
| `RJCTRSN_BTN_OK` | Reject Reason OK | Reject Reason Screen |

## Testing Your Buttons

### Step 1: Open Browser Console
Press `F12` to open developer tools and go to the Console tab.

### Step 2: Click an Incident Button
Click any of the buttons listed above.

### Step 3: Check Console Output
You should see logs like:
```
[Web_HSE] ===== Incident Button Click Handler =====
[Web_HSE] Button Name: ACDNTENT_ENTCMPLT
[Web_HSE] Screen Tag: HSE_TGACDNTENT
[Web_HSE] Event Object: {...}
```

## Troubleshooting

### Problem: No console logs appear

**Solution**: Check if the button name matches exactly. The service looks for these patterns:
- `ACDNTENT_ENTCMPLT`
- `ACDNTENT_RJC`
- `ACDNTENT_CNFRM`
- `ACDNTENT_ACCEPT`
- `ACDNTENT_CLOSE`
- `ACDNTENT_TRC_VWRJCTRSNS`
- `RJCTRSN_BTN_OK`

### Problem: "Button not handled by Incident service" message

**Possible causes**:
1. Button name doesn't match expected patterns
2. Check the button ID in your JSON configuration
3. Verify the button name is being passed correctly

**How to check**:
```javascript
// In browser console, check what button name is being sent:
console.log('Button clicked:', buttonName);
```

### Problem: "Unable to find Incident number"

**Possible causes**:
1. No record selected
2. Incident number field not found in record
3. FormGetField not working

**How to fix**:
1. Make sure you select a record before clicking the button
2. Check if the field `ACDNTENT_ACDNTNUM` exists in your record
3. Check console for more detailed error messages

### Problem: "System error: Required functions not available"

**Possible causes**:
1. devInterface not properly initialized
2. Missing functions in devInterface

**How to fix**:
1. Check that `hse.js` is properly setting up devInterface
2. Verify all required functions are available:
   - `FormGetField`
   - `executeSQL` or `executeSQLPromise`
   - `getUserName`
   - `refreshData`
   - `openScr` (for reject button)

### Problem: Database error when clicking button

**Possible causes**:
1. Stored procedure doesn't exist
2. Database connection issue
3. SQL syntax error

**How to check**:
1. Look at the console for the SQL being executed
2. Verify stored procedures exist in database:
   - `completeAccidentTXN`
   - `rejectIncident`
   - `confirmAccidentTXN`
   - `acceptIncidentTXN`
   - `closeIncidentTXN`

## Verifying Integration

### Check 1: Service Initialized
When the app starts, you should see:
```
[Web_HSE] Incident Service initialized
[Web_HSE] Supported buttons:
[Web_HSE]   - ACDNTENT_ENTCMPLT (Entry Completed)
[Web_HSE]   - ACDNTENT_RJC (Reject)
...
```

### Check 2: Button Click Detected
When you click a button, you should see:
```
[Web_HSE] ===== Incident Button Click Handler =====
[Web_HSE] Button Name: ACDNTENT_ENTCMPLT
```

### Check 3: Handler Executed
After button processing, you should see:
```
[Web_HSE] Completing Incident entry: EXECUTE completeAccidentTXN ...
[Web_HSE] ✓ Incident entry completed successfully
```

## Common Button Workflows

### Entry Completed Workflow
1. User clicks "Entry Completed" button
2. Service validates recommended actions
3. If invalid, prompts user to proceed
4. Executes `completeAccidentTXN` stored procedure
5. Shows success toast
6. Refreshes screen

### Reject Workflow
1. User clicks "Reject" button
2. Service opens reject reason screen
3. User enters reject reasons
4. User clicks OK in reject reason screen
5. Service executes `rejectIncident` stored procedure
6. Links reject reasons to tracing record
7. Shows success toast
8. Refreshes screen

### Confirm Workflow
1. User clicks "Confirm" button
2. Service checks for existing reject reasons
3. If found, prompts user to delete them
4. Saves record first
5. Executes `confirmAccidentTXN` stored procedure
6. Shows success toast
7. Refreshes screen

## Database Requirements

### Required Stored Procedures
```sql
-- Check if procedures exist
SELECT name FROM sys.procedures 
WHERE name IN (
    'completeAccidentTXN',
    'rejectIncident',
    'confirmAccidentTXN',
    'acceptIncidentTXN',
    'closeIncidentTXN'
);
```

### Required Function
```sql
-- Check if function exists
SELECT name FROM sys.objects 
WHERE name = 'ValidateBeforeIncdntEntryComplt' 
  AND type = 'FN';
```

### Required Tables
```sql
-- Check if tables exist
SELECT name FROM sys.tables 
WHERE name IN (
    'HSE_ACDNTENT',
    'HSE_ACDNTENT_TRC',
    'HSE_RJCTRSN'
);
```

## Testing Checklist

- [ ] Service initialized (check console on app start)
- [ ] Button click detected (check console when clicking)
- [ ] Handler executed (check console for SQL execution)
- [ ] Success toast appears
- [ ] Screen refreshes
- [ ] Database updated (check tracing table)
- [ ] No errors in console

## Need Help?

### Enable Detailed Logging
All operations log to console with `[Web_HSE]` prefix. Check browser console for detailed information.

### Check Integration
1. Verify `hse.js` imports `initializeIncidentService`
2. Verify `buttonEvents.js` imports `handleIncidentButtonClick`
3. Verify `buttonEvents.js` calls `handleIncidentButtonClick` in `ButtonClicked` function

### Check Button Configuration
1. Open incident screen JSON configuration
2. Verify button ID matches expected pattern
3. Check button is enabled and visible

## Example Console Output (Success)

```
[Web_HSE] Incident Service initialized
[Web_HSE] Supported buttons:
[Web_HSE]   - ACDNTENT_ENTCMPLT (Entry Completed)
[Web_HSE]   - ACDNTENT_RJC (Reject)
[Web_HSE]   - ACDNTENT_CNFRM (Confirm)
[Web_HSE]   - ACDNTENT_ACCEPT (Accept)
[Web_HSE]   - ACDNTENT_CLOSE (Close)
[Web_HSE]   - ACDNTENT_TRC_VWRJCTRSNS (View Reject Reasons from Tracing)
[Web_HSE]   - RJCTRSN_BTN_OK (Reject Reason OK)

[Web_HSE] ===== Incident Button Click Handler =====
[Web_HSE] Button Name: ACDNTENT_ENTCMPLT
[Web_HSE] Screen Tag: HSE_TGACDNTENT
[Web_HSE] Event Object: {fullRecord: Array(1), fullRecordArrKeys: Array(1), ...}
[Web_HSE] Completing Incident entry: EXECUTE completeAccidentTXN '12345','Incident Entry','testuser'
[Web_HSE] ✓ Incident entry completed successfully
```

## Quick Reference

### File Locations
- Service: `Web_HSE/hse/src/services/Incident service/`
- Integration: `Web_HSE/hse/hse.js` and `Web_HSE/hse/src/events/buttonEvents.js`

### Key Functions
- `handleIncidentButtonClick()` - Main dispatcher
- `handleEntryCompletedButton()` - Entry completed logic
- `handleRejectIncidentButton()` - Reject logic
- `handleConfirmIncidentButton()` - Confirm logic

### Status Codes
- `01` - New/Draft
- `02` - Entry Rejected
- `05` - Entry Completed
- `10` - Confirmed
- `15` - Accepted
- `20` - Closed

---

**Last Updated**: December 30, 2025  
**Status**: ✅ Integrated and Ready to Use




