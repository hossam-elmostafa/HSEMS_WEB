# Fix: Observation Entry Screen - "Invalid column name 'ID'"

## Problem
The backend server has **cached** the old screen configuration where the field was `"ID"`.

## Solution

### ✅ Already Fixed in Code
Changed `header.json` line 535:
- **Before:** `"DataFieldName": "ID"`
- **After:** `"DataFieldName": "MAINLINK"`

### 🔄 Need to Clear Cache

The backend caches screen configurations in:
- **Location:** System temp directory
- **File:** `scrStructureCache`
- **Code:** `Webinfra.Server-master/util/scrStructureUtil.js` (lines 23-27)

## How to Fix

### Option 1: Restart Backend Server (Easiest)
1. **Stop the backend:**
   - Go to Terminal 2 (where backend is running)
   - Press `Ctrl+C` to stop
   
2. **Start it again:**
   ```bash
   npm start
   ```

### Option 2: Clear Cache Manually
Find and delete the cache file:
- **Windows:** `C:\Users\Admin\AppData\Local\Temp\scrStructureCache`
- **Then restart the backend**

---

## Why This Happened
The backend uses `flat-cache` to store screen configurations for performance. When you changed the JSON file, the old configuration was still in memory/cache.

## Expected Result
After restarting, the observation entry screen should open successfully! ✅

