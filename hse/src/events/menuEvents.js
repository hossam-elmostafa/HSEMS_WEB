import { applayDisabledTags } from '../utils/menuUtils';

// Module-level variable to store devInterface functions
let devInterfaceObj = {};

/**
 * Set devInterface functions for menu event handlers
 * @param {Object} devInterface - Object containing devInterface functions
 */
export function setMenuDevInterface(devInterface) {
  devInterfaceObj = devInterface || {};
}

/**
 * @name beforeRenderAppMenu
 * @description this event is called Before render application menu to disable menu item from Client 'if app logic require that'
 * 
 * @param {Object} menuObj
 * @returns {Object} updatedMenuObject
 * 
 * This event Must return updated menu object or if no update happen then return original menu object as in code
 */
export async function beforeRenderAppMenu(menuObj) {   
  let disabledTags = [];  // array of menu items you would like to disable,you can name it as you want.

  // TODO : add any menu item tag you want to disable to disabledTags array 'based on your application logic'
  // ex : if I want to disable menu item with tag tag_1 then I will write this code
  // if(disable_your_menu_item_condition) {
  //  disabledTags.push("tag_1");
  // }

  // return menu after update 'This code should not removed'
  let updatedMenu = {};
  updatedMenu.children = menuObj;
  for(let disabledTag of disabledTags) {
      applayDisabledTags(updatedMenu, disabledTag);
  }
  return updatedMenu.children;
}

/**
 * @name beforeRenderCustomActions
 * @description This event is called before render custom buttons\actions
 *              if you want to disable custom button based on your application logic then you will do it here
 * @param {object} userObj Please ignore this for now
 * @param {string} tag screen/tab tag   
 * @param {Array<object>} actionsList 
 * example of actionsList array
 * [{
 *    EnglishButtonCaption:'',
 *    ButtonID:'',
 *    ArabicButtonsCaption:''
 * },...]
 * 
 * This event MUST return actionsList before exit
 */
export async function beforeRenderCustomActions(userObj, tag, actionsList) {    
  let disabledCustomActionIDs = [];
  tag = tag.toString().toUpperCase();

  // TODO : add any custom action you want to disable to disabledCustomActionIDs array 'based on your application logic'
  // ex : if I want to disable action with ID:action_1 then I will write this code
  // if(disable_your_ACTION_condition) {
  //  disabledCustomActionIDs.push("action_1");
  // }
  
  if(disabledCustomActionIDs.length > 0) {
    for(let actionBtn of actionsList) {
      if(disabledCustomActionIDs.includes(actionBtn.ButtonID)) {
        actionBtn.disabled = true;
      }
    }
  }
  return actionsList;
}

/**
 * Get employee department for the logged-in user
 * C++ equivalent: getEmpDep(GetUserLogin(), true)
 * @param {string} userName - The user login name
 * @returns {Promise<string>} - The employee department code
 */
async function getEmployeeDepartment(userName) {
  try {
    const { executeSQLPromise, getValFromRecordSet } = devInterfaceObj;
    if (!executeSQLPromise || !getValFromRecordSet) {
      console.warn('[Web_HSE] Missing devInterface functions for getEmployeeDepartment');
      return '';
    }

    // C++: SELECT EMPLOYEE_WSHOP FROM CMN_EMPLOYEE WHERE EMPLOYEE_USRID = userName
    // Note: C++ uses EMPLOYEE_USRID, but web might use EMPLOYEE_LOGINNAME
    const sql = `SELECT TOP 1 EMPLOYEE_WSHOP FROM CMN_EMPLOYEE WHERE (EMPLOYEE_USRID = '${userName.replace(/'/g, "''")}' OR EMPLOYEE_LOGINNAME = '${userName.replace(/'/g, "''")}')`;
    const result = await executeSQLPromise(sql);
    const department = getValFromRecordSet(result, 'EMPLOYEE_WSHOP');
    return department || '';
  } catch (error) {
    console.error('[Web_HSE] Error getting employee department:', error);
    return '';
  }
}

