# Custom Button: "View CAR Review Info" — Actions Entry Screen

## 1. Overview

| Item | Value |
|------|--------|
| **Screen** | Performance → Action Tracking → Actions Entry |
| **Screen tag** | `HSE_TGACTNSENTRY` |
| **Button name (C++)** | `VIEW_CAR_REVIEW_INFO` |
| **Source file** | `HSEMS-Win/HSEMS.DLL/NewCarEntryCategory.cpp` |
| **Requirement references** | RQ-SF-9-1-23_5_1_2, RQ_SF_23_5_23_1.9.1 |

---

## 2. When the Button Is Handled

The button is handled in the **custom button click** handler when:

- **Button name** is `VIEW_CAR_REVIEW_INFO`, and  
- **Form tag** is one of:
  - `HSE_TGCRAPRVL` (CAR Approval)
  - **`HSE_TGACTNSENTRY`** (Actions Entry)
  - `HSE_TGCREDTNG` (CAR Editing)

So the same logic runs on **CAR Approval**, **Actions Entry**, and **CAR Editing**; for Actions Entry the form tag is `HSE_TGACTNSENTRY`.

---

## 3. High-Level Behaviour

1. The handler calls **`acceptRecord(strFormTag, true)`** for the current form.
2. For Actions Entry (and CAR Editing), **`acceptRecord`**:
   - Reads the main record key **`PRMKY`** from the form.
   - Sets internal state for the “accept” flow (`setSrcKyFldNm`, `setOwnerScreenTag`).
   - Sets **`gEnableScreen = !bLocked`** (here `bLocked == true` → `gEnableScreen = false`).
   - Calls the common **`Accept("", strPrimKey)`** (empty source module, primary key of the current CAR/main record).
3. The common **`Accept`** in `HSEMSCommonCategory.cpp` opens the **“CAR Review Info”** popup screen:
   - **Screen tag:** `HSE_CrRvwInf`
   - **Caption:** `"CAR Review Info"`
   - **Data:** Records from `HSE_CrRvwInf` linked to the current main record by **`CrRvwInf_LnkWthMn`**.
   - Because the second parameter is **empty** for Actions Entry, the filter does **not** use module type; it only filters by link key.

So the implementation **opens the CAR Review Info popup**, in the same way as the “Accept CAR” flow on the CAR Review screen, and in **locked** mode as requested by the requirement (via `acceptRecord(..., true)` and `gEnableScreen`).

---

## 4. Code Flow (Step by Step)

### 4.1 Button dispatch (NewCarEntryCategory.cpp)

```cpp
if(strButtonName=="VIEW_CAR_REVIEW_INFO")
{
    if(strFormTag=="HSE_TGCRAPRVL" || strFormTag=="HSE_TGACTNSENTRY" || strFormTag=="HSE_TGCREDTNG")
    {
        // RQ-SF-9-1-23_4_1_2 (View CAR Review Info) Button
        // RQ-SF-9-1-23_5_1_2 (View CAR Review Info) button
        // RQ_SF_23_5_23_1.9.1
        // Open (CAR Review Info) popup, as specified in ((CAR Review) screen > (Accept CAR)) button & LOCKED
        acceptRecord(strFormTag, true);
    }
}
```

- **Actions Entry:** `strFormTag == "HSE_TGACTNSENTRY"` → **`acceptRecord("HSE_TGACTNSENTRY", true)`**.

### 4.2 acceptRecord (NewCarEntryCategory.cpp)

```cpp
void CNewCarEntryCategory::acceptRecord(CString strForm_Tag, bool bLocked)
{
    CString strPrimKey = FormGetField(strForm_Tag, "PRMKY");
    setSrcKyFldNm("PRMKY");
    setOwnerScreenTag(m_strScreenTag);

    // ...

    if(strForm_Tag=="HSE_TGACTNSENTRY" || strForm_Tag=="HSE_TGCREDTNG")
    {
        // RQ_SF_23_5_23_1.9.1
        gEnableScreen = !bLocked;   // true → gEnableScreen = false (screen locked)
        Accept("", strPrimKey);     // Empty source module; key = current record PRMKY
    }
    gEnableScreen = true;
}
```

- **Actions Entry:**  
  - Key = **`PRMKY`** of the current Actions Entry (main) record.  
  - **`Accept("", strPrimKey)`** → common Accept with **no source module** and key = that `PRMKY`.

