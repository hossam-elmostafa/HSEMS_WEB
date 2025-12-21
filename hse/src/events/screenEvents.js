/**
 * @name SubFieldChanged
 * @description this event called when you change field in screen or tab
 * @param {string} strScrTag header screen tag
 * @param {string} strTabTag tab tag if changed field in tab
 * @param {string} fieldName changed field name   
 * @param {string} oldFieldVal old field value
 * @param {string} fieldVal new field value
 * @param {object} updatedAppGlobalVal object Please ignore it for now
 * 
 * This event doesn't require callback
 */
export function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, updatedAppGlobalVal) {
  // Empty implementation - can be extended later
}

/**
 * @name MainSubReposition
 * @description this event is called when navigating between rows from screen or tab
 * @param {string} strFormTag header screen tag
 * @param {*} Main_Position   Not used
 * @param {*} Seleted_Tab     Not used
 * @param {string} strSelectedTabTag tab tag
 */
export function MainSubReposition(strFormTag, Main_Position, Seleted_Tab, strSelectedTabTag) {
  // Empty implementation - can be extended later
}

/**
 * @name ShowScreen
 * @description this event is called when screen\tab is open,Note : this event called after fetching data 'This mean you can't change open
 *              criteria here.
 *              You Must set attributes for screen buttons here using setScreenDisableBtn(bDisableNew, bDisableSave, bDisableDelete)
 * @param {Function} setScreenDisableBtn - Function to set screen button states
 * @param {*} strScrTag screen tag
 * @param {*} strTabTag tab tag
 */
export function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag) {
  setScreenDisableBtn(false, false, false);
}

