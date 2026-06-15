/**
 * RQ_HSE_23_3_26_23_15 — EQInspctnCategory::DisplayFieldChange parity (checklist, expected exit date, defect line serial).
 */

import { escapeSqlString } from '../../../services/ModuleButtonHandlers/moduleButtonHandlersUtils.js';
import { showEqInformationDialog } from '../../../utils/eqInspectionDialogs.js';

const TBL_HDR = 'HSE_EQINSPCTN';
const TBL_CHKLST = 'HSE_EQINSPCTN_INSPCTNCHCKLST';
const TBL_DFCT = 'HSE_EQINSPCTN_DFCTSDURNGWRK';

const EQ_SCR_TAGS = new Set([
  'HSE_EQINSPCTNRQUST',
  'HSE_EQINSPCTNRSULT',
  'HSE_EQINSPCTNWRK',
]);

/**
 * @returns {Promise<{ cancel?: number }>}
 */
export async function eqInspectionSubFieldChanged(
  strScrTag,
  strTabTag,
  fieldName,
  oldFieldVal,
  fieldVal,
  devInterfaceObj
) {
  const scr = String(strScrTag || '').toUpperCase();
  if (!EQ_SCR_TAGS.has(scr)) return { cancel: 0 };

  const { FormGetField, FormSetField, executeSQLPromise } = devInterfaceObj || {};
  if (!FormGetField || !FormSetField) return { cancel: 0 };

  const fld = String(fieldName || '').toUpperCase();
  if (fld === 'EQINSPCTN_EQTYP' && executeSQLPromise) {
    const typ = String(fieldVal ?? '').trim();
    if (typ) {
      try {
        const data = await executeSQLPromise(
          `SELECT ISNULL(COUNT(0),0) AS CHECK_LIST_COUNT FROM HSE_EQTYPS_INSPCTNCHCKLST WHERE MAINLINK = '${escapeSqlString(typ)}'`
        );
        const row = data?.recordset?.[0] ?? data?.[0];
        const cnt = parseInt(row?.CHECK_LIST_COUNT ?? row?.check_list_count ?? 0, 10) || 0;
        if (cnt === 0) {
          void showEqInformationDialog(
            devInterfaceObj,
            'No inspection checklist is defined for this equipment type.',
            'Information'
          );
        }
      } catch (e) {
        console.warn('[Web_HSE] EQ inspection checklist count:', e);
      }
    }
  }

  if (fld === 'EQINSPCTN_INSPCTNCHCKLST_DFCTFUND') {
    const nv = String(fieldVal ?? '').toUpperCase().trim();
    if (nv === 'N') {
      FormSetField(TBL_CHKLST, 'EQINSPCTN_INSPCTNCHCKLST_INSPCTNACCPTD', 'Y', 'tab');
    } else {
      const actn = String(
        FormGetField(TBL_CHKLST, 'EQINSPCTN_INSPCTNCHCKLST_ACTNDN', 'tab') ?? ''
      ).toUpperCase();
      if (actn === 'Y') {
        FormSetField(TBL_CHKLST, 'EQINSPCTN_INSPCTNCHCKLST_INSPCTNACCPTD', 'Y', 'tab');
      } else {
        FormSetField(TBL_CHKLST, 'EQINSPCTN_INSPCTNCHCKLST_INSPCTNACCPTD', 'N', 'tab');
      }
    }
  }

  if (fld === 'EQINSPCTN_INSPCTNCHCKLST_ACTNDN') {
    const nv = String(fieldVal ?? '').toUpperCase().trim();
    if (nv === 'Y') {
      FormSetField(TBL_CHKLST, 'EQINSPCTN_INSPCTNCHCKLST_INSPCTNACCPTD', 'Y', 'tab');
    } else {
      FormSetField(TBL_CHKLST, 'EQINSPCTN_INSPCTNCHCKLST_INSPCTNACCPTD', 'N', 'tab');
    }
  }

  if (
    fld === 'EQINSPCTN_EQENTRYDT' ||
    fld === 'EQINSPCTN_EQWRKNGDYS' ||
    fld === 'EQINSPCTN_EQWRKNGHURS'
  ) {
    const entryRaw = FormGetField(TBL_HDR, 'EQINSPCTN_EQENTRYDT', 'scr');
    const days = parseInt(String(FormGetField(TBL_HDR, 'EQINSPCTN_EQWRKNGDYS', 'scr') ?? '0').trim(), 10) || 0;
    const hours = parseInt(String(FormGetField(TBL_HDR, 'EQINSPCTN_EQWRKNGHURS', 'scr') ?? '0').trim(), 10) || 0;
    const dt = entryRaw != null && String(entryRaw).trim() !== '' ? new Date(entryRaw) : null;
    if (dt && !Number.isNaN(dt.getTime())) {
      const ms = dt.getTime() + days * 86400000 + hours * 3600000;
      const out = new Date(ms);
      const y = out.getFullYear();
      const m = String(out.getMonth() + 1).padStart(2, '0');
      const d = String(out.getDate()).padStart(2, '0');
      FormSetField(TBL_HDR, 'EQINSPCTN_EXPCTDEXTDT', `${y}-${m}-${d}`, 'scr');
    }
  }

  // C++ EQInspctnCategory::DisplayFieldChange — assign defect line serial when empty (runs on any field change; desktop had tab-gating TODO)
  if (executeSQLPromise) {
    const lineNo = String(FormGetField(TBL_DFCT, 'EQINSPCTN_DFCTSDURNGWRK_LINENO', 'tab') ?? '').trim();
    if (lineNo === '') {
      const mainLink =
        String(FormGetField(TBL_HDR, 'EQINSPCTN_PRMKY', 'scr') ?? '').trim() ||
        String(FormGetField(TBL_HDR, 'EQINSPCTN_PRMRYKY', 'scr') ?? '').trim();
      if (mainLink) {
        try {
          const data = await executeSQLPromise(
            `SELECT ISNULL(MAX(EQINSPCTN_DFCTSDURNGWRK_LINENO)+1,1) AS NXLN FROM HSE_EQINSPCTN_DFCTSDURNGWRK WHERE MAINLINK = '${escapeSqlString(mainLink)}'`
          );
          const row = data?.recordset?.[0] ?? data?.[0];
          const nx = row?.NXLN ?? row?.nxln ?? '';
          if (nx !== '' && nx != null) {
            FormSetField(TBL_DFCT, 'EQINSPCTN_DFCTSDURNGWRK_LINENO', String(nx), 'tab');
          }
        } catch (e) {
          console.warn('[Web_HSE] EQ defect line serial:', e);
        }
      }
    }
  }

  return { cancel: 0 };
}