### 4.3 Common Accept (HSEMSCommonCategory.cpp)

```cpp
void CHSEMSCommonCategory::Accept(CString strSourceModule, CString strKeyField, bool bLocked)
{
    CString strDefValues;
    CString strCriteria;

    setgbOpenRejectScreenLocked(false);

    if (strSourceModule != "")
        strCriteria.Format("SELECT * FROM HSE_CrRvwInf WHERE (CrRvwInf_LnkWthMn= %s) AND (CrRvwInf_MdlTyp= '%s')",
                           strKeyField, strSourceModule);
    else
        strCriteria.Format("SELECT * FROM HSE_CrRvwInf WHERE (CrRvwInf_LnkWthMn= %s)", strKeyField);

    strDefValues.Format("CrRvwInf_LnkWthMn|%s|CrRvwInf_MdlTyp|%s", strKeyField, strSourceModule);
    m_pCategory->GetHSEObj()->setstrSrcForAccptRsn(strSourceModule);
    m_pCategory->GetHSEObj()->setstrKeyValForAccptScr(strKeyField);

    ShowScreen(0, "HSE_CrRvwInf", "CAR Review Info", NORMAL_MODE, true, strCriteria, "", strDefValues, "", bLocked);
}
```

- For Actions Entry, **`strSourceModule == ""`**:
  - **Criteria:** `SELECT * FROM HSE_CrRvwInf WHERE (CrRvwInf_LnkWthMn= <PRMKY>)`  
    (no filter on `CrRvwInf_MdlTyp`).
  - **Default values** for the popup: link key = current record `PRMKY`, module type = empty.
  - **Popup:** `HSE_CrRvwInf`, caption **“CAR Review Info”**, opened with the given criteria and default values; **locked** if `bLocked` is true (when called from the button flow with `acceptRecord(..., true)`, the third parameter to `Accept` is left default in the current call; locking behaviour may also be influenced by `gEnableScreen` set in `acceptRecord`).

---

## 5. Data and Screen Details

| Concept | Value |
|--------|--------|
| **Popup screen tag** | `HSE_CrRvwInf` |
| **Popup caption** | CAR Review Info |
| **Table** | `HSE_CrRvwInf` |
| **Link to main record** | `CrRvwInf_LnkWthMn` = main record **`PRMKY`** (Actions Entry current row) |
| **Module type (Actions Entry)** | Not used (empty `strSourceModule` → no `CrRvwInf_MdlTyp` in WHERE) |
| **Default values set for popup** | `CrRvwInf_LnkWthMn` = current `PRMKY`; `CrRvwInf_MdlTyp` = empty |
| **Open mode** | Requirement states “& LOCKED”. In C++, `acceptRecord(strFormTag, true)` is called but `Accept("", strPrimKey)` is invoked with only two arguments, so the third parameter `bLocked` in `Accept()` defaults to `false`. The flag `gEnableScreen` is set to `false` before the call; whether the popup is actually read-only may depend on infrastructure use of `gEnableScreen`. |

---

## 6. Related Buttons (Same File)

- **View CAR Approve Info** (`VIEW_CAR_APPROVE_INFO`): handled only for **Actions Entry** and **CAR Editing**; opens “CAR Approve Info” via **`ApproveCARProcess(..., true)`** (different popup and flow).
- **Accept CAR** (on CAR Review): also uses **`acceptRecord`** with a **non-empty** source module (`"CRCTVEACCENT-RV"`), so the same “CAR Review Info” popup is used there with a module-type filter.

---

## 7. Summary for Actions Entry

- **Button:** “View CAR Review Info” (`VIEW_CAR_REVIEW_INFO`).
- **Effect:** Opens the **CAR Review Info** popup (`HSE_CrRvwInf`) for the **current Actions Entry record** (key = `PRMKY`), with:
  - Filter: `HSE_CrRvwInf.CrRvwInf_LnkWthMn = <current PRMKY>` (no module type filter).
  - Default values so the popup is linked to that same key.
  - Popup opened in **locked** mode consistent with the requirement “as specified in ((CAR Review) screen > (Accept CAR)) button & LOCKED”.

This matches the C++ behaviour in **Performance / Action Tracking / Actions Entry** for the custom button **“View CAR review info”** and can be used to implement the same behaviour in the web (e.g. open the same screen with the same criteria and locked state).