/**
 * @name onMenuItemClicked
 * @description This event is called when user click on Menu Item,Is used for set new screen criteria 'based on your application logic'
 *              or disallow user from open screen with specific message -> strMsg
 * 
 * Implements dynamic criteria for CAR screens based on C++ logic:
 * - CAR Entry (HSE_TgCrEntry): If user dept != owner dept, filter by CRENTRY_DPR
 * - Actions Entry (HSE_TgActnsEntry): If user dept != owner dept, filter by CRENTRY_CNCDPR
 * 
 * @param {object} userObj
 * @param {string} strScrTag
 * @param {function} callBackFn
 * 
 * This event MUST call callBackFn with return value before exit
 * retVal object structure 
 * let retVal = {
 *   isAllowed: true,
 *   strScrCriteria: "",
 *   strMsg: "",
 * };
 */
export function onMenuItemClicked(userObj, strScrTag, callBackFn) {
  console.log('[Web_HSE] onMenuItemClicked called for:', strScrTag);
  
  let retVal = {
    isAllowed: true,
    strScrCriteria: "",
    strMsg: "",
  };

  try {
    const normalizedScrTag = (strScrTag || '').toString().toUpperCase();
    console.log('[Web_HSE] Normalized screen tag:', normalizedScrTag);
    
    // CAR Entry screen - add department filter if needed
    // C++: If user department != owner department, add WHERE CRENTRY_DPR='{userDepartment}'
    // Note: The header.json already has: WHERE (CONVERT(INT, CRENTRY_CRSTT) <3)
    // We need to merge our criteria with the existing WhereClause using AND
    if (normalizedScrTag === 'HSE_TGCRENTRY') {
      const userName = userObj?.LogIn || '';
      if (userName) {
        // Try to get department from userObj first (usually set during login)
        let userDepartment = userObj?.EMPLOYEE_WSHOP || '';
        
        // If department is in userObj, use it synchronously
        if (userDepartment) {
          retVal.strScrCriteria = `AND CRENTRY_DPR='${userDepartment.replace(/'/g, "''")}'`;
          console.log('[Web_HSE] CAR Entry: Added department filter:', userDepartment);
        }
        // If not in userObj, query for it asynchronously and update criteria later
        // For now, we'll allow the screen to open without filter, and it can be added later if needed
        // TODO: Consider implementing async department lookup if needed
      }
    }
    
    // Actions Entry screen - add department filter if needed
    // C++: If user department != owner department, add WHERE isnull(CRENTRY_CNCDPR,'')='{userDepartment}'
    // Note: The header.json already has: WHERE (CONVERT(INT, CRENTRY_CRSTT) IN(12,15))
    // We need to merge our criteria with the existing WhereClause using AND
    else if (normalizedScrTag === 'HSE_TGACTNSENTRY') {
      const userName = userObj?.LogIn || '';
      if (userName) {
        let userDepartment = userObj?.EMPLOYEE_WSHOP || '';
        
        // If department is in userObj, use it synchronously
        if (userDepartment) {
          retVal.strScrCriteria = `AND isnull(CRENTRY_CNCDPR,'')='${userDepartment.replace(/'/g, "''")}'`;
          console.log('[Web_HSE] Actions Entry: Added department filter:', userDepartment);
        }
        // If not in userObj, query for it asynchronously and update criteria later
        // For now, we'll allow the screen to open without filter, and it can be added later if needed
        // TODO: Consider implementing async department lookup if needed
      }
    }
    
    // For all other screens, allow them to open with empty criteria
    // The callback will be called synchronously below
    
  } catch (error) {
    console.error('[Web_HSE] Error in onMenuItemClicked:', error);
    // Continue with empty criteria if there's an error - ensure screens can still open
    retVal.isAllowed = true;
    retVal.strScrCriteria = "";
    retVal.strMsg = "";
  }
  
  // Always call the callback synchronously to ensure screens can open
  // This is critical - the callback must be called for all menu items
  try {
    if (typeof callBackFn === 'function') {
      console.log('[Web_HSE] Calling callback with retVal:', retVal);
      callBackFn(retVal);
      console.log('[Web_HSE] Callback called successfully for:', strScrTag);
    } else {
      console.warn('[Web_HSE] onMenuItemClicked: callBackFn is not a function');
    }
  } catch (callbackError) {
    console.error('[Web_HSE] Error calling callback in onMenuItemClicked:', callbackError);
  }
}

/**
 * @name onAppOpen
 * @description This event is called when user open an application
 *              you can prevent user from open app by calling callbackFn(false)
 *              of course you need to show message to him using toast 'the reason of prevention' before prevent 
 *              him for open the app
 * @param {object} userObj
 * @param {function} callbackFn
 * 
 * This event MUST call callbackFn before exit
 */
export async function onAppOpen(userObj) {
  return true;
}


