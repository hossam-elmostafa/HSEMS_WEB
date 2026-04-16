/**
 * Centralized field-read helpers.
 * Rule: always pass srcType to FormGetField ('SCR' for main, 'TAB' for details).
 */

function normalizeSrcType(srcType) {
  const t = String(srcType || '').toUpperCase();
  return t === 'TAB' ? 'TAB' : 'SCR';
}

export function getFieldBySrcType(devInterfaceObj, tagName, fieldName, srcType) {
  const FormGetField = devInterfaceObj?.FormGetField;
  if (typeof FormGetField !== 'function') return '';
  return FormGetField(tagName, fieldName, normalizeSrcType(srcType));
}

export function getFieldSCR(devInterfaceObj, formTag, fieldName) {
  return getFieldBySrcType(devInterfaceObj, formTag, fieldName, 'SCR');
}

export function getFieldTAB(devInterfaceObj, tabTag, fieldName) {
  return getFieldBySrcType(devInterfaceObj, tabTag, fieldName, 'TAB');
}

