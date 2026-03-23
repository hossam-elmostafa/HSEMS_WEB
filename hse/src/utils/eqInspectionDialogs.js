/**
 * RQ_HSE_23_3_26_23_15 — Desktop-style informational dialogs (AfxMessageBox MB_OK / MB_ICONINFORMATION parity).
 * AskYesNoMessage(title, msg) uses (title, body); a single-argument call puts the text in title and leaves body as the default placeholder.
 */

/**
 * @param {Object} devInterfaceObj
 * @param {string} message
 * @param {string} [title='Information']
 * @returns {Promise<void>}
 */
export function showEqInformationDialog(devInterfaceObj, message, title = 'Information') {
  const openCustomDlg = devInterfaceObj?.openCustomDlg;
  const msg = message != null ? String(message) : '';
  const ttl = title || 'Information';
  if (typeof openCustomDlg === 'function') {
    return new Promise((resolve) => {
      openCustomDlg({
        open: true,
        dlgTitle: ttl,
        dlgMsg: msg,
        dlgActnArr: [{ btnID: 'OK', btnFunc: () => resolve() }],
        dlgStructure: {
          TagName: 'EqInformationMessage',
          frames: [
            {
              EnglishFrameName: '',
              frameWidth: '12',
              frameDirection: 'row',
              fields: [],
              hidden: true,
            },
          ],
        },
      });
    });
  }
  if (typeof devInterfaceObj?.AskYesNoMessage === 'function') {
    return devInterfaceObj.AskYesNoMessage(ttl, msg).then(() => {});
  }
  console.warn('[Web_HSE] EQ information:', msg);
  return Promise.resolve();
}

/**
 * @param {Function|undefined} AskYesNoMessage
 * @param {string} message
 * @returns {Promise<boolean>}
 */
export async function confirmEqYesNo(AskYesNoMessage, message) {
  if (typeof AskYesNoMessage !== 'function') return false;
  const ret = AskYesNoMessage('Prompt', message);
  if (ret && typeof ret.then === 'function') return !!(await ret);
  return !!ret;
}
