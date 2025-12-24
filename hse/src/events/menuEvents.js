import { applayDisabledTags } from '../utils/menuUtils';

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
 * @name onMenuItemClicked
 * @description This event is called when user click on Menu Item,Is used for set new screen criteria 'based on your application logic'
 *              or disallow user from open screen with specific message -> strMsg
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
  let retVal = {
    isAllowed: true,
    strScrCriteria: "",
    strMsg: "",
  };
  // TODO : Your application login here...
  callBackFn(retVal);
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


