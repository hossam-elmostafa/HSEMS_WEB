# Foreign Key Conflict: fk_HSE_CrEntry_HSE_CARMODELBAS

## What the error means

You are inserting a row into a table that has a **foreign key** to `HSE_CARMODELBAS`. The constraint **fk_HSE_CrEntry_HSE_CARMODELBAS** says:

- Some column in the table being inserted (almost certainly **HSE_CRENTRY** – CAR Entry) must contain a value that **already exists** in **HSE_CARMODELBAS.CARMODELBAS_COD**.
- The value you are inserting for that column **does not exist** in `HSE_CARMODELBAS`, so SQL Server blocks the INSERT.

So: **the INSERT is using a CAR Source/Basis code that is not in the CAR Model/Basis master table.**

---

## Where it comes from (Observation Close → CAR creation)

When you **close an observation** with **“Require CAR” = Yes**:

1. The stored procedure **closeNearMissTXN** (or similar) runs.
2. It creates a **Corrective Action Request (CAR)** by inserting into **HSE_CRENTRY**.
3. That INSERT sets **CAR Source** from policy: **HSE Policy > Observation > CAR Basis** (`HSEOBSRVTN_CrBss`).
4. The CAR Source column in **HSE_CRENTRY** (e.g. **CRENTRY_CRSRC**) is linked by the FK to **HSE_CARMODELBAS.CARMODELBAS_COD**.

If the policy value is **empty**, **null**, or a **code that does not exist** in **HSE_CARMODELBAS**, the INSERT fails with the conflict you see.

---

## Cause (summary)

| What | Detail |
|------|--------|
| **Constraint** | `fk_HSE_CrEntry_HSE_CARMODELBAS` |
| **Table inserted into** | **HSE_CRENTRY** (CAR Entry) |
| **Column that must match** | The column in HSE_CRENTRY that references **HSE_CARMODELBAS** (typically the CAR Source/Basis column, often **CRENTRY_CRSRC**). |
| **Referenced table** | **HSE_CARMODELBAS** (CAR Model/Basis – master data). |
| **Referenced column** | **CARMODELBAS_COD** |
| **Why it fails** | The value written into the CAR Source/Basis column is **not** present in **HSE_CARMODELBAS.CARMODELBAS_COD**. |

So the “problem” is: **the CAR Source/Basis value used on the new CAR is not a valid code in HSE_CARMODELBAS.**

---

## How to fix it

### 1. Fix the data (recommended)

- **Ensure CAR Basis in policy exists in HSE_CARMODELBAS**
  - Open **HSE Policy > Observation** (or equivalent) and check the **CAR Basis** (e.g. `HSEOBSRVTN_CrBss`) value.
  - Open the **CAR Model/Basis** screen (e.g. **HSE_CARMODELBAS** / “مصادر الإجراء التصحيحي”) and ensure there is a row with **CARMODELBAS_COD** = that same value.
- **Or add the missing code**
  - In **HSE_CARMODELBAS**, add a row whose **CARMODELBAS_COD** equals the value you use in policy (CAR Basis). Then the INSERT will satisfy the FK.

### 2. Check policy and master data in the database

Run something like:

```sql
-- See which CAR Basis is set in Observation policy
SELECT HSEOBSRVTN_CrBss FROM HSE_HSEOBSRVTN;

-- See which codes exist in CAR Model/Basis
SELECT CARMODELBAS_COD, CARMODELBAS_DESC FROM HSE_CARMODELBAS;
```

If the value from the first query is **NULL**, **empty**, or **not in the second result set**, that is the cause of the conflict. Fix by either:

- Updating policy to use an existing **CARMODELBAS_COD**, or  
- Inserting the missing code into **HSE_CARMODELBAS**.

### 3. Stored procedure (closeNearMissTXN)

If the CAR is created inside **closeNearMissTXN** (or another SP), the SP should use the **same** CAR Basis logic as the desktop (policy field **HSEOBSRVTN_CrBss**). If that value is empty or invalid, the SP can:

- **Validate** before INSERT: if the value is not in **HSE_CARMODELBAS**, either skip the CAR insert and log a message, or use a default **CARMODELBAS_COD** that you know exists (and document it).

---

## Diagram

```
┌─────────────────────┐         FK: value must exist here
│     HSE_CRENTRY     │         ┌──────────────────────────┐
│  (CAR Entry row)    │         │   HSE_CARMODELBAS        │
│                     │         │   (CAR Model/Basis)      │
│  CRENTRY_CRSRC  ────┼────────► │   CARMODELBAS_COD  (PK)  │
│  (CAR Source)       │         │   CARMODELBAS_DESC       │
└─────────────────────┘         └──────────────────────────┘
         │
         │  Value comes from:
         │  HSE_HSEOBSRVTN.HSEOBSRVTN_CrBss (Policy > Observation > CAR Basis)
         ▼
   If this value is not in HSE_CARMODELBAS.CARMODELBAS_COD
   → INSERT fails with fk_HSE_CrEntry_HSE_CARMODELBAS
```

---

## Summary

- **Error:** INSERT into a table that references **HSE_CARMODELBAS** is using a **CAR Source/Basis** value that does **not** exist in **HSE_CARMODELBAS.CARMODELBAS_COD**.
- **Fix:** Make the value used for CAR Source/Basis (from policy or SP) match an existing **CARMODELBAS_COD**, or add that code to **HSE_CARMODELBAS** and keep policy/SP in sync.
